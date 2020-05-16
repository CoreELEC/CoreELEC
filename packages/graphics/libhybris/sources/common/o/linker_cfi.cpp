/*
 * Copyright (C) 2008 The Android Open Source Project
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

#include "linker_cfi.h"

#include "linker_debug.h"
#include "linker_globals.h"
#include "linker_utils.h"
#include "private/bionic_page.h"
#include "private/bionic_prctl.h"

#include <sys/mman.h>
#include <sys/types.h>
#include <cstdint>

// Update shadow without making it writable by preparing the data on the side and mremap-ing it in
// place.
class ShadowWrite {
  char* shadow_start;
  char* shadow_end;
  char* aligned_start;
  char* aligned_end;
  char* tmp_start;

 public:
  ShadowWrite(uint16_t* s, uint16_t* e) {
    shadow_start = reinterpret_cast<char*>(s);
    shadow_end = reinterpret_cast<char*>(e);
    aligned_start = reinterpret_cast<char*>(PAGE_START(reinterpret_cast<uintptr_t>(shadow_start)));
    aligned_end = reinterpret_cast<char*>(PAGE_END(reinterpret_cast<uintptr_t>(shadow_end)));
    tmp_start =
        reinterpret_cast<char*>(mmap(nullptr, aligned_end - aligned_start, PROT_READ | PROT_WRITE,
                                     MAP_PRIVATE | MAP_ANONYMOUS, -1, 0));
    CHECK(tmp_start != MAP_FAILED);
    memcpy(tmp_start, aligned_start, shadow_start - aligned_start);
    memcpy(tmp_start + (shadow_end - aligned_start), shadow_end, aligned_end - shadow_end);
  }

  uint16_t* begin() {
    return reinterpret_cast<uint16_t*>(tmp_start + (shadow_start - aligned_start));
  }

  uint16_t* end() {
    return reinterpret_cast<uint16_t*>(tmp_start + (shadow_end - aligned_start));
  }

  ~ShadowWrite() {
    size_t size = aligned_end - aligned_start;
    mprotect(tmp_start, size, PROT_READ);
    void* res = mremap(tmp_start, size, size, MREMAP_MAYMOVE | MREMAP_FIXED,
                       reinterpret_cast<void*>(aligned_start));
    CHECK(res != MAP_FAILED);
  }
};

void CFIShadowWriter::FixupVmaName() {
  prctl(PR_SET_VMA, PR_SET_VMA_ANON_NAME, *shadow_start, kShadowSize, "cfi shadow");
}

void CFIShadowWriter::AddConstant(uintptr_t begin, uintptr_t end, uint16_t v) {
  uint16_t* shadow_begin = MemToShadow(begin);
  uint16_t* shadow_end = MemToShadow(end - 1) + 1;

  ShadowWrite sw(shadow_begin, shadow_end);
  std::fill(sw.begin(), sw.end(), v);
}

void CFIShadowWriter::AddUnchecked(uintptr_t begin, uintptr_t end) {
  AddConstant(begin, end, kUncheckedShadow);
}

void CFIShadowWriter::AddInvalid(uintptr_t begin, uintptr_t end) {
  AddConstant(begin, end, kInvalidShadow);
}

void CFIShadowWriter::Add(uintptr_t begin, uintptr_t end, uintptr_t cfi_check) {
  CHECK((cfi_check & (kCfiCheckAlign - 1)) == 0);

  // Don't fill anything below cfi_check. We can not represent those addresses
  // in the shadow, and must make sure at codegen to place all valid call
  // targets above cfi_check.
  begin = std::max(begin, cfi_check) & ~(kShadowAlign - 1);
  uint16_t* shadow_begin = MemToShadow(begin);
  uint16_t* shadow_end = MemToShadow(end - 1) + 1;

  ShadowWrite sw(shadow_begin, shadow_end);
  uint16_t sv_begin = ((begin + kShadowAlign - cfi_check) >> kCfiCheckGranularity) + kRegularShadowMin;

  // With each step of the loop below, __cfi_check address computation base is increased by
  // 2**ShadowGranularity.
  // To compensate for that, each next shadow value must be increased by 2**ShadowGranularity /
  // 2**CfiCheckGranularity.
  uint16_t sv_step = 1 << (kShadowGranularity - kCfiCheckGranularity);
  uint16_t sv = sv_begin;
  for (uint16_t& s : sw) {
    if (sv < sv_begin) {
      // If shadow value wraps around, also fall back to unchecked. This means the binary is too
      // large. FIXME: consider using a (slow) resolution function instead.
      s = kUncheckedShadow;
      continue;
    }
    // If there is something there already, fall back to unchecked. This may happen in rare cases
    // with MAP_FIXED libraries. FIXME: consider using a (slow) resolution function instead.
    s = (s == kInvalidShadow) ? sv : kUncheckedShadow;
    sv += sv_step;
  }
}

static soinfo* find_libdl(soinfo* solist) {
  for (soinfo* si = solist; si != nullptr; si = si->next) {
    const char* soname = si->get_soname();
    if (soname && strcmp(soname, "libdl.so") == 0) {
      return si;
    }
  }
  return nullptr;
}

static uintptr_t soinfo_find_symbol(soinfo* si, const char* s) {
  SymbolName name(s);
  const ElfW(Sym) * sym;
  if (si->find_symbol_by_name(name, nullptr, &sym) && sym) {
    return si->resolve_symbol_address(sym);
  }
  return 0;
}

uintptr_t soinfo_find_cfi_check(soinfo* si) {
  return soinfo_find_symbol(si, "__cfi_check");
}

uintptr_t CFIShadowWriter::MapShadow() {
  void* p =
      mmap(nullptr, kShadowSize, PROT_READ, MAP_PRIVATE | MAP_ANONYMOUS | MAP_NORESERVE, -1, 0);
  CHECK(p != MAP_FAILED);
  return reinterpret_cast<uintptr_t>(p);
}

bool CFIShadowWriter::AddLibrary(soinfo* si) {
  CHECK(shadow_start != nullptr);
  if (si->base == 0 || si->size == 0) {
    return true;
  }
  uintptr_t cfi_check = soinfo_find_cfi_check(si);
  if (cfi_check == 0) {
    INFO("[ CFI add 0x%zx + 0x%zx %s ]", static_cast<uintptr_t>(si->base),
         static_cast<uintptr_t>(si->size), si->get_soname());
    AddUnchecked(si->base, si->base + si->size);
    return true;
  }

  INFO("[ CFI add 0x%zx + 0x%zx %s: 0x%zx ]", static_cast<uintptr_t>(si->base),
       static_cast<uintptr_t>(si->size), si->get_soname(), cfi_check);
#ifdef __arm__
  // Require Thumb encoding.
  if ((cfi_check & 1UL) != 1UL) {
    DL_ERR("__cfi_check in not a Thumb function in the library \"%s\"", si->get_soname());
    return false;
  }
  cfi_check &= ~1UL;
#endif
  if ((cfi_check & (kCfiCheckAlign - 1)) != 0) {
    DL_ERR("unaligned __cfi_check in the library \"%s\"", si->get_soname());
    return false;
  }
  Add(si->base, si->base + si->size, cfi_check);
  return true;
}

// Pass the shadow mapping address to libdl.so. In return, we get an pointer to the location
// libdl.so uses to store the address.
bool CFIShadowWriter::NotifyLibDl(soinfo* solist, uintptr_t p) {
  soinfo* libdl = find_libdl(solist);
  if (libdl == nullptr) {
    DL_ERR("CFI could not find libdl");
    return false;
  }

  uintptr_t cfi_init = soinfo_find_symbol(libdl, "__cfi_init");
  CHECK(cfi_init != 0);
  shadow_start = reinterpret_cast<uintptr_t* (*)(uintptr_t)>(cfi_init)(p);
  CHECK(shadow_start != nullptr);
  CHECK(*shadow_start == p);
  mprotect(shadow_start, PAGE_SIZE, PROT_READ);
  return true;
}

bool CFIShadowWriter::MaybeInit(soinfo* new_si, soinfo* solist) {
  CHECK(initial_link_done);
  CHECK(shadow_start == nullptr);
  // Check if CFI shadow must be initialized at this time.
  bool found = false;
  if (new_si == nullptr) {
    // This is the case when we've just completed the initial link. There may have been earlier
    // calls to MaybeInit that were skipped. Look though the entire solist.
    for (soinfo* si = solist; si != nullptr; si = si->next) {
      if (soinfo_find_cfi_check(si)) {
        found = true;
        break;
      }
    }
  } else {
    // See if the new library uses CFI.
    found = soinfo_find_cfi_check(new_si);
  }

  // Nothing found.
  if (!found) {
    return true;
  }

  // Init shadow and add all currently loaded libraries (not just the new ones).
  if (!NotifyLibDl(solist, MapShadow()))
    return false;
  for (soinfo* si = solist; si != nullptr; si = si->next) {
    if (!AddLibrary(si))
      return false;
  }
  FixupVmaName();
  return true;
}

bool CFIShadowWriter::AfterLoad(soinfo* si, soinfo* solist) {
  if (!initial_link_done) {
    // Too early.
    return true;
  }

  if (shadow_start == nullptr) {
    return MaybeInit(si, solist);
  }

  // Add the new library to the CFI shadow.
  if (!AddLibrary(si))
    return false;
  FixupVmaName();
  return true;
}

void CFIShadowWriter::BeforeUnload(soinfo* si) {
  if (shadow_start == nullptr) return;
  if (si->base == 0 || si->size == 0) return;
  INFO("[ CFI remove 0x%zx + 0x%zx: %s ]", static_cast<uintptr_t>(si->base),
       static_cast<uintptr_t>(si->size), si->get_soname());
  AddInvalid(si->base, si->base + si->size);
  FixupVmaName();
}

bool CFIShadowWriter::InitialLinkDone(soinfo* solist) {
  CHECK(!initial_link_done);
  initial_link_done = true;
  return MaybeInit(nullptr, solist);
}

// Find __cfi_check in the caller and let it handle the problem. Since caller_pc is likely not a
// valid CFI target, we can not use CFI shadow for lookup. This does not need to be fast, do the
// regular symbol lookup.
void CFIShadowWriter::CfiFail(uint64_t CallSiteTypeId, void* Ptr, void* DiagData, void* CallerPc) {
  soinfo* si = find_containing_library(CallerPc);
  if (!si) {
    __builtin_trap();
  }

  uintptr_t cfi_check = soinfo_find_cfi_check(si);
  if (!cfi_check) {
    __builtin_trap();
  }

  reinterpret_cast<CFICheckFn>(cfi_check)(CallSiteTypeId, Ptr, DiagData);
}
