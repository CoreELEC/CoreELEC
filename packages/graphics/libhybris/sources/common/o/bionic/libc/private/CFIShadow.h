/*
 * Copyright (C) 2016 The Android Open Source Project
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

#ifndef CFI_SHADOW_H
#define CFI_SHADOW_H

#include <stdint.h>

#include "private/bionic_page.h"
#include "private/bionic_macros.h"

constexpr unsigned kLibraryAlignmentBits = 18;
constexpr size_t kLibraryAlignment = 1UL << kLibraryAlignmentBits;

// This class defines format of the shadow region for Control Flow Integrity support.
// See documentation in http://clang.llvm.org/docs/ControlFlowIntegrityDesign.html#shared-library-support.
//
// CFI shadow is effectively a very fast and specialized implementation of dladdr: given an address that
// belongs to a shared library or an executable, it can find the address of a specific export in that
// library (a function called "__cfi_check"). This is only guaranteed to work for
// addresses of possible CFI targets inside a library: indirectly called functions and virtual
// tables. A random address inside a library may not work in the future (but it does in the current
// implementation).
//
// Implementation is a sparse array of uint16_t where each element describes the location of
// __cfi_check for a 2**kShadowGranularity range of memory. Array elements (called "shadow values"
// below) are interpreted as follows.
//
// For an address P and corresponding shadow value V, the address of __cfi_check is calculated as
//   align_up(P, 2**kShadowGranularity) - (V - 2) * (2 ** kCfiCheckGranularity)
//
// Special shadow values:
//        0 = kInvalidShadow, this memory range has no valid CFI targets.
//        1 = kUncheckedShadow, any address is this memory range is a valid CFI target
//
// Loader requirement: each aligned 2**kShadowGranularity region of address space may contain at
// most one DSO.
// Compiler requirement: __cfi_check is aligned at kCfiCheckGranularity.
// Compiler requirement: __cfi_check for a given DSO is located below any CFI target for that DSO.
class CFIShadow {
 public:
  static constexpr uintptr_t kShadowGranularity = kLibraryAlignmentBits;
  static constexpr uintptr_t kCfiCheckGranularity = 12;

  // Each uint16_t element of the shadow corresponds to this much application memory.
  static constexpr uintptr_t kShadowAlign = 1UL << kShadowGranularity;

  // Alignment of __cfi_check.
  static constexpr uintptr_t kCfiCheckAlign = 1UL << kCfiCheckGranularity;  // 4K

#if defined (__LP64__)
  static constexpr uintptr_t kMaxTargetAddr = 0xffffffffffff;
#else
  static constexpr uintptr_t kMaxTargetAddr = 0xffffffff;
#endif

  // Shadow is 2 -> 2**kShadowGranularity.
  static constexpr uintptr_t kShadowSize =
      align_up((kMaxTargetAddr >> (kShadowGranularity - 1)), PAGE_SIZE);

  // Returns offset inside the shadow region for an address.
  static constexpr uintptr_t MemToShadowOffset(uintptr_t x) {
    return (x >> kShadowGranularity) << 1;
  }

  typedef int (*CFICheckFn)(uint64_t, void *, void *);

 public:
  enum ShadowValues : uint16_t {
    kInvalidShadow = 0,    // Not a valid CFI target.
    kUncheckedShadow = 1,  // Unchecked, valid CFI target.
    kRegularShadowMin = 2  // This and all higher values encode a negative offset to __cfi_check in
                           // the units of kCfiCheckGranularity, starting with 0 at
                           // kRegularShadowMin.
  };
};

#endif  // CFI_SHADOW_H
