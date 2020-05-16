/*
 * Copyright (C) 2007 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "linker.h"

#include <dlfcn.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <android/dlext.h>
#include <android/api-level.h>

#include <bionic/pthread_internal.h>
#include "private/bionic_tls.h"
#include "private/ScopedPthreadMutexLocker.h"
#include "private/ThreadLocalBuffer.h"

#include "hybris_compat.h"

#ifdef WANT_ARM_TRACING
#include "../wrappers.h"
#endif

extern void *(*_create_wrapper)(const char *symbol, void *function, int wrapper_type);

/* This file hijacks the symbols stubbed out in libdl.so. */

static __thread const char *dl_err_str;
char __thread dlerror_buffer[__BIONIC_DLERROR_BUFFER_SIZE];

static pthread_mutex_t g_dl_mutex = PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP;

static const char* __bionic_set_dlerror(char* new_value) {
#ifdef DISABLED_FOR_HYBRIS_SUPPORT
  char** dlerror_slot = &reinterpret_cast<char**>(__get_tls())[TLS_SLOT_DLERROR];

  const char* old_value = *dlerror_slot;
  *dlerror_slot = new_value;
  return old_value;
#else
  const char *old_value = dl_err_str;

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

extern "C" const char* android_dlerror() {
  const char* old_value = __bionic_set_dlerror(nullptr);
  return old_value;
}

void android_get_LD_LIBRARY_PATH(char* buffer, size_t buffer_size) {
  ScopedPthreadMutexLocker locker(&g_dl_mutex);
  do_android_get_LD_LIBRARY_PATH(buffer, buffer_size);
}

void android_update_LD_LIBRARY_PATH(const char* ld_library_path) {
  ScopedPthreadMutexLocker locker(&g_dl_mutex);
  do_android_update_LD_LIBRARY_PATH(ld_library_path);
}

static void* dlopen_ext(const char* filename, int flags, const android_dlextinfo* extinfo) {
  ScopedPthreadMutexLocker locker(&g_dl_mutex);
  soinfo* result = do_dlopen(filename, flags, extinfo);
  if (result == nullptr) {
    __bionic_format_dlerror("dlopen failed", linker_get_error_buffer());
    return nullptr;
  }
  return result;
}

extern "C" void* android_dlopen_ext(const char* filename, int flags, const android_dlextinfo* extinfo) {
  return dlopen_ext(filename, flags, extinfo);
}

extern "C" void* android_dlopen(const char* filename, int flags) {
  return dlopen_ext(filename, flags, nullptr);
}

extern "C" void* android_dlsym(void* handle, const char* symbol) {
  ScopedPthreadMutexLocker locker(&g_dl_mutex);

#if !defined(__LP64__)
  if (handle == nullptr) {
    __bionic_format_dlerror("dlsym library handle is null", nullptr);
    return nullptr;
  }
#endif

  if (symbol == nullptr) {
    __bionic_format_dlerror("dlsym symbol name is null", nullptr);
    return nullptr;
  }

  soinfo* found = nullptr;
  const ElfW(Sym)* sym = nullptr;
  void* caller_addr = __builtin_return_address(0);
  soinfo* caller = find_containing_library(caller_addr);

  if (handle == RTLD_DEFAULT || handle == RTLD_NEXT) {
    sym = dlsym_linear_lookup(symbol, &found, caller, handle);
  } else {
    sym = dlsym_handle_lookup(reinterpret_cast<soinfo*>(handle), &found, symbol);
  }

  if (sym != nullptr) {
    unsigned bind = ELF_ST_BIND(sym->st_info);

    if ((bind == STB_GLOBAL || bind == STB_WEAK) && sym->st_shndx != 0) {
#ifdef WANT_ARM_TRACING
      switch(ELF_ST_TYPE(sym->st_info))
      {
        case STT_FUNC:
        case STT_GNU_IFUNC:
        case STT_ARM_TFUNC:
          return reinterpret_cast<void*>(_create_wrapper((char*)symbol, (void*)found->resolve_symbol_address(sym), WRAPPER_DYNHOOK));
        default:
          return reinterpret_cast<void*>(found->resolve_symbol_address(sym));
      }
#else
      return reinterpret_cast<void*>(found->resolve_symbol_address(sym));
#endif
    }

    __bionic_format_dlerror("symbol found but not global", symbol);
    return nullptr;
  } else {
    __bionic_format_dlerror("undefined symbol", symbol);
    return nullptr;
  }
}

extern "C" int android_dladdr(const void* addr, Dl_info* info) {
  ScopedPthreadMutexLocker locker(&g_dl_mutex);

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

extern "C" int android_dlclose(void* handle) {
  ScopedPthreadMutexLocker locker(&g_dl_mutex);
  do_dlclose(reinterpret_cast<soinfo*>(handle));
  // dlclose has no defined errors.
  return 0;
}

int dl_iterate_phdr(int (*cb)(dl_phdr_info* info, size_t size, void* data), void* data) {
  ScopedPthreadMutexLocker locker(&g_dl_mutex);
  return do_dl_iterate_phdr(cb, data);
}

extern "C" void android_set_application_target_sdk_version(uint32_t target) {
  // lock to avoid modification in the middle of dlopen.
  ScopedPthreadMutexLocker locker(&g_dl_mutex);
  set_application_target_sdk_version(target);
}

extern "C" uint32_t android_get_application_target_sdk_version() {
  return get_application_target_sdk_version();
}

// name_offset: starting index of the name in libdl_info.strtab
#define ELF32_SYM_INITIALIZER(name_offset, value, shndx) \
    { name_offset, \
      reinterpret_cast<Elf32_Addr>(value), \
      /* st_size */ 0, \
      (shndx == 0) ? 0 : (STB_GLOBAL << 4), \
      /* st_other */ 0, \
      shndx, \
    }

#define ELF64_SYM_INITIALIZER(name_offset, value, shndx) \
    { name_offset, \
      (shndx == 0) ? 0 : (STB_GLOBAL << 4), \
      /* st_other */ 0, \
      shndx, \
      reinterpret_cast<Elf64_Addr>(value), \
      /* st_size */ 0, \
    }

#if defined(__arm__)
_Unwind_Ptr android_dl_unwind_find_exidx(_Unwind_Ptr pc, int *pcount);
#endif

static const char ANDROID_LIBDL_STRTAB[] =
  // 0000000 00011111 111112 22222222 2333333 3333444444444455555555556666666 6667777777777888888888899999 99999
  // 0123456 78901234 567890 12345678 9012345 6789012345678901234567890123456 7890123456789012345678901234 56789
    "dlopen\0dlclose\0dlsym\0dlerror\0dladdr\0android_update_LD_LIBRARY_PATH\0android_get_LD_LIBRARY_PATH\0dl_it"
  // 00000000001 1111111112222222222 3333333333444444444455555555556666666666777 777777788888888889999999999
  // 01234567890 1234567890123456789 0123456789012345678901234567890123456789012 345678901234567890123456789
    "erate_phdr\0android_dlopen_ext\0android_set_application_target_sdk_version\0android_get_application_tar"
  // 0000000000111111
  // 0123456789012345
    "get_sdk_version\0"
#if defined(__arm__)
  // 216
    "dl_unwind_find_exidx\0"
#endif
    ;

static ElfW(Sym) g_libdl_symtab[] = {
  // Total length of libdl_info.strtab, including trailing 0.
  // This is actually the STH_UNDEF entry. Technically, it's
  // supposed to have st_name == 0, but instead, it points to an index
  // in the strtab with a \0 to make iterating through the symtab easier.
  ELFW(SYM_INITIALIZER)(sizeof(ANDROID_LIBDL_STRTAB) - 1, nullptr, 0),
  ELFW(SYM_INITIALIZER)(  0, &android_dlopen, 1),
  ELFW(SYM_INITIALIZER)(  7, &android_dlclose, 1),
  ELFW(SYM_INITIALIZER)( 15, &android_dlsym, 1),
  ELFW(SYM_INITIALIZER)( 21, &android_dlerror, 1),
  ELFW(SYM_INITIALIZER)( 29, &android_dladdr, 1),
  ELFW(SYM_INITIALIZER)( 36, &android_update_LD_LIBRARY_PATH, 1),
  ELFW(SYM_INITIALIZER)( 67, &android_get_LD_LIBRARY_PATH, 1),
  ELFW(SYM_INITIALIZER)( 95, &dl_iterate_phdr, 1),
  ELFW(SYM_INITIALIZER)(111, &android_dlopen_ext, 1),
  ELFW(SYM_INITIALIZER)(130, &android_set_application_target_sdk_version, 1),
  ELFW(SYM_INITIALIZER)(173, &android_get_application_target_sdk_version, 1),
#if defined(__arm__)
  ELFW(SYM_INITIALIZER)(216, &dl_unwind_find_exidx, 1),
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
static unsigned g_libdl_chains[] = { 0, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 0 };
#else
static unsigned g_libdl_chains[] = { 0, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 0 };
#endif

static uint8_t __libdl_info_buf[sizeof(soinfo)] __attribute__((aligned(8)));
static soinfo* __libdl_info = nullptr;

// This is used by the dynamic linker. Every process gets these symbols for free.
soinfo* get_libdl_info() {
  if (__libdl_info == nullptr) {
    __libdl_info = new (__libdl_info_buf) soinfo("libdl.so", nullptr, 0, RTLD_GLOBAL);
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
    __libdl_info->soname_ = "libdl.so";
    __libdl_info->target_sdk_version_ = __ANDROID_API__;
#if DISABLED_FOR_HYBRIS_SUPPORT
#if defined(__arm__)
    strlcpy(__libdl_info->old_name_, __libdl_info->soname_, sizeof(__libdl_info->old_name_));
#endif
#endif
  }

  return __libdl_info;
}
