/*
 * Copyright (C) 2015 Jolla Ltd
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
 */

#ifndef HYBRIS_HWCOMPOSER_H
#define HYBRIS_HWCOMPOSER_H

#ifdef __cplusplus
extern "C" {
#endif

struct ANativeWindow;
struct ANativeWindowBuffer;

typedef void (*HWCPresentCallback)(void *user_data, struct ANativeWindow *window,
                                   struct ANativeWindowBuffer *buffer);

/** Create a new HWC ANativeWindow.
 *
 * The Window can be casted to EGLNativeWindowType and used to create
 * an EGLSurface.
 * The specified present callback will be called by the window when a new
 * buffer is ready to be presented on screen. It is responsibility of the
 * caller to make sure that happens, by using the hwcomposer API.
 * Returns the window on success or NULL on failure.
 *
 * \param width The width of the window in pixels.
 * \param height The height of the window in pixels.
 * \param format The HAL format of the window.
 * \param present The present callback the window will use.
 * \param cbData The callback data the window will pass along to the present callback.
 *
 * \sa HWCNativeWindowDestroy
 */
struct ANativeWindow *HWCNativeWindowCreate(unsigned int width, unsigned int height, unsigned int format,
                                            HWCPresentCallback present, void *cbData);

/** Destroy a HWC ANativeWindow.
 *
 * Destroys a native window created with HWCNativeWindowCreate().
 * It is not necessary to call this after using eglDestroyWindowSurface()
 * with an EGLSurface backing a valid ANativeWindow, the window will
 * be destroyed automatically.
 *
 * \sa HWCNativeWindowCreate
 */
void HWCNativeWindowDestroy(struct ANativeWindow *window);

/** Get the current fence FD on a buffer.
 *
 * The buffer must be a buffer passed from the HWC layer trough the present
 * callback of a ANativeWindow.
 *
 * \sa HWCNativeWindowCreate
 * \sa HWCNativeBufferSetFence
 */
int HWCNativeBufferGetFence(struct ANativeWindowBuffer *buf);

/** Set the current fence FD on a buffer.
 *
 * The buffer must be a buffer passed from the HWC layer trough the present
 * callback of a ANativeWindow.
 *
 * \sa HWCNativeWindowCreate
 * \sa HWCNativeBufferGetFence
 */
void HWCNativeBufferSetFence(struct ANativeWindowBuffer *buf, int fd);

#ifdef __cplusplus
}
#endif

#endif
