/*
 * Copyright (C) 2015 The Android Open Source Project
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

#include "linker_utils.h"

#include "linker_debug.h"
#include "linker_globals.h"

#include <android-base/file.h>
#include <android-base/unique_fd.h>

#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <linux/limits.h>

#include <algorithm>
#include <sstream>

#include "hybris_compat.h"

std::string trim(const std::string& s) {
  std::string result;

  if (s.size() == 0) {
    return result;
  }

  size_t start_index = 0;
  size_t end_index = s.size() - 1;

  // Skip initial whitespace.
  while (start_index < s.size()) {
    if (!isspace(s[start_index])) {
      break;
    }
    start_index++;
  }

  // Skip terminating whitespace.
  while (end_index >= start_index) {
    if (!isspace(s[end_index])) {
      break;
    }
    end_index--;
  }

  // All spaces, no beef.
  if (end_index < start_index) {
    return "";
  }
  // Start_index is the first non-space, end_index is the last one.
  return s.substr(start_index, end_index - start_index + 1);
}

std::vector<std::string> split(const std::string &text, const std::string &sep) {
  std::vector<std::string> tokens;
  std::size_t start = 0, end = 0;
  while ((end = text.find(sep, start)) != std::string::npos) {
    tokens.push_back(text.substr(start, end - start));
    start = end + 1;
  }
  tokens.push_back(text.substr(start));
  return tokens;
}

std::string join(const std::vector<std::string>& things, char separator) {
  if (things.empty()) {
    return "";
  }
  std::ostringstream result;
  result << *things.begin();
  for (auto it = std::next(things.begin()); it != things.end(); ++it) {
    result << separator << *it;
  }
  return result.str();
}

void format_string(std::string* str, const std::vector<std::pair<std::string, std::string>>& params) {
  size_t pos = 0;
  while (pos < str->size()) {
    pos = str->find("$", pos);
    if (pos == std::string::npos) break;
    for (const auto& param : params) {
      const std::string& token = param.first;
      const std::string& replacement = param.second;
      if (str->substr(pos + 1, token.size()) == token) {
        str->replace(pos, token.size() + 1, replacement);
        // -1 to compensate for the ++pos below.
        pos += replacement.size() - 1;
        break;
      } else if (str->substr(pos + 1, token.size() + 2) == "{" + token + "}") {
        str->replace(pos, token.size() + 3, replacement);
        pos += replacement.size() - 1;
        break;
      }
    }
    // Skip $ in case it did not match any of the known substitutions.
    ++pos;
  }
}

std::string dirname(const char* path) {
  const char* last_slash = strrchr(path, '/');

  if (last_slash == path) {
    return "/";
  } else if (last_slash == nullptr) {
    return ".";
  } else {
    return std::string(path, last_slash - path);
  }
}

bool normalize_path(const char* path, std::string* normalized_path) {
  // Input should be an absolute path
  if (path[0] != '/') {
    PRINT("normalize_path - invalid input: \"%s\", the input path should be absolute", path);
    return false;
  }

  const size_t len = strlen(path) + 1;
  char buf[len];

  const char* in_ptr = path;
  char* out_ptr = buf;

  while (*in_ptr != 0) {
    if (*in_ptr == '/') {
      char c1 = in_ptr[1];
      if (c1 == '.') {
        char c2 = in_ptr[2];
        if (c2 == '/') {
          in_ptr += 2;
          continue;
        } else if (c2 == '.' && (in_ptr[3] == '/' || in_ptr[3] == 0)) {
          in_ptr += 3;
          while (out_ptr > buf && *--out_ptr != '/') {
          }
          if (in_ptr[0] == 0) {
            // retain '/'
            out_ptr++;
          }
          continue;
        }
      } else if (c1 == '/') {
        ++in_ptr;
        continue;
      }
    }
    *out_ptr++ = *in_ptr++;
  }

  *out_ptr = 0;
  *normalized_path = buf;
  return true;
}

bool file_is_in_dir(const std::string& file, const std::string& dir) {
  const char* needle = dir.c_str();
  const char* haystack = file.c_str();
  size_t needle_len = strlen(needle);

  return strncmp(haystack, needle, needle_len) == 0 &&
         haystack[needle_len] == '/' &&
         strchr(haystack + needle_len + 1, '/') == nullptr;
}

bool file_is_under_dir(const std::string& file, const std::string& dir) {
  const char* needle = dir.c_str();
  const char* haystack = file.c_str();
  size_t needle_len = strlen(needle);

  return strncmp(haystack, needle, needle_len) == 0 &&
         haystack[needle_len] == '/';
}

const char* const kZipFileSeparator = "!/";

bool parse_zip_path(const char* input_path, std::string* zip_path, std::string* entry_path) {
  std::string normalized_path;
  if (!normalize_path(input_path, &normalized_path)) {
    return false;
  }

  const char* const path = normalized_path.c_str();
  TRACE("Trying zip file open from path \"%s\" -> normalized \"%s\"", input_path, path);

  // Treat an '!/' separator inside a path as the separator between the name
  // of the zip file on disk and the subdirectory to search within it.
  // For example, if path is "foo.zip!/bar/bas/x.so", then we search for
  // "bar/bas/x.so" within "foo.zip".
  const char* const separator = strstr(path, kZipFileSeparator);
  if (separator == nullptr) {
    return false;
  }

  char buf[512];
  if (strlcpy(buf, path, sizeof(buf)) >= sizeof(buf)) {
    PRINT("Warning: ignoring very long library path: %s", path);
    return false;
  }

  buf[separator - path] = '\0';

  *zip_path = buf;
  *entry_path = &buf[separator - path + 2];

  return true;
}

constexpr off64_t kPageMask = ~static_cast<off64_t>(PAGE_SIZE-1);

off64_t page_start(off64_t offset) {
  return offset & kPageMask;
}

bool safe_add(off64_t* out, off64_t a, size_t b) {
  CHECK(a >= 0);
  if (static_cast<uint64_t>(INT64_MAX - a) < b) {
    return false;
  }

  *out = a + b;
  return true;
}

size_t page_offset(off64_t offset) {
  return static_cast<size_t>(offset & (PAGE_SIZE-1));
}

void split_path(const char* path, const char* delimiters,
                std::vector<std::string>* paths) {
  if (path != nullptr && path[0] != 0) {
    *paths = split(path, delimiters);
  }
}

void resolve_paths(std::vector<std::string>& paths,
                   std::vector<std::string>* resolved_paths) {
  resolved_paths->clear();
  for (const auto& path : paths) {
    // skip empty paths
    if (path.empty()) {
      continue;
    }

    char resolved_path[PATH_MAX];
    const char* original_path = path.c_str();
    if (realpath(original_path, resolved_path) != nullptr) {
      struct stat s;
      if (stat(resolved_path, &s) == 0) {
        if (S_ISDIR(s.st_mode)) {
          resolved_paths->push_back(resolved_path);
        } else {
          DL_WARN("Warning: \"%s\" is not a directory (excluding from path)", resolved_path);
          continue;
        }
      } else {
        DL_WARN("Warning: cannot stat file \"%s\": %s", resolved_path, strerror(errno));
        continue;
      }
    } else {
      std::string zip_path;
      std::string entry_path;

      std::string normalized_path;

      if (!normalize_path(original_path, &normalized_path)) {
        DL_WARN("Warning: unable to normalize \"%s\"", original_path);
        continue;
      }

      if (parse_zip_path(normalized_path.c_str(), &zip_path, &entry_path)) {
        if (realpath(zip_path.c_str(), resolved_path) == nullptr) {
          DL_WARN("Warning: unable to resolve \"%s\": %s", zip_path.c_str(), strerror(errno));
          continue;
        }

        resolved_paths->push_back(std::string(resolved_path) + kZipFileSeparator + entry_path);
      }
    }
  }
}

void stringAppendV(std::string* dst, const char* format, va_list ap) {
  // First try with a small fixed size buffer
  char space[1024];

  // It's possible for methods that use a va_list to invalidate
  // the data in it upon use.  The fix is to make a copy
  // of the structure before using it and use that copy instead.
  va_list backup_ap;
  va_copy(backup_ap, ap);
  int result = vsnprintf(space, sizeof(space), format, backup_ap);
  va_end(backup_ap);

  if (result < static_cast<int>(sizeof(space))) {
    if (result >= 0) {
      // Normal case -- everything fit.
      dst->append(space, result);
      return;
    }

    if (result < 0) {
      // Just an error.
      return;
    }
  }

  // Increase the buffer size to the size requested by vsnprintf,
  // plus one for the closing \0.
  int length = result + 1;
  char* buf = new char[length];

  // Restore the va_list before we use it again
  va_copy(backup_ap, ap);
  result = vsnprintf(buf, length, format, backup_ap);
  va_end(backup_ap);

  if (result >= 0 && result < length) {
    // It fit
    dst->append(buf, result);
  }
  delete[] buf;
}

std::string stringPrintf(const char* fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  std::string result;
  stringAppendV(&result, fmt, ap);
  va_end(ap);
  return result;
}

void stringAppendF(std::string* dst, const char* format, ...) {
  va_list ap;
  va_start(ap, format);
  stringAppendV(dst, format, ap);
  va_end(ap);
}

bool readFdToString(int fd, std::string* content) {
  content->clear();

  // Although original we had small files in mind, this code gets used for
  // very large files too, where the std::string growth heuristics might not
  // be suitable. https://code.google.com/p/android/issues/detail?id=258500.
  struct stat sb;
  if (fstat(fd, &sb) != -1 && sb.st_size > 0) {
    content->reserve(sb.st_size);
  }

  char buf[BUFSIZ];
  ssize_t n;
  while ((n = TEMP_FAILURE_RETRY(read(fd, &buf[0], sizeof(buf)))) > 0) {
    content->append(buf, n);
  }
  return (n == 0) ? true : false;
}

bool readFileToString(const std::string& path, std::string* content, bool follow_symlinks) {
  content->clear();

  int flags = O_RDONLY | O_CLOEXEC | O_BINARY | (follow_symlinks ? 0 : O_NOFOLLOW);
  android::base::unique_fd fd(TEMP_FAILURE_RETRY(open(path.c_str(), flags)));
  if (fd == -1) {
    return false;
  }
  return readFdToString(fd, content);
}

bool startsWith(const std::string& s, const char* prefix) {
  return strncmp(s.c_str(), prefix, strlen(prefix)) == 0;
}

