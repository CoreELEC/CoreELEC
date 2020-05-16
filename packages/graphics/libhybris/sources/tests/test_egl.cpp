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

#include <android-config.h>
#include <EGL/egl.h>
#include <assert.h>
#include <stdio.h>
#include "hybris_nativebufferext.h"
#include <string.h>
#include <EGL/eglext.h>
int main(int argc, char **argv)
{
	EGLDisplay display;
	EGLConfig ecfg;
	EGLint num_config;
	EGLint attr[] = {       // some attributes to set up our egl-interface
		EGL_BUFFER_SIZE, 32,
		EGL_RENDERABLE_TYPE,
		EGL_OPENGL_ES2_BIT,
		EGL_NONE
	};
	EGLSurface surface;
	EGLint ctxattr[] = {
		EGL_CONTEXT_CLIENT_VERSION, 2,
		EGL_NONE
	};
	EGLContext context;

	EGLBoolean rv;
	PFNEGLHYBRISCREATENATIVEBUFFERPROC eglHybrisCreateNativeBuffer;
	PFNEGLHYBRISLOCKNATIVEBUFFERPROC eglHybrisLockNativeBuffer;
	PFNEGLHYBRISUNLOCKNATIVEBUFFERPROC eglHybrisUnlockNativeBuffer;
	PFNEGLHYBRISRELEASENATIVEBUFFERPROC eglHybrisReleaseNativeBuffer;
	PFNEGLCREATEIMAGEKHRPROC eglCreateImageKHR;
	PFNEGLDESTROYIMAGEKHRPROC eglDestroyImageKHR;

	display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
	assert(eglGetError() == EGL_SUCCESS);
	assert(display != EGL_NO_DISPLAY);

	rv = eglInitialize(display, NULL, NULL);
	assert(eglGetError() == EGL_SUCCESS);
	assert(rv == EGL_TRUE);

	rv = eglChooseConfig((EGLDisplay) display, attr, &ecfg, 1, &num_config);
	assert(eglGetError() == EGL_SUCCESS);
	assert(rv == EGL_TRUE);

	surface = eglCreateWindowSurface((EGLDisplay) display, ecfg, (EGLNativeWindowType)NULL, NULL);
	assert(eglGetError() == EGL_SUCCESS);
	assert(surface != EGL_NO_SURFACE);

	context = eglCreateContext((EGLDisplay) display, ecfg, EGL_NO_CONTEXT, ctxattr);
	assert(eglGetError() == EGL_SUCCESS);
	assert(context != EGL_NO_CONTEXT);

	assert(eglMakeCurrent((EGLDisplay) display, surface, surface, context) == EGL_TRUE);

	if (strstr(eglQueryString(display, EGL_EXTENSIONS), "EGL_HYBRIS_native_buffer") != NULL)
	{
		printf("Found EGL_HYBRIS_native_buffer\n");
		assert((eglHybrisCreateNativeBuffer = (PFNEGLHYBRISCREATENATIVEBUFFERPROC) eglGetProcAddress("eglHybrisCreateNativeBuffer")) != NULL);
		printf("Found eglHybrisCreateNativeBuffer\n");
		EGLClientBuffer buf;
		EGLint stride;
		void *loc;
		assert(eglHybrisCreateNativeBuffer(320, 480, HYBRIS_USAGE_SW_READ_RARELY|HYBRIS_USAGE_SW_WRITE_RARELY|HYBRIS_USAGE_HW_TEXTURE, HYBRIS_PIXEL_FORMAT_RGBA_8888, &stride, &buf) == EGL_TRUE);
		printf("Stride is %i\n", stride);
		assert((eglHybrisLockNativeBuffer = (PFNEGLHYBRISLOCKNATIVEBUFFERPROC) eglGetProcAddress("eglHybrisLockNativeBuffer")) != NULL);
		printf("Found eglHybrisLockBuffer\n");
		assert(eglHybrisLockNativeBuffer(buf, HYBRIS_USAGE_SW_WRITE_RARELY, 0, 0, 320, 480, &loc) == EGL_TRUE);
		printf("locked at %p\n", loc);
		printf("Memsetting..\n");
		memset(loc, 0xEF, (stride*480));
		assert((eglHybrisUnlockNativeBuffer = (PFNEGLHYBRISUNLOCKNATIVEBUFFERPROC) eglGetProcAddress("eglHybrisUnlockNativeBuffer")) != NULL);
		printf("Found eglHybrisUnlockBuffer\n");
		eglHybrisUnlockNativeBuffer(buf);
		assert((eglCreateImageKHR = (PFNEGLCREATEIMAGEKHRPROC) eglGetProcAddress("eglCreateImageKHR")) != NULL);
		printf("Found eglCreateImageKHR\n");
		EGLImageKHR image = eglCreateImageKHR(
								display, EGL_NO_CONTEXT, EGL_NATIVE_BUFFER_HYBRIS,
								(EGLClientBuffer)buf, NULL);
		assert(image != EGL_NO_IMAGE_KHR);
		assert((eglDestroyImageKHR = (PFNEGLDESTROYIMAGEKHRPROC) eglGetProcAddress("eglDestroyImageKHR")) != NULL);
		printf("Found eglDestroyImageKHR\n");
		assert(eglDestroyImageKHR(display, image));

		assert((eglHybrisReleaseNativeBuffer = (PFNEGLHYBRISRELEASENATIVEBUFFERPROC) eglGetProcAddress("eglHybrisReleaseNativeBuffer")) != NULL);
		printf("Found eglHybrisReleaseNativeBuffer\n");
		eglHybrisReleaseNativeBuffer(buf);
	}

	printf("stop\n");

	eglDestroyContext((EGLDisplay) display, context);
	printf("destroyed context\n");

	eglDestroySurface((EGLDisplay) display, surface);
	printf("destroyed surface\n");
	eglTerminate((EGLDisplay) display);
	return 0;
}

// vim:ts=4:sw=4:noexpandtab
