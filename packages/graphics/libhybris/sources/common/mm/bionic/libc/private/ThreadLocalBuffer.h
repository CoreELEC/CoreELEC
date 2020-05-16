/*
 * Copyright (C) 2012 The Android Open Source Project
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

#ifndef _BIONIC_THREAD_LOCAL_BUFFER_H_included
#define _BIONIC_THREAD_LOCAL_BUFFER_H_included

#include <malloc.h>
#include <pthread.h>

// TODO: use __thread instead?

template <typename T, size_t Size = sizeof(T)>
class ThreadLocalBuffer {
 public:
  ThreadLocalBuffer() {
    // We used to use pthread_once to initialize the keys, but life is more predictable
    // if we allocate them all up front when the C library starts up, via __constructor__.
    pthread_key_create(&key_, free);
  }

  T* get() {
    T* result = reinterpret_cast<T*>(pthread_getspecific(key_));
    if (result == nullptr) {
      result = reinterpret_cast<T*>(calloc(1, Size));
      pthread_setspecific(key_, result);
    }
    return result;
  }

  size_t size() { return Size; }

 private:
  pthread_key_t key_;
};

#endif // _BIONIC_THREAD_LOCAL_BUFFER_H_included
