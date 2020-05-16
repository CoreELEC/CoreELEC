/*
 * Copyright (C) 2013 Simon Busch <morphis@gravedo.de>
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

#ifndef UI_COMPATIBILITY_LAYER_H_
#define UI_COMPATIBILITY_LAYER_H_

#include <stdbool.h>
#include <stdint.h>
#include <unistd.h>

#ifdef __cplusplus
extern "C" {
#endif

    struct graphic_buffer;

    struct graphic_buffer* graphic_buffer_new(void);
    struct graphic_buffer* graphic_buffer_new_sized(uint32_t w, uint32_t h,
                                              int32_t format, uint32_t usage);
    struct graphic_buffer* graphic_buffer_new_existing(uint32_t w, uint32_t h,
                                              int32_t format, uint32_t usage,
                                              uint32_t stride, void *handle,
                                              bool keepOwnership);

    void graphic_buffer_free(struct graphic_buffer *buffer);

    uint32_t graphic_buffer_get_width(struct graphic_buffer *buffer);
    uint32_t graphic_buffer_get_height(struct graphic_buffer *buffer);
    uint32_t graphic_buffer_get_stride(struct graphic_buffer *buffer);
    uint32_t graphic_buffer_get_usage(struct graphic_buffer *buffer);
    int32_t graphic_buffer_get_pixel_format(struct graphic_buffer *buffer);

    uint32_t graphic_buffer_reallocate(struct graphic_buffer *buffer, uint32_t w,
                                       uint32_t h, int32_t f, uint32_t usage);
    uint32_t graphic_buffer_lock(struct graphic_buffer *buffer, uint32_t usage, void **vaddr);
    uint32_t graphic_buffer_unlock(struct graphic_buffer *buffer);

    void* graphic_buffer_get_native_buffer(struct graphic_buffer *buffer);

#if ANDROID_VERSION_MAJOR==4 && ANDROID_VERSION_MINOR<=3
    void graphic_buffer_set_index(struct graphic_buffer *buffer, int index);
    int graphic_buffer_get_index(struct graphic_buffer *buffer);
#endif

    int graphic_buffer_init_check(struct graphic_buffer *buffer);

#ifdef __cplusplus
}
#endif

#endif // UI_COMPATIBILITY_LAYER_H_
