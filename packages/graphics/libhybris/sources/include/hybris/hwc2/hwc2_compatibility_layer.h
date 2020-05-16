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

#ifndef HWC2_COMPATIBILITY_LAYER_H_
#define HWC2_COMPATIBILITY_LAYER_H_

#include <stdbool.h>
#include <stdint.h>
#include <unistd.h>

#include <hardware/hwcomposer2.h>
#include <system/graphics.h>
#include <system/window.h>

#ifdef __cplusplus
extern "C" {
#endif
    struct HWC2EventListener;
    typedef struct HWC2EventListener HWC2EventListener;

    typedef void (*on_vsync_received_callback)(HWC2EventListener* self,
                    int32_t sequenceId, hwc2_display_t display,
                    int64_t timestamp);
    typedef void (*on_hotplug_received_callback)(HWC2EventListener* self,
                    int32_t sequenceId, hwc2_display_t display,
                    bool connected, bool primaryDisplay);
    typedef void (*on_refresh_received_callback)(HWC2EventListener* self,
                    int32_t sequenceId, hwc2_display_t display);

    struct HWC2EventListener
    {
        on_vsync_received_callback on_vsync_received;
        on_hotplug_received_callback on_hotplug_received;
        on_refresh_received_callback on_refresh_received;
    };

    typedef struct HWC2DisplayConfig {
        hwc2_config_t id;
        hwc2_display_t display;
        int32_t width;
        int32_t height;
        int64_t vsyncPeriod;
        float dpiX;
        float dpiY;
    } HWC2DisplayConfig;

    struct hwc2_compat_device;
    typedef struct hwc2_compat_device hwc2_compat_device_t;

    struct hwc2_compat_display;
    typedef struct hwc2_compat_display hwc2_compat_display_t;

    struct hwc2_compat_layer;
    typedef struct hwc2_compat_layer hwc2_compat_layer_t;

    struct hwc2_compat_out_fences;
    typedef struct hwc2_compat_out_fences hwc2_compat_out_fences_t;

    hwc2_compat_device_t* hwc2_compat_device_new(bool);
    void hwc2_compat_device_register_callback(hwc2_compat_device_t* device,
                                              HWC2EventListener* listener,
                                              int composerSequenceId);

    void hwc2_compat_device_on_hotplug(hwc2_compat_device_t* device,
                                       hwc2_display_t displayId,
                                       bool connected);

    hwc2_compat_display_t* hwc2_compat_device_get_display_by_id(
                                hwc2_compat_device_t* device,
                                hwc2_display_t id);

    HWC2DisplayConfig* hwc2_compat_display_get_active_config(
                                hwc2_compat_display_t* display);

    hwc2_error_t hwc2_compat_display_accept_changes(hwc2_compat_display_t* display);
    hwc2_compat_layer_t* hwc2_compat_display_create_layer(hwc2_compat_display_t*
                                                          display);
    void hwc2_compat_display_destroy_layer(hwc2_compat_display_t* display,
                                           hwc2_compat_layer_t* layer);

    hwc2_error_t hwc2_compat_display_get_release_fences(
                                        hwc2_compat_display_t* display,
                                        hwc2_compat_out_fences_t** outFences);

    hwc2_error_t hwc2_compat_display_present(hwc2_compat_display_t* display,
                                     int32_t* outPresentFence);

    hwc2_error_t hwc2_compat_display_set_client_target(hwc2_compat_display_t* display,
                                               uint32_t slot,
                                               struct ANativeWindowBuffer* buffer,
                                               const int32_t acquireFenceFd,
                                               android_dataspace_t dataspace);

    hwc2_error_t hwc2_compat_display_set_power_mode(hwc2_compat_display_t* display,
                                            int mode);
    hwc2_error_t hwc2_compat_display_set_vsync_enabled(hwc2_compat_display_t* display,
                                               int enabled);

    hwc2_error_t hwc2_compat_display_validate(hwc2_compat_display_t* display,
                                         uint32_t* outNumTypes,
                                         uint32_t* outNumRequests);

    hwc2_error_t hwc2_compat_display_present_or_validate(hwc2_compat_display_t* display,
                                                 uint32_t* outNumTypes,
                                                 uint32_t* outNumRequests,
                                                 int32_t* outPresentFence,
                                                 uint32_t* state);

    hwc2_error_t hwc2_compat_layer_set_buffer(hwc2_compat_layer_t* layer,
                                              uint32_t slot,
                                              struct ANativeWindowBuffer* buffer,
                                              const int32_t acquireFenceFd);
    hwc2_error_t hwc2_compat_layer_set_blend_mode(hwc2_compat_layer_t* layer, int mode);
    hwc2_error_t hwc2_compat_layer_set_color(hwc2_compat_layer_t* layer,
                                     hwc_color_t color);
    hwc2_error_t hwc2_compat_layer_set_composition_type(hwc2_compat_layer_t* layer,
                                                int type);
    hwc2_error_t hwc2_compat_layer_set_dataspace(hwc2_compat_layer_t* layer,
                                         android_dataspace_t dataspace);
    hwc2_error_t hwc2_compat_layer_set_display_frame(hwc2_compat_layer_t* layer,
                                             int32_t left, int32_t top,
                                             int32_t right, int32_t bottom);
    hwc2_error_t hwc2_compat_layer_set_plane_alpha(hwc2_compat_layer_t* layer,
                                           float alpha);
    hwc2_error_t hwc2_compat_layer_set_sideband_stream(hwc2_compat_layer_t* layer,
                                               native_handle_t* stream);
    hwc2_error_t hwc2_compat_layer_set_source_crop(hwc2_compat_layer_t* layer,
                                           float left, float top,
                                           float right, float bottom);
    hwc2_error_t hwc2_compat_layer_set_transform(hwc2_compat_layer_t* layer,
                                         int transform);
    hwc2_error_t hwc2_compat_layer_set_visible_region(hwc2_compat_layer_t* layer,
                                              int32_t left, int32_t top,
                                              int32_t right, int32_t bottom);

    int32_t hwc2_compat_out_fences_get_fence(hwc2_compat_out_fences_t* fences,
                                             hwc2_compat_layer_t* layer);
    void hwc2_compat_out_fences_destroy(hwc2_compat_out_fences_t* fences);

#ifdef __cplusplus
}
#endif

#endif // HWC2_COMPATIBILITY_LAYER_H_
