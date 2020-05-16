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

#include "linker_block_allocator.h"
#include <inttypes.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>

#include "private/bionic_prctl.h"

#include "hybris_compat.h"

// the multiplier should be power of 2
static constexpr size_t round_up(size_t size, size_t multiplier) {
  return (size + (multiplier - 1)) & ~(multiplier-1);
}

struct LinkerBlockAllocatorPage {
  LinkerBlockAllocatorPage* next;
  uint8_t bytes[PAGE_SIZE - 16] __attribute__((aligned(16)));
};

struct FreeBlockInfo {
  void* next_block;
  size_t num_free_blocks;
};

LinkerBlockAllocator::LinkerBlockAllocator(size_t block_size)
  : block_size_(
      round_up(block_size < sizeof(FreeBlockInfo) ? sizeof(FreeBlockInfo) : block_size, 16)),
    page_list_(nullptr),
    free_block_list_(nullptr)
{}

void* LinkerBlockAllocator::alloc() {
  if (free_block_list_ == nullptr) {
    create_new_page();
  }

  FreeBlockInfo* block_info = reinterpret_cast<FreeBlockInfo*>(free_block_list_);
  if (block_info->num_free_blocks > 1) {
    FreeBlockInfo* next_block_info = reinterpret_cast<FreeBlockInfo*>(
      reinterpret_cast<char*>(free_block_list_) + block_size_);
    next_block_info->next_block = block_info->next_block;
    next_block_info->num_free_blocks = block_info->num_free_blocks - 1;
    free_block_list_ = next_block_info;
  } else {
    free_block_list_ = block_info->next_block;
  }

  memset(block_info, 0, block_size_);

  return block_info;
}

void LinkerBlockAllocator::free(void* block) {
  if (block == nullptr) {
    return;
  }

  LinkerBlockAllocatorPage* page = find_page(block);

  if (page == nullptr) {
    abort();
  }

  ssize_t offset = reinterpret_cast<uint8_t*>(block) - page->bytes;

  if (offset % block_size_ != 0) {
    abort();
  }

  memset(block, 0, block_size_);

  FreeBlockInfo* block_info = reinterpret_cast<FreeBlockInfo*>(block);

  block_info->next_block = free_block_list_;
  block_info->num_free_blocks = 1;

  free_block_list_ = block_info;
}

void LinkerBlockAllocator::protect_all(int prot) {
  for (LinkerBlockAllocatorPage* page = page_list_; page != nullptr; page = page->next) {
    if (mprotect(page, PAGE_SIZE, prot) == -1) {
      abort();
    }
  }
}

void LinkerBlockAllocator::create_new_page() {
  static_assert(sizeof(LinkerBlockAllocatorPage) == PAGE_SIZE,
                "Invalid sizeof(LinkerBlockAllocatorPage)");

  LinkerBlockAllocatorPage* page = reinterpret_cast<LinkerBlockAllocatorPage*>(
      mmap(nullptr, PAGE_SIZE, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, 0, 0));

  if (page == MAP_FAILED) {
    abort(); // oom
  }

  prctl(PR_SET_VMA, PR_SET_VMA_ANON_NAME, page, PAGE_SIZE, "linker_alloc");

  FreeBlockInfo* first_block = reinterpret_cast<FreeBlockInfo*>(page->bytes);
  first_block->next_block = free_block_list_;
  first_block->num_free_blocks = (PAGE_SIZE - sizeof(LinkerBlockAllocatorPage*))/block_size_;

  free_block_list_ = first_block;

  page->next = page_list_;
  page_list_ = page;
}

LinkerBlockAllocatorPage* LinkerBlockAllocator::find_page(void* block) {
  if (block == nullptr) {
    abort();
  }

  LinkerBlockAllocatorPage* page = page_list_;
  while (page != nullptr) {
    const uint8_t* page_ptr = reinterpret_cast<const uint8_t*>(page);
    if (block >= (page_ptr + sizeof(page->next)) && block < (page_ptr + PAGE_SIZE)) {
      return page;
    }

    page = page->next;
  }

  abort();
}
