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

#include <hybris/grallocusage/GrallocUsageConversion.h>

#include <hardware/gralloc.h>
#include <hardware/gralloc1.h>

#ifndef ANDROID_BUILD
#include <android-config.h>
#endif

void android_convertGralloc0To1Usage(int32_t usage, uint64_t* producerUsage,
                                     uint64_t* consumerUsage) {
    constexpr uint64_t PRODUCER_MASK =
        GRALLOC1_PRODUCER_USAGE_CPU_READ |
        /* GRALLOC1_PRODUCER_USAGE_CPU_READ_OFTEN | */
        GRALLOC1_PRODUCER_USAGE_CPU_WRITE |
        /* GRALLOC1_PRODUCER_USAGE_CPU_WRITE_OFTEN | */
        GRALLOC1_PRODUCER_USAGE_GPU_RENDER_TARGET | GRALLOC1_PRODUCER_USAGE_PROTECTED |
        GRALLOC1_PRODUCER_USAGE_CAMERA | GRALLOC1_PRODUCER_USAGE_VIDEO_DECODER
#if ANDROID_VERSION_MAJOR >= 8
        | GRALLOC1_PRODUCER_USAGE_SENSOR_DIRECT_DATA
#endif
        ;
    constexpr uint64_t CONSUMER_MASK =
        GRALLOC1_CONSUMER_USAGE_CPU_READ |
        /* GRALLOC1_CONSUMER_USAGE_CPU_READ_OFTEN | */
        GRALLOC1_CONSUMER_USAGE_GPU_TEXTURE | GRALLOC1_CONSUMER_USAGE_HWCOMPOSER |
        GRALLOC1_CONSUMER_USAGE_CLIENT_TARGET | GRALLOC1_CONSUMER_USAGE_CURSOR |
        GRALLOC1_CONSUMER_USAGE_VIDEO_ENCODER | GRALLOC1_CONSUMER_USAGE_CAMERA |
        GRALLOC1_CONSUMER_USAGE_RENDERSCRIPT
#if ANDROID_VERSION_MAJOR >= 8
	| GRALLOC1_CONSUMER_USAGE_GPU_DATA_BUFFER
#endif
        ;
    *producerUsage = static_cast<uint64_t>(usage) & PRODUCER_MASK;
    *consumerUsage = static_cast<uint64_t>(usage) & CONSUMER_MASK;
    if ((static_cast<uint32_t>(usage) & GRALLOC_USAGE_SW_READ_OFTEN) == GRALLOC_USAGE_SW_READ_OFTEN) {
        *producerUsage |= GRALLOC1_PRODUCER_USAGE_CPU_READ_OFTEN;
        *consumerUsage |= GRALLOC1_CONSUMER_USAGE_CPU_READ_OFTEN;
    }
    if ((static_cast<uint32_t>(usage) & GRALLOC_USAGE_SW_WRITE_OFTEN) ==
        GRALLOC_USAGE_SW_WRITE_OFTEN) {
        *producerUsage |= GRALLOC1_PRODUCER_USAGE_CPU_WRITE_OFTEN;
    }
}

int32_t android_convertGralloc1To0Usage(uint64_t producerUsage, uint64_t consumerUsage) {
    static_assert(uint64_t(GRALLOC1_CONSUMER_USAGE_CPU_READ_OFTEN) ==
                      uint64_t(GRALLOC1_PRODUCER_USAGE_CPU_READ_OFTEN),
                  "expected ConsumerUsage and ProducerUsage CPU_READ_OFTEN bits to match");
    uint64_t merged = producerUsage | consumerUsage;
    if ((merged & (GRALLOC1_CONSUMER_USAGE_CPU_READ_OFTEN)) ==
        GRALLOC1_CONSUMER_USAGE_CPU_READ_OFTEN) {
        merged &= ~uint64_t(GRALLOC1_CONSUMER_USAGE_CPU_READ_OFTEN);
        merged |= GRALLOC_USAGE_SW_READ_OFTEN;
    }
    if ((merged & (GRALLOC1_PRODUCER_USAGE_CPU_WRITE_OFTEN)) ==
        GRALLOC1_PRODUCER_USAGE_CPU_WRITE_OFTEN) {
        merged &= ~uint64_t(GRALLOC1_PRODUCER_USAGE_CPU_WRITE_OFTEN);
        merged |= GRALLOC_USAGE_SW_WRITE_OFTEN;
    }
    return static_cast<int32_t>(merged);
}
