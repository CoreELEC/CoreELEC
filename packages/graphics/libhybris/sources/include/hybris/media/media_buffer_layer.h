/*
 * Copyright (C) 2016 Canonical Ltd
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
 *
 * Authored by: Simon Fels <simon.fels@canonical.com>
 */

#ifndef MEDIA_BUFFER_LAYER_H_
#define MEDIA_BUFFER_LAYER_H_

#include <stdint.h>
#include <memory.h>

#include <hybris/media/media_message_layer.h>
#include <hybris/media/media_meta_data_layer.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef void MediaBufferWrapper;
typedef void (*MediaBufferReturnCallback)(MediaBufferWrapper *buffer, void *user_data);

MediaBufferWrapper* media_buffer_create(size_t size);
void media_buffer_destroy(MediaBufferWrapper *buffer);
void media_buffer_release(MediaBufferWrapper *buffer);
void media_buffer_ref(MediaBufferWrapper *buffer);

int media_buffer_get_refcount(MediaBufferWrapper *buffer);

void* media_buffer_get_data(MediaBufferWrapper *buffer);
size_t media_buffer_get_size(MediaBufferWrapper *buffer);
size_t media_buffer_get_range_offset(MediaBufferWrapper *buffer);
size_t media_buffer_get_range_length(MediaBufferWrapper *buffer);
MediaMetaDataWrapper* media_buffer_get_meta_data(MediaBufferWrapper *buffer);

void media_buffer_set_return_callback(MediaBufferWrapper *buffer,
    MediaBufferReturnCallback callback, void *user_data);

typedef void MediaABufferWrapper;

MediaABufferWrapper* media_abuffer_create(size_t capacity);
MediaABufferWrapper* media_abuffer_create_with_data(uint8_t *data, size_t size);

void media_abuffer_set_range(MediaABufferWrapper *buffer, size_t offset, size_t size);
void media_abuffer_set_media_buffer_base(MediaABufferWrapper *buffer, MediaBufferWrapper *mbuf);
MediaBufferWrapper* media_abuffer_get_media_buffer_base(MediaABufferWrapper *buffer);

void* media_abuffer_get_data(MediaABufferWrapper *buffer);
size_t media_abuffer_get_size(MediaABufferWrapper *buffer);
size_t media_abuffer_get_range_offset(MediaABufferWrapper *buffer);
size_t media_abuffer_get_capacity(MediaABufferWrapper *buffer);
MediaMessageWrapper* media_abuffer_get_meta(MediaABufferWrapper *buffer);

#ifdef __cplusplus
}
#endif

#endif
