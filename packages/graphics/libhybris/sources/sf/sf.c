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
 * Authored by: Michael Frey <michael.frey@canonical.com>
 *              Ricardo Salveti de Araujo <ricardo.salveti@canonical.com>
 */

#include <EGL/egl.h>
#include <dlfcn.h>
#include <stddef.h>

#include <hybris/common/binding.h>
#include <hybris/surface_flinger/surface_flinger_compatibility_layer.h>

#define COMPAT_LIBRARY_PATH "libsf_compat_layer.so"

HYBRIS_LIBRARY_INITIALIZE(sf, COMPAT_LIBRARY_PATH);

HYBRIS_IMPLEMENT_VOID_FUNCTION1(sf, sf_blank, size_t);
HYBRIS_IMPLEMENT_VOID_FUNCTION1(sf, sf_unblank, size_t);
HYBRIS_IMPLEMENT_FUNCTION1(sf, size_t, sf_get_display_width, size_t);
HYBRIS_IMPLEMENT_FUNCTION1(sf, size_t, sf_get_display_height, size_t);
HYBRIS_IMPLEMENT_FUNCTION2(sf, size_t, sf_get_display_info, size_t, struct SfDisplayInfo*);
HYBRIS_IMPLEMENT_FUNCTION1(sf, struct SfClient*, sf_client_create_full, int);
HYBRIS_IMPLEMENT_FUNCTION0(sf, struct SfClient*, sf_client_create);
HYBRIS_IMPLEMENT_FUNCTION1(sf, EGLDisplay, sf_client_get_egl_display,
	struct SfClient*);
HYBRIS_IMPLEMENT_FUNCTION1(sf, EGLConfig, sf_client_get_egl_config,
	struct SfClient*);
HYBRIS_IMPLEMENT_FUNCTION1(sf, EGLContext, sf_client_get_egl_context,
	struct SfClient*);
HYBRIS_IMPLEMENT_VOID_FUNCTION1(sf, sf_client_begin_transaction,
	struct SfClient*);
HYBRIS_IMPLEMENT_VOID_FUNCTION1(sf, sf_client_end_transaction,
	struct SfClient*);
HYBRIS_IMPLEMENT_FUNCTION2(sf, struct SfSurface*, sf_surface_create,
	struct SfClient*, SfSurfaceCreationParameters*);
HYBRIS_IMPLEMENT_FUNCTION1(sf, EGLSurface, sf_surface_get_egl_surface,
	struct SfSurface*);
HYBRIS_IMPLEMENT_FUNCTION1(sf, EGLNativeWindowType,
	sf_surface_get_egl_native_window, struct SfSurface*);
HYBRIS_IMPLEMENT_VOID_FUNCTION1(sf, sf_surface_make_current, struct SfSurface*);
HYBRIS_IMPLEMENT_VOID_FUNCTION3(sf, sf_surface_move_to,
	struct SfSurface*, int, int);
HYBRIS_IMPLEMENT_VOID_FUNCTION3(sf, sf_surface_set_size,
	struct SfSurface*, int, int);
HYBRIS_IMPLEMENT_VOID_FUNCTION2(sf, sf_surface_set_layer,
	struct SfSurface*, int);
HYBRIS_IMPLEMENT_VOID_FUNCTION2(sf, sf_surface_set_alpha,
	struct SfSurface*, float);
