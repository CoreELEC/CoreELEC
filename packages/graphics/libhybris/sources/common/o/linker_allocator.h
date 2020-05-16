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

#ifndef __LINKER_ALLOCATOR_H
#define __LINKER_ALLOCATOR_H

#include <stdlib.h>
#include <sys/cdefs.h>
#include <sys/mman.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>

#include <vector>

#include <async_safe/log.h>

#include "private/bionic_prctl.h"

const uint32_t kSmallObjectMaxSizeLog2 = 10;
const uint32_t kSmallObjectMinSizeLog2 = 4;
const uint32_t kSmallObjectAllocatorsCount = kSmallObjectMaxSizeLog2 - kSmallObjectMinSizeLog2 + 1;

class LinkerSmallObjectAllocator;

// This structure is placed at the beginning of each addressable page
// and has all information we need to find the corresponding memory allocator.
struct page_info {
  char signature[4];
  uint32_t type;
  union {
    // we use allocated_size for large objects allocator
    size_t allocated_size;
    // and allocator_addr for small ones.
    LinkerSmallObjectAllocator* allocator_addr;
  };
} __attribute__((aligned(16)));

struct small_object_page_record {
  void* page_addr;
  size_t free_blocks_cnt;
  size_t allocated_blocks_cnt;
};

// for lower_bound...
bool operator<(const small_object_page_record& one, const small_object_page_record& two);

struct small_object_block_record {
  small_object_block_record* next;
  size_t free_blocks_cnt;
};

// This is implementation for std::vector allocator
template <typename T>
class linker_vector_allocator {
 public:
  typedef T value_type;
  typedef T* pointer;
  typedef const T* const_pointer;
  typedef T& reference;
  typedef const T& const_reference;
  typedef size_t size_type;
  typedef ptrdiff_t difference_type;

  T* allocate(size_t n, const T* hint = nullptr) {
    size_t size = n * sizeof(T);
    void* ptr = mmap(const_cast<T*>(hint), size,
        PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, 0, 0);
    if (ptr == MAP_FAILED) {
      // Spec says we need to throw std::bad_alloc here but because our
      // code does not support exception handling anyways - we are going to abort.
      async_safe_fatal("mmap failed");
    }

    prctl(PR_SET_VMA, PR_SET_VMA_ANON_NAME, ptr, size, "linker_alloc_vector");

    return reinterpret_cast<T*>(ptr);
  }

  void deallocate(T* ptr, size_t n) {
    munmap(ptr, n * sizeof(T));
  }
};

typedef
    std::vector<small_object_page_record, linker_vector_allocator<small_object_page_record>>
    linker_vector_t;


class LinkerSmallObjectAllocator {
 public:
  LinkerSmallObjectAllocator(uint32_t type, size_t block_size);
  void* alloc();
  void free(void* ptr);

  size_t get_block_size() const { return block_size_; }
 private:
  void alloc_page();
  void free_page(linker_vector_t::iterator page_record);
  linker_vector_t::iterator find_page_record(void* ptr);
  void create_page_record(void* page_addr, size_t free_blocks_cnt);

  uint32_t type_;
  size_t block_size_;

  size_t free_pages_cnt_;
  small_object_block_record* free_blocks_list_;

  // sorted vector of page records
  linker_vector_t page_records_;
};

class LinkerMemoryAllocator {
 public:
  constexpr LinkerMemoryAllocator() : allocators_(nullptr), allocators_buf_() {}
  void* alloc(size_t size);

  // Note that this implementation of realloc never shrinks allocation
  void* realloc(void* ptr, size_t size);
  void free(void* ptr);
 private:
  void* alloc_mmap(size_t size);
  page_info* get_page_info(void* ptr);
  LinkerSmallObjectAllocator* get_small_object_allocator(uint32_t type);
  void initialize_allocators();

  LinkerSmallObjectAllocator* allocators_;
  uint8_t allocators_buf_[sizeof(LinkerSmallObjectAllocator)*kSmallObjectAllocatorsCount];
};


#endif  /* __LINKER_ALLOCATOR_H */
