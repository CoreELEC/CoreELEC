/*
 * Copyright (C) 2016 The Android Open Source Project
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

