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

#ifndef _LINKER_LOGGER_H_
#define _LINKER_LOGGER_H_

#include <stdlib.h>
#include <limits.h>
#include "private/bionic_macros.h"
#include "private/bionic_systrace.h"

#define LD_LOG(type, x...) \
  { \
    g_linker_logger.Log(type, x); \
  }

constexpr const uint32_t kLogErrors = 1 << 0;
constexpr const uint32_t kLogDlopen = 1 << 1;
constexpr const uint32_t kLogDlsym  = 1 << 2;

class LinkerLogger {
 public:
  LinkerLogger() : flags_(0) { }

  void ResetState();
  void Log(uint32_t type, const char* format, ...);
 private:
  uint32_t flags_;

  DISALLOW_COPY_AND_ASSIGN(LinkerLogger);
};

extern LinkerLogger g_linker_logger;
extern char** g_argv;

// If the system property debug.ld.greylist_disabled is true, we'll not use the greylist
// regardless of API level.
extern bool g_greylist_disabled;

#endif /* _LINKER_LOGGER_H_ */
