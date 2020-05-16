/*
 * Copyright (C) 2014 The Android Open Source Project
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

#ifndef __LINKER_BLOCK_ALLOCATOR_H
#define __LINKER_BLOCK_ALLOCATOR_H

#include <stdlib.h>
#include <limits.h>
#include "private/bionic_macros.h"

struct LinkerBlockAllocatorPage;

/*
 * This class is a non-template version of the LinkerTypeAllocator
 * It keeps code inside .cpp file by keeping the interface
 * template-free.
 *
 * Please use LinkerTypeAllocator<type> where possible (everywhere).
 */
class LinkerBlockAllocator {
 public:
  explicit LinkerBlockAllocator(size_t block_size);

  void* alloc();
  void free(void* block);
  void protect_all(int prot);

 private:
  void create_new_page();
  LinkerBlockAllocatorPage* find_page(void* block);

  size_t block_size_;
  LinkerBlockAllocatorPage* page_list_;
  void* free_block_list_;

  DISALLOW_COPY_AND_ASSIGN(LinkerBlockAllocator);
};

/*
 * A simple allocator for the dynamic linker. An allocator allocates instances
 * of a single fixed-size type. Allocations are backed by page-sized private
 * anonymous mmaps.
 *
 * The differences between this allocator and LinkerMemoryAllocator are:
 * 1. This allocator manages space more efficiently. LinkerMemoryAllocator
 *    operates in power-of-two sized blocks up to 1k, when this implementation
 *    splits the page to aligned size of structure; For example for structures
 *    with size 513 this allocator will use 516 (520 for lp64) bytes of data
 *    where generalized implementation is going to use 1024 sized blocks.
 *
 * 2. This allocator does not munmap allocated memory, where LinkerMemoryAllocator does.
 *
 * 3. This allocator provides mprotect services to the user, where LinkerMemoryAllocator
 *    always treats it's memory as READ|WRITE.
 */
template<typename T>
class LinkerTypeAllocator {
 public:
  LinkerTypeAllocator() : block_allocator_(sizeof(T)) {}
  T* alloc() { return reinterpret_cast<T*>(block_allocator_.alloc()); }
  void free(T* t) { block_allocator_.free(t); }
  void protect_all(int prot) { block_allocator_.protect_all(prot); }
 private:
  LinkerBlockAllocator block_allocator_;
  DISALLOW_COPY_AND_ASSIGN(LinkerTypeAllocator);
};

#endif // __LINKER_BLOCK_ALLOCATOR_H
