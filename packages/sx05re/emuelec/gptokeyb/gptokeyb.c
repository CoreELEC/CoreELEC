/* Copyright (c) 2021
*
* This program is free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public
* License as published by the Free Software Foundation; either
* version 2 of the License, or (at your option) any later version.
#
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
* General Public License for more details.
#
* You should have received a copy of the GNU General Public
* License along with this program; if not, write to the
* Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
* Boston, MA 02110-1301 USA
#
* Authored by: Kris Henriksen <krishenriksen.work@gmail.com>
#
* AnberPorts-Keyboard-Mouse
* 
* from https://github.com/krishenriksen/AnberPorts/blob/master/AnberPorts-Keyboard-Mouse/main.c
*
* And part of joystick.c from: https://gist.github.com/jasonwhite/c5b2048c15993d285130
* Author: Jason White
*
* Description:
* Reads joystick/gamepad events and displays them.
*
* Compile:
* gcc joystick.c -o joystick
*
* Run:
* ./joystick [/dev/input/jsX]
*
* See also:
* https://www.kernel.org/doc/Documentation/input/joystick-api.txt
* 
* Modified (badly) by: Shanti Gilbert for EmuELEC
* 
* 
*/

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

#include <linux/input.h>
#include <linux/uinput.h>

#include <libevdev-1.0/libevdev/libevdev.h>
#include <libevdev-1.0/libevdev/libevdev-uinput.h>

#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <linux/joystick.h>

struct libevdev* dev_joypad;
int fd_ev_joypad;
int rc_joypad;
struct input_event ev_joypad;

static int uinp_fd = -1;
struct uinput_user_dev uidev;

int select_b;
int start;
int a;
int b;
int up;
int down;
int left;
int right;
char* jsdevice;
char*  oga_ver;

/**
 * Reads a joystick event from the joystick device.
 *
 * Returns 0 on success. Otherwise -1 is returned.
 */
int read_event(int fd, struct js_event *event)
{
    ssize_t bytes;

    bytes = read(fd, event, sizeof(*event));

    if (bytes == sizeof(*event))
        return 0;

    /* Error, could not read full event. */
    return -1;
}

/**
 * Returns the number of axes on the controller or 0 if an error occurs.
 */
size_t get_axis_count(int fd)
{
    __u8 axes;

    if (ioctl(fd, JSIOCGAXES, &axes) == -1)
        return 0;

    return axes;
}

/**
 * Returns the number of buttons on the controller or 0 if an error occurs.
 */
size_t get_button_count(int fd)
{
    __u8 buttons;
    if (ioctl(fd, JSIOCGBUTTONS, &buttons) == -1)
        return 0;

    return buttons;
}

/**
 * Current state of an axis.
 */
struct axis_state {
    short x, y;
};

/**
 * Keeps track of the current axis state.
 *
 * NOTE: This function assumes that axes are numbered starting from 0, and that
 * the X axis is an even number, and the Y axis is an odd number. However, this
 * is usually a safe assumption.
 *
 * Returns the axis that the event indicated.
 */
size_t get_axis_state(struct js_event *event, struct axis_state axes[3])
{
    size_t axis = event->number / 2;

    if (axis < 4)
    {
        if (event->number % 2 == 0)
            axes[axis].x = event->value;
        else
            axes[axis].y = event->value;
    }

    return axis;
}


// end joystick.c

void print_usage() {
    printf("Usage: gptokeyb needs 10 non-optional arguments for buttons and dpad as well as JS device and oga_ver\n"
       "buttons are based on raw inputs (based on retroarch configurations), set oga_ver to 1 for OGA/S or 0 for other devices\n"
       "Example:\n"
       "gptokeyb 12 17 0 1 8 9 10 11 /dev/input/js0 1\n\n");
   exit(EXIT_FAILURE);
}


void emit(int type, int code, int val) {
   struct input_event ev;

   ev.type = type;
   ev.code = code;
   ev.value = val;
   /* timestamp values below are ignored */
   ev.time.tv_sec = 0;
   ev.time.tv_usec = 0;

   write(uinp_fd, &ev, sizeof(ev));
}

void handle_event(int type, int code, int value) {
    
    // printf("Info: code: %u, Value: %u, type: %u\n", code, value, type);
    // printf("Keys: up: %u, down: %u, left: %u Right: %u\n", up, down, left, right);
	
    if (type == 1) {
		if (code == start && (value == 1 || value == 2)) {
			emit(EV_KEY, KEY_ENTER, 1);
			emit(EV_SYN, SYN_REPORT, 0);
		}
		else if (code == select_b && (value == 1 || value == 2)) {
            emit(EV_KEY, KEY_PLAYPAUSE, 1);
			emit(EV_SYN, SYN_REPORT, 0);
		}
		else if (code == a && (value == 1 || value == 2)) {
			emit(EV_KEY, KEY_ENTER, 1);
			emit(EV_SYN, SYN_REPORT, 0);
		}
		else if (code == b && (value == 1 || value == 2)) {
			emit(EV_KEY, KEY_ESC, 1);
			emit(EV_SYN, SYN_REPORT, 0);
		}
		else {
			// reset press down
			emit(EV_KEY, KEY_ENTER, 0);
			emit(EV_SYN, SYN_REPORT, 0);

			emit(EV_KEY, KEY_ESC, 0);
			emit(EV_SYN, SYN_REPORT, 0);
            
            emit(EV_KEY, KEY_PLAYPAUSE, 0);
			emit(EV_SYN, SYN_REPORT, 0);
		}
	}

	// d-pad
	if (type == 3) {
		if (code == up && value == -1) {
			emit(EV_KEY, KEY_UP, 1);
			emit(EV_SYN, SYN_REPORT, 0);
		}
		else if (code == up && value == 0) {
			emit(EV_KEY, KEY_UP, 0);
			emit(EV_SYN, SYN_REPORT, 0);
		}

		if (code == down && value == 1) {
			emit(EV_KEY, KEY_DOWN, 1);
			emit(EV_SYN, SYN_REPORT, 0);

		}
		else if (code == down && value == 0) {
			emit(EV_KEY, KEY_DOWN, 0);
			emit(EV_SYN, SYN_REPORT, 0);
		}

		if (code == left && value == -1) {
			emit(EV_KEY, KEY_LEFT, 1);
			emit(EV_SYN, SYN_REPORT, 0);
		}
		else if (code == left && value == 0) {
			emit(EV_KEY, KEY_LEFT, 0);
			emit(EV_SYN, SYN_REPORT, 0);
		}

		if (code == right && value == 1) {
			emit(EV_KEY, KEY_RIGHT, 1);
			emit(EV_SYN, SYN_REPORT, 0);
		}
		else if (code == right && value == 0) {
			emit(EV_KEY, KEY_RIGHT, 0);
			emit(EV_SYN, SYN_REPORT, 0);
		}
	}
}

int main (int argc, char *argv[]) {
	int i = 0;

	uinp_fd = open("/dev/uinput", O_WRONLY | O_NONBLOCK);
	if (uinp_fd < 0) {
		printf("Unable to open /dev/uinput\n");
		return -1;
	}

	// Intialize the uInput device to NULL
	memset(&uidev, 0, sizeof(uidev));

	strncpy(uidev.name, "Fake Keyboard", UINPUT_MAX_NAME_SIZE);
	uidev.id.version = 1;
	uidev.id.bustype = BUS_USB;
	uidev.id.vendor = 0x1234; /* sample vendor */
	uidev.id.product = 0x5678; /* sample product */

	for (i = 0; i < 256; i++) {
		ioctl(uinp_fd, UI_SET_KEYBIT, i);
	}

	// Keys or Buttons
	ioctl(uinp_fd, UI_SET_EVBIT, EV_KEY);

	// Create input device into input sub-system
	write(uinp_fd, &uidev, sizeof(uidev));

	if (ioctl(uinp_fd, UI_DEV_CREATE)) {
		printf("Unable to create UINPUT device.");
		return -1;
	}

    int js;
    struct js_event event;
    struct axis_state axes[3] = {0};
    size_t axis;

if (argc > 10) {
    select_b = atoi(argv[1]);
    start = atoi(argv[2]);
    a = atoi(argv[3]);
    b = atoi(argv[4]);
    up = atoi(argv[5]);
    down = atoi(argv[6]);
    left = atoi(argv[7]);
    right = atoi(argv[8]);
    jsdevice = argv[9];
    oga_ver = argv[10];
    } else {
        print_usage();
    }

    js = open(jsdevice, O_RDONLY);

    if (js == -1)
        perror("Could not open joystick");

    /* This loop will exit if the controller is unplugged. */
    while (read_event(js, &event) == 0)
    {
        switch (event.type)
        {
            case JS_EVENT_BUTTON:
                //  printf("Button %u %s\n", event.number, event.value ? "pressed" : "released");
                // Since the OGA uses digital dpad instead of hat, we need to check for these to send the correct type
                if (strcmp(oga_ver,"1") == 0) { 
                 if  (event.number == up || event.number == left)
                    {
                        if (event.value == 1)
                        handle_event(3, event.number, -1);
                        else
                        handle_event(3, event.number, 0);
                        
                    } else if (event.number == down || event.number == right) {
                        if (event.value == 1)
                            handle_event(3, event.number, 1);
                        else
                            handle_event(3, event.number, 0);
                    } else {   
                        handle_event(1, event.number, event.value);
                    }
                } else {
                handle_event(1, event.number, event.value);
                }
                break;
            case JS_EVENT_AXIS:
                axis = get_axis_state(&event, axes);
                if (axis < 4) {
                    //   printf("Axis %zu at (%6d, %6d) %u %u\n", axis, axes[axis].x, axes[axis].y, event.number, event.value);
                    // Handle regulat joysticks that use hat as Dpad instead of buttons
                   if (event.number == 6 ) {
                    if (event.value < 0)
                        {
                            handle_event(3, left, -1); // left
                        }
                    else if (event.value > 0)
                        {
                            handle_event(3, right, 1); // right
                        }
                    else if (event.value == 0)
                        {
                        handle_event(3, left, 0); // left
                        handle_event(3, right, 0); // right
                        }
                } else if (event.number == 7) {
                    if (event.value < 0)
                        {
                            handle_event(3, up, -1); // up
                        }
                    else if (event.value > 0)
                        {
                            handle_event(3, down, 1); // down
                        }
                    else if (event.value == 0)
                        {
                        handle_event(3, up, 0); // left
                        handle_event(3, down, 0); // right
                        }
                    }
                }
                break;
            default:
                /* Ignore init events. */
                break;
        }
        fflush(stdout);
    }

    /*
	* Give userspace some time to read the events before we destroy the
	* device with UI_DEV_DESTROY.
	*/
	sleep(1);

	/* Clean up */
	ioctl(uinp_fd, UI_DEV_DESTROY);
	close(uinp_fd);
    close(js);
    return 0;
}
