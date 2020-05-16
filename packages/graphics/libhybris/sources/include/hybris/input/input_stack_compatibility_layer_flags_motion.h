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

#ifndef INPUT_STACK_COMPATIBILITY_LAYER_FLAGS_MOTION_H_
#define INPUT_STACK_COMPATIBILITY_LAYER_FLAGS_MOTION_H_

#ifdef __cplusplus
extern "C" {
#endif

    /*
     * Motion event actions.
     */

    /* Bit shift for the action bits holding the pointer index as
     * defined by ISCL_MOTION_EVENT_ACTION_POINTER_INDEX_MASK.
     */
#define ISCL_MOTION_EVENT_ACTION_POINTER_INDEX_SHIFT 8

    enum {
        /* Bit mask of the parts of the action code that are the action itself.
         */
        ISCL_MOTION_EVENT_ACTION_MASK = 0xff,

        /* Bits in the action code that represent a pointer index, used with
         * ISCL_MOTION_EVENT_ACTION_POINTER_DOWN and ISCL_MOTION_EVENT_ACTION_POINTER_UP.  Shifting
         * down by ISCL_MOTION_EVENT_ACTION_POINTER_INDEX_SHIFT provides the actual pointer
         * index where the data for the pointer going up or down can be found.
         */
        ISCL_MOTION_EVENT_ACTION_POINTER_INDEX_MASK  = 0xff00,

        /* A pressed gesture has started, the motion contains the initial starting location.
         */
        ISCL_MOTION_EVENT_ACTION_DOWN = 0,

        /* A pressed gesture has finished, the motion contains the final release location
         * as well as any intermediate points since the last down or move event.
         */
        ISCL_MOTION_EVENT_ACTION_UP = 1,

        /* A change has happened during a press gesture (between ISCL_MOTION_EVENT_ACTION_DOWN and
         * ISCL_MOTION_EVENT_ACTION_UP).  The motion contains the most recent point, as well as
         * any intermediate points since the last down or move event.
         */
        ISCL_MOTION_EVENT_ACTION_MOVE = 2,

        /* The current gesture has been aborted.
         * You will not receive any more points in it.  You should treat this as
         * an up event, but not perform any action that you normally would.
         */
        ISCL_MOTION_EVENT_ACTION_CANCEL = 3,

        /* A movement has happened outside of the normal bounds of the UI element.
         * This does not provide a full gesture, but only the initial location of the movement/touch.
         */
        ISCL_MOTION_EVENT_ACTION_OUTSIDE = 4,

        /* A non-primary pointer has gone down.
         * The bits in ISCL_MOTION_EVENT_ACTION_POINTER_INDEX_MASK indicate which pointer changed.
         */
        ISCL_MOTION_EVENT_ACTION_POINTER_DOWN = 5,

        /* A non-primary pointer has gone up.
         * The bits in ISCL_MOTION_EVENT_ACTION_POINTER_INDEX_MASK indicate which pointer changed.
         */
        ISCL_MOTION_EVENT_ACTION_POINTER_UP = 6,

        /* A change happened but the pointer is not down (unlike ISCL_MOTION_EVENT_ACTION_MOVE).
         * The motion contains the most recent point, as well as any intermediate points since
         * the last hover move event.
         */
        ISCL_MOTION_EVENT_ACTION_HOVER_MOVE = 7,

        /* The motion event contains relative vertical and/or horizontal scroll offsets.
         * Use getAxisValue to retrieve the information from ISCL_MOTION_EVENT_AXIS_VSCROLL
         * and ISCL_MOTION_EVENT_AXIS_HSCROLL.
         * The pointer may or may not be down when this event is dispatched.
         * This action is always delivered to the winder under the pointer, which
         * may not be the window currently touched.
         */
        ISCL_MOTION_EVENT_ACTION_SCROLL = 8,

        /* The pointer is not down but has entered the boundaries of a window or view.
         */
        ISCL_MOTION_EVENT_ACTION_HOVER_ENTER = 9,

        /* The pointer is not down but has exited the boundaries of a window or view.
         */
        ISCL_MOTION_EVENT_ACTION_HOVER_EXIT = 10,
    };

    /*
     * Motion event flags.
     */
    enum
    {
        /* This flag indicates that the window that received this motion event is partly
         * or wholly obscured by another visible window above it.  This flag is set to true
         * even if the event did not directly pass through the obscured area.
         * A security sensitive application can check this flag to identify situations in which
         * a malicious application may have covered up part of its content for the purpose
         * of misleading the user or hijacking touches.  An appropriate response might be
         * to drop the suspect touches or to take additional precautions to confirm the user's
         * actual intent.
         */
        ISCL_MOTION_EVENT_FLAG_WINDOW_IS_OBSCURED = 0x1,
    };

    /*
     * Motion event edge touch flags.
     */
    enum
    {
        /* No edges intersected */
        ISCL_MOTION_EVENT_EDGE_FLAG_NONE = 0,

        /* Flag indicating the motion event intersected the top edge of the screen. */
        ISCL_MOTION_EVENT_EDGE_FLAG_TOP = 0x01,

        /* Flag indicating the motion event intersected the bottom edge of the screen. */
        ISCL_MOTION_EVENT_EDGE_FLAG_BOTTOM = 0x02,

        /* Flag indicating the motion event intersected the left edge of the screen. */
        ISCL_MOTION_EVENT_EDGE_FLAG_LEFT = 0x04,

        /* Flag indicating the motion event intersected the right edge of the screen. */
        ISCL_MOTION_EVENT_EDGE_FLAG_RIGHT = 0x08
    };

#ifdef __cplusplus
}
#endif

#endif // INPUT_STACK_COMPATIBILITY_LAYER_FLAGS_MOTION_H_
