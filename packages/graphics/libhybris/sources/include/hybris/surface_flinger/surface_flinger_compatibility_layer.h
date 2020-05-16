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

#ifndef SURFACE_FLINGER_COMPATIBILITY_LAYER_H_
#define SURFACE_FLINGER_COMPATIBILITY_LAYER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <EGL/egl.h>
#include <EGL/eglext.h>

	struct SfClient;
	struct SfSurface;

        /* see frameworks/native/include/ui/DisplayInfo.h */
        struct SfDisplayInfo {
            uint32_t w;
            uint32_t h;
            float xdpi;
            float ydpi;
            float fps;
            float density;
            uint8_t orientation;
        };

        /* Display orientations as defined in Surface.java and ISurfaceComposer.h. */
        enum {
            DISPLAY_ORIENTATION_0 = 0,
            DISPLAY_ORIENTATION_90 = 1,
            DISPLAY_ORIENTATION_180 = 2,
            DISPLAY_ORIENTATION_270 = 3
        };

	enum
	{
		SURFACE_FLINGER_DEFAULT_DISPLAY_ID = 0
	};

	void sf_blank(size_t display_id);
	void sf_unblank(size_t display_id);

	size_t sf_get_display_width(size_t display_id);
	size_t sf_get_display_height(size_t display_id);
	size_t sf_get_display_info(size_t display_id, struct SfDisplayInfo* info);

	// The egl_support parameter disables the use of EGL inside the
	// library. sf_client_create() enables the use of EGL by default. When
	// disabled, the functions sf_client_get_egl_display(),
	// sf_client_get_egl_config(), sf_surface_get_egl_surface(),
	// sf_surface_make_current() and the create_egl_window_surface feature are
	// not supported anymore.
	struct SfClient* sf_client_create_full(int egl_support);
	struct SfClient* sf_client_create();

	EGLDisplay sf_client_get_egl_display(struct SfClient* display);
	EGLConfig sf_client_get_egl_config(struct SfClient* client);
	EGLContext sf_client_get_egl_context(struct SfClient* client);
	void sf_client_begin_transaction(struct SfClient*);
	void sf_client_end_transaction(struct SfClient*);

	typedef struct
	{
		int x;
		int y;
		int w;
		int h;
		int format;
		int layer;
		float alpha;
		int create_egl_window_surface;
		const char* name;
	} SfSurfaceCreationParameters;

	struct SfSurface* sf_surface_create(struct SfClient* client,
			SfSurfaceCreationParameters* params);
	EGLSurface sf_surface_get_egl_surface(struct SfSurface*);
	EGLNativeWindowType sf_surface_get_egl_native_window(struct SfSurface*);
	void sf_surface_make_current(struct SfSurface* surface);

	void sf_surface_move_to(struct SfSurface* surface, int x, int y);
	void sf_surface_set_size(struct SfSurface* surface, int w, int h);
	void sf_surface_set_layer(struct SfSurface* surface, int layer);
	void sf_surface_set_alpha(struct SfSurface* surface, float alpha);

#ifdef __cplusplus
}
#endif

#endif // SURFACE_FLINGER_COMPATIBILITY_LAYER_H_
