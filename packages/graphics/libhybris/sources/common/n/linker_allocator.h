/*
 * Copyright (C) 2015 The Android Open Source Project
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

#ifndef __LINKER_ALLOCATOR_H
#define __LINKER_ALLOCATOR_H

#include <stdlib.h>
#include <sys/cdefs.h>
#include <sys/mman.h>
#include <stddef.h>
#include <unistd.h>

#include <vector>

#include "private/bionic_prctl.h"
#include "private/libc_logging.h"

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
      __libc_fatal("mmap failed");
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
  LinkerSmallObjectAllocator();
  void init(uint32_t type, size_t block_size);
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
  LinkerMemoryAllocator();
  void* alloc(size_t size);

  // Note that this implementation of realloc never shrinks allocation
  void* realloc(void* ptr, size_t size);
  void free(void* ptr);
 private:
  void* alloc_mmap(size_t size);
  page_info* get_page_info(void* ptr);
  LinkerSmallObjectAllocator* get_small_object_allocator(uint32_t type);

  LinkerSmallObjectAllocator allocators_[kSmallObjectAllocatorsCount];
};


#endif  /* __LINKER_ALLOCATOR_H */
