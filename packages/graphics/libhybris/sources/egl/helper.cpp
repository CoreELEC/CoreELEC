
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


#include "helper.h"

#include <assert.h>
#include <map>


/* Keep track of active EGL window surfaces */
static std::map<EGLSurface,EGLNativeWindowType> _surface_window_map;


void egl_helper_push_mapping(EGLSurface surface, EGLNativeWindowType window)
{
    assert(!egl_helper_has_mapping(surface));

    _surface_window_map[surface] = window;
}

int egl_helper_has_mapping(EGLSurface surface)
{
    return (_surface_window_map.find(surface) != _surface_window_map.end());
}

EGLNativeWindowType egl_helper_get_mapping(EGLSurface surface)
{
    std::map<EGLSurface,EGLNativeWindowType>::iterator it;
    it = _surface_window_map.find(surface);

    /* Caller must check with egl_helper_has_mapping() before */
    assert(it != _surface_window_map.end());

    return it->second;
}

EGLNativeWindowType egl_helper_pop_mapping(EGLSurface surface)
{
    std::map<EGLSurface,EGLNativeWindowType>::iterator it;
    it = _surface_window_map.find(surface);

    /* Caller must check with egl_helper_has_mapping() before */
    assert(it != _surface_window_map.end());

    EGLNativeWindowType result = it->second;
    _surface_window_map.erase(it);
    return result;
}
