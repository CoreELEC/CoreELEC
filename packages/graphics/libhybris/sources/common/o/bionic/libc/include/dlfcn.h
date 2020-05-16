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

#ifndef __DLFCN_H__
#define __DLFCN_H__

#include <stdint.h>
#include <sys/cdefs.h>

__BEGIN_DECLS

#if defined(__cplusplus)
#define __BIONIC_CAST(_k,_t,_v) (_k<_t>(_v))
#else
#define __BIONIC_CAST(_k,_t,_v) ((_t) (_v))
#endif

#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wnullability-completeness"
#endif

typedef struct {
  /* Pathname of shared object that contains address. */
  const char* dli_fname;
  /* Address at which shared object is loaded. */
  void* dli_fbase;
  /* Name of nearest symbol with address lower than addr. */
  const char* dli_sname;
  /* Exact address of symbol named in dli_sname. */
  void* dli_saddr;
} Dl_info;

void* dlopen(const char* filename, int flag);
int dlclose(void* handle) __nonnull((1));
char* dlerror(void);
void* dlsym(void* handle, const char* symbol) __nonnull((2));
void* dlvsym(void* handle, const char* symbol, const char* version) __nonnull((2, 3));
int dladdr(const void* addr, Dl_info* info) __nonnull((2));

#define RTLD_LOCAL    0
#define RTLD_LAZY     0x00001
#define RTLD_NOW      0x00002
#define RTLD_NOLOAD   0x00004
#define RTLD_GLOBAL   0x00100
#define RTLD_NODELETE 0x01000

#if !defined(__LP64__)
/* LP32 is broken for historical reasons. */
#undef RTLD_NOW
#define RTLD_NOW      0x00000
#undef RTLD_GLOBAL
#define RTLD_GLOBAL   0x00002
#endif

#if defined (__LP64__)
#define RTLD_DEFAULT  __BIONIC_CAST(reinterpret_cast, void*, 0)
#define RTLD_NEXT     __BIONIC_CAST(reinterpret_cast, void*, -1L)
#else
#define RTLD_DEFAULT  __BIONIC_CAST(reinterpret_cast, void*, 0xffffffff)
#define RTLD_NEXT     __BIONIC_CAST(reinterpret_cast, void*, 0xfffffffe)
#endif

#if defined(__clang__)
#pragma clang diagnostic pop
#endif

__END_DECLS

#endif /* __DLFCN_H */
