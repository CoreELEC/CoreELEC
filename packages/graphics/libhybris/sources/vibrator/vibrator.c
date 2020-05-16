/*
 * Copyright (C) 2014 Jolla Ltd.
 * Contact: Simonas Leleiva <simonas.leleiva@jollamobile.com>
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

#include <hybris/common/binding.h>

HYBRIS_LIBRARY_INITIALIZE(vibrator, "libhardware_legacy.so");

HYBRIS_IMPLEMENT_FUNCTION0(vibrator, int, vibrator_exists);
HYBRIS_IMPLEMENT_FUNCTION1(vibrator, int, vibrator_on, int);
HYBRIS_IMPLEMENT_FUNCTION1(vibrator, int, vibrator_off, int);
