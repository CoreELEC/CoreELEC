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

#ifndef CAMERA_COMPATIBILITY_LAYER_CONFIGURATION_TRANSLATOR_H_
#define CAMERA_COMPATIBILITY_LAYER_CONFIGURATION_TRANSLATOR_H_

#include <hybris/camera/camera_compatibility_layer_capabilities.h>

#include <camera/CameraParameters.h>
#include <utils/KeyedVector.h>

#ifdef __cplusplus
extern "C" {
#endif

#pragma GCC diagnostic ignored "-Wreturn-type-c-linkage"

    static const char* effect_modes[] =
    {
        android::CameraParameters::EFFECT_NONE,
        android::CameraParameters::EFFECT_MONO,
        android::CameraParameters::EFFECT_NEGATIVE,
        android::CameraParameters::EFFECT_SOLARIZE,
        android::CameraParameters::EFFECT_SEPIA,
        android::CameraParameters::EFFECT_POSTERIZE,
        android::CameraParameters::EFFECT_WHITEBOARD,
        android::CameraParameters::EFFECT_BLACKBOARD,
        android::CameraParameters::EFFECT_AQUA
    };

    android::KeyedVector<android::String8, EffectMode> init_effect_modes_lut()
    {
        android::KeyedVector<android::String8, EffectMode> m;
        m.add(android::String8(android::CameraParameters::EFFECT_NONE), EFFECT_MODE_NONE);
        m.add(android::String8(android::CameraParameters::EFFECT_MONO), EFFECT_MODE_MONO);
        m.add(android::String8(android::CameraParameters::EFFECT_NEGATIVE), EFFECT_MODE_NEGATIVE);
        m.add(android::String8(android::CameraParameters::EFFECT_SOLARIZE), EFFECT_MODE_SOLARIZE);
        m.add(android::String8(android::CameraParameters::EFFECT_SEPIA), EFFECT_MODE_SEPIA);
        m.add(android::String8(android::CameraParameters::EFFECT_POSTERIZE), EFFECT_MODE_POSTERIZE);
        m.add(android::String8(android::CameraParameters::EFFECT_WHITEBOARD), EFFECT_MODE_WHITEBOARD);
        m.add(android::String8(android::CameraParameters::EFFECT_BLACKBOARD), EFFECT_MODE_BLACKBOARD);
        m.add(android::String8(android::CameraParameters::EFFECT_AQUA), EFFECT_MODE_AQUA);

        return m;
    }

    static android::KeyedVector<android::String8, EffectMode> effect_modes_lut = init_effect_modes_lut();

    static const char* white_balance_modes[] =
    {
        android::CameraParameters::WHITE_BALANCE_AUTO,
        android::CameraParameters::WHITE_BALANCE_DAYLIGHT,
        android::CameraParameters::WHITE_BALANCE_CLOUDY_DAYLIGHT,
        android::CameraParameters::WHITE_BALANCE_FLUORESCENT,
        android::CameraParameters::WHITE_BALANCE_INCANDESCENT
    };

    android::KeyedVector<android::String8, WhiteBalanceMode> init_white_balance_modes_lut()
    {
        android::KeyedVector<android::String8, WhiteBalanceMode> m;
        m.add(android::String8(android::CameraParameters::WHITE_BALANCE_AUTO), WHITE_BALANCE_MODE_AUTO);
        m.add(android::String8(android::CameraParameters::WHITE_BALANCE_DAYLIGHT), WHITE_BALANCE_MODE_DAYLIGHT);
        m.add(android::String8(android::CameraParameters::WHITE_BALANCE_CLOUDY_DAYLIGHT), WHITE_BALANCE_MODE_CLOUDY_DAYLIGHT);
        m.add(android::String8(android::CameraParameters::WHITE_BALANCE_FLUORESCENT), WHITE_BALANCE_MODE_FLUORESCENT);
        m.add(android::String8(android::CameraParameters::WHITE_BALANCE_INCANDESCENT), WHITE_BALANCE_MODE_INCANDESCENT);
        return m;
    }

    static android::KeyedVector<android::String8, WhiteBalanceMode> white_balance_modes_lut = init_white_balance_modes_lut();

    static const char* flash_modes[] =
    {
        android::CameraParameters::FLASH_MODE_OFF,
        android::CameraParameters::FLASH_MODE_AUTO,
        android::CameraParameters::FLASH_MODE_ON,
        android::CameraParameters::FLASH_MODE_TORCH
    };

    static android::KeyedVector<android::String8, FlashMode> init_flash_modes_lut()
    {
        android::KeyedVector<android::String8, FlashMode> m;
        m.add(android::String8(android::CameraParameters::FLASH_MODE_OFF), FLASH_MODE_OFF);
        m.add(android::String8(android::CameraParameters::FLASH_MODE_AUTO), FLASH_MODE_AUTO);
        m.add(android::String8(android::CameraParameters::FLASH_MODE_ON), FLASH_MODE_ON);
        m.add(android::String8(android::CameraParameters::FLASH_MODE_TORCH), FLASH_MODE_TORCH);
        m.add(android::String8(android::CameraParameters::FLASH_MODE_RED_EYE), FLASH_MODE_RED_EYE);

        return m;
    }

    static android::KeyedVector<android::String8, FlashMode> flash_modes_lut = init_flash_modes_lut();

    static const char* scene_modes[] =
    {
        android::CameraParameters::SCENE_MODE_AUTO,
        android::CameraParameters::SCENE_MODE_ACTION,
        android::CameraParameters::SCENE_MODE_NIGHT,
        android::CameraParameters::SCENE_MODE_PARTY,
        android::CameraParameters::SCENE_MODE_SUNSET,
        android::CameraParameters::SCENE_MODE_HDR
    };

    static android::DefaultKeyedVector<android::String8, SceneMode> init_scene_modes_lut()
    {
        android::DefaultKeyedVector<android::String8, SceneMode> m(SCENE_MODE_AUTO);
        m.add(android::String8(android::CameraParameters::SCENE_MODE_AUTO), SCENE_MODE_AUTO);
        m.add(android::String8(android::CameraParameters::SCENE_MODE_ACTION), SCENE_MODE_ACTION);
        m.add(android::String8(android::CameraParameters::SCENE_MODE_NIGHT), SCENE_MODE_NIGHT);
        m.add(android::String8(android::CameraParameters::SCENE_MODE_PARTY), SCENE_MODE_PARTY);
        m.add(android::String8(android::CameraParameters::SCENE_MODE_SUNSET), SCENE_MODE_SUNSET);
        m.add(android::String8(android::CameraParameters::SCENE_MODE_HDR), SCENE_MODE_HDR);

        return m;
    }

    static android::DefaultKeyedVector<android::String8, SceneMode> scene_modes_lut = init_scene_modes_lut();

    static const char* auto_focus_modes[] =
    {
        android::CameraParameters::FOCUS_MODE_FIXED,
        android::CameraParameters::FOCUS_MODE_CONTINUOUS_VIDEO,
        android::CameraParameters::FOCUS_MODE_AUTO,
        android::CameraParameters::FOCUS_MODE_MACRO,
        android::CameraParameters::FOCUS_MODE_CONTINUOUS_PICTURE,
        android::CameraParameters::FOCUS_MODE_INFINITY
    };

    static android::KeyedVector<android::String8, AutoFocusMode> init_auto_focus_modes_lut()
    {
        android::KeyedVector<android::String8, AutoFocusMode> m;
        m.add(android::String8(android::CameraParameters::FOCUS_MODE_FIXED), AUTO_FOCUS_MODE_OFF);
        m.add(android::String8(android::CameraParameters::FOCUS_MODE_CONTINUOUS_VIDEO), AUTO_FOCUS_MODE_CONTINUOUS_VIDEO);
        m.add(android::String8(android::CameraParameters::FOCUS_MODE_AUTO), AUTO_FOCUS_MODE_AUTO);
        m.add(android::String8(android::CameraParameters::FOCUS_MODE_MACRO), AUTO_FOCUS_MODE_MACRO);
        m.add(android::String8(android::CameraParameters::FOCUS_MODE_CONTINUOUS_PICTURE), AUTO_FOCUS_MODE_CONTINUOUS_PICTURE);
        m.add(android::String8(android::CameraParameters::FOCUS_MODE_INFINITY), AUTO_FOCUS_MODE_INFINITY);

        return m;
    }

    static android::KeyedVector<android::String8, AutoFocusMode> auto_focus_modes_lut = init_auto_focus_modes_lut();

    static const char* camera_pixel_formats[] =
    {
        android::CameraParameters::PIXEL_FORMAT_YUV422SP,
        android::CameraParameters::PIXEL_FORMAT_YUV420SP,
        android::CameraParameters::PIXEL_FORMAT_YUV422I,
        android::CameraParameters::PIXEL_FORMAT_YUV420P,
        android::CameraParameters::PIXEL_FORMAT_RGB565,
        android::CameraParameters::PIXEL_FORMAT_RGBA8888,
        android::CameraParameters::PIXEL_FORMAT_BAYER_RGGB
    };

    static android::KeyedVector<android::String8, CameraPixelFormat> init_pixel_formats_lut()
    {
        android::KeyedVector<android::String8, CameraPixelFormat> m;
        m.add(android::String8(android::CameraParameters::PIXEL_FORMAT_YUV422SP), CAMERA_PIXEL_FORMAT_YUV422SP);
        m.add(android::String8(android::CameraParameters::PIXEL_FORMAT_YUV420SP), CAMERA_PIXEL_FORMAT_YUV420SP);
        m.add(android::String8(android::CameraParameters::PIXEL_FORMAT_YUV422I), CAMERA_PIXEL_FORMAT_YUV422I);
        m.add(android::String8(android::CameraParameters::PIXEL_FORMAT_YUV420P), CAMERA_PIXEL_FORMAT_YUV420P);
        m.add(android::String8(android::CameraParameters::PIXEL_FORMAT_RGB565), CAMERA_PIXEL_FORMAT_RGB565);
        m.add(android::String8(android::CameraParameters::PIXEL_FORMAT_RGBA8888), CAMERA_PIXEL_FORMAT_RGBA8888);
        m.add(android::String8(android::CameraParameters::PIXEL_FORMAT_BAYER_RGGB), CAMERA_PIXEL_FORMAT_BAYER_RGGB);
        return m;
    }

    static android::KeyedVector<android::String8, CameraPixelFormat> pixel_formats_lut = init_pixel_formats_lut();

#ifdef __cplusplus
}
#endif

#endif
