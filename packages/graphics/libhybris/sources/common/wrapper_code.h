/*
 * Copyright (c) 2017 Franz-Josef Haider <f_haider@gmx.at>
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

#ifndef WRAPPER_CODE_H
#define WRAPPER_CODE_H

#ifdef __cplusplus
extern "C" {
#endif

void wrapper_code_generic() __attribute__((naked,noinline));

#ifdef __cplusplus
}
#endif

#endif /* WRAPPER_CODE_H */
