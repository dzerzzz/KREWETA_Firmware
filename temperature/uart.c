#include <stdio.h>
#include <stdlib.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <inttypes.h>
#include <time.h>
#include <termios.h>
#include <unistd.h>
#include <signal.h>
#include "iface.h"

#define UART_BAUD_RATE B9600

// uart FIFO file aka named pipe
const char *fifo_path = "/tmp/uart_fifo";
const char *uart_path = "/dev/ttyS0";
int uart_fd, fifo_fd;

void sigint_handler (int signal) {
    printf("\nTerminating program...\n");
	close(uart_fd);
    close(fifo_fd);
    remove(fifo_path);
    _exit(0);
}

int main() 
{
	signal(SIGINT, sigint_handler);

	uint8_t frame_buffer[FRAME_SIZE];
    mkfifo(fifo_path, 0666);

	if ((uart_fd = open(uart_path, O_RDWR | O_NOCTTY | O_NDELAY)) < 0) 
	{
		printf("Failed to open UART. \n");
		exit(1);
	}

	struct termios termOptions;
	tcgetattr(uart_fd, &termOptions);
	termOptions.c_cflag = UART_BAUD_RATE | CS8 | CLOCAL | CREAD;
	termOptions.c_iflag = IGNPAR;
	termOptions.c_oflag = 0;
	termOptions.c_lflag = 0;
	tcflush(uart_fd, TCIFLUSH);

	if(tcsetattr(uart_fd, TCSANOW, &termOptions) < 0) {
		printf("ERROR: Serial setup failed\r\n");
		return 1;
	}

	// main loop of this process reads unix fifo buffer and sends data to the UART
	fifo_fd = open(fifo_path, O_RDONLY);
    while(1) {
		frame received;
        if(read(fifo_fd, frame_buffer, FRAME_SIZE) > 0) {
			decode(&received, frame_buffer);

        	// Print the read string and close
        	printf("Sending to UART\n\tcommand: %d\n\tvalue: %.2f\n\n", received.command, received.value);

			// send to UART
        	write(uart_fd, frame_buffer, FRAME_SIZE);
		}
		else {
			printf("Waiting for client\n");
			sleep(1);
		}
        
    }

	close(fifo_fd);
}
