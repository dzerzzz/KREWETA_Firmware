#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include "iface.h"

#define TEMP_MIN 25
#define TEMP_MAX 36

const char *fifo_path = "/tmp/uart_fifo";

//////////////////////////////////////////////////////////////////////////
// this lets you set the target temperature manually by running command:
//  $> ./set_temp 26.65
/////////////////////////////////////////////////////////////////////////

int main(int argc, char *argv[]) {
    if(argc != 2) {
        printf("[ERROR] set_temp accepts exactly one parameter\n");
        return -1;
    }

    float target_temperature = atof(argv[1]);
    if(target_temperature < TEMP_MIN || target_temperature > TEMP_MAX) {
        printf("[ERROR] given temperature outside safety margins, exiting.\n");
        return -1;
    }

    if(access(fifo_path, F_OK) != 0) {
        printf("[ERROR] uart_handler is down, exiting\n");
        return -1;
    }
    mkfifo(fifo_path, 0666);

    uint8_t frame_buffer[FRAME_SIZE];
    frame out;
    out.command = SENSOR_READING;
    out.value = target_temperature;
    encode(frame_buffer, &out);

    int fifo_fd = open(fifo_path, O_WRONLY);
    write(fifo_fd, frame_buffer, FRAME_SIZE);
    close(fifo_fd);

    // print out frame buffer
    for(int i=0; i<FRAME_SIZE; i++) {
        printf("%d: %d\n", i, frame_buffer[i]);
    }
    printf("sizeof(COMMAND): %d\n", sizeof(COMMANDS));
    printf("sizeof(float): %d\n", sizeof(float));
    printf("sizeof(frame): %d\n", sizeof(frame));

    return 0;
}