/*
 * Copyright (C) 2015 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "linker_dlwarning.h"

#include <strings.h>

#include <string>

#include "hybris_compat.h"

static std::string current_msg;

void add_dlwarning(const char* sopath, const char* message, const char* value) {
  if (!current_msg.empty()) {
    current_msg += '\n';
  }

  current_msg = current_msg + basename(sopath) + ": " + message;

  if (value != nullptr) {
    current_msg = current_msg + " \"" + value + "\"";
  }
}

// Resets the current one (like dlerror but instead of
// being thread-local it is process-local).
void get_dlwarning(void* obj, void (*f)(void*, const char*)) {
  if (current_msg.empty()) {
    f(obj, nullptr);
  } else {
    std::string msg = current_msg;
    current_msg.clear();
    f(obj, msg.c_str());
  }
}
