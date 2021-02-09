/*
 * $id$
 *
 * Tests the force feedback driver
 * Copyright 2001-2002 Johann Deneux <deneux@ifrance.com>
 */

/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301 USA.
 *
 * You can contact the author by email at this address:
 * Johann Deneux <deneux@ifrance.com>
 */

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <linux/input.h>

#include "bitmaskros.h"


#define N_EFFECTS 6

char* effect_names[] = {
	"Sine vibration",
	"Constant Force",
	"Spring Condition",
	"Damping Condition",
	"Strong Rumble",
	"Weak Rumble"
};

int main(int argc, char** argv)
{
	struct ff_effect effects[N_EFFECTS];
	struct input_event play, stop, gain;
	int fd;
	const char * device_file_name = "/dev/input/event2";
	unsigned char relFeatures[1 + REL_MAX/8/sizeof(unsigned char)];
	unsigned char absFeatures[1 + ABS_MAX/8/sizeof(unsigned char)];
	unsigned char ffFeatures[1 + FF_MAX/8/sizeof(unsigned char)];
	int n_effects;	/* Number of effects the device can play at the same time */
	int i,j,k;
    int mode = 0;

	printf("Force feedback test program.\n");
	printf("HOLD FIRMLY YOUR WHEEL OR JOYSTICK TO PREVENT DAMAGES\n\n");

	for (i=1; i<argc; ++i) {
		if (strncmp(argv[i], "--help", 64) == 0) {
			printf("Usage: %s /dev/input/eventXX\n", argv[0]);
			printf("Tests the force feedback driver\n");
			exit(1);
		} else if(strncmp(argv[i], "left", 4) == 0) {
            mode = 0;
        } else if(strncmp(argv[i], "right", 5) == 0) {
            mode = 1;
        }
		/*else {
			device_file_name = argv[i];
		}*/
	}

	/* Open device */
	fd = open(device_file_name, O_RDWR);
	if (fd == -1) {
		perror("Open device file");
		exit(1);
	}
	printf("Device %s opened\n", device_file_name);

	/* Query device */
	printf("Features:\n");

	/* Absolute axes */
	memset(absFeatures, 0, sizeof(absFeatures)*sizeof(unsigned char));
	if (ioctl(fd, EVIOCGBIT(EV_ABS, sizeof(absFeatures)*sizeof(unsigned char)), absFeatures) == -1) {
		perror("Ioctl absolute axes features query");
		exit(1);
	}

	printf("  * Absolute axes: ");

	if (testBit(ABS_X, absFeatures)) printf("X, ");
	if (testBit(ABS_Y, absFeatures)) printf("Y, ");
	if (testBit(ABS_Z, absFeatures)) printf("Z, ");
	if (testBit(ABS_RX, absFeatures)) printf("RX, ");
	if (testBit(ABS_RY, absFeatures)) printf("RY, ");
	if (testBit(ABS_RZ, absFeatures)) printf("RZ, ");
	if (testBit(ABS_THROTTLE, absFeatures)) printf("Throttle, ");
	if (testBit(ABS_RUDDER, absFeatures)) printf("Rudder, ");
	if (testBit(ABS_WHEEL, absFeatures)) printf("Wheel, ");
	if (testBit(ABS_GAS, absFeatures)) printf("Gas, ");
	if (testBit(ABS_BRAKE, absFeatures)) printf("Brake, ");
	if (testBit(ABS_HAT0X, absFeatures)) printf("Hat 0 X, ");
	if (testBit(ABS_HAT0Y, absFeatures)) printf("Hat 0 Y, ");
	if (testBit(ABS_HAT1X, absFeatures)) printf("Hat 1 X, ");
	if (testBit(ABS_HAT1Y, absFeatures)) printf("Hat 1 Y, ");
	if (testBit(ABS_HAT2X, absFeatures)) printf("Hat 2 X, ");
	if (testBit(ABS_HAT2Y, absFeatures)) printf("Hat 2 Y, ");
	if (testBit(ABS_HAT3X, absFeatures)) printf("Hat 3 X, ");
	if (testBit(ABS_HAT3Y, absFeatures)) printf("Hat 3 Y, ");
	if (testBit(ABS_PRESSURE, absFeatures)) printf("Pressure, ");
	if (testBit(ABS_DISTANCE, absFeatures)) printf("Distance, ");
	if (testBit(ABS_TILT_X, absFeatures)) printf("Tilt X, ");
	if (testBit(ABS_TILT_Y, absFeatures)) printf("Tilt Y, ");
	if (testBit(ABS_TOOL_WIDTH, absFeatures)) printf("Tool width, ");
	if (testBit(ABS_VOLUME, absFeatures)) printf("Volume, ");
	if (testBit(ABS_MISC, absFeatures)) printf("Misc ,");

	printf("\n    [");
	for (i=0; i<sizeof(absFeatures)/sizeof(unsigned char);i++)
	    printf("%02X ", absFeatures[i]);
	printf("]\n");

	/* Relative axes */
	memset(relFeatures, 0, sizeof(relFeatures)*sizeof(unsigned char));
	if (ioctl(fd, EVIOCGBIT(EV_REL, sizeof(relFeatures)*sizeof(unsigned char)), relFeatures) == -1) {
		perror("Ioctl relative axes features query");
		exit(1);
	}

	printf("  * Relative axes: ");

	if (testBit(REL_X, relFeatures)) printf("X, ");
	if (testBit(REL_Y, relFeatures)) printf("Y, ");
	if (testBit(REL_Z, relFeatures)) printf("Z, ");
	if (testBit(REL_RX, relFeatures)) printf("RX, ");
	if (testBit(REL_RY, relFeatures)) printf("RY, ");
	if (testBit(REL_RZ, relFeatures)) printf("RZ, ");
	if (testBit(REL_HWHEEL, relFeatures)) printf("HWheel, ");
	if (testBit(REL_DIAL, relFeatures)) printf("Dial, ");
	if (testBit(REL_WHEEL, relFeatures)) printf("Wheel, ");
	if (testBit(REL_MISC, relFeatures)) printf("Misc, ");

	printf("\n    [");
	for (i=0; i<sizeof(relFeatures)/sizeof(unsigned char);i++)
	    printf("%02X ", relFeatures[i]);
	printf("]\n");

	/* Force feedback effects */
	memset(ffFeatures, 0, sizeof(ffFeatures)*sizeof(unsigned char));
	if (ioctl(fd, EVIOCGBIT(EV_FF, sizeof(ffFeatures)*sizeof(unsigned char)), ffFeatures) == -1) {
		perror("Ioctl force feedback features query");
		exit(1);
	}

	printf("  * Force feedback effects types: ");

	if (testBit(FF_CONSTANT, ffFeatures)) printf("Constant, ");
	if (testBit(FF_PERIODIC, ffFeatures)) printf("Periodic, ");
	if (testBit(FF_RAMP, ffFeatures)) printf("Ramp, ");
	if (testBit(FF_SPRING, ffFeatures)) printf("Spring, ");
	if (testBit(FF_FRICTION, ffFeatures)) printf("Friction, ");
	if (testBit(FF_DAMPER, ffFeatures)) printf("Damper, ");
	if (testBit(FF_RUMBLE, ffFeatures)) printf("Rumble, ");
	if (testBit(FF_INERTIA, ffFeatures)) printf("Inertia, ");
	if (testBit(FF_GAIN, ffFeatures)) printf("Gain, ");
	if (testBit(FF_AUTOCENTER, ffFeatures)) printf("Autocenter, ");

	printf("\n    Force feedback periodic effects: ");

	if (testBit(FF_SQUARE, ffFeatures)) printf("Square, ");
	if (testBit(FF_TRIANGLE, ffFeatures)) printf("Triangle, ");
	if (testBit(FF_SINE, ffFeatures)) printf("Sine, ");
	if (testBit(FF_SAW_UP, ffFeatures)) printf("Saw up, ");
	if (testBit(FF_SAW_DOWN, ffFeatures)) printf("Saw down, ");
	if (testBit(FF_CUSTOM, ffFeatures)) printf("Custom, ");

	printf("\n    [");
	for (i=0; i<sizeof(ffFeatures)/sizeof(unsigned char);i++)
	    printf("%02X ", ffFeatures[i]);
	printf("]\n");

	printf("  * Number of simultaneous effects: ");

	if (ioctl(fd, EVIOCGEFFECTS, &n_effects) == -1) {
		perror("Ioctl number of effects");
	}

	printf("%d\n\n", n_effects);

	/* Set master gain to 75% if supported */
	if (testBit(FF_GAIN, ffFeatures)) {
		memset(&gain, 0, sizeof(gain));
		gain.type = EV_FF;
		gain.code = FF_GAIN;
		gain.value = 0xFFFF; /*0xC000 [0, 0xFFFF]) */

		printf("Setting master gain to 75->100%% ... ");
		fflush(stdout);
		if (write(fd, &gain, sizeof(gain)) != sizeof(gain)) {
		  perror("Error:");
		} else {
		  printf("OK\n");
		}
	}

	/* download a periodic sinusoidal effect */
	memset(&effects[0],0,sizeof(effects[0]));
	effects[0].type = FF_PERIODIC;
	effects[0].id = -1;
	effects[0].u.periodic.waveform = FF_SINE;
	effects[0].u.periodic.period = 100;	/* 0.1 second */
	effects[0].u.periodic.magnitude = 0x7fff;	/* 0.5 * Maximum magnitude */
	effects[0].u.periodic.offset = 0;
	effects[0].u.periodic.phase = 0;
	effects[0].direction = 0x4000;	/* Along X axis */
	effects[0].u.periodic.envelope.attack_length = 1000;
	effects[0].u.periodic.envelope.attack_level = 0x7fff;
	effects[0].u.periodic.envelope.fade_length = 1000;
	effects[0].u.periodic.envelope.fade_level = 0x7fff;
	effects[0].trigger.button = 0;
	effects[0].trigger.interval = 0;
	effects[0].replay.length = 20000;  /* 20 seconds */
	effects[0].replay.delay = 1000;

	printf("Uploading effect #0 (Periodic sinusoidal) ... ");
	fflush(stdout);
	if (ioctl(fd, EVIOCSFF, &effects[0]) == -1) {
		perror("Error:");
	} else {
		printf("OK (id %d)\n", effects[0].id);
	}
	
	/* download a constant effect */
	effects[1].type = FF_CONSTANT;
	effects[1].id = -1;
	effects[1].u.constant.level = 0x2000;	/* Strength : 25 % */
	effects[1].direction = 0x6000;	/* 135 degrees */
	effects[1].u.constant.envelope.attack_length = 1000;
	effects[1].u.constant.envelope.attack_level = 0x1000;
	effects[1].u.constant.envelope.fade_length = 1000;
	effects[1].u.constant.envelope.fade_level = 0x1000;
	effects[1].trigger.button = 0;
	effects[1].trigger.interval = 0;
	effects[1].replay.length = 20000;  /* 20 seconds */
	effects[1].replay.delay = 0;

	printf("Uploading effect #1 (Constant) ... ");
	fflush(stdout);
	if (ioctl(fd, EVIOCSFF, &effects[1]) == -1) {
		perror("Error");
	} else {
		printf("OK (id %d)\n", effects[1].id);
	}

	/* download a condition spring effect */
	effects[2].type = FF_SPRING;
	effects[2].id = -1;
	effects[2].u.condition[0].right_saturation = 0x7fff;
	effects[2].u.condition[0].left_saturation = 0x7fff;
	effects[2].u.condition[0].right_coeff = 0x2000;
	effects[2].u.condition[0].left_coeff = 0x2000;
	effects[2].u.condition[0].deadband = 0x0;
	effects[2].u.condition[0].center = 0x0;
	effects[2].u.condition[1] = effects[2].u.condition[0];
	effects[2].trigger.button = 0;
	effects[2].trigger.interval = 0;
	effects[2].replay.length = 20000;  /* 20 seconds */
	effects[2].replay.delay = 0;

	printf("Uploading effect #2 (Spring) ... ");
	fflush(stdout);
	if (ioctl(fd, EVIOCSFF, &effects[2]) == -1) {
		perror("Error");
	} else {
		printf("OK (id %d)\n", effects[2].id);
	}

	/* download a condition damper effect */
	effects[3].type = FF_DAMPER;
	effects[3].id = -1;
	effects[3].u.condition[0].right_saturation = 0x7fff;
	effects[3].u.condition[0].left_saturation = 0x7fff;
	effects[3].u.condition[0].right_coeff = 0x2000;
	effects[3].u.condition[0].left_coeff = 0x2000;
	effects[3].u.condition[0].deadband = 0x0;
	effects[3].u.condition[0].center = 0x0;
	effects[3].u.condition[1] = effects[3].u.condition[0];
	effects[3].trigger.button = 0;
	effects[3].trigger.interval = 0;
	effects[3].replay.length = 20000;  /* 20 seconds */
	effects[3].replay.delay = 0;

	printf("Uploading effect #3 (Damper) ... ");
	fflush(stdout);
	if (ioctl(fd, EVIOCSFF, &effects[3]) == -1) {
		perror("Error");
	} else {
		printf("OK (id %d)\n", effects[3].id);
	}

	/* a strong rumbling effect */
	effects[4].type = FF_RUMBLE;
	effects[4].id = -1;
	effects[4].u.rumble.strong_magnitude = 0x7000;// > 0x100
	effects[4].u.rumble.weak_magnitude = mode;//0 left  1 right
	effects[4].replay.length = 3000;//stop time 5000
	effects[4].replay.delay = 0;//1000

	printf("Uploading effect #4 (Strong rumble, with heavy motor) ... ");
	fflush(stdout);
	if (ioctl(fd, EVIOCSFF, &effects[4]) == -1) {
		perror("Error");
	} else {
		printf("OK (id %d)\n", effects[4].id);
	}

	/* a weak rumbling effect */
	effects[5].type = FF_RUMBLE;
	effects[5].id = -1;
	effects[5].u.rumble.strong_magnitude = mode;//0 left 1 right
	effects[5].u.rumble.weak_magnitude = 0xc000;// > 0x100
	effects[5].replay.length = 1000;//stop time 5000
	effects[5].replay.delay = 0;

	printf("Uploading effect #5 (Weak rumble, with light motor) ... ");
	fflush(stdout);
	if (ioctl(fd, EVIOCSFF, &effects[5]) == -1) {
		perror("Error");
	} else {
		printf("OK (id %d)\n", effects[5].id);
	}


	i = 4;
    k = 0;
	/* Ask user what effects to play */
	do {
		//printf("Enter effect number, -1 to exit\n");
		printf("Do Strong & Weak rumble with heavy or light motor ...\n");
		//i = -1;
        for(j=0; j<100000; j++) {
            if(j%10000 == 0)
                printf("10000 print j ...\n");
        }
		
        //if (scanf("%d", &i) == EOF) {
		if (i<0) {
			printf("Read error\n");
		}
		else if (i >= 0 && i < N_EFFECTS) {
			memset(&play,0,sizeof(play));
			play.type = EV_FF;
			play.code = effects[i].id;
			play.value = 1;
            
            printf("Number: %d\n", i);
			if (write(fd, (const void*) &play, sizeof(play)) == -1) {
				perror("Play effect");
				exit(1);
			}

			printf("Now Playing: %s\n", effect_names[i]);
		}
		else if (i == -2) {
			/* Crash test */
			int i = *((int *)0);
			printf("Crash test: %d\n", i);
		}
		else if (i != -1) {
			printf("No such effect\n");
		}
        k += 1;//only do once
	} while (k<=5000);
	//} while (i>=0);

	/* Stop the effects */
	printf("Stopping effects\n");
	for (i=0; i<N_EFFECTS; ++i) {
		memset(&stop,0,sizeof(stop));
		stop.type = EV_FF;
		stop.code =  effects[i].id;
		stop.value = 0;
        
		if (write(fd, (const void*) &stop, sizeof(stop)) == -1) {
			perror("");
			exit(1);
		}
	}
	
	printf("End exit ...\n");

	exit(0);
}
