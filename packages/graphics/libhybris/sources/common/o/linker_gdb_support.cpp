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

#include "linker_gdb_support.h"

#include <pthread.h>

#include "private/ScopedPthreadMutexLocker.h"

#include "hybris_compat.h"

// This function is an empty stub where GDB locates a breakpoint to get notified
// about linker activity.
extern "C"
void __attribute__((noinline)) __attribute__((visibility("default"))) rtld_db_dlactivity();

r_debug _r_debug =
    {1, nullptr, reinterpret_cast<uintptr_t>(&rtld_db_dlactivity), r_debug::RT_CONSISTENT, 0};

static pthread_mutex_t g__r_debug_mutex = PTHREAD_MUTEX_INITIALIZER;
static link_map* r_debug_head = nullptr;

int _linker_enable_gdb_support = 0;

void insert_link_map_into_debug_map(link_map* map) {
  if (!_linker_enable_gdb_support) return;

  // Stick the new library at the end of the list.
  // gdb tends to care more about libc than it does
  // about leaf libraries, and ordering it this way
  // reduces the back-and-forth over the wire.

  ///// PATCHED: we don't want libhybris modifying glibc's
  /////          link_map objects, which should not be linked
  /////          to bionic's stripped link_map objects.
  /////        ==> make a copy of the whole chain
  if(r_debug_head == nullptr && _r_debug.r_map != nullptr) {
    link_map *glibc_link_map = new link_map(*_r_debug.r_map);
    r_debug_head = glibc_link_map;

    while(glibc_link_map->l_next != nullptr) {
      link_map *copy_next_link_map = new link_map(*glibc_link_map->l_next);
      glibc_link_map->l_next = copy_next_link_map;
      copy_next_link_map->l_prev = glibc_link_map;

      glibc_link_map = copy_next_link_map;
    }
  }

  if (r_debug_head != nullptr) {
    r_debug_head->l_prev = map;
    map->l_next = r_debug_head;
    map->l_prev = nullptr;
  } else {
    _r_debug.r_map = map;
    map->l_prev = nullptr;
    map->l_next = nullptr;
  }
  _r_debug.r_map = r_debug_head = map;
}

void remove_link_map_from_debug_map(link_map* map) {
  if (!_linker_enable_gdb_support) return;

  if (r_debug_head == map) {
    r_debug_head = map->l_next;
  }

  if (map->l_prev) {
    map->l_prev->l_next = map->l_next;
  }
  if (map->l_next) {
    map->l_next->l_prev = map->l_prev;
  }
}

void notify_gdb_of_load(link_map* map) {
  ScopedPthreadMutexLocker locker(&g__r_debug_mutex);

  _r_debug.r_state = r_debug::RT_ADD;
  rtld_db_dlactivity();

  insert_link_map_into_debug_map(map);

  _r_debug.r_state = r_debug::RT_CONSISTENT;
  rtld_db_dlactivity();
}

void notify_gdb_of_unload(link_map* map) {
  ScopedPthreadMutexLocker locker(&g__r_debug_mutex);

  _r_debug.r_state = r_debug::RT_DELETE;
  rtld_db_dlactivity();

  remove_link_map_from_debug_map(map);

  _r_debug.r_state = r_debug::RT_CONSISTENT;
  rtld_db_dlactivity();
}

void notify_gdb_of_libraries() {
  _r_debug.r_state = r_debug::RT_ADD;
  rtld_db_dlactivity();
  _r_debug.r_state = r_debug::RT_CONSISTENT;
  rtld_db_dlactivity();
}

