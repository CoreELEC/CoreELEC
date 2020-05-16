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

#ifndef _LINKER_CFI_H_
#define _LINKER_CFI_H_

#include "linker.h"
#include "linker_debug.h"

#include <algorithm>

#include "hybris_compat.h"
#include "private/CFIShadow.h"

// This class keeps the contents of CFI shadow up-to-date with the current set of loaded libraries.
// See the comment in CFIShadow.h for more context.
// See documentation in http://clang.llvm.org/docs/ControlFlowIntegrityDesign.html#shared-library-support.
//
// Shadow is mapped and initialized lazily as soon as the first CFI-enabled DSO is loaded.
// It is updated after any library is loaded (but before any constructors are ran), and
// before any library is unloaded.
class CFIShadowWriter : private CFIShadow {
  // Returns pointer to the shadow element for an address.
  uint16_t* MemToShadow(uintptr_t x) {
    return reinterpret_cast<uint16_t*>(*shadow_start + MemToShadowOffset(x));
  }

  // Update shadow for the address range to the given constant value.
  void AddConstant(uintptr_t begin, uintptr_t end, uint16_t v);

  // Update shadow for the address range to kUncheckedShadow.
  void AddUnchecked(uintptr_t begin, uintptr_t end);

  // Update shadow for the address range to kInvalidShadow.
  void AddInvalid(uintptr_t begin, uintptr_t end);

  // Update shadow for the address range to the given __cfi_check value.
  void Add(uintptr_t begin, uintptr_t end, uintptr_t cfi_check);

  // Add a DSO to CFI shadow.
  bool AddLibrary(soinfo* si);

  // Map CFI shadow.
  uintptr_t MapShadow();

  // Initialize CFI shadow and update its contents for everything in solist if any loaded library is
  // CFI-enabled. If new_si != nullptr, do an incremental check by looking only at new_si; otherwise
  // look at the entire solist.
  bool MaybeInit(soinfo *new_si, soinfo *solist);

  // Set a human readable name for the entire shadow region.
  void FixupVmaName();

  // Pass the pointer to the mapped shadow region to libdl. Must only be called once.
  // Flips shadow_start to a non-nullptr value.
  bool NotifyLibDl(soinfo *solist, uintptr_t p);

  // Pointer to the shadow start address.
  uintptr_t *shadow_start;

  bool initial_link_done;

 public:
  // Update shadow after loading a DSO.
  // This function will initialize the shadow if it sees a CFI-enabled DSO for the first time.
  // In that case it will retroactively update shadow for all previously loaded DSOs. "solist" is a
  // pointer to the global list.
  // This function must be called before any user code has observed the newly loaded DSO.
  bool AfterLoad(soinfo* si, soinfo *solist);

  // Update shadow before unloading a DSO.
  void BeforeUnload(soinfo* si);

  // This is called as soon as the initial set of libraries is linked.
  bool InitialLinkDone(soinfo *solist);

  // Handle failure to locate __cfi_check for a target address.
  static void CfiFail(uint64_t CallSiteTypeId, void* Ptr, void* DiagData, void *caller_pc);
};

CFIShadowWriter* get_cfi_shadow();

#endif // _LINKER_CFI_H_
