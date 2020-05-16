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

#ifndef SURFACE_FLINGER_COMPATIBILITY_LAYER_INTERNAL_H_
#define SURFACE_FLINGER_COMPATIBILITY_LAYER_INTERNAL_H_

#ifndef __ANDROID__
#error "Using this header is only safe when compiling for Android."
#endif

#include <utils/misc.h>

#include <gui/Surface.h>
#include <gui/SurfaceComposerClient.h>
#include <ui/PixelFormat.h>
#include <ui/Region.h>
#include <ui/Rect.h>

#include <cassert>
#include <cstdio>

struct SfClient
{
	android::sp<android::SurfaceComposerClient> client;
	EGLDisplay egl_display;
	EGLConfig egl_config;
	EGLContext egl_context;
	bool egl_support;
};

struct SfSurface
{
	SfSurface() : client(NULL)
	{
	}

	SfClient* client;
	android::sp<android::SurfaceControl> surface_control;
	android::sp<android::Surface> surface;
	EGLSurface egl_surface;
};

#endif // SURFACE_FLINGER_COMPATIBILITY_LAYER_INTERNAL_H_
