/*
droid vnc server - Android VNC server
Copyright (C) 2009 Jose Pereira <onaips@gmail.com>

Modified for AML TV Boxes by kszaq <kszaquitto@gmail.com>

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 3 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef COMMON_H
#define COMMON_H

#ifndef __cplusplus

#include <dirent.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <sys/mman.h>
#include <sys/ioctl.h>

#include <sys/stat.h>
#include <sys/sysmacros.h>             /* For makedev() */

#include <fcntl.h>
#include <linux/fb.h>
#include <linux/input.h>

#include <assert.h>
#include <errno.h>

#include "screenFormat.h"

#define L(...) do{ printf(__VA_ARGS__); } while (0);
#endif

struct fbinfo {
	unsigned int version;
	unsigned int bpp;
	unsigned int size;
	unsigned int width;
	unsigned int height;
	unsigned int red_offset;
	unsigned int red_length;
	unsigned int blue_offset;
	unsigned int blue_length;
	unsigned int green_offset;
	unsigned int green_length;
	unsigned int alpha_offset;
	unsigned int alpha_length;
} __attribute__((packed));

typedef int (*init_fn_type)(void);
typedef int (*close_fn_type)(void);
typedef unsigned char * (*readfb_fn_type)(void);
typedef screenFormat (*getscreenformat_fn_type)(void);

void rotate(int);
int getCurrentRotation();
int isIdle();
void setIdle(int i);
void close_app();
screenFormat screenformat;

#define ARR_LEN(a) (sizeof(a)/sizeof(a)[0])

#endif
