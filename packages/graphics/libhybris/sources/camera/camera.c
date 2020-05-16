/*
 * Copyright (C) 2013 - 2014 Canonical Ltd
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
 * Authored by: Michael Frey <michael.frey@canonical.com>
 *              Ricardo Salveti de Araujo <ricardo.salveti@canonical.com>
 */

#include <android-config.h>
#include <dlfcn.h>
#include <stddef.h>

#include <hybris/common/binding.h>
#include <hybris/camera/camera_compatibility_layer.h>
#include <hybris/camera/camera_compatibility_layer_capabilities.h>
#include <hybris/surface_flinger/surface_flinger_compatibility_layer.h>

#define COMPAT_LIBRARY_PATH "libcamera_compat_layer.so"

HYBRIS_LIBRARY_INITIALIZE(camera, COMPAT_LIBRARY_PATH);

HYBRIS_IMPLEMENT_FUNCTION0(camera, int, android_camera_get_number_of_devices);
HYBRIS_IMPLEMENT_FUNCTION3(camera, int, android_camera_get_device_info, int32_t, int*, int*);
HYBRIS_IMPLEMENT_FUNCTION2(camera, struct CameraControl*, android_camera_connect_to,
	CameraType, struct CameraControlListener*);
HYBRIS_IMPLEMENT_FUNCTION2(camera, struct CameraControl*, android_camera_connect_by_id,
	int32_t, struct CameraControlListener*);
HYBRIS_IMPLEMENT_VOID_FUNCTION1(camera, android_camera_disconnect,
	struct CameraControl*);
HYBRIS_IMPLEMENT_FUNCTION1(camera, int, android_camera_lock, struct CameraControl*);
HYBRIS_IMPLEMENT_FUNCTION1(camera, int, android_camera_unlock, struct CameraControl*);
HYBRIS_IMPLEMENT_VOID_FUNCTION1(camera, android_camera_delete, struct CameraControl*);
HYBRIS_IMPLEMENT_VOID_FUNCTION1(camera, android_camera_dump_parameters,
	struct CameraControl*);

// Setters
HYBRIS_IMPLEMENT_VOID_FUNCTION2(camera, android_camera_set_effect_mode,
	struct CameraControl*, EffectMode);
HYBRIS_IMPLEMENT_VOID_FUNCTION2(camera, android_camera_set_flash_mode,
	struct CameraControl*, FlashMode);
HYBRIS_IMPLEMENT_VOID_FUNCTION2(camera, android_camera_set_white_balance_mode,
	struct CameraControl*, WhiteBalanceMode);
HYBRIS_IMPLEMENT_VOID_FUNCTION2(camera, android_camera_set_scene_mode,
	struct CameraControl*, SceneMode);
HYBRIS_IMPLEMENT_VOID_FUNCTION2(camera, android_camera_set_auto_focus_mode,
	struct CameraControl*, AutoFocusMode);
HYBRIS_IMPLEMENT_VOID_FUNCTION2(camera, android_camera_set_preview_format,
	struct CameraControl*, CameraPixelFormat);
HYBRIS_IMPLEMENT_VOID_FUNCTION3(camera, android_camera_set_picture_size,
	struct CameraControl*, int, int);
HYBRIS_IMPLEMENT_VOID_FUNCTION3(camera, android_camera_set_thumbnail_size,
	struct CameraControl*, int, int);
HYBRIS_IMPLEMENT_VOID_FUNCTION3(camera, android_camera_set_preview_size,
	struct CameraControl*, int, int);
HYBRIS_IMPLEMENT_VOID_FUNCTION2(camera, android_camera_set_display_orientation,
	struct CameraControl*, int32_t);
HYBRIS_IMPLEMENT_VOID_FUNCTION2(camera, android_camera_set_preview_texture,
	struct CameraControl*, int);
HYBRIS_IMPLEMENT_VOID_FUNCTION2(camera, android_camera_set_preview_surface,
	struct CameraControl*, struct SfSurface*);
HYBRIS_IMPLEMENT_VOID_FUNCTION2(camera, android_camera_set_focus_region,
	struct CameraControl*, FocusRegion*);
HYBRIS_IMPLEMENT_VOID_FUNCTION1(camera, android_camera_reset_focus_region,
	struct CameraControl*);
HYBRIS_IMPLEMENT_VOID_FUNCTION2(camera, android_camera_set_metering_region,
        struct CameraControl*, MeteringRegion*);
HYBRIS_IMPLEMENT_VOID_FUNCTION1(camera, android_camera_reset_metering_region,
        struct CameraControl*);
HYBRIS_IMPLEMENT_VOID_FUNCTION2(camera, android_camera_set_preview_fps,
	struct CameraControl*, int);
HYBRIS_IMPLEMENT_VOID_FUNCTION2(camera, android_camera_set_rotation,
	struct CameraControl*, int);
HYBRIS_IMPLEMENT_VOID_FUNCTION6(camera, android_camera_set_location,
	struct CameraControl*, const float*, const float*, const float*, int, const char*);
HYBRIS_IMPLEMENT_VOID_FUNCTION3(camera, android_camera_set_video_size,
	struct CameraControl*, int, int);
HYBRIS_IMPLEMENT_VOID_FUNCTION2(camera, android_camera_set_jpeg_quality,
	struct CameraControl*, int);

// Getters
HYBRIS_IMPLEMENT_VOID_FUNCTION2(camera, android_camera_get_effect_mode,
	struct CameraControl*, EffectMode*);
HYBRIS_IMPLEMENT_VOID_FUNCTION2(camera, android_camera_get_flash_mode,
	struct CameraControl*, FlashMode*);
HYBRIS_IMPLEMENT_VOID_FUNCTION2(camera, android_camera_get_white_balance_mode,
	struct CameraControl*, WhiteBalanceMode*);
HYBRIS_IMPLEMENT_VOID_FUNCTION2(camera, android_camera_get_scene_mode,
	struct CameraControl*, SceneMode*);
HYBRIS_IMPLEMENT_VOID_FUNCTION2(camera, android_camera_get_auto_focus_mode,
	struct CameraControl*, AutoFocusMode*);
HYBRIS_IMPLEMENT_VOID_FUNCTION2(camera, android_camera_get_max_zoom,
	struct CameraControl*, int*);
HYBRIS_IMPLEMENT_VOID_FUNCTION3(camera, android_camera_get_picture_size,
	struct CameraControl*, int*, int*);
HYBRIS_IMPLEMENT_VOID_FUNCTION3(camera, android_camera_get_thumbnail_size,
	struct CameraControl*, int*, int*);
HYBRIS_IMPLEMENT_VOID_FUNCTION3(camera, android_camera_get_preview_size,
	struct CameraControl*, int*, int*);
HYBRIS_IMPLEMENT_VOID_FUNCTION3(camera, android_camera_get_preview_fps_range,
	struct CameraControl*, int*, int*);
HYBRIS_IMPLEMENT_VOID_FUNCTION2(camera, android_camera_get_preview_fps,
	struct CameraControl*, int*);
HYBRIS_IMPLEMENT_VOID_FUNCTION2(camera, android_camera_get_preview_texture_transformation,
	struct CameraControl*, float*);
HYBRIS_IMPLEMENT_VOID_FUNCTION3(camera, android_camera_get_video_size,
	struct CameraControl*, int*, int*);
HYBRIS_IMPLEMENT_VOID_FUNCTION2(camera, android_camera_get_jpeg_quality,
	struct CameraControl*, int*);

// Enumerators
HYBRIS_IMPLEMENT_VOID_FUNCTION3(camera, android_camera_enumerate_supported_picture_sizes,
	struct CameraControl*, size_callback, void*);
HYBRIS_IMPLEMENT_VOID_FUNCTION3(camera, android_camera_enumerate_supported_thumbnail_sizes,
	struct CameraControl*, size_callback, void*);
HYBRIS_IMPLEMENT_VOID_FUNCTION3(camera, android_camera_enumerate_supported_preview_sizes,
	struct CameraControl*, size_callback, void*);
HYBRIS_IMPLEMENT_VOID_FUNCTION3(camera, android_camera_enumerate_supported_video_sizes,
	struct CameraControl*, size_callback, void*);
HYBRIS_IMPLEMENT_VOID_FUNCTION3(camera, android_camera_enumerate_supported_scene_modes,
	struct CameraControl*, scene_mode_callback, void*);
HYBRIS_IMPLEMENT_VOID_FUNCTION3(camera, android_camera_enumerate_supported_flash_modes,
	struct CameraControl*, flash_mode_callback, void*);

HYBRIS_IMPLEMENT_VOID_FUNCTION1(camera, android_camera_update_preview_texture, struct CameraControl*);

HYBRIS_IMPLEMENT_VOID_FUNCTION1(camera, android_camera_start_preview, struct CameraControl*);
HYBRIS_IMPLEMENT_VOID_FUNCTION1(camera, android_camera_stop_preview, struct CameraControl*);
HYBRIS_IMPLEMENT_VOID_FUNCTION1(camera, android_camera_start_autofocus, struct CameraControl*);
HYBRIS_IMPLEMENT_VOID_FUNCTION1(camera, android_camera_stop_autofocus, struct CameraControl*);

HYBRIS_IMPLEMENT_VOID_FUNCTION2(camera, android_camera_start_zoom, struct CameraControl*, int32_t);
HYBRIS_IMPLEMENT_VOID_FUNCTION2(camera, android_camera_set_zoom, struct CameraControl*, int32_t);
HYBRIS_IMPLEMENT_VOID_FUNCTION1(camera, android_camera_stop_zoom, struct CameraControl*);
HYBRIS_IMPLEMENT_VOID_FUNCTION1(camera, android_camera_take_snapshot, struct CameraControl*);

HYBRIS_IMPLEMENT_FUNCTION2(camera, int, android_camera_set_preview_callback_mode,
	struct CameraControl*, PreviewCallbackMode);
