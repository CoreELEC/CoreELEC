/*
 * Copyright (C) 2013 Canonical Ltd
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
 * Authored by: Jim Hodapp <jim.hodapp@canonical.com>
 */

#ifndef MEDIA_CODEC_LIST_PRIV_H_
#define MEDIA_CODEC_LIST_PRIV_H_

#include <stddef.h>
#include <stdbool.h>
#include <unistd.h>

#ifdef __cplusplus
extern "C" {
#endif

    ssize_t media_codec_list_find_codec_by_type(const char *type, bool encoder, size_t startIndex);
    ssize_t media_codec_list_find_codec_by_name(const char *name);
    size_t media_codec_list_count_codecs();
    void media_codec_list_get_codec_info_at_id(size_t index);
    const char *media_codec_list_get_codec_name(size_t index);
    bool media_codec_list_is_encoder(size_t index);
    size_t media_codec_list_get_num_supported_types(size_t index);
    size_t media_codec_list_get_nth_supported_type_len(size_t index, size_t n);
    int media_codec_list_get_nth_supported_type(size_t index, char *type, size_t n);

    struct _profile_level
    {
        uint32_t profile;
        uint32_t level;
    };
    typedef struct _profile_level profile_level;

    size_t media_codec_list_get_num_profile_levels(size_t index, const char*);
    size_t media_codec_list_get_num_color_formats(size_t index, const char*);
    int media_codec_list_get_nth_codec_profile_level(size_t index, const char *type, profile_level *pro_level, size_t n);
    int media_codec_list_get_codec_color_formats(size_t index, const char *type, uint32_t *color_formats);

#ifdef __cplusplus
}
#endif

#endif // MEDIA_CODEC_LIST_PRIV_H_
