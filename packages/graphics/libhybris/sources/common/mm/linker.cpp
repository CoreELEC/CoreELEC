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
#include <dlfcn.h>
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

#include <new>
#include <string>
#include <vector>

// Private C library headers.
#include "private/bionic_tls.h"
#include "private/KernelArgumentBlock.h"
#include "private/ScopedPthreadMutexLocker.h"
#include "private/ScopeGuard.h"
#include "private/UniquePtr.h"

#include "linker.h"
#include "linker_block_allocator.h"
#include "linker_debug.h"
#include "linker_sleb128.h"
#include "linker_phdr.h"
#include "linker_relocs.h"
#include "linker_reloc_iterators.h"

#include "hybris_compat.h"

#ifdef WANT_ARM_TRACING
#include "../wrappers.h"
#endif

#ifdef DISABLED_FOR_HYBRIS_SUPPORT
extern void __libc_init_AT_SECURE(KernelArgumentBlock&);
#endif

// Override macros to use C++ style casts.
#undef ELF_ST_TYPE
#define ELF_ST_TYPE(x) (static_cast<uint32_t>(x) & 0xf)

static ElfW(Addr) get_elf_exec_load_bias(const ElfW(Ehdr)* elf);

static LinkerTypeAllocator<soinfo> g_soinfo_allocator;
static LinkerTypeAllocator<LinkedListEntry<soinfo>> g_soinfo_links_allocator;

static soinfo* solist = get_libdl_info();
static soinfo* sonext = get_libdl_info();
static soinfo* somain; // main process, always the one after libdl_info

static const char* const kDefaultLdPaths[] = {
#if defined(__LP64__)
  "/vendor/lib64",
  "/system/lib64",
  "/odm/lib64",
#else
  "/vendor/lib",
  "/system/lib",
  "/odm/lib",
#endif
  nullptr
};

static const ElfW(Versym) kVersymNotNeeded = 0;
static const ElfW(Versym) kVersymGlobal = 1;

static std::vector<std::string> g_ld_library_paths;
static std::vector<std::string> g_ld_preload_names;

static std::vector<soinfo*> g_ld_preloads;

int g_ld_debug_verbosity = 0;

abort_msg_t* g_abort_message = nullptr; // For debuggerd.

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

static char __linker_dl_err_buf[768];

char* linker_get_error_buffer() {
  return &__linker_dl_err_buf[0];
}

size_t linker_get_error_buffer_size() {
  return sizeof(__linker_dl_err_buf);
}

// This function is an empty stub where GDB locates a breakpoint to get notified
// about linker activity.
extern "C"
void __attribute__((noinline)) __attribute__((visibility("default"))) rtld_db_dlactivity();

static pthread_mutex_t g__r_debug_mutex = PTHREAD_MUTEX_INITIALIZER;
r_debug _r_debug =
    {1, nullptr, reinterpret_cast<uintptr_t>(&rtld_db_dlactivity), r_debug::RT_CONSISTENT, 0};

static link_map* r_debug_head = 0;

static void* (*_get_hooked_symbol)(const char *sym, const char *requester);

static int _linker_enable_gdb_support = 0;

#ifdef WANT_ARM_TRACING
void *(*_create_wrapper)(const char *symbol, void *function, int wrapper_type);
#endif

static void insert_soinfo_into_debug_map(soinfo* info) {
  if (!_linker_enable_gdb_support) return;
    
  // Copy the necessary fields into the debug structure.
  link_map* map = &(info->link_map_head);
  map->l_addr = info->load_bias;
  // link_map l_name field is not const.
  map->l_name = const_cast<char*>(info->get_realpath());
  map->l_ld = info->dynamic;

  // Stick the new library at the end of the list.
  // gdb tends to care more about libc than it does
  // about leaf libraries, and ordering it this way
  // reduces the back-and-forth over the wire.

  ///// PATCHED: we don't want libhybris modifying glibc's
  /////          link_map objects, which should not be linked
  /////          to bionic's stripped link_map objects.
  /////        ==> make a copy of the whole chain
  if(r_debug_head == 0 && _r_debug.r_map != 0) {
    link_map *glibc_link_map = new link_map(*_r_debug.r_map);
    r_debug_head = glibc_link_map;

    while(glibc_link_map->l_next != 0) {
      link_map *copy_next_link_map = new link_map(*glibc_link_map->l_next);
      glibc_link_map->l_next = copy_next_link_map;
      copy_next_link_map->l_prev = glibc_link_map;

      glibc_link_map = copy_next_link_map;
    }
  }

  if (r_debug_head != 0) {
    r_debug_head->l_prev = map;
    map->l_next = r_debug_head;
    map->l_prev = 0;
  } else {
    _r_debug.r_map = map;
    map->l_prev = 0;
    map->l_next = 0;
  }
  _r_debug.r_map = r_debug_head = map;
}

static void remove_soinfo_from_debug_map(soinfo* info) {
  if (!_linker_enable_gdb_support) return;
  
  link_map* map = &(info->link_map_head);

  if (r_debug_head == map) {
    r_debug_head = map->l_prev;
  }

  if (map->l_prev) {
    map->l_prev->l_next = map->l_next;
  }
  if (map->l_next) {
    map->l_next->l_prev = map->l_prev;
  }
}

static void notify_gdb_of_load(soinfo* info) {
  if (info->is_main_executable()) {
    // GDB already knows about the main executable
    return;
  }

  ScopedPthreadMutexLocker locker(&g__r_debug_mutex);

  _r_debug.r_state = r_debug::RT_ADD;
  rtld_db_dlactivity();

  insert_soinfo_into_debug_map(info);

  _r_debug.r_state = r_debug::RT_CONSISTENT;
  rtld_db_dlactivity();
}

static void notify_gdb_of_unload(soinfo* info) {
  if (info->is_main_executable()) {
    // GDB already knows about the main executable
    return;
  }

  ScopedPthreadMutexLocker locker(&g__r_debug_mutex);

  _r_debug.r_state = r_debug::RT_DELETE;
  rtld_db_dlactivity();

  remove_soinfo_from_debug_map(info);

  _r_debug.r_state = r_debug::RT_CONSISTENT;
  rtld_db_dlactivity();
}

void notify_gdb_of_libraries() {
  _r_debug.r_state = r_debug::RT_ADD;
  rtld_db_dlactivity();
  _r_debug.r_state = r_debug::RT_CONSISTENT;
  rtld_db_dlactivity();
}

LinkedListEntry<soinfo>* SoinfoListAllocator::alloc() {
  return g_soinfo_links_allocator.alloc();
}

void SoinfoListAllocator::free(LinkedListEntry<soinfo>* entry) {
  g_soinfo_links_allocator.free(entry);
}

static soinfo* soinfo_alloc(const char* name, struct stat* file_stat,
                            off64_t file_offset, uint32_t rtld_flags) {
  if (strlen(name) >= PATH_MAX) {
    DL_ERR("library name \"%s\" too long", name);
    return nullptr;
  }

  soinfo* si = new (g_soinfo_allocator.alloc()) soinfo(name, file_stat, file_offset, rtld_flags);

  sonext->next = si;
  sonext = si;

  TRACE("name %s: allocated soinfo @ %p", name, si);
  return si;
}

static void soinfo_free(soinfo* si) {
  if (si == nullptr) {
    return;
  }

  if (si->base != 0 && si->size != 0) {
    munmap(reinterpret_cast<void*>(si->base), si->size);
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

static void parse_path(const char* path, const char* delimiters,
                       std::vector<std::string>* paths) {
  if (path == nullptr) {
    return;
  }

  paths->clear();

  for (const char *p = path; ; ++p) {
    size_t len = strcspn(p, delimiters);
    // skip empty tokens
    if (len == 0) {
      continue;
    }

    paths->push_back(std::string(p, len));
    p += len;

    if (*p == '\0') {
      break;
    }
  }
}

static void parse_LD_LIBRARY_PATH(const char* path) {
  parse_path(path, ":", &g_ld_library_paths);
}

static void parse_LD_PRELOAD(const char* path) {
  // We have historically supported ':' as well as ' ' in LD_PRELOAD.
  parse_path(path, " :", &g_ld_preload_names);
}

static bool realpath_fd(int fd, std::string* realpath) {
  std::vector<char> buf(PATH_MAX), proc_self_fd(PATH_MAX);
  snprintf(&proc_self_fd[0], proc_self_fd.size(), "/proc/self/fd/%d", fd);
  if (readlink(&proc_self_fd[0], &buf[0], buf.size()) == -1) {
    PRINT("readlink('%s') failed: %s [fd=%d]", &proc_self_fd[0], strerror(errno), fd);
    return false;
  }

  *realpath = std::string(&buf[0]);
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
    DL_WARN("unexpected ST_BIND value: %d for '%s' in '%s'",
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

soinfo::soinfo(const char* realpath, const struct stat* file_stat,
               off64_t file_offset, int rtld_flags) {

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
}


uint32_t SymbolName::elf_hash() {
  if (!has_elf_hash_) {
    const uint8_t* name = reinterpret_cast<const uint8_t*>(name_);
    uint32_t h = 0, g;

    while (*name) {
      h = (h << 4) + *name++;
      g = h & 0xf0000000;
      h ^= g;
      h ^= g >> 24;
    }

    elf_hash_ = h;
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
      TypeBasedAllocator<LoadTask>::free(t);
    }
  };

  typedef UniquePtr<LoadTask, deleter_t> unique_ptr;

  static deleter_t deleter;

  static LoadTask* create(const char* name, soinfo* needed_by) {
    LoadTask* ptr = TypeBasedAllocator<LoadTask>::alloc();
    return new (ptr) LoadTask(name, needed_by);
  }

  const char* get_name() const {
    return name_;
  }

  soinfo* get_needed_by() const {
    return needed_by_;
  }
 private:
  LoadTask(const char* name, soinfo* needed_by)
    : name_(name), needed_by_(needed_by) {}

  const char* name_;
  soinfo* needed_by_;

  DISALLOW_IMPLICIT_CONSTRUCTORS(LoadTask);
};

LoadTask::deleter_t LoadTask::deleter;

template <typename T>
using linked_list_t = LinkedList<T, TypeBasedAllocator<LinkedListEntry<T>>>;

typedef linked_list_t<soinfo> SoinfoLinkedList;
typedef linked_list_t<const char> StringLinkedList;
typedef linked_list_t<LoadTask> LoadTaskList;


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
                                            soinfo** found, SymbolName& symbol_name) {
  const ElfW(Sym)* result = nullptr;
  bool skip_lookup = skip_until != nullptr;

  walk_dependencies_tree(&root, 1, [&](soinfo* current_soinfo) {
    if (skip_lookup) {
      skip_lookup = current_soinfo != skip_until;
      return true;
    }

    if (!current_soinfo->find_symbol_by_name(symbol_name, nullptr, &result)) {
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

// This is used by dlsym(3).  It performs symbol lookup only within the
// specified soinfo object and its dependencies in breadth first order.
const ElfW(Sym)* dlsym_handle_lookup(soinfo* si, soinfo** found, const char* name) {
  // According to man dlopen(3) and posix docs in the case when si is handle
  // of the main executable we need to search not only in the executable and its
  // dependencies but also in all libraries loaded with RTLD_GLOBAL.
  //
  // Since RTLD_GLOBAL is always set for the main executable and all dt_needed shared
  // libraries and they are loaded in breath-first (correct) order we can just execute
  // dlsym(RTLD_DEFAULT, ...); instead of doing two stage lookup.
  if (si == somain) {
    return dlsym_linear_lookup(name, found, nullptr, RTLD_DEFAULT);
  }

  SymbolName symbol_name(name);
  return dlsym_handle_lookup(si, nullptr, found, symbol_name);
}

/* This is used by dlsym(3) to performs a global symbol lookup. If the
   start value is null (for RTLD_DEFAULT), the search starts at the
   beginning of the global solist. Otherwise the search starts at the
   specified soinfo (for RTLD_NEXT).
 */
const ElfW(Sym)* dlsym_linear_lookup(const char* name,
                                     soinfo** found,
                                     soinfo* caller,
                                     void* handle) {
  SymbolName symbol_name(name);

  soinfo* start = solist;

  if (handle == RTLD_NEXT) {
    if (caller == nullptr) {
      return nullptr;
    } else {
      start = caller->next;
    }
  }

  const ElfW(Sym)* s = nullptr;
  for (soinfo* si = start; si != nullptr; si = si->next) {
    // Do not skip RTLD_LOCAL libraries in dlsym(RTLD_DEFAULT, ...)
    // if the library is opened by application with target api level <= 22
    // See http://b/21565766
    if ((si->get_rtld_flags() & RTLD_GLOBAL) == 0 && si->get_target_sdk_version() > 22) {
      continue;
    }

    if (!si->find_symbol_by_name(symbol_name, nullptr, &s)) {
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
        (handle == RTLD_NEXT) ? caller : nullptr, found, symbol_name);
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

static bool format_path(char* buf, size_t buf_size, const char* path, const char* name) {
  int n = snprintf(buf, buf_size, "%s/%s", path, name);
  if (n < 0 || n >= static_cast<int>(buf_size)) {
    PRINT("Warning: ignoring very long library path: %s/%s", path, name);
    return false;
  }

  return true;
}

static int open_library_on_default_path(const char* name, off64_t* file_offset) {
  for (size_t i = 0; kDefaultLdPaths[i] != nullptr; ++i) {
    char buf[512];
    if (!format_path(buf, sizeof(buf), kDefaultLdPaths[i], name)) {
      continue;
    }

    int fd = TEMP_FAILURE_RETRY(open(buf, O_RDONLY | O_CLOEXEC));
    if (fd != -1) {
      *file_offset = 0;
      return fd;
    }
  }

  return -1;
}

static int open_library_on_ld_library_path(const char* name, off64_t* file_offset) {
  for (const auto& path_str : g_ld_library_paths) {
    char buf[512];
    const char* const path = path_str.c_str();
    if (!format_path(buf, sizeof(buf), path, name)) {
      continue;
    }

    int fd = -1;
    if (fd == -1) {
      fd = TEMP_FAILURE_RETRY(open(buf, O_RDONLY | O_CLOEXEC));
      if (fd != -1) {
        *file_offset = 0;
      }
    }

    if (fd != -1) {
      return fd;
    }
  }

  return -1;
}

static int open_library(const char* name, off64_t* file_offset) {
  TRACE("[ opening %s ]", name);

  // If the name contains a slash, we should attempt to open it directly and not search the paths.
  if (strchr(name, '/') != nullptr) {
    int fd = TEMP_FAILURE_RETRY(open(name, O_RDONLY | O_CLOEXEC));
    //... But if it's not there, failback to search it in all other library paths
    if (fd != -1) {
      *file_offset = 0;
      return fd;
    }
  }

  // Otherwise we try LD_LIBRARY_PATH first, and fall back to the built-in well known paths.
  int fd = open_library_on_ld_library_path(name, file_offset);
  if (fd == -1) {
    fd = open_library_on_default_path(name, file_offset);
  }
  return fd;
}

static const char* fix_dt_needed(const char* dt_needed, const char* sopath) {
  (void) sopath;
#if !defined(__LP64__)
  // Work around incorrect DT_NEEDED entries for old apps: http://b/21364029
  if (get_application_target_sdk_version() <= 22) {
    const char* bname = basename(dt_needed);
    if (bname != dt_needed) {
      DL_WARN("'%s' library has invalid DT_NEEDED entry '%s'", sopath, dt_needed);
    }

    return bname;
  }
#endif
  return dt_needed;
}

template<typename F>
static void for_each_dt_needed(const soinfo* si, F action) {
  for (ElfW(Dyn)* d = si->dynamic; d->d_tag != DT_NULL; ++d) {
    if (d->d_tag == DT_NEEDED) {
      action(fix_dt_needed(si->get_string(d->d_un.d_val), si->get_realpath()));
    }
  }
}

static soinfo* load_library(int fd, off64_t file_offset,
                            LoadTaskList& load_tasks,
                            const char* name, int rtld_flags,
                            const android_dlextinfo* extinfo) {
  if ((file_offset % PAGE_SIZE) != 0) {
    DL_ERR("file offset for the library \"%s\" is not page-aligned: %" PRId64, name, file_offset);
    return nullptr;
  }
  if (file_offset < 0) {
    DL_ERR("file offset for the library \"%s\" is negative: %" PRId64, name, file_offset);
    return nullptr;
  }

  struct stat file_stat;
  if (TEMP_FAILURE_RETRY(fstat(fd, &file_stat)) != 0) {
    DL_ERR("unable to stat file for the library \"%s\": %s", name, strerror(errno));
    return nullptr;
  }
  if (file_offset >= file_stat.st_size) {
    DL_ERR("file offset for the library \"%s\" >= file size: %" PRId64 " >= %" PRId64,
        name, file_offset, file_stat.st_size);
    return nullptr;
  }

  // Check for symlink and other situations where
  // file can have different names, unless ANDROID_DLEXT_FORCE_LOAD is set
  if (extinfo == nullptr || (extinfo->flags & ANDROID_DLEXT_FORCE_LOAD) == 0) {
    for (soinfo* si = solist; si != nullptr; si = si->next) {
      if (si->get_st_dev() != 0 &&
          si->get_st_ino() != 0 &&
          si->get_st_dev() == file_stat.st_dev &&
          si->get_st_ino() == file_stat.st_ino &&
          si->get_file_offset() == file_offset) {
        TRACE("library \"%s\" is already loaded under different name/path \"%s\" - "
            "will return existing soinfo", name, si->get_realpath());
        return si;
      }
    }
  }

  if ((rtld_flags & RTLD_NOLOAD) != 0) {
    DL_ERR("library \"%s\" wasn't loaded and RTLD_NOLOAD prevented it", name);
    return nullptr;
  }

  std::string realpath = name;
  if (!realpath_fd(fd, &realpath)) {
    PRINT("warning: unable to get realpath for the library \"%s\". Will use given name.", name);
    realpath = name;
  }

  // Read the ELF header and load the segments.
  ElfReader elf_reader(realpath.c_str(), fd, file_offset, file_stat.st_size);
  if (!elf_reader.Load(extinfo)) {
    return nullptr;
  }

  soinfo* si = soinfo_alloc(realpath.c_str(), &file_stat, file_offset, rtld_flags);
  if (si == nullptr) {
    return nullptr;
  }
  si->base = elf_reader.load_start();
  si->size = elf_reader.load_size();
  si->load_bias = elf_reader.load_bias();
  si->phnum = elf_reader.phdr_count();
  si->phdr = elf_reader.loaded_phdr();

  if (!si->prelink_image()) {
    soinfo_free(si);
    return nullptr;
  }

  for_each_dt_needed(si, [&] (const char* name) {
    load_tasks.push_back(LoadTask::create(name, si));
  });

  return si;
}

static soinfo* load_library(LoadTaskList& load_tasks,
                            const char* name, int rtld_flags,
                            const android_dlextinfo* extinfo) {
  if (extinfo != nullptr && (extinfo->flags & ANDROID_DLEXT_USE_LIBRARY_FD) != 0) {
    off64_t file_offset = 0;
    if ((extinfo->flags & ANDROID_DLEXT_USE_LIBRARY_FD_OFFSET) != 0) {
      file_offset = extinfo->library_fd_offset;
    }
    return load_library(extinfo->library_fd, file_offset, load_tasks, name, rtld_flags, extinfo);
  }

  // Open the file.
  off64_t file_offset;
  int fd = open_library(name, &file_offset);
  if (fd == -1) {
    DL_ERR("library \"%s\" not found", name);
    return nullptr;
  }
  soinfo* result = load_library(fd, file_offset, load_tasks, name, rtld_flags, extinfo);
  close(fd);
  return result;
}

// Returns true if library was found and false in 2 cases
// 1. The library was found but loaded under different target_sdk_version
//    (*candidate != nullptr)
// 2. The library was not found by soname (*candidate is nullptr)
static bool find_loaded_library_by_soname(const char* name, soinfo** candidate) {
  *candidate = nullptr;

  // Ignore filename with path.
  if (strchr(name, '/') != nullptr) {
    return false;
  }

  uint32_t target_sdk_version = get_application_target_sdk_version();

  for (soinfo* si = solist; si != nullptr; si = si->next) {
    const char* soname = si->get_soname();
    if (soname != nullptr && (strcmp(name, soname) == 0)) {
      // If the library was opened under different target sdk version
      // skip this step and try to reopen it. The exceptions are
      // "libdl.so" and global group. There is no point in skipping
      // them because relocation process is going to use them
      // in any case.
      bool is_libdl = si == solist;
      if (is_libdl || (si->get_dt_flags_1() & DF_1_GLOBAL) != 0 ||
          !si->is_linked() || si->get_target_sdk_version() == target_sdk_version) {
        *candidate = si;
        return true;
      } else if (*candidate == nullptr) {
        // for the different sdk version - remember the first library.
        *candidate = si;
      }
    }
  }

  return false;
}

static soinfo* find_library_internal(LoadTaskList& load_tasks, const char* name,
                                     int rtld_flags, const android_dlextinfo* extinfo) {
  soinfo* candidate;

  if (find_loaded_library_by_soname(name, &candidate)) {
    return candidate;
  }

  // Library might still be loaded, the accurate detection
  // of this fact is done by load_library.
  TRACE("[ '%s' find_loaded_library_by_soname returned false (*candidate=%s@%p). Trying harder...]",
      name, candidate == nullptr ? "n/a" : candidate->get_realpath(), candidate);

  soinfo* si = load_library(load_tasks, name, rtld_flags, extinfo);

  // In case we were unable to load the library but there
  // is a candidate loaded under the same soname but different
  // sdk level - return it anyways.
  if (si == nullptr && candidate != nullptr) {
    si = candidate;
  }

  return si;
}

static void soinfo_unload(soinfo* si);

// TODO: this is slightly unusual way to construct
// the global group for relocation. Not every RTLD_GLOBAL
// library is included in this group for backwards-compatibility
// reasons.
//
// This group consists of the main executable, LD_PRELOADs
// and libraries with the DF_1_GLOBAL flag set.
static soinfo::soinfo_list_t make_global_group() {
  soinfo::soinfo_list_t global_group;
  for (soinfo* si = somain; si != nullptr; si = si->next) {
    if ((si->get_dt_flags_1() & DF_1_GLOBAL) != 0) {
      global_group.push_back(si);
    }
  }

  return global_group;
}

static bool find_libraries(soinfo* start_with, const char* const library_names[],
      size_t library_names_count, soinfo* soinfos[], std::vector<soinfo*>* ld_preloads,
      size_t ld_preloads_count, int rtld_flags, const android_dlextinfo* extinfo) {
  // Step 0: prepare.
  LoadTaskList load_tasks;
  for (size_t i = 0; i < library_names_count; ++i) {
    const char* name = library_names[i];
    load_tasks.push_back(LoadTask::create(name, start_with));
  }

  // Construct global_group.
  soinfo::soinfo_list_t global_group = make_global_group();

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

  auto failure_guard = make_scope_guard([&]() {
    // Housekeeping
    load_tasks.for_each([] (LoadTask* t) {
      LoadTask::deleter(t);
    });

    for (size_t i = 0; i<soinfos_count; ++i) {
      soinfo_unload(soinfos[i]);
    }
  });

  // Step 1: load and pre-link all DT_NEEDED libraries in breadth first order.
  for (LoadTask::unique_ptr task(load_tasks.pop_front());
      task.get() != nullptr; task.reset(load_tasks.pop_front())) {
    soinfo* si = find_library_internal(load_tasks, task->get_name(), rtld_flags, extinfo);
    if (si == nullptr) {
      return false;
    }

    soinfo* needed_by = task->get_needed_by();

    if (needed_by != nullptr) {
      needed_by->add_child(si);
    }

    if (si->is_linked()) {
      si->increment_ref_count();
    }

    // When ld_preloads is not null, the first
    // ld_preloads_count libs are in fact ld_preloads.
    if (ld_preloads != nullptr && soinfos_count < ld_preloads_count) {
      // Add LD_PRELOADed libraries to the global group for future runs.
      // There is no need to explicitly add them to the global group
      // for this run because they are going to appear in the local
      // group in the correct order.
      si->set_dt_flags_1(si->get_dt_flags_1() | DF_1_GLOBAL);
      ld_preloads->push_back(si);
    }

    if (soinfos_count < library_names_count) {
      soinfos[soinfos_count++] = si;
    }
  }

  // Step 2: link libraries.
  soinfo::soinfo_list_t local_group;
  walk_dependencies_tree(
      start_with == nullptr ? soinfos : &start_with,
      start_with == nullptr ? soinfos_count : 1,
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
      si->set_linked();
    }

    return true;
  });

  if (linked) {
    failure_guard.disable();
  }

  if (!was_local_group_root_linked) {
    local_group.front()->increment_ref_count();
  }

  return linked;
}

static soinfo* find_library(const char* name, int rtld_flags, const android_dlextinfo* extinfo) {
  soinfo* si;

  if (name == nullptr) {
    si = somain;
  } else if (!find_libraries(nullptr, &name, 1, &si, nullptr, 0, rtld_flags, extinfo)) {
    return nullptr;
  }

  return si;
}

static void soinfo_unload(soinfo* root) {
  // Note that the library can be loaded but not linked;
  // in which case there is no root but we still need
  // to walk the tree and unload soinfos involved.
  //
  // This happens on unsuccessful dlopen, when one of
  // the DT_NEEDED libraries could not be linked/found.
  if (root->is_linked()) {
    root = root->get_local_group_root();
  }

  if (!root->can_unload()) {
    TRACE("not unloading '%s' - the binary is flagged with NODELETE", root->get_realpath());
    return;
  }

  size_t ref_count = root->is_linked() ? root->decrement_ref_count() : 0;

  if (ref_count == 0) {
    soinfo::soinfo_list_t local_unload_list;
    soinfo::soinfo_list_t external_unload_list;
    soinfo::soinfo_list_t depth_first_list;
    depth_first_list.push_back(root);
    soinfo* si = nullptr;

    while ((si = depth_first_list.pop_front()) != nullptr) {
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
            depth_first_list.push_front(child);
          }
        }
      } else {
#if !defined(__work_around_b_19059885__)
        __libc_fatal("soinfo for \"%s\"@%p has no version", si->get_realpath(), si);
#else
        PRINT("warning: soinfo for \"%s\"@%p has no version", si->get_realpath(), si);
        for_each_dt_needed(si, [&] (const char* library_name) {
          TRACE("deprecated (old format of soinfo): %s needs to unload %s",
              si->get_realpath(), library_name);

          soinfo* needed = find_library(library_name, RTLD_NOLOAD, nullptr);
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
            depth_first_list.push_front(needed);
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
  } else {
    TRACE("not unloading '%s' group, decrementing ref_count to %zd",
        root->get_realpath(), ref_count);
  }
}

void do_android_get_LD_LIBRARY_PATH(char* buffer, size_t buffer_size) {
  // Use basic string manipulation calls to avoid snprintf.
  // snprintf indirectly calls pthread_getspecific to get the size of a buffer.
  // When debug malloc is enabled, this call returns 0. This in turn causes
  // snprintf to do nothing, which causes libraries to fail to load.
  // See b/17302493 for further details.
  // Once the above bug is fixed, this code can be modified to use
  // snprintf again.
  size_t required_len = strlen(kDefaultLdPaths[0]) + strlen(kDefaultLdPaths[1]) + 2;
  if (buffer_size < required_len) {
    __libc_fatal("android_get_LD_LIBRARY_PATH failed, buffer too small: "
                 "buffer len %zu, required len %zu", buffer_size, required_len);
  }
  char* end = stpcpy(buffer, kDefaultLdPaths[0]);
  *end = ':';
  strcpy(end + 1, kDefaultLdPaths[1]);
}

void do_android_update_LD_LIBRARY_PATH(const char* ld_library_path) {
  parse_LD_LIBRARY_PATH(ld_library_path);
}

soinfo* do_dlopen(const char* name, int flags, const android_dlextinfo* extinfo) {
  if ((flags & ~(RTLD_NOW|RTLD_LAZY|RTLD_LOCAL|RTLD_GLOBAL|RTLD_NODELETE|RTLD_NOLOAD)) != 0) {
    DL_ERR("invalid flags to dlopen: %x", flags);
    return nullptr;
  }
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
  }

  ProtectedDataGuard guard;
  soinfo* si = find_library(name, flags, extinfo);
  if (si != nullptr) {
    si->call_constructors();
  }
  return si;
}

void do_dlclose(soinfo* si) {
  ProtectedDataGuard guard;
  soinfo_unload(si);
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
static ElfW(Addr) get_addend(ElfW(Rela)* rela, ElfW(Addr) reloc_addr ) {
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

    DEBUG("Processing '%s' relocation at index %zd", get_realpath(), idx);
    if (type == R_GENERIC_NONE) {
      continue;
    }

    const ElfW(Sym)* s = nullptr;
    soinfo* lsi = nullptr;

    if (sym != 0) {
      sym_name = get_string(symtab_[sym].st_name);
      const version_info* vi = nullptr;

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
                   reloc, (sym_addr + addend), sym_name);
        *reinterpret_cast<ElfW(Addr)*>(reloc) += (sym_addr + addend);
        break;
      case R_AARCH64_ABS32:
        count_relocation(kRelocAbsolute);
        MARK(rel->r_offset);
        TRACE_TYPE(RELO, "RELO ABS32 %16" PRIx64 " <- %16" PRIx64 " %s\n",
                   reloc, (sym_addr + addend), sym_name);
        {
          const ElfW(Addr) reloc_value = *reinterpret_cast<ElfW(Addr)*>(reloc);
          const ElfW(Addr) min_value = static_cast<ElfW(Addr)>(INT32_MIN);
          const ElfW(Addr) max_value = static_cast<ElfW(Addr)>(UINT32_MAX);
          if ((min_value <= (reloc_value + (sym_addr + addend))) &&
              ((reloc_value + (sym_addr + addend)) <= max_value)) {
            *reinterpret_cast<ElfW(Addr)*>(reloc) += (sym_addr + addend);
          } else {
            DL_ERR("0x%016" PRIx64 " out of range 0x%016" PRIx64 " to 0x%016" PRIx64,
                   (reloc_value + (sym_addr + addend)), min_value, max_value);
            return false;
          }
        }
        break;
      case R_AARCH64_ABS16:
        count_relocation(kRelocAbsolute);
        MARK(rel->r_offset);
        TRACE_TYPE(RELO, "RELO ABS16 %16" PRIx64 " <- %16" PRIx64 " %s\n",
                   reloc, (sym_addr + addend), sym_name);
        {
          const ElfW(Addr) reloc_value = *reinterpret_cast<ElfW(Addr)*>(reloc);
          const ElfW(Addr) min_value = static_cast<ElfW(Addr)>(INT16_MIN);
          const ElfW(Addr) max_value = static_cast<ElfW(Addr)>(UINT16_MAX);
          if ((min_value <= (reloc_value + (sym_addr + addend))) &&
              ((reloc_value + (sym_addr + addend)) <= max_value)) {
            *reinterpret_cast<ElfW(Addr)*>(reloc) += (sym_addr + addend);
          } else {
            DL_ERR("0x%016" PRIx64 " out of range 0x%016" PRIx64 " to 0x%016" PRIx64,
                   reloc_value + (sym_addr + addend), min_value, max_value);
            return false;
          }
        }
        break;
      case R_AARCH64_PREL64:
        count_relocation(kRelocRelative);
        MARK(rel->r_offset);
        TRACE_TYPE(RELO, "RELO REL64 %16" PRIx64 " <- %16" PRIx64 " - %16" PRIx64 " %s\n",
                   reloc, (sym_addr + addend), rel->r_offset, sym_name);
        *reinterpret_cast<ElfW(Addr)*>(reloc) += (sym_addr + addend) - rel->r_offset;
        break;
      case R_AARCH64_PREL32:
        count_relocation(kRelocRelative);
        MARK(rel->r_offset);
        TRACE_TYPE(RELO, "RELO REL32 %16" PRIx64 " <- %16" PRIx64 " - %16" PRIx64 " %s\n",
                   reloc, (sym_addr + addend), rel->r_offset, sym_name);
        {
          const ElfW(Addr) reloc_value = *reinterpret_cast<ElfW(Addr)*>(reloc);
          const ElfW(Addr) min_value = static_cast<ElfW(Addr)>(INT32_MIN);
          const ElfW(Addr) max_value = static_cast<ElfW(Addr)>(UINT32_MAX);
          if ((min_value <= (reloc_value + ((sym_addr + addend) - rel->r_offset))) &&
              ((reloc_value + ((sym_addr + addend) - rel->r_offset)) <= max_value)) {
            *reinterpret_cast<ElfW(Addr)*>(reloc) += ((sym_addr + addend) - rel->r_offset);
          } else {
            DL_ERR("0x%016" PRIx64 " out of range 0x%016" PRIx64 " to 0x%016" PRIx64,
                   reloc_value + ((sym_addr + addend) - rel->r_offset), min_value, max_value);
            return false;
          }
        }
        break;
      case R_AARCH64_PREL16:
        count_relocation(kRelocRelative);
        MARK(rel->r_offset);
        TRACE_TYPE(RELO, "RELO REL16 %16" PRIx64 " <- %16" PRIx64 " - %16" PRIx64 " %s\n",
                   reloc, (sym_addr + addend), rel->r_offset, sym_name);
        {
          const ElfW(Addr) reloc_value = *reinterpret_cast<ElfW(Addr)*>(reloc);
          const ElfW(Addr) min_value = static_cast<ElfW(Addr)>(INT16_MIN);
          const ElfW(Addr) max_value = static_cast<ElfW(Addr)>(UINT16_MAX);
          if ((min_value <= (reloc_value + ((sym_addr + addend) - rel->r_offset))) &&
              ((reloc_value + ((sym_addr + addend) - rel->r_offset)) <= max_value)) {
            *reinterpret_cast<ElfW(Addr)*>(reloc) += ((sym_addr + addend) - rel->r_offset);
          } else {
            DL_ERR("0x%016" PRIx64 " out of range 0x%016" PRIx64 " to 0x%016" PRIx64,
                   reloc_value + ((sym_addr + addend) - rel->r_offset), min_value, max_value);
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
      case R_AARCH64_TLS_TPREL:
        TRACE_TYPE(RELO, "RELO TLS_TPREL *** %16" PRIx64 " <- %16" PRIx64 " - %16" PRIx64 "\n",
                   reloc, (sym_addr + addend), rel->r_offset);
        break;
      case R_AARCH64_TLS_DTPREL:
        TRACE_TYPE(RELO, "RELO TLS_DTPREL *** %16" PRIx64 " <- %16" PRIx64 " - %16" PRIx64 "\n",
                   reloc, (sym_addr + addend), rel->r_offset);
        break;
#elif defined(__x86_64__)
      case R_X86_64_32:
        count_relocation(kRelocRelative);
        MARK(rel->r_offset);
        TRACE_TYPE(RELO, "RELO R_X86_64_32 %08zx <- +%08zx %s", static_cast<size_t>(reloc),
                   static_cast<size_t>(sym_addr), sym_name);
        *reinterpret_cast<ElfW(Addr)*>(reloc) = sym_addr + addend;
        break;
      case R_X86_64_64:
        count_relocation(kRelocRelative);
        MARK(rel->r_offset);
        TRACE_TYPE(RELO, "RELO R_X86_64_64 %08zx <- +%08zx %s", static_cast<size_t>(reloc),
                   static_cast<size_t>(sym_addr), sym_name);
        *reinterpret_cast<ElfW(Addr)*>(reloc) = sym_addr + addend;
        break;
      case R_X86_64_PC32:
        count_relocation(kRelocRelative);
        MARK(rel->r_offset);
        TRACE_TYPE(RELO, "RELO R_X86_64_PC32 %08zx <- +%08zx (%08zx - %08zx) %s",
                   static_cast<size_t>(reloc), static_cast<size_t>(sym_addr - reloc),
                   static_cast<size_t>(sym_addr), static_cast<size_t>(reloc), sym_name);
        *reinterpret_cast<ElfW(Addr)*>(reloc) = sym_addr + addend - reloc;
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

void soinfo::call_array(const char* array_name , linker_function_t* functions,
                        size_t count, bool reverse) {
  if (functions == nullptr) {
    return;
  }

  TRACE("[ Calling %s (size %zd) @ %p for '%s' ]", array_name, count, functions, get_realpath());

  int begin = reverse ? (count - 1) : 0;
  int end = reverse ? -1 : count;
  int step = reverse ? -1 : 1;

  for (int i = begin; i != end; i += step) {
    TRACE("[ %s[%d] == %p ]", array_name, i, functions[i]);
    call_function("function", functions[i]);
  }

  TRACE("[ Done calling %s for '%s' ]", array_name, get_realpath());
}

void soinfo::call_function(const char* function_name , linker_function_t function) {
  if (function == nullptr || reinterpret_cast<uintptr_t>(function) == static_cast<uintptr_t>(-1)) {
    return;
  }

  TRACE("[ Calling %s @ %p for '%s' ]", function_name, function, get_realpath());
  function();
  TRACE("[ Done calling %s @ %p for '%s' ]", function_name, function, get_realpath());
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
    PRINT("\"%s\": ignoring %zd-entry DT_PREINIT_ARRAY in shared library!",
          get_realpath(), preinit_array_count_);
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

  // 2. Once everything untied - clear local lists.
  parents_.clear();
  children_.clear();
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

const char* soinfo::get_realpath() const {
#if defined(__work_around_b_19059885__)
  if (has_min_version(2)) {
    return realpath_.c_str();
  } else {
    return old_name_;
  }
#else
  return realpath_.c_str();
#endif
}

const char* soinfo::get_soname() const {
#if defined(__work_around_b_19059885__)
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
  return (get_rtld_flags() & (RTLD_NODELETE | RTLD_GLOBAL)) == 0;
}

bool soinfo::is_linked() const {
  return (flags_ & FLAG_LINKED) != 0;
}

bool soinfo::is_main_executable() const {
  return (flags_ & FLAG_EXE) != 0;
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

// This function returns api-level at the time of
// dlopen/load. Note that libraries opened by system
// will always have 'current' api level.
uint32_t soinfo::get_target_sdk_version() const {
  if (!has_min_version(2)) {
    return __ANDROID_API__;
  }

  return local_group_root_->target_sdk_version_;
}

bool soinfo::prelink_image() {
  /* Extract dynamic section */
  ElfW(Word) dynamic_flags = 0;
  phdr_table_get_dynamic_section(phdr, phnum, load_bias, &dynamic, &dynamic_flags);

  /* We can't log anything until the linker is relocated */
  bool relocating_linker = (flags_ & FLAG_LINKER) != 0;
  if (!relocating_linker) {
    INFO("[ linking %s ]", get_realpath());
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

      default:
        if (!relocating_linker) {
          DL_WARN("%s: unused DT entry: type %p arg %p", get_realpath(),
              reinterpret_cast<void*>(d->d_tag), reinterpret_cast<void*>(d->d_un.d_val));
        }
        break;
    }
  }

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
    if (d->d_tag == DT_SONAME) {
      soname_ = get_string(d->d_un.d_val);
#if defined(__work_around_b_19059885__)
      strlcpy(old_name_, soname_, sizeof(old_name_));
#endif
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
    // TODO (dimitry): remove != __ANDROID_API__ check once http://b/20020312 is fixed
    if (get_application_target_sdk_version() != __ANDROID_API__
        && get_application_target_sdk_version() > 22) {
      DL_ERR("%s: has text relocations", get_realpath());
      return false;
    }
    // Make segments writable to allow text relocations to work properly. We will later call
    // phdr_table_protect_segments() after all of them are applied and all constructors are run.
    DL_WARN("%s has text relocations. This is wasting memory and prevents "
            "security hardening. Please fix.", get_realpath());
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

  /* We can also turn on GNU RELRO protection */
  if (phdr_table_protect_gnu_relro(phdr, phnum, load_bias) < 0) {
    DL_ERR("can't enable GNU RELRO protection for \"%s\": %s",
           get_realpath(), strerror(errno));
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

  soinfo* si = soinfo_alloc("[vdso]", nullptr, 0, 0);

  si->phdr = reinterpret_cast<ElfW(Phdr)*>(reinterpret_cast<char*>(ehdr_vdso) + ehdr_vdso->e_phoff);
  si->phnum = ehdr_vdso->e_phnum;
  si->base = reinterpret_cast<ElfW(Addr)>(ehdr_vdso);
  si->size = phdr_table_get_load_size(si->phdr, si->phnum);
  si->load_bias = get_elf_exec_load_bias(ehdr_vdso);

  si->prelink_image();
  si->link_image(g_empty_list, soinfo::soinfo_list_t::make_list(si), nullptr);
#endif
}

/*
 * This is linker soinfo for GDB. See details below.
 */
#if defined(__LP64__)
#define LINKER_PATH "/system/bin/linker64"
#else
#define LINKER_PATH "/system/bin/linker"
#endif

// This is done to avoid calling c-tor prematurely
// because soinfo c-tor needs memory allocator
// which might be initialized after global variables.
static uint8_t linker_soinfo_for_gdb_buf[sizeof(soinfo)] __attribute__((aligned(8)));
static soinfo* linker_soinfo_for_gdb = nullptr;

/* gdb expects the linker to be in the debug shared object list.
 * Without this, gdb has trouble locating the linker's ".text"
 * and ".plt" sections. Gdb could also potentially use this to
 * relocate the offset of our exported 'rtld_db_dlactivity' symbol.
 * Don't use soinfo_alloc(), because the linker shouldn't
 * be on the soinfo list.
 */
static void init_linker_info_for_gdb(ElfW(Addr) linker_base) {
  linker_soinfo_for_gdb = new (linker_soinfo_for_gdb_buf) soinfo(LINKER_PATH, nullptr, 0, 0);

  linker_soinfo_for_gdb->load_bias = linker_base;

  /*
   * Set the dynamic field in the link map otherwise gdb will complain with
   * the following:
   *   warning: .dynamic section for "/system/bin/linker" is not at the
   *   expected address (wrong library or version mismatch?)
   */
  ElfW(Ehdr)* elf_hdr = reinterpret_cast<ElfW(Ehdr)*>(linker_base);
  ElfW(Phdr)* phdr = reinterpret_cast<ElfW(Phdr)*>(linker_base + elf_hdr->e_phoff);
  phdr_table_get_dynamic_section(phdr, elf_hdr->e_phnum, linker_base,
                                 &linker_soinfo_for_gdb->dynamic, nullptr);
  insert_soinfo_into_debug_map(linker_soinfo_for_gdb);
}

extern "C" int __system_properties_init(void);

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

  // These should have been sanitized by __libc_init_AT_SECURE, but the test
  // doesn't cost us anything.
  const char* ldpath_env = nullptr;
  const char* ldpreload_env = nullptr;
  if (!getauxval(AT_SECURE)) {
    ldpath_env = getenv("HYBRIS_LD_LIBRARY_PATH");
    ldpreload_env = getenv("HYBRIS_LD_PRELOAD");
  }

  INFO("[ android linker & debugger ]");

  soinfo* si = soinfo_alloc(args.argv[0], nullptr, 0, RTLD_GLOBAL);
  if (si == nullptr) {
    exit(EXIT_FAILURE);
  }

  /* bootstrap the link map, the main exe always needs to be first */
  si->set_main_executable();
  link_map* map = &(si->link_map_head);

  map->l_addr = 0;
  map->l_name = args.argv[0];
  map->l_prev = nullptr;
  map->l_next = nullptr;

  _r_debug.r_map = map;
  r_debug_head = map;

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

  ElfW(Ehdr)* elf_hdr = reinterpret_cast<ElfW(Ehdr)*>(si->base);
  if (elf_hdr->e_type != ET_DYN) {
    fprintf(stderr, "error: only position independent executables (PIE) are supported.\n");
    exit(EXIT_FAILURE);
  }

  // Use LD_LIBRARY_PATH and LD_PRELOAD (but only if we aren't setuid/setgid).
  if (ldpath_env)
    parse_LD_LIBRARY_PATH(ldpath_env);
  else
    parse_LD_LIBRARY_PATH(DEFAULT_HYBRIS_LD_LIBRARY_PATH);
  parse_LD_PRELOAD(ldpreload_env);

  somain = si;

  if (!si->prelink_image()) {
    fprintf(stderr, "CANNOT LINK EXECUTABLE: %s\n", linker_get_error_buffer());
    exit(EXIT_FAILURE);
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
      !find_libraries(si, needed_library_names, needed_libraries_count, nullptr,
          &g_ld_preloads, ld_preloads_count, RTLD_GLOBAL, nullptr)) {
    fprintf(stderr, "CANNOT LINK EXECUTABLE: %s\n", linker_get_error_buffer());
    exit(EXIT_FAILURE);
  } else if (needed_libraries_count == 0) {
    if (!si->link_image(g_empty_list, soinfo::soinfo_list_t::make_list(si), nullptr)) {
      fprintf(stderr, "CANNOT LINK EXECUTABLE: %s\n", linker_get_error_buffer());
      exit(EXIT_FAILURE);
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

  TRACE("[ Ready to execute '%s' @ %p ]", si->get_realpath(), reinterpret_cast<void*>(si->entry));
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
extern "C" void _start();

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

  soinfo linker_so(nullptr, nullptr, 0, 0);

  // If the linker is not acting as PT_INTERP entry_point is equal to
  // _start. Which means that the linker is running as an executable and
  // already linked by PT_INTERP.
  //
  // This happens when user tries to run 'adb shell /system/bin/linker'
  // see also https://code.google.com/p/android/issues/detail?id=63174
  if (reinterpret_cast<ElfW(Addr)>(&_start) == entry_point) {
    __libc_fatal("This is %s, the helper program for shared library executables.\n", args.argv[0]);
  }

  linker_so.base = linker_addr;
  linker_so.size = phdr_table_get_load_size(phdr, elf_hdr->e_phnum);
  linker_so.load_bias = get_elf_exec_load_bias(elf_hdr);
  linker_so.dynamic = nullptr;
  linker_so.phdr = phdr;
  linker_so.phnum = elf_hdr->e_phnum;
  linker_so.set_linker_flag();

  // This might not be obvious... The reasons why we pass g_empty_list
  // in place of local_group here are (1) we do not really need it, because
  // linker is built with DT_SYMBOLIC and therefore relocates its symbols against
  // itself without having to look into local_group and (2) allocators
  // are not yet initialized, and therefore we cannot use linked_list.push_*
  // functions at this point.
  if (!(linker_so.prelink_image() && linker_so.link_image(g_empty_list, g_empty_list, nullptr))) {
    // It would be nice to print an error message, but if the linker
    // can't link itself, there's no guarantee that we'll be able to
    // call write() (because it involves a GOT reference). We may as
    // well try though...
    const char* msg = "CANNOT LINK EXECUTABLE: ";
    int unused __attribute__((unused));
    unused = write(2, msg, strlen(msg));
    unused = write(2, __linker_dl_err_buf, strlen(__linker_dl_err_buf));
    unused = write(2, "\n", 1);
    _exit(EXIT_FAILURE);
  }

#ifdef DISABLED_FOR_HYBRIS_SUPPORT
  __libc_init_tls(args);
#endif

  // Initialize the linker's own global variables
  linker_so.call_constructors();

  // Initialize static variables. Note that in order to
  // get correct libdl_info we need to call constructors
  // before get_libdl_info().
  solist = get_libdl_info();
  sonext = get_libdl_info();

  // We have successfully fixed our own relocations. It's safe to run
  // the main part of the linker now.
  args.abort_message_ptr = &g_abort_message;
  ElfW(Addr) start_address = __linker_init_post_relocation(args, linker_addr);

  INFO("[ jumping to _start ]");

  // Return the address that the calling assembly stub should jump to.
  return start_address;
}

#endif
