/*
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

#include "config.h"

/* EGL function pointers */
#define EGL_EGLEXT_PROTOTYPES
#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <dlfcn.h>
#include <stddef.h>
#include <stdlib.h>
#include <malloc.h>
#include "ws.h"
#include "helper.h"
#include <assert.h>


#include <hybris/common/binding.h>
#include <string.h>

#include <system/window.h>
#include "logging.h"

static void *egl_handle = NULL;
static void *glesv2_handle = NULL;
static void *_hybris_libgles1 = NULL;
static void *_hybris_libgles2 = NULL;
static int _egl_context_client_version = 1;

static EGLDisplay  (*_eglGetDisplay)(EGLNativeDisplayType display_id) = NULL;
static EGLBoolean  (*_eglTerminate)(EGLDisplay dpy) = NULL;

static const char *  (*_eglQueryString)(EGLDisplay dpy, EGLint name) = NULL;

static EGLSurface  (*_eglCreateWindowSurface)(EGLDisplay dpy, EGLConfig config,
		EGLNativeWindowType win,
		const EGLint *attrib_list) = NULL;
static EGLBoolean  (*_eglDestroySurface)(EGLDisplay dpy, EGLSurface surface) = NULL;

static EGLBoolean  (*_eglSwapInterval)(EGLDisplay dpy, EGLint interval) = NULL;


static EGLContext  (*_eglCreateContext)(EGLDisplay dpy, EGLConfig config,
		EGLContext share_context,
		const EGLint *attrib_list) = NULL;

static EGLSurface  (*_eglGetCurrentSurface)(EGLint readdraw) = NULL;

static EGLBoolean  (*_eglSwapBuffers)(EGLDisplay dpy, EGLSurface surface) = NULL;


static EGLImageKHR (*_eglCreateImageKHR)(EGLDisplay dpy, EGLContext ctx, EGLenum target, EGLClientBuffer buffer, const EGLint *attrib_list) = NULL;
static EGLBoolean (*_eglDestroyImageKHR) (EGLDisplay dpy, EGLImageKHR image) = NULL;

static void (*_glEGLImageTargetTexture2DOES) (GLenum target, GLeglImageOES image) = NULL;

static __eglMustCastToProperFunctionPointerType (*_eglGetProcAddress)(const char *procname) = NULL;

static void _init_androidegl()
{
	egl_handle = (void *) android_dlopen(getenv("LIBEGL") ? getenv("LIBEGL") : "libEGL.so", RTLD_LAZY);
	glesv2_handle = (void *) android_dlopen(getenv("LIBGLESV2") ? getenv("LIBGLESV2") : "libGLESv2.so", RTLD_LAZY);
}

static inline void hybris_egl_initialize()
{
	_init_androidegl();
}

static inline void hybris_glesv2_initialize()
{
	_init_androidegl();
}

static void * _android_egl_dlsym(const char *symbol)
{
	if (egl_handle == NULL)
		_init_androidegl();

	return android_dlsym(egl_handle, symbol);
}

struct ws_egl_interface hybris_egl_interface = {
	_android_egl_dlsym,
	egl_helper_has_mapping,
	egl_helper_get_mapping,
};

HYBRIS_IMPLEMENT_FUNCTION0(egl, EGLint, eglGetError);

#define _EGL_MAX_DISPLAYS 100

struct _EGLDisplay *_displayMappings[_EGL_MAX_DISPLAYS];

void _addMapping(struct _EGLDisplay *display_id)
{
	int i;
	for (i = 0; i < _EGL_MAX_DISPLAYS; i++)
	{
		if (_displayMappings[i] == NULL)
		{
			_displayMappings[i] = display_id;
			return;
		}
	}
}

struct _EGLDisplay *hybris_egl_display_get_mapping(EGLDisplay display)
{
	int i;
	for (i = 0; i < _EGL_MAX_DISPLAYS; i++)
	{
		if (_displayMappings[i])
		{
			if (_displayMappings[i]->dpy == display)
			{
				return _displayMappings[i];
			}

		}
	}
	return EGL_NO_DISPLAY;
}

EGLDisplay eglGetDisplay(EGLNativeDisplayType display_id)
{
	HYBRIS_DLSYSM(egl, &_eglGetDisplay, "eglGetDisplay");
	EGLNativeDisplayType real_display;

	real_display = (*_eglGetDisplay)(EGL_DEFAULT_DISPLAY);
	if (real_display == EGL_NO_DISPLAY)
	{
		return EGL_NO_DISPLAY;
	}

	struct _EGLDisplay *dpy = hybris_egl_display_get_mapping(real_display);
	if (!dpy) {
		dpy = ws_GetDisplay(display_id);
		if (!dpy) {
			return EGL_NO_DISPLAY;
		}
		dpy->dpy = real_display;
		_addMapping(dpy);
	}

	return real_display;
}

HYBRIS_IMPLEMENT_FUNCTION3(egl, EGLBoolean, eglInitialize, EGLDisplay, EGLint *, EGLint *);

EGLBoolean eglTerminate(EGLDisplay dpy)
{
	HYBRIS_DLSYSM(egl, &_eglTerminate, "eglTerminate");

	struct _EGLDisplay *display = hybris_egl_display_get_mapping(dpy);
	ws_Terminate(display);
	return (*_eglTerminate)(dpy);
}

const char * eglQueryString(EGLDisplay dpy, EGLint name)
{
	HYBRIS_DLSYSM(egl, &_eglQueryString, "eglQueryString");
	return ws_eglQueryString(dpy, name, _eglQueryString);
}

HYBRIS_IMPLEMENT_FUNCTION4(egl, EGLBoolean, eglGetConfigs, EGLDisplay, EGLConfig *, EGLint, EGLint *);
HYBRIS_IMPLEMENT_FUNCTION5(egl, EGLBoolean, eglChooseConfig, EGLDisplay, const EGLint *, EGLConfig *, EGLint, EGLint *);
HYBRIS_IMPLEMENT_FUNCTION4(egl, EGLBoolean, eglGetConfigAttrib, EGLDisplay, EGLConfig, EGLint, EGLint *);

EGLSurface eglCreateWindowSurface(EGLDisplay dpy, EGLConfig config,
		EGLNativeWindowType win,
		const EGLint *attrib_list)
{
	HYBRIS_DLSYSM(egl, &_eglCreateWindowSurface, "eglCreateWindowSurface");

	HYBRIS_TRACE_BEGIN("hybris-egl", "eglCreateWindowSurface", "");
	struct _EGLDisplay *display = hybris_egl_display_get_mapping(dpy);
	win = ws_CreateWindow(win, display);

	assert(((struct ANativeWindowBuffer *) win)->common.magic == ANDROID_NATIVE_WINDOW_MAGIC);

	HYBRIS_TRACE_BEGIN("native-egl", "eglCreateWindowSurface", "");
	EGLSurface result = (*_eglCreateWindowSurface)(dpy, config, win, attrib_list);

	HYBRIS_TRACE_END("native-egl", "eglCreateWindowSurface", "");

	if (result != EGL_NO_SURFACE)
		egl_helper_push_mapping(result, win);

	HYBRIS_TRACE_END("hybris-egl", "eglCreateWindowSurface", "");
	return result;
}

HYBRIS_IMPLEMENT_FUNCTION3(egl, EGLSurface, eglCreatePbufferSurface, EGLDisplay, EGLConfig, const EGLint *);
HYBRIS_IMPLEMENT_FUNCTION4(egl, EGLSurface, eglCreatePixmapSurface, EGLDisplay, EGLConfig, EGLNativePixmapType, const EGLint *);

EGLBoolean eglDestroySurface(EGLDisplay dpy, EGLSurface surface)
{
	HYBRIS_DLSYSM(egl, &_eglDestroySurface, "eglDestroySurface");
	EGLBoolean result = (*_eglDestroySurface)(dpy, surface);

	/**
         * If the surface was created via eglCreateWindowSurface, we must
         * notify the ws about surface destruction for clean-up.
	 **/
	if (egl_helper_has_mapping(surface)) {
	    ws_DestroyWindow(egl_helper_pop_mapping(surface));
	}

	return result;
}

HYBRIS_IMPLEMENT_FUNCTION4(egl, EGLBoolean, eglQuerySurface, EGLDisplay, EGLSurface, EGLint, EGLint *);
HYBRIS_IMPLEMENT_FUNCTION1(egl, EGLBoolean, eglBindAPI, EGLenum);
HYBRIS_IMPLEMENT_FUNCTION0(egl, EGLenum, eglQueryAPI);
HYBRIS_IMPLEMENT_FUNCTION0(egl, EGLBoolean, eglWaitClient);
HYBRIS_IMPLEMENT_FUNCTION0(egl, EGLBoolean, eglReleaseThread);
HYBRIS_IMPLEMENT_FUNCTION5(egl, EGLSurface, eglCreatePbufferFromClientBuffer, EGLDisplay, EGLenum, EGLClientBuffer, EGLConfig, const EGLint *);
HYBRIS_IMPLEMENT_FUNCTION4(egl, EGLBoolean, eglSurfaceAttrib, EGLDisplay, EGLSurface, EGLint, EGLint);
HYBRIS_IMPLEMENT_FUNCTION3(egl, EGLBoolean, eglBindTexImage, EGLDisplay, EGLSurface, EGLint);
HYBRIS_IMPLEMENT_FUNCTION3(egl, EGLBoolean, eglReleaseTexImage, EGLDisplay, EGLSurface, EGLint);

EGLBoolean eglSwapInterval(EGLDisplay dpy, EGLint interval)
{
	EGLBoolean ret;
	EGLSurface surface;
	HYBRIS_TRACE_BEGIN("hybris-egl", "eglSwapInterval", "=%d", interval);

	/* Some egl implementations don't pass through the setSwapInterval
	 * call.  Since we may support various swap intervals internally, we'll
	 * call it anyway and then give the wrapped egl implementation a chance
	 * to chage it. */
	HYBRIS_DLSYSM(egl, &_eglGetCurrentSurface, "eglGetCurrentSurface");
	surface = (*_eglGetCurrentSurface)(EGL_DRAW);
	if (egl_helper_has_mapping(surface))
	    ws_setSwapInterval(dpy, egl_helper_get_mapping(surface), interval);

	HYBRIS_TRACE_BEGIN("native-egl", "eglSwapInterval", "=%d", interval);
	HYBRIS_DLSYSM(egl, &_eglSwapInterval, "eglSwapInterval");
	ret = (*_eglSwapInterval)(dpy, interval);
	HYBRIS_TRACE_END("native-egl", "eglSwapInterval", "");
	HYBRIS_TRACE_END("hybris-egl", "eglSwapInterval", "");
	return ret;
}

EGLContext eglCreateContext(EGLDisplay dpy, EGLConfig config,
		EGLContext share_context,
		const EGLint *attrib_list)
{
	HYBRIS_DLSYSM(egl, &_eglCreateContext, "eglCreateContext");

	const EGLint *p = attrib_list;
	while (p != NULL && *p != EGL_NONE) {
		if (*p == EGL_CONTEXT_CLIENT_VERSION) {
			_egl_context_client_version = p[1];
		}
		p += 2;
	}

	return (*_eglCreateContext)(dpy, config, share_context, attrib_list);
}

HYBRIS_IMPLEMENT_FUNCTION2(egl, EGLBoolean, eglDestroyContext, EGLDisplay, EGLContext);
HYBRIS_IMPLEMENT_FUNCTION4(egl, EGLBoolean, eglMakeCurrent, EGLDisplay, EGLSurface, EGLSurface, EGLContext);
HYBRIS_IMPLEMENT_FUNCTION0(egl, EGLContext, eglGetCurrentContext);
HYBRIS_IMPLEMENT_FUNCTION1(egl, EGLSurface, eglGetCurrentSurface, EGLint);
HYBRIS_IMPLEMENT_FUNCTION0(egl, EGLDisplay, eglGetCurrentDisplay);
HYBRIS_IMPLEMENT_FUNCTION4(egl, EGLBoolean, eglQueryContext, EGLDisplay, EGLContext, EGLint, EGLint *);
HYBRIS_IMPLEMENT_FUNCTION0(egl, EGLBoolean, eglWaitGL);
HYBRIS_IMPLEMENT_FUNCTION1(egl, EGLBoolean, eglWaitNative, EGLint);

EGLBoolean _my_eglSwapBuffersWithDamageEXT(EGLDisplay dpy, EGLSurface surface, EGLint *rects, EGLint n_rects)
{
	EGLNativeWindowType win;
	EGLBoolean ret;
	HYBRIS_TRACE_BEGIN("hybris-egl", "eglSwapBuffersWithDamageEXT", "");
	HYBRIS_DLSYSM(egl, &_eglSwapBuffers, "eglSwapBuffers");

	if (egl_helper_has_mapping(surface)) {
		HYBRIS_DEBUG("+++++");
		win = egl_helper_get_mapping(surface);
		ws_prepareSwap(dpy, win, rects, n_rects);
		HYBRIS_DEBUG("+++++ EGLDisplay: %d, EGLSurface: %d, ", sizeof(EGLDisplay), sizeof(EGLSurface));
		ret = (*_eglSwapBuffers)(dpy, surface);
		HYBRIS_DEBUG("+++++");
		ws_finishSwap(dpy, win);
		HYBRIS_DEBUG("+++++");
	} else {
		HYBRIS_DEBUG("+++++");
		ret = (*_eglSwapBuffers)(dpy, surface);
	}
	HYBRIS_TRACE_END("hybris-egl", "eglSwapBuffersWithDamageEXT", "");
	return ret;
}

EGLBoolean eglSwapBuffers(EGLDisplay dpy, EGLSurface surface)
{
	EGLBoolean ret;
	HYBRIS_TRACE_BEGIN("hybris-egl", "eglSwapBuffers", "");
	ret = _my_eglSwapBuffersWithDamageEXT(dpy, surface, NULL, 0);
	HYBRIS_TRACE_END("hybris-egl", "eglSwapBuffers", "");
	return ret;
}

HYBRIS_IMPLEMENT_FUNCTION3(egl, EGLBoolean, eglCopyBuffers, EGLDisplay, EGLSurface, EGLNativePixmapType);


static EGLImageKHR _my_eglCreateImageKHR(EGLDisplay dpy, EGLContext ctx, EGLenum target, EGLClientBuffer buffer, const EGLint *attrib_list)
{
	HYBRIS_DLSYSM(egl, &_eglCreateImageKHR, "eglCreateImageKHR");
	EGLContext newctx = ctx;
	EGLenum newtarget = target;
	EGLClientBuffer newbuffer = buffer;
	const EGLint *newattrib_list = attrib_list;

	ws_passthroughImageKHR(&newctx, &newtarget, &newbuffer, &newattrib_list);

	EGLImageKHR eik = (*_eglCreateImageKHR)(dpy, newctx, newtarget, newbuffer, newattrib_list);

	if (eik == EGL_NO_IMAGE_KHR) {
		return EGL_NO_IMAGE_KHR;
	}

	struct egl_image *image;
	image = malloc(sizeof *image);
	image->egl_image = eik;
	image->egl_buffer = buffer;
	image->target = target;

	return (EGLImageKHR)image;
}

static void _my_glEGLImageTargetTexture2DOES(GLenum target, GLeglImageOES image)
{
	HYBRIS_DLSYSM(glesv2, &_glEGLImageTargetTexture2DOES, "glEGLImageTargetTexture2DOES");
	struct egl_image *img = image;
	(*_glEGLImageTargetTexture2DOES)(target, img ? img->egl_image : NULL);
}

__eglMustCastToProperFunctionPointerType eglGetProcAddress(const char *procname)
{
	HYBRIS_DLSYSM(egl, &_eglGetProcAddress, "eglGetProcAddress");
	if (strcmp(procname, "eglCreateImageKHR") == 0)
	{
		return (__eglMustCastToProperFunctionPointerType) _my_eglCreateImageKHR;
	}
	else if (strcmp(procname, "eglDestroyImageKHR") == 0)
	{
		return (__eglMustCastToProperFunctionPointerType) eglDestroyImageKHR;
	}
	else if (strcmp(procname, "eglSwapBuffersWithDamageEXT") == 0)
	{
		return (__eglMustCastToProperFunctionPointerType) _my_eglSwapBuffersWithDamageEXT;
	}
	else if (strcmp(procname, "glEGLImageTargetTexture2DOES") == 0)
	{
		return (__eglMustCastToProperFunctionPointerType) _my_glEGLImageTargetTexture2DOES;
	}

	__eglMustCastToProperFunctionPointerType ret = NULL;

	switch (_egl_context_client_version) {
		case 1:  // OpenGL ES 1.x API
			if (_hybris_libgles1 == NULL) {
				_hybris_libgles1 = (void *) dlopen(getenv("HYBRIS_LIBGLESV1") ?: "libGLESv1_CM.so.1", RTLD_LAZY);
			}
			ret = _hybris_libgles1 ? dlsym(_hybris_libgles1, procname) : NULL;
			break;
		case 2:  // OpenGL ES 2.0 API
		case 3:  // OpenGL ES 3.x API, backwards compatible with OpenGL ES 2.0 so we implement in same library
			if (_hybris_libgles2 == NULL) {
				_hybris_libgles2 = (void *) dlopen(getenv("HYBRIS_LIBGLESV2") ?: "libGLESv2.so.2", RTLD_LAZY);
			}
			ret = _hybris_libgles2 ? dlsym(_hybris_libgles2, procname) : NULL;
			break;
		default:
			HYBRIS_WARN("Unknown EGL context client version: %d", _egl_context_client_version);
			break;
	}

	if (ret == NULL) {
		ret = ws_eglGetProcAddress(procname);
	}

	if (ret == NULL) {
		ret = (*_eglGetProcAddress)(procname);
	}

	return ret;
}

EGLBoolean eglDestroyImageKHR(EGLDisplay dpy, EGLImageKHR image)
{
	HYBRIS_DLSYSM(egl, &_eglDestroyImageKHR, "eglDestroyImageKHR");
	struct egl_image *img = image;
	EGLBoolean ret = (*_eglDestroyImageKHR)(dpy, img ? img->egl_image : NULL);
	if (ret == EGL_TRUE) {
		free(img);
		return EGL_TRUE;
	}
	return ret;
}

// vim:ts=4:sw=4:noexpandtab
