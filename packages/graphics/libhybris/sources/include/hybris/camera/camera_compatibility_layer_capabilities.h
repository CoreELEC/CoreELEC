/*
 * Copyright (C) 2013-2014 Canonical Ltd
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
 * Authored by: Thomas Voss <thomas.voss@canonical.com>
 */

#ifndef CAMERA_COMPATIBILITY_LAYER_CONFIGURATION_H_
#define CAMERA_COMPATIBILITY_LAYER_CONFIGURATION_H_

#ifdef __cplusplus
extern "C" {
#endif

    struct CameraControl;

    typedef enum
    {
        FLASH_MODE_OFF,
        FLASH_MODE_AUTO,
        FLASH_MODE_ON,
        FLASH_MODE_TORCH,
        FLASH_MODE_RED_EYE
    } FlashMode;

    typedef enum
    {
        WHITE_BALANCE_MODE_AUTO,
        WHITE_BALANCE_MODE_DAYLIGHT,
        WHITE_BALANCE_MODE_CLOUDY_DAYLIGHT,
        WHITE_BALANCE_MODE_FLUORESCENT,
        WHITE_BALANCE_MODE_INCANDESCENT
    } WhiteBalanceMode;

    typedef enum
    {
        SCENE_MODE_AUTO,
        SCENE_MODE_ACTION,
        SCENE_MODE_NIGHT,
        SCENE_MODE_PARTY,
        SCENE_MODE_SUNSET,
        SCENE_MODE_HDR
    } SceneMode;

    typedef enum
    {
        AUTO_FOCUS_MODE_OFF,
        AUTO_FOCUS_MODE_CONTINUOUS_VIDEO,
        AUTO_FOCUS_MODE_AUTO,
        AUTO_FOCUS_MODE_MACRO,
        AUTO_FOCUS_MODE_CONTINUOUS_PICTURE,
        AUTO_FOCUS_MODE_INFINITY
    } AutoFocusMode;

    typedef enum
    {
        EFFECT_MODE_NONE,
        EFFECT_MODE_MONO,
        EFFECT_MODE_NEGATIVE,
        EFFECT_MODE_SOLARIZE,
        EFFECT_MODE_SEPIA,
        EFFECT_MODE_POSTERIZE,
        EFFECT_MODE_WHITEBOARD,
        EFFECT_MODE_BLACKBOARD,
        EFFECT_MODE_AQUA
    } EffectMode;

    typedef enum
    {
        CAMERA_PIXEL_FORMAT_YUV422SP,
        CAMERA_PIXEL_FORMAT_YUV420SP,
        CAMERA_PIXEL_FORMAT_YUV422I,
        CAMERA_PIXEL_FORMAT_YUV420P,
        CAMERA_PIXEL_FORMAT_RGB565,
        CAMERA_PIXEL_FORMAT_RGBA8888,
        CAMERA_PIXEL_FORMAT_BAYER_RGGB
    } CameraPixelFormat;

    typedef struct
    {
        int top;
        int left;
        int bottom;
        int right;
        int weight;
    } FocusRegion, MeteringRegion;

    typedef void (*size_callback)(void* ctx, int width, int height);
    typedef void (*scene_mode_callback)(void* ctx, SceneMode mode);
    typedef void (*flash_mode_callback)(void* ctx, FlashMode mode);

    // Dumps the camera parameters to stdout.
    void android_camera_dump_parameters(struct CameraControl* control);

    // Query camera parameters
    int android_camera_get_number_of_devices();
    int android_camera_get_device_info(int32_t camera_id, int* facing, int* orientation);
    void android_camera_enumerate_supported_preview_sizes(struct CameraControl* control, size_callback cb, void* ctx);
    void android_camera_get_preview_fps_range(struct CameraControl* control, int* min, int* max);
    void android_camera_get_preview_fps(struct CameraControl* control, int* fps);
    void android_camera_enumerate_supported_picture_sizes(struct CameraControl* control, size_callback cb, void* ctx);
    void android_camera_enumerate_supported_thumbnail_sizes(struct CameraControl* control, size_callback cb, void* ctx);
    void android_camera_get_preview_size(struct CameraControl* control, int* width, int* height);
    void android_camera_get_picture_size(struct CameraControl* control, int* width, int* height);
    void android_camera_get_thumbnail_size(struct CameraControl* control, int* width, int* height);
    void android_camera_get_jpeg_quality(struct CameraControl* control, int* quality);

    void android_camera_get_current_zoom(struct CameraControl* control, int* zoom);
    void android_camera_get_max_zoom(struct CameraControl* control, int* max_zoom);

    void android_camera_get_effect_mode(struct CameraControl* control, EffectMode* mode);
    void android_camera_get_flash_mode(struct CameraControl* control, FlashMode* mode);
    void android_camera_enumerate_supported_flash_modes(struct CameraControl* control, flash_mode_callback cb, void* ctx);
    void android_camera_get_white_balance_mode(struct CameraControl* control, WhiteBalanceMode* mode);
    void android_camera_enumerate_supported_scene_modes(struct CameraControl* control, scene_mode_callback cb, void* ctx);
    void android_camera_get_scene_mode(struct CameraControl* control, SceneMode* mode);
    void android_camera_get_auto_focus_mode(struct CameraControl* control, AutoFocusMode* mode);
    void android_camera_get_preview_format(struct CameraControl* control, CameraPixelFormat* format);

    // Adjusts camera parameters
    void android_camera_set_preview_size(struct CameraControl* control, int width, int height);
    void android_camera_set_preview_fps(struct CameraControl* control, int fps);
    void android_camera_set_picture_size(struct CameraControl* control, int width, int height);
    void android_camera_set_thumbnail_size(struct CameraControl* control, int width, int height);
    void android_camera_set_effect_mode(struct CameraControl* control, EffectMode mode);
    void android_camera_set_flash_mode(struct CameraControl* control, FlashMode mode);
    void android_camera_set_white_balance_mode(struct CameraControl* control, WhiteBalanceMode mode);
    void android_camera_set_scene_mode(struct CameraControl* control, SceneMode mode);
    void android_camera_set_auto_focus_mode(struct CameraControl* control, AutoFocusMode mode);
    void android_camera_set_preview_format(struct CameraControl* control, CameraPixelFormat format);
    void android_camera_set_jpeg_quality(struct CameraControl* control, int quality);

    void android_camera_set_focus_region(struct CameraControl* control, FocusRegion* region);
    void android_camera_reset_focus_region(struct CameraControl* control);

    void android_camera_set_metering_region(struct CameraControl* control, MeteringRegion* region);
    void android_camera_reset_metering_region(struct CameraControl* control);

    // Set photo metadata
    void android_camera_set_rotation(struct CameraControl* control, int rotation);
    void android_camera_set_location(struct CameraControl* control, const float* latitude, const float* longitude, const float* altitude, int timestamp, const char* method);

    // Video support
    void android_camera_enumerate_supported_video_sizes(struct CameraControl* control, size_callback cb, void* ctx);
    void android_camera_get_video_size(struct CameraControl* control, int* width, int* height);
    void android_camera_set_video_size(struct CameraControl* control, int width, int height);

#ifdef __cplusplus
}
#endif

#endif // CAMERA_COMPATIBILITY_LAYER_CONFIGURATION_H_
