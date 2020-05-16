/*
 * Copyright (C) 2007 The Android Open Source Project
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

#define LOG_TAG "NativeHandle"

#include <android-config.h>
#include <stdint.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include <cutils/native_handle.h>

native_handle_t* native_handle_create(int numFds, int numInts)
{
    native_handle_t* h = malloc(
            sizeof(native_handle_t) + sizeof(int)*(numFds+numInts));

    h->version = sizeof(native_handle_t);
    h->numFds = numFds;
    h->numInts = numInts;
    return h;
}

int native_handle_delete(native_handle_t* h)
{
    if (h) {
        if (h->version != sizeof(native_handle_t))
            return -EINVAL;
        free(h);
    }
    return 0;
}

int native_handle_close(const native_handle_t* h)
{
    if (h->version != sizeof(native_handle_t))
        return -EINVAL;

    const int numFds = h->numFds;
    int i;
    for (i=0 ; i<numFds ; i++) {
        close(h->data[i]);
    }
    return 0;
}
