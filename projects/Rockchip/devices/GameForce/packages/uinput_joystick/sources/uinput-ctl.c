#include <errno.h>
#include <stdio.h>
#include <string.h>
 
#include "uinput-ctl.h" 
 
static int fd = -1;
 
static int open_joystick_device()
{
    if (fd > 0)
    {
        ALOGE("virtual joystick has been installed already.");
        return fd;
    }
 
    //open joystick device
    if ((fd = open(DEV_NAME, O_WRONLY | O_NDELAY)) <= 0)
    {
        ALOGE("open joypad(%s) failed.", DEV_NAME);
        fd = -1;
    }
 

 	ALOGE("open joystick device fd = %d", fd);
	
    return fd;
}
 
static void close_joystick_device()
{
    if (fd > 0)
    {
        close(fd);
        fd = -1;
    }
}

void joystic_init(void)
{
	ALOGE("joystic_init ...");
	if (fd <= 0)
    {
    	joy.deadzone = 20;
		joy.adcs[0].max = 899;
		joy.adcs[0].min = -900;
		joy.adcs[1].max = 899;
		joy.adcs[1].min = -900;
		
        open_joystick_device();
    }
}

void joystick_move(int xVal, int yVal)
{
    if (fd > 0)
    {
        struct input_event event;
        memset(&event, 0, sizeof(event));
 
        //x coordinate
        gettimeofday(&event.time, 0);
        event.type = EV_ABS;
        event.code = ABS_RX;
        event.value = xVal;
        write(fd, &event, sizeof(event));
 
        //y coordinate
        event.type  = EV_ABS;
        event.code  = ABS_RY;
        event.value = yVal;
        write(fd, &event, sizeof(event));
 
        //execute move event
        event.type  = EV_SYN;
        event.code  = SYN_REPORT;
        event.value = 0;
        write(fd, &event, sizeof(event));
    }
    else
    {
        ALOGE("invalid device file handler.");
    }
}
