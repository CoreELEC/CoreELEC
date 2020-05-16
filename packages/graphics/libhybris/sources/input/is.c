/*
 * Copyright (C) 2013 Canonical Ltd
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
 *
 * Authored by: Michael Frey <michael.frey@canonical.com>
 *              Ricardo Salveti de Araujo <ricardo.salveti@canonical.com>
 */

#include <dlfcn.h>
#include <stddef.h>
#include <stdbool.h>

#include <hybris/common/binding.h>
#include <hybris/input/input_stack_compatibility_layer.h>

#define COMPAT_LIBRARY_PATH "libis_compat_layer.so"

HYBRIS_LIBRARY_INITIALIZE(is, COMPAT_LIBRARY_PATH);

int android_input_check_availability()
{
	/* Both are defined via HYBRIS_LIBRARY_INITIALIZE */
	if (!is_handle)
		hybris_is_initialize();
	return is_handle ? 1 : 0;
}

HYBRIS_IMPLEMENT_VOID_FUNCTION2(is, android_input_stack_initialize,
	struct AndroidEventListener*, struct InputStackConfiguration*);
HYBRIS_IMPLEMENT_VOID_FUNCTION0(is, android_input_stack_start);
HYBRIS_IMPLEMENT_VOID_FUNCTION1(is, android_input_stack_start_waiting_for_flag, bool*);
HYBRIS_IMPLEMENT_VOID_FUNCTION0(is, android_input_stack_stop);
HYBRIS_IMPLEMENT_VOID_FUNCTION0(is, android_input_stack_shutdown);
