/*
droid VNC server  - a vnc server for android
Copyright (C) 2011 Jose Pereira <onaips@gmail.com>

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

#define OUT_T CONCAT3E(uint,OUT,_t)
#define FUNCTION CONCAT2E(update_screen_,OUT)

void FUNCTION(void) {
	int i, j;
	int offset = 0, pixelToVirtual;
	OUT_T* a;
	OUT_T* b = 0;
	struct fb_var_screeninfo scrinfo; //we'll need this to detect double FB on framebuffer
	
	scrinfo = FB_getscrinfo();
	b = (OUT_T*) readBufferFB();
	
	a = (OUT_T*)cmpbuf;
	int max_x = -1, max_y = -1, min_x = 99999, min_y = 99999;
	idle = 1;
	
	for (j = 0; j < vncscr->height; j++) {
		offset = j * vncscr->width;
		
		for (i = 0; i < vncscr->width; i++) {
			
			// multiply by 2 for scaling
			pixelToVirtual = PIXEL_TO_VIRTUALPIXEL_FB(i * 2, j * 2);
			
			if (a[i + offset]!=b[pixelToVirtual]) {
				a[i + offset]=b[pixelToVirtual];
				if (i>max_x) max_x=i;
				if (i<min_x) min_x=i;
				
				if (j>max_y) max_y=j;
				if (j<min_y) min_y=j;
				
				idle = 0;
			}
		}
	}
	
	if (!idle) {
		memcpy(vncbuf, a, screenformat.width * screenformat.height * screenformat.bitsPerPixel / CHAR_BIT);
		
		min_x--;
		min_x--;
		max_x++;
		max_y++;
		
		rfbMarkRectAsModified(vncscr, min_x, min_y, max_x, max_y);
	}
}
