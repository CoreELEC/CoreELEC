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

#ifndef HOOKS_SHM_H_
#define HOOKS_SHM_H_

#include <stddef.h>
#include <stdint.h>

/* Leave space to workaround the issue that Android might pass negative int values */
#define HYBRIS_SHM_MASK_TOP (UINTPTR_MAX - 15)

typedef uintptr_t hybris_shm_pointer_t;

/* 
 * Allocate a space in the shared memory region of hybris
 */
hybris_shm_pointer_t hybris_shm_alloc(size_t size);
/* 
 * Test if the pointers points to the shm region
 */
int hybris_is_pointer_in_shm(void *ptr);
/* 
 * Convert an offset pointer to the shared memory to an absolute pointer that can be used in user space 
 * This function will return a NULL pointer if the handle does not actually point to the shm region
 */
void *hybris_get_shmpointer(hybris_shm_pointer_t handle);

#endif

// vim:ts=4:sw=4:noexpandtab
