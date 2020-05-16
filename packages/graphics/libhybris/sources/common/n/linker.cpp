/*
 * Copyright (C) 2008 The Android Open Source Project
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <android/api-level.h>
#include <errno.h>
#include <fcntl.h>
#include <inttypes.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/param.h>
#include <unistd.h>

#include <algorithm>
#include <new>
#include <string>
#include <unordered_map>
#include <sstream>
#include <vector>

#include "hybris_compat.h"

// Private C library headers.
#include "private/bionic_globals.h"
#include "private/bionic_tls.h"
#include "private/KernelArgumentBlock.h"
#include "private/ScopedPthreadMutexLocker.h"
#include "private/ScopeGuard.h"

#include "linker.h"
#include "linker_block_allocator.h"
#include "linker_gdb_support.h"
#include "linker_debug.h"
#include "linker_dlwarning.h"
#include "linker_sleb128.h"
#include "linker_phdr.h"
#include "linker_relocs.h"
#include "linker_reloc_iterators.h"
#include "linker_utils.h"
#ifdef ENABLE_NON_PIE_SUPPORT
#include "linker_non_pie.h"
#endif

#ifdef WANT_ARM_TRACING
#include "../wrappers.h"
#endif

// Stay compatible with newer glibc
#ifndef R_AARCH64_TLS_TPREL64
#define R_AARCH64_TLS_TPREL64 R_AARCH64_TLS_TPREL
#endif
#ifndef R_AARCH64_TLS_DTPREL32
#define R_AARCH64_TLS_DTPREL32 R_AARCH64_TLS_DTPREL
#endif

//#include "android-base/strings.h"
//#include "ziparchive/zip_archive.h"

extern void __libc_init_globals(KernelArgumentBlock&);
#ifdef DISABLED_FOR_HYBRIS_SUPPORT
extern void __libc_init_AT_SECURE(KernelArgumentBlock&);
#endif

#ifdef DISABLED_FOR_HYBRIS_SUPPORT
extern "C" void _start();
#endif

// Override macros to use C++ style casts.
#undef ELF_ST_TYPE
#define ELF_ST_TYPE(x) (static_cast<uint32_t>(x) & 0xf)

struct android_namespace_t {
 public:
  android_namespace_t() : name_(nullptr), is_isolated_(false) {}

  const char* get_name() const { return name_; }
  void set_name(const char* name) { name_ = name; }

  bool is_isolated() const { return is_isolated_; }
  void set_isolated(bool isolated) { is_isolated_ = isolated; }

  const std::vector<std::string>& get_ld_library_paths() const {
    return ld_library_paths_;
  }
  void set_ld_library_paths(std::vector<std::string>&& library_paths) {
    ld_library_paths_ = library_paths;
  }

  const std::vector<std::string>& get_default_library_paths() const {
    return default_library_paths_;
  }
  void set_default_library_paths(std::vector<std::string>&& library_paths) {
    default_library_paths_ = library_paths;
  }

  const std::vector<std::string>& get_permitted_paths() const {
    return permitted_paths_;
  }
  void set_permitted_paths(std::vector<std::string>&& permitted_paths) {
    permitted_paths_ = permitted_paths;
  }

  void add_soinfo(soinfo* si) {
    soinfo_list_.push_back(si);
  }

  void add_soinfos(const soinfo::soinfo_list_t& soinfos) {
    for (auto si : soinfos) {
      add_soinfo(si);
      si->add_secondary_namespace(this);
    }
  }

  void remove_soinfo(soinfo* si) {
    soinfo_list_.remove_if([&](soinfo* candidate) {
      return si == candidate;
    });
  }

  const soinfo::soinfo_list_t& soinfo_list() const { return soinfo_list_; }

  // For isolated namespaces - checks if the file is on the search path;
  // always returns true for not isolated namespace.
  bool is_accessible(const std::string& path);

 private:
  const char* name_;
  bool is_isolated_;
  std::vector<std::string> ld_library_paths_;
  std::vector<std::string> default_library_paths_;
  std::vector<std::string> permitted_paths_;
  soinfo::soinfo_list_t soinfo_list_;

  DISALLOW_COPY_AND_ASSIGN(android_namespace_t);
};

static LinkerTypeAllocator<android_namespace_t> g_namespace_allocator;
static LinkerTypeAllocator<LinkedListEntry<android_namespace_t>> g_namespace_list_allocator;

android_namespace_t *g_default_namespace = new (g_namespace_allocator.alloc()) android_namespace_t();

static std::unordered_map<uintptr_t, soinfo*> g_soinfo_handles_map;
static android_namespace_t* g_anonymous_namespace = g_default_namespace;

static ElfW(Addr) get_elf_exec_load_bias(const ElfW(Ehdr)* elf);

static LinkerTypeAllocator<soinfo> g_soinfo_allocator;
static LinkerTypeAllocator<LinkedListEntry<soinfo>> g_soinfo_links_allocator;

static soinfo* solist = get_libdl_info();
static soinfo* sonext = get_libdl_info();
static soinfo* somain; // main process, always the one after libdl_info

#if defined(__LP64__)
static const char* const kSystemLibDir     = "/system/lib64";
static const char* const kVendorLibDir     = "/vendor/lib64";
static const char* const kVendorLibEglDir  = "/vendor/lib64/egl";
static const char* const kOdmLibDir        = "/odm/lib64";
static const char* const kOdmLibEglDir     = "/odm/lib64/egl";
static const char* const kAsanSystemLibDir = "/data/lib64";
static const char* const kAsanVendorLibDir = "/data/vendor/lib64";
static const char* const kAsanVendorLibEglDir = "/data/vendor/lib64/egl";
static const char* const kAsanOdmLibDir    = "/data/odm/lib64";
static const char* const kAsanOdmLibEglDir = "/data/odm/lib64/egl";
#else
static const char* const kSystemLibDir     = "/system/lib";
static const char* const kVendorLibDir     = "/vendor/lib";
static const char* const kVendorLibEglDir  = "/vendor/lib/egl";
static const char* const kOdmLibDir        = "/odm/lib";
static const char* const kOdmLibEglDir     = "/odm/lib/egl";
static const char* const kAsanSystemLibDir = "/data/lib";
static const char* const kAsanVendorLibDir = "/data/vendor/lib";
static const char* const kAsanVendorLibEglDir = "/data/vendor/lib/egl";
static const char* const kAsanOdmLibDir    = "/data/odm/lib";
static const char* const kAsanOdmLibEglDir = "/data/odm/lib/egl";
#endif

static const char* const kDefaultLdPaths[] = {
  kSystemLibDir,
  kOdmLibDir,
  kVendorLibDir,

  // libhybris support:
  kVendorLibEglDir,
  kOdmLibEglDir,
  nullptr
};

static const char* const kAsanDefaultLdPaths[] = {
  kAsanSystemLibDir,
  kSystemLibDir,
  kAsanOdmLibDir,
  kOdmLibDir,
  kAsanVendorLibDir,
  kVendorLibDir,

  // libhybris support:
  kAsanOdmLibEglDir,
  kOdmLibEglDir,
  kAsanVendorLibEglDir,
  kVendorLibEglDir,
  nullptr
};

std::vector<std::string> split(const std::string &text, const std::string &sep) {
  std::vector<std::string> tokens;
  std::size_t start = 0, end = 0;
  while ((end = text.find(sep, start)) != std::string::npos) {
    tokens.push_back(text.substr(start, end - start));
    start = end + 1;
  }
  tokens.push_back(text.substr(start));
  return tokens;
}

std::string join(const std::vector<std::string>& things, char separator) {
  if (things.empty()) {
    return "";
  }
  std::ostringstream result;
  result << *things.begin();
  for (auto it = std::next(things.begin()); it != things.end(); ++it) {
    result << separator << *it;
  }
  return result.str();
}

// Is ASAN enabled?
static bool g_is_asan = false;

static bool is_system_library(const std::string& realpath) {
  for (const auto& dir : g_default_namespace->get_default_library_paths()) {
    if (file_is_in_dir(realpath, dir)) {
      return true;
    }
  }
  return false;
}

// Checks if the file exists and not a directory.
static bool file_exists(const char* path) {
  int fd = TEMP_FAILURE_RETRY(open(path, O_RDONLY | O_CLOEXEC));
  if (fd == -1) {
    return false;
  } else {
    close(fd);
    return true;
  }
}
static std::string dirname(const char *path);

// TODO(dimitry): The grey-list is a workaround for http://b/26394120 ---
// gradually remove libraries from this list until it is gone.
static bool is_greylisted(const char* name, const soinfo* needed_by) {
  static const char* const kLibraryGreyList[] = {
    "libandroid_runtime.so",
    "libbinder.so",
    "libcrypto.so",
    "libcutils.so",
    "libexpat.so",
    "libgui.so",
    "libmedia.so",
    "libnativehelper.so",
    "libskia.so",
    "libssl.so",
    "libstagefright.so",
    "libsqlite.so",
    "libui.so",
    "libutils.so",
    "libvorbisidec.so",
    nullptr
  };

  // limit greylisting to apps targeting sdk version 23 and below
  if (get_application_target_sdk_version() > 23) {
    return false;
  }

  // if the library needed by a system library - implicitly assume it
  // is greylisted

  if (needed_by != nullptr && is_system_library(needed_by->get_realpath())) {
    return true;
  }

  // if this is an absolute path - make sure it points to /system/lib(64)
  if (name[0] == '/' && dirname(name) == kSystemLibDir) {
    // and reduce the path to basename
    name = basename(name);
  }

  for (size_t i = 0; kLibraryGreyList[i] != nullptr; ++i) {
    if (strcmp(name, kLibraryGreyList[i]) == 0) {
      return true;
    }
  }

  return false;
}
// END OF WORKAROUND

static const ElfW(Versym) kVersymNotNeeded = 0;
static const ElfW(Versym) kVersymGlobal = 1;

static const char* const* g_default_ld_paths;
static std::vector<std::string> g_ld_preload_names;

static std::vector<soinfo*> g_ld_preloads;

static bool g_public_namespace_initialized;
static soinfo::soinfo_list_t g_public_namespace;

int g_ld_debug_verbosity;

abort_msg_t* g_abort_message = nullptr; // For debuggerd.

static std::string dirname(const char *path) {
  const char* last_slash = strrchr(path, '/');
  if (last_slash == path) return "/";
  else if (last_slash == nullptr) return ".";
  else
    return std::string(path, last_slash - path);
}

#if STATS
struct linker_stats_t {
  int count[kRelocMax];
};

static linker_stats_t linker_stats;

void count_relocation(RelocationKind kind) {
  ++linker_stats.count[kind];
}
#else
void count_relocation(RelocationKind) {
}
#endif

#if COUNT_PAGES
uint32_t bitmask[4096];
#endif

static void* (*_get_hooked_symbol)(const char *sym, const char *requester);

#ifdef WANT_ARM_TRACING
void *(*_create_wrapper)(const char *symbol, void *function, int wrapper_type);
#endif

static char __linker_dl_err_buf[768];

char* linker_get_error_buffer() {
  return &__linker_dl_err_buf[0];
}

size_t linker_get_error_buffer_size() {
  return sizeof(__linker_dl_err_buf);
}

static void notify_gdb_of_load(soinfo* info) {
  if (info->is_linker() || info->is_main_executable()) {
    // gdb already knows about the linker and the main executable.
    return;
  }

  link_map* map = &(info->link_map_head);

  map->l_addr = info->load_bias;
  // link_map l_name field is not const.
  map->l_name = const_cast<char*>(info->get_realpath());
  map->l_ld = info->dynamic;

  CHECK(map->l_name != nullptr);
  CHECK(map->l_name[0] != '\0');

  notify_gdb_of_load(map);
}

static void notify_gdb_of_unload(soinfo* info) {
  notify_gdb_of_unload(&(info->link_map_head));
}

bool android_namespace_t::is_accessible(const std::string& file) {
  if (!is_isolated_) {
    return true;
  }

  for (const auto& dir : ld_library_paths_) {
    if (file_is_in_dir(file, dir)) {
      return true;
    }
  }

  for (const auto& dir : default_library_paths_) {
    if (file_is_in_dir(file, dir)) {
      return true;
    }
  }

  for (const auto& dir : permitted_paths_) {
    if (file_is_under_dir(file, dir)) {
      return true;
    }
  }

  return false;
}

LinkedListEntry<soinfo>* SoinfoListAllocator::alloc() {
  return g_soinfo_links_allocator.alloc();
}

void SoinfoListAllocator::free(LinkedListEntry<soinfo>* entry) {
  g_soinfo_links_allocator.free(entry);
}

LinkedListEntry<android_namespace_t>* NamespaceListAllocator::alloc() {
  return g_namespace_list_allocator.alloc();
}

void NamespaceListAllocator::free(LinkedListEntry<android_namespace_t>* entry) {
  g_namespace_list_allocator.free(entry);
}

static soinfo* soinfo_alloc(android_namespace_t* ns, const char* name,
                            struct stat* file_stat, off64_t file_offset,
                            uint32_t rtld_flags) {
  if (strlen(name) >= PATH_MAX) {
    DL_ERR("library name \"%s\" too long", name);
    return nullptr;
  }

  soinfo* si = new (g_soinfo_allocator.alloc()) soinfo(ns, name, file_stat,
                                                       file_offset, rtld_flags);

  sonext->next = si;
  sonext = si;

  si->generate_handle();
  ns->add_soinfo(si);

  TRACE("name %s: allocated soinfo @ %p", name, si);
  return si;
}

static void soinfo_free(soinfo* si) {
  if (si == nullptr) {
    return;
  }

  if (si->base != 0 && si->size != 0) {
    if (!si->is_mapped_by_caller()) {
      munmap(reinterpret_cast<void*>(si->base), si->size);
    } else {
      // remap the region as PROT_NONE, MAP_ANONYMOUS | MAP_NORESERVE
      mmap(reinterpret_cast<void*>(si->base), si->size, PROT_NONE,
           MAP_FIXED | MAP_PRIVATE | MAP_ANONYMOUS | MAP_NORESERVE, -1, 0);
    }
  }

  soinfo *prev = nullptr, *trav;

  TRACE("name %s: freeing soinfo @ %p", si->get_realpath(), si);

  for (trav = solist; trav != nullptr; trav = trav->next) {
    if (trav == si) {
      break;
    }
    prev = trav;
  }

  if (trav == nullptr) {
    // si was not in solist
    DL_ERR("name \"%s\"@%p is not in solist!", si->get_realpath(), si);
    return;
  }

  // clear links to/from si
  si->remove_all_links();

  // prev will never be null, because the first entry in solist is
  // always the static libdl_info.
  prev->next = si->next;
  if (si == sonext) {
    sonext = prev;
  }

  si->~soinfo();
  g_soinfo_allocator.free(si);
}

// For every path element this function checks of it exists, and is a directory,
// and normalizes it:
// 1. For regular path it converts it to realpath()
// 2. For path in a zip file it uses realpath on the zipfile
//    normalizes entry name by calling normalize_path function.
static void resolve_paths(std::vector<std::string>& paths,
                          std::vector<std::string>* resolved_paths) {
  resolved_paths->clear();
  for (const auto& path : paths) {
    char resolved_path[PATH_MAX];
    const char* original_path = path.c_str();
    if (realpath(original_path, resolved_path) != nullptr) {
      struct stat s;
      if (stat(resolved_path, &s) == 0) {
        if (S_ISDIR(s.st_mode)) {
          resolved_paths->push_back(resolved_path);
        } else {
          DL_WARN("Warning: \"%s\" is not a directory (excluding from path)", resolved_path);
          continue;
        }
      } else {
        DL_WARN("Warning: cannot stat file \"%s\": %s", resolved_path, strerror(errno));
        continue;
      }
    } else {
      std::string zip_path;
      std::string entry_path;

      std::string normalized_path;

      if (!normalize_path(original_path, &normalized_path)) {
        DL_WARN("Warning: unable to normalize \"%s\"", original_path);
        continue;
      }

      if (parse_zip_path(normalized_path.c_str(), &zip_path, &entry_path)) {
        if (realpath(zip_path.c_str(), resolved_path) == nullptr) {
          DL_WARN("Warning: unable to resolve \"%s\": %s", zip_path.c_str(), strerror(errno));
          continue;
        }

        resolved_paths->push_back(std::string(resolved_path) + kZipFileSeparator + entry_path);
      }
    }
  }
}

static void split_path(const char* path, const char* delimiters,
                       std::vector<std::string>* paths) {
  if (path != nullptr && path[0] != 0) {
    *paths = split(path, delimiters);
  }
}

static void parse_path(const char* path, const char* delimiters,
                       std::vector<std::string>* resolved_paths) {
  std::vector<std::string> paths;
  split_path(path, delimiters, &paths);
  resolve_paths(paths, resolved_paths);
}

static void parse_LD_LIBRARY_PATH(const char* path) {
  std::vector<std::string> ld_libary_paths;
  parse_path(path, ":", &ld_libary_paths);
  g_default_namespace->set_ld_library_paths(std::move(ld_libary_paths));
}

void soinfo::set_dt_runpath(const char* path) {
  if (!has_min_version(3)) {
    return;
  }

  std::vector<std::string> runpaths;

  split_path(path, ":", &runpaths);

  std::string origin = dirname(get_realpath());
  // FIXME: add $LIB and $PLATFORM.
  std::pair<std::string, std::string> substs[] = {{"ORIGIN", origin}};
  for (auto&& s : runpaths) {
    size_t pos = 0;
    while (pos < s.size()) {
      pos = s.find("$", pos);
      if (pos == std::string::npos) break;
      for (const auto& subst : substs) {
        const std::string& token = subst.first;
        const std::string& replacement = subst.second;
        if (s.substr(pos + 1, token.size()) == token) {
          s.replace(pos, token.size() + 1, replacement);
          // -1 to compensate for the ++pos below.
          pos += replacement.size() - 1;
          break;
        } else if (s.substr(pos + 1, token.size() + 2) == "{" + token + "}") {
          s.replace(pos, token.size() + 3, replacement);
          pos += replacement.size() - 1;
          break;
        }
      }
      // Skip $ in case it did not match any of the known substitutions.
      ++pos;
    }
  }

  resolve_paths(runpaths, &dt_runpath_);
}

static void parse_LD_PRELOAD(const char* path) {
  g_ld_preload_names.clear();
  if (path != nullptr) {
    // We have historically supported ':' as well as ' ' in LD_PRELOAD.
    g_ld_preload_names = split(path, " :");
    std::remove_if(g_ld_preload_names.begin(),
                   g_ld_preload_names.end(),
                   [] (const std::string& s) { return s.empty(); });
  }
}

static bool realpath_fd(int fd, std::string* realpath) {
  std::vector<char> buf(PATH_MAX), proc_self_fd(PATH_MAX);
  snprintf(&proc_self_fd[0], proc_self_fd.size(), "/proc/self/fd/%d", fd);
  if (readlink(&proc_self_fd[0], &buf[0], buf.size()) == -1) {
    PRINT("readlink(\"%s\") failed: %s [fd=%d]", &proc_self_fd[0], strerror(errno), fd);
    return false;
  }

  *realpath = &buf[0];
  return true;
}

#if defined(__arm__)

// For a given PC, find the .so that it belongs to.
// Returns the base address of the .ARM.exidx section
// for that .so, and the number of 8-byte entries
// in that section (via *pcount).
//
// Intended to be called by libc's __gnu_Unwind_Find_exidx().
//
// This function is exposed via dlfcn.cpp and libdl.so.
_Unwind_Ptr dl_unwind_find_exidx(_Unwind_Ptr pc, int* pcount) {
  uintptr_t addr = reinterpret_cast<uintptr_t>(pc);

  for (soinfo* si = solist; si != 0; si = si->next) {
    if ((addr >= si->base) && (addr < (si->base + si->size))) {
        *pcount = si->ARM_exidx_count;
        return reinterpret_cast<_Unwind_Ptr>(si->ARM_exidx);
    }
  }
  *pcount = 0;
  return nullptr;
}

#endif

// Here, we only have to provide a callback to iterate across all the
// loaded libraries. gcc_eh does the rest.
int do_dl_iterate_phdr(int (*cb)(dl_phdr_info* info, size_t size, void* data), void* data) {
  int rv = 0;
  for (soinfo* si = solist; si != nullptr; si = si->next) {
    dl_phdr_info dl_info;
    dl_info.dlpi_addr = si->link_map_head.l_addr;
    dl_info.dlpi_name = si->link_map_head.l_name;
    dl_info.dlpi_phdr = si->phdr;
    dl_info.dlpi_phnum = si->phnum;
    rv = cb(&dl_info, sizeof(dl_phdr_info), data);
    if (rv != 0) {
      break;
    }
  }
  return rv;
}

const ElfW(Versym)* soinfo::get_versym(size_t n) const {
  if (has_min_version(2) && versym_ != nullptr) {
    return versym_ + n;
  }

  return nullptr;
}

ElfW(Addr) soinfo::get_verneed_ptr() const {
  if (has_min_version(2)) {
    return verneed_ptr_;
  }

  return 0;
}

size_t soinfo::get_verneed_cnt() const {
  if (has_min_version(2)) {
    return verneed_cnt_;
  }

  return 0;
}

ElfW(Addr) soinfo::get_verdef_ptr() const {
  if (has_min_version(2)) {
    return verdef_ptr_;
  }

  return 0;
}

size_t soinfo::get_verdef_cnt() const {
  if (has_min_version(2)) {
    return verdef_cnt_;
  }

  return 0;
}

template<typename F>
static bool for_each_verdef(const soinfo* si, F functor) {
  if (!si->has_min_version(2)) {
    return true;
  }

  uintptr_t verdef_ptr = si->get_verdef_ptr();
  if (verdef_ptr == 0) {
    return true;
  }

  size_t offset = 0;

  size_t verdef_cnt = si->get_verdef_cnt();
  for (size_t i = 0; i<verdef_cnt; ++i) {
    const ElfW(Verdef)* verdef = reinterpret_cast<ElfW(Verdef)*>(verdef_ptr + offset);
    size_t verdaux_offset = offset + verdef->vd_aux;
    offset += verdef->vd_next;

    if (verdef->vd_version != 1) {
      DL_ERR("unsupported verdef[%zd] vd_version: %d (expected 1) library: %s",
          i, verdef->vd_version, si->get_realpath());
      return false;
    }

    if ((verdef->vd_flags & VER_FLG_BASE) != 0) {
      // "this is the version of the file itself.  It must not be used for
      //  matching a symbol. It can be used to match references."
      //
      // http://www.akkadia.org/drepper/symbol-versioning
      continue;
    }

    if (verdef->vd_cnt == 0) {
      DL_ERR("invalid verdef[%zd] vd_cnt == 0 (version without a name)", i);
      return false;
    }

    const ElfW(Verdaux)* verdaux = reinterpret_cast<ElfW(Verdaux)*>(verdef_ptr + verdaux_offset);

    if (functor(i, verdef, verdaux) == true) {
      break;
    }
  }

  return true;
}

bool soinfo::find_verdef_version_index(const version_info* vi, ElfW(Versym)* versym) const {
  if (vi == nullptr) {
    *versym = kVersymNotNeeded;
    return true;
  }

  *versym = kVersymGlobal;

  return for_each_verdef(this,
    [&](size_t, const ElfW(Verdef)* verdef, const ElfW(Verdaux)* verdaux) {
      if (verdef->vd_hash == vi->elf_hash &&
          strcmp(vi->name, get_string(verdaux->vda_name)) == 0) {
        *versym = verdef->vd_ndx;
        return true;
      }

      return false;
    }
  );
}

bool soinfo::find_symbol_by_name(SymbolName& symbol_name,
                                 const version_info* vi,
                                 const ElfW(Sym)** symbol) const {
  uint32_t symbol_index;
  bool success =
      is_gnu_hash() ?
      gnu_lookup(symbol_name, vi, &symbol_index) :
      elf_lookup(symbol_name, vi, &symbol_index);

  if (success) {
    *symbol = symbol_index == 0 ? nullptr : symtab_ + symbol_index;
  }

  return success;
}

static bool is_symbol_global_and_defined(const soinfo* si, const ElfW(Sym)* s) {
  if (ELF_ST_BIND(s->st_info) == STB_GLOBAL ||
      ELF_ST_BIND(s->st_info) == STB_WEAK) {
    return s->st_shndx != SHN_UNDEF;
  } else if (ELF_ST_BIND(s->st_info) != STB_LOCAL) {
    DL_WARN("unexpected ST_BIND value: %d for \"%s\" in \"%s\"",
            ELF_ST_BIND(s->st_info), si->get_string(s->st_name), si->get_realpath());
  }

  return false;
}

static const ElfW(Versym) kVersymHiddenBit = 0x8000;

static inline bool is_versym_hidden(const ElfW(Versym)* versym) {
  // the symbol is hidden if bit 15 of versym is set.
  return versym != nullptr && (*versym & kVersymHiddenBit) != 0;
}

static inline bool check_symbol_version(const ElfW(Versym) verneed,
                                        const ElfW(Versym)* verdef) {
  return verneed == kVersymNotNeeded ||
      verdef == nullptr ||
      verneed == (*verdef & ~kVersymHiddenBit);
}

bool soinfo::gnu_lookup(SymbolName& symbol_name,
                        const version_info* vi,
                        uint32_t* symbol_index) const {
  uint32_t hash = symbol_name.gnu_hash();
  uint32_t h2 = hash >> gnu_shift2_;

  uint32_t bloom_mask_bits = sizeof(ElfW(Addr))*8;
  uint32_t word_num = (hash / bloom_mask_bits) & gnu_maskwords_;
  ElfW(Addr) bloom_word = gnu_bloom_filter_[word_num];

  *symbol_index = 0;

  TRACE_TYPE(LOOKUP, "SEARCH %s in %s@%p (gnu)",
      symbol_name.get_name(), get_realpath(), reinterpret_cast<void*>(base));

  // test against bloom filter
  if ((1 & (bloom_word >> (hash % bloom_mask_bits)) & (bloom_word >> (h2 % bloom_mask_bits))) == 0) {
    TRACE_TYPE(LOOKUP, "NOT FOUND %s in %s@%p",
        symbol_name.get_name(), get_realpath(), reinterpret_cast<void*>(base));

    return true;
  }

  // bloom test says "probably yes"...
  uint32_t n = gnu_bucket_[hash % gnu_nbucket_];

  if (n == 0) {
    TRACE_TYPE(LOOKUP, "NOT FOUND %s in %s@%p",
        symbol_name.get_name(), get_realpath(), reinterpret_cast<void*>(base));

    return true;
  }

  // lookup versym for the version definition in this library
  // note the difference between "version is not requested" (vi == nullptr)
  // and "version not found". In the first case verneed is kVersymNotNeeded
  // which implies that the default version can be accepted; the second case results in
  // verneed = 1 (kVersymGlobal) and implies that we should ignore versioned symbols
  // for this library and consider only *global* ones.
  ElfW(Versym) verneed = 0;
  if (!find_verdef_version_index(vi, &verneed)) {
    return false;
  }

  do {
    ElfW(Sym)* s = symtab_ + n;
    const ElfW(Versym)* verdef = get_versym(n);
    // skip hidden versions when verneed == kVersymNotNeeded (0)
    if (verneed == kVersymNotNeeded && is_versym_hidden(verdef)) {
        continue;
    }
    if (((gnu_chain_[n] ^ hash) >> 1) == 0 &&
        check_symbol_version(verneed, verdef) &&
        strcmp(get_string(s->st_name), symbol_name.get_name()) == 0 &&
        is_symbol_global_and_defined(this, s)) {
      TRACE_TYPE(LOOKUP, "FOUND %s in %s (%p) %zd",
          symbol_name.get_name(), get_realpath(), reinterpret_cast<void*>(s->st_value),
          static_cast<size_t>(s->st_size));
      *symbol_index = n;
      return true;
    }
  } while ((gnu_chain_[n++] & 1) == 0);

  TRACE_TYPE(LOOKUP, "NOT FOUND %s in %s@%p",
             symbol_name.get_name(), get_realpath(), reinterpret_cast<void*>(base));

  return true;
}

bool soinfo::elf_lookup(SymbolName& symbol_name,
                        const version_info* vi,
                        uint32_t* symbol_index) const {
  uint32_t hash = symbol_name.elf_hash();

  TRACE_TYPE(LOOKUP, "SEARCH %s in %s@%p h=%x(elf) %zd",
             symbol_name.get_name(), get_realpath(),
             reinterpret_cast<void*>(base), hash, hash % nbucket_);

  ElfW(Versym) verneed = 0;
  if (!find_verdef_version_index(vi, &verneed)) {
    return false;
  }

  for (uint32_t n = bucket_[hash % nbucket_]; n != 0; n = chain_[n]) {
    ElfW(Sym)* s = symtab_ + n;
    const ElfW(Versym)* verdef = get_versym(n);

    // skip hidden versions when verneed == 0
    if (verneed == kVersymNotNeeded && is_versym_hidden(verdef)) {
        continue;
    }

    if (check_symbol_version(verneed, verdef) &&
        strcmp(get_string(s->st_name), symbol_name.get_name()) == 0 &&
        is_symbol_global_and_defined(this, s)) {
      TRACE_TYPE(LOOKUP, "FOUND %s in %s (%p) %zd",
                 symbol_name.get_name(), get_realpath(),
                 reinterpret_cast<void*>(s->st_value),
                 static_cast<size_t>(s->st_size));
      *symbol_index = n;
      return true;
    }
  }

  TRACE_TYPE(LOOKUP, "NOT FOUND %s in %s@%p %x %zd",
             symbol_name.get_name(), get_realpath(),
             reinterpret_cast<void*>(base), hash, hash % nbucket_);

  *symbol_index = 0;
  return true;
}

soinfo::soinfo(android_namespace_t* ns, const char* realpath,
               const struct stat* file_stat, off64_t file_offset,
               int rtld_flags) {

  if (realpath != nullptr) {
    realpath_ = realpath;
  }

  flags_ = FLAG_NEW_SOINFO;
  version_ = SOINFO_VERSION;

  if (file_stat != nullptr) {
    this->st_dev_ = file_stat->st_dev;
    this->st_ino_ = file_stat->st_ino;
    this->file_offset_ = file_offset;
  }

  this->rtld_flags_ = rtld_flags;
  this->primary_namespace_ = ns;
}

soinfo::~soinfo() {
  g_soinfo_handles_map.erase(handle_);
}

static uint32_t calculate_elf_hash(const char* name) {
  const uint8_t* name_bytes = reinterpret_cast<const uint8_t*>(name);
  uint32_t h = 0, g;

  while (*name_bytes) {
    h = (h << 4) + *name_bytes++;
    g = h & 0xf0000000;
    h ^= g;
    h ^= g >> 24;
  }

  return h;
}

uint32_t SymbolName::elf_hash() {
  if (!has_elf_hash_) {
    elf_hash_ = calculate_elf_hash(name_);
    has_elf_hash_ = true;
  }

  return elf_hash_;
}

uint32_t SymbolName::gnu_hash() {
  if (!has_gnu_hash_) {
    uint32_t h = 5381;
    const uint8_t* name = reinterpret_cast<const uint8_t*>(name_);
    while (*name != 0) {
      h += (h << 5) + *name++; // h*33 + c = h + h * 32 + c = h + h << 5 + c
    }

    gnu_hash_ =  h;
    has_gnu_hash_ = true;
  }

  return gnu_hash_;
}

bool soinfo_do_lookup(soinfo* si_from, const char* name, const version_info* vi,
                      soinfo** si_found_in, const soinfo::soinfo_list_t& global_group,
                      const soinfo::soinfo_list_t& local_group, const ElfW(Sym)** symbol) {
  SymbolName symbol_name(name);
  const ElfW(Sym)* s = nullptr;

  /* "This element's presence in a shared object library alters the dynamic linker's
   * symbol resolution algorithm for references within the library. Instead of starting
   * a symbol search with the executable file, the dynamic linker starts from the shared
   * object itself. If the shared object fails to supply the referenced symbol, the
   * dynamic linker then searches the executable file and other shared objects as usual."
   *
   * http://www.sco.com/developers/gabi/2012-12-31/ch5.dynamic.html
   *
   * Note that this is unlikely since static linker avoids generating
   * relocations for -Bsymbolic linked dynamic executables.
   */
  if (si_from->has_DT_SYMBOLIC) {
    DEBUG("%s: looking up %s in local scope (DT_SYMBOLIC)", si_from->get_realpath(), name);
    if (!si_from->find_symbol_by_name(symbol_name, vi, &s)) {
      return false;
    }

    if (s != nullptr) {
      *si_found_in = si_from;
    }
  }

  // 1. Look for it in global_group
  if (s == nullptr) {
    bool error = false;
    global_group.visit([&](soinfo* global_si) {
      DEBUG("%s: looking up %s in %s (from global group)",
          si_from->get_realpath(), name, global_si->get_realpath());
      if (!global_si->find_symbol_by_name(symbol_name, vi, &s)) {
        error = true;
        return false;
      }

      if (s != nullptr) {
        *si_found_in = global_si;
        return false;
      }

      return true;
    });

    if (error) {
      return false;
    }
  }

  // 2. Look for it in the local group
  if (s == nullptr) {
    bool error = false;
    local_group.visit([&](soinfo* local_si) {
      if (local_si == si_from && si_from->has_DT_SYMBOLIC) {
        // we already did this - skip
        return true;
      }

      DEBUG("%s: looking up %s in %s (from local group)",
          si_from->get_realpath(), name, local_si->get_realpath());
      if (!local_si->find_symbol_by_name(symbol_name, vi, &s)) {
        error = true;
        return false;
      }

      if (s != nullptr) {
        *si_found_in = local_si;
        return false;
      }

      return true;
    });

    if (error) {
      return false;
    }
  }

  if (s != nullptr) {
    TRACE_TYPE(LOOKUP, "si %s sym %s s->st_value = %p, "
               "found in %s, base = %p, load bias = %p",
               si_from->get_realpath(), name, reinterpret_cast<void*>(s->st_value),
               (*si_found_in)->get_realpath(), reinterpret_cast<void*>((*si_found_in)->base),
               reinterpret_cast<void*>((*si_found_in)->load_bias));
  }

  *symbol = s;
  return true;
}

class ProtectedDataGuard {
 public:
  ProtectedDataGuard() {
    if (ref_count_++ == 0) {
      protect_data(PROT_READ | PROT_WRITE);
    }
  }

  ~ProtectedDataGuard() {
    if (ref_count_ == 0) { // overflow
      __libc_fatal("Too many nested calls to dlopen()");
    }

    if (--ref_count_ == 0) {
      protect_data(PROT_READ);
    }
  }
 private:
  void protect_data(int protection) {
    g_soinfo_allocator.protect_all(protection);
    g_soinfo_links_allocator.protect_all(protection);
    g_namespace_allocator.protect_all(protection);
    g_namespace_list_allocator.protect_all(protection);
  }

  static size_t ref_count_;
};

size_t ProtectedDataGuard::ref_count_ = 0;

// Each size has it's own allocator.
template<size_t size>
class SizeBasedAllocator {
 public:
  static void* alloc() {
    return allocator_.alloc();
  }

  static void free(void* ptr) {
    allocator_.free(ptr);
  }

 private:
  static LinkerBlockAllocator allocator_;
};

template<size_t size>
LinkerBlockAllocator SizeBasedAllocator<size>::allocator_(size);

template<typename T>
class TypeBasedAllocator {
 public:
  static T* alloc() {
    return reinterpret_cast<T*>(SizeBasedAllocator<sizeof(T)>::alloc());
  }

  static void free(T* ptr) {
    SizeBasedAllocator<sizeof(T)>::free(ptr);
  }
};

class LoadTask {
 public:
  struct deleter_t {
    void operator()(LoadTask* t) {
      t->~LoadTask();
      TypeBasedAllocator<LoadTask>::free(t);
    }
  };

  static deleter_t deleter;

  static LoadTask* create(const char* name, soinfo* needed_by,
                          std::unordered_map<const soinfo*, ElfReader>* readers_map) {
    LoadTask* ptr = TypeBasedAllocator<LoadTask>::alloc();
    return new (ptr) LoadTask(name, needed_by, readers_map);
  }

  const char* get_name() const {
    return name_;
  }

  soinfo* get_needed_by() const {
    return needed_by_;
  }

  soinfo* get_soinfo() const {
    return si_;
  }

  void set_soinfo(soinfo* si) {
    si_ = si;
  }

  off64_t get_file_offset() const {
    return file_offset_;
  }

  void set_file_offset(off64_t offset) {
    file_offset_ = offset;
  }

  int get_fd() const {
    return fd_;
  }

  void set_fd(int fd, bool assume_ownership) {
    fd_ = fd;
    close_fd_ = assume_ownership;
  }

  const android_dlextinfo* get_extinfo() const {
    return extinfo_;
  }

  void set_extinfo(const android_dlextinfo* extinfo) {
    extinfo_ = extinfo;
  }

  bool is_dt_needed() const {
    return is_dt_needed_;
  }

  void set_dt_needed(bool is_dt_needed) {
    is_dt_needed_ = is_dt_needed;
  }

  const ElfReader& get_elf_reader() const {
    CHECK(si_ != nullptr);
    return (*elf_readers_map_)[si_];
  }

  ElfReader& get_elf_reader() {
    CHECK(si_ != nullptr);
    return (*elf_readers_map_)[si_];
  }

  std::unordered_map<const soinfo*, ElfReader>* get_readers_map() {
    return elf_readers_map_;
  }

  bool read(const char* realpath, off64_t file_size) {
    ElfReader& elf_reader = get_elf_reader();
    return elf_reader.Read(realpath, fd_, file_offset_, file_size);
  }

  bool load() {
    ElfReader& elf_reader = get_elf_reader();
    if (!elf_reader.Load(extinfo_)) {
      return false;
    }

    si_->base = elf_reader.load_start();
    si_->size = elf_reader.load_size();
    si_->set_mapped_by_caller(elf_reader.is_mapped_by_caller());
    si_->load_bias = elf_reader.load_bias();
    si_->phnum = elf_reader.phdr_count();
    si_->phdr = elf_reader.loaded_phdr();

    return true;
  }

 private:
  LoadTask(const char* name, soinfo* needed_by,
           std::unordered_map<const soinfo*, ElfReader>* readers_map)
    : name_(name), needed_by_(needed_by), si_(nullptr),
      fd_(-1), close_fd_(false), file_offset_(0), elf_readers_map_(readers_map),
      is_dt_needed_(false) {}

  ~LoadTask() {
    if (fd_ != -1 && close_fd_) {
      close(fd_);
    }
  }

  const char* name_;
  soinfo* needed_by_;
  soinfo* si_;
  const android_dlextinfo* extinfo_;
  int fd_;
  bool close_fd_;
  off64_t file_offset_;
  std::unordered_map<const soinfo*, ElfReader>* elf_readers_map_;
  // TODO(dimitry): needed by workaround for http://b/26394120 (the grey-list)
  bool is_dt_needed_;
  // END OF WORKAROUND

  DISALLOW_IMPLICIT_CONSTRUCTORS(LoadTask);
};

LoadTask::deleter_t LoadTask::deleter;

template <typename T>
using linked_list_t = LinkedList<T, TypeBasedAllocator<LinkedListEntry<T>>>;

typedef linked_list_t<soinfo> SoinfoLinkedList;
typedef linked_list_t<const char> StringLinkedList;
typedef std::vector<LoadTask*> LoadTaskList;

static soinfo* find_library(android_namespace_t* ns,
                           const char* name, int rtld_flags,
                           const android_dlextinfo* extinfo,
                           soinfo* needed_by);

// g_ld_all_shim_libs maintains the references to memory as it used
// in the soinfo structures and in the g_active_shim_libs list.

typedef std::pair<std::string, std::string> ShimDescriptor;
static std::vector<ShimDescriptor> g_ld_all_shim_libs;

// g_active_shim_libs are all shim libs that are still eligible
// to be loaded.  We must remove a shim lib from the list before
// we load the library to avoid recursive loops (load shim libA
// for libB where libA also links against libB).

static linked_list_t<const ShimDescriptor> g_active_shim_libs;

static void reset_g_active_shim_libs(void) {
  g_active_shim_libs.clear();
  for (const auto& pair : g_ld_all_shim_libs) {
    g_active_shim_libs.push_back(&pair);
  }
}

static void parse_shim_libs(const char* path) {
  if (path != nullptr) {
    // We have historically supported ':' as well as ' ' in LD_SHIM_LIBS.
    for (const auto& pair : split(path, " :")) {
      size_t pos = pair.find('|');
      if (pos > 0 && pos < pair.length() - 1) {
        auto desc = std::pair<std::string, std::string>(pair.substr(0, pos), pair.substr(pos + 1));
        g_ld_all_shim_libs.push_back(desc);
      }
    }
  }
  reset_g_active_shim_libs();
}

static void parse_LD_SHIM_LIBS(const char* path) {
  g_ld_all_shim_libs.clear();
#ifdef FORCED_SHIM_LIBS
  parse_shim_libs(FORCED_SHIM_LIBS);
#endif
  parse_shim_libs(path);
}

template<typename F>
static void for_each_matching_shim(const char *const path, F action) {
  if (path == nullptr) return;
  INFO("Finding shim libs for \"%s\"\n", path);
  std::vector<const ShimDescriptor *> matched;

  g_active_shim_libs.for_each([&](const ShimDescriptor *a_pair) {
    if (a_pair->first == path) {
      matched.push_back(a_pair);
    }
  });

  g_active_shim_libs.remove_if([&](const ShimDescriptor *a_pair) {
    return a_pair->first == path;
  });

  for (const auto& one_pair : matched) {
    INFO("Injecting shim lib \"%s\" as needed for %s", one_pair->second.c_str(), path);
    action(one_pair->second.c_str());
  }
}

// This function walks down the tree of soinfo dependencies
// in breadth-first order and
//   * calls action(soinfo* si) for each node, and
//   * terminates walk if action returns false.
//
// walk_dependencies_tree returns false if walk was terminated
// by the action and true otherwise.
template<typename F>
static bool walk_dependencies_tree(soinfo* root_soinfos[], size_t root_soinfos_size, F action) {
  SoinfoLinkedList visit_list;
  SoinfoLinkedList visited;

  for (size_t i = 0; i < root_soinfos_size; ++i) {
    visit_list.push_back(root_soinfos[i]);
  }

  soinfo* si;
  while ((si = visit_list.pop_front()) != nullptr) {
    if (visited.contains(si)) {
      continue;
    }

    if (!action(si)) {
      return false;
    }

    visited.push_back(si);

    si->get_children().for_each([&](soinfo* child) {
      visit_list.push_back(child);
    });
  }

  return true;
}


static const ElfW(Sym)* dlsym_handle_lookup(soinfo* root, soinfo* skip_until,
                                            soinfo** found, SymbolName& symbol_name,
                                            const version_info* vi) {
  const ElfW(Sym)* result = nullptr;
  bool skip_lookup = skip_until != nullptr;

  walk_dependencies_tree(&root, 1, [&](soinfo* current_soinfo) {
    if (skip_lookup) {
      skip_lookup = current_soinfo != skip_until;
      return true;
    }

    if (!current_soinfo->find_symbol_by_name(symbol_name, vi, &result)) {
      result = nullptr;
      return false;
    }

    if (result != nullptr) {
      *found = current_soinfo;
      return false;
    }

    return true;
  });

  return result;
}

static const ElfW(Sym)* dlsym_linear_lookup(android_namespace_t* ns,
                                            const char* name,
                                            const version_info* vi,
                                            soinfo** found,
                                            soinfo* caller,
                                            void* handle);

// This is used by dlsym(3).  It performs symbol lookup only within the
// specified soinfo object and its dependencies in breadth first order.
static const ElfW(Sym)* dlsym_handle_lookup(soinfo* si, soinfo** found,
                                            const char* name, const version_info* vi) {
  // According to man dlopen(3) and posix docs in the case when si is handle
  // of the main executable we need to search not only in the executable and its
  // dependencies but also in all libraries loaded with RTLD_GLOBAL.
  //
  // Since RTLD_GLOBAL is always set for the main executable and all dt_needed shared
  // libraries and they are loaded in breath-first (correct) order we can just execute
  // dlsym(RTLD_DEFAULT, ...); instead of doing two stage lookup.
  if (si == somain) {
    return dlsym_linear_lookup(g_default_namespace, name, vi, found, nullptr, RTLD_DEFAULT);
  }

  SymbolName symbol_name(name);
  return dlsym_handle_lookup(si, nullptr, found, symbol_name, vi);
}

/* This is used by dlsym(3) to performs a global symbol lookup. If the
   start value is null (for RTLD_DEFAULT), the search starts at the
   beginning of the global solist. Otherwise the search starts at the
   specified soinfo (for RTLD_NEXT).
 */
static const ElfW(Sym)* dlsym_linear_lookup(android_namespace_t* ns,
                                            const char* name,
                                            const version_info* vi,
                                            soinfo** found,
                                            soinfo* caller,
                                            void* handle) {
  SymbolName symbol_name(name);

  auto& soinfo_list = ns->soinfo_list();
  auto start = soinfo_list.begin();

  if (handle == RTLD_NEXT) {
    if (caller == nullptr) {
      return nullptr;
    } else {
      auto it = soinfo_list.find(caller);
      CHECK (it != soinfo_list.end());
      start = ++it;
    }
  }

  const ElfW(Sym)* s = nullptr;
  for (auto it = start, end = soinfo_list.end(); it != end; ++it) {
    soinfo* si = *it;
    // Do not skip RTLD_LOCAL libraries in dlsym(RTLD_DEFAULT, ...)
    // if the library is opened by application with target api level <= 22
    // See http://b/21565766
    if ((si->get_rtld_flags() & RTLD_GLOBAL) == 0 && si->get_target_sdk_version() > 22) {
      continue;
    }

    if (!si->find_symbol_by_name(symbol_name, vi, &s)) {
      return nullptr;
    }

    if (s != nullptr) {
      *found = si;
      break;
    }
  }

  // If not found - use dlsym_handle_lookup for caller's
  // local_group unless it is part of the global group in which
  // case we already did it.
  if (s == nullptr && caller != nullptr &&
      (caller->get_rtld_flags() & RTLD_GLOBAL) == 0) {
    return dlsym_handle_lookup(caller->get_local_group_root(),
        (handle == RTLD_NEXT) ? caller : nullptr, found, symbol_name, vi);
  }

  if (s != nullptr) {
    TRACE_TYPE(LOOKUP, "%s s->st_value = %p, found->base = %p",
               name, reinterpret_cast<void*>(s->st_value), reinterpret_cast<void*>((*found)->base));
  }

  return s;
}

soinfo* find_containing_library(const void* p) {
  ElfW(Addr) address = reinterpret_cast<ElfW(Addr)>(p);
  for (soinfo* si = solist; si != nullptr; si = si->next) {
    if (address >= si->base && address - si->base < si->size) {
      return si;
    }
  }
  return nullptr;
}

ElfW(Sym)* soinfo::find_symbol_by_address(const void* addr) {
  return is_gnu_hash() ? gnu_addr_lookup(addr) : elf_addr_lookup(addr);
}

static bool symbol_matches_soaddr(const ElfW(Sym)* sym, ElfW(Addr) soaddr) {
  return sym->st_shndx != SHN_UNDEF &&
      soaddr >= sym->st_value &&
      soaddr < sym->st_value + sym->st_size;
}

ElfW(Sym)* soinfo::gnu_addr_lookup(const void* addr) {
  ElfW(Addr) soaddr = reinterpret_cast<ElfW(Addr)>(addr) - load_bias;

  for (size_t i = 0; i < gnu_nbucket_; ++i) {
    uint32_t n = gnu_bucket_[i];

    if (n == 0) {
      continue;
    }

    do {
      ElfW(Sym)* sym = symtab_ + n;
      if (symbol_matches_soaddr(sym, soaddr)) {
        return sym;
      }
    } while ((gnu_chain_[n++] & 1) == 0);
  }

  return nullptr;
}

ElfW(Sym)* soinfo::elf_addr_lookup(const void* addr) {
  ElfW(Addr) soaddr = reinterpret_cast<ElfW(Addr)>(addr) - load_bias;

  // Search the library's symbol table for any defined symbol which
  // contains this address.
  for (size_t i = 0; i < nchain_; ++i) {
    ElfW(Sym)* sym = symtab_ + i;
    if (symbol_matches_soaddr(sym, soaddr)) {
      return sym;
    }
  }

  return nullptr;
}

class ZipArchiveCache {
 public:
  ZipArchiveCache() {}
  ~ZipArchiveCache();

  //bool get_or_open(const char* zip_path, ZipArchiveHandle* handle);
 private:
  DISALLOW_COPY_AND_ASSIGN(ZipArchiveCache);

  //std::unordered_map<std::string, ZipArchiveHandle> cache_;
};

/*
bool ZipArchiveCache::get_or_open(const char* zip_path, ZipArchiveHandle* handle) {
  std::string key(zip_path);

  auto it = cache_.find(key);
  if (it != cache_.end()) {
    *handle = it->second;
    return true;
  }

  int fd = TEMP_FAILURE_RETRY(open(zip_path, O_RDONLY | O_CLOEXEC));
  if (fd == -1) {
    return false;
  }

  if (OpenArchiveFd(fd, "", handle) != 0) {
    // invalid zip-file (?)
    CloseArchive(handle);
    close(fd);
    return false;
  }

  cache_[key] = *handle;
  return true;
}
*/

ZipArchiveCache::~ZipArchiveCache() {
/*
  for (const auto& it : cache_) {
    CloseArchive(it.second);
  }
*/
}
/*
static int open_library_in_zipfile(ZipArchiveCache* zip_archive_cache,
                                   const char* const input_path,
                                   off64_t* file_offset, std::string* realpath) {
  std::string normalized_path;
  if (!normalize_path(input_path, &normalized_path)) {
    return -1;
  }

  const char* const path = normalized_path.c_str();
  TRACE("Trying zip file open from path \"%s\" -> normalized \"%s\"", input_path, path);

  // Treat an '!/' separator inside a path as the separator between the name
  // of the zip file on disk and the subdirectory to search within it.
  // For example, if path is "foo.zip!/bar/bas/x.so", then we search for
  // "bar/bas/x.so" within "foo.zip".
  const char* const separator = strstr(path, kZipFileSeparator);
  if (separator == nullptr) {
    return -1;
  }

  char buf[512];
  if (strlcpy(buf, path, sizeof(buf)) >= sizeof(buf)) {
    PRINT("Warning: ignoring very long library path: %s", path);
    return -1;
  }

  buf[separator - path] = '\0';

  const char* zip_path = buf;
  const char* file_path = &buf[separator - path + 2];
  int fd = TEMP_FAILURE_RETRY(open(zip_path, O_RDONLY | O_CLOEXEC));
  if (fd == -1) {
    return -1;
  }

  ZipArchiveHandle handle;
  if (!zip_archive_cache->get_or_open(zip_path, &handle)) {
    // invalid zip-file (?)
    close(fd);
    return -1;
  }

  ZipEntry entry;

  if (FindEntry(handle, ZipString(file_path), &entry) != 0) {
    // Entry was not found.
    close(fd);
    return -1;
  }

  // Check if it is properly stored
  if (entry.method != kCompressStored || (entry.offset % PAGE_SIZE) != 0) {
    close(fd);
    return -1;
  }

  *file_offset = entry.offset;

  if (realpath_fd(fd, realpath)) {
    *realpath += separator;
  } else {
    PRINT("warning: unable to get realpath for the library \"%s\". Will use given path.",
          normalized_path.c_str());
    *realpath = normalized_path;
  }

  return fd;
}
*/

static bool format_path(char* buf, size_t buf_size, const char* path, const char* name) {
  int n = snprintf(buf, buf_size, "%s/%s", path, name);
  if (n < 0 || n >= static_cast<int>(buf_size)) {
    PRINT("Warning: ignoring very long library path: %s/%s", path, name);
    return false;
  }

  return true;
}

static int open_library_on_paths(ZipArchiveCache* zip_archive_cache,
                                 const char* name, off64_t* file_offset,
                                 const std::vector<std::string>& paths,
                                 std::string* realpath) {
  for (const auto& path : paths) {
    char buf[512];
    if (!format_path(buf, sizeof(buf), path.c_str(), name)) {
      continue;
    }

    int fd = -1;
/*
    if (strstr(buf, kZipFileSeparator) != nullptr) {
      fd = open_library_in_zipfile(zip_archive_cache, buf, file_offset, realpath);
    }
*/
    if (fd == -1) {
      fd = TEMP_FAILURE_RETRY(open(buf, O_RDONLY | O_CLOEXEC));
      if (fd != -1) {
        *file_offset = 0;
        if (!realpath_fd(fd, realpath)) {
          PRINT("warning: unable to get realpath for the library \"%s\". Will use given path.", buf);
          *realpath = buf;
        }
      }
    }

    if (fd != -1) {
      return fd;
    }
  }

  return -1;
}

static int open_library(android_namespace_t* ns,
                        ZipArchiveCache* zip_archive_cache,
                        const char* name, soinfo *needed_by,
                        off64_t* file_offset, std::string* realpath) {
  TRACE("[ opening %s ]", name);

  // If the name contains a slash, we should attempt to open it directly and not search the paths.
  if (strchr(name, '/') != nullptr) {
    int fd = -1;
/*
    if (strstr(name, kZipFileSeparator) != nullptr) {
      fd = open_library_in_zipfile(zip_archive_cache, name, file_offset, realpath);
    }
*/
    if (fd == -1) {
      fd = TEMP_FAILURE_RETRY(open(name, O_RDONLY | O_CLOEXEC));
      if (fd != -1) {
        *file_offset = 0;
        if (!realpath_fd(fd, realpath)) {
          PRINT("warning: unable to get realpath for the library \"%s\". Will use given path.", name);
          *realpath = name;
        }
      }
    }

    return fd;
  }

  // Otherwise we try LD_LIBRARY_PATH first, and fall back to the default library path
  int fd = open_library_on_paths(zip_archive_cache, name, file_offset, ns->get_ld_library_paths(), realpath);
  if (fd == -1 && needed_by != nullptr) {
    fd = open_library_on_paths(zip_archive_cache, name, file_offset, needed_by->get_dt_runpath(), realpath);
    // Check if the library is accessible
    if (fd != -1 && !ns->is_accessible(*realpath)) {
      fd = -1;
    }
  }

  if (fd == -1) {
    fd = open_library_on_paths(zip_archive_cache, name, file_offset, ns->get_default_library_paths(), realpath);
  }

  // TODO(dimitry): workaround for http://b/26394120 (the grey-list)
  if (fd == -1 && ns != g_default_namespace && is_greylisted(name, needed_by)) {
    // try searching for it on default_namespace default_library_path
    fd = open_library_on_paths(zip_archive_cache, name, file_offset,
                               g_default_namespace->get_default_library_paths(), realpath);
  }
  // END OF WORKAROUND

  return fd;
}

static const char* fix_dt_needed(const char* dt_needed, const char* sopath) {
  (void) sopath;
#if !defined(__LP64__)
  // Work around incorrect DT_NEEDED entries for old apps: http://b/21364029
  if (get_application_target_sdk_version() <= 22) {
    const char* bname = basename(dt_needed);
    if (bname != dt_needed) {
      DL_WARN("library \"%s\" has invalid DT_NEEDED entry \"%s\"", sopath, dt_needed);
      add_dlwarning(sopath, "invalid DT_NEEDED entry",  dt_needed);
    }

    return bname;
  }
#endif
  return dt_needed;
}

template<typename F>
static void for_each_dt_needed(const soinfo* si, F action) {
  for_each_matching_shim(si->get_realpath(), action);
  for (const ElfW(Dyn)* d = si->dynamic; d->d_tag != DT_NULL; ++d) {
    if (d->d_tag == DT_NEEDED) {
      action(fix_dt_needed(si->get_string(d->d_un.d_val), si->get_realpath()));
    }
  }
}

template<typename F>
static void for_each_dt_needed(const ElfReader& elf_reader, F action) {
  for_each_matching_shim(elf_reader.name(), action);
  for (const ElfW(Dyn)* d = elf_reader.dynamic(); d->d_tag != DT_NULL; ++d) {
    if (d->d_tag == DT_NEEDED) {
      action(fix_dt_needed(elf_reader.get_string(d->d_un.d_val), elf_reader.name()));
    }
  }
}

static bool load_library(android_namespace_t* ns,
                         LoadTask* task,
                         LoadTaskList* load_tasks,
                         int rtld_flags,
                         const std::string& realpath) {
  off64_t file_offset = task->get_file_offset();
  const char* name = task->get_name();
  const android_dlextinfo* extinfo = task->get_extinfo();

  if ((file_offset % PAGE_SIZE) != 0) {
    DL_ERR("file offset for the library \"%s\" is not page-aligned: %" PRId64, name, file_offset);
    return false;
  }
  if (file_offset < 0) {
    DL_ERR("file offset for the library \"%s\" is negative: %" PRId64, name, file_offset);
    return false;
  }

  struct stat file_stat;
  if (TEMP_FAILURE_RETRY(fstat(task->get_fd(), &file_stat)) != 0) {
    DL_ERR("unable to stat file for the library \"%s\": %s", name, strerror(errno));
    return false;
  }
  if (file_offset >= file_stat.st_size) {
    DL_ERR("file offset for the library \"%s\" >= file size: %" PRId64 " >= %" PRId64,
        name, file_offset, file_stat.st_size);
    return false;
  }

  // Check for symlink and other situations where
  // file can have different names, unless ANDROID_DLEXT_FORCE_LOAD is set
  if (extinfo == nullptr || (extinfo->flags & ANDROID_DLEXT_FORCE_LOAD) == 0) {
    auto predicate = [&](soinfo* si) {
      return si->get_st_dev() != 0 &&
             si->get_st_ino() != 0 &&
             si->get_st_dev() == file_stat.st_dev &&
             si->get_st_ino() == file_stat.st_ino &&
             si->get_file_offset() == file_offset;
    };

    soinfo* si = ns->soinfo_list().find_if(predicate);

    // check public namespace
    if (si == nullptr) {
      si = g_public_namespace.find_if(predicate);
      if (si != nullptr) {
        ns->add_soinfo(si);
      }
    }

    if (si != nullptr) {
      TRACE("library \"%s\" is already loaded under different name/path \"%s\" - "
            "will return existing soinfo", name, si->get_realpath());
      task->set_soinfo(si);
      return true;
    }
  }

  if ((rtld_flags & RTLD_NOLOAD) != 0) {
    DL_ERR("library \"%s\" wasn't loaded and RTLD_NOLOAD prevented it", name);
    return false;
  }

  if (!ns->is_accessible(realpath)) {
    // TODO(dimitry): workaround for http://b/26394120 - the grey-list
    const soinfo* needed_by = task->is_dt_needed() ? task->get_needed_by() : nullptr;
    if (is_greylisted(name, needed_by)) {
      // print warning only if needed by non-system library
      if (needed_by == nullptr || !is_system_library(needed_by->get_realpath())) {
        const soinfo* needed_or_dlopened_by = task->get_needed_by();
        const char* sopath = needed_or_dlopened_by == nullptr ? "(unknown)" :
                                                      needed_or_dlopened_by->get_realpath();
        DL_WARN("library \"%s\" (\"%s\") needed or dlopened by \"%s\" is not accessible for the namespace \"%s\""
                " - the access is temporarily granted as a workaround for http://b/26394120, note that the access"
                " will be removed in future releases of Android.",
                name, realpath.c_str(), sopath, ns->get_name());
        add_dlwarning(sopath, "unauthorized access to",  name);
      }
    } else {
      // do not load libraries if they are not accessible for the specified namespace.
      const char* needed_or_dlopened_by = task->get_needed_by() == nullptr ?
                                          "(unknown)" :
                                          task->get_needed_by()->get_realpath();

      DL_ERR("library \"%s\" needed or dlopened by \"%s\" is not accessible for the namespace \"%s\"",
             name, needed_or_dlopened_by, ns->get_name());

      PRINT("library \"%s\" (\"%s\") needed or dlopened by \"%s\" is not accessible for the"
            " namespace: [name=\"%s\", ld_library_paths=\"%s\", default_library_paths=\"%s\","
            " permitted_paths=\"%s\"]",
            name, realpath.c_str(),
            needed_or_dlopened_by,
            ns->get_name(),
            join(ns->get_ld_library_paths(), ':').c_str(),
            join(ns->get_default_library_paths(), ':').c_str(),
            join(ns->get_permitted_paths(), ':').c_str());
      return false;
    }
  }

  soinfo* si = soinfo_alloc(ns, realpath.c_str(), &file_stat, file_offset, rtld_flags);
  if (si == nullptr) {
    return false;
  }

  task->set_soinfo(si);

  // Read the ELF header and some of the segments.
  if (!task->read(realpath.c_str(), file_stat.st_size)) {
    soinfo_free(si);
    task->set_soinfo(nullptr);
    return false;
  }

  // find and set DT_RUNPATH and dt_soname
  // Note that these field values are temporary and are
  // going to be overwritten on soinfo::prelink_image
  // with values from PT_LOAD segments.
  const ElfReader& elf_reader = task->get_elf_reader();
  for (const ElfW(Dyn)* d = elf_reader.dynamic(); d->d_tag != DT_NULL; ++d) {
    if (d->d_tag == DT_RUNPATH) {
      si->set_dt_runpath(elf_reader.get_string(d->d_un.d_val));
    }
    if (d->d_tag == DT_SONAME) {
      si->set_soname(elf_reader.get_string(d->d_un.d_val));
    }
  }

  for_each_dt_needed(task->get_elf_reader(), [&](const char* name) {
    load_tasks->push_back(LoadTask::create(name, si, task->get_readers_map()));
  });

  return true;
}

static bool load_library(android_namespace_t* ns,
                         LoadTask* task,
                         ZipArchiveCache* zip_archive_cache,
                         LoadTaskList* load_tasks,
                         int rtld_flags) {
  const char* name = task->get_name();
  soinfo* needed_by = task->get_needed_by();
  const android_dlextinfo* extinfo = task->get_extinfo();

  off64_t file_offset;
  std::string realpath;
  if (extinfo != nullptr && (extinfo->flags & ANDROID_DLEXT_USE_LIBRARY_FD) != 0) {
    file_offset = 0;
    if ((extinfo->flags & ANDROID_DLEXT_USE_LIBRARY_FD_OFFSET) != 0) {
      file_offset = extinfo->library_fd_offset;
    }

    if (!realpath_fd(extinfo->library_fd, &realpath)) {
      PRINT("warning: unable to get realpath for the library \"%s\" by extinfo->library_fd. "
            "Will use given name.", name);
      realpath = name;
    }

    task->set_fd(extinfo->library_fd, false);
    task->set_file_offset(file_offset);
    return load_library(ns, task, load_tasks, rtld_flags, realpath);
  }

  // Open the file.
  int fd = open_library(ns, zip_archive_cache, name, needed_by, &file_offset, &realpath);
  if (fd == -1) {
    DL_ERR("library \"%s\" not found", name);
    return false;
  }

  task->set_fd(fd, true);
  task->set_file_offset(file_offset);

  return load_library(ns, task, load_tasks, rtld_flags, realpath);
}

// Returns true if library was found and false in 2 cases
// 1. (for default namespace only) The library was found but loaded under different
//    target_sdk_version (*candidate != nullptr)
// 2. The library was not found by soname (*candidate is nullptr)
static bool find_loaded_library_by_soname(android_namespace_t* ns,
                                          const char* name, soinfo** candidate) {
  *candidate = nullptr;

  // Ignore filename with path.
  if (strchr(name, '/') != nullptr) {
    return false;
  }

  uint32_t target_sdk_version = get_application_target_sdk_version();

  return !ns->soinfo_list().visit([&](soinfo* si) {
    const char* soname = si->get_soname();
    if (soname != nullptr && (strcmp(name, soname) == 0)) {
      // If the library was opened under different target sdk version
      // skip this step and try to reopen it. The exceptions are
      // "libdl.so" and global group. There is no point in skipping
      // them because relocation process is going to use them
      // in any case.
      bool is_libdl = si == solist;
      if (is_libdl || (si->get_dt_flags_1() & DF_1_GLOBAL) != 0 ||
          !si->is_linked() || si->get_target_sdk_version() == target_sdk_version ||
          ns != g_default_namespace) {
        *candidate = si;
        return false;
      } else if (*candidate == nullptr) {
        // for the different sdk version in the default namespace
        // remember the first library.
        *candidate = si;
      }
    }

    return true;
  });
}

static bool find_library_internal(android_namespace_t* ns,
                                  LoadTask* task,
                                  ZipArchiveCache* zip_archive_cache,
                                  LoadTaskList* load_tasks,
                                  int rtld_flags) {
  soinfo* candidate;

  if (find_loaded_library_by_soname(ns, task->get_name(), &candidate)) {
    task->set_soinfo(candidate);
    return true;
  }

  if (ns != g_default_namespace) {
    // check public namespace
    candidate = g_public_namespace.find_if([&](soinfo* si) {
      return strcmp(task->get_name(), si->get_soname()) == 0;
    });

    if (candidate != nullptr) {
      ns->add_soinfo(candidate);
      task->set_soinfo(candidate);
      return true;
    }
  }

  // Library might still be loaded, the accurate detection
  // of this fact is done by load_library.
  TRACE("[ \"%s\" find_loaded_library_by_soname failed (*candidate=%s@%p). Trying harder...]",
      task->get_name(), candidate == nullptr ? "n/a" : candidate->get_realpath(), candidate);

  if (load_library(ns, task, zip_archive_cache, load_tasks, rtld_flags)) {
    return true;
  } else {
    // In case we were unable to load the library but there
    // is a candidate loaded under the same soname but different
    // sdk level - return it anyways.
    if (candidate != nullptr) {
      task->set_soinfo(candidate);
      return true;
    }
  }

  return false;
}

static void soinfo_unload(soinfo* si);
static void soinfo_unload(soinfo* soinfos[], size_t count);

// TODO: this is slightly unusual way to construct
// the global group for relocation. Not every RTLD_GLOBAL
// library is included in this group for backwards-compatibility
// reasons.
//
// This group consists of the main executable, LD_PRELOADs
// and libraries with the DF_1_GLOBAL flag set.
static soinfo::soinfo_list_t make_global_group(android_namespace_t* ns) {
  soinfo::soinfo_list_t global_group;
  ns->soinfo_list().for_each([&](soinfo* si) {
    if ((si->get_dt_flags_1() & DF_1_GLOBAL) != 0) {
      global_group.push_back(si);
    }
  });

  return global_group;
}

// This function provides a list of libraries to be shared
// by the namespace. For the default namespace this is the global
// group (see make_global_group). For all others this is a group
// of RTLD_GLOBAL libraries (which includes the global group from
// the default namespace).
static soinfo::soinfo_list_t get_shared_group(android_namespace_t* ns) {
  if (ns == g_default_namespace) {
    return make_global_group(ns);
  }

  soinfo::soinfo_list_t shared_group;
  ns->soinfo_list().for_each([&](soinfo* si) {
    if ((si->get_rtld_flags() & RTLD_GLOBAL) != 0) {
      shared_group.push_back(si);
    }
  });

  return shared_group;
}

/*
static void shuffle(std::vector<LoadTask*>* v) {
  for (size_t i = 0, size = v->size(); i < size; ++i) {
    size_t n = size - i;
    size_t r = arc4random_uniform(n);
    std::swap((*v)[n-1], (*v)[r]);
  }
}
*/

// add_as_children - add first-level loaded libraries (i.e. library_names[], but
// not their transitive dependencies) as children of the start_with library.
// This is false when find_libraries is called for dlopen(), when newly loaded
// libraries must form a disjoint tree.
static bool find_libraries(android_namespace_t* ns,
                           soinfo* start_with,
                           const char* const library_names[],
                           size_t library_names_count, soinfo* soinfos[],
                           std::vector<soinfo*>* ld_preloads,
                           size_t ld_preloads_count, int rtld_flags,
                           const android_dlextinfo* extinfo,
                           bool add_as_children) {
  // Step 0: prepare.
  LoadTaskList load_tasks;
  std::unordered_map<const soinfo*, ElfReader> readers_map;

  for (size_t i = 0; i < library_names_count; ++i) {
    const char* name = library_names[i];
    load_tasks.push_back(LoadTask::create(name, start_with, &readers_map));
  }

  // Construct global_group.
  soinfo::soinfo_list_t global_group = make_global_group(ns);

  // If soinfos array is null allocate one on stack.
  // The array is needed in case of failure; for example
  // when library_names[] = {libone.so, libtwo.so} and libone.so
  // is loaded correctly but libtwo.so failed for some reason.
  // In this case libone.so should be unloaded on return.
  // See also implementation of failure_guard below.

  if (soinfos == nullptr) {
    size_t soinfos_size = sizeof(soinfo*)*library_names_count;
    soinfos = reinterpret_cast<soinfo**>(alloca(soinfos_size));
    memset(soinfos, 0, soinfos_size);
  }

  // list of libraries to link - see step 2.
  size_t soinfos_count = 0;

  auto scope_guard = make_scope_guard([&]() {
    for (LoadTask* t : load_tasks) {
      LoadTask::deleter(t);
    }
  });

  auto failure_guard = make_scope_guard([&]() {
    // Housekeeping
    soinfo_unload(soinfos, soinfos_count);
  });

  ZipArchiveCache zip_archive_cache;

  // Step 1: expand the list of load_tasks to include
  // all DT_NEEDED libraries (do not load them just yet)
  for (size_t i = 0; i<load_tasks.size(); ++i) {
    LoadTask* task = load_tasks[i];
    soinfo* needed_by = task->get_needed_by();

    bool is_dt_needed = needed_by != nullptr && (needed_by != start_with || add_as_children);
    task->set_extinfo(is_dt_needed ? nullptr : extinfo);
    task->set_dt_needed(is_dt_needed);

    if(!find_library_internal(ns, task, &zip_archive_cache, &load_tasks, rtld_flags)) {
      return false;
    }

    soinfo* si = task->get_soinfo();

    if (is_dt_needed) {
      needed_by->add_child(si);
    }

    if (si->is_linked()) {
      si->increment_ref_count();
    }

    // When ld_preloads is not null, the first
    // ld_preloads_count libs are in fact ld_preloads.
    if (ld_preloads != nullptr && soinfos_count < ld_preloads_count) {
      ld_preloads->push_back(si);
    }

    if (soinfos_count < library_names_count) {
      soinfos[soinfos_count++] = si;
    }
  }

  // Step 2: Load libraries in random order (see b/24047022)
  LoadTaskList load_list;
  for (auto&& task : load_tasks) {
    soinfo* si = task->get_soinfo();
    auto pred = [&](const LoadTask* t) {
      return t->get_soinfo() == si;
    };

    if (!si->is_linked() &&
        std::find_if(load_list.begin(), load_list.end(), pred) == load_list.end() ) {
      load_list.push_back(task);
    }
  }
  //shuffle(&load_list);

  for (auto&& task : load_list) {
    if (!task->load()) {
      return false;
    }
  }

  // Step 3: pre-link all DT_NEEDED libraries in breadth first order.
  for (auto&& task : load_tasks) {
    soinfo* si = task->get_soinfo();
    if (!si->is_linked() && !si->prelink_image()) {
      return false;
    }
  }

  // Step 4: Add LD_PRELOADed libraries to the global group for
  // future runs. There is no need to explicitly add them to
  // the global group for this run because they are going to
  // appear in the local group in the correct order.
  if (ld_preloads != nullptr) {
    for (auto&& si : *ld_preloads) {
      si->set_dt_flags_1(si->get_dt_flags_1() | DF_1_GLOBAL);
    }
  }


  // Step 5: link libraries.
  soinfo::soinfo_list_t local_group;
  walk_dependencies_tree(
      (start_with != nullptr && add_as_children) ? &start_with : soinfos,
      (start_with != nullptr && add_as_children) ? 1 : soinfos_count,
      [&] (soinfo* si) {
    local_group.push_back(si);
    return true;
  });

  // We need to increment ref_count in case
  // the root of the local group was not linked.
  bool was_local_group_root_linked = local_group.front()->is_linked();

  bool linked = local_group.visit([&](soinfo* si) {
    if (!si->is_linked()) {
      if (!si->link_image(global_group, local_group, extinfo)) {
        return false;
      }
    }

    return true;
  });

  if (linked) {
    local_group.for_each([](soinfo* si) {
      if (!si->is_linked()) {
        si->set_linked();
      }
    });

    failure_guard.disable();
  }

  if (!was_local_group_root_linked) {
    local_group.front()->increment_ref_count();
  }

  return linked;
}

static soinfo* find_library(android_namespace_t* ns,
                            const char* name, int rtld_flags,
                            const android_dlextinfo* extinfo,
                            soinfo* needed_by) {
  soinfo* si;

  if (name == nullptr) {
    si = somain;
  } else if (!find_libraries(ns, needed_by, &name, 1, &si, nullptr, 0, rtld_flags,
                             extinfo, /* add_as_children */ false)) {
    return nullptr;
  }

  return si;
}

static void soinfo_unload(soinfo* root) {
  if (root->is_linked()) {
    root = root->get_local_group_root();
  }

  if (!root->can_unload()) {
    TRACE("not unloading \"%s\" - the binary is flagged with NODELETE", root->get_realpath());
    return;
  }

  soinfo_unload(&root, 1);
}

static void soinfo_unload(soinfo* soinfos[], size_t count) {
  // Note that the library can be loaded but not linked;
  // in which case there is no root but we still need
  // to walk the tree and unload soinfos involved.
  //
  // This happens on unsuccessful dlopen, when one of
  // the DT_NEEDED libraries could not be linked/found.
  if (count == 0) {
    return;
  }

  soinfo::soinfo_list_t unload_list;
  for (size_t i = 0; i < count; ++i) {
    soinfo* si = soinfos[i];

    if (si->can_unload()) {
      size_t ref_count = si->is_linked() ? si->decrement_ref_count() : 0;
      if (ref_count == 0) {
        unload_list.push_back(si);
      } else {
        TRACE("not unloading '%s' group, decrementing ref_count to %zd",
            si->get_realpath(), ref_count);
      }
    } else {
      TRACE("not unloading '%s' - the binary is flagged with NODELETE", si->get_realpath());
      return;
    }
  }

  // This is used to identify soinfos outside of the load-group
  // note that we cannot have > 1 in the array and have any of them
  // linked. This is why we can safely use the first one.
  soinfo* root = soinfos[0];

  soinfo::soinfo_list_t local_unload_list;
  soinfo::soinfo_list_t external_unload_list;
  soinfo* si = nullptr;

  while ((si = unload_list.pop_front()) != nullptr) {
    if (local_unload_list.contains(si)) {
      continue;
    }

    local_unload_list.push_back(si);

    if (si->has_min_version(0)) {
      soinfo* child = nullptr;
      while ((child = si->get_children().pop_front()) != nullptr) {
        TRACE("%s@%p needs to unload %s@%p", si->get_realpath(), si,
            child->get_realpath(), child);

        if (local_unload_list.contains(child)) {
          continue;
        } else if (child->is_linked() && child->get_local_group_root() != root) {
          external_unload_list.push_back(child);
        } else {
          unload_list.push_front(child);
        }
      }
    } else {
#if !defined(__work_around_b_24465209__)
      __libc_fatal("soinfo for \"%s\"@%p has no version", si->get_realpath(), si);
#else
      PRINT("warning: soinfo for \"%s\"@%p has no version", si->get_realpath(), si);
      for_each_dt_needed(si, [&] (const char* library_name) {
        TRACE("deprecated (old format of soinfo): %s needs to unload %s",
            si->get_realpath(), library_name);

        soinfo* needed = find_library(si->get_primary_namespace(),
                                      library_name, RTLD_NOLOAD, nullptr, nullptr);

        if (needed != nullptr) {
          // Not found: for example if symlink was deleted between dlopen and dlclose
          // Since we cannot really handle errors at this point - print and continue.
          PRINT("warning: couldn't find %s needed by %s on unload.",
              library_name, si->get_realpath());
          return;
        } else if (local_unload_list.contains(needed)) {
          // already visited
          return;
        } else if (needed->is_linked() && needed->get_local_group_root() != root) {
          // external group
          external_unload_list.push_back(needed);
        } else {
          // local group
          unload_list.push_front(needed);
        }
      });
#endif
    }
  }

  local_unload_list.for_each([](soinfo* si) {
    si->call_destructors();
  });

  while ((si = local_unload_list.pop_front()) != nullptr) {
    notify_gdb_of_unload(si);
    soinfo_free(si);
  }

  while ((si = external_unload_list.pop_front()) != nullptr) {
    soinfo_unload(si);
  }
}

static std::string symbol_display_name(const char* sym_name, const char* sym_ver) {
  if (sym_ver == nullptr) {
    return sym_name;
  }

  return std::string(sym_name) + ", version " + sym_ver;
}

static android_namespace_t* get_caller_namespace(soinfo* caller) {
  return caller != nullptr ? caller->get_primary_namespace() : g_anonymous_namespace;
}

void do_android_get_LD_LIBRARY_PATH(char* buffer, size_t buffer_size) {
  // Use basic string manipulation calls to avoid snprintf.
  // snprintf indirectly calls pthread_getspecific to get the size of a buffer.
  // When debug malloc is enabled, this call returns 0. This in turn causes
  // snprintf to do nothing, which causes libraries to fail to load.
  // See b/17302493 for further details.
  // Once the above bug is fixed, this code can be modified to use
  // snprintf again.
  size_t required_len = 0;
  for (size_t i = 0; g_default_ld_paths[i] != nullptr; ++i) {
    required_len += strlen(g_default_ld_paths[i]) + 1;
  }
  if (buffer_size < required_len) {
    __libc_fatal("android_get_LD_LIBRARY_PATH failed, buffer too small: "
                 "buffer len %zu, required len %zu", buffer_size, required_len);
  }
  char* end = buffer;
  for (size_t i = 0; g_default_ld_paths[i] != nullptr; ++i) {
    if (i > 0) *end++ = ':';
    end = stpcpy(end, g_default_ld_paths[i]);
  }
}

void do_android_update_LD_LIBRARY_PATH(const char* ld_library_path) {
  parse_LD_LIBRARY_PATH(ld_library_path);
}

void* do_dlopen(const char* name, int flags, const android_dlextinfo* extinfo,
                  void* caller_addr) {
  soinfo* const caller = find_containing_library(caller_addr);

  if ((flags & ~(RTLD_NOW|RTLD_LAZY|RTLD_LOCAL|RTLD_GLOBAL|RTLD_NODELETE|RTLD_NOLOAD)) != 0) {
    DL_ERR("invalid flags to dlopen: %x", flags);
    return nullptr;
  }

  android_namespace_t* ns = get_caller_namespace(caller);

  if (extinfo != nullptr) {
    if ((extinfo->flags & ~(ANDROID_DLEXT_VALID_FLAG_BITS)) != 0) {
      DL_ERR("invalid extended flags to android_dlopen_ext: 0x%" PRIx64, extinfo->flags);
      return nullptr;
    }

    if ((extinfo->flags & ANDROID_DLEXT_USE_LIBRARY_FD) == 0 &&
        (extinfo->flags & ANDROID_DLEXT_USE_LIBRARY_FD_OFFSET) != 0) {
      DL_ERR("invalid extended flag combination (ANDROID_DLEXT_USE_LIBRARY_FD_OFFSET without "
          "ANDROID_DLEXT_USE_LIBRARY_FD): 0x%" PRIx64, extinfo->flags);
      return nullptr;
    }

    if ((extinfo->flags & ANDROID_DLEXT_LOAD_AT_FIXED_ADDRESS) != 0 &&
        (extinfo->flags & (ANDROID_DLEXT_RESERVED_ADDRESS | ANDROID_DLEXT_RESERVED_ADDRESS_HINT)) != 0) {
      DL_ERR("invalid extended flag combination: ANDROID_DLEXT_LOAD_AT_FIXED_ADDRESS is not "
             "compatible with ANDROID_DLEXT_RESERVED_ADDRESS/ANDROID_DLEXT_RESERVED_ADDRESS_HINT");
      return nullptr;
    }

    if ((extinfo->flags & ANDROID_DLEXT_USE_NAMESPACE) != 0) {
      if (extinfo->library_namespace == nullptr) {
        DL_ERR("ANDROID_DLEXT_USE_NAMESPACE is set but extinfo->library_namespace is null");
        return nullptr;
      }
      ns = extinfo->library_namespace;
    }
  }

  std::string asan_name_holder;

  const char* translated_name = name;
  if (g_is_asan) {
    if (file_is_in_dir(name, kSystemLibDir)) {
      asan_name_holder = std::string(kAsanSystemLibDir) + "/" + basename(name);
      if (file_exists(asan_name_holder.c_str())) {
        translated_name = asan_name_holder.c_str();
        PRINT("linker_asan dlopen translating \"%s\" -> \"%s\"", name, translated_name);
      }
    } else if (file_is_in_dir(name, kVendorLibDir)) {
      asan_name_holder = std::string(kAsanVendorLibDir) + "/" + basename(name);
      if (file_exists(asan_name_holder.c_str())) {
        translated_name = asan_name_holder.c_str();
        PRINT("linker_asan dlopen translating \"%s\" -> \"%s\"", name, translated_name);
      }
    } else if (file_is_in_dir(name, kOdmLibDir)) {
      asan_name_holder = std::string(kAsanOdmLibDir) + "/" + basename(name);
      if (file_exists(asan_name_holder.c_str())) {
        translated_name = asan_name_holder.c_str();
        PRINT("linker_asan dlopen translating \"%s\" -> \"%s\"", name, translated_name);
      }
    }
  }

  ProtectedDataGuard guard;
  reset_g_active_shim_libs();
  soinfo* si = find_library(ns, translated_name, flags, extinfo, caller);
  if (si != nullptr) {
    si->call_constructors();
    return si->to_handle();
  }

  return nullptr;
}

int do_dladdr(const void* addr, Dl_info* info) {
  // Determine if this address can be found in any library currently mapped.
  soinfo* si = find_containing_library(addr);
  if (si == nullptr) {
    return 0;
  }

  memset(info, 0, sizeof(Dl_info));

  info->dli_fname = si->get_realpath();
  // Address at which the shared object is loaded.
  info->dli_fbase = reinterpret_cast<void*>(si->base);

  // Determine if any symbol in the library contains the specified address.
  ElfW(Sym)* sym = si->find_symbol_by_address(addr);
  if (sym != nullptr) {
    info->dli_sname = si->get_string(sym->st_name);
    info->dli_saddr = reinterpret_cast<void*>(si->resolve_symbol_address(sym));
  }

  return 1;
}

static soinfo* soinfo_from_handle(void* handle) {
  if ((reinterpret_cast<uintptr_t>(handle) & 1) != 0) {
    auto it = g_soinfo_handles_map.find(reinterpret_cast<uintptr_t>(handle));
    if (it == g_soinfo_handles_map.end()) {
      return nullptr;
    } else {
      return it->second;
    }
  }

  return static_cast<soinfo*>(handle);
}

bool do_dlsym(void* handle, const char* sym_name, const char* sym_ver,
              void* caller_addr, void** symbol) {
#if !defined(__LP64__)
  if (handle == nullptr) {
    DL_ERR("dlsym failed: library handle is null");
    return false;
  }
#endif

  if (sym_name == nullptr) {
    DL_ERR("dlsym failed: symbol name is null");
    return false;
  }

  soinfo* found = nullptr;
  const ElfW(Sym)* sym = nullptr;
  soinfo* caller = find_containing_library(caller_addr);
  android_namespace_t* ns = get_caller_namespace(caller);

  version_info vi_instance;
  version_info* vi = nullptr;

  if (sym_ver != nullptr) {
    vi_instance.name = sym_ver;
    vi_instance.elf_hash = calculate_elf_hash(sym_ver);
    vi = &vi_instance;
  }

  if (handle == RTLD_DEFAULT || handle == RTLD_NEXT) {
    sym = dlsym_linear_lookup(ns, sym_name, vi, &found, caller, handle);
  } else {
    soinfo* si = soinfo_from_handle(handle);
    if (si == nullptr) {
      DL_ERR("dlsym failed: invalid handle: %p", handle);
      return false;
    }
    sym = dlsym_handle_lookup(si, &found, sym_name, vi);
  }

  if (sym != nullptr) {
    uint32_t bind = ELF_ST_BIND(sym->st_info);

    if ((bind == STB_GLOBAL || bind == STB_WEAK) && sym->st_shndx != 0) {
#ifdef WANT_ARM_TRACING
      switch(ELF_ST_TYPE(sym->st_info))
      {
        case STT_FUNC:
        case STT_GNU_IFUNC:
        case STT_ARM_TFUNC:
          *symbol = reinterpret_cast<void*>(_create_wrapper((char*)symbol, (void*)found->resolve_symbol_address(sym), WRAPPER_DYNHOOK));
        default:
          *symbol = reinterpret_cast<void*>(found->resolve_symbol_address(sym));
      }
#else
      *symbol = reinterpret_cast<void*>(found->resolve_symbol_address(sym));
#endif
      return true;
    }

    DL_ERR("symbol \"%s\" found but not global", symbol_display_name(sym_name, sym_ver).c_str());
    return false;
  }

  DL_ERR_NO_PRINT("undefined symbol: %s", symbol_display_name(sym_name, sym_ver).c_str());
  return false;
}

int do_dlclose(void* handle) {
  ProtectedDataGuard guard;
  soinfo* si = soinfo_from_handle(handle);
  if (si == nullptr) {
    DL_ERR("invalid handle: %p", handle);
    return -1;
  }

  soinfo_unload(si);
  return 0;
}

bool init_namespaces(const char* public_ns_sonames, const char* anon_ns_library_path) {
  CHECK(public_ns_sonames != nullptr);
  if (g_public_namespace_initialized) {
    DL_ERR("public namespace has already been initialized.");
    return false;
  }

  std::vector<std::string> sonames = split(public_ns_sonames, ":");

  ProtectedDataGuard guard;

  auto failure_guard = make_scope_guard([&]() {
    g_public_namespace.clear();
  });

  for (const auto& soname : sonames) {
    soinfo* candidate = nullptr;

    find_loaded_library_by_soname(g_default_namespace, soname.c_str(), &candidate);

    if (candidate == nullptr) {
      DL_ERR("error initializing public namespace: a library with soname \"%s\""
             " was not found in the default namespace", soname.c_str());
      return false;
    }

    candidate->set_nodelete();
    g_public_namespace.push_back(candidate);
  }

  g_public_namespace_initialized = true;

  // create anonymous namespace
  // When the caller is nullptr - create_namespace will take global group
  // from the anonymous namespace, which is fine because anonymous namespace
  // is still pointing to the default one.
  android_namespace_t* anon_ns =
      create_namespace(nullptr, "(anonymous)", nullptr, anon_ns_library_path,
                       ANDROID_NAMESPACE_TYPE_REGULAR, nullptr, g_default_namespace);

  if (anon_ns == nullptr) {
    g_public_namespace_initialized = false;
    return false;
  }
  g_anonymous_namespace = anon_ns;
  failure_guard.disable();
  return true;
}

android_namespace_t* create_namespace(const void* caller_addr,
                                      const char* name,
                                      const char* ld_library_path,
                                      const char* default_library_path,
                                      uint64_t type,
                                      const char* permitted_when_isolated_path,
                                      android_namespace_t* parent_namespace) {
  if (!g_public_namespace_initialized) {
    DL_ERR("cannot create namespace: public namespace is not initialized.");
    return nullptr;
  }

  if (parent_namespace == nullptr) {
    // if parent_namespace is nullptr -> set it to the caller namespace
    soinfo* caller_soinfo = find_containing_library(caller_addr);

    parent_namespace = caller_soinfo != nullptr ?
                       caller_soinfo->get_primary_namespace() :
                       g_anonymous_namespace;
  }

  ProtectedDataGuard guard;
  std::vector<std::string> ld_library_paths;
  std::vector<std::string> default_library_paths;
  std::vector<std::string> permitted_paths;

  parse_path(ld_library_path, ":", &ld_library_paths);
  parse_path(default_library_path, ":", &default_library_paths);
  parse_path(permitted_when_isolated_path, ":", &permitted_paths);

  android_namespace_t* ns = new (g_namespace_allocator.alloc()) android_namespace_t();
  ns->set_name(name);
  ns->set_isolated((type & ANDROID_NAMESPACE_TYPE_ISOLATED) != 0);
  ns->set_ld_library_paths(std::move(ld_library_paths));
  ns->set_default_library_paths(std::move(default_library_paths));
  ns->set_permitted_paths(std::move(permitted_paths));

  if ((type & ANDROID_NAMESPACE_TYPE_SHARED) != 0) {
    // If shared - clone the parent namespace
    ns->add_soinfos(parent_namespace->soinfo_list());
  } else {
    // If not shared - copy only the shared group
    ns->add_soinfos(get_shared_group(parent_namespace));
  }

  return ns;
}

static ElfW(Addr) call_ifunc_resolver(ElfW(Addr) resolver_addr) {
  typedef ElfW(Addr) (*ifunc_resolver_t)(void);
  ifunc_resolver_t ifunc_resolver = reinterpret_cast<ifunc_resolver_t>(resolver_addr);
  ElfW(Addr) ifunc_addr = ifunc_resolver();
  TRACE_TYPE(RELO, "Called ifunc_resolver@%p. The result is %p",
      ifunc_resolver, reinterpret_cast<void*>(ifunc_addr));

  return ifunc_addr;
}

const version_info* VersionTracker::get_version_info(ElfW(Versym) source_symver) const {
  if (source_symver < 2 ||
      source_symver >= version_infos.size() ||
      version_infos[source_symver].name == nullptr) {
    return nullptr;
  }

  return &version_infos[source_symver];
}

void VersionTracker::add_version_info(size_t source_index,
                                      ElfW(Word) elf_hash,
                                      const char* ver_name,
                                      const soinfo* target_si) {
  if (source_index >= version_infos.size()) {
    version_infos.resize(source_index+1);
  }

  version_infos[source_index].elf_hash = elf_hash;
  version_infos[source_index].name = ver_name;
  version_infos[source_index].target_si = target_si;
}

bool VersionTracker::init_verneed(const soinfo* si_from) {
  uintptr_t verneed_ptr = si_from->get_verneed_ptr();

  if (verneed_ptr == 0) {
    return true;
  }

  size_t verneed_cnt = si_from->get_verneed_cnt();

  for (size_t i = 0, offset = 0; i<verneed_cnt; ++i) {
    const ElfW(Verneed)* verneed = reinterpret_cast<ElfW(Verneed)*>(verneed_ptr + offset);
    size_t vernaux_offset = offset + verneed->vn_aux;
    offset += verneed->vn_next;

    if (verneed->vn_version != 1) {
      DL_ERR("unsupported verneed[%zd] vn_version: %d (expected 1)", i, verneed->vn_version);
      return false;
    }

    const char* target_soname = si_from->get_string(verneed->vn_file);
    // find it in dependencies
    soinfo* target_si = si_from->get_children().find_if([&](const soinfo* si) {
      return si->get_soname() != nullptr && strcmp(si->get_soname(), target_soname) == 0;
    });

    if (target_si == nullptr) {
      DL_ERR("cannot find \"%s\" from verneed[%zd] in DT_NEEDED list for \"%s\"",
          target_soname, i, si_from->get_realpath());
      return false;
    }

    for (size_t j = 0; j<verneed->vn_cnt; ++j) {
      const ElfW(Vernaux)* vernaux = reinterpret_cast<ElfW(Vernaux)*>(verneed_ptr + vernaux_offset);
      vernaux_offset += vernaux->vna_next;

      const ElfW(Word) elf_hash = vernaux->vna_hash;
      const char* ver_name = si_from->get_string(vernaux->vna_name);
      ElfW(Half) source_index = vernaux->vna_other;

      add_version_info(source_index, elf_hash, ver_name, target_si);
    }
  }

  return true;
}

bool VersionTracker::init_verdef(const soinfo* si_from) {
  return for_each_verdef(si_from,
    [&](size_t, const ElfW(Verdef)* verdef, const ElfW(Verdaux)* verdaux) {
      add_version_info(verdef->vd_ndx, verdef->vd_hash,
          si_from->get_string(verdaux->vda_name), si_from);
      return false;
    }
  );
}

bool VersionTracker::init(const soinfo* si_from) {
  if (!si_from->has_min_version(2)) {
    return true;
  }

  return init_verneed(si_from) && init_verdef(si_from);
}

bool soinfo::lookup_version_info(const VersionTracker& version_tracker, ElfW(Word) sym,
                                 const char* sym_name, const version_info** vi) {
  const ElfW(Versym)* sym_ver_ptr = get_versym(sym);
  ElfW(Versym) sym_ver = sym_ver_ptr == nullptr ? 0 : *sym_ver_ptr;

  if (sym_ver != VER_NDX_LOCAL && sym_ver != VER_NDX_GLOBAL) {
    *vi = version_tracker.get_version_info(sym_ver);

    if (*vi == nullptr) {
      DL_ERR("cannot find verneed/verdef for version index=%d "
          "referenced by symbol \"%s\" at \"%s\"", sym_ver, sym_name, get_realpath());
      return false;
    }
  } else {
    // there is no version info
    *vi = nullptr;
  }

  return true;
}

#if !defined(__mips__)
#if defined(USE_RELA)
static ElfW(Addr) get_addend(ElfW(Rela)* rela, ElfW(Addr) reloc_addr) {
  return rela->r_addend;
}
#else
static ElfW(Addr) get_addend(ElfW(Rel)* rel, ElfW(Addr) reloc_addr) {
  if (ELFW(R_TYPE)(rel->r_info) == R_GENERIC_RELATIVE ||
      ELFW(R_TYPE)(rel->r_info) == R_GENERIC_IRELATIVE) {
    return *reinterpret_cast<ElfW(Addr)*>(reloc_addr);
  }
  return 0;
}
#endif

template<typename ElfRelIteratorT>
bool soinfo::relocate(const VersionTracker& version_tracker, ElfRelIteratorT&& rel_iterator,
                      const soinfo_list_t& global_group, const soinfo_list_t& local_group) {
  for (size_t idx = 0; rel_iterator.has_next(); ++idx) {
    const auto rel = rel_iterator.next();
    if (rel == nullptr) {
      return false;
    }

    ElfW(Word) type = ELFW(R_TYPE)(rel->r_info);
    ElfW(Word) sym = ELFW(R_SYM)(rel->r_info);

    ElfW(Addr) reloc = static_cast<ElfW(Addr)>(rel->r_offset + load_bias);
    ElfW(Addr) sym_addr = 0;
    const char* sym_name = nullptr;
    ElfW(Addr) addend = get_addend(rel, reloc);

    DEBUG("Processing \"%s\" relocation at index %zd", get_realpath(), idx);
    if (type == R_GENERIC_NONE) {
      continue;
    }

    const ElfW(Sym)* s = nullptr;
    soinfo* lsi = nullptr;
    const version_info* vi = nullptr;

    if (sym != 0) {
      sym_name = get_string(symtab_[sym].st_name);

      sym_addr = reinterpret_cast<ElfW(Addr)>(_get_hooked_symbol(sym_name, get_realpath()));
      if (!sym_addr) {
        if (!lookup_version_info(version_tracker, sym, sym_name, &vi)) {
          return false;
        }

        if (!soinfo_do_lookup(this, sym_name, vi, &lsi, global_group, local_group, &s)) {
          return false;
        }
      }
#ifdef WANT_ARM_TRACING
      else
      {
        // this will be slower.
        if (!lookup_version_info(version_tracker, sym, sym_name, &vi)) {
          return false;
        }

        if (!soinfo_do_lookup(this, sym_name, vi, &lsi, global_group, local_group, &s)) {
          return false;
        }

        switch(ELF_ST_TYPE(s->st_info))
        {
          case STT_FUNC:
          case STT_GNU_IFUNC:
          case STT_ARM_TFUNC:
            sym_addr = (ElfW(Addr))_create_wrapper(sym_name, (void*)sym_addr, WRAPPER_HOOKED);
            break;
        }
      }
#endif

      if (sym_addr == 0 && s == nullptr) {
        // We only allow an undefined symbol if this is a weak reference...
        s = &symtab_[sym];
        if (ELF_ST_BIND(s->st_info) != STB_WEAK) {
          DL_ERR("cannot locate symbol \"%s\" referenced by \"%s\"...", sym_name, get_realpath());
          return false;
        }

        /* IHI0044C AAELF 4.5.1.1:

           Libraries are not searched to resolve weak references.
           It is not an error for a weak reference to remain unsatisfied.

           During linking, the value of an undefined weak reference is:
           - Zero if the relocation type is absolute
           - The address of the place if the relocation is pc-relative
           - The address of nominal base address if the relocation
             type is base-relative.
         */

        switch (type) {
          case R_GENERIC_JUMP_SLOT:
          case R_GENERIC_GLOB_DAT:
          case R_GENERIC_RELATIVE:
          case R_GENERIC_IRELATIVE:
#if defined(__aarch64__)
          case R_AARCH64_ABS64:
          case R_AARCH64_ABS32:
          case R_AARCH64_ABS16:
#elif defined(__x86_64__)
          case R_X86_64_32:
          case R_X86_64_64:
#elif defined(__arm__)
          case R_ARM_ABS32:
#elif defined(__i386__)
          case R_386_32:
#endif
            /*
             * The sym_addr was initialized to be zero above, or the relocation
             * code below does not care about value of sym_addr.
             * No need to do anything.
             */
            break;
#if defined(__x86_64__)
          case R_X86_64_PC32:
            sym_addr = reloc;
            break;
#elif defined(__i386__)
          case R_386_PC32:
            sym_addr = reloc;
            break;
#endif
          default:
            DL_ERR("unknown weak reloc type %d @ %p (%zu)", type, rel, idx);
            return false;
        }
      } else if (sym_addr == 0) { // We got a definition.
#if !defined(__LP64__)
        // When relocating dso with text_relocation .text segment is
        // not executable. We need to restore elf flags before resolving
        // STT_GNU_IFUNC symbol.
        bool protect_segments = has_text_relocations &&
                                lsi == this &&
                                ELF_ST_TYPE(s->st_info) == STT_GNU_IFUNC;
        if (protect_segments) {
          if (phdr_table_protect_segments(phdr, phnum, load_bias) < 0) {
            DL_ERR("can't protect segments for \"%s\": %s",
                   get_realpath(), strerror(errno));
            return false;
          }
        }
#endif

#ifdef WANT_ARM_TRACING
        switch(ELF_ST_TYPE(s->st_info))
        {
          case STT_FUNC:
          case STT_GNU_IFUNC:
          case STT_ARM_TFUNC:
            sym_addr = (ElfW(Addr))_create_wrapper(sym_name,
                    (void*)lsi->resolve_symbol_address(s), WRAPPER_UNHOOKED);
            break;
          default:
            sym_addr = lsi->resolve_symbol_address(s);
            break;
        }
#else
         sym_addr = lsi->resolve_symbol_address(s);
#endif

#if !defined(__LP64__)
        if (protect_segments) {
          if (phdr_table_unprotect_segments(phdr, phnum, load_bias) < 0) {
            DL_ERR("can't unprotect loadable segments for \"%s\": %s",
                   get_realpath(), strerror(errno));
            return false;
          }
        }
#endif
      }
      count_relocation(kRelocSymbol);
    }

    switch (type) {
      case R_GENERIC_JUMP_SLOT:
        count_relocation(kRelocAbsolute);
        MARK(rel->r_offset);
        TRACE_TYPE(RELO, "RELO JMP_SLOT %16p <- %16p %s\n",
                   reinterpret_cast<void*>(reloc),
                   reinterpret_cast<void*>(sym_addr + addend), sym_name);

        *reinterpret_cast<ElfW(Addr)*>(reloc) = (sym_addr + addend);
        break;
      case R_GENERIC_GLOB_DAT:
        count_relocation(kRelocAbsolute);
        MARK(rel->r_offset);
        TRACE_TYPE(RELO, "RELO GLOB_DAT %16p <- %16p %s\n",
                   reinterpret_cast<void*>(reloc),
                   reinterpret_cast<void*>(sym_addr + addend), sym_name);
        *reinterpret_cast<ElfW(Addr)*>(reloc) = (sym_addr + addend);
        break;
      case R_GENERIC_RELATIVE:
        count_relocation(kRelocRelative);
        MARK(rel->r_offset);
        TRACE_TYPE(RELO, "RELO RELATIVE %16p <- %16p\n",
                   reinterpret_cast<void*>(reloc),
                   reinterpret_cast<void*>(load_bias + addend));
        *reinterpret_cast<ElfW(Addr)*>(reloc) = (load_bias + addend);
        break;
      case R_GENERIC_IRELATIVE:
        count_relocation(kRelocRelative);
        MARK(rel->r_offset);
        TRACE_TYPE(RELO, "RELO IRELATIVE %16p <- %16p\n",
                    reinterpret_cast<void*>(reloc),
                    reinterpret_cast<void*>(load_bias + addend));
        {
#if !defined(__LP64__)
          // When relocating dso with text_relocation .text segment is
          // not executable. We need to restore elf flags for this
          // particular call.
          if (has_text_relocations) {
            if (phdr_table_protect_segments(phdr, phnum, load_bias) < 0) {
              DL_ERR("can't protect segments for \"%s\": %s",
                     get_realpath(), strerror(errno));
              return false;
            }
          }
#endif
          ElfW(Addr) ifunc_addr = call_ifunc_resolver(load_bias + addend);
#if !defined(__LP64__)
          // Unprotect it afterwards...
          if (has_text_relocations) {
            if (phdr_table_unprotect_segments(phdr, phnum, load_bias) < 0) {
              DL_ERR("can't unprotect loadable segments for \"%s\": %s",
                     get_realpath(), strerror(errno));
              return false;
            }
          }
#endif
          *reinterpret_cast<ElfW(Addr)*>(reloc) = ifunc_addr;
        }
        break;

#if defined(__aarch64__)
      case R_AARCH64_ABS64:
        count_relocation(kRelocAbsolute);
        MARK(rel->r_offset);
        TRACE_TYPE(RELO, "RELO ABS64 %16" PRIx64 " <- %16" PRIx64 " %s\n",
                   reloc, sym_addr + addend, sym_name);
        *reinterpret_cast<ElfW(Addr)*>(reloc) = sym_addr + addend;
        break;
      case R_AARCH64_ABS32:
        count_relocation(kRelocAbsolute);
        MARK(rel->r_offset);
        TRACE_TYPE(RELO, "RELO ABS32 %16" PRIx64 " <- %16" PRIx64 " %s\n",
                   reloc, sym_addr + addend, sym_name);
        {
          const ElfW(Addr) min_value = static_cast<ElfW(Addr)>(INT32_MIN);
          const ElfW(Addr) max_value = static_cast<ElfW(Addr)>(UINT32_MAX);
          if ((min_value <= (sym_addr + addend)) &&
              ((sym_addr + addend) <= max_value)) {
            *reinterpret_cast<ElfW(Addr)*>(reloc) = sym_addr + addend;
          } else {
            DL_ERR("0x%016" PRIx64 " out of range 0x%016" PRIx64 " to 0x%016" PRIx64,
                   sym_addr + addend, min_value, max_value);
            return false;
          }
        }
        break;
      case R_AARCH64_ABS16:
        count_relocation(kRelocAbsolute);
        MARK(rel->r_offset);
        TRACE_TYPE(RELO, "RELO ABS16 %16" PRIx64 " <- %16" PRIx64 " %s\n",
                   reloc, sym_addr + addend, sym_name);
        {
          const ElfW(Addr) min_value = static_cast<ElfW(Addr)>(INT16_MIN);
          const ElfW(Addr) max_value = static_cast<ElfW(Addr)>(UINT16_MAX);
          if ((min_value <= (sym_addr + addend)) &&
              ((sym_addr + addend) <= max_value)) {
            *reinterpret_cast<ElfW(Addr)*>(reloc) = (sym_addr + addend);
          } else {
            DL_ERR("0x%016" PRIx64 " out of range 0x%016" PRIx64 " to 0x%016" PRIx64,
                   sym_addr + addend, min_value, max_value);
            return false;
          }
        }
        break;
      case R_AARCH64_PREL64:
        count_relocation(kRelocRelative);
        MARK(rel->r_offset);
        TRACE_TYPE(RELO, "RELO REL64 %16" PRIx64 " <- %16" PRIx64 " - %16" PRIx64 " %s\n",
                   reloc, sym_addr + addend, rel->r_offset, sym_name);
        *reinterpret_cast<ElfW(Addr)*>(reloc) = sym_addr + addend - rel->r_offset;
        break;
      case R_AARCH64_PREL32:
        count_relocation(kRelocRelative);
        MARK(rel->r_offset);
        TRACE_TYPE(RELO, "RELO REL32 %16" PRIx64 " <- %16" PRIx64 " - %16" PRIx64 " %s\n",
                   reloc, sym_addr + addend, rel->r_offset, sym_name);
        {
          const ElfW(Addr) min_value = static_cast<ElfW(Addr)>(INT32_MIN);
          const ElfW(Addr) max_value = static_cast<ElfW(Addr)>(UINT32_MAX);
          if ((min_value <= (sym_addr + addend - rel->r_offset)) &&
              ((sym_addr + addend - rel->r_offset) <= max_value)) {
            *reinterpret_cast<ElfW(Addr)*>(reloc) = sym_addr + addend - rel->r_offset;
          } else {
            DL_ERR("0x%016" PRIx64 " out of range 0x%016" PRIx64 " to 0x%016" PRIx64,
                   sym_addr + addend - rel->r_offset, min_value, max_value);
            return false;
          }
        }
        break;
      case R_AARCH64_PREL16:
        count_relocation(kRelocRelative);
        MARK(rel->r_offset);
        TRACE_TYPE(RELO, "RELO REL16 %16" PRIx64 " <- %16" PRIx64 " - %16" PRIx64 " %s\n",
                   reloc, sym_addr + addend, rel->r_offset, sym_name);
        {
          const ElfW(Addr) min_value = static_cast<ElfW(Addr)>(INT16_MIN);
          const ElfW(Addr) max_value = static_cast<ElfW(Addr)>(UINT16_MAX);
          if ((min_value <= (sym_addr + addend - rel->r_offset)) &&
              ((sym_addr + addend - rel->r_offset) <= max_value)) {
            *reinterpret_cast<ElfW(Addr)*>(reloc) = sym_addr + addend - rel->r_offset;
          } else {
            DL_ERR("0x%016" PRIx64 " out of range 0x%016" PRIx64 " to 0x%016" PRIx64,
                   sym_addr + addend - rel->r_offset, min_value, max_value);
            return false;
          }
        }
        break;

      case R_AARCH64_COPY:
        /*
         * ET_EXEC is not supported so this should not happen.
         *
         * http://infocenter.arm.com/help/topic/com.arm.doc.ihi0056b/IHI0056B_aaelf64.pdf
         *
         * Section 4.6.11 "Dynamic relocations"
         * R_AARCH64_COPY may only appear in executable objects where e_type is
         * set to ET_EXEC.
         */
        DL_ERR("%s R_AARCH64_COPY relocations are not supported", get_realpath());
        return false;
      case R_AARCH64_TLS_TPREL64:
        TRACE_TYPE(RELO, "RELO TLS_TPREL64 *** %16" PRIx64 " <- %16" PRIx64 " - %16llx\n",
                   reloc, (sym_addr + addend), rel->r_offset);
        break;
      case R_AARCH64_TLS_DTPREL32:
        TRACE_TYPE(RELO, "RELO TLS_DTPREL32 *** %16" PRIx64 " <- %16" PRIx64 " - %16llx\n",
                   reloc, (sym_addr + addend), rel->r_offset);
        break;
#elif defined(__x86_64__)
      case R_X86_64_32:
        count_relocation(kRelocRelative);
        MARK(rel->r_offset);
        TRACE_TYPE(RELO, "RELO R_X86_64_32 %08zx <- +%08zx %s", static_cast<size_t>(reloc),
                   static_cast<size_t>(sym_addr), sym_name);
        *reinterpret_cast<Elf32_Addr*>(reloc) = sym_addr + addend;
        break;
      case R_X86_64_64:
        count_relocation(kRelocRelative);
        MARK(rel->r_offset);
        TRACE_TYPE(RELO, "RELO R_X86_64_64 %08zx <- +%08zx %s", static_cast<size_t>(reloc),
                   static_cast<size_t>(sym_addr), sym_name);
        *reinterpret_cast<Elf64_Addr*>(reloc) = sym_addr + addend;
        break;
      case R_X86_64_PC32:
        count_relocation(kRelocRelative);
        MARK(rel->r_offset);
        TRACE_TYPE(RELO, "RELO R_X86_64_PC32 %08zx <- +%08zx (%08zx - %08zx) %s",
                   static_cast<size_t>(reloc), static_cast<size_t>(sym_addr - reloc),
                   static_cast<size_t>(sym_addr), static_cast<size_t>(reloc), sym_name);
        *reinterpret_cast<Elf32_Addr*>(reloc) = sym_addr + addend - reloc;
        break;
#elif defined(__arm__)
      case R_ARM_ABS32:
        count_relocation(kRelocAbsolute);
        MARK(rel->r_offset);
        TRACE_TYPE(RELO, "RELO ABS %08x <- %08x %s", reloc, sym_addr, sym_name);
        *reinterpret_cast<ElfW(Addr)*>(reloc) += sym_addr;
        break;
      case R_ARM_REL32:
        count_relocation(kRelocRelative);
        MARK(rel->r_offset);
        TRACE_TYPE(RELO, "RELO REL32 %08x <- %08x - %08x %s",
                   reloc, sym_addr, rel->r_offset, sym_name);
        *reinterpret_cast<ElfW(Addr)*>(reloc) += sym_addr - rel->r_offset;
        break;
      case R_ARM_COPY:
        /*
         * ET_EXEC is not supported so this should not happen.
         *
         * http://infocenter.arm.com/help/topic/com.arm.doc.ihi0044d/IHI0044D_aaelf.pdf
         *
         * Section 4.6.1.10 "Dynamic relocations"
         * R_ARM_COPY may only appear in executable objects where e_type is
         * set to ET_EXEC.
         */
#ifdef ENABLE_NON_PIE_SUPPORT
        if (allow_non_pie(get_realpath())) {
          if ((flags_ & FLAG_EXE) == 0) {
            DL_ERR("%s R_ARM_COPY relocations only supported for ET_EXEC", get_realpath());
            return false;
          }
          count_relocation(kRelocCopy);
          MARK(rel->r_offset);
          TRACE_TYPE(RELO, "RELO %08x <- %d @ %08x %s", reloc, s->st_size, sym_addr, sym_name);
          if (reloc == sym_addr) {
            const ElfW(Sym)* src = nullptr;

            if (!soinfo_do_lookup(NULL, sym_name, vi, &lsi, global_group, local_group, &src)) {
              DL_ERR("%s R_ARM_COPY relocation source cannot be resolved", get_realpath());
              return false;
            }
            if (lsi->has_DT_SYMBOLIC) {
              DL_ERR("%s invalid R_ARM_COPY relocation against DT_SYMBOLIC shared "
                     "library %s (built with -Bsymbolic?)", get_realpath(), lsi->soname_);
              return false;
            }
            if (s->st_size < src->st_size) {
              DL_ERR("%s R_ARM_COPY relocation size mismatch (%d < %d)",
                     get_realpath(), s->st_size, src->st_size);
              return false;
            }
            memcpy(reinterpret_cast<void*>(reloc),
                   reinterpret_cast<void*>(src->st_value + lsi->load_bias), src->st_size);
          } else {
            DL_ERR("%s R_ARM_COPY relocation target cannot be resolved", get_realpath());
            return false;
          }
          break;
        }
#endif
        DL_ERR("%s R_ARM_COPY relocations are not supported", get_realpath());
        return false;
#elif defined(__i386__)
      case R_386_32:
        count_relocation(kRelocRelative);
        MARK(rel->r_offset);
        TRACE_TYPE(RELO, "RELO R_386_32 %08x <- +%08x %s", reloc, sym_addr, sym_name);
        *reinterpret_cast<ElfW(Addr)*>(reloc) += sym_addr;
        break;
      case R_386_PC32:
        count_relocation(kRelocRelative);
        MARK(rel->r_offset);
        TRACE_TYPE(RELO, "RELO R_386_PC32 %08x <- +%08x (%08x - %08x) %s",
                   reloc, (sym_addr - reloc), sym_addr, reloc, sym_name);
        *reinterpret_cast<ElfW(Addr)*>(reloc) += (sym_addr - reloc);
        break;
#endif
      default:
        DL_ERR("unknown reloc type %d @ %p (%zu)", type, rel, idx);
        return false;
    }
  }
  return true;
}
#endif  // !defined(__mips__)

void soinfo::call_array(const char* array_name, linker_function_t* functions,
                        size_t count, bool reverse) {
  if (functions == nullptr) {
    return;
  }

  TRACE("[ Calling %s (size %zd) @ %p for \"%s\" ]", array_name, count, functions, get_realpath());

  int begin = reverse ? (count - 1) : 0;
  int end = reverse ? -1 : count;
  int step = reverse ? -1 : 1;

  for (int i = begin; i != end; i += step) {
    TRACE("[ %s[%d] == %p ]", array_name, i, functions[i]);
    call_function("function", functions[i]);
  }

  TRACE("[ Done calling %s for \"%s\" ]", array_name, get_realpath());
}

void soinfo::call_function(const char* function_name, linker_function_t function) {
  if (function == nullptr || reinterpret_cast<uintptr_t>(function) == static_cast<uintptr_t>(-1)) {
    return;
  }

  TRACE("[ Calling %s @ %p for \"%s\" ]", function_name, function, get_realpath());
  function();
  TRACE("[ Done calling %s @ %p for \"%s\" ]", function_name, function, get_realpath());
}

void soinfo::call_pre_init_constructors() {
  // DT_PREINIT_ARRAY functions are called before any other constructors for executables,
  // but ignored in a shared library.
  call_array("DT_PREINIT_ARRAY", preinit_array_, preinit_array_count_, false);
}

void soinfo::call_constructors() {
  if (constructors_called) {
    return;
  }

  if (soname_ != nullptr && strcmp(soname_, "libc.so") == 0) {
    DEBUG("HYBRIS: =============> Skipping libc.so\n");
    return;
  }

  // We set constructors_called before actually calling the constructors, otherwise it doesn't
  // protect against recursive constructor calls. One simple example of constructor recursion
  // is the libc debug malloc, which is implemented in libc_malloc_debug_leak.so:
  // 1. The program depends on libc, so libc's constructor is called here.
  // 2. The libc constructor calls dlopen() to load libc_malloc_debug_leak.so.
  // 3. dlopen() calls the constructors on the newly created
  //    soinfo for libc_malloc_debug_leak.so.
  // 4. The debug .so depends on libc, so CallConstructors is
  //    called again with the libc soinfo. If it doesn't trigger the early-
  //    out above, the libc constructor will be called again (recursively!).
  constructors_called = true;

  if (!is_main_executable() && preinit_array_ != nullptr) {
    // The GNU dynamic linker silently ignores these, but we warn the developer.
    PRINT("\"%s\": ignoring DT_PREINIT_ARRAY in shared library!", get_realpath());
  }

  get_children().for_each([] (soinfo* si) {
    si->call_constructors();
  });

  TRACE("\"%s\": calling constructors", get_realpath());

  // DT_INIT should be called before DT_INIT_ARRAY if both are present.
  call_function("DT_INIT", init_func_);
  call_array("DT_INIT_ARRAY", init_array_, init_array_count_, false);
}

void soinfo::call_destructors() {
  if (!constructors_called) {
    return;
  }
  TRACE("\"%s\": calling destructors", get_realpath());

  // DT_FINI_ARRAY must be parsed in reverse order.
  call_array("DT_FINI_ARRAY", fini_array_, fini_array_count_, true);

  // DT_FINI should be called after DT_FINI_ARRAY if both are present.
  call_function("DT_FINI", fini_func_);

  // This is needed on second call to dlopen
  // after library has been unloaded with RTLD_NODELETE
  constructors_called = false;
}

void soinfo::add_child(soinfo* child) {
  if (has_min_version(0)) {
    child->parents_.push_back(this);
    this->children_.push_back(child);
  }
}

void soinfo::remove_all_links() {
  if (!has_min_version(0)) {
    return;
  }

  // 1. Untie connected soinfos from 'this'.
  children_.for_each([&] (soinfo* child) {
    child->parents_.remove_if([&] (const soinfo* parent) {
      return parent == this;
    });
  });

  parents_.for_each([&] (soinfo* parent) {
    parent->children_.remove_if([&] (const soinfo* child) {
      return child == this;
    });
  });

  // 2. Remove from the primary namespace
  primary_namespace_->remove_soinfo(this);
  primary_namespace_ = nullptr;

  // 3. Remove from secondary namespaces
  secondary_namespaces_.for_each([&](android_namespace_t* ns) {
    ns->remove_soinfo(this);
  });


  // 4. Once everything untied - clear local lists.
  parents_.clear();
  children_.clear();
  secondary_namespaces_.clear();
}

dev_t soinfo::get_st_dev() const {
  if (has_min_version(0)) {
    return st_dev_;
  }

  return 0;
};

ino_t soinfo::get_st_ino() const {
  if (has_min_version(0)) {
    return st_ino_;
  }

  return 0;
}

off64_t soinfo::get_file_offset() const {
  if (has_min_version(1)) {
    return file_offset_;
  }

  return 0;
}

uint32_t soinfo::get_rtld_flags() const {
  if (has_min_version(1)) {
    return rtld_flags_;
  }

  return 0;
}

uint32_t soinfo::get_dt_flags_1() const {
  if (has_min_version(1)) {
    return dt_flags_1_;
  }

  return 0;
}

void soinfo::set_dt_flags_1(uint32_t dt_flags_1) {
  if (has_min_version(1)) {
    if ((dt_flags_1 & DF_1_GLOBAL) != 0) {
      rtld_flags_ |= RTLD_GLOBAL;
    }

    if ((dt_flags_1 & DF_1_NODELETE) != 0) {
      rtld_flags_ |= RTLD_NODELETE;
    }

    dt_flags_1_ = dt_flags_1;
  }
}

void soinfo::set_nodelete() {
  rtld_flags_ |= RTLD_NODELETE;
}

const char* soinfo::get_realpath() const {
#if defined(__work_around_b_24465209__)
  if (has_min_version(2)) {
    return realpath_.c_str();
  } else {
    return old_name_;
  }
#else
  return realpath_.c_str();
#endif
}

void soinfo::set_soname(const char* soname) {
#if defined(__work_around_b_24465209__)
  if (has_min_version(2)) {
    soname_ = soname;
  }
  strlcpy(old_name_, soname_, sizeof(old_name_));
#else
  soname_ = soname;
#endif
}

const char* soinfo::get_soname() const {
#if defined(__work_around_b_24465209__)
  if (has_min_version(2)) {
    return soname_;
  } else {
    return old_name_;
  }
#else
  return soname_;
#endif
}

// This is a return on get_children()/get_parents() if
// 'this->flags' does not have FLAG_NEW_SOINFO set.
static soinfo::soinfo_list_t g_empty_list;

soinfo::soinfo_list_t& soinfo::get_children() {
  if (has_min_version(0)) {
    return children_;
  }

  return g_empty_list;
}

const soinfo::soinfo_list_t& soinfo::get_children() const {
  if (has_min_version(0)) {
    return children_;
  }

  return g_empty_list;
}

soinfo::soinfo_list_t& soinfo::get_parents() {
  if (has_min_version(0)) {
    return parents_;
  }

  return g_empty_list;
}

static std::vector<std::string> g_empty_runpath;

const std::vector<std::string>& soinfo::get_dt_runpath() const {
  if (has_min_version(3)) {
    return dt_runpath_;
  }

  return g_empty_runpath;
}

android_namespace_t* soinfo::get_primary_namespace() {
  if (has_min_version(3)) {
    return primary_namespace_;
  }

  return g_default_namespace;
}

void soinfo::add_secondary_namespace(android_namespace_t* secondary_ns) {
  CHECK(has_min_version(3));
  secondary_namespaces_.push_back(secondary_ns);
}

ElfW(Addr) soinfo::resolve_symbol_address(const ElfW(Sym)* s) const {
  if (ELF_ST_TYPE(s->st_info) == STT_GNU_IFUNC) {
    return call_ifunc_resolver(s->st_value + load_bias);
  }

  return static_cast<ElfW(Addr)>(s->st_value + load_bias);
}

const char* soinfo::get_string(ElfW(Word) index) const {
  if (has_min_version(1) && (index >= strtab_size_)) {
    __libc_fatal("%s: strtab out of bounds error; STRSZ=%zd, name=%d",
        get_realpath(), strtab_size_, index);
  }

  return strtab_ + index;
}

bool soinfo::is_gnu_hash() const {
  return (flags_ & FLAG_GNU_HASH) != 0;
}

bool soinfo::can_unload() const {
  return !is_linked() || ((get_rtld_flags() & (RTLD_NODELETE | RTLD_GLOBAL)) == 0);
}

bool soinfo::is_linked() const {
  return (flags_ & FLAG_LINKED) != 0;
}

bool soinfo::is_main_executable() const {
  return (flags_ & FLAG_EXE) != 0;
}

bool soinfo::is_linker() const {
  return (flags_ & FLAG_LINKER) != 0;
}

void soinfo::set_linked() {
  flags_ |= FLAG_LINKED;
}

void soinfo::set_linker_flag() {
  flags_ |= FLAG_LINKER;
}

void soinfo::set_main_executable() {
  flags_ |= FLAG_EXE;
}

void soinfo::increment_ref_count() {
  local_group_root_->ref_count_++;
}

size_t soinfo::decrement_ref_count() {
  return --local_group_root_->ref_count_;
}

soinfo* soinfo::get_local_group_root() const {
  return local_group_root_;
}


void soinfo::set_mapped_by_caller(bool mapped_by_caller) {
  if (mapped_by_caller) {
    flags_ |= FLAG_MAPPED_BY_CALLER;
  } else {
    flags_ &= ~FLAG_MAPPED_BY_CALLER;
  }
}

bool soinfo::is_mapped_by_caller() const {
  return (flags_ & FLAG_MAPPED_BY_CALLER) != 0;
}

// This function returns api-level at the time of
// dlopen/load. Note that libraries opened by system
// will always have 'current' api level.
uint32_t soinfo::get_target_sdk_version() const {
  if (!has_min_version(2)) {
    return __ANDROID_API__;
  }

  return local_group_root_->target_sdk_version_;
}

uintptr_t soinfo::get_handle() const {
  CHECK(has_min_version(3));
  CHECK(handle_ != 0);
  return handle_;
}

void* soinfo::to_handle() {
  if (get_application_target_sdk_version() <= 23 || !has_min_version(3)) {
    return this;
  }

  return reinterpret_cast<void*>(get_handle());
}

void soinfo::generate_handle() {
  CHECK(has_min_version(3));
  CHECK(handle_ == 0); // Make sure this is the first call

  // Make sure the handle is unique and does not collide
  // with special values which are RTLD_DEFAULT and RTLD_NEXT.
  do {
    handle_ = rand();
    //arc4random_buf(&handle_, sizeof(handle_));
    // the least significant bit for the handle is always 1
    // making it easy to test the type of handle passed to
    // dl* functions.
    handle_ = handle_ | 1;
  } while (handle_ == reinterpret_cast<uintptr_t>(RTLD_DEFAULT) ||
           handle_ == reinterpret_cast<uintptr_t>(RTLD_NEXT) ||
           g_soinfo_handles_map.find(handle_) != g_soinfo_handles_map.end());

  g_soinfo_handles_map[handle_] = this;
}

bool soinfo::prelink_image() {
  /* Extract dynamic section */
  ElfW(Word) dynamic_flags = 0;
  phdr_table_get_dynamic_section(phdr, phnum, load_bias, &dynamic, &dynamic_flags);

  /* We can't log anything until the linker is relocated */
  bool relocating_linker = (flags_ & FLAG_LINKER) != 0;
  if (!relocating_linker) {
    INFO("[ Linking \"%s\" ]", get_realpath());
    DEBUG("si->base = %p si->flags = 0x%08x", reinterpret_cast<void*>(base), flags_);
  }

  if (dynamic == nullptr) {
    if (!relocating_linker) {
      DL_ERR("missing PT_DYNAMIC in \"%s\"", get_realpath());
    }
    return false;
  } else {
    if (!relocating_linker) {
      DEBUG("dynamic = %p", dynamic);
    }
  }

#if defined(__arm__)
  (void) phdr_table_get_arm_exidx(phdr, phnum, load_bias,
                                  &ARM_exidx, &ARM_exidx_count);
#endif

  // Extract useful information from dynamic section.
  // Note that: "Except for the DT_NULL element at the end of the array,
  // and the relative order of DT_NEEDED elements, entries may appear in any order."
  //
  // source: http://www.sco.com/developers/gabi/1998-04-29/ch5.dynamic.html
  uint32_t needed_count = 0;
  for (ElfW(Dyn)* d = dynamic; d->d_tag != DT_NULL; ++d) {
    DEBUG("d = %p, d[0](tag) = %p d[1](val) = %p",
          d, reinterpret_cast<void*>(d->d_tag), reinterpret_cast<void*>(d->d_un.d_val));
    switch (d->d_tag) {
      case DT_SONAME:
        // this is parsed after we have strtab initialized (see below).
        break;

      case DT_HASH:
        nbucket_ = reinterpret_cast<uint32_t*>(load_bias + d->d_un.d_ptr)[0];
        nchain_ = reinterpret_cast<uint32_t*>(load_bias + d->d_un.d_ptr)[1];
        bucket_ = reinterpret_cast<uint32_t*>(load_bias + d->d_un.d_ptr + 8);
        chain_ = reinterpret_cast<uint32_t*>(load_bias + d->d_un.d_ptr + 8 + nbucket_ * 4);
        break;

      case DT_GNU_HASH:
        gnu_nbucket_ = reinterpret_cast<uint32_t*>(load_bias + d->d_un.d_ptr)[0];
        // skip symndx
        gnu_maskwords_ = reinterpret_cast<uint32_t*>(load_bias + d->d_un.d_ptr)[2];
        gnu_shift2_ = reinterpret_cast<uint32_t*>(load_bias + d->d_un.d_ptr)[3];

        gnu_bloom_filter_ = reinterpret_cast<ElfW(Addr)*>(load_bias + d->d_un.d_ptr + 16);
        gnu_bucket_ = reinterpret_cast<uint32_t*>(gnu_bloom_filter_ + gnu_maskwords_);
        // amend chain for symndx = header[1]
        gnu_chain_ = gnu_bucket_ + gnu_nbucket_ -
            reinterpret_cast<uint32_t*>(load_bias + d->d_un.d_ptr)[1];

        if (!powerof2(gnu_maskwords_)) {
          DL_ERR("invalid maskwords for gnu_hash = 0x%x, in \"%s\" expecting power to two",
              gnu_maskwords_, get_realpath());
          return false;
        }
        --gnu_maskwords_;

        flags_ |= FLAG_GNU_HASH;
        break;

      case DT_STRTAB:
        strtab_ = reinterpret_cast<const char*>(load_bias + d->d_un.d_ptr);
        break;

      case DT_STRSZ:
        strtab_size_ = d->d_un.d_val;
        break;

      case DT_SYMTAB:
        symtab_ = reinterpret_cast<ElfW(Sym)*>(load_bias + d->d_un.d_ptr);
        break;

      case DT_SYMENT:
        if (d->d_un.d_val != sizeof(ElfW(Sym))) {
          DL_ERR("invalid DT_SYMENT: %zd in \"%s\"",
              static_cast<size_t>(d->d_un.d_val), get_realpath());
          return false;
        }
        break;

      case DT_PLTREL:
#if defined(USE_RELA)
        if (d->d_un.d_val != DT_RELA) {
          DL_ERR("unsupported DT_PLTREL in \"%s\"; expected DT_RELA", get_realpath());
          return false;
        }
#else
        if (d->d_un.d_val != DT_REL) {
          DL_ERR("unsupported DT_PLTREL in \"%s\"; expected DT_REL", get_realpath());
          return false;
        }
#endif
        break;

      case DT_JMPREL:
#if defined(USE_RELA)
        plt_rela_ = reinterpret_cast<ElfW(Rela)*>(load_bias + d->d_un.d_ptr);
#else
        plt_rel_ = reinterpret_cast<ElfW(Rel)*>(load_bias + d->d_un.d_ptr);
#endif
        break;

      case DT_PLTRELSZ:
#if defined(USE_RELA)
        plt_rela_count_ = d->d_un.d_val / sizeof(ElfW(Rela));
#else
        plt_rel_count_ = d->d_un.d_val / sizeof(ElfW(Rel));
#endif
        break;

      case DT_PLTGOT:
#if defined(__mips__)
        // Used by mips and mips64.
        plt_got_ = reinterpret_cast<ElfW(Addr)**>(load_bias + d->d_un.d_ptr);
#endif
        // Ignore for other platforms... (because RTLD_LAZY is not supported)
        break;

      case DT_DEBUG:
        // Set the DT_DEBUG entry to the address of _r_debug for GDB
        // if the dynamic table is writable
// FIXME: not working currently for N64
// The flags for the LOAD and DYNAMIC program headers do not agree.
// The LOAD section containing the dynamic table has been mapped as
// read-only, but the DYNAMIC header claims it is writable.
#if !(defined(__mips__) && defined(__LP64__))
        if ((dynamic_flags & PF_W) != 0) {
          d->d_un.d_val = reinterpret_cast<uintptr_t>(&_r_debug);
        }
#endif
        break;
#if defined(USE_RELA)
      case DT_RELA:
        rela_ = reinterpret_cast<ElfW(Rela)*>(load_bias + d->d_un.d_ptr);
        break;

      case DT_RELASZ:
        rela_count_ = d->d_un.d_val / sizeof(ElfW(Rela));
        break;

      case DT_ANDROID_RELA:
        android_relocs_ = reinterpret_cast<uint8_t*>(load_bias + d->d_un.d_ptr);
        break;

      case DT_ANDROID_RELASZ:
        android_relocs_size_ = d->d_un.d_val;
        break;

      case DT_ANDROID_REL:
        DL_ERR("unsupported DT_ANDROID_REL in \"%s\"", get_realpath());
        return false;

      case DT_ANDROID_RELSZ:
        DL_ERR("unsupported DT_ANDROID_RELSZ in \"%s\"", get_realpath());
        return false;

      case DT_RELAENT:
        if (d->d_un.d_val != sizeof(ElfW(Rela))) {
          DL_ERR("invalid DT_RELAENT: %zd", static_cast<size_t>(d->d_un.d_val));
          return false;
        }
        break;

      // ignored (see DT_RELCOUNT comments for details)
      case DT_RELACOUNT:
        break;

      case DT_REL:
        DL_ERR("unsupported DT_REL in \"%s\"", get_realpath());
        return false;

      case DT_RELSZ:
        DL_ERR("unsupported DT_RELSZ in \"%s\"", get_realpath());
        return false;

#else
      case DT_REL:
        rel_ = reinterpret_cast<ElfW(Rel)*>(load_bias + d->d_un.d_ptr);
        break;

      case DT_RELSZ:
        rel_count_ = d->d_un.d_val / sizeof(ElfW(Rel));
        break;

      case DT_RELENT:
        if (d->d_un.d_val != sizeof(ElfW(Rel))) {
          DL_ERR("invalid DT_RELENT: %zd", static_cast<size_t>(d->d_un.d_val));
          return false;
        }
        break;

      case DT_ANDROID_REL:
        android_relocs_ = reinterpret_cast<uint8_t*>(load_bias + d->d_un.d_ptr);
        break;

      case DT_ANDROID_RELSZ:
        android_relocs_size_ = d->d_un.d_val;
        break;

      case DT_ANDROID_RELA:
        DL_ERR("unsupported DT_ANDROID_RELA in \"%s\"", get_realpath());
        return false;

      case DT_ANDROID_RELASZ:
        DL_ERR("unsupported DT_ANDROID_RELASZ in \"%s\"", get_realpath());
        return false;

      // "Indicates that all RELATIVE relocations have been concatenated together,
      // and specifies the RELATIVE relocation count."
      //
      // TODO: Spec also mentions that this can be used to optimize relocation process;
      // Not currently used by bionic linker - ignored.
      case DT_RELCOUNT:
        break;

      case DT_RELA:
        DL_ERR("unsupported DT_RELA in \"%s\"", get_realpath());
        return false;

      case DT_RELASZ:
        DL_ERR("unsupported DT_RELASZ in \"%s\"", get_realpath());
        return false;

#endif
      case DT_INIT:
        init_func_ = reinterpret_cast<linker_function_t>(load_bias + d->d_un.d_ptr);
        DEBUG("%s constructors (DT_INIT) found at %p", get_realpath(), init_func_);
        break;

      case DT_FINI:
        fini_func_ = reinterpret_cast<linker_function_t>(load_bias + d->d_un.d_ptr);
        DEBUG("%s destructors (DT_FINI) found at %p", get_realpath(), fini_func_);
        break;

      case DT_INIT_ARRAY:
        init_array_ = reinterpret_cast<linker_function_t*>(load_bias + d->d_un.d_ptr);
        DEBUG("%s constructors (DT_INIT_ARRAY) found at %p", get_realpath(), init_array_);
        break;

      case DT_INIT_ARRAYSZ:
        init_array_count_ = static_cast<uint32_t>(d->d_un.d_val) / sizeof(ElfW(Addr));
        break;

      case DT_FINI_ARRAY:
        fini_array_ = reinterpret_cast<linker_function_t*>(load_bias + d->d_un.d_ptr);
        DEBUG("%s destructors (DT_FINI_ARRAY) found at %p", get_realpath(), fini_array_);
        break;

      case DT_FINI_ARRAYSZ:
        fini_array_count_ = static_cast<uint32_t>(d->d_un.d_val) / sizeof(ElfW(Addr));
        break;

      case DT_PREINIT_ARRAY:
        preinit_array_ = reinterpret_cast<linker_function_t*>(load_bias + d->d_un.d_ptr);
        DEBUG("%s constructors (DT_PREINIT_ARRAY) found at %p", get_realpath(), preinit_array_);
        break;

      case DT_PREINIT_ARRAYSZ:
        preinit_array_count_ = static_cast<uint32_t>(d->d_un.d_val) / sizeof(ElfW(Addr));
        break;

      case DT_TEXTREL:
#if defined(__LP64__)
        DL_ERR("text relocations (DT_TEXTREL) found in 64-bit ELF file \"%s\"", get_realpath());
        return false;
#else
        has_text_relocations = true;
        break;
#endif

      case DT_SYMBOLIC:
        has_DT_SYMBOLIC = true;
        break;

      case DT_NEEDED:
        ++needed_count;
        break;

      case DT_FLAGS:
        if (d->d_un.d_val & DF_TEXTREL) {
#if defined(__LP64__)
          DL_ERR("text relocations (DF_TEXTREL) found in 64-bit ELF file \"%s\"", get_realpath());
          return false;
#else
          has_text_relocations = true;
#endif
        }
        if (d->d_un.d_val & DF_SYMBOLIC) {
          has_DT_SYMBOLIC = true;
        }
        break;

      case DT_FLAGS_1:
        set_dt_flags_1(d->d_un.d_val);

        if ((d->d_un.d_val & ~SUPPORTED_DT_FLAGS_1) != 0) {
          DL_WARN("%s: unsupported flags DT_FLAGS_1=%p", get_realpath(), reinterpret_cast<void*>(d->d_un.d_val));
        }
        break;
#if defined(__mips__)
      case DT_MIPS_RLD_MAP:
        // Set the DT_MIPS_RLD_MAP entry to the address of _r_debug for GDB.
        {
          r_debug** dp = reinterpret_cast<r_debug**>(load_bias + d->d_un.d_ptr);
          *dp = &_r_debug;
        }
        break;
      case DT_MIPS_RLD_MAP2:
        // Set the DT_MIPS_RLD_MAP2 entry to the address of _r_debug for GDB.
        {
          r_debug** dp = reinterpret_cast<r_debug**>(
              reinterpret_cast<ElfW(Addr)>(d) + d->d_un.d_val);
          *dp = &_r_debug;
        }
        break;

      case DT_MIPS_RLD_VERSION:
      case DT_MIPS_FLAGS:
      case DT_MIPS_BASE_ADDRESS:
      case DT_MIPS_UNREFEXTNO:
        break;

      case DT_MIPS_SYMTABNO:
        mips_symtabno_ = d->d_un.d_val;
        break;

      case DT_MIPS_LOCAL_GOTNO:
        mips_local_gotno_ = d->d_un.d_val;
        break;

      case DT_MIPS_GOTSYM:
        mips_gotsym_ = d->d_un.d_val;
        break;
#endif
      // Ignored: "Its use has been superseded by the DF_BIND_NOW flag"
      case DT_BIND_NOW:
        break;

      case DT_VERSYM:
        versym_ = reinterpret_cast<ElfW(Versym)*>(load_bias + d->d_un.d_ptr);
        break;

      case DT_VERDEF:
        verdef_ptr_ = load_bias + d->d_un.d_ptr;
        break;
      case DT_VERDEFNUM:
        verdef_cnt_ = d->d_un.d_val;
        break;

      case DT_VERNEED:
        verneed_ptr_ = load_bias + d->d_un.d_ptr;
        break;

      case DT_VERNEEDNUM:
        verneed_cnt_ = d->d_un.d_val;
        break;

      case DT_RUNPATH:
        // this is parsed after we have strtab initialized (see below).
        break;

      default:
        if (!relocating_linker) {
          DL_WARN("%s: unused DT entry: type %p arg %p", get_realpath(),
              reinterpret_cast<void*>(d->d_tag), reinterpret_cast<void*>(d->d_un.d_val));
        }
        break;
    }
  }

#if defined(__mips__) && !defined(__LP64__)
  if (!mips_check_and_adjust_fp_modes()) {
    return false;
  }
#endif

  DEBUG("si->base = %p, si->strtab = %p, si->symtab = %p",
        reinterpret_cast<void*>(base), strtab_, symtab_);

  // Sanity checks.
  if (relocating_linker && needed_count != 0) {
    DL_ERR("linker cannot have DT_NEEDED dependencies on other libraries");
    return false;
  }
  if (nbucket_ == 0 && gnu_nbucket_ == 0) {
    DL_ERR("empty/missing DT_HASH/DT_GNU_HASH in \"%s\" "
        "(new hash type from the future?)", get_realpath());
    return false;
  }
  if (strtab_ == 0) {
    DL_ERR("empty/missing DT_STRTAB in \"%s\"", get_realpath());
    return false;
  }
  if (symtab_ == 0) {
    DL_ERR("empty/missing DT_SYMTAB in \"%s\"", get_realpath());
    return false;
  }

  // second pass - parse entries relying on strtab
  for (ElfW(Dyn)* d = dynamic; d->d_tag != DT_NULL; ++d) {
    switch (d->d_tag) {
      case DT_SONAME:
        set_soname(get_string(d->d_un.d_val));
        break;
      case DT_RUNPATH:
        set_dt_runpath(get_string(d->d_un.d_val));
        break;
    }
  }

  // Before M release linker was using basename in place of soname.
  // In the case when dt_soname is absent some apps stop working
  // because they can't find dt_needed library by soname.
  // This workaround should keep them working. (applies only
  // for apps targeting sdk version <=22). Make an exception for
  // the main executable and linker; they do not need to have dt_soname
  if (soname_ == nullptr && this != somain && (flags_ & FLAG_LINKER) == 0 &&
      get_application_target_sdk_version() <= 22) {
    soname_ = basename(realpath_.c_str());
    DL_WARN("%s: is missing DT_SONAME will use basename as a replacement: \"%s\"",
        get_realpath(), soname_);
    // Don't call add_dlwarning because a missing DT_SONAME isn't important enough to show in the UI
  }
  return true;
}

bool soinfo::link_image(const soinfo_list_t& global_group, const soinfo_list_t& local_group,
                        const android_dlextinfo* extinfo) {

  local_group_root_ = local_group.front();
  if (local_group_root_ == nullptr) {
    local_group_root_ = this;
  }

  if ((flags_ & FLAG_LINKER) == 0 && local_group_root_ == this) {
    target_sdk_version_ = get_application_target_sdk_version();
  }

  VersionTracker version_tracker;

  if (!version_tracker.init(this)) {
    return false;
  }

#if !defined(__LP64__)
  if (has_text_relocations) {
    // Fail if app is targeting sdk version > 22
#if !defined(__i386__) // ffmpeg says that they require text relocations on x86
#if defined(TARGET_NEEDS_PLATFORM_TEXT_RELOCATIONS)
    if (get_application_target_sdk_version() != __ANDROID_API__
        && get_application_target_sdk_version() > 22) {
#else
    if (get_application_target_sdk_version() > 22) {
#endif
      PRINT("%s: has text relocations", get_realpath());
      DL_ERR("%s: has text relocations", get_realpath());
      return false;
    }
#endif
    // Make segments writable to allow text relocations to work properly. We will later call
    // phdr_table_protect_segments() after all of them are applied.
    DL_WARN("%s has text relocations. This is wasting memory and prevents "
            "security hardening. Please fix.", get_realpath());
    add_dlwarning(get_realpath(), "text relocations");
    if (phdr_table_unprotect_segments(phdr, phnum, load_bias) < 0) {
      DL_ERR("can't unprotect loadable segments for \"%s\": %s",
             get_realpath(), strerror(errno));
      return false;
    }
  }
#endif

  if (android_relocs_ != nullptr) {
    // check signature
    if (android_relocs_size_ > 3 &&
        android_relocs_[0] == 'A' &&
        android_relocs_[1] == 'P' &&
        android_relocs_[2] == 'S' &&
        android_relocs_[3] == '2') {
      DEBUG("[ android relocating %s ]", get_realpath());

      bool relocated = false;
      const uint8_t* packed_relocs = android_relocs_ + 4;
      const size_t packed_relocs_size = android_relocs_size_ - 4;

      relocated = relocate(
          version_tracker,
          packed_reloc_iterator<sleb128_decoder>(
            sleb128_decoder(packed_relocs, packed_relocs_size)),
          global_group, local_group);

      if (!relocated) {
        return false;
      }
    } else {
      DL_ERR("bad android relocation header.");
      return false;
    }
  }

#if defined(USE_RELA)
  if (rela_ != nullptr) {
    DEBUG("[ relocating %s ]", get_realpath());
    if (!relocate(version_tracker,
            plain_reloc_iterator(rela_, rela_count_), global_group, local_group)) {
      return false;
    }
  }
  if (plt_rela_ != nullptr) {
    DEBUG("[ relocating %s plt ]", get_realpath());
    if (!relocate(version_tracker,
            plain_reloc_iterator(plt_rela_, plt_rela_count_), global_group, local_group)) {
      return false;
    }
  }
#else
  if (rel_ != nullptr) {
    DEBUG("[ relocating %s ]", get_realpath());
    if (!relocate(version_tracker,
            plain_reloc_iterator(rel_, rel_count_), global_group, local_group)) {
      return false;
    }
  }
  if (plt_rel_ != nullptr) {
    DEBUG("[ relocating %s plt ]", get_realpath());
    if (!relocate(version_tracker,
            plain_reloc_iterator(plt_rel_, plt_rel_count_), global_group, local_group)) {
      return false;
    }
  }
#endif

#if defined(__mips__)
  if (!mips_relocate_got(version_tracker, global_group, local_group)) {
    return false;
  }
#endif

  DEBUG("[ finished linking %s ]", get_realpath());

#if !defined(__LP64__)
  if (has_text_relocations) {
    // All relocations are done, we can protect our segments back to read-only.
    if (phdr_table_protect_segments(phdr, phnum, load_bias) < 0) {
      DL_ERR("can't protect segments for \"%s\": %s",
             get_realpath(), strerror(errno));
      return false;
    }
  }
#endif

  // We can also turn on GNU RELRO protection if we're not linking the dynamic linker
  // itself --- it can't make system calls yet, and will have to call protect_relro later.
  if (!is_linker() && !protect_relro()) {
    return false;
  }

  /* Handle serializing/sharing the RELRO segment */
  if (extinfo && (extinfo->flags & ANDROID_DLEXT_WRITE_RELRO)) {
    if (phdr_table_serialize_gnu_relro(phdr, phnum, load_bias,
                                       extinfo->relro_fd) < 0) {
      DL_ERR("failed serializing GNU RELRO section for \"%s\": %s",
             get_realpath(), strerror(errno));
      return false;
    }
  } else if (extinfo && (extinfo->flags & ANDROID_DLEXT_USE_RELRO)) {
    if (phdr_table_map_gnu_relro(phdr, phnum, load_bias,
                                 extinfo->relro_fd) < 0) {
      DL_ERR("failed mapping GNU RELRO section for \"%s\": %s",
             get_realpath(), strerror(errno));
      return false;
    }
  }

  notify_gdb_of_load(this);
  return true;
}

bool soinfo::protect_relro() {
  if (phdr_table_protect_gnu_relro(phdr, phnum, load_bias) < 0) {
    DL_ERR("can't enable GNU RELRO protection for \"%s\": %s",
           get_realpath(), strerror(errno));
    return false;
  }
  return true;
}

/*
 * This function add vdso to internal dso list.
 * It helps to stack unwinding through signal handlers.
 * Also, it makes bionic more like glibc.
 */
static void add_vdso(KernelArgumentBlock& args) {
#if defined(AT_SYSINFO_EHDR)
  ElfW(Ehdr)* ehdr_vdso = reinterpret_cast<ElfW(Ehdr)*>(args.getauxval(AT_SYSINFO_EHDR));
  if (ehdr_vdso == nullptr) {
    return;
  }

  soinfo* si = soinfo_alloc(g_default_namespace, "[vdso]", nullptr, 0, 0);

  si->phdr = reinterpret_cast<ElfW(Phdr)*>(reinterpret_cast<char*>(ehdr_vdso) + ehdr_vdso->e_phoff);
  si->phnum = ehdr_vdso->e_phnum;
  si->base = reinterpret_cast<ElfW(Addr)>(ehdr_vdso);
  si->size = phdr_table_get_load_size(si->phdr, si->phnum);
  si->load_bias = get_elf_exec_load_bias(ehdr_vdso);

  si->prelink_image();
  si->link_image(g_empty_list, soinfo::soinfo_list_t::make_list(si), nullptr);
#endif
}

/* gdb expects the linker to be in the debug shared object list.
 * Without this, gdb has trouble locating the linker's ".text"
 * and ".plt" sections. Gdb could also potentially use this to
 * relocate the offset of our exported 'rtld_db_dlactivity' symbol.
 * Note that the linker shouldn't be on the soinfo list.
 */
static void init_linker_info_for_gdb(ElfW(Addr) linker_base) {
  static link_map linker_link_map_for_gdb;
#if defined(__LP64__)
  static char kLinkerPath[] = "/system/bin/linker64";
#else
  static char kLinkerPath[] = "/system/bin/linker";
#endif

  linker_link_map_for_gdb.l_addr = linker_base;
  linker_link_map_for_gdb.l_name = kLinkerPath;

  /*
   * Set the dynamic field in the link map otherwise gdb will complain with
   * the following:
   *   warning: .dynamic section for "/system/bin/linker" is not at the
   *   expected address (wrong library or version mismatch?)
   */
  ElfW(Ehdr)* elf_hdr = reinterpret_cast<ElfW(Ehdr)*>(linker_base);
  ElfW(Phdr)* phdr = reinterpret_cast<ElfW(Phdr)*>(linker_base + elf_hdr->e_phoff);
  phdr_table_get_dynamic_section(phdr, elf_hdr->e_phnum, linker_base,
                                 &linker_link_map_for_gdb.l_ld, nullptr);

  insert_link_map_into_debug_map(&linker_link_map_for_gdb);
}

static void init_default_namespace() {
  g_default_namespace->set_name("(default)");
  g_default_namespace->set_isolated(false);

  const char *interp = phdr_table_get_interpreter_name(somain->phdr, somain->phnum,
                                                       somain->load_bias);
  const char* bname = basename(interp);
  if (bname && (strcmp(bname, "linker_asan") == 0 || strcmp(bname, "linker_asan64") == 0)) {
    g_default_ld_paths = kAsanDefaultLdPaths;
    g_is_asan = true;
  } else {
    g_default_ld_paths = kDefaultLdPaths;
  }

  char real_path[PATH_MAX];
  std::vector<std::string> ld_default_paths;
  for (size_t i = 0; g_default_ld_paths[i] != nullptr; ++i) {
    if (realpath(g_default_ld_paths[i], real_path) != nullptr) {
      ld_default_paths.push_back(real_path);
    } else {
      ld_default_paths.push_back(g_default_ld_paths[i]);
    }
  }

  g_default_namespace->set_default_library_paths(std::move(ld_default_paths));
};

extern "C" int __system_properties_init(void);

static const char* get_executable_path() {
  static std::string executable_path;
  if (executable_path.empty()) {
    char path[PATH_MAX];
    ssize_t path_len = readlink("/proc/self/exe", path, sizeof(path));
    if (path_len == -1 || path_len >= static_cast<ssize_t>(sizeof(path))) {
      __libc_fatal("readlink('/proc/self/exe') failed: %s", strerror(errno));
    }
    executable_path = std::string(path, path_len);
  }

  return executable_path.c_str();
}

/*
 * This code is called after the linker has linked itself and
 * fixed it's own GOT. It is safe to make references to externs
 * and other non-local data at this point.
 */
static ElfW(Addr) __linker_init_post_relocation(KernelArgumentBlock& args, ElfW(Addr) linker_base) {
#if TIMING
  struct timeval t0, t1;
  gettimeofday(&t0, 0);
#endif

#ifdef DISABLED_FOR_HYBRIS_SUPPORT
  // Sanitize the environment.
  __libc_init_AT_SECURE(args);

  // Initialize system properties
  __system_properties_init(); // may use 'environ'
#endif

#ifdef DISABLED_FOR_HYBRIS_SUPPORT
  debuggerd_init();
#endif

  // Get a few environment variables.
  const char* LD_DEBUG = getenv("HYBRIS_LD_DEBUG");
  if (LD_DEBUG != nullptr) {
    g_ld_debug_verbosity = atoi(LD_DEBUG);
  }

#if defined(__LP64__)
  INFO("[ Android dynamic linker (64-bit) ]");
#else
  INFO("[ Android dynamic linker (32-bit) ]");
#endif

  // These should have been sanitized by __libc_init_AT_SECURE, but the test
  // doesn't cost us anything.
  const char* ldpath_env = nullptr;
  const char* ldpreload_env = nullptr;
  const char* ldshim_libs_env = nullptr;
  if (!getauxval(AT_SECURE)) {
    ldpath_env = getenv("HYBRIS_LD_LIBRARY_PATH");
    if (ldpath_env != nullptr) {
      INFO("[ LD_LIBRARY_PATH set to \"%s\" ]", ldpath_env);
    }
    ldpreload_env = getenv("HYBRIS_LD_PRELOAD");
    if (ldpreload_env != nullptr) {
      INFO("[ LD_PRELOAD set to \"%s\" ]", ldpreload_env);
    }
    ldshim_libs_env = getenv("LD_SHIM_LIBS");
  }

  struct stat file_stat;
  // Stat "/proc/self/exe" instead of executable_path because
  // the executable could be unlinked by this point and it should
  // not cause a crash (see http://b/31084669)
  if (TEMP_FAILURE_RETRY(stat("/proc/self/exe", &file_stat)) != 0) {
    __libc_fatal("unable to stat \"/proc/self/exe\": %s", strerror(errno));
  }

  const char* executable_path = get_executable_path();
  soinfo* si = soinfo_alloc(g_default_namespace, executable_path, &file_stat, 0, RTLD_GLOBAL);
  if (si == nullptr) {
    __libc_fatal("Couldn't allocate soinfo: out of memory?");
  }

  /* bootstrap the link map, the main exe always needs to be first */
  si->set_main_executable();
  link_map* map = &(si->link_map_head);

  // Register the main executable and the linker upfront to have
  // gdb aware of them before loading the rest of the dependency
  // tree.
  map->l_addr = 0;
  map->l_name = const_cast<char*>(executable_path);
  insert_link_map_into_debug_map(map);
  init_linker_info_for_gdb(linker_base);

  // Extract information passed from the kernel.
  si->phdr = reinterpret_cast<ElfW(Phdr)*>(args.getauxval(AT_PHDR));
  si->phnum = args.getauxval(AT_PHNUM);
  si->entry = args.getauxval(AT_ENTRY);

  /* Compute the value of si->base. We can't rely on the fact that
   * the first entry is the PHDR because this will not be true
   * for certain executables (e.g. some in the NDK unit test suite)
   */
  si->base = 0;
  si->size = phdr_table_get_load_size(si->phdr, si->phnum);
  si->load_bias = 0;
  for (size_t i = 0; i < si->phnum; ++i) {
    if (si->phdr[i].p_type == PT_PHDR) {
      si->load_bias = reinterpret_cast<ElfW(Addr)>(si->phdr) - si->phdr[i].p_vaddr;
      si->base = reinterpret_cast<ElfW(Addr)>(si->phdr) - si->phdr[i].p_offset;
      break;
    }
  }
  si->dynamic = nullptr;

#ifdef ENABLE_NON_PIE_SUPPORT
  if (allow_non_pie(executable_path)) {
    DL_WARN("Non position independent executable (non PIE) allowed: %s",
            executable_path);
  } else {
#endif
  ElfW(Ehdr)* elf_hdr = reinterpret_cast<ElfW(Ehdr)*>(si->base);
  if (elf_hdr->e_type != ET_DYN) {
    fprintf(stderr, "\"%s\": error: only position independent executables (PIE) are supported.",
                 args.argv[0]);
  }
#ifdef ENABLE_NON_PIE_SUPPORT
  }
#endif

  // Use LD_LIBRARY_PATH and LD_PRELOAD (but only if we aren't setuid/setgid).
  if (ldpath_env)
    parse_LD_LIBRARY_PATH(ldpath_env);
  else
    parse_LD_LIBRARY_PATH(DEFAULT_HYBRIS_LD_LIBRARY_PATH);
  parse_LD_PRELOAD(ldpreload_env);
  parse_LD_SHIM_LIBS(ldshim_libs_env);

  somain = si;

  init_default_namespace();

  if (!si->prelink_image()) {
    fprintf(stderr, "CANNOT LINK EXECUTABLE \"%s\": %s", args.argv[0], linker_get_error_buffer());
  }

  // add somain to global group
  si->set_dt_flags_1(si->get_dt_flags_1() | DF_1_GLOBAL);

  // Load ld_preloads and dependencies.
  StringLinkedList needed_library_name_list;
  size_t needed_libraries_count = 0;
  size_t ld_preloads_count = 0;

  for (const auto& ld_preload_name : g_ld_preload_names) {
    needed_library_name_list.push_back(ld_preload_name.c_str());
    ++needed_libraries_count;
    ++ld_preloads_count;
  }

  for_each_dt_needed(si, [&](const char* name) {
    needed_library_name_list.push_back(name);
    ++needed_libraries_count;
  });

  const char* needed_library_names[needed_libraries_count];

  memset(needed_library_names, 0, sizeof(needed_library_names));
  needed_library_name_list.copy_to_array(needed_library_names, needed_libraries_count);

  if (needed_libraries_count > 0 &&
      !find_libraries(g_default_namespace, si, needed_library_names, needed_libraries_count,
                      nullptr, &g_ld_preloads, ld_preloads_count, RTLD_GLOBAL, nullptr,
                      /* add_as_children */ true)) {
    fprintf(stderr, "CANNOT LINK EXECUTABLE \"%s\": %s", args.argv[0], linker_get_error_buffer());
  } else if (needed_libraries_count == 0) {
    if (!si->link_image(g_empty_list, soinfo::soinfo_list_t::make_list(si), nullptr)) {
      fprintf(stderr, "CANNOT LINK EXECUTABLE \"%s\": %s", args.argv[0], linker_get_error_buffer());
    }
    si->increment_ref_count();
  }

  add_vdso(args);

  {
    ProtectedDataGuard guard;

    si->call_pre_init_constructors();

    /* After the prelink_image, the si->load_bias is initialized.
     * For so lib, the map->l_addr will be updated in notify_gdb_of_load.
     * We need to update this value for so exe here. So Unwind_Backtrace
     * for some arch like x86 could work correctly within so exe.
     */
    map->l_addr = si->load_bias;
    si->call_constructors();
  }

#if TIMING
  gettimeofday(&t1, nullptr);
  PRINT("LINKER TIME: %s: %d microseconds", args.argv[0], (int) (
           (((long long)t1.tv_sec * 1000000LL) + (long long)t1.tv_usec) -
           (((long long)t0.tv_sec * 1000000LL) + (long long)t0.tv_usec)));
#endif
#if STATS
  PRINT("RELO STATS: %s: %d abs, %d rel, %d copy, %d symbol", args.argv[0],
         linker_stats.count[kRelocAbsolute],
         linker_stats.count[kRelocRelative],
         linker_stats.count[kRelocCopy],
         linker_stats.count[kRelocSymbol]);
#endif
#if COUNT_PAGES
  {
    unsigned n;
    unsigned i;
    unsigned count = 0;
    for (n = 0; n < 4096; n++) {
      if (bitmask[n]) {
        unsigned x = bitmask[n];
#if defined(__LP64__)
        for (i = 0; i < 32; i++) {
#else
        for (i = 0; i < 8; i++) {
#endif
          if (x & 1) {
            count++;
          }
          x >>= 1;
        }
      }
    }
    PRINT("PAGES MODIFIED: %s: %d (%dKB)", args.argv[0], count, count * 4);
  }
#endif

#if TIMING || STATS || COUNT_PAGES
  fflush(stdout);
#endif

  TRACE("[ Ready to execute \"%s\" @ %p ]", si->get_realpath(), reinterpret_cast<void*>(si->entry));
  return si->entry;
}

/* Compute the load-bias of an existing executable. This shall only
 * be used to compute the load bias of an executable or shared library
 * that was loaded by the kernel itself.
 *
 * Input:
 *    elf    -> address of ELF header, assumed to be at the start of the file.
 * Return:
 *    load bias, i.e. add the value of any p_vaddr in the file to get
 *    the corresponding address in memory.
 */
static ElfW(Addr) get_elf_exec_load_bias(const ElfW(Ehdr)* elf) {
  ElfW(Addr) offset = elf->e_phoff;
  const ElfW(Phdr)* phdr_table =
      reinterpret_cast<const ElfW(Phdr)*>(reinterpret_cast<uintptr_t>(elf) + offset);
  const ElfW(Phdr)* phdr_end = phdr_table + elf->e_phnum;

  for (const ElfW(Phdr)* phdr = phdr_table; phdr < phdr_end; phdr++) {
    if (phdr->p_type == PT_LOAD) {
      return reinterpret_cast<ElfW(Addr)>(elf) + phdr->p_offset - phdr->p_vaddr;
    }
  }
  return 0;
}

static void __linker_cannot_link(KernelArgumentBlock& args) {
  __libc_fatal("CANNOT LINK EXECUTABLE \"%s\": %s", args.argv[0], linker_get_error_buffer());
}

#ifdef WANT_ARM_TRACING
extern "C" void android_linker_init(int sdk_version, void* (*get_hooked_symbol)(const char*, const char*), int enable_linker_gdb_support, void *(create_wrapper)(const char*, void*, int)) {
#else
extern "C" void android_linker_init(int sdk_version, void* (*get_hooked_symbol)(const char*, const char*), int enable_linker_gdb_support) {
#endif
  // Get a few environment variables.
  const char* LD_DEBUG = getenv("HYBRIS_LD_DEBUG");
  if (LD_DEBUG != nullptr) {
    g_ld_debug_verbosity = atoi(LD_DEBUG);
  }

  const char* ldpath_env = nullptr;
  const char* ldpreload_env = nullptr;
  if (!getauxval(AT_SECURE)) {
    ldpath_env = getenv("HYBRIS_LD_LIBRARY_PATH");
    ldpreload_env = getenv("HYBRIS_LD_PRELOAD");
  }

  if (ldpath_env)
    parse_LD_LIBRARY_PATH(ldpath_env);
  else
    parse_LD_LIBRARY_PATH(DEFAULT_HYBRIS_LD_LIBRARY_PATH);
  parse_LD_PRELOAD(ldpreload_env);

  if (sdk_version > 0)
    set_application_target_sdk_version(sdk_version);

  _get_hooked_symbol = get_hooked_symbol;
  _linker_enable_gdb_support = enable_linker_gdb_support;
#ifdef WANT_ARM_TRACING
  _create_wrapper = create_wrapper;
#endif
}

#ifdef DISABLED_FOR_HYBRIS_SUPPORT
/*
 * This is the entry point for the linker, called from begin.S. This
 * method is responsible for fixing the linker's own relocations, and
 * then calling __linker_init_post_relocation().
 *
 * Because this method is called before the linker has fixed it's own
 * relocations, any attempt to reference an extern variable, extern
 * function, or other GOT reference will generate a segfault.
 */
extern "C" ElfW(Addr) __linker_init(void* raw_args) {
  KernelArgumentBlock args(raw_args);

  ElfW(Addr) linker_addr = args.getauxval(AT_BASE);
  ElfW(Addr) entry_point = args.getauxval(AT_ENTRY);
  ElfW(Ehdr)* elf_hdr = reinterpret_cast<ElfW(Ehdr)*>(linker_addr);
  ElfW(Phdr)* phdr = reinterpret_cast<ElfW(Phdr)*>(linker_addr + elf_hdr->e_phoff);

  soinfo linker_so(nullptr, nullptr, nullptr, 0, 0);

  // If the linker is not acting as PT_INTERP entry_point is equal to
  // _start. Which means that the linker is running as an executable and
  // already linked by PT_INTERP.
  //
  // This happens when user tries to run 'adb shell /system/bin/linker'
  // see also https://code.google.com/p/android/issues/detail?id=63174
  if (reinterpret_cast<ElfW(Addr)>(&_start) == entry_point) {
    __libc_format_fd(STDOUT_FILENO,
                     "This is %s, the helper program for shared library executables.\n",
                     args.argv[0]);
    exit(0);
  }

  linker_so.base = linker_addr;
  linker_so.size = phdr_table_get_load_size(phdr, elf_hdr->e_phnum);
  linker_so.load_bias = get_elf_exec_load_bias(elf_hdr);
  linker_so.dynamic = nullptr;
  linker_so.phdr = phdr;
  linker_so.phnum = elf_hdr->e_phnum;
  linker_so.set_linker_flag();

  // Prelink the linker so we can access linker globals.
  if (!linker_so.prelink_image()) __linker_cannot_link(args);

  // This might not be obvious... The reasons why we pass g_empty_list
  // in place of local_group here are (1) we do not really need it, because
  // linker is built with DT_SYMBOLIC and therefore relocates its symbols against
  // itself without having to look into local_group and (2) allocators
  // are not yet initialized, and therefore we cannot use linked_list.push_*
  // functions at this point.
  if (!linker_so.link_image(g_empty_list, g_empty_list, nullptr)) __linker_cannot_link(args);

#if defined(__i386__)
  // On x86, we can't make system calls before this point.
  // We can't move this up because this needs to assign to a global.
  // Note that until we call __libc_init_main_thread below we have
  // no TLS, so you shouldn't make a system call that can fail, because
  // it will SEGV when it tries to set errno.
  __libc_init_sysinfo(args);
#endif

  // Initialize the main thread (including TLS, so system calls really work).
  __libc_init_main_thread(args);

  // We didn't protect the linker's RELRO pages in link_image because we
  // couldn't make system calls on x86 at that point, but we can now...
  if (!linker_so.protect_relro()) __linker_cannot_link(args);

  // Initialize the linker's static libc's globals
  __libc_init_globals(args);

  // Initialize the linker's own global variables
  linker_so.call_constructors();

  // Initialize static variables. Note that in order to
  // get correct libdl_info we need to call constructors
  // before get_libdl_info().
  solist = get_libdl_info();
  sonext = get_libdl_info();
  g_default_namespace->add_soinfo(get_libdl_info());

  // We have successfully fixed our own relocations. It's safe to run
  // the main part of the linker now.
  args.abort_message_ptr = &g_abort_message;
  ElfW(Addr) start_address = __linker_init_post_relocation(args, linker_addr);

  INFO("[ Jumping to _start (%p)... ]", reinterpret_cast<void*>(start_address));

  // Return the address that the calling assembly stub should jump to.
  return start_address;
}

#endif
