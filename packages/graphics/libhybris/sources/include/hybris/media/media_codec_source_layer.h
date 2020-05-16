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

#ifndef MEDIA_CODEC_SOURCE_LAYER_H_
#define MEDIA_CODEC_SOURCE_LAYER_H_

#include <stdint.h>

#include <hybris/media/media_format_layer.h>
#include <hybris/media/media_message_layer.h>
#include <hybris/media/media_buffer_layer.h>
#include <hybris/media/media_meta_data_layer.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef void MediaSourceWrapper;

typedef int (*MediaSourceStartCallback)(MediaMetaDataWrapper *meta, void *user_data);
typedef int (*MediaSourceStopCallback)(void *user_data);
typedef int (*MediaSourceReadCallback)(MediaBufferWrapper **buffer, void *user_data);
typedef int (*MediaSourcePauseCallback)(void *user_data);

MediaSourceWrapper* media_source_create(void);
void media_source_release(MediaSourceWrapper *source);
void media_source_set_format(MediaSourceWrapper *source, MediaMetaDataWrapper *meta);
void media_source_set_start_callback(MediaSourceWrapper *source, MediaSourceStartCallback callback, void *user_data);
void media_source_set_stop_callback(MediaSourceWrapper *source, MediaSourceStopCallback callback, void *user_data);
void media_source_set_read_callback(MediaSourceWrapper *source, MediaSourceReadCallback callback, void *user_data);
void media_source_set_pause_callback(MediaSourceWrapper *source, MediaSourcePauseCallback callback, void *user_data);

enum MediaCodecSourceFlags
{
    MEDIA_CODEC_SOURCE_FLAG_USE_SURFACE_INPUT = 1,
    MEDIA_CODEC_SOURCE_FLAG_USE_METADATA_INPUT = 2,
};

typedef void MediaCodecSourceWrapper;
typedef void MediaNativeWindowHandle;

MediaCodecSourceWrapper* media_codec_source_create(MediaMessageWrapper *format, MediaSourceWrapper *source, int flags);
void media_codec_source_release(MediaCodecSourceWrapper *source);

MediaNativeWindowHandle* media_codec_source_get_native_window_handle(MediaCodecSourceWrapper *source);

// Returned instance is owned by the caller and must be freed
MediaMetaDataWrapper* media_codec_source_get_format(MediaCodecSourceWrapper *source);

bool media_codec_source_start(MediaCodecSourceWrapper *source);
bool media_codec_source_stop(MediaCodecSourceWrapper *source);
bool media_codec_source_pause(MediaCodecSourceWrapper *source);

bool media_codec_source_read(MediaCodecSourceWrapper *source, MediaBufferWrapper **buffer);

bool media_codec_source_request_idr_frame(MediaBufferWrapper *source);

#ifdef __cplusplus
}
#endif

#endif
