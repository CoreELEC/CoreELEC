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

#include "wrapper_code.h"

void
wrapper_code_generic()
{
    // we can never use r0-r11, neither the stack
    asm volatile(
        // preserve the registers
	".arm\n\
        push {r0-r11, lr}\n"

	".arm\n\
        ldr r0, fun\n" // load the function pointer to r0
	".arm\n\
        ldr r1, name\n" // load the address of the functions name to r1
	".arm\n\
        ldr r2, str\n" // load the string to print
	".arm\n\
        ldr r4, tc\n" // load the address of trace_callback to r4
	".arm\n\
        blx r4\n" // call trace_callback

        // restore the registers
	".arm\n\
        pop {r0-r11, lr}\n"
	".arm\n\
        ldr pc, fun\n"     // jump to function

        // dummy instructions, this is where we locate our pointers
        "name: .word 0xFFFFFFFF\n" // name of function to call
        "fun: .word 0xFFFFFFFF\n" // function to call
        "tc: .word 0xFFFFFFFF\n" // address of trace_callback
        "str: .word 0xFFFFFFFF\n" // the string being printed in trace_callback
    );
}
