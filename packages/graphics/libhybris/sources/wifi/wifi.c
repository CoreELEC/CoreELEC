/*
 * Copyright (C) 2014 Canonical Ltd
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
 * Authored by: Alfonso Sanchez-Beato <alfonso.sanchez-beato@canonical.com>
 */

#include <dlfcn.h>
#include <stddef.h>

#include <hybris/common/binding.h>

#define COMPAT_LIBRARY_PATH "libhardware_legacy.so"

HYBRIS_LIBRARY_INITIALIZE(wifi, COMPAT_LIBRARY_PATH);

int wifi_compat_check_availability()
{
    /* Both are defined via HYBRIS_LIBRARY_INITIALIZE */
    hybris_wifi_initialize();
    return wifi_handle ? 1 : 0;
}

HYBRIS_IMPLEMENT_FUNCTION0(wifi, int, wifi_load_driver);
HYBRIS_IMPLEMENT_FUNCTION0(wifi, int, wifi_unload_driver);
HYBRIS_IMPLEMENT_FUNCTION0(wifi, int, is_wifi_driver_loaded);
HYBRIS_IMPLEMENT_FUNCTION1(wifi, int, wifi_start_supplicant, int);
HYBRIS_IMPLEMENT_FUNCTION1(wifi, int, wifi_stop_supplicant, int);
HYBRIS_IMPLEMENT_FUNCTION0(wifi, int, wifi_connect_to_supplicant);
HYBRIS_IMPLEMENT_VOID_FUNCTION0(wifi, wifi_close_supplicant_connection);
HYBRIS_IMPLEMENT_FUNCTION2(wifi, int, wifi_wait_for_event, char *, size_t);
HYBRIS_IMPLEMENT_FUNCTION3(wifi, int, wifi_command,
				const char *, char *, size_t *);
HYBRIS_IMPLEMENT_FUNCTION7(wifi, int, do_dhcp_request, int *, int *, int *,
				int *, int *, int *, int *);
HYBRIS_IMPLEMENT_FUNCTION0(wifi, const char *, get_dhcp_error_string);
HYBRIS_IMPLEMENT_FUNCTION1(wifi, const char *, wifi_get_fw_path, int);
HYBRIS_IMPLEMENT_FUNCTION1(wifi, int, wifi_change_fw_path, const char *);
HYBRIS_IMPLEMENT_FUNCTION0(wifi, int, ensure_entropy_file_exists);
HYBRIS_IMPLEMENT_FUNCTION4(wifi, int, wifi_send_driver_command,
				char*, char*, char*, size_t);
