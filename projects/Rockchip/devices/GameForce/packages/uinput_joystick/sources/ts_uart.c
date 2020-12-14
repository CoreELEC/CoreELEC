#include "common.h"
#include "uinput-ctl.h"

/*
*	v1.0 start to use version to manageme the code
*
*
*
*/


int main(int argc, char **argv) {
	int i = 0, rd = 0;
	fd_set readFds;
	int retval;
	struct timeval respTime;
	unsigned char input_buf[bufsize],output_buf[bufsize];
	//int size=0;
	int sendsize,sleeptime=0;
	int j,error=0;
	int firsttime = 1;
	int x = 0,y = 0;


	/* to handle ctrl +c */
	if(signal(SIGINT, signalHandler) == SIG_ERR) {
		printf("ERROR : Failed to register signal handler\n");
		exit(1);
	}
	

	ut.baudrate = 115200;
	ut.flow_cntrl = 0;
	ut.max_delay = 0;
	ut.random_enable = 0;
	printf("open default uart: %s\n", UART_DEV_NAME);
	ut.fd = open(UART_DEV_NAME, O_RDWR | O_NOCTTY | O_NDELAY | O_NONBLOCK );


	if (ut.fd == -1) {
		printf("open_port: Unable to open port - %s ", UART_DEV_NAME);
		exit(1);
	} else {
		printf("open_port: open port - %s ut.fd = %d", UART_DEV_NAME, ut.fd);
		fcntl(ut.fd, F_SETFL, 0);
    }
    
	printf("\n Existing port baud=%d", getbaud(ut.fd));
	/* save current port settings */
	tcgetattr(ut.fd, &oldtio);
	initport(ut.fd, ut.baudrate, ut.flow_cntrl);
	printf("\n Configured port for New baud=%d\n", getbaud(ut.fd));


	read_flag = 0;
	while(1) {
	 	FD_ZERO(&readFds);
		FD_SET(ut.fd, &readFds);
		respTime.tv_sec =  10;
		respTime.tv_usec = 0;

		/* Sleep for command timeout and expect the response to be ready. */
		retval = select(FD_SETSIZE, &readFds, NULL, NULL, &respTime);
		if (retval == 0 ) {
			if (read_flag == 0) {
				printf("\n Waited until timeout; no data was available to read. Exiting... \n");
				#ifdef TEST_FILE
				if (unlink("mcu") == -1)
					printf("\n Failed to delete the file %s \n",argv[2]);
				#endif
				close_port();
			}
			else {
				//printf("\n select() timed out... waiting for more data\n");
				continue; /*skip this loop because FD_SET doesn't have any data */
			}
		}
		else if (retval == ERROR)
			printf("\n select: error :: %d\n",retval);

		/* Read data, abort if read fails. */
		if(FD_ISSET(ut.fd, &readFds) != 0)  {
			//fcntl(ut.fd, F_SETFL, FNDELAY);
		    /*	printf("\n entering readport func \n"); */
			if(read_flag == 0) {
				gettimeofday(&ut.start_time, NULL);			
				read_flag = 1;
				printf("\n Reading data from ttyS0 port ...\n");
			}
			rd = readport(&ut.fd, output_buf);
			if (ERROR == rd) {
				printf("Read Port failed\n");
				close_port();
			}
		}
		if(rd == 0)
			break;
		//size += rd;

		#if 1
		x = (output_buf[2]<<8) + output_buf[3];
		y = (output_buf[4]<<8) + output_buf[5];
		if(firsttime) {
			if ((x < 1000) || (x > 2500) || (y < 1000) || (y > 2500))
				continue;//init failed
			joy.adcs[0].cal = x;
			joy.adcs[1].cal = y;
			firsttime = 0;					
			joystic_init();
			printf("\n Default value: x cal: %d  y cal: %d \n", joy.adcs[0].cal, joy.adcs[1].cal);
		}
		if((x < 300) || (x >3500) || (y < 300) || (y >3500))
			continue;//filter out
		//printf("\n get x: %d  y: %d \n", x, y);
		if ((x < joy.adcs[0].cal + joy.deadzone) && (x > joy.adcs[0].cal - joy.deadzone))
			x = joy.adcs[0].cal;
		
		if ((y < joy.adcs[1].cal + joy.deadzone) && (y > joy.adcs[1].cal - joy.deadzone))
			y = joy.adcs[1].cal;
		
		x = x - joy.adcs[0].cal;
		x = x > joy.adcs[0].max ? joy.adcs[0].max : x;
		x = x < joy.adcs[0].min ? joy.adcs[0].min : x;
		
		y = y - joy.adcs[1].cal;
		y = y > joy.adcs[1].max ? joy.adcs[1].max : y;
		y = y < joy.adcs[1].min ? joy.adcs[1].min : y;
		
		x = x * (-1);
		//y = y;				
		//printf("\n get x: %d  y: %d \n", x, y);
		joystick_move(x, y);
		#endif
		
		#if 0
		printf("\n Read data:\n");
		for(i=0; i<11; i++)
			printf("%02x\t", output_buf[i]);
		#endif
		
		memset(output_buf,0,bufsize);
	}
	gettimeofday(&ut.end_time,NULL);
	//printf("\n Total read %d bytes from port \n",size);
	FD_CLR(ut.fd, &readFds);	
			

	timersub(&ut.end_time,&ut.start_time,&ut.diff_time);
	ut.diff_time.tv_sec -= 5;
    printf("\n Time taken %08ld sec, %08ld usec\n\n ",ut.diff_time.tv_sec,ut.diff_time.tv_usec);
    /* restore th old port settings. */
	close_port();
	return 0;
}


