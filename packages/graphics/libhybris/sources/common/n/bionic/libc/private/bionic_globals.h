/*
 * Copyright (C) 2015 The Android Open Source Project
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

#ifndef _PRIVATE_BIONIC_GLOBALS_H
#define _PRIVATE_BIONIC_GLOBALS_H

#include <sys/cdefs.h>

#include "private/bionic_malloc_dispatch.h"
#include "private/bionic_vdso.h"
#include "private/WriteProtected.h"

struct libc_globals {
  vdso_entry vdso[VDSO_END];
  long setjmp_cookie;
  MallocDispatch malloc_dispatch;
};

extern WriteProtected<libc_globals> __libc_globals;

class KernelArgumentBlock;
void __libc_init_global_stack_chk_guard(KernelArgumentBlock& args);
void __libc_init_malloc(libc_globals* globals);
void __libc_init_setjmp_cookie(libc_globals* globals, KernelArgumentBlock& args);
void __libc_init_vdso(libc_globals* globals, KernelArgumentBlock& args);

#if defined(__i386__)
extern void* __libc_sysinfo;
void __libc_init_sysinfo(KernelArgumentBlock& args);
#endif

#endif
