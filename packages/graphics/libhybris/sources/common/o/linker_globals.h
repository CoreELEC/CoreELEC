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

#ifndef __LINKER_GLOBALS_H
#define __LINKER_GLOBALS_H

#include <link.h>
#include <stddef.h>

#include <unordered_map>

#include <async_safe/log.h>

#define DL_ERR(fmt, x...) \
    do { \
      fprintf(stderr, fmt, ##x); \
      fprintf(stderr, "\n"); \
      /* If LD_DEBUG is set high enough, log every dlerror(3) message. */ \
    } while (false)

#define DL_ERR_NO_PRINT(fmt, x...) \
    do { \
      /* If LD_DEBUG is set high enough, log every dlerror(3) message. */ \
      DEBUG("%s\n", linker_get_error_buffer()); \
    } while (false)

#define DL_WARN(fmt, x...) \
    do { \
      fprintf(stderr, "WARNING: linker " fmt, ##x); \
      fprintf(stderr, "\n"); \
    } while (false)

#define DL_ERR_AND_LOG(fmt, x...) \
  do { \
    DL_ERR(fmt, x); \
    PRINT(fmt, x); \
  } while (false)

constexpr ElfW(Versym) kVersymNotNeeded = 0;
constexpr ElfW(Versym) kVersymGlobal = 1;

// These values are used to call constructors for .init_array && .preinit_array
extern int g_argc;
extern char** g_argv;
extern char** g_envp;

struct soinfo;
struct android_namespace_t;

extern android_namespace_t *g_default_namespace;

extern std::unordered_map<uintptr_t, soinfo*> g_soinfo_handles_map;

// Error buffer "variable"
char* linker_get_error_buffer();
size_t linker_get_error_buffer_size();

extern void* (*_get_hooked_symbol)(const char *sym, const char *requester);

#ifdef WANT_ARM_TRACING
extern void *(*_create_wrapper)(const char *symbol, void *function, int wrapper_type);
#endif

#endif  /* __LINKER_GLOBALS_H */
