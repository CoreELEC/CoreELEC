/****************************************************************************************
 **
 ** Copyright (C) 2013 Jolla Ltd.
 ** Contact: Carsten Munk <carsten.munk@jollamobile.com>
 ** All rights reserved.
 **
 ** This file is part of Wayland enablement for libhybris
 **
 ** You may use this file under the terms of the GNU Lesser General
 ** Public License version 2.1 as published by the Free Software Foundation
 ** and appearing in the file license.lgpl included in the packaging
 ** of this file.
 **
 ** This library is free software; you can redistribute it and/or
 ** modify it under the terms of the GNU Lesser General Public
 ** License version 2.1 as published by the Free Software Foundation
 ** and appearing in the file license.lgpl included in the packaging
 ** of this file.
 **
 ** This library is distributed in the hope that it will be useful,
 ** but WITHOUT ANY WARRANTY; without even the implied warranty of
 ** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 ** Lesser General Public License for more details.
 **
 ****************************************************************************************/

#include <android-config.h>
#include <ws.h>
#include <malloc.h>
#include <assert.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <assert.h>
#include <stdlib.h>
#include <dlfcn.h>
extern "C" {
#include <eglplatformcommon.h>
};
#include <eglhybris.h>

#include <EGL/eglext.h>

extern "C" {
#include <wayland-client.h>
#include <wayland-egl.h>
}

#include <hybris/gralloc/gralloc.h>
#include "wayland_window.h"
#include "logging.h"
#include "wayland-egl-priv.h"
#include "server_wlegl_buffer.h"
#include "wayland-android-client-protocol.h"

static const char *  (*_eglQueryString)(EGLDisplay dpy, EGLint name) = NULL;
static __eglMustCastToProperFunctionPointerType (*_eglGetProcAddress)(const char *procname) = NULL;
static EGLSyncKHR (*_eglCreateSyncKHR)(EGLDisplay dpy, EGLenum type, const EGLint *attrib_list) = NULL;
static EGLBoolean (*_eglDestroySyncKHR)(EGLDisplay dpy, EGLSyncKHR sync) = NULL;
static EGLint (*_eglClientWaitSyncKHR)(EGLDisplay dpy, EGLSyncKHR sync, EGLint flags, EGLTimeKHR timeout) = NULL;

/* The following function is implemented in libhybris's libEGL.so.
 * However, eglplatform_wayland.so is not linking to libEGL directly,
 * causing undefined symbol errors during loading, if libEGL was not
 * already loaded by some other dependencies. Therefore, we should try
 * to load libEGL at runtime here and resolve this function dynamically */
typedef struct _EGLDisplay *(*PFNHYBRISEGLDISPLAYGETMAPPINGPROC)(EGLDisplay dpy);

struct WaylandDisplay {
	_EGLDisplay base;

	wl_display *wl_dpy;
	wl_event_queue *queue;
	wl_registry *registry;
	android_wlegl *wlegl;
};

extern "C" void waylandws_init_module(struct ws_egl_interface *egl_iface)
{
	hybris_gralloc_initialize(0);
	eglplatformcommon_init(egl_iface);
}

static void _init_egl_funcs(EGLDisplay display)
{
	if (_eglQueryString != NULL)
		return;

	_eglQueryString = (const char * (*)(void*, int))
			hybris_android_egl_dlsym("eglQueryString");
	assert(_eglQueryString);
	_eglGetProcAddress = (__eglMustCastToProperFunctionPointerType (*)(const char *))
			hybris_android_egl_dlsym("eglGetProcAddress");
	assert(_eglGetProcAddress);

	const char *extensions = (*_eglQueryString)(display, EGL_EXTENSIONS);

	if (strstr(extensions, "EGL_KHR_fence_sync")) {
		_eglCreateSyncKHR = (PFNEGLCREATESYNCKHRPROC)
				(*_eglGetProcAddress)("eglCreateSyncKHR");
		assert(_eglCreateSyncKHR);
		_eglDestroySyncKHR = (PFNEGLDESTROYSYNCKHRPROC)
				(*_eglGetProcAddress)("eglDestroySyncKHR");
		assert(_eglDestroySyncKHR);
		_eglClientWaitSyncKHR = (PFNEGLCLIENTWAITSYNCKHRPROC)
				(*_eglGetProcAddress)("eglClientWaitSyncKHR");
		assert(_eglClientWaitSyncKHR);
	}
}

static void registry_handle_global(void *data, wl_registry *registry, uint32_t name, const char *interface, uint32_t version)
{
	WaylandDisplay *dpy = (WaylandDisplay *)data;

	if (strcmp(interface, "android_wlegl") == 0) {
		dpy->wlegl = static_cast<struct android_wlegl *>(wl_registry_bind(registry, name, &android_wlegl_interface, std::min(2u, version)));
	}
}

static const wl_registry_listener registry_listener = {
    registry_handle_global
};

static void callback_done(void *data, wl_callback *cb, uint32_t d)
{
    WaylandDisplay *dpy = (WaylandDisplay *)data;

    wl_callback_destroy(cb);
    if (!dpy->wlegl) {
        fprintf(stderr, "Fatal: the server doesn't advertise the android_wlegl global!");
        abort();
    }
}

static const wl_callback_listener callback_listener = {
    callback_done
};

extern "C" _EGLDisplay *waylandws_GetDisplay(EGLNativeDisplayType display)
{
	WaylandDisplay *wdpy = new WaylandDisplay;
	wdpy->wl_dpy = display ? (wl_display *)display : wl_display_connect(NULL);
	wdpy->wlegl = NULL;
	wdpy->queue = wl_display_create_queue(wdpy->wl_dpy);
	wdpy->registry = wl_display_get_registry(wdpy->wl_dpy);
	wl_proxy_set_queue((wl_proxy *) wdpy->registry, wdpy->queue);
	wl_registry_add_listener(wdpy->registry, &registry_listener, wdpy);

	wl_callback *cb = wl_display_sync(wdpy->wl_dpy);
	wl_proxy_set_queue((wl_proxy *) cb, wdpy->queue);
	wl_callback_add_listener(cb, &callback_listener, wdpy);

	return &wdpy->base;
}

extern "C" void waylandws_Terminate(_EGLDisplay *dpy)
{
	WaylandDisplay *wdpy = (WaylandDisplay *)dpy;
	int ret = 0;
	// We still have the sync callback on flight, wait for it to arrive
	while (ret == 0 && !wdpy->wlegl) {
		ret = wl_display_dispatch_queue(wdpy->wl_dpy, wdpy->queue);
	}
	assert(ret >= 0);
	android_wlegl_destroy(wdpy->wlegl);
	wl_registry_destroy(wdpy->registry);
	wl_event_queue_destroy(wdpy->queue);
	delete wdpy;
}

extern "C" EGLNativeWindowType waylandws_CreateWindow(EGLNativeWindowType win, _EGLDisplay *display)
{
	struct wl_egl_window *wl_window = (struct wl_egl_window*) win;
	struct wl_display *wl_display = (struct wl_display*) display;

	if (wl_window == 0 || wl_display == 0) {
		HYBRIS_ERROR("Running with EGL_PLATFORM=wayland without setup wayland environment is not possible");
		HYBRIS_ERROR("If you want to run a standlone EGL client do it like this:");
		HYBRIS_ERROR(" $ export EGL_PLATFORM=null");
		HYBRIS_ERROR(" $ test_glevs2");
		abort();
	}

	WaylandDisplay *wdpy = (WaylandDisplay *)display;

	int ret = 0;
	while (ret == 0 && !wdpy->wlegl) {
		ret = wl_display_dispatch_queue(wdpy->wl_dpy, wdpy->queue);
	}
	assert(ret >= 0);

	WaylandNativeWindow *window = new WaylandNativeWindow((struct wl_egl_window *) win, wdpy->wl_dpy, wdpy->wlegl);
	window->common.incRef(&window->common);
	return (EGLNativeWindowType) static_cast<struct ANativeWindow *>(window);
}

extern "C" void waylandws_DestroyWindow(EGLNativeWindowType win)
{
	WaylandNativeWindow *window = static_cast<WaylandNativeWindow *>((struct ANativeWindow *)win);
	window->common.decRef(&window->common);
}

extern "C" int waylandws_post(EGLNativeWindowType win, void *buffer)
{
	struct wl_egl_window *eglwin = (struct wl_egl_window *) win;

	return ((WaylandNativeWindow *) eglwin->driver_private)->postBuffer((ANativeWindowBuffer *) buffer);
}

/**
 * Loads libhybris's libEGL at runtime to call hybris_egl_display_get_mapping()
 */
static struct _EGLDisplay *_hybris_egl_display_get_mapping(EGLDisplay dpy)
{
	static void *libEGL_handle = NULL;
	static PFNHYBRISEGLDISPLAYGETMAPPINGPROC hybris_egl_display_get_mapping_fn = NULL;

	if (!libEGL_handle) {
		dlerror();  // cleanup error buffer
		libEGL_handle = dlopen("libEGL.so.1", RTLD_NOW | RTLD_GLOBAL);
		if (!libEGL_handle) {
			HYBRIS_ERROR("ERROR: Failed to dlopen libEGL! %s", dlerror());
			abort();
		}
	}

	if (!hybris_egl_display_get_mapping_fn) {
		dlerror();  // cleanup error buffer
		hybris_egl_display_get_mapping_fn = (PFNHYBRISEGLDISPLAYGETMAPPINGPROC)dlsym(
			libEGL_handle, "hybris_egl_display_get_mapping");
		if (!hybris_egl_display_get_mapping_fn) {
			HYBRIS_ERROR("ERROR: Cannot resolve 'hybris_egl_display_get_mapping' in libEGL! %s", dlerror());
			abort();
		}
	}
	
	return hybris_egl_display_get_mapping_fn(dpy);
}

extern "C" wl_buffer *waylandws_createWlBuffer(EGLDisplay dpy, EGLImageKHR image)
{
	egl_image *img = reinterpret_cast<egl_image *>(image);
	if (!img) {
	    // The spec says we should send a EGL_BAD_PARAMETER error here, but we don't have the
	    // means, as of now.
	    return NULL;
	}
	if (img->target == EGL_WAYLAND_BUFFER_WL) {
		WaylandDisplay *wdpy = (WaylandDisplay *)_hybris_egl_display_get_mapping(dpy);
		server_wlegl_buffer *buf = server_wlegl_buffer_from((wl_resource *)img->egl_buffer);
		WaylandNativeWindowBuffer wnb(buf->buf);
		// The buffer will be managed by the app, so pass NULL as the queue so that
		// it will be assigned to the default queue
		wnb.wlbuffer_from_native_handle(wdpy->wlegl, wdpy->wl_dpy, NULL);
		return wnb.wlbuffer;
	}
	// EGL_BAD_MATCH
	return NULL;
}

extern "C" __eglMustCastToProperFunctionPointerType waylandws_eglGetProcAddress(const char *procname)
{
	if (strcmp(procname, "eglHybrisWaylandPostBuffer") == 0)
	{
		return (__eglMustCastToProperFunctionPointerType) waylandws_post;
	}
	else if (strcmp(procname, "eglCreateWaylandBufferFromImageWL") == 0)
    {
        return (__eglMustCastToProperFunctionPointerType) waylandws_createWlBuffer;
    }
	else
	return eglplatformcommon_eglGetProcAddress(procname);
}

extern "C" void waylandws_passthroughImageKHR(EGLContext *ctx, EGLenum *target, EGLClientBuffer *buffer, const EGLint **attrib_list)
{
	eglplatformcommon_passthroughImageKHR(ctx, target, buffer, attrib_list);
}

extern "C" const char *waylandws_eglQueryString(EGLDisplay dpy, EGLint name, const char *(*real_eglQueryString)(EGLDisplay dpy, EGLint name))
{
	const char *ret = eglplatformcommon_eglQueryString(dpy, name, real_eglQueryString);
	if (ret && name == EGL_EXTENSIONS)
	{
		static char eglextensionsbuf[2048];
		snprintf(eglextensionsbuf, 2046, "%s %s", ret,
			"EGL_EXT_swap_buffers_with_damage EGL_WL_create_wayland_buffer_from_image"
		);
		ret = eglextensionsbuf;
	}
	return ret;
}

extern "C" void waylandws_prepareSwap(EGLDisplay dpy, EGLNativeWindowType win, EGLint *damage_rects, EGLint damage_n_rects)
{
	WaylandNativeWindow *window = static_cast<WaylandNativeWindow *>((struct ANativeWindow *)win);
	window->prepareSwap(damage_rects, damage_n_rects);
}

extern "C" void waylandws_finishSwap(EGLDisplay dpy, EGLNativeWindowType win)
{
	_init_egl_funcs(dpy);
	WaylandNativeWindow *window = static_cast<WaylandNativeWindow *>((struct ANativeWindow *)win);
	if (_eglCreateSyncKHR) {
		EGLSyncKHR sync = (*_eglCreateSyncKHR)(dpy, EGL_SYNC_FENCE_KHR, NULL);
		(*_eglClientWaitSyncKHR)(dpy, sync, EGL_SYNC_FLUSH_COMMANDS_BIT_KHR, EGL_FOREVER_KHR);
		(*_eglDestroySyncKHR)(dpy, sync);
	}
	window->finishSwap();
}

extern "C" void waylandws_setSwapInterval(EGLDisplay dpy, EGLNativeWindowType win, EGLint interval)
{
	WaylandNativeWindow *window = static_cast<WaylandNativeWindow *>((struct ANativeWindow *)win);
	window->setSwapInterval(interval);
}

struct ws_module ws_module_info = {
	waylandws_init_module,
	waylandws_GetDisplay,
	waylandws_Terminate,
	waylandws_CreateWindow,
	waylandws_DestroyWindow,
	waylandws_eglGetProcAddress,
	waylandws_passthroughImageKHR,
	waylandws_eglQueryString,
	waylandws_prepareSwap,
	waylandws_finishSwap,
	waylandws_setSwapInterval,
};







