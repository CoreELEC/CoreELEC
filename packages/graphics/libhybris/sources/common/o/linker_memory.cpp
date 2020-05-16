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

#include "linker_allocator.h"

#include <stdlib.h>
#include <sys/cdefs.h>
#include <unistd.h>

#include <async_safe/log.h>

#if DISABLED_FOR_HYBRIS_SUPPORT
static LinkerMemoryAllocator g_linker_allocator;
static pid_t fallback_tid = 0;

// Used by libdebuggerd_handler to switch allocators during a crash dump, in
// case the linker heap is corrupted. Do not use this function.
extern "C" void __linker_enable_fallback_allocator() {
  if (fallback_tid != 0) {
    async_safe_fatal("attempted to use currently-in-use fallback allocator");
  }

  fallback_tid = gettid();
}

extern "C" void __linker_disable_fallback_allocator() {
  if (fallback_tid == 0) {
    async_safe_fatal("attempted to disable unused fallback allocator");
  }

  fallback_tid = 0;
}

static LinkerMemoryAllocator& get_fallback_allocator() {
  static LinkerMemoryAllocator fallback_allocator;
  return fallback_allocator;
}

static LinkerMemoryAllocator& get_allocator() {
  if (__predict_false(fallback_tid) && __predict_false(gettid() == fallback_tid)) {
    return get_fallback_allocator();
  }
  return g_linker_allocator;
}

void* malloc(size_t byte_count) {
  return get_allocator().alloc(byte_count);
}

void* calloc(size_t item_count, size_t item_size) {
  return get_allocator().alloc(item_count*item_size);
}

void* realloc(void* p, size_t byte_count) {
  return get_allocator().realloc(p, byte_count);
}

void free(void* ptr) {
  get_allocator().free(ptr);
}

#endif
