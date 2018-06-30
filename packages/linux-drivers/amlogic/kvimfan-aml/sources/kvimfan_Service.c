/*

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, write to the Free Software
	Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

*/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <sys/select.h>
#include <time.h>
#include <sys/ioctl.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/ipc.h>
#include <dirent.h>
#include <stdint.h>
#include <signal.h>
#include <time.h>

#define DEV_NAME        "kvimfan"
#define DRV_NAME	"/dev/" DEV_NAME

int kvim_fan_fd;

void kvim_fan_loop()
{
	char data[10];
	while(1) {
		read(kvim_fan_fd,&data,sizeof(data));
		sleep(10);
	}
}

void *kvim_fan_thread_handler(void *arg)
{
	kvim_fan_loop();
	pthread_exit(NULL);
}



int main(int argc, char *argv[])
{
	bool b;
	pthread_t kvim_fan_id;
	int ret;

	kvim_fan_fd = open(DRV_NAME, O_RDONLY);
	if (kvim_fan_fd < 0) {
		perror("Open device failed.\n");
		exit(1);
	}
	ret = pthread_create(&kvim_fan_id, NULL, kvim_fan_thread_handler, &b);
	if(ret != 0) {
		perror("Create kvimfan thread error\n");
		return ret;
	}
	pthread_join(kvim_fan_id, NULL);
	close(kvim_fan_fd);
	return 0;
}


