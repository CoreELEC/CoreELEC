/*
 * Copyright (C) 2017 The LineageOS Project
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

/*
 * linker_non_pie_executables.h syntax:
 *
 * const char* linker_non_pie_executables[] = {
 *     "/path/to/executable1",
 *     "/path/to/executable2",
 * };
 */

#include <string.h>
#include "linker_non_pie_executables.h"

#include "hybris_compat.h"

bool allow_non_pie(const char* executable) {
    const int array_len = sizeof(linker_non_pie_executables)/sizeof(*linker_non_pie_executables);
    for (int i = 0; i < array_len; i++) {
        if (!strcmp(linker_non_pie_executables[i], executable))
            return true;
    }
    return false;
}
