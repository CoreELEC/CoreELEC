/*
 * Copyright (c) 2013 Intel Corporation
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

#ifndef _HYBRIS_DLFCN_H_
#define _HYBRIS_DLFCN_H_

#ifdef __cplusplus
extern "C" {
#endif

void *hybris_dlopen(const char *filename, int flag);
void *hybris_dlsym(void *handle, const char *symbol);
int   hybris_dlclose(void *handle);
char *hybris_dlerror(void);

#ifdef __cplusplus
}
#endif

#endif // _HYBRIS_DLFCN_H_

// vim: noai:ts=4:sw=4:ss=4:expandtab
