/*
 * Copyright (C) 2008, 2009 The Android Open Source Project
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

#include "config.h"

#include <linux/auxvec.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <dlfcn.h>
#include <sys/stat.h>

#include <pthread.h>

#include <sys/mman.h>

/* special private C library header - see Android.mk */
//#include "bionic_tls.h"

#include "linker.h"
#include "linker_debug.h"
#include "linker_environ.h"
#include "linker_format.h"

#define ALLOW_SYMBOLS_FROM_MAIN 1
#define SO_MAX 128

/* Assume average path length of 64 and max 8 paths */
#define LDPATH_BUFSIZE 512
#define LDPATH_MAX 8

#define LDPRELOAD_BUFSIZE 512
#define LDPRELOAD_MAX 8

/* >>> IMPORTANT NOTE - READ ME BEFORE MODIFYING <<<
 *
 * Do NOT use malloc() and friends or pthread_*() code here.
 * Don't use printf() either; it's caused mysterious memory
 * corruption in the past.
 * The linker runs before we bring up libc and it's easiest
 * to make sure it does not depend on any complex libc features
 *
 * open issues / todo:
 *
 * - are we doing everything we should for ARM_COPY relocations?
 * - cleaner error reporting
 * - after linking, set as much stuff as possible to READONLY
 *   and NOEXEC
 * - linker hardcodes PAGE_SIZE and PAGE_MASK because the kernel
 *   headers provide versions that are negative...
 * - allocate space for soinfo structs dynamically instead of
 *   having a hard limit (64)
*/

static void* (*_get_hooked_symbol)(const char *symbol, const char *requester);

static int link_image(soinfo *si, unsigned wr_offset);

static int socount = 0;
static soinfo sopool[SO_MAX];
static soinfo *freelist = NULL;
static soinfo *solist = &libdl_info;
static soinfo *sonext = &libdl_info;
#if ALLOW_SYMBOLS_FROM_MAIN
static soinfo *somain; /* main process, always the one after libdl_info */
#endif


static inline int validate_soinfo(soinfo *si)
{
    return (si >= sopool && si < sopool + SO_MAX) ||
        si == &libdl_info;
}

static char ldpaths_buf[LDPATH_BUFSIZE];
static const char *ldpaths[LDPATH_MAX + 1];

static char ldpreloads_buf[LDPRELOAD_BUFSIZE];
static const char *ldpreload_names[LDPRELOAD_MAX + 1];

static soinfo *preloads[LDPRELOAD_MAX + 1];

#if LINKER_DEBUG
int debug_verbosity = 0;
int debug_stdout = 0;
#endif

static int pid;

/* This boolean is set if the program being loaded is setuid */
static int program_is_setuid;

#if STATS
struct _link_stats linker_stats;
#endif

#if COUNT_PAGES
unsigned bitmask[4096];
#endif

#ifndef PT_ARM_EXIDX
#define PT_ARM_EXIDX    0x70000001      /* .ARM.exidx segment */
#endif

#if 0
// disable abort() since this is not a linker anymore
#define HOODLUM(name, ret, ...)                                               \
    ret name __VA_ARGS__                                                      \
    {                                                                         \
        char errstr[] = "ERROR: " #name " called from the dynamic linker!\n"; \
        write(2, errstr, sizeof(errstr));                                     \
        abort();                                                              \
    }
HOODLUM(malloc, void *, (size_t size));
HOODLUM(free, void, (void *ptr));
HOODLUM(realloc, void *, (void *ptr, size_t size));
HOODLUM(calloc, void *, (size_t cnt, size_t size));
#endif

static char tmp_err_buf[768];
static char __linker_dl_err_buf[768];
#define DL_ERR(fmt, x...)                                                     \
    do {                                                                      \
        format_buffer(__linker_dl_err_buf, sizeof(__linker_dl_err_buf),            \
                 "%s[%d]: " fmt, __func__, __LINE__, ##x);                    \
        ERROR(fmt "\n", ##x);                                                      \
    } while(0)

const char *linker_get_error(void)
{
    return (const char *)&__linker_dl_err_buf[0];
}

/*
 * This function is an empty stub where GDB locates a breakpoint to get notified
 * about linker activity.
 */
extern void __attribute__((noinline)) rtld_db_dlactivity(void);

struct r_debug _r_debug = {1, NULL, &rtld_db_dlactivity,
                                  RT_CONSISTENT, 0};
static struct link_map *r_debug_tail = 0;

static pthread_mutex_t _r_debug_lock = PTHREAD_MUTEX_INITIALIZER;

static int _linker_enable_gdb_support = 0;

static void insert_soinfo_into_debug_map(soinfo * info)
{
    if (!_linker_enable_gdb_support) return;

    struct link_map * map;

    /* Copy the necessary fields into the debug structure.
     */
    map = &(info->linkmap);
    map->l_addr = info->base;
    map->l_name = (char*) info->name;
    map->l_ld = (uintptr_t)info->dynamic;

    /* Stick the new library at the end of the list.
     * gdb tends to care more about libc than it does
     * about leaf libraries, and ordering it this way
     * reduces the back-and-forth over the wire.
     */

    ///// PATCHED: we don't want libhybris modifying glibc's
    /////          link_map objects, which should not be linked
    /////          to bionic's stripped link_map objects.
    /////        ==> make a copy of the whole chain
    if(r_debug_tail == 0 && _r_debug.r_map != 0) {
      struct link_map *glibc_link_map = malloc(sizeof(struct link_map));
      glibc_link_map->l_addr = _r_debug.r_map->l_addr;
      glibc_link_map->l_name = _r_debug.r_map->l_name;
      glibc_link_map->l_ld = _r_debug.r_map->l_ld;
      glibc_link_map->l_next = _r_debug.r_map->l_next;
      glibc_link_map->l_prev = _r_debug.r_map->l_prev;

      r_debug_tail = glibc_link_map;

      while(glibc_link_map->l_next != 0) {
        struct link_map *copy_next_link_map = malloc(sizeof(struct link_map));
        copy_next_link_map->l_addr = glibc_link_map->l_next->l_addr;
        copy_next_link_map->l_name = glibc_link_map->l_next->l_name;
        copy_next_link_map->l_ld = glibc_link_map->l_next->l_ld;
        copy_next_link_map->l_next = glibc_link_map->l_next->l_next;
        copy_next_link_map->l_prev = glibc_link_map->l_next->l_prev;

        glibc_link_map->l_next = copy_next_link_map;
        copy_next_link_map->l_prev = glibc_link_map;

        glibc_link_map = copy_next_link_map;
      }
    }

    if (r_debug_tail) {
        r_debug_tail->l_prev = map;
        map->l_next = r_debug_tail;
        map->l_prev = 0;
    } else {
        _r_debug.r_map = map;
        map->l_prev = 0;
        map->l_next = 0;
    }
    _r_debug.r_map = r_debug_tail = map;
}

static void remove_soinfo_from_debug_map(soinfo * info)
{
    if (!_linker_enable_gdb_support) return;
    
    struct link_map * map = &(info->linkmap);

    if (r_debug_tail == map)
        r_debug_tail = map->l_prev;

    if (map->l_prev) map->l_prev->l_next = map->l_next;
    if (map->l_next) map->l_next->l_prev = map->l_prev;
}

void notify_gdb_of_load(soinfo * info)
{
    if (info->flags & FLAG_EXE) {
        // GDB already knows about the main executable
        return;
    }

    pthread_mutex_lock(&_r_debug_lock);

    _r_debug.r_state = RT_ADD;
    rtld_db_dlactivity();

    insert_soinfo_into_debug_map(info);

    _r_debug.r_state = RT_CONSISTENT;
    rtld_db_dlactivity();

    pthread_mutex_unlock(&_r_debug_lock);
}

void notify_gdb_of_unload(soinfo * info)
{
    if (info->flags & FLAG_EXE) {
        // GDB already knows about the main executable
        return;
    }

    pthread_mutex_lock(&_r_debug_lock);

    _r_debug.r_state = RT_DELETE;
    rtld_db_dlactivity();

    remove_soinfo_from_debug_map(info);

    _r_debug.r_state = RT_CONSISTENT;
    rtld_db_dlactivity();

    pthread_mutex_unlock(&_r_debug_lock);
}

void notify_gdb_of_libraries()
{
    pthread_mutex_lock(&_r_debug_lock);
    _r_debug.r_state = RT_ADD;
    rtld_db_dlactivity();
    _r_debug.r_state = RT_CONSISTENT;
    rtld_db_dlactivity();
    pthread_mutex_unlock(&_r_debug_lock);
}

static soinfo *alloc_info(const char *name)
{
    soinfo *si;

    if(strlen(name) >= SOINFO_NAME_LEN) {
        DL_ERR("%5d library name %s too long", pid, name);
        return NULL;
    }

    /* The freelist is populated when we call free_info(), which in turn is
       done only by dlclose(), which is not likely to be used.
    */
    if (!freelist) {
        if(socount == SO_MAX) {
            DL_ERR("%5d too many libraries when loading %s", pid, name);
            return NULL;
        }
        freelist = sopool + socount++;
        freelist->next = NULL;
    }

    si = freelist;
    freelist = freelist->next;

    /* Make sure we get a clean block of soinfo */
    memset(si, 0, sizeof(soinfo));
    strlcpy((char*) si->name, name, sizeof(si->name));
    sonext->next = si;
    si->next = NULL;
    si->refcount = 0;
    sonext = si;

    TRACE("%5d name %s: allocated soinfo @ %p\n", pid, name, si);
    return si;
}

static void free_info(soinfo *si)
{
    soinfo *prev = NULL, *trav;

    TRACE("%5d name %s: freeing soinfo @ %p\n", pid, si->name, si);

    for(trav = solist; trav != NULL; trav = trav->next){
        if (trav == si)
            break;
        prev = trav;
    }
    if (trav == NULL) {
        /* si was not ni solist */
        DL_ERR("%5d name %s is not in solist!", pid, si->name);
        return;
    }

    /* prev will never be NULL, because the first entry in solist is
       always the static libdl_info.
    */
    prev->next = si->next;
    if (si == sonext) sonext = prev;
    si->next = freelist;
    freelist = si;
}

const char *addr_to_name(unsigned addr)
{
    soinfo *si;

    for(si = solist; si != 0; si = si->next){
        if((addr >= si->base) && (addr < (si->base + si->size))) {
            return si->name;
        }
    }

    return "";
}

/* For a given PC, find the .so that it belongs to.
 * Returns the base address of the .ARM.exidx section
 * for that .so, and the number of 8-byte entries
 * in that section (via *pcount).
 *
 * Intended to be called by libc's __gnu_Unwind_Find_exidx().
 *
 * This function is exposed via dlfcn.c and libdl.so.
 */
#ifdef ANDROID_ARM_LINKER
_Unwind_Ptr android_dl_unwind_find_exidx(_Unwind_Ptr pc, int *pcount)
{
    soinfo *si;
    unsigned addr = (unsigned)pc;

    for (si = solist; si != 0; si = si->next){
        if ((addr >= si->base) && (addr < (si->base + si->size))) {
            *pcount = si->ARM_exidx_count;
            return (_Unwind_Ptr)(si->base + (unsigned long)si->ARM_exidx);
        }
    }
   *pcount = 0;
    return NULL;
}
#endif
/* Here, we only have to provide a callback to iterate across all the
 * loaded libraries. gcc_eh does the rest. */
int
android_dl_iterate_phdr(int (*cb)(struct dl_phdr_info *info, size_t size, void *data),
                void *data)
{
    soinfo *si;
    struct dl_phdr_info dl_info;
    int rv = 0;

    for (si = solist; si != NULL; si = si->next) {
        dl_info.dlpi_addr = si->linkmap.l_addr;
        dl_info.dlpi_name = si->linkmap.l_name;
        dl_info.dlpi_phdr = si->phdr;
        dl_info.dlpi_phnum = si->phnum;
        rv = cb(&dl_info, sizeof (struct dl_phdr_info), data);
        if (rv != 0)
            break;
    }
    return rv;
}

static Elf_Sym *_elf_lookup(soinfo *si, unsigned hash, const char *name)
{
    Elf_Sym *s;
    Elf_Sym *symtab = si->symtab;
    const char *strtab = si->strtab;
    unsigned n;

    TRACE_TYPE(LOOKUP, "%5d SEARCH %s in %s@0x%08x %08x %d\n", pid,
               name, si->name, si->base, hash, hash % si->nbucket);
    n = hash % si->nbucket;

    for(n = si->bucket[hash % si->nbucket]; n != 0; n = si->chain[n]){
        s = symtab + n;
        if(strcmp(strtab + s->st_name, name)) continue;

            /* only concern ourselves with global and weak symbol definitions */
        switch(ELF32_ST_BIND(s->st_info)){
        case STB_GLOBAL:
        case STB_WEAK:
                /* no section == undefined */
            if(s->st_shndx == 0) continue;

            TRACE_TYPE(LOOKUP, "%5d FOUND %s in %s (%08x) %d\n", pid,
                       name, si->name, s->st_value, s->st_size);
            return s;
        }
    }

    return NULL;
}

static unsigned elfhash(const char *_name)
{
    const unsigned char *name = (const unsigned char *) _name;
    unsigned h = 0, g;

    while(*name) {
        h = (h << 4) + *name++;
        g = h & 0xf0000000;
        h ^= g;
        h ^= g >> 24;
    }
    return h;
}

static Elf_Sym *
_do_lookup(soinfo *si, const char *name, unsigned *base)
{
    unsigned elf_hash = elfhash(name);
    Elf_Sym *s;
    unsigned *d;
    soinfo *lsi = si;
    int i;

    /* Look for symbols in the local scope (the object who is
     * searching). This happens with C++ templates on i386 for some
     * reason.
     *
     * Notes on weak symbols:
     * The ELF specs are ambigious about treatment of weak definitions in
     * dynamic linking.  Some systems return the first definition found
     * and some the first non-weak definition.   This is system dependent.
     * Here we return the first definition found for simplicity.  */

    s = _elf_lookup(si, elf_hash, name);
    if(s != NULL)
        goto done;

    /* Next, look for it in the preloads list */
    for(i = 0; preloads[i] != NULL; i++) {
        lsi = preloads[i];
        s = _elf_lookup(lsi, elf_hash, name);
        if(s != NULL)
            goto done;
    }

    for(d = si->dynamic; *d; d += 2) {
        if(d[0] == DT_NEEDED){
            lsi = (soinfo *)d[1];
            if (!validate_soinfo(lsi)) {
                DL_ERR("%5d bad DT_NEEDED pointer in %s",
                       pid, si->name);
                return NULL;
            }

            DEBUG("%5d %s: looking up %s in %s\n",
                  pid, si->name, name, lsi->name);
            s = _elf_lookup(lsi, elf_hash, name);
            if ((s != NULL) && (s->st_shndx != SHN_UNDEF))
                goto done;
        }
    }

#if ALLOW_SYMBOLS_FROM_MAIN
    /* If we are resolving relocations while dlopen()ing a library, it's OK for
     * the library to resolve a symbol that's defined in the executable itself,
     * although this is rare and is generally a bad idea.
     */
    if (somain) {
        lsi = somain;
        DEBUG("%5d %s: looking up %s in executable %s\n",
              pid, si->name, name, lsi->name);
        s = _elf_lookup(lsi, elf_hash, name);
    }
#endif

done:
    if(s != NULL) {
        TRACE_TYPE(LOOKUP, "%5d si %s sym %s s->st_value = 0x%08x, "
                   "found in %s, base = 0x%08x\n",
                   pid, si->name, name, s->st_value, lsi->name, lsi->base);
        *base = lsi->base;
        return s;
    }

    return NULL;
}

/* This is used by dl_sym().  It performs symbol lookup only within the
   specified soinfo object and not in any of its dependencies.
 */
Elf_Sym *lookup_in_library(soinfo *si, const char *name)
{
    return _elf_lookup(si, elfhash(name), name);
}

/* This is used by dl_sym().  It performs a global symbol lookup.
 */
Elf_Sym *lookup(const char *name, soinfo **found, soinfo *start)
{
    unsigned elf_hash = elfhash(name);
    Elf_Sym *s = NULL;
    soinfo *si;

    if(start == NULL) {
        start = solist;
    }

    for(si = start; (s == NULL) && (si != NULL); si = si->next)
    {
        if(si->flags & FLAG_ERROR)
            continue;
        s = _elf_lookup(si, elf_hash, name);
        if (s != NULL) {
            *found = si;
            break;
        }
    }

    if(s != NULL) {
        TRACE_TYPE(LOOKUP, "%5d %s s->st_value = 0x%08x, "
                   "si->base = 0x%08x\n", pid, name, s->st_value, si->base);
        return s;
    }

    return NULL;
}

soinfo *find_containing_library(const void *addr)
{
    soinfo *si;

    for(si = solist; si != NULL; si = si->next)
    {
        if((unsigned)addr >= si->base && (unsigned)addr - si->base < si->size) {
            return si;
        }
    }

    return NULL;
}

Elf_Sym *find_containing_symbol(const void *addr, soinfo *si)
{
    unsigned int i;
    unsigned soaddr = (unsigned)addr - si->base;

    /* Search the library's symbol table for any defined symbol which
     * contains this address */
    for(i=0; i<si->nchain; i++) {
        Elf_Sym *sym = &si->symtab[i];

        if(sym->st_shndx != SHN_UNDEF &&
           soaddr >= sym->st_value &&
           soaddr < sym->st_value + sym->st_size) {
            return sym;
        }
    }

    return NULL;
}

#if 0
static void dump(soinfo *si)
{
    Elf_Sym *s = si->symtab;
    unsigned n;

    for(n = 0; n < si->nchain; n++) {
        TRACE("%5d %04d> %08x: %02x %04x %08x %08x %s\n", pid, n, s,
               s->st_info, s->st_shndx, s->st_value, s->st_size,
               si->strtab + s->st_name);
        s++;
    }
}
#endif

static const char *sopaths[] = {
    "/vendor/lib",
    "/system/lib",
    0
};

static int _open_lib(const char *name)
{
    int fd;
    struct stat filestat;

    if ((stat(name, &filestat) >= 0) && S_ISREG(filestat.st_mode)) {
        if ((fd = open(name, O_RDONLY)) >= 0)
            return fd;
    }

    return -1;
}

static void parse_library_path(const char *path, char *delim);

static int open_library(const char *name)
{
    int fd;
    char buf[512];
    const char **path;
    int n;

    TRACE("[ %5d opening %s ]\n", pid, name);

    if(name == 0) return -1;
    if(strlen(name) > 256) return -1;

    if ((name[0] == '/') && (getenv("HYBRIS_LINKER_FORCE_BASENAME") != NULL))
    {
        const char *bname = strrchr(name, '/');
        if ((bname != NULL) && ((fd = open_library(bname+1)) >= 0))
            return fd;
    }

    if ((name[0] == '/') && ((fd = _open_lib(name)) >= 0))
        return fd;

#ifdef DEFAULT_HYBRIS_LD_LIBRARY_PATH
    if (getenv("HYBRIS_LD_LIBRARY_PATH") == NULL && *ldpaths == 0)
    {
        parse_library_path(DEFAULT_HYBRIS_LD_LIBRARY_PATH, ":");
    }
#endif
    if (getenv("HYBRIS_LD_LIBRARY_PATH") != NULL && *ldpaths == 0)
    {
        parse_library_path(getenv("HYBRIS_LD_LIBRARY_PATH"), ":");
    }

    for (path = ldpaths; *path; path++) {
        n = format_buffer(buf, sizeof(buf), "%s/%s", *path, name);
        if (n < 0 || n >= (int)sizeof(buf)) {
            WARN("Ignoring very long library path: %s/%s\n", *path, name);
            continue;
        }
        if ((fd = _open_lib(buf)) >= 0)
            return fd;
    }
    for (path = sopaths; *path; path++) {
        n = format_buffer(buf, sizeof(buf), "%s/%s", *path, name);
        if (n < 0 || n >= (int)sizeof(buf)) {
            WARN("Ignoring very long library path: %s/%s\n", *path, name);
            continue;
        }
        if ((fd = _open_lib(buf)) >= 0)
            return fd;
    }

    return -1;
}

/* temporary space for holding the first page of the shared lib
 * which contains the elf header (with the pht). */
static unsigned char __header[PAGE_SIZE];

typedef struct {
    long mmap_addr;
    char tag[4]; /* 'P', 'R', 'E', ' ' */
} prelink_info_t;

/* Returns the requested base address if the library is prelinked,
 * and 0 otherwise.  */
static unsigned long
is_prelinked(int fd, const char *name)
{
    off_t sz;
    prelink_info_t info;

    sz = lseek(fd, -sizeof(prelink_info_t), SEEK_END);
    if (sz < 0) {
        DL_ERR("lseek() failed!");
        return 0;
    }

    if (read(fd, &info, sizeof(info)) != sizeof(info)) {
        INFO("Could not read prelink_info_t structure for `%s`\n", name);
        return 0;
    }

    if (strncmp(info.tag, "PRE ", 4)) {
        INFO("`%s` is not a prelinked library\n", name);
        return 0;
    }

    return (unsigned long)info.mmap_addr;
}

/* verify_elf_object
 *      Verifies if the object @ base is a valid ELF object
 *
 * Args:
 *
 * Returns:
 *       0 on success
 *      -1 if no valid ELF object is found @ base.
 */
static int
verify_elf_object(void *base, const char *name)
{
    Elf_Ehdr *hdr = (Elf_Ehdr *) base;

    if (hdr->e_ident[EI_MAG0] != ELFMAG0) return -1;
    if (hdr->e_ident[EI_MAG1] != ELFMAG1) return -1;
    if (hdr->e_ident[EI_MAG2] != ELFMAG2) return -1;
    if (hdr->e_ident[EI_MAG3] != ELFMAG3) return -1;

    /* TODO: Should we verify anything else in the header? */
#ifdef ANDROID_ARM_LINKER
    if (hdr->e_machine != EM_ARM) return -1;
#elif defined(ANDROID_X86_LINKER)
    if (hdr->e_machine != EM_386) return -1;
#endif
    return 0;
}


/* get_lib_extents
 *      Retrieves the base (*base) address where the ELF object should be
 *      mapped and its overall memory size (*total_sz).
 *
 * Args:
 *      fd: Opened file descriptor for the library
 *      name: The name of the library
 *      _hdr: Pointer to the header page of the library
 *      total_sz: Total size of the memory that should be allocated for
 *                this library
 *
 * Returns:
 *      -1 if there was an error while trying to get the lib extents.
 *         The possible reasons are:
 *             - Could not determine if the library was prelinked.
 *             - The library provided is not a valid ELF object
 *       0 if the library did not request a specific base offset (normal
 *         for non-prelinked libs)
 *     > 0 if the library requests a specific address to be mapped to.
 *         This indicates a pre-linked library.
 */
static unsigned
get_lib_extents(int fd, const char *name, void *__hdr, unsigned *total_sz)
{
    unsigned req_base;
    unsigned min_vaddr = 0xffffffff;
    unsigned max_vaddr = 0;
    unsigned char *_hdr = (unsigned char *)__hdr;
    Elf_Ehdr *ehdr = (Elf_Ehdr *)_hdr;
    Elf_Phdr *phdr;
    int cnt;

    TRACE("[ %5d Computing extents for '%s'. ]\n", pid, name);
    if (verify_elf_object(_hdr, name) < 0) {
        DL_ERR("%5d - %s is not a valid ELF object", pid, name);
        return (unsigned)-1;
    }

    req_base = (unsigned) is_prelinked(fd, name);
    if (req_base == (unsigned)-1)
        return -1;
    else if (req_base != 0) {
        TRACE("[ %5d - Prelinked library '%s' requesting base @ 0x%08x ]\n",
              pid, name, req_base);
    } else {
        TRACE("[ %5d - Non-prelinked library '%s' found. ]\n", pid, name);
    }

    phdr = (Elf_Phdr *)(_hdr + ehdr->e_phoff);

    /* find the min/max p_vaddrs from all the PT_LOAD segments so we can
     * get the range. */
    for (cnt = 0; cnt < ehdr->e_phnum; ++cnt, ++phdr) {
        if (phdr->p_type == PT_LOAD) {
            if ((phdr->p_vaddr + phdr->p_memsz) > max_vaddr)
                max_vaddr = phdr->p_vaddr + phdr->p_memsz;
            if (phdr->p_vaddr < min_vaddr)
                min_vaddr = phdr->p_vaddr;
        }
    }

    if ((min_vaddr == 0xffffffff) && (max_vaddr == 0)) {
        DL_ERR("%5d - No loadable segments found in %s.", pid, name);
        return (unsigned)-1;
    }

    /* truncate min_vaddr down to page boundary */
    min_vaddr &= ~PAGE_MASK;

    /* round max_vaddr up to the next page */
    max_vaddr = (max_vaddr + PAGE_SIZE - 1) & ~PAGE_MASK;

    *total_sz = (max_vaddr - min_vaddr);
    return (unsigned)req_base;
}

/* reserve_mem_region
 *
 *     This function reserves a chunk of memory to be used for mapping in
 *     a prelinked shared library. We reserve the entire memory region here, and
 *     then the rest of the linker will relocate the individual loadable
 *     segments into the correct locations within this memory range.
 *
 * Args:
 *     si->base: The requested base of the allocation.
 *     si->size: The size of the allocation.
 *
 * Returns:
 *     -1 on failure, and 0 on success.  On success, si->base will contain
 *     the virtual address at which the library will be mapped.
 */

static int reserve_mem_region(soinfo *si)
{
    void *base = mmap((void *)si->base, si->size, PROT_NONE,
                      MAP_FIXED | MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (base == MAP_FAILED) {
        DL_ERR("%5d can NOT map (%sprelinked) library '%s' at 0x%08x "
              "as requested, will try general pool: %d (%s)",
              pid, (si->base ? "" : "non-"), si->name, si->base,
              errno, strerror(errno));
        return -1;
    } else if (base != (void *)si->base) {
        DL_ERR("OOPS: %5d %sprelinked library '%s' mapped at 0x%08x, "
              "not at 0x%08x", pid, (si->base ? "" : "non-"),
              si->name, (unsigned)base, si->base);
        munmap(base, si->size);
        return -1;
    }
    return 0;
}

static int alloc_mem_region(soinfo *si)
{
    if (si->base) {
        /* Attempt to mmap a prelinked library. */
        return reserve_mem_region(si);
    }

    /* This is not a prelinked library, so we use the kernel's default
       allocator.
    */

    void *base = mmap(NULL, si->size, PROT_NONE,
                      MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (base == MAP_FAILED) {
        DL_ERR("%5d mmap of library '%s' failed: %d (%s)\n",
              pid, si->name,
              errno, strerror(errno));
        goto err;
    }
    si->base = (unsigned) base;
    INFO("%5d mapped library '%s' to %08x via kernel allocator.\n",
          pid, si->name, si->base);
    return 0;

err:
    DL_ERR("OOPS: %5d cannot map library '%s'. no vspace available.",
          pid, si->name);
    return -1;
}

#define MAYBE_MAP_FLAG(x,from,to)    (((x) & (from)) ? (to) : 0)
#define PFLAGS_TO_PROT(x)            (MAYBE_MAP_FLAG((x), PF_X, PROT_EXEC) | \
                                      MAYBE_MAP_FLAG((x), PF_R, PROT_READ) | \
                                      MAYBE_MAP_FLAG((x), PF_W, PROT_WRITE))
/* load_segments
 *
 *     This function loads all the loadable (PT_LOAD) segments into memory
 *     at their appropriate memory offsets off the base address.
 *
 * Args:
 *     fd: Open file descriptor to the library to load.
 *     header: Pointer to a header page that contains the ELF header.
 *             This is needed since we haven't mapped in the real file yet.
 *     si: ptr to soinfo struct describing the shared object.
 *
 * Returns:
 *     0 on success, -1 on failure.
 */
static int
load_segments(int fd, void *header, soinfo *si)
{
    Elf_Ehdr *ehdr = (Elf_Ehdr *)header;
    Elf_Phdr *phdr = (Elf_Phdr *)((unsigned char *)header + ehdr->e_phoff);
    Elf_Addr base = (Elf_Addr) si->base;
    int cnt;
    unsigned len;
    Elf_Addr tmp;
    unsigned char *pbase;
    unsigned char *extra_base;
    unsigned extra_len;
    unsigned total_sz = 0;

    si->wrprotect_start = 0xffffffff;
    si->wrprotect_end = 0;

    TRACE("[ %5d - Begin loading segments for '%s' @ 0x%08x ]\n",
          pid, si->name, (unsigned)si->base);
    /* Now go through all the PT_LOAD segments and map them into memory
     * at the appropriate locations. */
    for (cnt = 0; cnt < ehdr->e_phnum; ++cnt, ++phdr) {
        if (phdr->p_type == PT_LOAD) {
            DEBUG_DUMP_PHDR(phdr, "PT_LOAD", pid);
            /* we want to map in the segment on a page boundary */
            tmp = base + (phdr->p_vaddr & (~PAGE_MASK));
            /* add the # of bytes we masked off above to the total length. */
            len = phdr->p_filesz + (phdr->p_vaddr & PAGE_MASK);

            TRACE("[ %d - Trying to load segment from '%s' @ 0x%08x "
                  "(0x%08x). p_vaddr=0x%08x p_offset=0x%08x ]\n", pid, si->name,
                  (unsigned)tmp, len, phdr->p_vaddr, phdr->p_offset);
            pbase = mmap((void *)tmp, len, PFLAGS_TO_PROT(phdr->p_flags),
                         MAP_PRIVATE | MAP_FIXED, fd,
                         phdr->p_offset & (~PAGE_MASK));
            if (pbase == MAP_FAILED) {
                DL_ERR("%d failed to map segment from '%s' @ 0x%08x (0x%08x). "
                      "p_vaddr=0x%08x p_offset=0x%08x", pid, si->name,
                      (unsigned)tmp, len, phdr->p_vaddr, phdr->p_offset);
                goto fail;
            }

            /* If 'len' didn't end on page boundary, and it's a writable
             * segment, zero-fill the rest. */
            if ((len & PAGE_MASK) && (phdr->p_flags & PF_W))
                memset((void *)(pbase + len), 0, PAGE_SIZE - (len & PAGE_MASK));

            /* Check to see if we need to extend the map for this segment to
             * cover the diff between filesz and memsz (i.e. for bss).
             *
             *  base           _+---------------------+  page boundary
             *                  .                     .
             *                  |                     |
             *                  .                     .
             *  pbase          _+---------------------+  page boundary
             *                  |                     |
             *                  .                     .
             *  base + p_vaddr _|                     |
             *                  . \          \        .
             *                  . | filesz   |        .
             *  pbase + len    _| /          |        |
             *     <0 pad>      .            .        .
             *  extra_base     _+------------|--------+  page boundary
             *               /  .            .        .
             *               |  .            .        .
             *               |  +------------|--------+  page boundary
             *  extra_len->  |  |            |        |
             *               |  .            | memsz  .
             *               |  .            |        .
             *               \ _|            /        |
             *                  .                     .
             *                  |                     |
             *                 _+---------------------+  page boundary
             */
            tmp = (Elf_Addr)(((unsigned)pbase + len + PAGE_SIZE - 1) &
                                    (~PAGE_MASK));
            if (tmp < (base + phdr->p_vaddr + phdr->p_memsz)) {
                extra_len = base + phdr->p_vaddr + phdr->p_memsz - tmp;
                TRACE("[ %5d - Need to extend segment from '%s' @ 0x%08x "
                      "(0x%08x) ]\n", pid, si->name, (unsigned)tmp, extra_len);
                /* map in the extra page(s) as anonymous into the range.
                 * This is probably not necessary as we already mapped in
                 * the entire region previously, but we just want to be
                 * sure. This will also set the right flags on the region
                 * (though we can probably accomplish the same thing with
                 * mprotect).
                 */
                extra_base = mmap((void *)tmp, extra_len,
                                  PFLAGS_TO_PROT(phdr->p_flags),
                                  MAP_PRIVATE | MAP_FIXED | MAP_ANONYMOUS,
                                  -1, 0);
                if (extra_base == MAP_FAILED) {
                    DL_ERR("[ %5d - failed to extend segment from '%s' @ 0x%08x"
                           " (0x%08x) ]", pid, si->name, (unsigned)tmp,
                          extra_len);
                    goto fail;
                }
                /* TODO: Check if we need to memset-0 this region.
                 * Anonymous mappings are zero-filled copy-on-writes, so we
                 * shouldn't need to. */
                TRACE("[ %5d - Segment from '%s' extended @ 0x%08x "
                      "(0x%08x)\n", pid, si->name, (unsigned)extra_base,
                      extra_len);
            }
            /* set the len here to show the full extent of the segment we
             * just loaded, mostly for debugging */
            len = (((unsigned)base + phdr->p_vaddr + phdr->p_memsz +
                    PAGE_SIZE - 1) & (~PAGE_MASK)) - (unsigned)pbase;
            TRACE("[ %5d - Successfully loaded segment from '%s' @ 0x%08x "
                  "(0x%08x). p_vaddr=0x%08x p_offset=0x%08x\n", pid, si->name,
                  (unsigned)pbase, len, phdr->p_vaddr, phdr->p_offset);
            total_sz += len;
            /* Make the section writable just in case we'll have to write to
             * it during relocation (i.e. text segment). However, we will
             * remember what range of addresses should be write protected.
             *
             */
            if (!(phdr->p_flags & PF_W)) {
                if ((unsigned)pbase < si->wrprotect_start)
                    si->wrprotect_start = (unsigned)pbase;
                if (((unsigned)pbase + len) > si->wrprotect_end)
                    si->wrprotect_end = (unsigned)pbase + len;
                mprotect(pbase, len,
                         PFLAGS_TO_PROT(phdr->p_flags) | PROT_WRITE);
            }
        } else if (phdr->p_type == PT_DYNAMIC) {
            DEBUG_DUMP_PHDR(phdr, "PT_DYNAMIC", pid);
            /* this segment contains the dynamic linking information */
            si->dynamic = (unsigned *)(base + phdr->p_vaddr);
        } else if (phdr->p_type == PT_GNU_RELRO) {
            if ((phdr->p_vaddr >= si->size)
                    || ((phdr->p_vaddr + phdr->p_memsz) > si->size)
                    || ((base + phdr->p_vaddr + phdr->p_memsz) < base)) {
                DL_ERR("%d invalid GNU_RELRO in '%s' "
                       "p_vaddr=0x%08x p_memsz=0x%08x", pid, si->name,
                       phdr->p_vaddr, phdr->p_memsz);
                goto fail;
            }
            si->gnu_relro_start = (Elf_Addr) (base + phdr->p_vaddr);
            si->gnu_relro_len = (unsigned) phdr->p_memsz;
        } else {
#ifdef ANDROID_ARM_LINKER
            if (phdr->p_type == PT_ARM_EXIDX) {
                DEBUG_DUMP_PHDR(phdr, "PT_ARM_EXIDX", pid);
                /* exidx entries (used for stack unwinding) are 8 bytes each.
                 */
                si->ARM_exidx = (unsigned *)phdr->p_vaddr;
                si->ARM_exidx_count = phdr->p_memsz / 8;
            }
#endif
        }

    }

    /* Sanity check */
    if (total_sz > si->size) {
        DL_ERR("%5d - Total length (0x%08x) of mapped segments from '%s' is "
              "greater than what was allocated (0x%08x). THIS IS BAD!",
              pid, total_sz, si->name, si->size);
        goto fail;
    }

    TRACE("[ %5d - Finish loading segments for '%s' @ 0x%08x. "
          "Total memory footprint: 0x%08x bytes ]\n", pid, si->name,
          (unsigned)si->base, si->size);
    return 0;

fail:
    /* We can just blindly unmap the entire region even though some things
     * were mapped in originally with anonymous and others could have been
     * been mapped in from the file before we failed. The kernel will unmap
     * all the pages in the range, irrespective of how they got there.
     */
    munmap((void *)si->base, si->size);
    si->flags |= FLAG_ERROR;
    return -1;
}

/* TODO: Implement this to take care of the fact that Android ARM
 * ELF objects shove everything into a single loadable segment that has the
 * write bit set. wr_offset is then used to set non-(data|bss) pages to be
 * non-writable.
 */
#if 0
static unsigned
get_wr_offset(int fd, const char *name, Elf_Ehdr *ehdr)
{
    Elf_Shdr *shdr_start;
    Elf_Shdr *shdr;
    int shdr_sz = ehdr->e_shnum * sizeof(Elf_Shdr);
    int cnt;
    unsigned wr_offset = 0xffffffff;

    shdr_start = mmap(0, shdr_sz, PROT_READ, MAP_PRIVATE, fd,
                      ehdr->e_shoff & (~PAGE_MASK));
    if (shdr_start == MAP_FAILED) {
        WARN("%5d - Could not read section header info from '%s'. Will not "
             "not be able to determine write-protect offset.\n", pid, name);
        return (unsigned)-1;
    }

    for(cnt = 0, shdr = shdr_start; cnt < ehdr->e_shnum; ++cnt, ++shdr) {
        if ((shdr->sh_type != SHT_NULL) && (shdr->sh_flags & SHF_WRITE) &&
            (shdr->sh_addr < wr_offset)) {
            wr_offset = shdr->sh_addr;
        }
    }

    munmap(shdr_start, shdr_sz);
    return wr_offset;
}
#endif

static soinfo *
load_library(const char *name)
{
    int fd = open_library(name);
    int cnt;
    unsigned ext_sz;
    unsigned req_base;
    const char *bname;
    soinfo *si = NULL;
    Elf_Ehdr *hdr;

    if(fd == -1) {
        DL_ERR("Library '%s' not found", name);
        return NULL;
    }

    /* We have to read the ELF header to figure out what to do with this image
     */
    if (lseek(fd, 0, SEEK_SET) < 0) {
        DL_ERR("lseek() failed!");
        goto fail;
    }

    if ((cnt = read(fd, &__header[0], PAGE_SIZE)) < 0) {
        DL_ERR("read() failed!");
        goto fail;
    }

    /* Parse the ELF header and get the size of the memory footprint for
     * the library */
    req_base = get_lib_extents(fd, name, &__header[0], &ext_sz);
    if (req_base == (unsigned)-1)
        goto fail;
    TRACE("[ %5d - '%s' (%s) wants base=0x%08x sz=0x%08x ]\n", pid, name,
          (req_base ? "prelinked" : "not pre-linked"), req_base, ext_sz);

    /* Now configure the soinfo struct where we'll store all of our data
     * for the ELF object. If the loading fails, we waste the entry, but
     * same thing would happen if we failed during linking. Configuring the
     * soinfo struct here is a lot more convenient.
     */
    bname = strrchr(name, '/');
    si = alloc_info(bname ? bname + 1 : name);
    if (si == NULL)
        goto fail;

    /* Carve out a chunk of memory where we will map in the individual
     * segments */
    si->base = req_base;
    si->size = ext_sz;
    si->flags = 0;
    si->entry = 0;
    si->dynamic = (unsigned *)-1;
    if (alloc_mem_region(si) < 0)
        goto fail;

    TRACE("[ %5d allocated memory for %s @ %p (0x%08x) ]\n",
          pid, name, (void *)si->base, (unsigned) ext_sz);

    /* Now actually load the library's segments into right places in memory */
    if (load_segments(fd, &__header[0], si) < 0) {
        goto fail;
    }

    /* this might not be right. Technically, we don't even need this info
     * once we go through 'load_segments'. */
    hdr = (Elf_Ehdr *)si->base;
    si->phdr = (Elf_Phdr *)((unsigned char *)si->base + hdr->e_phoff);
    si->phnum = hdr->e_phnum;
    /**/

    close(fd);
    return si;

fail:
    if (si) free_info(si);
    close(fd);
    return NULL;
}

static soinfo *
init_library(soinfo *si)
{
    unsigned wr_offset = 0xffffffff;

#if LINKER_DEBUG
    /* Has to be set via init_library as we don't get called via the
     * traditional android init library path  */
    const char* env;
    env = getenv("HYBRIS_LINKER_DEBUG");
    if (env)
        debug_verbosity = atoi(env);
    if (getenv("HYBRIS_LINKER_STDOUT"))
        debug_stdout = 1;

    INFO("[ HYBRIS: initializing library '%s']\n", si->name);
#endif

    /* At this point we know that whatever is loaded @ base is a valid ELF
     * shared library whose segments are properly mapped in. */
    TRACE("[ %5d init_library base=0x%08x sz=0x%08x name='%s') ]\n",
          pid, si->base, si->size, si->name);

    if(link_image(si, wr_offset)) {
            /* We failed to link.  However, we can only restore libbase
            ** if no additional libraries have moved it since we updated it.
            */
        munmap((void *)si->base, si->size);
        return NULL;
    }

    return si;
}

soinfo *find_library(const char *name)
{
    soinfo *si;
    const char *bname;

#if ALLOW_SYMBOLS_FROM_MAIN
    if (name == NULL)
        return somain;
#else
    if (name == NULL)
        return NULL;
#endif

    bname = strrchr(name, '/');
    bname = bname ? bname + 1 : name;

    for(si = solist; si != 0; si = si->next){
        if(!strcmp(bname, si->name)) {
            if(si->flags & FLAG_ERROR) {
                DL_ERR("%5d '%s' failed to load previously", pid, bname);
                return NULL;
            }
            if(si->flags & FLAG_LINKED) return si;
            DL_ERR("OOPS: %5d recursive link to '%s'", pid, si->name);
            return NULL;
        }
    }

    TRACE("[ %5d '%s' has not been loaded yet.  Locating...]\n", pid, name);
    si = load_library(name);
    if(si == NULL)
        return NULL;
    return init_library(si);
}

/* TODO:
 *   notify gdb of unload
 *   for non-prelinked libraries, find a way to decrement libbase
 */
static void call_destructors(soinfo *si);
unsigned unload_library(soinfo *si)
{
    unsigned *d;
    if (si->refcount == 1) {
        TRACE("%5d unloading '%s'\n", pid, si->name);
        call_destructors(si);

        /*
         * Make sure that we undo the PT_GNU_RELRO protections we added
         * in link_image. This is needed to undo the DT_NEEDED hack below.
         */
        if ((si->gnu_relro_start != 0) && (si->gnu_relro_len != 0)) {
            Elf_Addr start = (si->gnu_relro_start & ~PAGE_MASK);
            unsigned len = (si->gnu_relro_start - start) + si->gnu_relro_len;
            if (mprotect((void *) start, len, PROT_READ | PROT_WRITE) < 0)
                DL_ERR("%5d %s: could not undo GNU_RELRO protections. "
                       "Expect a crash soon. errno=%d (%s)",
                       pid, si->name, errno, strerror(errno));

        }

        for(d = si->dynamic; *d; d += 2) {
            if(d[0] == DT_NEEDED){
                soinfo *lsi = (soinfo *)d[1];

                // The next line will segfault if the we don't undo the
                // PT_GNU_RELRO protections (see comments above and in
                // link_image().
                d[1] = 0;

                if (validate_soinfo(lsi)) {
                    TRACE("%5d %s needs to unload %s\n", pid,
                          si->name, lsi->name);
                    unload_library(lsi);
                }
                else
                    DL_ERR("%5d %s: could not unload dependent library",
                           pid, si->name);
            }
        }

        munmap((char *)si->base, si->size);
        notify_gdb_of_unload(si);
        free_info(si);
        si->refcount = 0;
    }
    else {
        si->refcount--;
        INFO("%5d not unloading '%s', decrementing refcount to %d\n",
              pid, si->name, si->refcount);
    }
    return si->refcount;
}

/* TODO: don't use unsigned for addrs below. It works, but is not
 * ideal. They should probably be either uint32_t, Elf_Addr, or unsigned
 * long.
 */
static int reloc_library(soinfo *si, Elf_Rel *rel, unsigned count)
{
    Elf_Sym *symtab = si->symtab;
    const char *strtab = si->strtab;
    Elf_Sym *s;
    unsigned base;
    Elf_Rel *start = rel;
    unsigned idx;

    for (idx = 0; idx < count; ++idx) {
        unsigned type = ELF32_R_TYPE(rel->r_info);
        unsigned sym = ELF32_R_SYM(rel->r_info);
        unsigned reloc = (unsigned)(rel->r_offset + si->base);
        unsigned sym_addr = 0;
        char *sym_name = NULL;

        DEBUG("%5d Processing '%s' relocation at index %d\n", pid,
              si->name, idx);
        if(sym != 0) {
            sym_name = (char *)(strtab + symtab[sym].st_name);
            INFO("HYBRIS: '%s' checking hooks for sym '%s'\n", si->name, sym_name);
            sym_addr = (unsigned)_get_hooked_symbol(sym_name, si->name);
            if (sym_addr != 0) {
                INFO("HYBRIS: '%s' hooked symbol %s to %x\n", si->name,
                                                  sym_name, sym_addr);
            } else {
               s = _do_lookup(si, sym_name, &base);
            }
            if(sym_addr == 0) {
            if(s == NULL) {
                /* We only allow an undefined symbol if this is a weak
                   reference..   */
                s = &symtab[sym];
                if (ELF32_ST_BIND(s->st_info) != STB_WEAK && strcmp(si->name, "libdsyscalls.so") != 0) {
                    DL_ERR("%5d cannot locate '%s'...\n", pid, sym_name);
                    return -1;
                }

                /* IHI0044C AAELF 4.5.1.1:

                   Libraries are not searched to resolve weak references.
                   It is not an error for a weak reference to remain
                   unsatisfied.

                   During linking, the value of an undefined weak reference is:
                   - Zero if the relocation type is absolute
                   - The address of the place if the relocation is pc-relative
                   - The address of nominial base address if the relocation
                     type is base-relative.
                  */

                switch (type) {
#if defined(ANDROID_ARM_LINKER)
                case R_ARM_JUMP_SLOT:
                case R_ARM_GLOB_DAT:
                case R_ARM_ABS32:
                case R_ARM_RELATIVE:    /* Don't care. */
                case R_ARM_NONE:        /* Don't care. */
#elif defined(ANDROID_X86_LINKER)
                case R_386_JUMP_SLOT:
                case R_386_GLOB_DAT:
                case R_386_32:
                case R_386_RELATIVE:    /* Dont' care. */
#endif /* ANDROID_*_LINKER */
                    /* sym_addr was initialized to be zero above or relocation
                       code below does not care about value of sym_addr.
                       No need to do anything.  */
                    break;

#if defined(ANDROID_X86_LINKER)
                case R_386_PC32:
                    sym_addr = reloc;
                    break;
#endif /* ANDROID_X86_LINKER */

#if defined(ANDROID_ARM_LINKER)
                case R_ARM_COPY:
                    /* Fall through.  Can't really copy if weak symbol is
                       not found in run-time.  */
#endif /* ANDROID_ARM_LINKER */
                default:
                    DL_ERR("%5d unknown weak reloc type %d @ %p (%d)\n",
                                 pid, type, rel, (int) (rel - start));
                    return -1;
                }
            } else {
                /* We got a definition.  */
#if 0
            if((base == 0) && (si->base != 0)){
                    /* linking from libraries to main image is bad */
                DL_ERR("%5d cannot locate '%s'...",
                       pid, strtab + symtab[sym].st_name);
                return -1;
            }
#endif
                sym_addr = (unsigned)(s->st_value + base);
            }
            }
            COUNT_RELOC(RELOC_SYMBOL);
        } else {
            s = NULL;
        }

/* TODO: This is ugly. Split up the relocations by arch into
 * different files.
 */
        switch(type){
#if defined(ANDROID_ARM_LINKER)
        case R_ARM_JUMP_SLOT:
            COUNT_RELOC(RELOC_ABSOLUTE);
            MARK(rel->r_offset);
            TRACE_TYPE(RELO, "%5d RELO JMP_SLOT %08x <- %08x %s\n", pid,
                       reloc, sym_addr, sym_name);
            *((unsigned*)reloc) = sym_addr;
            break;
        case R_ARM_GLOB_DAT:
            COUNT_RELOC(RELOC_ABSOLUTE);
            MARK(rel->r_offset);
            TRACE_TYPE(RELO, "%5d RELO GLOB_DAT %08x <- %08x %s\n", pid,
                       reloc, sym_addr, sym_name);
            *((unsigned*)reloc) = sym_addr;
            break;
        case R_ARM_ABS32:
            COUNT_RELOC(RELOC_ABSOLUTE);
            MARK(rel->r_offset);
            TRACE_TYPE(RELO, "%5d RELO ABS %08x <- %08x %s\n", pid,
                       reloc, sym_addr, sym_name);
            *((unsigned*)reloc) += sym_addr;
            break;
        case R_ARM_REL32:
            COUNT_RELOC(RELOC_RELATIVE);
            MARK(rel->r_offset);
            TRACE_TYPE(RELO, "%5d RELO REL32 %08x <- %08x - %08x %s\n", pid,
                       reloc, sym_addr, rel->r_offset, sym_name);
            *((unsigned*)reloc) += sym_addr - rel->r_offset;
            break;
#elif defined(ANDROID_X86_LINKER)
        case R_386_JUMP_SLOT:
            COUNT_RELOC(RELOC_ABSOLUTE);
            MARK(rel->r_offset);
            TRACE_TYPE(RELO, "%5d RELO JMP_SLOT %08x <- %08x %s\n", pid,
                       reloc, sym_addr, sym_name);
            *((unsigned*)reloc) = sym_addr;
            break;
        case R_386_GLOB_DAT:
            COUNT_RELOC(RELOC_ABSOLUTE);
            MARK(rel->r_offset);
            TRACE_TYPE(RELO, "%5d RELO GLOB_DAT %08x <- %08x %s\n", pid,
                       reloc, sym_addr, sym_name);
            *((unsigned*)reloc) = sym_addr;
            break;
#endif /* ANDROID_*_LINKER */

#if defined(ANDROID_ARM_LINKER)
        case R_ARM_RELATIVE:
#elif defined(ANDROID_X86_LINKER)
        case R_386_RELATIVE:
#endif /* ANDROID_*_LINKER */
            COUNT_RELOC(RELOC_RELATIVE);
            MARK(rel->r_offset);
            if(sym){
                DL_ERR("%5d odd RELATIVE form...", pid);
                return -1;
            }
            TRACE_TYPE(RELO, "%5d RELO RELATIVE %08x <- +%08x\n", pid,
                       reloc, si->base);
            *((unsigned*)reloc) += si->base;
            break;

#if defined(ANDROID_X86_LINKER)
        case R_386_32:
            COUNT_RELOC(RELOC_RELATIVE);
            MARK(rel->r_offset);

            TRACE_TYPE(RELO, "%5d RELO R_386_32 %08x <- +%08x %s\n", pid,
                       reloc, sym_addr, sym_name);
            *((unsigned *)reloc) += (unsigned)sym_addr;
            break;

        case R_386_PC32:
            COUNT_RELOC(RELOC_RELATIVE);
            MARK(rel->r_offset);
            TRACE_TYPE(RELO, "%5d RELO R_386_PC32 %08x <- "
                       "+%08x (%08x - %08x) %s\n", pid, reloc,
                       (sym_addr - reloc), sym_addr, reloc, sym_name);
            *((unsigned *)reloc) += (unsigned)(sym_addr - reloc);
            break;
#endif /* ANDROID_X86_LINKER */

#ifdef ANDROID_ARM_LINKER
        case R_ARM_COPY:
            COUNT_RELOC(RELOC_COPY);
            MARK(rel->r_offset);
            TRACE_TYPE(RELO, "%5d RELO %08x <- %d @ %08x %s\n", pid,
                       reloc, s->st_size, sym_addr, sym_name);
            memcpy((void*)reloc, (void*)sym_addr, s->st_size);
            break;
        case R_ARM_NONE:
            break;
#endif /* ANDROID_ARM_LINKER */

        default:
            DL_ERR("%5d unknown reloc type %d @ %p (%d)",
                  pid, type, rel, (int) (rel - start));
            return -1;
        }
        rel++;
    }
    return 0;
}

/* Please read the "Initialization and Termination functions" functions.
 * of the linker design note in bionic/linker/README.TXT to understand
 * what the following code is doing.
 *
 * The important things to remember are:
 *
 *   DT_PREINIT_ARRAY must be called first for executables, and should
 *   not appear in shared libraries.
 *
 *   DT_INIT should be called before DT_INIT_ARRAY if both are present
 *
 *   DT_FINI should be called after DT_FINI_ARRAY if both are present
 *
 *   DT_FINI_ARRAY must be parsed in reverse order.
 */

static void call_array(unsigned *ctor, int count, int reverse)
{
    int n, inc = 1;

    if (reverse) {
        ctor += (count-1);
        inc   = -1;
    }

    for(n = count; n > 0; n--) {
        TRACE("[ %5d Looking at %s *0x%08x == 0x%08x ]\n", pid,
              reverse ? "dtor" : "ctor",
              (unsigned)ctor, (unsigned)*ctor);
        void (*func)() = (void (*)()) *ctor;
        ctor += inc;
        if(((int) func == 0) || ((int) func == -1)) continue;
        TRACE("[ %5d Calling func @ 0x%08x ]\n", pid, (unsigned)func);
        func();
    }
}

void call_constructors_recursive(soinfo *si)
{
    if (si->constructors_called)
        return;
    if (strcmp(si->name,"libc.so") == 0) {
        INFO("HYBRIS: =============> Skipping libc.so\n");
        return;
    }

    // Set this before actually calling the constructors, otherwise it doesn't
    // protect against recursive constructor calls. One simple example of
    // constructor recursion is the libc debug malloc, which is implemented in
    // libc_malloc_debug_leak.so:
    // 1. The program depends on libc, so libc's constructor is called here.
    // 2. The libc constructor calls dlopen() to load libc_malloc_debug_leak.so.
    // 3. dlopen() calls call_constructors_recursive() with the newly created
    //    soinfo for libc_malloc_debug_leak.so.
    // 4. The debug so depends on libc, so call_constructors_recursive() is
    //    called again with the libc soinfo. If it doesn't trigger the early-
    //    out above, the libc constructor will be called again (recursively!).
    si->constructors_called = 1;

    if (si->flags & FLAG_EXE) {
        TRACE("[ %5d Calling preinit_array @ 0x%08x [%d] for '%s' ]\n",
              pid, (unsigned)si->preinit_array, si->preinit_array_count,
              si->name);
        call_array(si->preinit_array, si->preinit_array_count, 0);
        TRACE("[ %5d Done calling preinit_array for '%s' ]\n", pid, si->name);
    } else {
        if (si->preinit_array) {
            DL_ERR("%5d Shared library '%s' has a preinit_array table @ 0x%08x."
                   " This is INVALID.", pid, si->name,
                   (unsigned)si->preinit_array);
        }
    }

    if (si->dynamic) {
        unsigned *d;
        for(d = si->dynamic; *d; d += 2) {
            if(d[0] == DT_NEEDED){
                soinfo* lsi = (soinfo *)d[1];
                if (!validate_soinfo(lsi)) {
                    DL_ERR("%5d bad DT_NEEDED pointer in %s",
                           pid, si->name);
                } else {
                    call_constructors_recursive(lsi);
                }
            }
        }
    }

    if (si->init_func) {
        TRACE("[ %5d Calling init_func @ 0x%08x for '%s' ]\n", pid,
              (unsigned)si->init_func, si->name);
        si->init_func();
        TRACE("[ %5d Done calling init_func for '%s' ]\n", pid, si->name);
    }

    if (si->init_array) {
        TRACE("[ %5d Calling init_array @ 0x%08x [%d] for '%s' ]\n", pid,
              (unsigned)si->init_array, si->init_array_count, si->name);
        call_array(si->init_array, si->init_array_count, 0);
        TRACE("[ %5d Done calling init_array for '%s' ]\n", pid, si->name);
    }

}

static void call_destructors(soinfo *si)
{
    if (si->fini_array) {
        TRACE("[ %5d Calling fini_array @ 0x%08x [%d] for '%s' ]\n", pid,
              (unsigned)si->fini_array, si->fini_array_count, si->name);
        call_array(si->fini_array, si->fini_array_count, 1);
        TRACE("[ %5d Done calling fini_array for '%s' ]\n", pid, si->name);
    }

    if (si->fini_func) {
        TRACE("[ %5d Calling fini_func @ 0x%08x for '%s' ]\n", pid,
              (unsigned)si->fini_func, si->name);
        si->fini_func();
        TRACE("[ %5d Done calling fini_func for '%s' ]\n", pid, si->name);
    }
}

/* Force any of the closed stdin, stdout and stderr to be associated with
   /dev/null. */
static int nullify_closed_stdio (void)
{
    int dev_null, i, status;
    int return_value = 0;

    dev_null = open("/dev/null", O_RDWR);
    if (dev_null < 0) {
        DL_ERR("Cannot open /dev/null.");
        return -1;
    }
    TRACE("[ %5d Opened /dev/null file-descriptor=%d]\n", pid, dev_null);

    /* If any of the stdio file descriptors is valid and not associated
       with /dev/null, dup /dev/null to it.  */
    for (i = 0; i < 3; i++) {
        /* If it is /dev/null already, we are done. */
        if (i == dev_null)
            continue;

        TRACE("[ %5d Nullifying stdio file descriptor %d]\n", pid, i);
        /* The man page of fcntl does not say that fcntl(..,F_GETFL)
           can be interrupted but we do this just to be safe. */
        do {
          status = fcntl(i, F_GETFL);
        } while (status < 0 && errno == EINTR);

        /* If file is openned, we are good. */
        if (status >= 0)
          continue;

        /* The only error we allow is that the file descriptor does not
           exist, in which case we dup /dev/null to it. */
        if (errno != EBADF) {
            DL_ERR("nullify_stdio: unhandled error %s", strerror(errno));
            return_value = -1;
            continue;
        }

        /* Try dupping /dev/null to this stdio file descriptor and
           repeat if there is a signal.  Note that any errors in closing
           the stdio descriptor are lost.  */
        do {
            status = dup2(dev_null, i);
        } while (status < 0 && errno == EINTR);

        if (status < 0) {
            DL_ERR("nullify_stdio: dup2 error %s", strerror(errno));
            return_value = -1;
            continue;
        }
    }

    /* If /dev/null is not one of the stdio file descriptors, close it. */
    if (dev_null > 2) {
        TRACE("[ %5d Closing /dev/null file-descriptor=%d]\n", pid, dev_null);
        do {
            status = close(dev_null);
        } while (status < 0 && errno == EINTR);

        if (status < 0) {
            DL_ERR("nullify_stdio: close error %s", strerror(errno));
            return_value = -1;
        }
    }

    close(dev_null);
    return return_value;
}

static int link_image(soinfo *si, unsigned wr_offset)
{
    unsigned *d;
    Elf_Phdr *phdr = si->phdr;
    int phnum = si->phnum;

    INFO("[ %5d linking %s ]\n", pid, si->name);
    DEBUG("%5d si->base = 0x%08x si->flags = 0x%08x\n", pid,
          si->base, si->flags);

    if (si->flags & (FLAG_EXE | FLAG_LINKER)) {
        /* Locate the needed program segments (DYNAMIC/ARM_EXIDX) for
         * linkage info if this is the executable or the linker itself. 
         * If this was a dynamic lib, that would have been done at load time.
         *
         * TODO: It's unfortunate that small pieces of this are
         * repeated from the load_library routine. Refactor this just
         * slightly to reuse these bits.
         */
        si->size = 0;
        for(; phnum > 0; --phnum, ++phdr) {
#ifdef ANDROID_ARM_LINKER
            if(phdr->p_type == PT_ARM_EXIDX) {
                /* exidx entries (used for stack unwinding) are 8 bytes each.
                 */
                si->ARM_exidx = (unsigned *)phdr->p_vaddr;
                si->ARM_exidx_count = phdr->p_memsz / 8;
            }
#endif
            if (phdr->p_type == PT_LOAD) {
                /* For the executable, we use the si->size field only in
                   dl_unwind_find_exidx(), so the meaning of si->size
                   is not the size of the executable; it is the distance
                   between the load location of the executable and the last
                   address of the loadable part of the executable.
                   We use the range [si->base, si->base + si->size) to
                   determine whether a PC value falls within the executable
                   section. Of course, if a value is between si->base and
                   (si->base + phdr->p_vaddr), it's not in the executable
                   section, but a) we shouldn't be asking for such a value
                   anyway, and b) if we have to provide an EXIDX for such a
                   value, then the executable's EXIDX is probably the better
                   choice.
                */
                DEBUG_DUMP_PHDR(phdr, "PT_LOAD", pid);
                if (phdr->p_vaddr + phdr->p_memsz > si->size)
                    si->size = phdr->p_vaddr + phdr->p_memsz;
                /* try to remember what range of addresses should be write
                 * protected */
                if (!(phdr->p_flags & PF_W)) {
                    unsigned _end;

                    if (si->base + phdr->p_vaddr < si->wrprotect_start)
                        si->wrprotect_start = si->base + phdr->p_vaddr;
                    _end = (((si->base + phdr->p_vaddr + phdr->p_memsz + PAGE_SIZE - 1) &
                             (~PAGE_MASK)));
                    if (_end > si->wrprotect_end)
                        si->wrprotect_end = _end;
                    /* Make the section writable just in case we'll have to
                     * write to it during relocation (i.e. text segment).
                     * However, we will remember what range of addresses
                     * should be write protected.
                     */
                    mprotect((void *) (si->base + phdr->p_vaddr),
                             phdr->p_memsz,
                             PFLAGS_TO_PROT(phdr->p_flags) | PROT_WRITE);
                }
            } else if (phdr->p_type == PT_DYNAMIC) {
                if (si->dynamic != (unsigned *)-1) {
                    DL_ERR("%5d multiple PT_DYNAMIC segments found in '%s'. "
                          "Segment at 0x%08x, previously one found at 0x%08x",
                          pid, si->name, si->base + phdr->p_vaddr,
                          (unsigned)si->dynamic);
                    goto fail;
                }
                DEBUG_DUMP_PHDR(phdr, "PT_DYNAMIC", pid);
                si->dynamic = (unsigned *) (si->base + phdr->p_vaddr);
            } else if (phdr->p_type == PT_GNU_RELRO) {
                if ((phdr->p_vaddr >= si->size)
                        || ((phdr->p_vaddr + phdr->p_memsz) > si->size)
                        || ((si->base + phdr->p_vaddr + phdr->p_memsz) < si->base)) {
                    DL_ERR("%d invalid GNU_RELRO in '%s' "
                           "p_vaddr=0x%08x p_memsz=0x%08x", pid, si->name,
                           phdr->p_vaddr, phdr->p_memsz);
                    goto fail;
                }
                si->gnu_relro_start = (Elf_Addr) (si->base + phdr->p_vaddr);
                si->gnu_relro_len = (unsigned) phdr->p_memsz;
            }
        }
    }

    if (si->dynamic == (unsigned *)-1) {
        DL_ERR("%5d missing PT_DYNAMIC?!", pid);
        goto fail;
    }

    DEBUG("%5d dynamic = %p\n", pid, si->dynamic);

    /* extract useful information from dynamic section */
    for(d = si->dynamic; *d; d++){
        DEBUG("%5d d = %p, d[0] = 0x%08x d[1] = 0x%08x\n", pid, d, d[0], d[1]);
        switch(*d++){
        case DT_HASH:
            si->nbucket = ((unsigned *) (si->base + *d))[0];
            si->nchain = ((unsigned *) (si->base + *d))[1];
            si->bucket = (unsigned *) (si->base + *d + 8);
            si->chain = (unsigned *) (si->base + *d + 8 + si->nbucket * 4);
            break;
        case DT_STRTAB:
            si->strtab = (const char *) (si->base + *d);
            break;
        case DT_SYMTAB:
            si->symtab = (Elf_Sym *) (si->base + *d);
            break;
        case DT_PLTREL:
            if(*d != DT_REL) {
                DL_ERR("DT_RELA not supported");
                goto fail;
            }
            break;
        case DT_JMPREL:
            si->plt_rel = (Elf_Rel*) (si->base + *d);
            break;
        case DT_PLTRELSZ:
            si->plt_rel_count = *d / 8;
            break;
        case DT_REL:
            si->rel = (Elf_Rel*) (si->base + *d);
            break;
        case DT_RELSZ:
            si->rel_count = *d / 8;
            break;
        case DT_PLTGOT:
            /* Save this in case we decide to do lazy binding. We don't yet. */
            si->plt_got = (unsigned *)(si->base + *d);
            break;
        case DT_DEBUG:
            // Set the DT_DEBUG entry to the addres of _r_debug for GDB
            *d = (int) &_r_debug;
            break;
         case DT_RELA:
            DL_ERR("%5d DT_RELA not supported", pid);
            goto fail;
        case DT_INIT:
            si->init_func = (void (*)(void))(si->base + *d);
            DEBUG("%5d %s constructors (init func) found at %p\n",
                  pid, si->name, si->init_func);
            break;
        case DT_FINI:
            si->fini_func = (void (*)(void))(si->base + *d);
            DEBUG("%5d %s destructors (fini func) found at %p\n",
                  pid, si->name, si->fini_func);
            break;
        case DT_INIT_ARRAY:
            si->init_array = (unsigned *)(si->base + *d);
            DEBUG("%5d %s constructors (init_array) found at %p\n",
                  pid, si->name, si->init_array);
            break;
        case DT_INIT_ARRAYSZ:
            si->init_array_count = ((unsigned)*d) / sizeof(Elf_Addr);
            break;
        case DT_FINI_ARRAY:
            si->fini_array = (unsigned *)(si->base + *d);
            DEBUG("%5d %s destructors (fini_array) found at %p\n",
                  pid, si->name, si->fini_array);
            break;
        case DT_FINI_ARRAYSZ:
            si->fini_array_count = ((unsigned)*d) / sizeof(Elf_Addr);
            break;
        case DT_PREINIT_ARRAY:
            si->preinit_array = (unsigned *)(si->base + *d);
            DEBUG("%5d %s constructors (preinit_array) found at %p\n",
                  pid, si->name, si->preinit_array);
            break;
        case DT_PREINIT_ARRAYSZ:
            si->preinit_array_count = ((unsigned)*d) / sizeof(Elf_Addr);
            break;
        case DT_TEXTREL:
            /* TODO: make use of this. */
            /* this means that we might have to write into where the text
             * segment was loaded during relocation... Do something with
             * it.
             */
            DEBUG("%5d Text segment should be writable during relocation.\n",
                  pid);
            break;
        }
    }

    DEBUG("%5d si->base = 0x%08x, si->strtab = %p, si->symtab = %p\n",
           pid, si->base, si->strtab, si->symtab);

    if((si->strtab == 0) || (si->symtab == 0)) {
        DL_ERR("%5d missing essential tables", pid);
        goto fail;
    }

    /* if this is the main executable, then load all of the preloads now */
    if(si->flags & FLAG_EXE) {
        int i;
        memset(preloads, 0, sizeof(preloads));
        for(i = 0; ldpreload_names[i] != NULL; i++) {
            soinfo *lsi = find_library(ldpreload_names[i]);
            if(lsi == 0) {
                strlcpy(tmp_err_buf, linker_get_error(), sizeof(tmp_err_buf));
                DL_ERR("%5d could not load needed library '%s' for '%s' (%s)",
                       pid, ldpreload_names[i], si->name, tmp_err_buf);
                goto fail;
            }
            lsi->refcount++;
            preloads[i] = lsi;
        }
    }

    for(d = si->dynamic; *d; d += 2) {
        if(d[0] == DT_NEEDED){
            DEBUG("%5d %s needs %s\n", pid, si->name, si->strtab + d[1]);
            soinfo *lsi = find_library(si->strtab + d[1]);
            if(lsi == 0) {
                strlcpy(tmp_err_buf, linker_get_error(), sizeof(tmp_err_buf));
                DL_ERR("%5d could not load needed library '%s' for '%s' (%s)",
                       pid, si->strtab + d[1], si->name, tmp_err_buf);
                goto fail;
            }
            /* Save the soinfo of the loaded DT_NEEDED library in the payload
               of the DT_NEEDED entry itself, so that we can retrieve the
               soinfo directly later from the dynamic segment.  This is a hack,
               but it allows us to map from DT_NEEDED to soinfo efficiently
               later on when we resolve relocations, trying to look up a symbol
               with dlsym().
            */
            d[1] = (unsigned)lsi;
            lsi->refcount++;
        }
    }

    if(si->plt_rel) {
        DEBUG("[ %5d relocating %s plt ]\n", pid, si->name );
        if(reloc_library(si, si->plt_rel, si->plt_rel_count))
            goto fail;
    }
    if(si->rel) {
        DEBUG("[ %5d relocating %s ]\n", pid, si->name );
        if(reloc_library(si, si->rel, si->rel_count))
            goto fail;
    }

    si->flags |= FLAG_LINKED;
    DEBUG("[ %5d finished linking %s ]\n", pid, si->name);

#if 0
    /* This is the way that the old dynamic linker did protection of
     * non-writable areas. It would scan section headers and find where
     * .text ended (rather where .data/.bss began) and assume that this is
     * the upper range of the non-writable area. This is too coarse,
     * and is kept here for reference until we fully move away from single
     * segment elf objects. See the code in get_wr_offset (also #if'd 0)
     * that made this possible.
     */
    if(wr_offset < 0xffffffff){
        mprotect((void*) si->base, wr_offset, PROT_READ | PROT_EXEC);
    }
#else
    /* TODO: Verify that this does the right thing in all cases, as it
     * presently probably does not. It is possible that an ELF image will
     * come with multiple read-only segments. What we ought to do is scan
     * the program headers again and mprotect all the read-only segments.
     * To prevent re-scanning the program header, we would have to build a
     * list of loadable segments in si, and then scan that instead. */
    if (si->wrprotect_start != 0xffffffff && si->wrprotect_end != 0) {
        mprotect((void *)si->wrprotect_start,
                 si->wrprotect_end - si->wrprotect_start,
                 PROT_READ | PROT_EXEC);
    }
#endif

    if (si->gnu_relro_start != 0 && si->gnu_relro_len != 0) {
        Elf_Addr start = (si->gnu_relro_start & ~PAGE_MASK);
        unsigned len = (si->gnu_relro_start - start) + si->gnu_relro_len;
        if (mprotect((void *) start, len, PROT_READ) < 0) {
            DL_ERR("%5d GNU_RELRO mprotect of library '%s' failed: %d (%s)\n",
                   pid, si->name, errno, strerror(errno));
            goto fail;
        }
    }

    /* If this is a SET?ID program, dup /dev/null to opened stdin,
       stdout and stderr to close a security hole described in:

    ftp://ftp.freebsd.org/pub/FreeBSD/CERT/advisories/FreeBSD-SA-02:23.stdio.asc

     */
    if (program_is_setuid)
        nullify_closed_stdio ();
    notify_gdb_of_load(si);
    return 0;

fail:
    ERROR("failed to link %s\n", si->name);
    si->flags |= FLAG_ERROR;
    return -1;
}

static void parse_library_path(const char *path, char *delim)
{
    size_t len;
    char *ldpaths_bufp = ldpaths_buf;
    int i = 0;

    len = strlcpy(ldpaths_buf, path, sizeof(ldpaths_buf));

    while (i < LDPATH_MAX && (ldpaths[i] = strsep(&ldpaths_bufp, delim))) {
        if (*ldpaths[i] != '\0')
            ++i;
    }

    /* Forget the last path if we had to truncate; this occurs if the 2nd to
     * last char isn't '\0' (i.e. not originally a delim). */
    if (i > 0 && len >= sizeof(ldpaths_buf) &&
            ldpaths_buf[sizeof(ldpaths_buf) - 2] != '\0') {
        ldpaths[i - 1] = NULL;
    } else {
        ldpaths[i] = NULL;
    }
}

static void parse_preloads(const char *path, char *delim)
{
    size_t len;
    char *ldpreloads_bufp = ldpreloads_buf;
    int i = 0;

    len = strlcpy(ldpreloads_buf, path, sizeof(ldpreloads_buf));

    while (i < LDPRELOAD_MAX && (ldpreload_names[i] = strsep(&ldpreloads_bufp, delim))) {
        if (*ldpreload_names[i] != '\0') {
            ++i;
        }
    }

    /* Forget the last path if we had to truncate; this occurs if the 2nd to
     * last char isn't '\0' (i.e. not originally a delim). */
    if (i > 0 && len >= sizeof(ldpreloads_buf) &&
            ldpreloads_buf[sizeof(ldpreloads_buf) - 2] != '\0') {
        ldpreload_names[i - 1] = NULL;
    } else {
        ldpreload_names[i] = NULL;
    }
}

/*
 * This code is called after the linker has linked itself and
 * fixed it's own GOT. It is safe to make references to externs
 * and other non-local data at this point.
 */
static unsigned __linker_init_post_relocation(unsigned **elfdata)
{
    static soinfo linker_soinfo;

    int argc = (int) *elfdata;
    char **argv = (char**) (elfdata + 1);
    unsigned *vecs = (unsigned*) (argv + argc + 1);
    unsigned *v;
    soinfo *si;
    struct link_map * map;
    const char *ldpath_env = NULL;
    const char *ldpreload_env = NULL;

    /* NOTE: we store the elfdata pointer on a special location
     *       of the temporary TLS area in order to pass it to
     *       the C Library's runtime initializer.
     *
     *       The initializer must clear the slot and reset the TLS
     *       to point to a different location to ensure that no other
     *       shared library constructor can access it.
     */
    //__libc_init_tls(elfdata);

    pid = getpid();

#if TIMING
    struct timeval t0, t1;
    gettimeofday(&t0, 0);
#endif

    /* Initialize environment functions, and get to the ELF aux vectors table */
    vecs = linker_env_init(vecs);

    /* Check auxv for AT_SECURE first to see if program is setuid, setgid,
       has file caps, or caused a SELinux/AppArmor domain transition. */
    for (v = vecs; v[0]; v += 2) {
        if (v[0] == AT_SECURE) {
            /* kernel told us whether to enable secure mode */
            program_is_setuid = v[1];
            goto sanitize;
        }
    }

    /* Kernel did not provide AT_SECURE - fall back on legacy test. */
    program_is_setuid = (getuid() != geteuid()) || (getgid() != getegid());

sanitize:
    /* Sanitize environment if we're loading a setuid program */
    if (program_is_setuid)
        linker_env_secure();

    //debugger_init();

    /* Get a few environment variables */
    {
#if LINKER_DEBUG
        const char* env;
        env = linker_env_get("DEBUG"); /* XXX: TODO: Change to LD_DEBUG */
        if (env)
            debug_verbosity = atoi(env);
#endif

        /* Normally, these are cleaned by linker_env_secure, but the test
         * against program_is_setuid doesn't cost us anything */
        if (!program_is_setuid) {
            ldpath_env = linker_env_get("LD_LIBRARY_PATH");
            ldpreload_env = linker_env_get("LD_PRELOAD");
        }
    }

    INFO("[ android linker & debugger ]\n");
    DEBUG("%5d elfdata @ 0x%08x\n", pid, (unsigned)elfdata);

    si = alloc_info(argv[0]);
    if(si == 0) {
        exit(-1);
    }

        /* bootstrap the link map, the main exe always needs to be first */
    si->flags |= FLAG_EXE;
    map = &(si->linkmap);

    map->l_addr = 0;
    map->l_name = argv[0];
    map->l_prev = NULL;
    map->l_next = NULL;

    _r_debug.r_map = map;
    r_debug_tail = map;

        /* gdb expects the linker to be in the debug shared object list,
         * and we need to make sure that the reported load address is zero.
         * Without this, gdb gets the wrong idea of where rtld_db_dlactivity()
         * is.  Don't use alloc_info(), because the linker shouldn't
         * be on the soinfo list.
         */
    strlcpy((char*) linker_soinfo.name, "/system/bin/linker", sizeof linker_soinfo.name);
    linker_soinfo.flags = 0;
    linker_soinfo.base = 0;     // This is the important part; must be zero.
    insert_soinfo_into_debug_map(&linker_soinfo);

        /* extract information passed from the kernel */
    while(vecs[0] != 0){
        switch(vecs[0]){
        case AT_PHDR:
            si->phdr = (Elf_Phdr*) vecs[1];
            break;
        case AT_PHNUM:
            si->phnum = (int) vecs[1];
            break;
        case AT_ENTRY:
            si->entry = vecs[1];
            break;
        }
        vecs += 2;
    }

    /* Compute the value of si->base. We can't rely on the fact that
     * the first entry is the PHDR because this will not be true
     * for certain executables (e.g. some in the NDK unit test suite)
     */
    int nn;
    si->base = 0;
    for ( nn = 0; nn < si->phnum; nn++ ) {
        if (si->phdr[nn].p_type == PT_PHDR) {
            si->base = (Elf_Addr) si->phdr - si->phdr[nn].p_vaddr;
            break;
        }
    }
    si->dynamic = (unsigned *)-1;
    si->wrprotect_start = 0xffffffff;
    si->wrprotect_end = 0;
    si->refcount = 1;
    si->gnu_relro_start = 0;
    si->gnu_relro_len = 0;

        /* Use LD_LIBRARY_PATH if we aren't setuid/setgid */
    if (ldpath_env)
        parse_library_path(ldpath_env, ":");

    if (ldpreload_env) {
        parse_preloads(ldpreload_env, " :");
    }

    if(link_image(si, 0)) {
        char errmsg[] = "CANNOT LINK EXECUTABLE\n";
        write(2, __linker_dl_err_buf, strlen(__linker_dl_err_buf));
        write(2, errmsg, sizeof(errmsg));
        exit(-1);
    }

    call_constructors_recursive(si);

#if ALLOW_SYMBOLS_FROM_MAIN
    /* Set somain after we've loaded all the libraries in order to prevent
     * linking of symbols back to the main image, which is not set up at that
     * point yet.
     */
    somain = si;
#endif

#if TIMING
    gettimeofday(&t1,NULL);
    PRINT("LINKER TIME: %s: %d microseconds\n", argv[0], (int) (
               (((long long)t1.tv_sec * 1000000LL) + (long long)t1.tv_usec) -
               (((long long)t0.tv_sec * 1000000LL) + (long long)t0.tv_usec)
               ));
#endif
#if STATS
    PRINT("RELO STATS: %s: %d abs, %d rel, %d copy, %d symbol\n", argv[0],
           linker_stats.reloc[RELOC_ABSOLUTE],
           linker_stats.reloc[RELOC_RELATIVE],
           linker_stats.reloc[RELOC_COPY],
           linker_stats.reloc[RELOC_SYMBOL]);
#endif
#if COUNT_PAGES
    {
        unsigned n;
        unsigned i;
        unsigned count = 0;
        for(n = 0; n < 4096; n++){
            if(bitmask[n]){
                unsigned x = bitmask[n];
                for(i = 0; i < 8; i++){
                    if(x & 1) count++;
                    x >>= 1;
                }
            }
        }
        PRINT("PAGES MODIFIED: %s: %d (%dKB)\n", argv[0], count, count * 4);
    }
#endif

#if TIMING || STATS || COUNT_PAGES
    fflush(stdout);
#endif

    TRACE("[ %5d Ready to execute '%s' @ 0x%08x ]\n", pid, si->name,
          si->entry);
    return si->entry;
}

/*
 * Find the value of AT_BASE passed to us by the kernel. This is the load
 * location of the linker.
 */
static unsigned find_linker_base(unsigned **elfdata) {
    int argc = (int) *elfdata;
    char **argv = (char**) (elfdata + 1);
    unsigned *vecs = (unsigned*) (argv + argc + 1);
    while (vecs[0] != 0) {
        vecs++;
    }

    /* The end of the environment block is marked by two NULL pointers */
    vecs++;

    while(vecs[0]) {
        if (vecs[0] == AT_BASE) {
            return vecs[1];
        }
        vecs += 2;
    }

    return 0; // should never happen
}

/*
 * This is the entry point for the linker, called from begin.S. This
 * method is responsible for fixing the linker's own relocations, and
 * then calling __linker_init_post_relocation().
 *
 * Because this method is called before the linker has fixed it's own
 * relocations, any attempt to reference an extern variable, extern
 * function, or other GOT reference will generate a segfault.
 */
unsigned __linker_init(unsigned **elfdata) {
    unsigned linker_addr = find_linker_base(elfdata);
    Elf_Ehdr *elf_hdr = (Elf_Ehdr *) linker_addr;
    Elf_Phdr *phdr =
        (Elf_Phdr *)((unsigned char *) linker_addr + elf_hdr->e_phoff);

    soinfo linker_so;
    memset(&linker_so, 0, sizeof(soinfo));

    linker_so.base = linker_addr;
    linker_so.dynamic = (unsigned *) -1;
    linker_so.phdr = phdr;
    linker_so.phnum = elf_hdr->e_phnum;
    linker_so.flags |= FLAG_LINKER;
    linker_so.wrprotect_start = 0xffffffff;
    linker_so.wrprotect_end = 0;
    linker_so.gnu_relro_start = 0;
    linker_so.gnu_relro_len = 0;

    if (link_image(&linker_so, 0)) {
        // It would be nice to print an error message, but if the linker
        // can't link itself, there's no guarantee that we'll be able to
        // call write() (because it involves a GOT reference).
        //
        // This situation should never occur unless the linker itself
        // is corrupt.
        exit(-1);
    }

    // We have successfully fixed our own relocations. It's safe to run
    // the main part of the linker now.
    return __linker_init_post_relocation(elfdata);
}

#ifdef WANT_ARM_TRACING
void android_linker_init(int sdk_version, void *(get_hooked_symbol)(const char*, const char*), int enable_linker_gdb_support, void *(create_wrapper)(const char*, void*, int)) {
#else
void android_linker_init(int sdk_version, void *(get_hooked_symbol)(const char*, const char*), int enable_linker_gdb_support) {
#endif
   (void) sdk_version;
   _get_hooked_symbol = get_hooked_symbol;
  _linker_enable_gdb_support = enable_linker_gdb_support;
}
