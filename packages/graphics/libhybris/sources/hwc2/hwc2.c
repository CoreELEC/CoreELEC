/*
 * Copyright (C) 2018 TheKit <nekit1000@gmail.com>
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

#if HAS_HWCOMPOSER2_HEADERS

#include <dlfcn.h>
#include <stddef.h>

#include <hybris/common/binding.h>
#include <hybris/hwc2/hwc2_compatibility_layer.h>

#define COMPAT_LIBRARY_PATH		"libhwc2_compat_layer.so"

HYBRIS_LIBRARY_INITIALIZE(hwc2, COMPAT_LIBRARY_PATH);

HYBRIS_IMPLEMENT_FUNCTION1(hwc2, hwc2_compat_device_t*, hwc2_compat_device_new,
                           bool);

HYBRIS_IMPLEMENT_VOID_FUNCTION3(hwc2, hwc2_compat_device_register_callback,
                                hwc2_compat_device_t*, HWC2EventListener*, int);

HYBRIS_IMPLEMENT_VOID_FUNCTION3(hwc2, hwc2_compat_device_on_hotplug,
                                hwc2_compat_device_t*, hwc2_display_t, bool);

HYBRIS_IMPLEMENT_FUNCTION2(hwc2, hwc2_compat_display_t*,
                           hwc2_compat_device_get_display_by_id,
                           hwc2_compat_device_t*,
                           hwc2_display_t);

HYBRIS_IMPLEMENT_FUNCTION1(hwc2, HWC2DisplayConfig*,
                           hwc2_compat_display_get_active_config,
                           hwc2_compat_display_t*);

HYBRIS_IMPLEMENT_FUNCTION1(hwc2, hwc2_error_t,
                           hwc2_compat_display_accept_changes,
                           hwc2_compat_display_t*);

HYBRIS_IMPLEMENT_FUNCTION1(hwc2, hwc2_compat_layer_t*,
                           hwc2_compat_display_create_layer,
                           hwc2_compat_display_t*);

HYBRIS_IMPLEMENT_VOID_FUNCTION2(hwc2, hwc2_compat_display_destroy_layer,
                                hwc2_compat_display_t*, hwc2_compat_layer_t*);

HYBRIS_IMPLEMENT_FUNCTION2(hwc2, hwc2_error_t,
                           hwc2_compat_display_get_release_fences,
                           hwc2_compat_display_t*, hwc2_compat_out_fences_t**);

HYBRIS_IMPLEMENT_FUNCTION2(hwc2, hwc2_error_t, hwc2_compat_display_present,
                           hwc2_compat_display_t*, int32_t*);

HYBRIS_IMPLEMENT_FUNCTION5(hwc2, hwc2_error_t, hwc2_compat_display_set_client_target,
                           hwc2_compat_display_t*, uint32_t,
                           ANativeWindowBuffer_t*, int32_t,
                           android_dataspace_t);

HYBRIS_IMPLEMENT_FUNCTION2(hwc2, hwc2_error_t, hwc2_compat_display_set_power_mode,
                           hwc2_compat_display_t*, int);
HYBRIS_IMPLEMENT_FUNCTION2(hwc2, hwc2_error_t, hwc2_compat_display_set_vsync_enabled,
                           hwc2_compat_display_t*, int);

HYBRIS_IMPLEMENT_FUNCTION3(hwc2, hwc2_error_t, hwc2_compat_display_validate,
                           hwc2_compat_display_t*, uint32_t*, uint32_t*);

HYBRIS_IMPLEMENT_FUNCTION5(hwc2, hwc2_error_t, hwc2_compat_display_present_or_validate,
                           hwc2_compat_display_t*, uint32_t*, uint32_t*,
                           int32_t*, uint32_t*);

HYBRIS_IMPLEMENT_FUNCTION4(hwc2, hwc2_error_t, hwc2_compat_layer_set_buffer,
                           hwc2_compat_layer_t*, uint32_t,
                           ANativeWindowBuffer_t*, int32_t);
HYBRIS_IMPLEMENT_FUNCTION2(hwc2, hwc2_error_t, hwc2_compat_layer_set_blend_mode,
                           hwc2_compat_layer_t*, int);
HYBRIS_IMPLEMENT_FUNCTION2(hwc2, hwc2_error_t, hwc2_compat_layer_set_color,
                           hwc2_compat_layer_t*, hwc_color_t);
HYBRIS_IMPLEMENT_FUNCTION2(hwc2, hwc2_error_t, hwc2_compat_layer_set_composition_type,
                           hwc2_compat_layer_t*, int);
HYBRIS_IMPLEMENT_FUNCTION2(hwc2, hwc2_error_t, hwc2_compat_layer_set_dataspace,
                           hwc2_compat_layer_t*, android_dataspace_t);
HYBRIS_IMPLEMENT_FUNCTION5(hwc2, hwc2_error_t, hwc2_compat_layer_set_display_frame,
                           hwc2_compat_layer_t*, int32_t, int32_t,
                           int32_t, int32_t);
HYBRIS_IMPLEMENT_FUNCTION2(hwc2, hwc2_error_t, hwc2_compat_layer_set_plane_alpha,
                           hwc2_compat_layer_t*, float);
HYBRIS_IMPLEMENT_FUNCTION2(hwc2, hwc2_error_t, hwc2_compat_layer_set_sideband_stream,
                           hwc2_compat_layer_t*, native_handle_t*);
HYBRIS_IMPLEMENT_FUNCTION5(hwc2, hwc2_error_t, hwc2_compat_layer_set_source_crop,
                           hwc2_compat_layer_t*, float, float, float, float);
HYBRIS_IMPLEMENT_FUNCTION2(hwc2, hwc2_error_t, hwc2_compat_layer_set_transform,
                           hwc2_compat_layer_t*, int);
HYBRIS_IMPLEMENT_FUNCTION5(hwc2, hwc2_error_t, hwc2_compat_layer_set_visible_region,
                           hwc2_compat_layer_t*, int32_t, int32_t,
                           int32_t, int32_t);

HYBRIS_IMPLEMENT_FUNCTION2(hwc2, int32_t, hwc2_compat_out_fences_get_fence,
                           hwc2_compat_out_fences_t*, hwc2_compat_layer_t*);
HYBRIS_IMPLEMENT_VOID_FUNCTION1(hwc2, hwc2_compat_out_fences_destroy,
                                hwc2_compat_out_fences_t*);

#endif

