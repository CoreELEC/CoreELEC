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

#ifndef _LINKER_DEBUG_H_
#define _LINKER_DEBUG_H_

// You can increase the verbosity of debug traces by defining the LD_DEBUG
// environment variable to a numeric value from 0 to 2 (corresponding to
// INFO, TRACE, and DEBUG calls in the source). This will only
// affect new processes being launched.

// By default, traces are sent to logcat, with the "linker" tag. You can
// change this to go to stdout instead by setting the definition of
// LINKER_DEBUG_TO_LOG to 0.
#define LINKER_DEBUG_TO_LOG  0

#define TRACE_DEBUG          1
#define DO_TRACE_LOOKUP      1
#define DO_TRACE_RELO        1
#define DO_TRACE_IFUNC       1
#define TIMING               0
#define STATS                0
#define COUNT_PAGES          0

/*********************************************************************
 * You shouldn't need to modify anything below unless you are adding
 * more debugging information.
 *
 * To enable/disable specific debug options, change the defines above
 *********************************************************************/

#include "private/libc_logging.h"

extern int g_ld_debug_verbosity;

#define _PRINTVF(v, x...) \
    do { \
      if (g_ld_debug_verbosity > (v)) { fprintf(stderr, x); fprintf(stderr, "\n"); } \
    } while (0)

#define PRINT(x...)          _PRINTVF(-1, x)
#define INFO(x...)           _PRINTVF(0, x)
#define TRACE(x...)          _PRINTVF(1, x)

#if TRACE_DEBUG
#define DEBUG(x...)          _PRINTVF(2, "DEBUG: " x)
#else /* !TRACE_DEBUG */
#define DEBUG(x...)          do {} while (0)
#endif /* TRACE_DEBUG */

#define TRACE_TYPE(t, x...)   do { if (DO_TRACE_##t) { TRACE(x); } } while (0)

#if COUNT_PAGES
extern uint32_t bitmask[];
#if defined(__LP64__)
#define MARK(offset) \
    do { \
      if ((((offset) >> 12) >> 5) < 4096) \
          bitmask[((offset) >> 12) >> 5] |= (1 << (((offset) >> 12) & 31)); \
    } while (0)
#else
#define MARK(offset) \
    do { \
      bitmask[((offset) >> 12) >> 3] |= (1 << (((offset) >> 12) & 7)); \
    } while (0)
#endif
#else
#define MARK(x) do {} while (0)

#endif

#endif /* _LINKER_DEBUG_H_ */
