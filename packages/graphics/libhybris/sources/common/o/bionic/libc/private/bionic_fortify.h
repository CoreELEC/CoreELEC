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

#include <poll.h> // For struct pollfd.
#include <stdarg.h>
#include <stdlib.h>
#include <sys/select.h> // For struct fd_set.

#include <async_safe/log.h>

static inline __noreturn void __fortify_fatal(const char* fmt, ...) {
  va_list args;
  va_start(args, fmt);
  async_safe_fatal_va_list("FORTIFY", fmt, args);
  va_end(args);
  abort();
}

//
// Common helpers.
//

static inline void __check_fd_set(const char* fn, int fd, size_t set_size) {
  if (__predict_false(fd < 0)) {
    __fortify_fatal("%s: file descriptor %d < 0", fn, fd);
  }
  if (__predict_false(fd >= FD_SETSIZE)) {
    __fortify_fatal("%s: file descriptor %d >= FD_SETSIZE %zu", fn, fd, set_size);
  }
  if (__predict_false(set_size < sizeof(fd_set))) {
    __fortify_fatal("%s: set size %zu is too small to be an fd_set", fn, set_size);
  }
}

static inline void __check_pollfd_array(const char* fn, size_t fds_size, nfds_t fd_count) {
  size_t pollfd_array_length = fds_size / sizeof(pollfd);
  if (__predict_false(pollfd_array_length < fd_count)) {
    __fortify_fatal("%s: %zu-element pollfd array too small for %u fds",
                    fn, pollfd_array_length, fd_count);
  }
}

static inline void __check_count(const char* fn, const char* identifier, size_t value) {
  if (__predict_false(value > SSIZE_MAX)) {
    __fortify_fatal("%s: %s %zu > SSIZE_MAX", fn, identifier, value);
  }
}

static inline void __check_buffer_access(const char* fn, const char* action,
                                         size_t claim, size_t actual) {
  if (__predict_false(claim > actual)) {
    __fortify_fatal("%s: prevented %zu-byte %s %zu-byte buffer", fn, claim, action, actual);
  }
}
