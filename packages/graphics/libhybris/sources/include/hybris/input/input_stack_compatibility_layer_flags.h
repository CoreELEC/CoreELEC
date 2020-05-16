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
 * Authored by: Thomas Voss <thomas.voss@canonical.com>
 */

#ifndef INPUT_STACK_COMPATIBILITY_LAYER_FLAGS_H_
#define INPUT_STACK_COMPATIBILITY_LAYER_FLAGS_H_

#include <hybris/input/input_stack_compatibility_layer_flags_key.h>
#include <hybris/input/input_stack_compatibility_layer_flags_motion.h>

/*
 * Constants that identify tool types.
 * Refer to the documentation on the MotionEvent class for descriptions of each tool type.
 */
enum
{
    ISCL_MOTION_EVENT_TOOL_TYPE_UNKNOWN = 0,
    ISCL_MOTION_EVENT_TOOL_TYPE_FINGER = 1,
    ISCL_MOTION_EVENT_TOOL_TYPE_STYLUS = 2,
    ISCL_MOTION_EVENT_TOOL_TYPE_MOUSE = 3,
    ISCL_MOTION_EVENT_TOOL_TYPE_ERASER = 4,
};

/*
 * Input sources.
 *
 * Refer to the documentation on android.view.InputDevice for more details about input sources
 * and their correct interpretation.
 */
enum
{
    ISCL_INPUT_SOURCE_CLASS_MASK = 0x000000ff,

    ISCL_INPUT_SOURCE_CLASS_BUTTON = 0x00000001,
    ISCL_INPUT_SOURCE_CLASS_POINTER = 0x00000002,
    ISCL_INPUT_SOURCE_CLASS_NAVIGATION = 0x00000004,
    ISCL_INPUT_SOURCE_CLASS_POSITION = 0x00000008,
    ISCL_INPUT_SOURCE_CLASS_JOYSTICK = 0x00000010,
};

enum
{
    ISCL_INPUT_SOURCE_UNKNOWN = 0x00000000,

    ISCL_INPUT_SOURCE_KEYBOARD = 0x00000100 | ISCL_INPUT_SOURCE_CLASS_BUTTON,
    ISCL_INPUT_SOURCE_DPAD = 0x00000200 | ISCL_INPUT_SOURCE_CLASS_BUTTON,
    ISCL_INPUT_SOURCE_GAMEPAD = 0x00000400 | ISCL_INPUT_SOURCE_CLASS_BUTTON,
    ISCL_INPUT_SOURCE_TOUCHSCREEN = 0x00001000 | ISCL_INPUT_SOURCE_CLASS_POINTER,
    ISCL_INPUT_SOURCE_MOUSE = 0x00002000 | ISCL_INPUT_SOURCE_CLASS_POINTER,
    ISCL_INPUT_SOURCE_STYLUS = 0x00004000 | ISCL_INPUT_SOURCE_CLASS_POINTER,
    ISCL_INPUT_SOURCE_TRACKBALL = 0x00010000 | ISCL_INPUT_SOURCE_CLASS_NAVIGATION,
    ISCL_INPUT_SOURCE_TOUCHPAD = 0x00100000 | ISCL_INPUT_SOURCE_CLASS_POSITION,
    ISCL_INPUT_SOURCE_JOYSTICK = 0x01000000 | ISCL_INPUT_SOURCE_CLASS_JOYSTICK,

    ISCL_INPUT_SOURCE_ANY = 0xffffff00,
};

#endif // INPUT_STACK_COMPATIBILITY_LAYER_FLAGS_H_
