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
 * Authored by: Thomas Voss <thomas.voss@canonical.com>
 */

#ifndef CAMERA_COMPATIBILITY_LAYER_H_
#define CAMERA_COMPATIBILITY_LAYER_H_

#include <stdint.h>
#include <unistd.h>

#ifdef __cplusplus
extern "C" {
#endif

    // Forward declarations
    struct SfSurface;

    typedef enum
    {
        // This camera has higher quality and features like high resolution and flash
        BACK_FACING_CAMERA_TYPE,
        // The camera that is on the same side as the touch display. Usually used for video calls
        FRONT_FACING_CAMERA_TYPE
    } CameraType;

    typedef enum
    {
        PREVIEW_CALLBACK_DISABLED,
        PREVIEW_CALLBACK_ENABLED
    } PreviewCallbackMode;

    struct CameraControl;

    typedef void (*on_msg_error)(void* context);
    typedef void (*on_msg_shutter)(void* context);
    typedef void (*on_msg_focus)(void* context);
    typedef void (*on_msg_zoom)(void* context, int32_t new_zoom_level);

    typedef void (*on_data_raw_image)(void* data, uint32_t data_size, void* context);
    typedef void (*on_data_compressed_image)(void* data, uint32_t data_size, void* context);
    typedef void (*on_preview_texture_needs_update)(void* context);
    typedef void (*on_preview_frame)(void* data, uint32_t data_size, void* context);

    struct CameraControlListener
    {
        // Called whenever an error occurs while the camera HAL executes a command
        on_msg_error on_msg_error_cb;
        // Called while taking a picture when the shutter has been triggered
        on_msg_shutter on_msg_shutter_cb;
        // Called while focusing
        on_msg_focus on_msg_focus_cb;
        // Called while zooming
        on_msg_zoom on_msg_zoom_cb;

        // Raw image data (Bayer pattern) is reported over this callback
        on_data_raw_image on_data_raw_image_cb;
        // JPEG-compressed image is reported over this callback
        on_data_compressed_image on_data_compressed_image_cb;

        // If a texture has been set as a destination for preview frames,
        // this callback is invoked whenever a new buffer from the camera is available
        // and needs to be uploaded to the texture by means of calling
        // android_camera_update_preview_texture. Please note that this callback can
        // be invoked on any thread, and android_camera_update_preview_texture must only
        // be called on the thread that setup the EGL/GL context.
        on_preview_texture_needs_update on_preview_texture_needs_update_cb;

        void* context;

        // Called when there is a new preview frame
        on_preview_frame on_preview_frame_cb;
    };

    // Initializes a connection to the camera, returns NULL on error.
    struct CameraControl* android_camera_connect_to(CameraType camera_type, struct CameraControlListener* listener);
    struct CameraControl* android_camera_connect_by_id(int32_t camera_id, struct CameraControlListener* listener);

    // Disconnects the camera and deletes the pointer
    void android_camera_disconnect(struct CameraControl* control);

    int android_camera_lock(struct CameraControl* control);
    int android_camera_unlock(struct CameraControl* control);

    // Deletes the CameraControl
    void android_camera_delete(struct CameraControl* control);

    // Passes the rotation r of the display in [°] relative to the camera to the camera HAL. r \in [0, 359].
    void android_camera_set_display_orientation(struct CameraControl* control, int32_t clockwise_rotation_degree);

    // Prepares the camera HAL to display preview images to the
    // supplied surface/texture in a H/W-acclerated way.  New frames
    // are reported via the
    // 'on_preview_texture_needs_update'-callback, and clients of this
    // API should invoke android_camera_update_preview_texture
    // subsequently. Please note that the texture is bound automatically by the underlying
    // implementation.
    void android_camera_set_preview_texture(struct CameraControl* control, int texture_id);

    // Reads out the transformation matrix that needs to be applied when displaying the texture
    void android_camera_get_preview_texture_transformation(struct CameraControl* control, float m[16]);

    // Updates the texture to the last received frame and binds the texture
    void android_camera_update_preview_texture(struct CameraControl* control);

    // Prepares the camera HAL to display preview images to the supplied surface/texture in a H/W-acclerated way.
    void android_camera_set_preview_surface(struct CameraControl* control, struct SfSurface* surface);

    // Starts the camera preview
    void android_camera_start_preview(struct CameraControl* control);

    // Stops the camera preview
    void android_camera_stop_preview(struct CameraControl* control);

    // Starts an autofocus operation of the camera, results are reported via callback.
    void android_camera_start_autofocus(struct CameraControl* control);

    // Stops an ongoing autofocus operation.
    void android_camera_stop_autofocus(struct CameraControl* control);

    // Starts a zooming operation, zoom values are absolute, valid values [1, \infty].
    void android_camera_start_zoom(struct CameraControl* control, int32_t zoom);

    // Stops an ongoing zoom operation.
    void android_camera_stop_zoom(struct CameraControl* control);

    // Adjust the zoom level immediately as opposed to smoothly zoomin gin.
    void android_camera_set_zoom(struct CameraControl* control, int32_t zoom);

    // Takes a picture and reports back image data via
    // callback. Please note that this stops the preview and thus, the
    // preview needs to be restarted after the picture operation has
    // completed. Ideally, this is done from the raw data callback.
    void android_camera_take_snapshot(struct CameraControl* control);

    // Enable or disable the preview callback for clients that want software frames
    int android_camera_set_preview_callback_mode(struct CameraControl* control, PreviewCallbackMode mode);

#ifdef __cplusplus
}
#endif

#endif // CAMERA_COMPATIBILITY_LAYER_H_
