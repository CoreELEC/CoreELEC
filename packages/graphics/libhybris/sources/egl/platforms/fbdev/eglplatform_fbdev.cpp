#include <android-config.h>
#include <ws.h>
#include "fbdev_window.h"
#include <malloc.h>
#include <assert.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <assert.h>

extern "C" {
#include <eglplatformcommon.h>
};

#include "logging.h"
#include <hybris/gralloc/gralloc.h>

static FbDevNativeWindow *_nativewindow = NULL;

extern "C" void fbdevws_init_module(struct ws_egl_interface *egl_iface)
{
    hybris_gralloc_initialize(1);
	eglplatformcommon_init(egl_iface);
}

extern "C" _EGLDisplay *fbdevws_GetDisplay(EGLNativeDisplayType display)
{
	_EGLDisplay *dpy = 0;
	if (display == EGL_DEFAULT_DISPLAY) {
		dpy = new _EGLDisplay;
	}
	return dpy;
}

extern "C" void fbdevws_Terminate(_EGLDisplay *dpy)
{
	delete dpy;
}

extern "C" EGLNativeWindowType fbdevws_CreateWindow(EGLNativeWindowType win, _EGLDisplay *display)
{
	assert (_nativewindow == NULL);

	_nativewindow = new FbDevNativeWindow();
	_nativewindow->common.incRef(&_nativewindow->common);
	return (EGLNativeWindowType) static_cast<struct ANativeWindow *>(_nativewindow);
}

extern "C" void fbdevws_DestroyWindow(EGLNativeWindowType win)
{
	assert (_nativewindow != NULL);
	assert (static_cast<FbDevNativeWindow *>((struct ANativeWindow *)win) == _nativewindow);

	_nativewindow->common.decRef(&_nativewindow->common);
	/* We are done with it, refcounting will delete the window when appropriate */
	_nativewindow = NULL;
}

extern "C" __eglMustCastToProperFunctionPointerType fbdevws_eglGetProcAddress(const char *procname) 
{
	return eglplatformcommon_eglGetProcAddress(procname);
}

extern "C" void fbdevws_passthroughImageKHR(EGLContext *ctx, EGLenum *target, EGLClientBuffer *buffer, const EGLint **attrib_list)
{
	eglplatformcommon_passthroughImageKHR(ctx, target, buffer, attrib_list);
}

extern "C" void fbdevws_setSwapInterval(EGLDisplay dpy, EGLNativeWindowType win, EGLint interval)
{
	FbDevNativeWindow *window = static_cast<FbDevNativeWindow *>((struct ANativeWindow *)win);
	window->setSwapInterval(interval);
}

struct ws_module ws_module_info = {
	fbdevws_init_module,
	fbdevws_GetDisplay,
	fbdevws_Terminate,
	fbdevws_CreateWindow,
	fbdevws_DestroyWindow,
	fbdevws_eglGetProcAddress,
	fbdevws_passthroughImageKHR,
	eglplatformcommon_eglQueryString,
	NULL,
	NULL,
	fbdevws_setSwapInterval,
};

// vim:ts=4:sw=4:noexpandtab
