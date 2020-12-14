#include <stdio.h>   /* Standard input/output definitions */
#include <string.h>  /* String function definitions */
#include <unistd.h>  /* UNIX standard function definitions */
#include <fcntl.h>   /* File control definitions */
#include <sys/time.h>    /* To calulate time taken for operaton */
//#include <time.h>
#include <errno.h>   /* Error number definitions */
#include <termios.h> /* terminal control definitions */
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>

#define ERROR -1
#define SUCCESS 1

#define UART_DEV_NAME   "/dev/ttyS0"
#define TIOCM_LOOP      0x8000
#define TIOCMSET        0x5418

//static int bufsize = 4096 ;             /* buffer of size 4K */
//static int bufsize = 2048 ;             /* buffer of size 2K */
//static int bufsize = 320 ;             /* buffer of size 320 */
static int bufsize = 20 ;             /* buffer of size 20 */

/* contains varibles required for UART PORT TESTING */
struct uart_test {
    int fd;                                 		/* varibale to store the port file descriptor value */
    long int baudrate;             		 	        /* baudrate to be set for Tx and Rx */
    int flow_cntrl;  		        		/* flow control data */
    struct timeval start_time, end_time,diff_time;  	/* used to calulate time intervals */
	unsigned int max_delay;					/* max delay between send bursts */
	unsigned int random_enable;				/* determines if data packets sent are fixed or "random" size */
};

struct termios oldtio;

int fd1,fd2;
struct uart_test ut;
char tx_rx;
int read_flag;

int writeport(int *fd, unsigned char *chars,int len);

int readport(int *fd, unsigned char *result);

int getbaud(int fd);

long int getbaud_flag(long int baudrate);

int initport(int fd,long int baudrate,int flow_ctrl);

void close_port();

void signalHandler();

int get_rand_value(int max);
