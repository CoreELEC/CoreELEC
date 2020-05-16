/*
 * Copyright (C) 2016 The Android Open Source Project
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

#include "hybris_compat.h"

#include "linker_main.h"

#include "linker_debug.h"
#include "linker_cfi.h"
#include "linker_gdb_support.h"
#include "linker_globals.h"
#include "linker_phdr.h"
#include "linker_utils.h"

#include "private/bionic_globals.h"
#include "private/bionic_tls.h"
#include "private/KernelArgumentBlock.h"

#include "android-base/strings.h"
#include "android-base/stringprintf.h"
#ifdef __ANDROID__
#include "debuggerd/handler.h"
#endif

#include <async_safe/log.h>

#include <vector>

extern void __libc_init_globals(KernelArgumentBlock&);
#ifdef DISABLED_FOR_HYBRIS_SUPPORT
extern void __libc_init_AT_SECURE(KernelArgumentBlock&);
#endif

#ifdef DISABLED_FOR_HYBRIS_SUPPORT
extern "C" void _start();
#endif

static ElfW(Addr) get_elf_exec_load_bias(const ElfW(Ehdr)* elf);

// These should be preserved static to avoid emitting
// RELATIVE relocations for the part of the code running
// before linker links itself.

// TODO (dimtiry): remove somain, rename solist to solist_head
static soinfo* solist;
static soinfo* sonext;
static soinfo* somain; // main process, always the one after libdl_info

void solist_add_soinfo(soinfo* si) {
  sonext->next = si;
  sonext = si;
}

bool solist_remove_soinfo(soinfo* si) {
  soinfo *prev = nullptr, *trav;
  for (trav = solist; trav != nullptr; trav = trav->next) {
    if (trav == si) {
      break;
    }
    prev = trav;
  }

  if (trav == nullptr) {
    // si was not in solist
    PRINT("name \"%s\"@%p is not in solist!", si->get_realpath(), si);
    return false;
  }

  // prev will never be null, because the first entry in solist is
  // always the static libdl_info.
  CHECK(prev != nullptr);
  prev->next = si->next;
  if (si == sonext) {
    sonext = prev;
  }

  return true;
}

soinfo* solist_get_head() {
  return solist;
}

soinfo* solist_get_somain() {
  return somain;
}

int g_ld_debug_verbosity;
abort_msg_t* g_abort_message = nullptr; // For debuggerd.

static std::vector<std::string> g_ld_preload_names;

static std::vector<soinfo*> g_ld_preloads;

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

// An empty list of soinfos
static soinfo_list_t g_empty_list;

static void add_vdso(KernelArgumentBlock& args) {
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
  si->link_image(g_empty_list, soinfo_list_t::make_list(si), nullptr);
}

/* gdb expects the linker to be in the debug shared object list.
 * Without this, gdb has trouble locating the linker's ".text"
 * and ".plt" sections. Gdb could also potentially use this to
 * relocate the offset of our exported 'rtld_db_dlactivity' symbol.
 * Note that the linker shouldn't be on the soinfo list.
 */
static link_map linker_link_map;

static void init_linker_info_for_gdb(ElfW(Addr) linker_base, char* linker_path) {
  linker_link_map.l_addr = linker_base;
  linker_link_map.l_name = linker_path;

  /*
   * Set the dynamic field in the link map otherwise gdb will complain with
   * the following:
   *   warning: .dynamic section for "/system/bin/linker" is not at the
   *   expected address (wrong library or version mismatch?)
   */
  ElfW(Ehdr)* elf_hdr = reinterpret_cast<ElfW(Ehdr)*>(linker_base);
  ElfW(Phdr)* phdr = reinterpret_cast<ElfW(Phdr)*>(linker_base + elf_hdr->e_phoff);
  phdr_table_get_dynamic_section(phdr, elf_hdr->e_phnum, linker_base,
                                 &linker_link_map.l_ld, nullptr);

}

extern "C" int __system_properties_init(void);

static const char* get_executable_path() {
  static std::string executable_path;
  if (executable_path.empty()) {
    char path[PATH_MAX];
    ssize_t path_len = readlink("/proc/self/exe", path, sizeof(path));
    if (path_len == -1 || path_len >= static_cast<ssize_t>(sizeof(path))) {
      async_safe_fatal("readlink('/proc/self/exe') failed: %s", strerror(errno));
    }
    executable_path = std::string(path, path_len);
  }

  return executable_path.c_str();
}

#if defined(__LP64__)
static char kLinkerPath[] = "/system/bin/linker64";
#else
static char kLinkerPath[] = "/system/bin/linker";
#endif

/*
 * This code is called after the linker has linked itself and
 * fixed it's own GOT. It is safe to make references to externs
 * and other non-local data at this point.
 */
static ElfW(Addr) __linker_init_post_relocation(KernelArgumentBlock& args) {
  ProtectedDataGuard guard;

#if TIMING
  struct timeval t0, t1;
  gettimeofday(&t0, 0);
#endif

#ifdef DISABLED_FOR_HYBRIS_SUPPORT
  // Sanitize the environment.
  __libc_init_AT_SECURE(args);

  // Initialize system properties
  __system_properties_init(); // may use 'environ'

  // Register the debuggerd signal handler.
#ifdef __ANDROID__
  debuggerd_callbacks_t callbacks = {
    .get_abort_message = []() {
      return g_abort_message;
    },
    .post_dump = &notify_gdb_of_libraries,
  };
  debuggerd_init(&callbacks);
#endif
#endif

  g_linker_logger.ResetState();

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
  if (!getauxval(AT_SECURE)) {
    ldpath_env = getenv("HYBRIS_LD_LIBRARY_PATH");
    if (ldpath_env != nullptr) {
      INFO("[ LD_LIBRARY_PATH set to \"%s\" ]", ldpath_env);
    }
    ldpreload_env = getenv("HYBRIS_LD_PRELOAD");
    if (ldpreload_env != nullptr) {
      INFO("[ LD_PRELOAD set to \"%s\" ]", ldpreload_env);
    }
  }

  struct stat file_stat;
  // Stat "/proc/self/exe" instead of executable_path because
  // the executable could be unlinked by this point and it should
  // not cause a crash (see http://b/31084669)
  if (TEMP_FAILURE_RETRY(stat("/proc/self/exe", &file_stat)) != 0) {
    async_safe_fatal("unable to stat \"/proc/self/exe\": %s", strerror(errno));
  }

  const char* executable_path = get_executable_path();
  soinfo* si = soinfo_alloc(g_default_namespace, executable_path, &file_stat, 0, RTLD_GLOBAL);
  if (si == nullptr) {
    async_safe_fatal("Couldn't allocate soinfo: out of memory?");
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
  insert_link_map_into_debug_map(&linker_link_map);

  // Extract information passed from the kernel.
  si->phdr = reinterpret_cast<ElfW(Phdr)*>(args.getauxval(AT_PHDR));
  si->phnum = args.getauxval(AT_PHNUM);

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

  if (si->base == 0) {
    async_safe_fatal("Could not find a PHDR: broken executable?");
  }

  si->dynamic = nullptr;

  ElfW(Ehdr)* elf_hdr = reinterpret_cast<ElfW(Ehdr)*>(si->base);

  // We haven't supported non-PIE since Lollipop for security reasons.
  if (elf_hdr->e_type != ET_DYN) {
    // We don't use __libc_fatal here because we don't want a tombstone: it's
    // been several years now but we still find ourselves on app compatibility
    // investigations because some app's trying to launch an executable that
    // hasn't worked in at least three years, and we've "helpfully" dropped a
    // tombstone for them. The tombstone never provided any detail relevant to
    // fixing the problem anyway, and the utility of drawing extra attention
    // to the problem is non-existent at this late date.
    fprintf(stderr,
                     "\"%s\": error: Android 5.0 and later only support "
                     "position-independent executables (-fPIE).\n",
                     g_argv[0]);
    exit(EXIT_FAILURE);
  }

  // Use LD_LIBRARY_PATH and LD_PRELOAD (but only if we aren't setuid/setgid).
  if (ldpath_env)
    parse_LD_LIBRARY_PATH(ldpath_env);
  else
    parse_LD_LIBRARY_PATH(DEFAULT_HYBRIS_LD_LIBRARY_PATH);
  parse_LD_PRELOAD(ldpreload_env);

  somain = si;

  std::vector<android_namespace_t*> namespaces = init_default_namespaces(executable_path);

  if (!si->prelink_image()) {
    async_safe_fatal("CANNOT LINK EXECUTABLE \"%s\": %s", g_argv[0], linker_get_error_buffer());
  }

  // add somain to global group
  si->set_dt_flags_1(si->get_dt_flags_1() | DF_1_GLOBAL);
  // ... and add it to all other linked namespaces
  for (auto linked_ns : namespaces) {
    if (linked_ns != g_default_namespace) {
      linked_ns->add_soinfo(somain);
      somain->add_secondary_namespace(linked_ns);
    }
  }

  // Load ld_preloads and dependencies.
  std::vector<const char*> needed_library_name_list;
  size_t ld_preloads_count = 0;

  for (const auto& ld_preload_name : g_ld_preload_names) {
    needed_library_name_list.push_back(ld_preload_name.c_str());
    ++ld_preloads_count;
  }

  for_each_dt_needed(si, [&](const char* name) {
    needed_library_name_list.push_back(name);
  });

  const char** needed_library_names = &needed_library_name_list[0];
  size_t needed_libraries_count = needed_library_name_list.size();

  // readers_map is shared across recursive calls to find_libraries so that we
  // don't need to re-load elf headers.
  std::unordered_map<const soinfo*, ElfReader> readers_map;
  if (needed_libraries_count > 0 &&
      !find_libraries(g_default_namespace,
                      si,
                      needed_library_names,
                      needed_libraries_count,
                      nullptr,
                      &g_ld_preloads,
                      ld_preloads_count,
                      RTLD_GLOBAL,
                      nullptr,
                      true /* add_as_children */,
                      true /* search_linked_namespaces */,
                      readers_map,
                      &namespaces)) {
    fprintf(stderr, "CANNOT LINK EXECUTABLE \"%s\": %s", g_argv[0], linker_get_error_buffer());
  } else if (needed_libraries_count == 0) {
    if (!si->link_image(g_empty_list, soinfo_list_t::make_list(si), nullptr)) {
      fprintf(stderr, "CANNOT LINK EXECUTABLE \"%s\": %s", g_argv[0], linker_get_error_buffer());
    }
    si->increment_ref_count();
  }

  add_vdso(args);

  if (!get_cfi_shadow()->InitialLinkDone(solist)) {
    fprintf(stderr, "CANNOT LINK EXECUTABLE \"%s\": %s", g_argv[0], linker_get_error_buffer());
  }

  si->call_pre_init_constructors();

  /* After the prelink_image, the si->load_bias is initialized.
   * For so lib, the map->l_addr will be updated in notify_gdb_of_load.
   * We need to update this value for so exe here. So Unwind_Backtrace
   * for some arch like x86 could work correctly within so exe.
   */
  map->l_addr = si->load_bias;
  si->call_constructors();

#if TIMING
  gettimeofday(&t1, nullptr);
  PRINT("LINKER TIME: %s: %d microseconds", g_argv[0], (int) (
           (((long long)t1.tv_sec * 1000000LL) + (long long)t1.tv_usec) -
           (((long long)t0.tv_sec * 1000000LL) + (long long)t0.tv_usec)));
#endif
#if STATS
  PRINT("RELO STATS: %s: %d abs, %d rel, %d copy, %d symbol", g_argv[0],
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
    PRINT("PAGES MODIFIED: %s: %d (%dKB)", g_argv[0], count, count * 4);
  }
#endif

#if TIMING || STATS || COUNT_PAGES
  fflush(stdout);
#endif

  ElfW(Addr) entry = args.getauxval(AT_ENTRY);
  TRACE("[ Ready to execute \"%s\" @ %p ]", si->get_realpath(), reinterpret_cast<void*>(entry));
  return entry;
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

static void __linker_cannot_link(const char* argv0) {
  async_safe_fatal("CANNOT LINK EXECUTABLE \"%s\": %s", argv0, linker_get_error_buffer());
}

void* (*_get_hooked_symbol)(const char *sym, const char *requester);
void *(*_create_wrapper)(const char *symbol, void *function, int wrapper_type);

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

  sonext = solist = get_libdl_info(kLinkerPath, linker_link_map);

  init_default_namespaces(get_executable_path());
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

  // AT_BASE is set to 0 in the case when linker is run by iself
  // so in order to link the linker it needs to calcuate AT_BASE
  // using information at hand. The trick below takes advantage
  // of the fact that the value of linktime_addr before relocations
  // are run is an offset and this can be used to calculate AT_BASE.
  static uintptr_t linktime_addr = reinterpret_cast<uintptr_t>(&linktime_addr);
  ElfW(Addr) linker_addr = reinterpret_cast<uintptr_t>(&linktime_addr) - linktime_addr;

#if defined(__clang_analyzer__)
  // The analyzer assumes that linker_addr will always be null. Make it an
  // unknown value so we don't have to mark N places with NOLINTs.
  //
  // (`+=`, rather than `=`, allows us to sidestep a potential "unused store"
  // complaint)
  linker_addr += reinterpret_cast<uintptr_t>(raw_args);
#endif

  ElfW(Addr) entry_point = args.getauxval(AT_ENTRY);
  ElfW(Ehdr)* elf_hdr = reinterpret_cast<ElfW(Ehdr)*>(linker_addr);
  ElfW(Phdr)* phdr = reinterpret_cast<ElfW(Phdr)*>(linker_addr + elf_hdr->e_phoff);

  soinfo linker_so(nullptr, nullptr, nullptr, 0, 0);

  linker_so.base = linker_addr;
  linker_so.size = phdr_table_get_load_size(phdr, elf_hdr->e_phnum);
  linker_so.load_bias = get_elf_exec_load_bias(elf_hdr);
  linker_so.dynamic = nullptr;
  linker_so.phdr = phdr;
  linker_so.phnum = elf_hdr->e_phnum;
  linker_so.set_linker_flag();

  // Prelink the linker so we can access linker globals.
  if (!linker_so.prelink_image()) __linker_cannot_link(args.argv[0]);

  // This might not be obvious... The reasons why we pass g_empty_list
  // in place of local_group here are (1) we do not really need it, because
  // linker is built with DT_SYMBOLIC and therefore relocates its symbols against
  // itself without having to look into local_group and (2) allocators
  // are not yet initialized, and therefore we cannot use linked_list.push_*
  // functions at this point.
  if (!linker_so.link_image(g_empty_list, g_empty_list, nullptr)) __linker_cannot_link(args.argv[0]);

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
  if (!linker_so.protect_relro()) __linker_cannot_link(args.argv[0]);

  // Initialize the linker's static libc's globals
  __libc_init_globals(args);

  // store argc/argv/envp to use them for calling constructors
  g_argc = args.argc;
  g_argv = args.argv;
  g_envp = args.envp;

  // Initialize the linker's own global variables
  linker_so.call_constructors();

  // If the linker is not acting as PT_INTERP entry_point is equal to
  // _start. Which means that the linker is running as an executable and
  // already linked by PT_INTERP.
  //
  // This happens when user tries to run 'adb shell /system/bin/linker'
  // see also https://code.google.com/p/android/issues/detail?id=63174
  if (reinterpret_cast<ElfW(Addr)>(&_start) == entry_point) {
    async_safe_format_fd(STDOUT_FILENO,
                     "This is %s, the helper program for dynamic executables.\n",
                     args.argv[0]);
    exit(0);
  }

  init_linker_info_for_gdb(linker_addr, kLinkerPath);

  // Initialize static variables. Note that in order to
  // get correct libdl_info we need to call constructors
  // before get_libdl_info().
  sonext = solist = get_libdl_info(kLinkerPath, linker_link_map);
  g_default_namespace->add_soinfo(solist);

  // We have successfully fixed our own relocations. It's safe to run
  // the main part of the linker now.
  args.abort_message_ptr = &g_abort_message;
  ElfW(Addr) start_address = __linker_init_post_relocation(args);

  INFO("[ Jumping to _start (%p)... ]", reinterpret_cast<void*>(start_address));

  // Return the address that the calling assembly stub should jump to.
  return start_address;
}
#endif
