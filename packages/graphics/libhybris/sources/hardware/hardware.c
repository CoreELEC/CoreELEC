/*
 * Copyright (c) 2012 Simon Busch <morphis@gravedo.de>
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

#include <dlfcn.h>
#include <stddef.h>
#include <errno.h>
#include <hardware/hardware.h>
#include <hybris/common/binding.h>

#pragma GCC visibility push(hidden)
HYBRIS_LIBRARY_INITIALIZE(hardware, "libhardware.so");
#pragma GCC visibility pop

HYBRIS_IMPLEMENT_FUNCTION2(hardware, int, hw_get_module, const char *, const struct hw_module_t **);
HYBRIS_IMPLEMENT_FUNCTION3(hardware, int, hw_get_module_by_class, const char *, const char *, const struct hw_module_t **);

// vim:ts=4:sw=4:noexpandtab
