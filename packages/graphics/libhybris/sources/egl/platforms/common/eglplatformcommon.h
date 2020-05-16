#ifndef __EGLPLATFORMCOMMON_H
#define __EGLPLATFORMCOMMON_H
#include <string.h>
#include <EGL/egl.h>

void eglplatformcommon_init(struct ws_egl_interface *egl_iface);
__eglMustCastToProperFunctionPointerType eglplatformcommon_eglGetProcAddress(const char *procname);
void eglplatformcommon_passthroughImageKHR(EGLContext *ctx, EGLenum *target, EGLClientBuffer *buffer, const EGLint **attrib_list);
const char *eglplatformcommon_eglQueryString(EGLDisplay dpy, EGLint name, const char *(*real_eglQueryString)(EGLDisplay dpy, EGLint name));
#endif
