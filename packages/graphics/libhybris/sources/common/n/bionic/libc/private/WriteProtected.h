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

#ifndef _PRIVATE_WRITEPROTECTED_H
#define _PRIVATE_WRITEPROTECTED_H

#include <errno.h>
#include <string.h>
#include <sys/cdefs.h>
#include <sys/mman.h>
#include <sys/user.h>

#include "private/bionic_macros.h"
#include "private/bionic_prctl.h"
#include "private/libc_logging.h"

template <typename T>
union WriteProtectedContents {
  T value;
  char padding[PAGE_SIZE];

  WriteProtectedContents() = default;
  DISALLOW_COPY_AND_ASSIGN(WriteProtectedContents);
} __attribute__((aligned(PAGE_SIZE)));

// Write protected wrapper class that aligns its contents to a page boundary,
// and sets the memory protection to be non-writable, except when being modified
// explicitly.
template <typename T>
class WriteProtected {
  static_assert(sizeof(T) < PAGE_SIZE,
                "WriteProtected only supports contents up to PAGE_SIZE");
  static_assert(__is_pod(T), "WriteProtected only supports POD contents");

  WriteProtectedContents<T> contents;

 public:
  WriteProtected() = default;
  DISALLOW_COPY_AND_ASSIGN(WriteProtected);

  void initialize() {
    // Not strictly necessary, but this will hopefully segfault if we initialize
    // multiple times by accident.
    memset(&contents, 0, sizeof(contents));

    if (mprotect(&contents, PAGE_SIZE, PROT_READ)) {
      __libc_fatal("failed to make WriteProtected nonwritable in initialize");
    }
  }

  const T* operator->() {
    return &contents.value;
  }

  const T& operator*() {
    return contents.value;
  }

  template <typename Mutator>
  void mutate(Mutator mutator) {
    if (mprotect(&contents, PAGE_SIZE, PROT_READ | PROT_WRITE) != 0) {
      __libc_fatal("failed to make WriteProtected writable in mutate: %s",
                   strerror(errno));
    }
    mutator(&contents.value);
    if (mprotect(&contents, PAGE_SIZE, PROT_READ) != 0) {
      __libc_fatal("failed to make WriteProtected nonwritable in mutate: %s",
                   strerror(errno));
    }
  }
};

#endif
