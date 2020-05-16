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

#ifndef _LINKER_SLEB128_H
#define _LINKER_SLEB128_H

#include <stdint.h>

// Helper classes for decoding LEB128, used in packed relocation data.
// http://en.wikipedia.org/wiki/LEB128

class sleb128_decoder {
 public:
  sleb128_decoder(const uint8_t* buffer, size_t count)
      : current_(buffer), end_(buffer+count) { }

  size_t pop_front() {
    size_t value = 0;
    static const size_t size = CHAR_BIT * sizeof(value);

    size_t shift = 0;
    uint8_t byte;

    do {
      if (current_ >= end_) {
        __libc_fatal("sleb128_decoder ran out of bounds");
      }
      byte = *current_++;
      value |= (static_cast<size_t>(byte & 127) << shift);
      shift += 7;
    } while (byte & 128);

    if (shift < size && (byte & 64)) {
      value |= -(static_cast<size_t>(1) << shift);
    }

    return value;
  }

 private:
  const uint8_t* current_;
  const uint8_t* const end_;
};

#endif // __LINKER_SLEB128_H
