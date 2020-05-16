/*
 * Copyright (C) 2014 The Android Open Source Project
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
