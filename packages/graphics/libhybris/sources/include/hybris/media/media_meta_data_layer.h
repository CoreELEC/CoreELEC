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

#ifndef MEDIA_META_DATA_LAYER_H_
#define MEDIA_META_DATA_LAYER_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef void MediaMetaDataWrapper;

enum {
    MEDIA_META_DATA_KEY_TIME = 1,
    MEDIA_META_DATA_KEY_IS_CODEC_CONFIG = 2,
    MEDIA_META_DATA_KEY_MIME = 3,
    MEDIA_META_DATA_KEY_NUM_BUFFERS = 4,
    MEDIA_META_DATA_KEY_WIDTH = 5,
    MEDIA_META_DATA_KEY_HEIGHT = 6,
    MEDIA_META_DATA_KEY_STRIDE = 7,
    MEDIA_META_DATA_KEY_COLOR_FORMAT = 8,
    MEDIA_META_DATA_KEY_SLICE_HEIGHT = 9,
    MEDIA_META_DATA_KEY_FRAMERATE = 10,
    MEDIA_META_DATA_KEY_MEDIA_BUFFER = 11
};

uint32_t media_meta_data_get_key_id(int key);

MediaMetaDataWrapper* media_meta_data_create();
void media_meta_data_release(MediaMetaDataWrapper *meta_data);

void media_meta_data_clear(MediaMetaDataWrapper *meta_data);
bool media_meta_data_remove(MediaMetaDataWrapper *meta_data, uint32_t key);

bool media_meta_data_set_cstring(MediaMetaDataWrapper *meta_data, uint32_t key, const char *value);
bool media_meta_data_set_int32(MediaMetaDataWrapper *meta_data, uint32_t key, int32_t value);
bool media_meta_data_set_int64(MediaMetaDataWrapper *meta_data, uint32_t key, int64_t value);
bool media_meta_data_set_float(MediaMetaDataWrapper *meta_data, uint32_t key, float value);
bool media_meta_data_set_pointer(MediaMetaDataWrapper *meta_data, uint32_t key, void *value);

bool media_meta_data_find_cstring(MediaMetaDataWrapper *meta_data, uint32_t key, const char **value);
bool media_meta_data_find_int32(MediaMetaDataWrapper *meta_data, uint32_t key, int32_t *value);
bool media_meta_data_find_int64(MediaMetaDataWrapper *meta_data, uint32_t key, int64_t *value);
bool media_meta_data_find_float(MediaMetaDataWrapper *meta_data, uint32_t key, float *value);
bool media_meta_data_find_double(MediaMetaDataWrapper *meta_data, uint32_t key, double *value);
bool media_meta_data_find_pointer(MediaMetaDataWrapper *meta_data, uint32_t key, void **value);

#ifdef __cplusplus
}
#endif

#endif
