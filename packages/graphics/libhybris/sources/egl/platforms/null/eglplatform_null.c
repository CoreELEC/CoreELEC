#include <android-config.h>
#include <ws.h>
#include <dlfcn.h>
#include <stdlib.h>
#include <string.h>

#include <hybris/common/binding.h>
#include <eglplatformcommon.h>
#include "logging.h"
#include <hybris/gralloc/gralloc.h>

#pragma GCC visibility push(hidden)
HYBRIS_LIBRARY_INITIALIZE(nullui, "/system/lib/libui.so");
#pragma GCC visibility pop

static HYBRIS_IMPLEMENT_FUNCTION0(nullui, EGLNativeWindowType, android_createDisplaySurface);

static void nullws_init_module(struct ws_egl_interface *egl_iface)
{
        hybris_gralloc_initialize(0);
	eglplatformcommon_init(egl_iface);
}

static struct _EGLDisplay *nullws_GetDisplay(EGLNativeDisplayType display)
{
	return malloc(sizeof(struct _EGLDisplay));
}

static void nullws_Terminate(struct _EGLDisplay *dpy)
{
	free(dpy);
}

static EGLNativeWindowType nullws_CreateWindow(EGLNativeWindowType win, struct _EGLDisplay *display)
{
	if (win == 0)
	{
		return android_createDisplaySurface();
	}
	else
		return win;
}

static void nullws_DestroyWindow(EGLNativeWindowType win)
{
	// TODO: Cleanup?
}

struct ws_module ws_module_info = {
	nullws_init_module,
	nullws_GetDisplay,
	nullws_Terminate,
	nullws_CreateWindow,
	nullws_DestroyWindow,
	eglplatformcommon_eglGetProcAddress,
	eglplatformcommon_passthroughImageKHR,
	eglplatformcommon_eglQueryString
};

