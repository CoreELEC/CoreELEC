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

#ifndef _LINKER_H_
#define _LINKER_H_

#include <elf.h>
#include <inttypes.h>
#include <link.h>
#include <unistd.h>
#include <android/dlext.h>
#include <sys/stat.h>

#include "private/libc_logging.h"
#include "linked_list.h"

#include <string>
#include <vector>

#define DL_ERR(fmt, x...) \
    do { \
      fprintf(stderr, fmt, ##x); \
      fprintf(stderr, "\n"); \
      /* If LD_DEBUG is set high enough, log every dlerror(3) message. */ \
      DEBUG("%s\n", linker_get_error_buffer()); \
    } while (false)

#define DL_WARN(fmt, x...) \
    do { \
      fprintf(stderr, "WARNING: linker " fmt, ##x); \
      fprintf(stderr, "\n"); \
    } while (false)

#if defined(__LP64__)
#define ELFW(what) ELF64_ ## what
#else
#define ELFW(what) ELF32_ ## what
#endif

// mips64 interprets Elf64_Rel structures' r_info field differently.
// bionic (like other C libraries) has macros that assume regular ELF files,
// but the dynamic linker needs to be able to load mips64 ELF files.
#if defined(__mips__) && defined(__LP64__)
#undef ELF64_R_SYM
#undef ELF64_R_TYPE
#undef ELF64_R_INFO
#define ELF64_R_SYM(info)   (((info) >> 0) & 0xffffffff)
#define ELF64_R_SSYM(info)  (((info) >> 32) & 0xff)
#define ELF64_R_TYPE3(info) (((info) >> 40) & 0xff)
#define ELF64_R_TYPE2(info) (((info) >> 48) & 0xff)
#define ELF64_R_TYPE(info)  (((info) >> 56) & 0xff)
#endif

// Returns the address of the page containing address 'x'.
#define PAGE_START(x)  ((x) & PAGE_MASK)

// Returns the offset of address 'x' in its page.
#define PAGE_OFFSET(x) ((x) & ~PAGE_MASK)

// Returns the address of the next page after address 'x', unless 'x' is
// itself at the start of a page.
#define PAGE_END(x)    PAGE_START((x) + (PAGE_SIZE-1))

#define FLAG_LINKED     0x00000001
#define FLAG_EXE        0x00000004 // The main executable
#define FLAG_LINKER     0x00000010 // The linker itself
#define FLAG_GNU_HASH   0x00000040 // uses gnu hash
#define FLAG_NEW_SOINFO 0x40000000 // new soinfo format

#define SUPPORTED_DT_FLAGS_1 (DF_1_NOW | DF_1_GLOBAL | DF_1_NODELETE)

#define SOINFO_VERSION 2

#if defined(__work_around_b_19059885__)
#define SOINFO_NAME_LEN 128
#endif

typedef void (*linker_function_t)();

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

class SymbolName {
 public:
  explicit SymbolName(const char* name)
      : name_(name), has_elf_hash_(false), has_gnu_hash_(false),
        elf_hash_(0), gnu_hash_(0) { }

  const char* get_name() {
    return name_;
  }

  uint32_t elf_hash();
  uint32_t gnu_hash();

 private:
  const char* name_;
  bool has_elf_hash_;
  bool has_gnu_hash_;
  uint32_t elf_hash_;
  uint32_t gnu_hash_;

  DISALLOW_IMPLICIT_CONSTRUCTORS(SymbolName);
};

struct version_info {
  version_info() : elf_hash(0), name(nullptr), target_si(nullptr) {}

  uint32_t elf_hash;
  const char* name;
  const soinfo* target_si;
};

// Class used construct version dependency graph.
class VersionTracker {
 public:
  VersionTracker() = default;
  bool init(const soinfo* si_from);

  const version_info* get_version_info(ElfW(Versym) source_symver) const;
 private:
  bool init_verneed(const soinfo* si_from);
  bool init_verdef(const soinfo* si_from);
  void add_version_info(size_t source_index, ElfW(Word) elf_hash,
      const char* ver_name, const soinfo* target_si);

  std::vector<version_info> version_infos;

  DISALLOW_COPY_AND_ASSIGN(VersionTracker);
};

struct soinfo {
 public:
  typedef LinkedList<soinfo, SoinfoListAllocator> soinfo_list_t;
#if defined(__work_around_b_19059885__)
 private:
  char old_name_[SOINFO_NAME_LEN];
#endif
 public:
  const ElfW(Phdr)* phdr;
  size_t phnum;
  ElfW(Addr) entry;
  ElfW(Addr) base;
  size_t size;

#if defined(__work_around_b_19059885__)
  uint32_t unused1;  // DO NOT USE, maintained for compatibility.
#endif

  ElfW(Dyn)* dynamic;

#if defined(__work_around_b_19059885__)
  uint32_t unused2; // DO NOT USE, maintained for compatibility
  uint32_t unused3; // DO NOT USE, maintained for compatibility
#endif

  soinfo* next;
 private:
  uint32_t flags_;

  const char* strtab_;
  ElfW(Sym)* symtab_;

  size_t nbucket_;
  size_t nchain_;
  uint32_t* bucket_;
  uint32_t* chain_;

#if defined(__mips__) || !defined(__LP64__)
  // This is only used by mips and mips64, but needs to be here for
  // all 32-bit architectures to preserve binary compatibility.
  ElfW(Addr)** plt_got_;
#endif

#if defined(USE_RELA)
  ElfW(Rela)* plt_rela_;
  size_t plt_rela_count_;

  ElfW(Rela)* rela_;
  size_t rela_count_;
#else
  ElfW(Rel)* plt_rel_;
  size_t plt_rel_count_;

  ElfW(Rel)* rel_;
  size_t rel_count_;
#endif

  linker_function_t* preinit_array_;
  size_t preinit_array_count_;

  linker_function_t* init_array_;
  size_t init_array_count_;
  linker_function_t* fini_array_;
  size_t fini_array_count_;

  linker_function_t init_func_;
  linker_function_t fini_func_;

#if defined(__arm__)
 public:
  // ARM EABI section used for stack unwinding.
  uint32_t* ARM_exidx;
  size_t ARM_exidx_count;
 private:
#elif defined(__mips__)
  uint32_t mips_symtabno_;
  uint32_t mips_local_gotno_;
  uint32_t mips_gotsym_;
  bool mips_relocate_got(const VersionTracker& version_tracker,
                         const soinfo_list_t& global_group,
                         const soinfo_list_t& local_group);

#endif
  size_t ref_count_;
 public:
  link_map link_map_head;

  bool constructors_called;

  // When you read a virtual address from the ELF file, add this
  // value to get the corresponding address in the process' address space.
  ElfW(Addr) load_bias;

#if !defined(__LP64__)
  bool has_text_relocations;
#endif
  bool has_DT_SYMBOLIC;

 public:
  soinfo(const char* name, const struct stat* file_stat, off64_t file_offset, int rtld_flags);

  void call_constructors();
  void call_destructors();
  void call_pre_init_constructors();
  bool prelink_image();
  bool link_image(const soinfo_list_t& global_group, const soinfo_list_t& local_group,
                  const android_dlextinfo* extinfo);

  void add_child(soinfo* child);
  void remove_all_links();

  ino_t get_st_ino() const;
  dev_t get_st_dev() const;
  off64_t get_file_offset() const;

  uint32_t get_rtld_flags() const;
  uint32_t get_dt_flags_1() const;
  void set_dt_flags_1(uint32_t dt_flags_1);

  soinfo_list_t& get_children();
  const soinfo_list_t& get_children() const;

  soinfo_list_t& get_parents();

  bool find_symbol_by_name(SymbolName& symbol_name,
                           const version_info* vi,
                           const ElfW(Sym)** symbol) const;

  ElfW(Sym)* find_symbol_by_address(const void* addr);
  ElfW(Addr) resolve_symbol_address(const ElfW(Sym)* s) const;

  const char* get_string(ElfW(Word) index) const;
  bool can_unload() const;
  bool is_gnu_hash() const;

  bool inline has_min_version(uint32_t min_version) const {
#if defined(__work_around_b_19059885__)
    (void) min_version;
    return (flags_ & FLAG_NEW_SOINFO) != 0 && version_ >= min_version;
#else
    return true;
#endif
  }

  bool is_linked() const;
  bool is_main_executable() const;

  void set_linked();
  void set_linker_flag();
  void set_main_executable();

  void increment_ref_count();
  size_t decrement_ref_count();

  soinfo* get_local_group_root() const;

  const char* get_soname() const;
  const char* get_realpath() const;
  const ElfW(Versym)* get_versym(size_t n) const;
  ElfW(Addr) get_verneed_ptr() const;
  size_t get_verneed_cnt() const;
  ElfW(Addr) get_verdef_ptr() const;
  size_t get_verdef_cnt() const;

  bool find_verdef_version_index(const version_info* vi, ElfW(Versym)* versym) const;

  uint32_t get_target_sdk_version() const;

 private:
  bool elf_lookup(SymbolName& symbol_name, const version_info* vi, uint32_t* symbol_index) const;
  ElfW(Sym)* elf_addr_lookup(const void* addr);
  bool gnu_lookup(SymbolName& symbol_name, const version_info* vi, uint32_t* symbol_index) const;
  ElfW(Sym)* gnu_addr_lookup(const void* addr);

  bool lookup_version_info(const VersionTracker& version_tracker, ElfW(Word) sym,
                           const char* sym_name, const version_info** vi);

  void call_array(const char* array_name, linker_function_t* functions, size_t count, bool reverse);
  void call_function(const char* function_name, linker_function_t function);
  template<typename ElfRelIteratorT>
  bool relocate(const VersionTracker& version_tracker, ElfRelIteratorT&& rel_iterator,
                const soinfo_list_t& global_group, const soinfo_list_t& local_group);

 private:
  // This part of the structure is only available
  // when FLAG_NEW_SOINFO is set in this->flags.
  uint32_t version_;

  // version >= 0
  dev_t st_dev_;
  ino_t st_ino_;

  // dependency graph
  soinfo_list_t children_;
  soinfo_list_t parents_;

  // version >= 1
  off64_t file_offset_;
  uint32_t rtld_flags_;
  uint32_t dt_flags_1_;
  size_t strtab_size_;

  // version >= 2

  size_t gnu_nbucket_;
  uint32_t* gnu_bucket_;
  uint32_t* gnu_chain_;
  uint32_t gnu_maskwords_;
  uint32_t gnu_shift2_;
  ElfW(Addr)* gnu_bloom_filter_;

  soinfo* local_group_root_;

  uint8_t* android_relocs_;
  size_t android_relocs_size_;

  const char* soname_;
  std::string realpath_;

  const ElfW(Versym)* versym_;

  ElfW(Addr) verdef_ptr_;
  size_t verdef_cnt_;

  ElfW(Addr) verneed_ptr_;
  size_t verneed_cnt_;

  uint32_t target_sdk_version_;

  friend soinfo* get_libdl_info();
};

bool soinfo_do_lookup(soinfo* si_from, const char* name, const version_info* vi,
                      soinfo** si_found_in, const soinfo::soinfo_list_t& global_group,
                      const soinfo::soinfo_list_t& local_group, const ElfW(Sym)** symbol);

enum RelocationKind {
  kRelocAbsolute = 0,
  kRelocRelative,
  kRelocCopy,
  kRelocSymbol,
  kRelocMax
};

void count_relocation(RelocationKind kind);

soinfo* get_libdl_info();

void do_android_get_LD_LIBRARY_PATH(char*, size_t);
void do_android_update_LD_LIBRARY_PATH(const char* ld_library_path);
soinfo* do_dlopen(const char* name, int flags, const android_dlextinfo* extinfo);
void do_dlclose(soinfo* si);

int do_dl_iterate_phdr(int (*cb)(dl_phdr_info* info, size_t size, void* data), void* data);

const ElfW(Sym)* dlsym_linear_lookup(const char* name, soinfo** found, soinfo* caller, void* handle);
soinfo* find_containing_library(const void* addr);

const ElfW(Sym)* dlsym_handle_lookup(soinfo* si, soinfo** found, const char* name);

void debuggerd_init();
extern "C" abort_msg_t* g_abort_message;
extern "C" void notify_gdb_of_libraries();

char* linker_get_error_buffer();
size_t linker_get_error_buffer_size();

void set_application_target_sdk_version(uint32_t target);
uint32_t get_application_target_sdk_version();

#endif
