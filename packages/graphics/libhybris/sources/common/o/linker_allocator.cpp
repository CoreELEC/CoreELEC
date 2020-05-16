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
#include "linker_debug.h"
#include "linker_utils.h"
#include "linker.h"

#include <algorithm>
#include <vector>

#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>

#include <async_safe/log.h>

#include <memory.h>

#include "private/bionic_prctl.h"

#include "hybris_compat.h"

//
// LinkerMemeoryAllocator is general purpose allocator
// designed to provide the same functionality as the malloc/free/realloc
// libc functions.
//
// On alloc:
// If size is >= 1k allocator proxies malloc call directly to mmap
// If size < 1k allocator uses SmallObjectAllocator for the size
// rounded up to the nearest power of two.
//
// On free:
//
// For a pointer allocated using proxy-to-mmap allocator unmaps
// the memory.
//
// For a pointer allocated using SmallObjectAllocator it adds
// the block to free_blocks_list_. If the number of free pages reaches 2,
// SmallObjectAllocator munmaps one of the pages keeping the other one
// in reserve.

static const char kSignature[4] = {'L', 'M', 'A', 1};

static const size_t kSmallObjectMaxSize = 1 << kSmallObjectMaxSizeLog2;

// This type is used for large allocations (with size >1k)
static const uint32_t kLargeObject = 111;

bool operator<(const small_object_page_record& one, const small_object_page_record& two) {
  return one.page_addr < two.page_addr;
}

static inline uint16_t log2(size_t number) {
  uint16_t result = 0;
  number--;

  while (number != 0) {
    result++;
    number >>= 1;
  }

  return result;
}

LinkerSmallObjectAllocator::LinkerSmallObjectAllocator(uint32_t type, size_t block_size)
    : type_(type), block_size_(block_size), free_pages_cnt_(0), free_blocks_list_(nullptr) {}

void* LinkerSmallObjectAllocator::alloc() {
  CHECK(block_size_ != 0);

  if (free_blocks_list_ == nullptr) {
    alloc_page();
  }

  small_object_block_record* block_record = free_blocks_list_;
  if (block_record->free_blocks_cnt > 1) {
    small_object_block_record* next_free = reinterpret_cast<small_object_block_record*>(
        reinterpret_cast<uint8_t*>(block_record) + block_size_);
    next_free->next = block_record->next;
    next_free->free_blocks_cnt = block_record->free_blocks_cnt - 1;
    free_blocks_list_ = next_free;
  } else {
    free_blocks_list_ = block_record->next;
  }

  // bookkeeping...
  auto page_record = find_page_record(block_record);

  if (page_record->allocated_blocks_cnt == 0) {
    free_pages_cnt_--;
  }

  page_record->free_blocks_cnt--;
  page_record->allocated_blocks_cnt++;

  memset(block_record, 0, block_size_);

  return block_record;
}

void LinkerSmallObjectAllocator::free_page(linker_vector_t::iterator page_record) {
  void* page_start = reinterpret_cast<void*>(page_record->page_addr);
  void* page_end = reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(page_start) + PAGE_SIZE);

  while (free_blocks_list_ != nullptr &&
      free_blocks_list_ > page_start &&
      free_blocks_list_ < page_end) {
    free_blocks_list_ = free_blocks_list_->next;
  }

  small_object_block_record* current = free_blocks_list_;

  while (current != nullptr) {
    while (current->next > page_start && current->next < page_end) {
      current->next = current->next->next;
    }

    current = current->next;
  }

  munmap(page_start, PAGE_SIZE);
  page_records_.erase(page_record);
  free_pages_cnt_--;
}

void LinkerSmallObjectAllocator::free(void* ptr) {
  auto page_record = find_page_record(ptr);

  ssize_t offset = reinterpret_cast<uintptr_t>(ptr) - sizeof(page_info);

  if (offset % block_size_ != 0) {
    async_safe_fatal("invalid pointer: %p (block_size=%zd)", ptr, block_size_);
  }

  memset(ptr, 0, block_size_);
  small_object_block_record* block_record = reinterpret_cast<small_object_block_record*>(ptr);

  block_record->next = free_blocks_list_;
  block_record->free_blocks_cnt = 1;

  free_blocks_list_ = block_record;

  page_record->free_blocks_cnt++;
  page_record->allocated_blocks_cnt--;

  if (page_record->allocated_blocks_cnt == 0) {
    if (free_pages_cnt_++ > 1) {
      // if we already have a free page - unmap this one.
      free_page(page_record);
    }
  }
}

linker_vector_t::iterator LinkerSmallObjectAllocator::find_page_record(void* ptr) {
  void* addr = reinterpret_cast<void*>(PAGE_START(reinterpret_cast<uintptr_t>(ptr)));
  small_object_page_record boundary;
  boundary.page_addr = addr;
  linker_vector_t::iterator it = std::lower_bound(
      page_records_.begin(), page_records_.end(), boundary);

  if (it == page_records_.end() || it->page_addr != addr) {
    // not found...
    async_safe_fatal("page record for %p was not found (block_size=%zd)", ptr, block_size_);
  }

  return it;
}

void LinkerSmallObjectAllocator::create_page_record(void* page_addr, size_t free_blocks_cnt) {
  small_object_page_record record;
  record.page_addr = page_addr;
  record.free_blocks_cnt = free_blocks_cnt;
  record.allocated_blocks_cnt = 0;

  linker_vector_t::iterator it = std::lower_bound(
      page_records_.begin(), page_records_.end(), record);
  page_records_.insert(it, record);
}

void LinkerSmallObjectAllocator::alloc_page() {
  static_assert(sizeof(page_info) % 16 == 0,
                "sizeof(page_info) is not multiple of 16");
  void* map_ptr = mmap(nullptr, PAGE_SIZE,
      PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, 0, 0);
  if (map_ptr == MAP_FAILED) {
    async_safe_fatal("mmap failed");
  }

  prctl(PR_SET_VMA, PR_SET_VMA_ANON_NAME, map_ptr, PAGE_SIZE, "linker_alloc_small_objects");

  page_info* info = reinterpret_cast<page_info*>(map_ptr);
  memcpy(info->signature, kSignature, sizeof(kSignature));
  info->type = type_;
  info->allocator_addr = this;

  size_t free_blocks_cnt = (PAGE_SIZE - sizeof(page_info))/block_size_;

  create_page_record(map_ptr, free_blocks_cnt);

  small_object_block_record* first_block = reinterpret_cast<small_object_block_record*>(info + 1);

  first_block->next = free_blocks_list_;
  first_block->free_blocks_cnt = free_blocks_cnt;

  free_blocks_list_ = first_block;
}


void LinkerMemoryAllocator::initialize_allocators() {
  if (allocators_ != nullptr) {
    return;
  }

  LinkerSmallObjectAllocator* allocators =
      reinterpret_cast<LinkerSmallObjectAllocator*>(allocators_buf_);

  for (size_t i = 0; i < kSmallObjectAllocatorsCount; ++i) {
    uint32_t type = i + kSmallObjectMinSizeLog2;
    new (allocators + i) LinkerSmallObjectAllocator(type, 1 << type);
  }

  allocators_ = allocators;
}

void* LinkerMemoryAllocator::alloc_mmap(size_t size) {
  size_t allocated_size = PAGE_END(size + sizeof(page_info));
  void* map_ptr = mmap(nullptr, allocated_size,
      PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, 0, 0);

  if (map_ptr == MAP_FAILED) {
    async_safe_fatal("mmap failed");
  }

  prctl(PR_SET_VMA, PR_SET_VMA_ANON_NAME, map_ptr, allocated_size, "linker_alloc_lob");

  page_info* info = reinterpret_cast<page_info*>(map_ptr);
  memcpy(info->signature, kSignature, sizeof(kSignature));
  info->type = kLargeObject;
  info->allocated_size = allocated_size;

  return info + 1;
}

void* LinkerMemoryAllocator::alloc(size_t size) {
  // treat alloc(0) as alloc(1)
  if (size == 0) {
    size = 1;
  }

  if (size > kSmallObjectMaxSize) {
    return alloc_mmap(size);
  }

  uint16_t log2_size = log2(size);

  if (log2_size < kSmallObjectMinSizeLog2) {
    log2_size = kSmallObjectMinSizeLog2;
  }

  return get_small_object_allocator(log2_size)->alloc();
}

page_info* LinkerMemoryAllocator::get_page_info(void* ptr) {
  page_info* info = reinterpret_cast<page_info*>(PAGE_START(reinterpret_cast<size_t>(ptr)));
  if (memcmp(info->signature, kSignature, sizeof(kSignature)) != 0) {
    async_safe_fatal("invalid pointer %p (page signature mismatch)", ptr);
  }

  return info;
}

void* LinkerMemoryAllocator::realloc(void* ptr, size_t size) {
  if (ptr == nullptr) {
    return alloc(size);
  }

  if (size == 0) {
    free(ptr);
    return nullptr;
  }

  page_info* info = get_page_info(ptr);

  size_t old_size = 0;

  if (info->type == kLargeObject) {
    old_size = info->allocated_size - sizeof(page_info);
  } else {
    LinkerSmallObjectAllocator* allocator = get_small_object_allocator(info->type);
    if (allocator != info->allocator_addr) {
      async_safe_fatal("invalid pointer %p (page signature mismatch)", ptr);
    }

    old_size = allocator->get_block_size();
  }

  if (old_size < size) {
    void *result = alloc(size);
    memcpy(result, ptr, old_size);
    free(ptr);
    return result;
  }

  return ptr;
}

void LinkerMemoryAllocator::free(void* ptr) {
  if (ptr == nullptr) {
    return;
  }

  page_info* info = get_page_info(ptr);

  if (info->type == kLargeObject) {
    munmap(info, info->allocated_size);
  } else {
    LinkerSmallObjectAllocator* allocator = get_small_object_allocator(info->type);
    if (allocator != info->allocator_addr) {
      async_safe_fatal("invalid pointer %p (invalid allocator address for the page)", ptr);
    }

    allocator->free(ptr);
  }
}

LinkerSmallObjectAllocator* LinkerMemoryAllocator::get_small_object_allocator(uint32_t type) {
  if (type < kSmallObjectMinSizeLog2 || type > kSmallObjectMaxSizeLog2) {
    async_safe_fatal("invalid type: %u", type);
  }

  initialize_allocators();
  return &allocators_[type - kSmallObjectMinSizeLog2];
}
