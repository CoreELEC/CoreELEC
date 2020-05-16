/*
 * Copyright (C) 2015 The Android Open Source Project
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

#ifndef __LINKER_RELOCS_H
#define __LINKER_RELOCS_H

#include <elf.h>

#define R_GENERIC_NONE 0 // R_*_NONE is always 0

#if defined (__aarch64__)

#define R_GENERIC_JUMP_SLOT R_AARCH64_JUMP_SLOT
#define R_GENERIC_GLOB_DAT  R_AARCH64_GLOB_DAT
#define R_GENERIC_RELATIVE  R_AARCH64_RELATIVE
#define R_GENERIC_IRELATIVE R_AARCH64_IRELATIVE

#elif defined (__arm__)

#define R_GENERIC_JUMP_SLOT R_ARM_JUMP_SLOT
#define R_GENERIC_GLOB_DAT  R_ARM_GLOB_DAT
#define R_GENERIC_RELATIVE  R_ARM_RELATIVE
#define R_GENERIC_IRELATIVE R_ARM_IRELATIVE

#elif defined (__i386__)

#define R_GENERIC_JUMP_SLOT R_386_JMP_SLOT
#define R_GENERIC_GLOB_DAT  R_386_GLOB_DAT
#define R_GENERIC_RELATIVE  R_386_RELATIVE
#define R_GENERIC_IRELATIVE R_386_IRELATIVE

#elif defined (__x86_64__)

#define R_GENERIC_JUMP_SLOT R_X86_64_JUMP_SLOT
#define R_GENERIC_GLOB_DAT  R_X86_64_GLOB_DAT
#define R_GENERIC_RELATIVE  R_X86_64_RELATIVE
#define R_GENERIC_IRELATIVE R_X86_64_IRELATIVE

#endif

#endif // __LINKER_RELOCS_H
