/*
 * Copyright (c) 2016 Canonical Ltd
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

#include <stdio.h>
#include <assert.h>

void *android_dlopen(const char *filename, int flag)
{
    fprintf(stderr, "%s called but not implemented!\n", __func__);
    return NULL;
}

void *android_dlsym(void *name, const char *symbol)
{
    fprintf(stderr, "%s called but not implemented!\n", __func__);
    return NULL;
}

int android_dlclose(void *handle)
{
    fprintf(stderr, "%s called but not implemented!\n", __func__);
    return -1;
}

const char *android_dlerror(void)
{
    fprintf(stderr, "%s called but not implemented!\n", __func__);
    return NULL;
}

int android_dladdr(const void *addr, void *info)
{
    fprintf(stderr, "%s called but not implemented!\n", __func__);
    return -1;
}
