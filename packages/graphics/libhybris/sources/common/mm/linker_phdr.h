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
#ifndef LINKER_PHDR_H
#define LINKER_PHDR_H

/* Declarations related to the ELF program header table and segments.
 *
 * The design goal is to provide an API that is as close as possible
 * to the ELF spec, and does not depend on linker-specific data
 * structures (e.g. the exact layout of struct soinfo).
 */

#include "linker.h"

class ElfReader {
 public:
  ElfReader(const char* name, int fd, off64_t file_offset, off64_t file_size);
  ~ElfReader();

  bool Load(const android_dlextinfo* extinfo);

  size_t phdr_count() { return phdr_num_; }
  ElfW(Addr) load_start() { return reinterpret_cast<ElfW(Addr)>(load_start_); }
  size_t load_size() { return load_size_; }
  ElfW(Addr) load_bias() { return load_bias_; }
  const ElfW(Phdr)* loaded_phdr() { return loaded_phdr_; }

 private:
  bool ReadElfHeader();
  bool VerifyElfHeader();
  bool ReadProgramHeader();
  bool ReserveAddressSpace(const android_dlextinfo* extinfo);
  bool LoadSegments();
  bool FindPhdr();
  bool CheckPhdr(ElfW(Addr));

  const char* name_;
  int fd_;
  off64_t file_offset_;
  off64_t file_size_;

  ElfW(Ehdr) header_;
  size_t phdr_num_;

  void* phdr_mmap_;
  ElfW(Phdr)* phdr_table_;
  ElfW(Addr) phdr_size_;

  // First page of reserved address space.
  void* load_start_;
  // Size in bytes of reserved address space.
  size_t load_size_;
  // Load bias.
  ElfW(Addr) load_bias_;

  // Loaded phdr.
  const ElfW(Phdr)* loaded_phdr_;
};

size_t phdr_table_get_load_size(const ElfW(Phdr)* phdr_table, size_t phdr_count,
                                ElfW(Addr)* min_vaddr = nullptr, ElfW(Addr)* max_vaddr = nullptr);

int phdr_table_protect_segments(const ElfW(Phdr)* phdr_table,
                                size_t phdr_count, ElfW(Addr) load_bias);

int phdr_table_unprotect_segments(const ElfW(Phdr)* phdr_table, size_t phdr_count,
                                  ElfW(Addr) load_bias);

int phdr_table_protect_gnu_relro(const ElfW(Phdr)* phdr_table, size_t phdr_count,
                                 ElfW(Addr) load_bias);

int phdr_table_serialize_gnu_relro(const ElfW(Phdr)* phdr_table, size_t phdr_count,
                                   ElfW(Addr) load_bias, int fd);

int phdr_table_map_gnu_relro(const ElfW(Phdr)* phdr_table, size_t phdr_count,
                             ElfW(Addr) load_bias, int fd);

#if defined(__arm__)
int phdr_table_get_arm_exidx(const ElfW(Phdr)* phdr_table, size_t phdr_count, ElfW(Addr) load_bias,
                             ElfW(Addr)** arm_exidx, size_t* arm_exidix_count);
#endif

void phdr_table_get_dynamic_section(const ElfW(Phdr)* phdr_table, size_t phdr_count,
                                    ElfW(Addr) load_bias, ElfW(Dyn)** dynamic,
                                    ElfW(Word)* dynamic_flags);

#endif /* LINKER_PHDR_H */
