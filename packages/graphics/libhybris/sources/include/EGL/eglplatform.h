/*
 * This confidential and proprietary software may be used only as
 * authorised by a licensing agreement from ARM Limited
 * (C) COPYRIGHT 2008-2010, 2013 ARM Limited
 * ALL RIGHTS RESERVED
 * The entire notice above must be reproduced on all authorised
 * copies and copies may only be made to the extent permitted
 * by a licensing agreement from ARM Limited.
 */

#ifndef __EGLPLATFORM_H__
#define __EGLPLATFORM_H__

#ifdef __cplusplus
extern "C" {
#endif

#ifndef EGLAPIENTRY
#define EGLAPIENTRY
#endif
#ifndef EGLAPIENTRYP
#define EGLAPIENTRYP EGLAPIENTRY*
#endif
#ifndef EGLAPI
#define EGLAPI extern
#endif

#include <EGL/fbdev_window.h>
#include <KHR/khrplatform.h>

typedef fbdev_window *NativeWindowType;
typedef void *NativePixmapType;
typedef void *NativeDisplayType;

#ifdef __cplusplus
}
#endif

/* EGL 1.2 types, renamed for consistency in EGL 1.3 */
typedef NativeDisplayType EGLNativeDisplayType;
typedef NativePixmapType EGLNativePixmapType;
typedef NativeWindowType EGLNativeWindowType;

/* Define EGLint. This must be an integral type large enough to contain
 * all legal attribute names and values passed into and out of EGL,
 * whether their type is boolean, bitmask, enumerant (symbolic
 * constant), integer, handle, or other.
 * While in general a 32-bit integer will suffice, if handles are
 * represented as pointers, then EGLint should be defined as a 64-bit
 * integer type.
 */
typedef int EGLint;

#endif /* __EGLPLATFORM_H__ */

