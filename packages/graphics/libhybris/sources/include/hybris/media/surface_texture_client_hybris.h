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

#ifndef SURFACE_TEXTURE_CLIENT_HYBRIS_H_
#define SURFACE_TEXTURE_CLIENT_HYBRIS_H_

#include <stdint.h>
#include <unistd.h>

#include <EGL/egl.h>

#ifdef __ARM_PCS_VFP
#define FP_ATTRIB __attribute__((pcs("aapcs")))
#else
#define FP_ATTRIB
#endif

#ifdef __cplusplus
extern "C" {
#endif

    // Taken from native_window.h
    enum {
        WINDOW_FORMAT_RGBA_8888     = 1,
        WINDOW_FORMAT_RGBX_8888     = 2,
        WINDOW_FORMAT_RGB_565       = 4,
    };

    typedef void* SurfaceTextureClientHybris;
    typedef void* GLConsumerHybris;
    typedef void* GLConsumerWrapperHybris;
    typedef void* IGraphicBufferConsumerHybris;
    typedef void* IGraphicBufferProducerHybris;
    typedef void* IGBCWrapperHybris;
    typedef void* IGBPWrapperHybris;

    /** Used to set a callback function to be called when a frame is ready to be rendered **/
    typedef void (*FrameAvailableCbHybris)(GLConsumerWrapperHybris wrapper, void *context);
    typedef void (*DecodingClientDeathCbHybris)(void *context);

    //SurfaceTextureClientHybris surface_texture_client_get_instance();
    SurfaceTextureClientHybris surface_texture_client_create(EGLNativeWindowType native_window);
    SurfaceTextureClientHybris surface_texture_client_create_by_id(unsigned int texture_id);
    SurfaceTextureClientHybris surface_texture_client_create_by_igbp(IGBPWrapperHybris wrapper);
    GLConsumerWrapperHybris gl_consumer_create_by_id_with_igbc(unsigned int texture_id, IGBCWrapperHybris wrapper);
    int gl_consumer_set_frame_available_cb(GLConsumerWrapperHybris wrapper, FrameAvailableCbHybris cb, void *context);
    void gl_consumer_get_transformation_matrix(GLConsumerWrapperHybris wrapper, float *matrix) FP_ATTRIB;
    void gl_consumer_update_texture(GLConsumerWrapperHybris wrapper);
    uint8_t surface_texture_client_is_ready_for_rendering(SurfaceTextureClientHybris stc);
    uint8_t surface_texture_client_hardware_rendering(SurfaceTextureClientHybris stc);
    void surface_texture_client_set_hardware_rendering(SurfaceTextureClientHybris stc, uint8_t hardware_rendering);
    void surface_texture_client_get_transformation_matrix(SurfaceTextureClientHybris stc, float *matrix) FP_ATTRIB;
    void surface_texture_client_update_texture(SurfaceTextureClientHybris stc);
    void surface_texture_client_destroy(SurfaceTextureClientHybris stc);
    void surface_texture_client_ref(SurfaceTextureClientHybris stc);
    void surface_texture_client_unref(SurfaceTextureClientHybris stc);
    void surface_texture_client_set_surface_texture(SurfaceTextureClientHybris stc, EGLNativeWindowType native_window);

#ifdef __cplusplus
}
#endif

#endif // SURFACE_TEXTURE_CLIENT_HYBRIS_H_
