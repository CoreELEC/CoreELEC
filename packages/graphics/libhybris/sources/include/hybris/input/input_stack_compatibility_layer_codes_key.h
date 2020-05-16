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

#ifndef INPUT_STACK_COMPATIBILITY_LAYER_CODES_KEY_H_
#define INPUT_STACK_COMPATIBILITY_LAYER_CODES_KEY_H_

/******************************************************************
 *
 * IMPORTANT NOTICE:
 *
 *   This file is part of Android's set of stable system headers
 *   exposed by the Android NDK (Native Development Kit).
 *
 *   Third-party source AND binary code relies on the definitions
 *   here to be FROZEN ON ALL UPCOMING PLATFORM RELEASES.
 *
 *   - DO NOT MODIFY ENUMS (EXCEPT IF YOU ADD NEW 32-BIT VALUES)
 *   - DO NOT MODIFY CONSTANTS OR FUNCTIONAL MACROS
 *   - DO NOT CHANGE THE SIGNATURE OF FUNCTIONS IN ANY WAY
 *   - DO NOT CHANGE THE LAYOUT OR SIZE OF STRUCTURES
 */

#include <sys/types.h>

#ifdef __cplusplus
extern "C" {
#endif

    /*
     * Key codes.
     */
    enum {
        ISCL_KEYCODE_UNKNOWN         = 0,
        ISCL_KEYCODE_SOFT_LEFT       = 1,
        ISCL_KEYCODE_SOFT_RIGHT      = 2,
        ISCL_KEYCODE_HOME            = 3,
        ISCL_KEYCODE_BACK            = 4,
        ISCL_KEYCODE_CALL            = 5,
        ISCL_KEYCODE_ENDCALL         = 6,
        ISCL_KEYCODE_0               = 7,
        ISCL_KEYCODE_1               = 8,
        ISCL_KEYCODE_2               = 9,
        ISCL_KEYCODE_3               = 10,
        ISCL_KEYCODE_4               = 11,
        ISCL_KEYCODE_5               = 12,
        ISCL_KEYCODE_6               = 13,
        ISCL_KEYCODE_7               = 14,
        ISCL_KEYCODE_8               = 15,
        ISCL_KEYCODE_9               = 16,
        ISCL_KEYCODE_STAR            = 17,
        ISCL_KEYCODE_POUND           = 18,
        ISCL_KEYCODE_DPAD_UP         = 19,
        ISCL_KEYCODE_DPAD_DOWN       = 20,
        ISCL_KEYCODE_DPAD_LEFT       = 21,
        ISCL_KEYCODE_DPAD_RIGHT      = 22,
        ISCL_KEYCODE_DPAD_CENTER     = 23,
        ISCL_KEYCODE_VOLUME_UP       = 24,
        ISCL_KEYCODE_VOLUME_DOWN     = 25,
        ISCL_KEYCODE_POWER           = 26,
        ISCL_KEYCODE_CAMERA          = 27,
        ISCL_KEYCODE_CLEAR           = 28,
        ISCL_KEYCODE_A               = 29,
        ISCL_KEYCODE_B               = 30,
        ISCL_KEYCODE_C               = 31,
        ISCL_KEYCODE_D               = 32,
        ISCL_KEYCODE_E               = 33,
        ISCL_KEYCODE_F               = 34,
        ISCL_KEYCODE_G               = 35,
        ISCL_KEYCODE_H               = 36,
        ISCL_KEYCODE_I               = 37,
        ISCL_KEYCODE_J               = 38,
        ISCL_KEYCODE_K               = 39,
        ISCL_KEYCODE_L               = 40,
        ISCL_KEYCODE_M               = 41,
        ISCL_KEYCODE_N               = 42,
        ISCL_KEYCODE_O               = 43,
        ISCL_KEYCODE_P               = 44,
        ISCL_KEYCODE_Q               = 45,
        ISCL_KEYCODE_R               = 46,
        ISCL_KEYCODE_S               = 47,
        ISCL_KEYCODE_T               = 48,
        ISCL_KEYCODE_U               = 49,
        ISCL_KEYCODE_V               = 50,
        ISCL_KEYCODE_W               = 51,
        ISCL_KEYCODE_X               = 52,
        ISCL_KEYCODE_Y               = 53,
        ISCL_KEYCODE_Z               = 54,
        ISCL_KEYCODE_COMMA           = 55,
        ISCL_KEYCODE_PERIOD          = 56,
        ISCL_KEYCODE_ALT_LEFT        = 57,
        ISCL_KEYCODE_ALT_RIGHT       = 58,
        ISCL_KEYCODE_SHIFT_LEFT      = 59,
        ISCL_KEYCODE_SHIFT_RIGHT     = 60,
        ISCL_KEYCODE_TAB             = 61,
        ISCL_KEYCODE_SPACE           = 62,
        ISCL_KEYCODE_SYM             = 63,
        ISCL_KEYCODE_EXPLORER        = 64,
        ISCL_KEYCODE_ENVELOPE        = 65,
        ISCL_KEYCODE_ENTER           = 66,
        ISCL_KEYCODE_DEL             = 67,
        ISCL_KEYCODE_GRAVE           = 68,
        ISCL_KEYCODE_MINUS           = 69,
        ISCL_KEYCODE_EQUALS          = 70,
        ISCL_KEYCODE_LEFT_BRACKET    = 71,
        ISCL_KEYCODE_RIGHT_BRACKET   = 72,
        ISCL_KEYCODE_BACKSLASH       = 73,
        ISCL_KEYCODE_SEMICOLON       = 74,
        ISCL_KEYCODE_APOSTROPHE      = 75,
        ISCL_KEYCODE_SLASH           = 76,
        ISCL_KEYCODE_AT              = 77,
        ISCL_KEYCODE_NUM             = 78,
        ISCL_KEYCODE_HEADSETHOOK     = 79,
        ISCL_KEYCODE_FOCUS           = 80,   // *Camera* focus
        ISCL_KEYCODE_PLUS            = 81,
        ISCL_KEYCODE_MENU            = 82,
        ISCL_KEYCODE_NOTIFICATION    = 83,
        ISCL_KEYCODE_SEARCH          = 84,
        ISCL_KEYCODE_MEDIA_PLAY_PAUSE= 85,
        ISCL_KEYCODE_MEDIA_STOP      = 86,
        ISCL_KEYCODE_MEDIA_NEXT      = 87,
        ISCL_KEYCODE_MEDIA_PREVIOUS  = 88,
        ISCL_KEYCODE_MEDIA_REWIND    = 89,
        ISCL_KEYCODE_MEDIA_FAST_FORWARD = 90,
        ISCL_KEYCODE_MUTE            = 91,
        ISCL_KEYCODE_PAGE_UP         = 92,
        ISCL_KEYCODE_PAGE_DOWN       = 93,
        ISCL_KEYCODE_PICTSYMBOLS     = 94,
        ISCL_KEYCODE_SWITCH_CHARSET  = 95,
        ISCL_KEYCODE_BUTTON_A        = 96,
        ISCL_KEYCODE_BUTTON_B        = 97,
        ISCL_KEYCODE_BUTTON_C        = 98,
        ISCL_KEYCODE_BUTTON_X        = 99,
        ISCL_KEYCODE_BUTTON_Y        = 100,
        ISCL_KEYCODE_BUTTON_Z        = 101,
        ISCL_KEYCODE_BUTTON_L1       = 102,
        ISCL_KEYCODE_BUTTON_R1       = 103,
        ISCL_KEYCODE_BUTTON_L2       = 104,
        ISCL_KEYCODE_BUTTON_R2       = 105,
        ISCL_KEYCODE_BUTTON_THUMBL   = 106,
        ISCL_KEYCODE_BUTTON_THUMBR   = 107,
        ISCL_KEYCODE_BUTTON_START    = 108,
        ISCL_KEYCODE_BUTTON_SELECT   = 109,
        ISCL_KEYCODE_BUTTON_MODE     = 110,
        ISCL_KEYCODE_ESCAPE          = 111,
        ISCL_KEYCODE_FORWARD_DEL     = 112,
        ISCL_KEYCODE_CTRL_LEFT       = 113,
        ISCL_KEYCODE_CTRL_RIGHT      = 114,
        ISCL_KEYCODE_CAPS_LOCK       = 115,
        ISCL_KEYCODE_SCROLL_LOCK     = 116,
        ISCL_KEYCODE_META_LEFT       = 117,
        ISCL_KEYCODE_META_RIGHT      = 118,
        ISCL_KEYCODE_FUNCTION        = 119,
        ISCL_KEYCODE_SYSRQ           = 120,
        ISCL_KEYCODE_BREAK           = 121,
        ISCL_KEYCODE_MOVE_HOME       = 122,
        ISCL_KEYCODE_MOVE_END        = 123,
        ISCL_KEYCODE_INSERT          = 124,
        ISCL_KEYCODE_FORWARD         = 125,
        ISCL_KEYCODE_MEDIA_PLAY      = 126,
        ISCL_KEYCODE_MEDIA_PAUSE     = 127,
        ISCL_KEYCODE_MEDIA_CLOSE     = 128,
        ISCL_KEYCODE_MEDIA_EJECT     = 129,
        ISCL_KEYCODE_MEDIA_RECORD    = 130,
        ISCL_KEYCODE_F1              = 131,
        ISCL_KEYCODE_F2              = 132,
        ISCL_KEYCODE_F3              = 133,
        ISCL_KEYCODE_F4              = 134,
        ISCL_KEYCODE_F5              = 135,
        ISCL_KEYCODE_F6              = 136,
        ISCL_KEYCODE_F7              = 137,
        ISCL_KEYCODE_F8              = 138,
        ISCL_KEYCODE_F9              = 139,
        ISCL_KEYCODE_F10             = 140,
        ISCL_KEYCODE_F11             = 141,
        ISCL_KEYCODE_F12             = 142,
        ISCL_KEYCODE_NUM_LOCK        = 143,
        ISCL_KEYCODE_NUMPAD_0        = 144,
        ISCL_KEYCODE_NUMPAD_1        = 145,
        ISCL_KEYCODE_NUMPAD_2        = 146,
        ISCL_KEYCODE_NUMPAD_3        = 147,
        ISCL_KEYCODE_NUMPAD_4        = 148,
        ISCL_KEYCODE_NUMPAD_5        = 149,
        ISCL_KEYCODE_NUMPAD_6        = 150,
        ISCL_KEYCODE_NUMPAD_7        = 151,
        ISCL_KEYCODE_NUMPAD_8        = 152,
        ISCL_KEYCODE_NUMPAD_9        = 153,
        ISCL_KEYCODE_NUMPAD_DIVIDE   = 154,
        ISCL_KEYCODE_NUMPAD_MULTIPLY = 155,
        ISCL_KEYCODE_NUMPAD_SUBTRACT = 156,
        ISCL_KEYCODE_NUMPAD_ADD      = 157,
        ISCL_KEYCODE_NUMPAD_DOT      = 158,
        ISCL_KEYCODE_NUMPAD_COMMA    = 159,
        ISCL_KEYCODE_NUMPAD_ENTER    = 160,
        ISCL_KEYCODE_NUMPAD_EQUALS   = 161,
        ISCL_KEYCODE_NUMPAD_LEFT_PAREN = 162,
        ISCL_KEYCODE_NUMPAD_RIGHT_PAREN = 163,
        ISCL_KEYCODE_VOLUME_MUTE     = 164,
        ISCL_KEYCODE_INFO            = 165,
        ISCL_KEYCODE_CHANNEL_UP      = 166,
        ISCL_KEYCODE_CHANNEL_DOWN    = 167,
        ISCL_KEYCODE_ZOOM_IN         = 168,
        ISCL_KEYCODE_ZOOM_OUT        = 169,
        ISCL_KEYCODE_TV              = 170,
        ISCL_KEYCODE_WINDOW          = 171,
        ISCL_KEYCODE_GUIDE           = 172,
        ISCL_KEYCODE_DVR             = 173,
        ISCL_KEYCODE_BOOKMARK        = 174,
        ISCL_KEYCODE_CAPTIONS        = 175,
        ISCL_KEYCODE_SETTINGS        = 176,
        ISCL_KEYCODE_TV_POWER        = 177,
        ISCL_KEYCODE_TV_INPUT        = 178,
        ISCL_KEYCODE_STB_POWER       = 179,
        ISCL_KEYCODE_STB_INPUT       = 180,
        ISCL_KEYCODE_AVR_POWER       = 181,
        ISCL_KEYCODE_AVR_INPUT       = 182,
        ISCL_KEYCODE_PROG_RED        = 183,
        ISCL_KEYCODE_PROG_GREEN      = 184,
        ISCL_KEYCODE_PROG_YELLOW     = 185,
        ISCL_KEYCODE_PROG_BLUE       = 186,
        ISCL_KEYCODE_APP_SWITCH      = 187,
        ISCL_KEYCODE_BUTTON_1        = 188,
        ISCL_KEYCODE_BUTTON_2        = 189,
        ISCL_KEYCODE_BUTTON_3        = 190,
        ISCL_KEYCODE_BUTTON_4        = 191,
        ISCL_KEYCODE_BUTTON_5        = 192,
        ISCL_KEYCODE_BUTTON_6        = 193,
        ISCL_KEYCODE_BUTTON_7        = 194,
        ISCL_KEYCODE_BUTTON_8        = 195,
        ISCL_KEYCODE_BUTTON_9        = 196,
        ISCL_KEYCODE_BUTTON_10       = 197,
        ISCL_KEYCODE_BUTTON_11       = 198,
        ISCL_KEYCODE_BUTTON_12       = 199,
        ISCL_KEYCODE_BUTTON_13       = 200,
        ISCL_KEYCODE_BUTTON_14       = 201,
        ISCL_KEYCODE_BUTTON_15       = 202,
        ISCL_KEYCODE_BUTTON_16       = 203,
        ISCL_KEYCODE_LANGUAGE_SWITCH = 204,
        ISCL_KEYCODE_MANNER_MODE     = 205,
        ISCL_KEYCODE_3D_MODE         = 206,
        ISCL_KEYCODE_CONTACTS        = 207,
        ISCL_KEYCODE_CALENDAR        = 208,
        ISCL_KEYCODE_MUSIC           = 209,
        ISCL_KEYCODE_CALCULATOR      = 210,

    };

#ifdef __cplusplus
}
#endif

#endif // INPUT_STACK_COMPATIBILITY_LAYER_CODES_KEY_H_
