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

#include "test_common.h"

#include <stdio.h>
#include <stdlib.h>

#include <cutils/log.h>
#include <sync/sync.h>

#include "test_common.h"

#if USE_HWCOMPOSER

#include "logging.h"

inline static uint32_t interpreted_version(hw_device_t *hwc_device)
{
	uint32_t version = hwc_device->version;

	if ((version & 0xffff0000) == 0) {
		// Assume header version is always 1
		uint32_t header_version = 1;

		// Legacy version encoding
		version = (version << 16) | header_version;
	}
	return version;
}

#if HAS_HWCOMPOSER2_HEADERS

hwc2_compat_device_t* hwcDevice;

HWComposer2::HWComposer2(unsigned int width, unsigned int height,
    unsigned int format, hwc2_compat_display_t* display,
    hwc2_compat_layer_t *layer)
        : HWComposer(width, height, format)
{
	this->layer = layer;
	this->hwcDisplay = display;
}

void HWComposer2::present(HWComposerNativeWindowBuffer *buffer)
{
	uint32_t numTypes = 0;
	uint32_t numRequests = 0;

	hwc2_error_t error = hwc2_compat_display_validate(hwcDisplay, &numTypes,
                                                      &numRequests);

	if (error != HWC2_ERROR_NONE && error != HWC2_ERROR_HAS_CHANGES) {
		HYBRIS_ERROR("prepare: validate failed for display %lu: %s (%d)", displayId,
			to_string(static_cast<HWC2::Error>(error)).c_str(), error);
		return;
	}

	if (numTypes || numRequests) {
		HYBRIS_ERROR("prepare: validate required changes for display %lu: %s (%d)",
			displayId, to_string(static_cast<HWC2::Error>(error)).c_str(),
			error);
		return;
	}

	error = hwc2_compat_display_accept_changes(hwcDisplay);
	if (error != HWC2_ERROR_NONE) {
		HYBRIS_ERROR("prepare: acceptChanges failed: %s",
			to_string(static_cast<HWC2::Error>(error)).c_str());
		return;
	}

	hwc2_compat_display_set_client_target(hwcDisplay, /* slot */0, buffer,
	                                      getFenceBufferFd(buffer),
	                                      HAL_DATASPACE_UNKNOWN);

	int presentFence;
	hwc2_compat_display_present(hwcDisplay, &presentFence);

	if (error != HWC2_ERROR_NONE) {
		HYBRIS_ERROR("presentAndGetReleaseFences: failed for display %lu: %s (%d)",
			displayId,
			to_string(static_cast<HWC2::Error>(error)).c_str(), error);
		return;
	}

	hwc2_compat_out_fences_t* fences;
	error = hwc2_compat_display_get_release_fences(
		hwcDisplay, &fences);

	if (error != HWC2_ERROR_NONE) {
		HYBRIS_ERROR("presentAndGetReleaseFences: Failed to get release fences "
			"for display %lu: %s (%d)",
			displayId, to_string(static_cast<HWC2::Error>(error)).c_str(),
			error);
		return;
	}

	int fenceFd = hwc2_compat_out_fences_get_fence(fences, layer);
	if (fenceFd != -1)
		setFenceBufferFd(buffer, fenceFd);

	hwc2_compat_out_fences_destroy(fences);

	if (lastPresentFence != -1) {
		sync_wait(lastPresentFence, -1);
		close(lastPresentFence);
	}
	lastPresentFence = presentFence;
}

void onVsyncReceived(HWC2EventListener* listener, int32_t sequenceId,
                     hwc2_display_t display, int64_t timestamp)
{
}

void onHotplugReceived(HWC2EventListener* listener, int32_t sequenceId,
                       hwc2_display_t display, bool connected,
                       bool primaryDisplay)
{
	HYBRIS_INFO("onHotplugReceived(%d, %" PRIu64 ", %s, %s)",
		sequenceId, display,
		connected ? "connected" : "disconnected",
		primaryDisplay ? "primary" : "external");

	hwc2_compat_device_on_hotplug(hwcDevice, display, connected);
}

void onRefreshReceived(HWC2EventListener* listener,
                       int32_t sequenceId, hwc2_display_t display)
{
}

HWC2EventListener eventListener = {
    &onVsyncReceived,
    &onHotplugReceived,
    &onRefreshReceived
};

HWComposer *create_hwcomposer2_window()
{
	int composerSequenceId = 0;

	hwcDevice = hwc2_compat_device_new(false);
	assert(hwcDevice);

	hwc2_compat_device_register_callback(hwcDevice, &eventListener,
	                                     composerSequenceId);

	hwc2_compat_display_t* hwcDisplay;
	for (int i = 0; i < 5 * 1000; ++i) {
		/* Wait at most 5s for hotplug events */
		if ((hwcDisplay = hwc2_compat_device_get_display_by_id(hwcDevice, 0)))
			break;
		usleep(1000);
	}
	assert(hwcDisplay);

	hwc2_compat_display_set_power_mode(hwcDisplay, HWC2_POWER_MODE_ON);

	HWC2DisplayConfig* config = hwc2_compat_display_get_active_config(hwcDisplay);

	printf("width: %i height: %i\n", config->width, config->height);

	hwc2_compat_layer_t* layer = hwc2_compat_display_create_layer(hwcDisplay);

	hwc2_compat_layer_set_composition_type(layer, HWC2_COMPOSITION_CLIENT);
	hwc2_compat_layer_set_blend_mode(layer, HWC2_BLEND_MODE_NONE);
	hwc2_compat_layer_set_source_crop(layer, 0.0f, 0.0f, config->width,
	                                  config->height);
	hwc2_compat_layer_set_display_frame(layer, 0, 0, config->width,
	                                    config->height);
	hwc2_compat_layer_set_visible_region(layer, 0, 0, config->width,
	                                     config->height);

	HWComposer2 *win = new HWComposer2(config->width, config->height,
	                                   HAL_PIXEL_FORMAT_RGBA_8888, hwcDisplay,
	                                   layer);
	return win;
}
#endif

// Hwcomposer 1
HWComposer1::HWComposer1(unsigned int width, unsigned int height,
    unsigned int format, hwc_composer_device_1_t *device,
    hwc_display_contents_1_t **mList, hwc_layer_1_t *layer)
        : HWComposer(width, height, format)
{
	fblayer = layer;
	hwcdevice = device;
	mlist = mList;
}

void HWComposer1::present(HWComposerNativeWindowBuffer *buffer)
{
	int oldretire = mlist[0]->retireFenceFd;
	mlist[0]->retireFenceFd = -1;
	fblayer->handle = buffer->handle;
	fblayer->acquireFenceFd = getFenceBufferFd(buffer);
	fblayer->releaseFenceFd = -1;
	int err = hwcdevice->prepare(hwcdevice, HWC_NUM_DISPLAY_TYPES, mlist);
	assert(err == 0);

	err = hwcdevice->set(hwcdevice, HWC_NUM_DISPLAY_TYPES, mlist);
	// in android surfaceflinger ignores the return value as not all display types may be supported
	setFenceBufferFd(buffer, fblayer->releaseFenceFd);

	if (oldretire != -1) {
		sync_wait(oldretire, -1);
		close(oldretire);
	}
}

HWComposer *create_hwcomposer1_window()
{
	int err;
	hw_module_t const* module = NULL;
	err = hw_get_module(GRALLOC_HARDWARE_MODULE_ID, &module);
	assert(err == 0);
	framebuffer_device_t* fbDev = NULL;
	framebuffer_open(module, &fbDev);

	hw_module_t *hwcModule = 0;
	hwc_composer_device_1_t *hwcDevicePtr = 0;

	err = hw_get_module(HWC_HARDWARE_MODULE_ID, (const hw_module_t **) &hwcModule);
	assert(err == 0);

	err = hwc_open_1(hwcModule, &hwcDevicePtr);
	assert(err == 0);

	hw_device_t *hwcDevice = &hwcDevicePtr->common;

	uint32_t hwc_version = interpreted_version(hwcDevice);

#ifdef HWC_DEVICE_API_VERSION_1_4
	if (hwc_version == HWC_DEVICE_API_VERSION_1_4) {
		hwcDevicePtr->setPowerMode(hwcDevicePtr, 0, HWC_POWER_MODE_NORMAL);
	} else
#endif
#ifdef HWC_DEVICE_API_VERSION_1_5
	if (hwc_version == HWC_DEVICE_API_VERSION_1_5) {
		hwcDevicePtr->setPowerMode(hwcDevicePtr, 0, HWC_POWER_MODE_NORMAL);
	} else
#endif
		hwcDevicePtr->blank(hwcDevicePtr, 0, 0);

	uint32_t configs[5];
	size_t numConfigs = 5;

	err = hwcDevicePtr->getDisplayConfigs(hwcDevicePtr, 0, configs, &numConfigs);
	assert (err == 0);

	int32_t attr_values[2];
	uint32_t attributes[] = { HWC_DISPLAY_WIDTH, HWC_DISPLAY_HEIGHT, HWC_DISPLAY_NO_ATTRIBUTE };

	hwcDevicePtr->getDisplayAttributes(hwcDevicePtr, 0,
		configs[0], attributes, attr_values);

	printf("width: %i height: %i\n", attr_values[0], attr_values[1]);

	size_t size = sizeof(hwc_display_contents_1_t) + 2 * sizeof(hwc_layer_1_t);
	hwc_display_contents_1_t *list = (hwc_display_contents_1_t *) malloc(size);
	hwc_display_contents_1_t **mList = (hwc_display_contents_1_t **) malloc(HWC_NUM_DISPLAY_TYPES * sizeof(hwc_display_contents_1_t *));
	const hwc_rect_t r = { 0, 0, attr_values[0], attr_values[1] };

	int counter = 0;
	for (; counter < HWC_NUM_DISPLAY_TYPES; counter++)
		mList[counter] = NULL;

	// Assign the layer list only to the first display,
	// otherwise HWC might freeze if others are disconnected
	mList[0] = list;

	hwc_layer_1_t *layer = &list->hwLayers[0];
	memset(layer, 0, sizeof(hwc_layer_1_t));
	layer->compositionType = HWC_FRAMEBUFFER;
	layer->hints = 0;
	layer->flags = 0;
	layer->handle = 0;
	layer->transform = 0;
	layer->blending = HWC_BLENDING_NONE;
#ifdef HWC_DEVICE_API_VERSION_1_3
	layer->sourceCropf.top = 0.0f;
	layer->sourceCropf.left = 0.0f;
	layer->sourceCropf.bottom = (float) attr_values[1];
	layer->sourceCropf.right = (float) attr_values[0];
#else
	layer->sourceCrop = r;
#endif
	layer->displayFrame = r;
	layer->visibleRegionScreen.numRects = 1;
	layer->visibleRegionScreen.rects = &layer->displayFrame;
	layer->acquireFenceFd = -1;
	layer->releaseFenceFd = -1;
#if (ANDROID_VERSION_MAJOR >= 4) && (ANDROID_VERSION_MINOR >= 3) || (ANDROID_VERSION_MAJOR >= 5)
	// We've observed that qualcomm chipsets enters into compositionType == 6
	// (HWC_BLIT), an undocumented composition type which gives us rendering
	// glitches and warnings in logcat. By setting the planarAlpha to non-
	// opaque, we attempt to force the HWC into using HWC_FRAMEBUFFER for this
	// layer so the HWC_FRAMEBUFFER_TARGET layer actually gets used.
	bool tryToForceGLES = getenv("QPA_HWC_FORCE_GLES") != NULL;
	layer->planeAlpha = tryToForceGLES ? 1 : 255;
#endif
#ifdef HWC_DEVICE_API_VERSION_1_5
	layer->surfaceDamage.numRects = 0;
#endif

	layer = &list->hwLayers[1];
	memset(layer, 0, sizeof(hwc_layer_1_t));
	layer->compositionType = HWC_FRAMEBUFFER_TARGET;
	layer->hints = 0;
	layer->flags = 0;
	layer->handle = 0;
	layer->transform = 0;
	layer->blending = HWC_BLENDING_NONE;
#ifdef HWC_DEVICE_API_VERSION_1_3
	layer->sourceCropf.top = 0.0f;
	layer->sourceCropf.left = 0.0f;
	layer->sourceCropf.bottom = (float) attr_values[1];
	layer->sourceCropf.right = (float) attr_values[0];
#else
	layer->sourceCrop = r;
#endif
	layer->displayFrame = r;
	layer->visibleRegionScreen.numRects = 1;
	layer->visibleRegionScreen.rects = &layer->displayFrame;
	layer->acquireFenceFd = -1;
	layer->releaseFenceFd = -1;
#if (ANDROID_VERSION_MAJOR >= 4) && (ANDROID_VERSION_MINOR >= 3) || (ANDROID_VERSION_MAJOR >= 5)
	layer->planeAlpha = 0xff;
#endif
#ifdef HWC_DEVICE_API_VERSION_1_5
	layer->surfaceDamage.numRects = 0;
#endif

	list->retireFenceFd = -1;
	list->flags = HWC_GEOMETRY_CHANGED;
	list->numHwLayers = 2;

	HWComposer1 *win = new HWComposer1(attr_values[0], attr_values[1], HAL_PIXEL_FORMAT_RGBA_8888, hwcDevicePtr, mList, &list->hwLayers[1]);

	return win;
}

HWComposer *create_hwcomposer_window()
{
#if HAS_HWCOMPOSER2_HEADERS
	int err;
	hw_module_t *hwcModule = 0;
	hwc_composer_device_1_t *hwcDevicePtr = 0;

	err = hw_get_module(HWC_HARDWARE_MODULE_ID, (const hw_module_t **) &hwcModule);
	assert(err == 0);

	err = hwc_open_1(hwcModule, &hwcDevicePtr);
	assert(err == 0);

	hw_device_t *hwcDevice = &hwcDevicePtr->common;

	uint32_t hwc_version = interpreted_version(hwcDevice);

	hwc_close_1(hwcDevicePtr);
	if (hwc_version == HWC_DEVICE_API_VERSION_2_0) {
		return create_hwcomposer2_window();
	} else
#endif
	{
		return create_hwcomposer1_window();
	}
}

#endif

GLuint load_shader(GLenum shaderType, const char* pSource)
{
	GLuint shader = glCreateShader(shaderType);

	if (shader) {
		glShaderSource(shader, 1, &pSource, NULL);
		glCompileShader(shader);
		GLint compiled = 0;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);

		if (!compiled) {
			GLint infoLen = 0;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
			if (infoLen) {
				char* buf = (char*) malloc(infoLen);
				if (buf) {
					glGetShaderInfoLog(shader, infoLen, NULL, buf);
					fprintf(stderr, "Could not compile shader %d:\n%s\n",
						shaderType, buf);
					free(buf);
				}
				glDeleteShader(shader);
				shader = 0;
			}
		}
	} else {
		printf("Error, during shader creation: %i\n", glGetError());
	}

	return shader;
}

GLuint create_program(const char* pVertexSource, const char* pFragmentSource)
{
	GLuint vertexShader = load_shader(GL_VERTEX_SHADER, pVertexSource);
	if (!vertexShader) {
		printf("vertex shader not compiled\n");
		return 0;
	}

	GLuint pixelShader = load_shader(GL_FRAGMENT_SHADER, pFragmentSource);
	if (!pixelShader) {
		printf("frag shader not compiled\n");
		return 0;
	}

	GLuint program = glCreateProgram();
	if (program) {
		glAttachShader(program, vertexShader);
		glAttachShader(program, pixelShader);
		glLinkProgram(program);
		GLint linkStatus = GL_FALSE;

		glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
		if (linkStatus != GL_TRUE) {
			GLint bufLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &bufLength);
			if (bufLength) {
				char* buf = (char*) malloc(bufLength);
				if (buf) {
					glGetProgramInfoLog(program, bufLength, NULL, buf);
					fprintf(stderr, "Could not link program:\n%s\n", buf);
					free(buf);
				}
			}
			glDeleteProgram(program);
			program = 0;
		}
	}

	return program;
}
