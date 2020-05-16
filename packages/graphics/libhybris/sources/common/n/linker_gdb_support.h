/*
 * Copyright (C) 2016 The Android Open Source Project
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
#ifndef __LINKER_GDB_SUPPORT_H
#define __LINKER_GDB_SUPPORT_H

#include <link.h>
#include <sys/cdefs.h>

__BEGIN_DECLS

void insert_link_map_into_debug_map(link_map* map);
void remove_link_map_from_debug_map(link_map* map);
void notify_gdb_of_load(link_map* map);
void notify_gdb_of_unload(link_map* map);
void notify_gdb_of_libraries();

extern struct r_debug _r_debug;
extern int _linker_enable_gdb_support;

__END_DECLS

#endif
