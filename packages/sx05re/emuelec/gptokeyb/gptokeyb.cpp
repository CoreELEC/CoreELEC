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
* Part of the code is from from https://github.com/krishenriksen/AnberPorts/blob/master/AnberPorts-Keyboard-Mouse/main.c (mostly the fake keyboard)
*
* Modified (badly) by: Shanti Gilbert for EmuELEC
* Modified further by: Nikolai Wuttke for EmuELEC (Added support for SDL and the SDLGameControllerdb.txt)
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
#include <sstream>

#include <SDL.h>


static int uinp_fd = -1;
struct uinput_user_dev uidev;
bool kill_mode = false;
bool openbor_mode = false;
char* AppToKill;
bool back_pressed = false;
bool start_pressed = false;
int back_jsdevice;
int start_jsdevice;

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

void emitKey(int code, bool is_pressed) {
    emit(EV_KEY, code, is_pressed ? 1 : 0);
    emit(EV_SYN, SYN_REPORT, 0);
}

int main(int argc, char *argv[]) {

if (argc > 1) {
    if (strcmp( argv[1], "openbor") == 0) {
    openbor_mode = true;
    } else {
    kill_mode = argv[1];
    AppToKill = argv[2];
    }
}

// We do not need fake keyboard in kill mode
if (kill_mode == false) {
    // printf("Running in Fake Keyboard mode\n");
    
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

    for (int i = 0; i < 256; i++) {
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
} //fake keyboard kill mode 

    // SDL initialization and main loop
    if (SDL_Init(SDL_INIT_GAMECONTROLLER) != 0) {
        printf("SDL_Init() failed: %s\n", SDL_GetError());
        return -1;
    }

    if (const char* db_file = SDL_getenv("SDL_GAMECONTROLLERCONFIG_FILE")) {
        SDL_GameControllerAddMappingsFromFile(db_file);
    }

    SDL_Event event;
    bool running = true;
    while (running) {
        if (!SDL_WaitEvent(&event)) {
            printf("SDL_WaitEvent() failed: %s\n", SDL_GetError());
            return -1;
        }

        switch (event.type) {
            case SDL_CONTROLLERBUTTONDOWN:
            case SDL_CONTROLLERBUTTONUP:
                {
                    
                    const bool is_pressed =
                        event.type == SDL_CONTROLLERBUTTONDOWN;
                
                if (kill_mode) {
                    // Kill mode
                    switch (event.cbutton.button) {
                            case SDL_CONTROLLER_BUTTON_GUIDE:
                                back_jsdevice = event.cdevice.which;
                                back_pressed = is_pressed;
                                break;

                            case SDL_CONTROLLER_BUTTON_START:
                                start_jsdevice = event.cdevice.which;
                                start_pressed = is_pressed;
                                break;
                        }

                        if (start_pressed && back_pressed) {
                            // printf("Killing: %s\n", AppToKill);
                            if (start_jsdevice == back_jsdevice) {
                                    system( (" killall  '"+std::string(AppToKill)+"' ").c_str() );
                                    system("show_splash.sh exit");
                                    sleep(3);
                                    if (system( (" pgrep '"+std::string(AppToKill)+"' ").c_str() ) == 0) { 
                                        printf("Forcefully Killing: %s\n", AppToKill);
                                        system( (" killall  -9 '"+std::string(AppToKill)+"' ").c_str() );
                                    }
                                exit(0);
                            }
                        }
                    } else if (openbor_mode) {
                        // Fake Openbor mode
                    switch (event.cbutton.button) {
                        case SDL_CONTROLLER_BUTTON_DPAD_LEFT:
                            emitKey(KEY_LEFT, is_pressed);
                            break;

                        case SDL_CONTROLLER_BUTTON_DPAD_UP:
                            emitKey(KEY_UP, is_pressed);
                            break;

                        case SDL_CONTROLLER_BUTTON_DPAD_RIGHT:
                            emitKey(KEY_RIGHT, is_pressed);
                            break;

                        case SDL_CONTROLLER_BUTTON_DPAD_DOWN:
                            emitKey(KEY_DOWN, is_pressed);
                            break;

                        case SDL_CONTROLLER_BUTTON_A:
                            emitKey(KEY_A, is_pressed);
                            break;

                        case SDL_CONTROLLER_BUTTON_B:
                            emitKey(KEY_D, is_pressed);
                            break;

                        case SDL_CONTROLLER_BUTTON_X:
                            emitKey(KEY_S, is_pressed);
                            break;

                        case SDL_CONTROLLER_BUTTON_Y:
                            emitKey(KEY_F, is_pressed);
                            break;

                        case SDL_CONTROLLER_BUTTON_LEFTSHOULDER:
                            emitKey(KEY_Z, is_pressed);
                            break;

                        case SDL_CONTROLLER_BUTTON_RIGHTSHOULDER:
                            emitKey(KEY_X, is_pressed);
                            break;

                        case SDL_CONTROLLER_BUTTON_BACK: // aka select
                            emitKey(KEY_F12, is_pressed);
                            break;

                        case SDL_CONTROLLER_BUTTON_START:
                            emitKey(KEY_ENTER, is_pressed);
                            break;
                        }
                    } else {
                         // Fake Keyboard mode
                    switch (event.cbutton.button) {
                        case SDL_CONTROLLER_BUTTON_DPAD_LEFT:
                            emitKey(KEY_LEFT, is_pressed);
                            break;

                        case SDL_CONTROLLER_BUTTON_DPAD_UP:
                            emitKey(KEY_UP, is_pressed);
                            break;

                        case SDL_CONTROLLER_BUTTON_DPAD_RIGHT:
                            emitKey(KEY_RIGHT, is_pressed);
                            break;

                        case SDL_CONTROLLER_BUTTON_DPAD_DOWN:
                            emitKey(KEY_DOWN, is_pressed);
                            break;

                        case SDL_CONTROLLER_BUTTON_A:
                            emitKey(KEY_ENTER, is_pressed);
                            break;

                        case SDL_CONTROLLER_BUTTON_B:
                            emitKey(KEY_ESC, is_pressed);
                            break;

                        case SDL_CONTROLLER_BUTTON_BACK: // aka select
                            emitKey(KEY_PLAYPAUSE, is_pressed);
                            break;

                        case SDL_CONTROLLER_BUTTON_START:
                            emitKey(KEY_ENTER, is_pressed);
                            break;
                    }
                } //kill mode
                }
                break;

            case SDL_CONTROLLERDEVICEADDED:
                SDL_GameControllerOpen(event.cdevice.which);
                break;

            case SDL_CONTROLLERDEVICEREMOVED:
                if (SDL_GameController* controller = SDL_GameControllerFromInstanceID(event.cdevice.which))
                {
                    SDL_GameControllerClose(controller);
                }
                break;

            case SDL_QUIT:
                running = false;
                break;
        }
    }

    SDL_Quit();

    /*
    * Give userspace some time to read the events before we destroy the
    * device with UI_DEV_DESTROY.
    */
    sleep(1);

    /* Clean up */
    ioctl(uinp_fd, UI_DEV_DESTROY);
    close(uinp_fd);
    return 0;
}
