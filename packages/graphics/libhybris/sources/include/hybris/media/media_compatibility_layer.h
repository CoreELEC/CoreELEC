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

#ifndef MEDIA_COMPATIBILITY_LAYER_H_
#define MEDIA_COMPATIBILITY_LAYER_H_

#include <stdint.h>
#include <unistd.h>
#include <stdbool.h>

#include <GLES2/gl2.h>

#ifdef __cplusplus
extern "C" {
#endif

    // Common compat calls
    int media_compat_check_availability();

    unsigned int hybris_media_get_version();

    // Callback types
    typedef void (*on_msg_set_video_size)(int height, int width, void *context);
    typedef void (*on_video_texture_needs_update)(void *context);
    typedef void (*on_msg_error)(void *context);
    typedef void (*on_playback_complete)(void *context);
    typedef void (*on_media_prepared)(void *context);

    struct MediaPlayerWrapper;

    // ----- Start of C API ----- //

    // Callback setters
    void android_media_set_video_size_cb(struct MediaPlayerWrapper *mp, on_msg_set_video_size cb, void *context);
    void android_media_set_video_texture_needs_update_cb(struct MediaPlayerWrapper *mp, on_video_texture_needs_update cb, void *context);
    void android_media_set_error_cb(struct MediaPlayerWrapper *mp, on_msg_error cb, void *context);
    void android_media_set_playback_complete_cb(struct MediaPlayerWrapper *mp, on_playback_complete cb, void *context);
    void android_media_set_media_prepared_cb(struct MediaPlayerWrapper *mp, on_media_prepared cb, void *context);

    // Main player control API
    struct MediaPlayerWrapper *android_media_new_player();
    int android_media_set_data_source(struct MediaPlayerWrapper *mp, const char* url);
    int android_media_set_preview_texture(struct MediaPlayerWrapper *mp, int texture_id);
    void android_media_update_surface_texture(struct MediaPlayerWrapper *mp);
    void android_media_surface_texture_get_transformation_matrix(struct MediaPlayerWrapper *mp, GLfloat*matrix);
    int android_media_play(struct MediaPlayerWrapper *mp);
    int android_media_pause(struct MediaPlayerWrapper *mp);
    int android_media_stop(struct MediaPlayerWrapper *mp);
    bool android_media_is_playing(struct MediaPlayerWrapper *mp);

    int android_media_seek_to(struct MediaPlayerWrapper *mp, int msec);
    int android_media_get_current_position(struct MediaPlayerWrapper *mp, int *msec);
    int android_media_get_duration(struct MediaPlayerWrapper *mp, int *msec);

    int android_media_get_volume(struct MediaPlayerWrapper *mp, int *volume);
    int android_media_set_volume(struct MediaPlayerWrapper *mp, int volume);

#ifdef __cplusplus
}
#endif

#endif // MEDIA_COMPATIBILITY_LAYER_H_
