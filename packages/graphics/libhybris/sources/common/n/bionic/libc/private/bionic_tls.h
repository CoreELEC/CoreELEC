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

#ifndef __BIONIC_PRIVATE_BIONIC_TLS_H_
#define __BIONIC_PRIVATE_BIONIC_TLS_H_

#include <sys/cdefs.h>

#include "bionic_macros.h"
#include "__get_tls.h"

__BEGIN_DECLS

/** WARNING WARNING WARNING
 **
 ** This header file is *NOT* part of the public Bionic ABI/API
 ** and should not be used/included by user-serviceable parts of
 ** the system (e.g. applications).
 **
 ** It is only provided here for the benefit of the system dynamic
 ** linker and the OpenGL sub-system (which needs to access the
 ** pre-allocated slot directly for performance reason).
 **/

// Well-known TLS slots. What data goes in which slot is arbitrary unless otherwise noted.
enum {
  TLS_SLOT_SELF = 0, // The kernel requires this specific slot for x86.
  TLS_SLOT_THREAD_ID,
  TLS_SLOT_ERRNO = 5,

  // These two aren't used by bionic itself, but allow the graphics code to
  // access TLS directly rather than using the pthread API.
  TLS_SLOT_OPENGL_API = 6,
  TLS_SLOT_OPENGL = 7,

  // This slot is only used to pass information from the dynamic linker to
  // libc.so when the C library is loaded in to memory. The C runtime init
  // function will then clear it. Since its use is extremely temporary,
  // we reuse an existing location that isn't needed during libc startup.
  TLS_SLOT_BIONIC_PREINIT = TLS_SLOT_OPENGL_API,

  TLS_SLOT_STACK_GUARD = 8, // GCC requires this specific slot for x86.
  TLS_SLOT_DLERROR,

  // Fast storage for Thread::Current() in ART.
  TLS_SLOT_ART_THREAD_SELF,

  // Lets TSAN avoid using pthread_getspecific for finding the current thread
  // state.
  TLS_SLOT_TSAN,

  BIONIC_TLS_SLOTS // Must come last!
};

/*
 * Bionic uses some pthread keys internally. All pthread keys used internally
 * should be created in constructors, except for keys that may be used in or
 * before constructors.
 *
 * We need to manually maintain the count of pthread keys used internally, but
 * pthread_test should fail if we forget.
 *
 * These are the pthread keys currently used internally by libc:
 *
 *  basename               libc (ThreadLocalBuffer)
 *  dirname                libc (ThreadLocalBuffer)
 *  uselocale              libc (can be used in constructors)
 *  getmntent_mntent       libc (ThreadLocalBuffer)
 *  getmntent_strings      libc (ThreadLocalBuffer)
 *  ptsname                libc (ThreadLocalBuffer)
 *  ttyname                libc (ThreadLocalBuffer)
 *  strerror               libc (ThreadLocalBuffer)
 *  strsignal              libc (ThreadLocalBuffer)
 *  passwd                 libc (ThreadLocalBuffer)
 *  group                  libc (ThreadLocalBuffer)
 *  _res_key               libc (constructor in BSD code)
 */

#define LIBC_PTHREAD_KEY_RESERVED_COUNT 12

/* Internally, jemalloc uses a single key for per thread data. */
#define JEMALLOC_PTHREAD_KEY_RESERVED_COUNT 1
#define BIONIC_PTHREAD_KEY_RESERVED_COUNT (LIBC_PTHREAD_KEY_RESERVED_COUNT + JEMALLOC_PTHREAD_KEY_RESERVED_COUNT)

/*
 * Maximum number of pthread keys allocated.
 * This includes pthread keys used internally and externally.
 */
#define BIONIC_PTHREAD_KEY_COUNT (BIONIC_PTHREAD_KEY_RESERVED_COUNT + PTHREAD_KEYS_MAX)

__END_DECLS

#if defined(__cplusplus)
class KernelArgumentBlock;
extern void __libc_init_main_thread(KernelArgumentBlock&);
#endif

#endif /* __BIONIC_PRIVATE_BIONIC_TLS_H_ */
