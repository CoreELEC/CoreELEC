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

#ifndef __LINKER_MAIN_H
#define __LINKER_MAIN_H

#include <android/dlext.h>

#include <unordered_map>
#include <vector>

#include "linker_namespaces.h"
#include "linker_soinfo.h"

class ProtectedDataGuard {
 public:
  ProtectedDataGuard();
  ~ProtectedDataGuard();

 private:
  void protect_data(int protection);
  static size_t ref_count_;
};

class ElfReader;

std::vector<android_namespace_t*> init_default_namespaces(const char* executable_path);
soinfo* soinfo_alloc(android_namespace_t* ns, const char* name,
                     struct stat* file_stat, off64_t file_offset,
                     uint32_t rtld_flags);

bool find_libraries(android_namespace_t* ns,
                    soinfo* start_with,
                    const char* const library_names[],
                    size_t library_names_count,
                    soinfo* soinfos[],
                    std::vector<soinfo*>* ld_preloads,
                    size_t ld_preloads_count,
                    int rtld_flags,
                    const android_dlextinfo* extinfo,
                    bool add_as_children,
                    bool search_linked_namespaces,
                    std::unordered_map<const soinfo*, ElfReader>& readers_map,
                    std::vector<android_namespace_t*>* namespaces = nullptr);

void solist_add_soinfo(soinfo* si);
bool solist_remove_soinfo(soinfo* si);
soinfo* solist_get_head();
soinfo* solist_get_somain();

#endif
