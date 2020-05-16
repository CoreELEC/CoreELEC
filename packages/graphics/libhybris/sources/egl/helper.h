
/*
 * Copyright (c) 2013 Jolla Ltd.
 * Contact: Thomas Perl <thomas.perl@jollamobile.com>
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


#ifndef LIBHYBRIS_EGL_HELPER_H
#define LIBHYBRIS_EGL_HELPER_H


#include <EGL/egl.h>


#ifdef __cplusplus
extern "C" {
#endif


/* Add new mapping from surface to window */
void egl_helper_push_mapping(EGLSurface surface, EGLNativeWindowType window);

/* Check if a mapping for a surface exist */
int egl_helper_has_mapping(EGLSurface surface);

/* Return (without removing) the mapping for a surface */
EGLNativeWindowType egl_helper_get_mapping(EGLSurface surface);

/* Return and remove the mapping for a surface */
EGLNativeWindowType egl_helper_pop_mapping(EGLSurface surface);


#ifdef __cplusplus
};
#endif

#endif /* LIBHYBRIS_EGL_HELPER_H */
