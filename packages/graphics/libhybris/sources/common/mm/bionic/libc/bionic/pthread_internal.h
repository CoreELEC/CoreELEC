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
#ifndef _PTHREAD_INTERNAL_H_
#define _PTHREAD_INTERNAL_H_

#include <pthread.h>
#include <atomic>

#include "private/bionic_tls.h"

/* Has the thread been detached by a pthread_join or pthread_detach call? */
#define PTHREAD_ATTR_FLAG_DETACHED 0x00000001

/* Has the thread been joined by another thread? */
#define PTHREAD_ATTR_FLAG_JOINED 0x00000002

struct pthread_key_data_t {
  uintptr_t seq; // Use uintptr_t just for alignment, as we use pointer below.
  void* data;
};

enum ThreadJoinState {
  THREAD_NOT_JOINED,
  THREAD_EXITED_NOT_JOINED,
  THREAD_JOINED,
  THREAD_DETACHED
};

struct pthread_internal_t {
  struct pthread_internal_t* next;
  struct pthread_internal_t* prev;

  pid_t tid;

 private:
  pid_t cached_pid_;

 public:
  pid_t invalidate_cached_pid() {
    pid_t old_value;
    get_cached_pid(&old_value);
    set_cached_pid(0);
    return old_value;
  }

  void set_cached_pid(pid_t value) {
    cached_pid_ = value;
  }

  bool get_cached_pid(pid_t* cached_pid) {
    *cached_pid = cached_pid_;
    return (*cached_pid != 0);
  }

  pthread_attr_t attr;

  ThreadJoinState join_state;

  //__pthread_cleanup_t* cleanup_stack;

  void* (*start_routine)(void*);
  void* start_routine_arg;
  void* return_value;

  void* alternate_signal_stack;

  pthread_mutex_t startup_handshake_mutex;

  size_t mmap_size;

  void* tls[BIONIC_TLS_SLOTS];

  pthread_key_data_t key_data[BIONIC_PTHREAD_KEY_COUNT];

  /*
   * The dynamic linker implements dlerror(3), which makes it hard for us to implement this
   * per-thread buffer by simply using malloc(3) and free(3).
   */
#define __BIONIC_DLERROR_BUFFER_SIZE 512
  char dlerror_buffer[__BIONIC_DLERROR_BUFFER_SIZE];
};

 int __init_thread(pthread_internal_t* thread);
 void __init_tls(pthread_internal_t* thread);
 void __init_alternate_signal_stack(pthread_internal_t*);

 pthread_t           __pthread_internal_add(pthread_internal_t* thread);
 pthread_internal_t* __pthread_internal_find(pthread_t pthread_id);
 void                __pthread_internal_remove(pthread_internal_t* thread);
 void                __pthread_internal_remove_and_free(pthread_internal_t* thread);

// Make __get_thread() inlined for performance reason. See http://b/19825434.
static inline pthread_internal_t* __get_thread() {
  return reinterpret_cast<pthread_internal_t*>(__get_tls()[TLS_SLOT_THREAD_ID]);
}

 void pthread_key_clean_all(void);

/*
 * Traditionally we gave threads a 1MiB stack. When we started
 * allocating per-thread alternate signal stacks to ease debugging of
 * stack overflows, we subtracted the same amount we were using there
 * from the default thread stack size. This should keep memory usage
 * roughly constant.
 */
#define PTHREAD_STACK_SIZE_DEFAULT ((1 * 1024 * 1024) - SIGSTKSZ)

/* Leave room for a guard page in the internally created signal stacks. */
#define SIGNAL_STACK_SIZE (SIGSTKSZ + PAGE_SIZE)

/* Needed by fork. */
 extern void __bionic_atfork_run_prepare();
 extern void __bionic_atfork_run_child();
 extern void __bionic_atfork_run_parent();

#endif /* _PTHREAD_INTERNAL_H_ */
