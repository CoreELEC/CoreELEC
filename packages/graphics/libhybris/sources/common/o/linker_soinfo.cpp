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

#include "linker_soinfo.h"

#include <dlfcn.h>
#include <elf.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#include <async_safe/log.h>
#include <android/api-level.h>

#include "linker_debug.h"
#include "linker_globals.h"
#include "linker_logger.h"
#include "linker_utils.h"

#include "hybris_compat.h"

#define ELF_ST_TYPE(x) ((x) & 0xf)

// TODO(dimitry): These functions are currently located in linker.cpp - find a better place for it
bool find_verdef_version_index(const soinfo* si, const version_info* vi, ElfW(Versym)* versym);
ElfW(Addr) call_ifunc_resolver(ElfW(Addr) resolver_addr);
uint32_t get_application_target_sdk_version();

soinfo::soinfo(android_namespace_t* ns, const char* realpath,
               const struct stat* file_stat, off64_t file_offset,
               int rtld_flags) {

  if (realpath != nullptr) {
    realpath_ = realpath;
  }

  flags_ = FLAG_NEW_SOINFO;
  version_ = SOINFO_VERSION;

  if (file_stat != nullptr) {
    this->st_dev_ = file_stat->st_dev;
    this->st_ino_ = file_stat->st_ino;
    this->file_offset_ = file_offset;
  }

  this->rtld_flags_ = rtld_flags;
  this->primary_namespace_ = ns;
}

soinfo::~soinfo() {
  g_soinfo_handles_map.erase(handle_);
}

void soinfo::set_dt_runpath(const char* path) {
  if (!has_min_version(3)) {
    return;
  }

  std::vector<std::string> runpaths;

  split_path(path, ":", &runpaths);

  std::string origin = dirname(get_realpath());
  // FIXME: add $LIB and $PLATFORM.
  std::vector<std::pair<std::string, std::string>> params = {{"ORIGIN", origin}};
  for (auto&& s : runpaths) {
    format_string(&s, params);
  }

  resolve_paths(runpaths, &dt_runpath_);
}

const ElfW(Versym)* soinfo::get_versym(size_t n) const {
  if (has_min_version(2) && versym_ != nullptr) {
    return versym_ + n;
  }

  return nullptr;
}

ElfW(Addr) soinfo::get_verneed_ptr() const {
  if (has_min_version(2)) {
    return verneed_ptr_;
  }

  return 0;
}

size_t soinfo::get_verneed_cnt() const {
  if (has_min_version(2)) {
    return verneed_cnt_;
  }

  return 0;
}

ElfW(Addr) soinfo::get_verdef_ptr() const {
  if (has_min_version(2)) {
    return verdef_ptr_;
  }

  return 0;
}

size_t soinfo::get_verdef_cnt() const {
  if (has_min_version(2)) {
    return verdef_cnt_;
  }

  return 0;
}

bool soinfo::find_symbol_by_name(SymbolName& symbol_name,
                                 const version_info* vi,
                                 const ElfW(Sym)** symbol) const {
  uint32_t symbol_index;
  bool success =
      is_gnu_hash() ?
      gnu_lookup(symbol_name, vi, &symbol_index) :
      elf_lookup(symbol_name, vi, &symbol_index);

  if (success) {
    *symbol = symbol_index == 0 ? nullptr : symtab_ + symbol_index;
  }

  return success;
}

static bool is_symbol_global_and_defined(const soinfo* si, const ElfW(Sym)* s) {
  if (ELF_ST_BIND(s->st_info) == STB_GLOBAL ||
      ELF_ST_BIND(s->st_info) == STB_WEAK) {
    return s->st_shndx != SHN_UNDEF;
  } else if (ELF_ST_BIND(s->st_info) != STB_LOCAL) {
    DL_WARN("unexpected ST_BIND value: %d for \"%s\" in \"%s\"",
            ELF_ST_BIND(s->st_info), si->get_string(s->st_name), si->get_realpath());
  }

  return false;
}

static const ElfW(Versym) kVersymHiddenBit = 0x8000;

static inline bool is_versym_hidden(const ElfW(Versym)* versym) {
  // the symbol is hidden if bit 15 of versym is set.
  return versym != nullptr && (*versym & kVersymHiddenBit) != 0;
}

static inline bool check_symbol_version(const ElfW(Versym) verneed,
                                        const ElfW(Versym)* verdef) {
  return verneed == kVersymNotNeeded ||
      verdef == nullptr ||
      verneed == (*verdef & ~kVersymHiddenBit);
}

bool soinfo::gnu_lookup(SymbolName& symbol_name,
                        const version_info* vi,
                        uint32_t* symbol_index) const {
  uint32_t hash = symbol_name.gnu_hash();
  uint32_t h2 = hash >> gnu_shift2_;

  uint32_t bloom_mask_bits = sizeof(ElfW(Addr))*8;
  uint32_t word_num = (hash / bloom_mask_bits) & gnu_maskwords_;
  ElfW(Addr) bloom_word = gnu_bloom_filter_[word_num];

  *symbol_index = 0;

  TRACE_TYPE(LOOKUP, "SEARCH %s in %s@%p (gnu)",
      symbol_name.get_name(), get_realpath(), reinterpret_cast<void*>(base));

  // test against bloom filter
  if ((1 & (bloom_word >> (hash % bloom_mask_bits)) & (bloom_word >> (h2 % bloom_mask_bits))) == 0) {
    TRACE_TYPE(LOOKUP, "NOT FOUND %s in %s@%p",
        symbol_name.get_name(), get_realpath(), reinterpret_cast<void*>(base));

    return true;
  }

  // bloom test says "probably yes"...
  uint32_t n = gnu_bucket_[hash % gnu_nbucket_];

  if (n == 0) {
    TRACE_TYPE(LOOKUP, "NOT FOUND %s in %s@%p",
        symbol_name.get_name(), get_realpath(), reinterpret_cast<void*>(base));

    return true;
  }

  // lookup versym for the version definition in this library
  // note the difference between "version is not requested" (vi == nullptr)
  // and "version not found". In the first case verneed is kVersymNotNeeded
  // which implies that the default version can be accepted; the second case results in
  // verneed = 1 (kVersymGlobal) and implies that we should ignore versioned symbols
  // for this library and consider only *global* ones.
  ElfW(Versym) verneed = 0;
  if (!find_verdef_version_index(this, vi, &verneed)) {
    return false;
  }

  do {
    ElfW(Sym)* s = symtab_ + n;
    const ElfW(Versym)* verdef = get_versym(n);
    // skip hidden versions when verneed == kVersymNotNeeded (0)
    if (verneed == kVersymNotNeeded && is_versym_hidden(verdef)) {
        continue;
    }
    if (((gnu_chain_[n] ^ hash) >> 1) == 0 &&
        check_symbol_version(verneed, verdef) &&
        strcmp(get_string(s->st_name), symbol_name.get_name()) == 0 &&
        is_symbol_global_and_defined(this, s)) {
      TRACE_TYPE(LOOKUP, "FOUND %s in %s (%p) %zd",
          symbol_name.get_name(), get_realpath(), reinterpret_cast<void*>(s->st_value),
          static_cast<size_t>(s->st_size));
      *symbol_index = n;
      return true;
    }
  } while ((gnu_chain_[n++] & 1) == 0);

  TRACE_TYPE(LOOKUP, "NOT FOUND %s in %s@%p",
             symbol_name.get_name(), get_realpath(), reinterpret_cast<void*>(base));

  return true;
}

bool soinfo::elf_lookup(SymbolName& symbol_name,
                        const version_info* vi,
                        uint32_t* symbol_index) const {
  uint32_t hash = symbol_name.elf_hash();

  TRACE_TYPE(LOOKUP, "SEARCH %s in %s@%p h=%x(elf) %zd",
             symbol_name.get_name(), get_realpath(),
             reinterpret_cast<void*>(base), hash, hash % nbucket_);

  ElfW(Versym) verneed = 0;
  if (!find_verdef_version_index(this, vi, &verneed)) {
    return false;
  }

  for (uint32_t n = bucket_[hash % nbucket_]; n != 0; n = chain_[n]) {
    ElfW(Sym)* s = symtab_ + n;
    const ElfW(Versym)* verdef = get_versym(n);

    // skip hidden versions when verneed == 0
    if (verneed == kVersymNotNeeded && is_versym_hidden(verdef)) {
        continue;
    }

    if (check_symbol_version(verneed, verdef) &&
        strcmp(get_string(s->st_name), symbol_name.get_name()) == 0 &&
        is_symbol_global_and_defined(this, s)) {
      TRACE_TYPE(LOOKUP, "FOUND %s in %s (%p) %zd",
                 symbol_name.get_name(), get_realpath(),
                 reinterpret_cast<void*>(s->st_value),
                 static_cast<size_t>(s->st_size));
      *symbol_index = n;
      return true;
    }
  }

  TRACE_TYPE(LOOKUP, "NOT FOUND %s in %s@%p %x %zd",
             symbol_name.get_name(), get_realpath(),
             reinterpret_cast<void*>(base), hash, hash % nbucket_);

  *symbol_index = 0;
  return true;
}

ElfW(Sym)* soinfo::find_symbol_by_address(const void* addr) {
  return is_gnu_hash() ? gnu_addr_lookup(addr) : elf_addr_lookup(addr);
}

static bool symbol_matches_soaddr(const ElfW(Sym)* sym, ElfW(Addr) soaddr) {
  return sym->st_shndx != SHN_UNDEF &&
      soaddr >= sym->st_value &&
      soaddr < sym->st_value + sym->st_size;
}

ElfW(Sym)* soinfo::gnu_addr_lookup(const void* addr) {
  ElfW(Addr) soaddr = reinterpret_cast<ElfW(Addr)>(addr) - load_bias;

  for (size_t i = 0; i < gnu_nbucket_; ++i) {
    uint32_t n = gnu_bucket_[i];

    if (n == 0) {
      continue;
    }

    do {
      ElfW(Sym)* sym = symtab_ + n;
      if (symbol_matches_soaddr(sym, soaddr)) {
        return sym;
      }
    } while ((gnu_chain_[n++] & 1) == 0);
  }

  return nullptr;
}

ElfW(Sym)* soinfo::elf_addr_lookup(const void* addr) {
  ElfW(Addr) soaddr = reinterpret_cast<ElfW(Addr)>(addr) - load_bias;

  // Search the library's symbol table for any defined symbol which
  // contains this address.
  for (size_t i = 0; i < nchain_; ++i) {
    ElfW(Sym)* sym = symtab_ + i;
    if (symbol_matches_soaddr(sym, soaddr)) {
      return sym;
    }
  }

  return nullptr;
}

static void call_function(const char* function_name,
                          linker_ctor_function_t function,
                          const char* realpath) {
  (void)realpath;
  if (function == nullptr || reinterpret_cast<uintptr_t>(function) == static_cast<uintptr_t>(-1)) {
    return;
  }

  TRACE("[ Calling c-tor %s @ %p for '%s' ]", function_name, function, realpath);
  function(g_argc, g_argv, g_envp);
  TRACE("[ Done calling c-tor %s @ %p for '%s' ]", function_name, function, realpath);
}

static void call_function(const char* function_name,
                          linker_dtor_function_t function,
                          const char* realpath) {
  (void)function_name;
  (void)realpath;
  if (function == nullptr || reinterpret_cast<uintptr_t>(function) == static_cast<uintptr_t>(-1)) {
    return;
  }

  TRACE("[ Calling d-tor %s @ %p for '%s' ]", function_name, function, realpath);
  function();
  TRACE("[ Done calling d-tor %s @ %p for '%s' ]", function_name, function, realpath);
}

template <typename F>
static void call_array(const char* array_name,
                       F* functions,
                       size_t count,
                       bool reverse,
                       const char* realpath) {
  if (functions == nullptr) {
    return;
  }

  TRACE("[ Calling %s (size %zd) @ %p for '%s' ]", array_name, count, functions, realpath);

  int begin = reverse ? (count - 1) : 0;
  int end = reverse ? -1 : count;
  int step = reverse ? -1 : 1;

  for (int i = begin; i != end; i += step) {
    TRACE("[ %s[%d] == %p ]", array_name, i, functions[i]);
    call_function("function", functions[i], realpath);
  }

  TRACE("[ Done calling %s for '%s' ]", array_name, realpath);
}

void soinfo::call_pre_init_constructors() {
  // DT_PREINIT_ARRAY functions are called before any other constructors for executables,
  // but ignored in a shared library.
  call_array("DT_PREINIT_ARRAY", preinit_array_, preinit_array_count_, false, get_realpath());
}

// Defined somewhere else in this linker.
extern "C" void* android_dlsym(void* handle, const char* symbol);

void (*bionic___system_properties_init)(void) = NULL;

void soinfo::call_constructors() {
  if (constructors_called) {
    return;
  }

  if (soname_ != nullptr && strcmp(soname_, "libc.so") == 0) {
    DEBUG("HYBRIS: =============> Skipping libc.so (but initializing properties)\n");
    bionic___system_properties_init = (void(*)())android_dlsym(this, "__system_properties_init");
    if (!bionic___system_properties_init) {
        fprintf(stderr, "Could not initialize android system properties!\n");
        abort();
    }
    bionic___system_properties_init();
    constructors_called = true;
    return;
  }

  // We set constructors_called before actually calling the constructors, otherwise it doesn't
  // protect against recursive constructor calls. One simple example of constructor recursion
  // is the libc debug malloc, which is implemented in libc_malloc_debug_leak.so:
  // 1. The program depends on libc, so libc's constructor is called here.
  // 2. The libc constructor calls dlopen() to load libc_malloc_debug_leak.so.
  // 3. dlopen() calls the constructors on the newly created
  //    soinfo for libc_malloc_debug_leak.so.
  // 4. The debug .so depends on libc, so CallConstructors is
  //    called again with the libc soinfo. If it doesn't trigger the early-
  //    out above, the libc constructor will be called again (recursively!).
  constructors_called = true;

  if (!is_main_executable() && preinit_array_ != nullptr) {
    // The GNU dynamic linker silently ignores these, but we warn the developer.
    PRINT("\"%s\": ignoring DT_PREINIT_ARRAY in shared library!", get_realpath());
  }

  get_children().for_each([] (soinfo* si) {
    si->call_constructors();
  });

  if (!is_linker()) {
    bionic_trace_begin((std::string("calling constructors: ") + get_realpath()).c_str());
  }

  // DT_INIT should be called before DT_INIT_ARRAY if both are present.
  call_function("DT_INIT", init_func_, get_realpath());
  call_array("DT_INIT_ARRAY", init_array_, init_array_count_, false, get_realpath());

  if (!is_linker()) {
    bionic_trace_end();
  }
}

void soinfo::call_destructors() {
  if (!constructors_called || (soname_ != nullptr && (strcmp(soname_, "libc.so") == 0))) {
    return;
  }

  ScopedTrace trace((std::string("calling destructors: ") + get_realpath()).c_str());

  // DT_FINI_ARRAY must be parsed in reverse order.
  call_array("DT_FINI_ARRAY", fini_array_, fini_array_count_, true, get_realpath());

  // DT_FINI should be called after DT_FINI_ARRAY if both are present.
  call_function("DT_FINI", fini_func_, get_realpath());
}

void soinfo::add_child(soinfo* child) {
  if (has_min_version(0)) {
    child->parents_.push_back(this);
    this->children_.push_back(child);
  }
}

void soinfo::remove_all_links() {
  if (!has_min_version(0)) {
    return;
  }

  // 1. Untie connected soinfos from 'this'.
  children_.for_each([&] (soinfo* child) {
    child->parents_.remove_if([&] (const soinfo* parent) {
      return parent == this;
    });
  });

  parents_.for_each([&] (soinfo* parent) {
    parent->children_.remove_if([&] (const soinfo* child) {
      return child == this;
    });
  });

  // 2. Remove from the primary namespace
  primary_namespace_->remove_soinfo(this);
  primary_namespace_ = nullptr;

  // 3. Remove from secondary namespaces
  secondary_namespaces_.for_each([&](android_namespace_t* ns) {
    ns->remove_soinfo(this);
  });


  // 4. Once everything untied - clear local lists.
  parents_.clear();
  children_.clear();
  secondary_namespaces_.clear();
}

dev_t soinfo::get_st_dev() const {
  if (has_min_version(0)) {
    return st_dev_;
  }

  return 0;
};

ino_t soinfo::get_st_ino() const {
  if (has_min_version(0)) {
    return st_ino_;
  }

  return 0;
}

off64_t soinfo::get_file_offset() const {
  if (has_min_version(1)) {
    return file_offset_;
  }

  return 0;
}

uint32_t soinfo::get_rtld_flags() const {
  if (has_min_version(1)) {
    return rtld_flags_;
  }

  return 0;
}

uint32_t soinfo::get_dt_flags_1() const {
  if (has_min_version(1)) {
    return dt_flags_1_;
  }

  return 0;
}

void soinfo::set_dt_flags_1(uint32_t dt_flags_1) {
  if (has_min_version(1)) {
    if ((dt_flags_1 & DF_1_GLOBAL) != 0) {
      rtld_flags_ |= RTLD_GLOBAL;
    }

    if ((dt_flags_1 & DF_1_NODELETE) != 0) {
      rtld_flags_ |= RTLD_NODELETE;
    }

    dt_flags_1_ = dt_flags_1;
  }
}

void soinfo::set_nodelete() {
  rtld_flags_ |= RTLD_NODELETE;
}

const char* soinfo::get_realpath() const {
#if defined(__work_around_b_24465209__)
  if (has_min_version(2)) {
    return realpath_.c_str();
  } else {
    return old_name_;
  }
#else
  return realpath_.c_str();
#endif
}

void soinfo::set_soname(const char* soname) {
#if defined(__work_around_b_24465209__)
  if (has_min_version(2)) {
    soname_ = soname;
  }
  strlcpy(old_name_, soname_, sizeof(old_name_));
#else
  soname_ = soname;
#endif
}

const char* soinfo::get_soname() const {
#if defined(__work_around_b_24465209__)
  if (has_min_version(2)) {
    return soname_;
  } else {
    return old_name_;
  }
#else
  return soname_;
#endif
}

// This is a return on get_children()/get_parents() if
// 'this->flags' does not have FLAG_NEW_SOINFO set.
static soinfo_list_t g_empty_list;

soinfo_list_t& soinfo::get_children() {
  if (has_min_version(0)) {
    return children_;
  }

  return g_empty_list;
}

const soinfo_list_t& soinfo::get_children() const {
  if (has_min_version(0)) {
    return children_;
  }

  return g_empty_list;
}

soinfo_list_t& soinfo::get_parents() {
  if (has_min_version(0)) {
    return parents_;
  }

  return g_empty_list;
}

static std::vector<std::string> g_empty_runpath;

const std::vector<std::string>& soinfo::get_dt_runpath() const {
  if (has_min_version(3)) {
    return dt_runpath_;
  }

  return g_empty_runpath;
}

android_namespace_t* soinfo::get_primary_namespace() {
  if (has_min_version(3)) {
    return primary_namespace_;
  }

  return g_default_namespace;
}

void soinfo::add_secondary_namespace(android_namespace_t* secondary_ns) {
  CHECK(has_min_version(3));
  secondary_namespaces_.push_back(secondary_ns);
}

android_namespace_list_t& soinfo::get_secondary_namespaces() {
  CHECK(has_min_version(3));
  return secondary_namespaces_;
}

ElfW(Addr) soinfo::resolve_symbol_address(const ElfW(Sym)* s) const {
  if (ELF_ST_TYPE(s->st_info) == STT_GNU_IFUNC) {
    return call_ifunc_resolver(s->st_value + load_bias);
  }

  return static_cast<ElfW(Addr)>(s->st_value + load_bias);
}

const char* soinfo::get_string(ElfW(Word) index) const {
  if (has_min_version(1) && (index >= strtab_size_)) {
    async_safe_fatal("%s: strtab out of bounds error; STRSZ=%zd, name=%d",
        get_realpath(), strtab_size_, index);
  }

  return strtab_ + index;
}

bool soinfo::is_gnu_hash() const {
  return (flags_ & FLAG_GNU_HASH) != 0;
}

bool soinfo::can_unload() const {
  return !is_linked() || ((get_rtld_flags() & (RTLD_NODELETE | RTLD_GLOBAL)) == 0);
}

bool soinfo::is_linked() const {
  return (flags_ & FLAG_LINKED) != 0;
}

bool soinfo::is_main_executable() const {
  return (flags_ & FLAG_EXE) != 0;
}

bool soinfo::is_linker() const {
  return (flags_ & FLAG_LINKER) != 0;
}

void soinfo::set_linked() {
  flags_ |= FLAG_LINKED;
}

void soinfo::set_linker_flag() {
  flags_ |= FLAG_LINKER;
}

void soinfo::set_main_executable() {
  flags_ |= FLAG_EXE;
}

void soinfo::increment_ref_count() {
  local_group_root_->ref_count_++;
}

size_t soinfo::decrement_ref_count() {
  return --local_group_root_->ref_count_;
}

soinfo* soinfo::get_local_group_root() const {
  return local_group_root_;
}

void soinfo::set_mapped_by_caller(bool mapped_by_caller) {
  if (mapped_by_caller) {
    flags_ |= FLAG_MAPPED_BY_CALLER;
  } else {
    flags_ &= ~FLAG_MAPPED_BY_CALLER;
  }
}

bool soinfo::is_mapped_by_caller() const {
  return (flags_ & FLAG_MAPPED_BY_CALLER) != 0;
}

// This function returns api-level at the time of
// dlopen/load. Note that libraries opened by system
// will always have 'current' api level.
uint32_t soinfo::get_target_sdk_version() const {
  if (!has_min_version(2)) {
    return __ANDROID_API__;
  }

  return local_group_root_->target_sdk_version_;
}

uintptr_t soinfo::get_handle() const {
  CHECK(has_min_version(3));
  CHECK(handle_ != 0);
  return handle_;
}

void* soinfo::to_handle() {
  if (get_application_target_sdk_version() < __ANDROID_API_N__ || !has_min_version(3)) {
    return this;
  }

  return reinterpret_cast<void*>(get_handle());
}

void soinfo::generate_handle() {
  CHECK(has_min_version(3));
  CHECK(handle_ == 0); // Make sure this is the first call

  // Make sure the handle is unique and does not collide
  // with special values which are RTLD_DEFAULT and RTLD_NEXT.
  do {
    handle_ = rand();
    //arc4random_buf(&handle_, sizeof(handle_));
    // the least significant bit for the handle is always 1
    // making it easy to test the type of handle passed to
    // dl* functions.
    handle_ = handle_ | 1;
  } while (handle_ == reinterpret_cast<uintptr_t>(RTLD_DEFAULT) ||
           handle_ == reinterpret_cast<uintptr_t>(RTLD_NEXT) ||
           g_soinfo_handles_map.find(handle_) != g_soinfo_handles_map.end());

  g_soinfo_handles_map[handle_] = this;
}

// TODO(dimitry): Move SymbolName methods to a separate file.

uint32_t calculate_elf_hash(const char* name) {
  const uint8_t* name_bytes = reinterpret_cast<const uint8_t*>(name);
  uint32_t h = 0, g;

  while (*name_bytes) {
    h = (h << 4) + *name_bytes++;
    g = h & 0xf0000000;
    h ^= g;
    h ^= g >> 24;
  }

  return h;
}

uint32_t SymbolName::elf_hash() {
  if (!has_elf_hash_) {
    elf_hash_ = calculate_elf_hash(name_);
    has_elf_hash_ = true;
  }

  return elf_hash_;
}

uint32_t SymbolName::gnu_hash() {
  if (!has_gnu_hash_) {
    uint32_t h = 5381;
    const uint8_t* name = reinterpret_cast<const uint8_t*>(name_);
    while (*name != 0) {
      h += (h << 5) + *name++; // h*33 + c = h + h * 32 + c = h + h << 5 + c
    }

    gnu_hash_ =  h;
    has_gnu_hash_ = true;
  }

  return gnu_hash_;
}
