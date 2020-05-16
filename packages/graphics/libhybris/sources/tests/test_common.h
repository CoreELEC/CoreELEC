/*
 * Copyright (C) 2020 Matti Lehtimäki <matti.lehtimaki@gmail.com>
 * Copyright (C) 2018 TheKit <nekit1000@gmail.com>
 * Copyright (c) 2012 Carsten Munk <carsten.munk@gmail.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */
#ifndef __LIBHYBRIS_TEST_COMMON_H
#define __LIBHYBRIS_TEST_COMMON_H

#include <EGL/egl.h>

#ifdef GL_ES_VERSION_3_0
#include <GLES3/gl3.h>
#include <GLES3/gl3ext.h>
#else
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#endif

#if USE_HWCOMPOSER

#include <hwcomposer_window.h>
#if HAS_HWCOMPOSER2_HEADERS

#include <hybris/hwc2/hwc2_compatibility_layer.h>
#endif

#ifdef __cplusplus

class HWComposer : public HWComposerNativeWindow
{
public:
	HWComposer(unsigned int width, unsigned int height, unsigned int format)
		: HWComposerNativeWindow(width, height, format)
	{
	}

protected:
	virtual void present(HWComposerNativeWindowBuffer *buffer) = 0;
};

#if HAS_HWCOMPOSER2_HEADERS

// Hwcomposer 2
class HWComposer2 : public HWComposer
{
private:
	hwc2_compat_layer_t *layer;
	hwc2_compat_display_t *hwcDisplay;
	int lastPresentFence = -1;
protected:
	void present(HWComposerNativeWindowBuffer *buffer);

public:
	HWComposer2(unsigned int width, unsigned int height, unsigned int format,
	            hwc2_compat_display_t *display, hwc2_compat_layer_t *layer);
};

#endif

#include <hwcomposer_window.h>
#include <hardware/hardware.h>
#include <hardware/hwcomposer.h>

// Hwcomposer 1
class HWComposer1 : public HWComposer
{
private:
	hwc_layer_1_t *fblayer;
	hwc_composer_device_1_t *hwcdevice;
	hwc_display_contents_1_t **mlist;
protected:
	void present(HWComposerNativeWindowBuffer *buffer);

public:
	HWComposer1(unsigned int width, unsigned int height, unsigned int format,
	            hwc_composer_device_1_t *device, hwc_display_contents_1_t **mList, hwc_layer_1_t *layer);
};

HWComposer *create_hwcomposer_window();

#endif

#endif // USE_HWCOMPOSER

GLuint create_program(const char* pVertexSource, const char* pFragmentSource);

#endif
