/*
 * Copyright (C) 2011 The Android Open Source Project
 * Copyright (C) 2013 Jolla Ltd.
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

#ifndef EGL_HYBRIS_native_buffer
#define EGL_HYBRIS_native_buffer 1

#define EGL_NATIVE_BUFFER_HYBRIS             0x3140
typedef EGLBoolean (EGLAPIENTRYP PFNEGLHYBRISCREATENATIVEBUFFERPROC)(EGLint width, EGLint height, EGLint usage, EGLint format, EGLint *stride, EGLClientBuffer *buffer);
typedef EGLBoolean (EGLAPIENTRYP PFNEGLHYBRISLOCKNATIVEBUFFERPROC)(EGLClientBuffer buffer, EGLint usage, EGLint l, EGLint t, EGLint w, EGLint h, void **vaddr);
typedef EGLBoolean (EGLAPIENTRYP PFNEGLHYBRISUNLOCKNATIVEBUFFERPROC)(EGLClientBuffer buffer);
typedef EGLBoolean (EGLAPIENTRYP PFNEGLHYBRISRELEASENATIVEBUFFERPROC)(EGLClientBuffer buffer);

typedef EGLBoolean (EGLAPIENTRYP PFNEGLHYBRISGETNATIVEBUFFERINFOPROC)(EGLClientBuffer buffer, int *num_ints, int *num_fds);
typedef EGLBoolean (EGLAPIENTRYP PFNEGLHYBRISSERIALIZENATIVEBUFFERPROC)(EGLClientBuffer buffer, int *ints, int *fds);

typedef EGLBoolean (EGLAPIENTRYP PFNEGLHYBRISCREATEREMOTEBUFFERPROC)(EGLint width, EGLint height, EGLint usage, EGLint format, EGLint stride,
                                                                     int num_ints, int *ints, int num_fds, int *fds, EGLClientBuffer *buffer);

enum {
    /* buffer is never read in software */
    HYBRIS_USAGE_SW_READ_NEVER         = 0x00000000,
    /* buffer is rarely read in software */
    HYBRIS_USAGE_SW_READ_RARELY        = 0x00000002,
    /* buffer is often read in software */
    HYBRIS_USAGE_SW_READ_OFTEN         = 0x00000003,
    /* mask for the software read values */
    HYBRIS_USAGE_SW_READ_MASK          = 0x0000000F,
    
    /* buffer is never written in software */
    HYBRIS_USAGE_SW_WRITE_NEVER        = 0x00000000,
    /* buffer is rarely written in software */
    HYBRIS_USAGE_SW_WRITE_RARELY       = 0x00000020,
    /* buffer is often written in software */
    HYBRIS_USAGE_SW_WRITE_OFTEN        = 0x00000030,
    /* mask for the software write values */
    HYBRIS_USAGE_SW_WRITE_MASK         = 0x000000F0,

    /* buffer will be used as an OpenGL ES texture */
    HYBRIS_USAGE_HW_TEXTURE            = 0x00000100,
    /* buffer will be used as an OpenGL ES render target */
    HYBRIS_USAGE_HW_RENDER             = 0x00000200,
    /* buffer will be used by the 2D hardware blitter */
    HYBRIS_USAGE_HW_2D                 = 0x00000400,
    /* buffer will be used by the HWComposer HAL module */
    HYBRIS_USAGE_HW_COMPOSER           = 0x00000800,
    /* buffer will be used with the framebuffer device */
    HYBRIS_USAGE_HW_FB                 = 0x00001000,
    /* buffer will be used with the HW video encoder */
    HYBRIS_USAGE_HW_VIDEO_ENCODER      = 0x00010000,
    /* buffer will be written by the HW camera pipeline */
    HYBRIS_USAGE_HW_CAMERA_WRITE       = 0x00020000,
    /* buffer will be read by the HW camera pipeline */
    HYBRIS_USAGE_HW_CAMERA_READ        = 0x00040000,
    /* buffer will be used as part of zero-shutter-lag queue */
    HYBRIS_USAGE_HW_CAMERA_ZSL         = 0x00060000,
    /* mask for the camera access values */
    HYBRIS_USAGE_HW_CAMERA_MASK        = 0x00060000,
    /* mask for the software usage bit-mask */
    HYBRIS_USAGE_HW_MASK               = 0x00071F00,

    /* buffer should be displayed full-screen on an external display when
     * possible
     */
    HYBRIS_USAGE_EXTERNAL_DISP         = 0x00002000,

    /* Must have a hardware-protected path to external display sink for
     * this buffer.  If a hardware-protected path is not available, then
     * either don't composite only this buffer (preferred) to the
     * external sink, or (less desirable) do not route the entire
     * composition to the external sink.
     */
    HYBRIS_USAGE_PROTECTED             = 0x00004000,
};

/**
 * pixel format definitions
 */

enum {
    HYBRIS_PIXEL_FORMAT_RGBA_8888          = 1,
    HYBRIS_PIXEL_FORMAT_RGBX_8888          = 2,
    HYBRIS_PIXEL_FORMAT_RGB_888            = 3,
    HYBRIS_PIXEL_FORMAT_RGB_565            = 4,
    HYBRIS_PIXEL_FORMAT_BGRA_8888          = 5,
    HYBRIS_PIXEL_FORMAT_RGBA_5551          = 6,
    HYBRIS_PIXEL_FORMAT_RGBA_4444          = 7,

    /* 0x8 - 0xFF range unavailable */

    /*
     * 0x100 - 0x1FF
     *
     * This range is reserved for pixel formats that are specific to the HAL
     * implementation.  Implementations can use any value in this range to
     * communicate video pixel formats between their HAL modules.  These formats
     * must not have an alpha channel.  Additionally, an EGLimage created from a
     * gralloc buffer of one of these formats must be supported for use with the
     * GL_OES_EGL_image_external OpenGL ES extension.
     */

    /*
     * Android YUV format:
     *
     * This format is exposed outside of the HAL to software decoders and
     * applications.  EGLImageKHR must support it in conjunction with the
     * OES_EGL_image_external extension.
     *
     * YV12 is a 4:2:0 YCrCb planar format comprised of a WxH Y plane followed
     * by (W/2) x (H/2) Cr and Cb planes.
     *
     * This format assumes
     * - an even width
     * - an even height
     * - a horizontal stride multiple of 16 pixels
     * - a vertical stride equal to the height
     *
     *   y_size = stride * height
     *   c_stride = ALIGN(stride/2, 16)
     *   c_size = c_stride * height/2
     *   size = y_size + c_size * 2
     *   cr_offset = y_size
     *   cb_offset = y_size + c_size
     *
     */
    HYBRIS_PIXEL_FORMAT_YV12   = 0x32315659, // YCrCb 4:2:0 Planar


    /*
     * Android Y8 format:
     *
     * This format is exposed outside of the HAL to the framework.
     * The expected gralloc usage flags are SW_* and HW_CAMERA_*,
     * and no other HW_ flags will be used.
     *
     * Y8 is a YUV planar format comprised of a WxH Y plane,
     * with each pixel being represented by 8 bits.
     *
     * It is equivalent to just the Y plane from YV12.
     *
     * This format assumes
     * - an even width
     * - an even height
     * - a horizontal stride multiple of 16 pixels
     * - a vertical stride equal to the height
     *
     *   size = stride * height
     *
     */
    HYBRIS_PIXEL_FORMAT_Y8     = 0x20203859,

    /*
     * Android Y16 format:
     *
     * This format is exposed outside of the HAL to the framework.
     * The expected gralloc usage flags are SW_* and HW_CAMERA_*,
     * and no other HW_ flags will be used.
     *
     * Y16 is a YUV planar format comprised of a WxH Y plane,
     * with each pixel being represented by 16 bits.
     *
     * It is just like Y8, but has double the bits per pixel (little endian).
     *
     * This format assumes
     * - an even width
     * - an even height
     * - a horizontal stride multiple of 16 pixels
     * - a vertical stride equal to the height
     * - strides are specified in pixels, not in bytes
     *
     *   size = stride * height * 2
     *
     */
    HYBRIS_PIXEL_FORMAT_Y16    = 0x20363159,

    /*
     * Android RAW sensor format:
     *
     * This format is exposed outside of the HAL to applications.
     *
     * RAW_SENSOR is a single-channel 16-bit format, typically representing raw
     * Bayer-pattern images from an image sensor, with minimal processing.
     *
     * The exact pixel layout of the data in the buffer is sensor-dependent, and
     * needs to be queried from the camera device.
     *
     * Generally, not all 16 bits are used; more common values are 10 or 12
     * bits. All parameters to interpret the raw data (black and white points,
     * color space, etc) must be queried from the camera device.
     *
     * This format assumes
     * - an even width
     * - an even height
     * - a horizontal stride multiple of 16 pixels (32 bytes).
     */
    HYBRIS_PIXEL_FORMAT_RAW_SENSOR = 0x20,

    /*
     * Android binary blob graphics buffer format:
     *
     * This format is used to carry task-specific data which does not have a
     * standard image structure. The details of the format are left to the two
     * endpoints.
     *
     * A typical use case is for transporting JPEG-compressed images from the
     * Camera HAL to the framework or to applications.
     *
     * Buffers of this format must have a height of 1, and width equal to their
     * size in bytes.
     */
    HYBRIS_PIXEL_FORMAT_BLOB = 0x21,

    /*
     * Android format indicating that the choice of format is entirely up to the
     * device-specific Gralloc implementation.
     *
     * The Gralloc implementation should examine the usage bits passed in when
     * allocating a buffer with this format, and it should derive the pixel
     * format from those usage flags.  This format will never be used with any
     * of the GRALLOC_USAGE_SW_* usage flags.
     *
     * If a buffer of this format is to be used as an OpenGL ES texture, the
     * framework will assume that sampling the texture will always return an
     * alpha value of 1.0 (i.e. the buffer contains only opaque pixel values).
     *
     */
    HYBRIS_PIXEL_FORMAT_IMPLEMENTATION_DEFINED = 0x22,

    /*
     * Android flexible YCbCr formats
     *
     * This format allows platforms to use an efficient YCbCr/YCrCb buffer
     * layout, while still describing the buffer layout in a way accessible to
     * the CPU in a device-independent manner.  While called YCbCr, it can be
     * used to describe formats with either chromatic ordering, as well as
     * whole planar or semiplanar layouts.
     *
     * struct android_ycbcr (below) is the the struct used to describe it.
     *
     * This format must be accepted by the gralloc module when
     * USAGE_HW_CAMERA_WRITE and USAGE_SW_READ_* are set.
     *
     * This format is locked for use by gralloc's (*lock_ycbcr) method, and
     * locking with the (*lock) method will return an error.
     */
    HYBRIS_PIXEL_FORMAT_YCbCr_420_888 = 0x23,

    /* Legacy formats (deprecated), used by ImageFormat.java */
    HYBRIS_PIXEL_FORMAT_YCbCr_422_SP       = 0x10, // NV16
    HYBRIS_PIXEL_FORMAT_YCrCb_420_SP       = 0x11, // NV21
    HYBRIS_PIXEL_FORMAT_YCbCr_422_I        = 0x14, // YUY2
};

#endif
