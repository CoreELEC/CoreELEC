/*
droid VNC server  - a vnc server for android
Copyright (C) 2011 Jose Pereira <onaips@gmail.com>

Other contributors:
  Oleksandr Andrushchenko <andr2000@gmail.com>

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

#include "framebuffer.h"
#include <limits.h>

int fbfd = -1;
unsigned int *fbmmap;

char framebuffer_device[256] = "/dev/fb0";

int roundUpToPageSize(int x);

struct fb_var_screeninfo scrinfo;
struct fb_fix_screeninfo fscrinfo;

void FB_setDevice(char *s) {
	strcpy(framebuffer_device,s);
}

void update_fb_info(void) {
	if (ioctl(fbfd, FBIOGET_VSCREENINFO, &scrinfo) != 0) {
		L("ioctl error\n");
		exit(EXIT_FAILURE);
	}
}

inline int roundUpToPageSize(int x) {
	return (x + (getpagesize()-1)) & ~(getpagesize()-1);
}

int initFB(void) {
	L("--Initializing framebuffer access method--\n");
	
	fbmmap = MAP_FAILED;
	
	if ((fbfd = open(framebuffer_device, O_RDWR)) == -1) {
		L("Cannot open fb device %s\n", framebuffer_device);
		return -1;
	}
	
	update_fb_info();
	
	if (ioctl(fbfd, FBIOGET_FSCREENINFO, &fscrinfo) != 0) {
		L("ioctl error\n");
		return -1;
	}
	
	L("line_length=%d xres=%d, yres=%d, xresv=%d, yresv=%d, xoffs=%d, yoffs=%d, bpp=%d\n",
		(int)fscrinfo.line_length,(int)scrinfo.xres, (int)scrinfo.yres,
		(int)scrinfo.xres_virtual, (int)scrinfo.yres_virtual,
		(int)scrinfo.xoffset, (int)scrinfo.yoffset,
		(int)scrinfo.bits_per_pixel);
	
	size_t size = scrinfo.yres_virtual;
	
	size_t fbSize = roundUpToPageSize(fscrinfo.line_length * size);
	
	fbmmap = mmap(NULL, fbSize , PROT_READ|PROT_WRITE ,  MAP_SHARED , fbfd, 0);
	
	if (fbmmap == MAP_FAILED) {
		L("mmap failed\n");
		return -1;
	}
	
	// always scale down by 2
	screenformat.width = scrinfo.xres / 2;
	screenformat.height = scrinfo.yres / 2;
	
	screenformat.bitsPerPixel = scrinfo.bits_per_pixel;
	screenformat.size = screenformat.width * screenformat.height * screenformat.bitsPerPixel / CHAR_BIT;
	screenformat.redShift = scrinfo.red.offset;
	screenformat.redMax = scrinfo.red.length;
	screenformat.greenShift = scrinfo.green.offset;
	screenformat.greenMax = scrinfo.green.length;
	screenformat.blueShift = scrinfo.blue.offset;
	screenformat.blueMax = scrinfo.blue.length;
	
	return 1;
}

void closeFB(void) {
	if(fbfd != -1)
	close(fbfd);
}

struct fb_var_screeninfo FB_getscrinfo(void) {
	return scrinfo;
}

unsigned int *readBufferFB(void) {
	update_fb_info();
	return fbmmap;
}
