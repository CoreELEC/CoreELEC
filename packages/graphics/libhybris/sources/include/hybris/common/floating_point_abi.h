/*
 * Copyright (C) 2013 Jolla Ltd.
 * Contact: Thomas Perl <thomas.perl@jollamobile.com>
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

#ifndef HYBRIS_FLOATING_POINT_ABI_H_
#define HYBRIS_FLOATING_POINT_ABI_H_

/**
 * Make sure to use FP_ATTRIB on all functions that are loaded from
 * Android (bionic libc) libraries to make sure floating point arguments
 * are passed the right way.
 *
 * See: http://wiki.debian.org/ArmHardFloatPort/VfpComparison
 *      http://gcc.gnu.org/onlinedocs/gcc/Function-Attributes.html
 *
 * It doesn't hurt to have it added for non-floating point arguments and
 * return types, even though it does not really have any effect.
 *
 * If you use the convenience macros in hybris/common/binding.h, your
 * wrapper functions will automatically make use of this attribute.
 **/

#ifdef __ARM_PCS_VFP
#  define FP_ATTRIB __attribute__((pcs("aapcs")))
#else
#  define FP_ATTRIB
#endif

#endif /* HYBRIS_FLOATING_POINT_ABI_H_ */
