/*
 * Copyright (C) 2007 The Android Open Source Project
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

#include "linker.h"
#include "linker_cfi.h"
#include "linker_globals.h"
#include "linker_dlwarning.h"

#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <android/api-level.h>

#include <bionic/pthread_internal.h>
#include "private/bionic_tls.h"
#include "private/ScopedPthreadMutexLocker.h"

static pthread_mutex_t g_dl_mutex = PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP;

static __thread char *dl_err_str;
char __thread dlerror_buffer[__BIONIC_DLERROR_BUFFER_SIZE];

static char* __bionic_set_dlerror(char* new_value) {
#ifdef DISABLED_FOR_HYBRIS_SUPPORT
  char** dlerror_slot = &reinterpret_cast<char**>(__get_tls())[TLS_SLOT_DLERROR];

  char* old_value = *dlerror_slot;
  *dlerror_slot = new_value;
  if (new_value != nullptr) LD_LOG(kLogErrors, "dlerror set to \"%s\"", new_value);
  return old_value;
#else
  char *old_value = dl_err_str;
  dl_err_str = new_value;
  return old_value;
#endif
}

static void __bionic_format_dlerror(const char* msg, const char* detail) {
#ifdef DISABLED_FOR_HYBRIS_SUPPORT
  char* buffer = __get_thread()->dlerror_buffer;
#else
  char* buffer = dlerror_buffer;
#endif
  strlcpy(buffer, msg, __BIONIC_DLERROR_BUFFER_SIZE);
  if (detail != nullptr) {
    strlcat(buffer, ": ", __BIONIC_DLERROR_BUFFER_SIZE);
    strlcat(buffer, detail, __BIONIC_DLERROR_BUFFER_SIZE);
  }

  __bionic_set_dlerror(buffer);
}

char* __dlerror() {
  char* old_value = __bionic_set_dlerror(nullptr);
  return old_value;
}

void __android_get_LD_LIBRARY_PATH(char* buffer, size_t buffer_size) {
  ScopedPthreadMutexLocker locker(&g_dl_mutex);
  do_android_get_LD_LIBRARY_PATH(buffer, buffer_size);
}

void __android_update_LD_LIBRARY_PATH(const char* ld_library_path) {
  ScopedPthreadMutexLocker locker(&g_dl_mutex);
  do_android_update_LD_LIBRARY_PATH(ld_library_path);
}

static void* dlopen_ext(const char* filename,
                        int flags,
                        const android_dlextinfo* extinfo,
                        const void* caller_addr) {
  ScopedPthreadMutexLocker locker(&g_dl_mutex);
  g_linker_logger.ResetState();
  void* result = do_dlopen(filename, flags, extinfo, caller_addr);
  if (result == nullptr) {
    __bionic_format_dlerror("dlopen failed", linker_get_error_buffer());
    return nullptr;
  }
  return result;
}

void* __android_dlopen_ext(const char* filename,
                           int flags,
                           const android_dlextinfo* extinfo,
                           const void* caller_addr) {
  return dlopen_ext(filename, flags, extinfo, caller_addr);
}

void* __dlopen(const char* filename, int flags, const void* caller_addr) {
  return dlopen_ext(filename, flags, nullptr, caller_addr);
}

void* dlsym_impl(void* handle, const char* symbol, const char* version, const void* caller_addr) {
  ScopedPthreadMutexLocker locker(&g_dl_mutex);
  g_linker_logger.ResetState();
  void* result;
  if (!do_dlsym(handle, symbol, version, caller_addr, &result)) {
    __bionic_format_dlerror(linker_get_error_buffer(), nullptr);
    return nullptr;
  }

  return result;
}

void* __dlsym(void* handle, const char* symbol, const void* caller_addr) {
  return dlsym_impl(handle, symbol, nullptr, caller_addr);
}

void* __dlvsym(void* handle, const char* symbol, const char* version, const void* caller_addr) {
  return dlsym_impl(handle, symbol, version, caller_addr);
}

int __dladdr(const void* addr, Dl_info* info) {
  ScopedPthreadMutexLocker locker(&g_dl_mutex);
  return do_dladdr(addr, info);
}

int __dlclose(void* handle) {
  ScopedPthreadMutexLocker locker(&g_dl_mutex);
  int result = do_dlclose(handle);
  if (result != 0) {
    __bionic_format_dlerror("dlclose failed", linker_get_error_buffer());
  }
  return result;
}

// This function is needed by libgcc.a (this is why there is no prefix for this one) // in hybris there is a prefix, libgcc.a from android doesn't affect us here.
int __android_dl_iterate_phdr(int (*cb)(dl_phdr_info* info, size_t size, void* data), void* data) {
  ScopedPthreadMutexLocker locker(&g_dl_mutex);
  return do_dl_iterate_phdr(cb, data);
}

#if defined(__arm__)
_Unwind_Ptr __android_dl_unwind_find_exidx(_Unwind_Ptr pc, int* pcount) {
  ScopedPthreadMutexLocker locker(&g_dl_mutex);
  return do_dl_unwind_find_exidx(pc, pcount);
}
#endif

void __android_set_application_target_sdk_version(uint32_t target) {
  // lock to avoid modification in the middle of dlopen.
  ScopedPthreadMutexLocker locker(&g_dl_mutex);
  set_application_target_sdk_version(target);
}

uint32_t __android_get_application_target_sdk_version() {
  return get_application_target_sdk_version();
}

void __android_dlwarning(void* obj, void (*f)(void*, const char*)) {
  ScopedPthreadMutexLocker locker(&g_dl_mutex);
  get_dlwarning(obj, f);
}

bool __android_init_anonymous_namespace(const char* shared_libs_sonames,
                                         const char* library_search_path) {
  ScopedPthreadMutexLocker locker(&g_dl_mutex);
  bool success = init_anonymous_namespace(shared_libs_sonames, library_search_path);
  if (!success) {
    __bionic_format_dlerror("android_init_anonymous_namespace failed", linker_get_error_buffer());
  }

  return success;
}

android_namespace_t* __android_create_namespace(const char* name,
                                                const char* ld_library_path,
                                                const char* default_library_path,
                                                uint64_t type,
                                                const char* permitted_when_isolated_path,
                                                android_namespace_t* parent_namespace,
                                                const void* caller_addr) {
  ScopedPthreadMutexLocker locker(&g_dl_mutex);

  android_namespace_t* result = create_namespace(caller_addr,
                                                 name,
                                                 ld_library_path,
                                                 default_library_path,
                                                 type,
                                                 permitted_when_isolated_path,
                                                 parent_namespace);

  if (result == nullptr) {
    __bionic_format_dlerror("android_create_namespace failed", linker_get_error_buffer());
  }

  return result;
}

bool __android_link_namespaces(android_namespace_t* namespace_from,
                               android_namespace_t* namespace_to,
                               const char* shared_libs_sonames) {
  ScopedPthreadMutexLocker locker(&g_dl_mutex);

  bool success = link_namespaces(namespace_from, namespace_to, shared_libs_sonames);

  if (!success) {
    __bionic_format_dlerror("android_link_namespaces failed", linker_get_error_buffer());
  }

  return success;
}

android_namespace_t* __android_get_exported_namespace(const char* name) {
  return get_exported_namespace(name);
}

void __cfi_fail(uint64_t CallSiteTypeId, void* Ptr, void *DiagData, void *CallerPc) {
  CFIShadowWriter::CfiFail(CallSiteTypeId, Ptr, DiagData, CallerPc);
}

// name_offset: starting index of the name in libdl_info.strtab
#define ELF32_SYM_INITIALIZER(name_offset, value, shndx) \
    { name_offset, \
      reinterpret_cast<Elf32_Addr>(value), \
      /* st_size */ 0, \
      ((shndx) == 0) ? 0 : (STB_GLOBAL << 4), \
      /* st_other */ 0, \
      shndx, \
    }

#define ELF64_SYM_INITIALIZER(name_offset, value, shndx) \
    { name_offset, \
      ((shndx) == 0) ? 0 : (STB_GLOBAL << 4), \
      /* st_other */ 0, \
      shndx, \
      reinterpret_cast<Elf64_Addr>(value), \
      /* st_size */ 0, \
    }

#if defined(__arm__)
_Unwind_Ptr __android_dl_unwind_find_exidx(_Unwind_Ptr pc, int *pcount);
#endif

static const char ANDROID_LIBDL_STRTAB[] =
  // 0000000000111111 11112222222222333 333333344444444 44555555555566666 6666677777777778 8888888889999999999
  // 0123456789012345 67890123456789012 345678901234567 89012345678901234 5678901234567890 1234567890123456789
    "__loader_dlopen\0__loader_dlclose\0__loader_dlsym\0__loader_dlerror\0__loader_dladdr\0__loader_android_up"
  // 1*
  // 000000000011111111112 2222222223333333333444444444455555555 5566666666667777777777888 88888889999999999
  // 012345678901234567890 1234567890123456789012345678901234567 8901234567890123456789012 34567890123456789
    "date_LD_LIBRARY_PATH\0__loader_android_get_LD_LIBRARY_PATH\0__loader_dl_iterate_phdr\0__loader_android_"
  // 2*
  // 00000000001 1111111112222222222333333333344444444445555555555666 6666666777777777788888888889999999999
  // 01234567890 1234567890123456789012345678901234567890123456789012 3456789012345678901234567890123456789
    "dlopen_ext\0__loader_android_set_application_target_sdk_version\0__loader_android_get_application_targ"
  // 3*
  // 000000000011111 111112222222222333333333344444444445555555 5556666666666777777777788888888889 999999999
  // 012345678901234 567890123456789012345678901234567890123456 7890123456789012345678901234567890 123456789
    "et_sdk_version\0__loader_android_init_anonymous_namespace\0__loader_android_create_namespace\0__loader_"
  // 4*
  // 0000000 000111111111122222222223333 333333444444444455 555555556666666666777777777788888 888889999999999
  // 0123456 789012345678901234567890123 456789012345678901 234567890123456789012345678901234 567890123456789
    "dlvsym\0__loader_android_dlwarning\0__loader_cfi_fail\0__loader_android_link_namespaces\0__loader_androi"
  // 5*
  // 0000000000111111111122222 22222
  // 0123456789012345678901234 56789
    "d_get_exported_namespace\0"
#if defined(__arm__)
  // 525
    "__loader_dl_unwind_find_exidx\0"
#endif
    ;

static ElfW(Sym) g_libdl_symtab[] = {
  // Total length of libdl_info.strtab, including trailing 0.
  // This is actually the STH_UNDEF entry. Technically, it's
  // supposed to have st_name == 0, but instead, it points to an index
  // in the strtab with a \0 to make iterating through the symtab easier.
  ELFW(SYM_INITIALIZER)(sizeof(ANDROID_LIBDL_STRTAB) - 1, nullptr, 0),
  ELFW(SYM_INITIALIZER)(  0, &__dlopen, 1),
  ELFW(SYM_INITIALIZER)( 16, &__dlclose, 1),
  ELFW(SYM_INITIALIZER)( 33, &__dlsym, 1),
  ELFW(SYM_INITIALIZER)( 48, &__dlerror, 1),
  ELFW(SYM_INITIALIZER)( 65, &__dladdr, 1),
  ELFW(SYM_INITIALIZER)( 81, &__android_update_LD_LIBRARY_PATH, 1),
  ELFW(SYM_INITIALIZER)(121, &__android_get_LD_LIBRARY_PATH, 1),
  ELFW(SYM_INITIALIZER)(158, &__android_dl_iterate_phdr, 1),
  ELFW(SYM_INITIALIZER)(183, &__android_dlopen_ext, 1),
  ELFW(SYM_INITIALIZER)(211, &__android_set_application_target_sdk_version, 1),
  ELFW(SYM_INITIALIZER)(263, &__android_get_application_target_sdk_version, 1),
  ELFW(SYM_INITIALIZER)(315, &__android_init_anonymous_namespace, 1),
  ELFW(SYM_INITIALIZER)(357, &__android_create_namespace, 1),
  ELFW(SYM_INITIALIZER)(391, &__dlvsym, 1),
  ELFW(SYM_INITIALIZER)(407, &__android_dlwarning, 1),
  ELFW(SYM_INITIALIZER)(434, &__cfi_fail, 1),
  ELFW(SYM_INITIALIZER)(452, &__android_link_namespaces, 1),
  ELFW(SYM_INITIALIZER)(485, &__android_get_exported_namespace, 1),
#if defined(__arm__)
  ELFW(SYM_INITIALIZER)(525, &__android_dl_unwind_find_exidx, 1),
#endif
};

// Fake out a hash table with a single bucket.
//
// A search of the hash table will look through g_libdl_symtab starting with index 1, then
// use g_libdl_chains to find the next index to look at. g_libdl_chains should be set up to
// walk through every element in g_libdl_symtab, and then end with 0 (sentinel value).
//
// That is, g_libdl_chains should look like { 0, 2, 3, ... N, 0 } where N is the number
// of actual symbols, or nelems(g_libdl_symtab)-1 (since the first element of g_libdl_symtab is not
// a real symbol). (See soinfo_elf_lookup().)
//
// Note that adding any new symbols here requires stubbing them out in libdl.
static unsigned g_libdl_buckets[1] = { 1 };
#if defined(__arm__)
static unsigned g_libdl_chains[] = { 0, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 0 };
#else
static unsigned g_libdl_chains[] = { 0, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 0 };
#endif

static uint8_t __libdl_info_buf[sizeof(soinfo)] __attribute__((aligned(8)));
static soinfo* __libdl_info = nullptr;

// This is used by the dynamic linker. Every process gets these symbols for free.
soinfo* get_libdl_info(const char* linker_path, const link_map& linker_map) {
  if (__libdl_info == nullptr) {
    __libdl_info = new (__libdl_info_buf) soinfo(g_default_namespace, linker_path, nullptr, 0, 0);
    __libdl_info->flags_ |= FLAG_LINKED;
    __libdl_info->strtab_ = ANDROID_LIBDL_STRTAB;
    __libdl_info->symtab_ = g_libdl_symtab;
    __libdl_info->nbucket_ = sizeof(g_libdl_buckets)/sizeof(unsigned);
    __libdl_info->nchain_ = sizeof(g_libdl_chains)/sizeof(unsigned);
    __libdl_info->bucket_ = g_libdl_buckets;
    __libdl_info->chain_ = g_libdl_chains;
    __libdl_info->ref_count_ = 1;
    __libdl_info->strtab_size_ = sizeof(ANDROID_LIBDL_STRTAB);
    __libdl_info->local_group_root_ = __libdl_info;
    __libdl_info->soname_ = "ld-android.so";
    __libdl_info->target_sdk_version_ = __ANDROID_API__;
    __libdl_info->generate_handle();
    __libdl_info->link_map_head.l_addr = linker_map.l_addr;
    __libdl_info->link_map_head.l_name = linker_map.l_name;
    __libdl_info->link_map_head.l_ld = linker_map.l_ld;
#if DISABLED_FOR_HYBRIS_SUPPORT
#if defined(__work_around_b_24465209__)
    strlcpy(__libdl_info->old_name_, __libdl_info->soname_, sizeof(__libdl_info->old_name_));
#endif
#endif
  }

  return __libdl_info;
}

// hybris compat
extern "C" {
void* android_dlopen(const char* filename, int flag) {
  const void* caller_addr = __builtin_return_address(0);
  return __dlopen(filename, flag, caller_addr);
}

char* android_dlerror() {
  return __dlerror();
}

void* android_dlsym(void* handle, const char* symbol) {
  const void* caller_addr = __builtin_return_address(0);
  return __dlsym(handle, symbol, caller_addr);
}

void* android_dlvsym(void* handle, const char* symbol, const char* version) {
  const void* caller_addr = __builtin_return_address(0);
  return __dlvsym(handle, symbol, version, caller_addr);
}

int android_dladdr(const void* addr, Dl_info* info) {
  return __dladdr(addr, info);
}

int android_dlclose(void* handle) {
  return __dlclose(handle);
}

#if defined(__arm__)
_Unwind_Ptr android_dl_unwind_find_exidx(_Unwind_Ptr pc, int* pcount) {
  return __android_dl_unwind_find_exidx(pc, pcount);
}
#endif

int android_dl_iterate_phdr(int (*cb)(struct dl_phdr_info* info, size_t size, void* data), void* data) {
  return __android_dl_iterate_phdr(cb, data);
}

void android_get_LD_LIBRARY_PATH(char* buffer, size_t buffer_size) {
  __android_get_LD_LIBRARY_PATH(buffer, buffer_size);
}

void android_update_LD_LIBRARY_PATH(const char* ld_library_path) {
  __android_update_LD_LIBRARY_PATH(ld_library_path);
}

void* android_dlopen_ext(const char* filename, int flag, const android_dlextinfo* extinfo) {
  const void* caller_addr = __builtin_return_address(0);
  return __android_dlopen_ext(filename, flag, extinfo, caller_addr);
}

void android_set_application_target_sdk_version(uint32_t target) {
  __android_set_application_target_sdk_version(target);
}
uint32_t android_get_application_target_sdk_version() {
  return __android_get_application_target_sdk_version();
}

bool android_init_anonymous_namespace(const char* shared_libs_sonames,
                                      const char* library_search_path) {
  return __android_init_anonymous_namespace(shared_libs_sonames, library_search_path);
}

struct android_namespace_t* android_create_namespace(const char* name,
                                                     const char* ld_library_path,
                                                     const char* default_library_path,
                                                     uint64_t type,
                                                     const char* permitted_when_isolated_path,
                                                     struct android_namespace_t* parent) {
  const void* caller_addr = __builtin_return_address(0);
  return __android_create_namespace(name,
                                           ld_library_path,
                                           default_library_path,
                                           type,
                                           permitted_when_isolated_path,
                                           parent,
                                           caller_addr);
}

bool android_link_namespaces(struct android_namespace_t* namespace_from,
                             struct android_namespace_t* namespace_to,
                             const char* shared_libs_sonames) {
  return __android_link_namespaces(namespace_from, namespace_to, shared_libs_sonames);
}

void android_dlwarning(void* obj, void (*f)(void*, const char*)) {
  __android_dlwarning(obj, f);
}

struct android_namespace_t* android_get_exported_namespace(const char* name) {
  return __android_get_exported_namespace(name);
}
}

