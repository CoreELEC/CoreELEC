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

#include "common.h"
#include "framebuffer.h"

#include "common.h"
#include "newinput.h"

#include <rfb/rfb.h>
#include <rfb/keysym.h>

#include <time.h>

#define CONCAT2(a,b) a##b
#define CONCAT2E(a,b) CONCAT2(a,b)
#define CONCAT3(a,b,c) a##b##c
#define CONCAT3E(a,b,c) CONCAT3(a,b,c)

char VNC_PASSWORD[256] = "";
int VNC_PORT = 5900;

unsigned int *cmpbuf;
unsigned int *vncbuf;

static rfbScreenInfoPtr vncscr;

uint32_t idle = 1;
uint32_t standby = 1;

//reverse connection
char *rhost = NULL;
int rport = 5500;

void (*update_screen)(void) = NULL;

#define PIXEL_TO_VIRTUALPIXEL_FB(i,j) ((j + scrinfo.yoffset) * scrinfo.xres_virtual + i + scrinfo.xoffset)
#define PIXEL_TO_VIRTUALPIXEL(i,j) ((j * screenformat.width) + i)

#define OUT 32
#include "updateScreen.c"
#undef OUT

void setIdle(int i) {
	idle=i;
}

ClientGoneHookPtr clientGone(rfbClientPtr cl) {
	return 0;
}

rfbNewClientHookPtr clientHook(rfbClientPtr cl) {
	cl->clientGoneHook=(ClientGoneHookPtr)clientGone;
	
	return RFB_CLIENT_ACCEPT;
}


void initVncServer(int argc, char **argv) {
	vncbuf = calloc(screenformat.width * screenformat.height, screenformat.bitsPerPixel/CHAR_BIT);
	cmpbuf = calloc(screenformat.width * screenformat.height, screenformat.bitsPerPixel/CHAR_BIT);
	
	assert(vncbuf != NULL);
	assert(cmpbuf != NULL);
	
	vncscr = rfbGetScreen(&argc, argv, screenformat.width, screenformat.height, 0 /* not used */ , 3,  screenformat.bitsPerPixel/CHAR_BIT);
	
	assert(vncscr != NULL);
	
	vncscr->desktopName = "CoreELEC";
	vncscr->frameBuffer =(char *)vncbuf;
	vncscr->port = VNC_PORT;
	vncscr->kbdAddEvent = dokey;
	vncscr->newClientHook = (rfbNewClientHookPtr)clientHook;
	
	if (strcmp(VNC_PASSWORD, "") != 0) {
		char **passwords = (char **)malloc(2 * sizeof(char **));
		passwords[0] = VNC_PASSWORD;
		passwords[1] = NULL;
		vncscr->authPasswdData = passwords;
		vncscr->passwordCheck = rfbCheckPasswordByList;
	}
	
	vncscr->serverFormat.redShift = screenformat.redShift;
	vncscr->serverFormat.greenShift = screenformat.greenShift;
	vncscr->serverFormat.blueShift = screenformat.blueShift;
	
	vncscr->serverFormat.redMax = (( 1 << screenformat.redMax) -1);
	vncscr->serverFormat.greenMax = (( 1 << screenformat.greenMax) -1);
	vncscr->serverFormat.blueMax = (( 1 << screenformat.blueMax) -1);
	
	vncscr->serverFormat.trueColour = TRUE;
	vncscr->serverFormat.bitsPerPixel = screenformat.bitsPerPixel;
	
	vncscr->alwaysShared = TRUE;
	
	rfbInitServer(vncscr);
	
	update_screen = update_screen_32;
	
	/* Mark as dirty since we haven't sent any updates at all yet. */
	rfbMarkRectAsModified(vncscr, 0, 0, vncscr->width, vncscr->height);
}

void close_app() {
	L("Cleaning up...\n");
	closeFB();
	closeUinput();
	exit(0); /* normal exit status */
}

void extractReverseHostPort(char *str) {
	int len = strlen(str);
	char *p;
	/* copy in to host */
	rhost = (char *) malloc(len + 1);
	if (! rhost) {
		L("reverse_connect: could not malloc string %d\n", len);
		exit(-1);
	}
	strncpy(rhost, str, len);
	rhost[len] = '\0';
	
	/* extract port, if any */
	if ((p = strrchr(rhost, ':')) != NULL) {
		rport = atoi(p + 1);
		if (rport < 0) {
			rport = -rport;
		}
		else if (rport < 20) {
			rport = 5500 + rport;
		}
		*p = '\0';
	}
}

void printUsage(char **argv) {
	L("\naml-server [parameters]\n"
		"-f <device>\t- Framebuffer device\n"
		"-h\t\t- Print this help\n"
		"-p <password>\t- Password to access server\n"
		"-R <host:port>\t- Host for reverse connection\n");
}

int main(int argc, char **argv) {
	long usec;
	
	if(argc > 1) {
		int i = 1;
		while(i < argc) {
		if(*argv[i] == '-') {
			switch(*(argv[i] + 1)) {
				case 'h':
					printUsage(argv);
					exit(0);
					break;
				case 'p':
					i++;
					strcpy(VNC_PASSWORD,argv[i]);
					break;
				case 'f':
					i++;
					FB_setDevice(argv[i]);
					break;
				case 'P':
					i++;
					VNC_PORT=atoi(argv[i]);
					break;
				case 'R':
					i++;
					extractReverseHostPort(argv[i]);
					break;
			}
		}
		i++;
		}
	}
	
	L("Initializing grabber method...\n");
	initFB();
	
	L("Initializing virtual keyboard...\n");
	initUinput();
	
	L("Initializing VNC server:\n");
	L("	width:  %d\n", (int)screenformat.width);
	L("	height: %d\n", (int)screenformat.height);
	L("	bpp:    %d\n", (int)screenformat.bitsPerPixel);
	L("	port:   %d\n", (int)VNC_PORT);
	
	L("Colourmap_rgba=%d:%d:%d    length=%d:%d:%d\n", screenformat.redShift, screenformat.greenShift, screenformat.blueShift,
		screenformat.redMax, screenformat.greenMax, screenformat.blueMax);
	
	initVncServer(argc, argv);
	
	if (rhost) {
		rfbClientPtr cl;
		cl = rfbReverseConnection(vncscr, rhost, rport);
		if (cl == NULL) {
			char *str = malloc(255 * sizeof(char));
			L("Couldn't connect to remote host: %s\n",rhost);
			free(str);
		}
		else {
			cl->onHold = FALSE;
			rfbStartOnHoldClient(cl);
		}
	}
	
	
	while (1) {
		usec = (vncscr->deferUpdateTime + standby) * 1000;
		rfbProcessEvents(vncscr, usec);
		if (idle)
			standby = 100;
		else
			standby = 10;
		
		if (vncscr->clientHead != NULL)
			update_screen();
	}
	
	close_app();
}
