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

#ifndef MEDIA_MESSAGE_LAYER_H_
#define MEDIA_MESSAGE_LAYER_H_

#include <sys/types.h>

#include <stdint.h>
#include <stdbool.h>
#include <memory.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef void MediaMessageWrapper;

MediaMessageWrapper* media_message_create();
void media_message_release(MediaMessageWrapper *msg);

void media_message_clear(MediaMessageWrapper *msg);
const char* media_message_dump(MediaMessageWrapper *msg);

void media_message_set_int32(MediaMessageWrapper *msg, const char *name, int32_t value);
void media_message_set_int64(MediaMessageWrapper *msg, const char *name, int64_t value);
void media_message_set_size(MediaMessageWrapper *msg, const char *name, size_t value);
void media_message_set_float(MediaMessageWrapper *msg, const char *name, float value);
void media_message_set_double(MediaMessageWrapper *msg, const char *name, double value);
void media_message_set_string(MediaMessageWrapper *msg, const char *name, const char *value, ssize_t len);

bool media_message_find_int32(MediaMessageWrapper *msg, const char *name, int32_t *value);
bool media_message_find_int64(MediaMessageWrapper *msg, const char *name, int64_t *value);
bool media_message_find_size(MediaMessageWrapper *msg, const char *name, size_t *value);
bool media_message_find_float(MediaMessageWrapper *msg, const char *name, float *value);
bool media_message_find_double(MediaMessageWrapper *msg, const char *name, double *value);

#ifdef __cplusplus
}
#endif

#endif
