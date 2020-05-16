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

#include "linker_logger.h"

#include <string.h>
#include <sys/prctl.h>
#include <unistd.h>

#include <string>
#include <vector>

#include <async_safe/log.h>

#include "linker_utils.h"
#ifdef DISABLED_FOR_HYBRIS_SUPPORT
#include "private/CachedProperty.h"
#endif

#include "hybris_compat.h"

LinkerLogger g_linker_logger;
bool g_greylist_disabled = false;

#ifdef DISABLED_FOR_HYBRIS_SUPPORT
static uint32_t ParseProperty(const std::string& value) {
  if (value.empty()) {
    return 0;
  }

  std::vector<std::string> options = split(value, ",");

  uint32_t flags = 0;

  for (const auto& o : options) {
    if (o == "dlerror") {
      flags |= kLogErrors;
    } else if (o == "dlopen") {
      flags |= kLogDlopen;
    } else if (o == "dlsym") {
      flags |= kLogDlsym;
    } else {
      async_safe_format_log(ANDROID_LOG_WARN, "linker", "Ignoring unknown debug.ld option \"%s\"",
                            o.c_str());
    }
  }

  return flags;
}
#endif

#ifdef DISABLED_FOR_HYBRIS_SUPPORT
static void GetAppSpecificProperty(char* buffer) {
  // Get process basename.
  const char* process_name_start = basename(g_argv[0]);

  // Remove ':' and everything after it. This is the naming convention for
  // services: https://developer.android.com/guide/components/services.html
  const char* process_name_end = strchr(process_name_start, ':');

  std::string process_name = (process_name_end != nullptr) ?
                             std::string(process_name_start, (process_name_end - process_name_start)) :
                             std::string(process_name_start);

  std::string property_name = std::string("debug.ld.app.") + process_name;
  __system_property_get(property_name.c_str(), buffer);
}
#endif

void LinkerLogger::ResetState() {
  // The most likely scenario app is not debuggable and
  // is running on a user build, in which case logging is disabled.
  if (prctl(PR_GET_DUMPABLE, 0, 0, 0, 0) == 0) {
    return;
  }

  // This is a convenient place to check whether the greylist should be disabled for testing.
#ifdef DISABLED_FOR_HYBRIS_SUPPORT
  static CachedProperty greylist_disabled("debug.ld.greylist_disabled");
  bool old_value = g_greylist_disabled;
  g_greylist_disabled = (strcmp(greylist_disabled.Get(), "true") == 0);
  if (g_greylist_disabled != old_value) {
    async_safe_format_log(ANDROID_LOG_INFO, "linker", "%s greylist",
                          g_greylist_disabled ? "Disabling" : "Enabling");
  }
  flags_ = 0;

  // For logging, check the flag applied to all processes first.
  static CachedProperty debug_ld_all("debug.ld.all");
  flags_ |= ParseProperty(debug_ld_all.Get());
#endif
  // Ignore processes started without argv (http://b/33276926).
  if (g_argv[0] == nullptr) {
    return;
  }
#ifdef DISABLED_FOR_HYBRIS_SUPPORT
  // Otherwise check the app-specific property too.
  // We can't easily cache the property here because argv[0] changes.
  char debug_ld_app[PROP_VALUE_MAX] = {};
  GetAppSpecificProperty(debug_ld_app);
  flags_ |= ParseProperty(debug_ld_app);
#endif
}

void LinkerLogger::Log(uint32_t type, const char* format, ...) {
  if ((flags_ & type) == 0) {
    return;
  }

  va_list ap;
  va_start(ap, format);
  async_safe_format_log_va_list(ANDROID_LOG_DEBUG, "linker", format, ap);
  va_end(ap);
}
