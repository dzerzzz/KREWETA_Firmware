#include <stdio.h>
#include <stdlib.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <inttypes.h>
#include <time.h>
#include <unistd.h>
#include <signal.h>
#include "iface.h"

const char *fifo_path = "/tmp/uart_fifo";
int fifo_fd;

void sigint_handler (int signal) {
    printf("\nTerminating program...\n");
    close(fifo_fd);
    _exit(0);
}

int main() 
{
	signal(SIGINT, sigint_handler);

	// Create I2C bus
	int file;
	char *bus = "/dev/i2c-1";
	if ((file = open(bus, O_RDWR)) < 0) 
	{
		printf("Failed to open the bus. \n");
		exit(1);
	}
	// Get I2C device, MCP9808 I2C address is 0x18(24)
	ioctl(file, I2C_SLAVE, 0x18);

	// Select configuration register(0x01)
	// Continuous conversion mode, Power-up default(0x00, 0x00)
	char config[3] = {0};
	config[0] = 0x01;
	config[1] = 0x00;
	config[2] = 0x00;
	write(file, config, 3);
	// Select resolution rgister(0x08)
	// Resolution = +0.0625 / C(0x03)
	config[0] = 0x08;
	config[1] = 0x02;
	write(file, config, 2);
	sleep(1);

	// Read 2 bytes of data from register(0x05)
	// temp msb, temp lsb
	char reg[1] = {0x05};
	write(file, reg, 1);
	char data[2] = {0};

	time_t now = time(NULL);
	struct tm* local = localtime(&now);
	char filename[30];
	strftime(filename, sizeof(filename), "TMP_%d-%H-%M.txt", local);

	FILE *outputFile;
	outputFile = fopen(filename, "w");
	if(outputFile == NULL){
		printf("Error opening file.\n");
		return 1;
	}

	// Open FIFO buffer of UART handler 
	   if (access(fifo_path, F_OK) != 0) {
        printf("[ERROR] uart_handler is down, exiting\n");
        return -1;
    }
    mkfifo(fifo_path, 0666);
	
    fifo_fd = open(fifo_path, O_WRONLY);
	uint8_t frame_buffer[FRAME_SIZE];
	frame out;
    out.command = SENSOR_READING;

	while(1) {
		if(read(file, data, 2) != 2)
		{
			printf("Error : Input/Output error \n");
			return 1;
		}
		else
		{
			// Convert the data to 13-bits
			int temp = ((data[0] & 0x1F) * 256 + data[1]);
			if(temp > 4095)
			{
				temp -= 8192;
			}
			float cTemp = temp * 0.0625;

			// Output data to screen
			printf("Temperature in Celsius is : %.2f C \n", cTemp);
			time_t ts = time(NULL);
			printf("test %d %.2f\n", ts, cTemp);
			fprintf(outputFile, "%d %.2f\n", ts, cTemp); 

			// Send to UART handler
			out.value = cTemp;
    		encode(frame_buffer, &out);
			write(fifo_fd, frame_buffer, FRAME_SIZE);
		}
		sleep(1);
		fflush(outputFile);	
	}
}
