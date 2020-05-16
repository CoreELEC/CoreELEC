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

#ifndef __LINKER_COMMON_TYPES_H
#define __LINKER_COMMON_TYPES_H

#include <android/dlext.h>
#include "linked_list.h"

// TODO(dimitry): move this to linker_defines.h? Unless it is removed by
// consequent refactoring steps.

// Android uses RELA for aarch64 and x86_64. mips64 still uses REL.
#if defined(__aarch64__) || defined(__x86_64__)
#define USE_RELA 1
#endif


struct soinfo;

class SoinfoListAllocator {
 public:
  static LinkedListEntry<soinfo>* alloc();
  static void free(LinkedListEntry<soinfo>* entry);

 private:
  // unconstructable
  DISALLOW_IMPLICIT_CONSTRUCTORS(SoinfoListAllocator);
};

class NamespaceListAllocator {
 public:
  static LinkedListEntry<android_namespace_t>* alloc();
  static void free(LinkedListEntry<android_namespace_t>* entry);

 private:
  // unconstructable
  DISALLOW_IMPLICIT_CONSTRUCTORS(NamespaceListAllocator);
};

typedef LinkedList<soinfo, SoinfoListAllocator> soinfo_list_t;
typedef LinkedList<android_namespace_t, NamespaceListAllocator> android_namespace_list_t;

#endif  /* __LINKER_COMMON_TYPES_H */
