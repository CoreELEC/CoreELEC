/*
 * Copyright 2017 The Android Open Source Project
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

#ifndef ANDROID_GRALLOCUSAGE_GRALLOC_USAGE_CONVERSION_H
#define ANDROID_GRALLOCUSAGE_GRALLOC_USAGE_CONVERSION_H 1

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// Conversion functions are out-of-line so that users don't have to be exposed to
// android/hardware/graphics/allocator/2.0/types.h and link against
// android.hardware.graphics.allocator@2.0 to get that in their search path.

// Convert a 32-bit gralloc0 usage mask to a producer/consumer pair of 64-bit usage masks as used
// by android.hardware.graphics.allocator@2.0 (and gralloc1). This conversion properly handles the
// mismatch between a.h.g.allocator@2.0's CPU_{READ,WRITE}_OFTEN and gralloc0's
// SW_{READ,WRITE}_OFTEN.
void android_convertGralloc0To1Usage(int32_t usage, uint64_t* producerUsage,
                                     uint64_t* consumerUsage);

// Convert a producer/consumer pair of 64-bit usage masks as used by
// android.hardware.graphics.allocator@2.0 (and gralloc1) to a 32-bit gralloc0 usage mask. This
// conversion properly handles the mismatch between a.h.g.allocator@2.0's CPU_{READ,WRITE}_OFTEN
// and gralloc0's SW_{READ,WRITE}_OFTEN.
int32_t android_convertGralloc1To0Usage(uint64_t producerUsage, uint64_t consumerUsage);

#ifdef __cplusplus
}
#endif

#endif  // ANDROID_GRALLOCUSAGE_GRALLOC_USAGE_CONVERSION_H
