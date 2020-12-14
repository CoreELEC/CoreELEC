#ifndef UINPUT_CTL_H_
#define UINPUT_CTL_H_
#include <sys/ioctl.h>
#include <linux/uinput.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
 
 
#define ALOGE(...) \
        printf(__VA_ARGS__); \
        printf("\n")
 
 
#define DEV_NAME      "/dev/joypad"

struct bt_adc {
	int channel;
	int old_value;
    int report_type;
    int max;
	int min;
	int cal;
	int scale;
	//bool invert;
};

struct joypad {
	int fuzz;
	int flat;
	int scale;
	int deadzone;
	struct bt_adc adcs[2];
};
struct joypad  joy;

void joystic_init(void);
void joystick_move(int xVal, int yVal);

#endif
