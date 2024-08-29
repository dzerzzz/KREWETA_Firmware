#include <inttypes.h>

#define FRAME_SIZE sizeof(frame)

typedef enum COMMANDS {
    SET_TARGET_TEMPERATURE,
    SENSOR_READING
} COMMANDS;

typedef struct frame {
    COMMANDS command;   //4B
    float value;        //4B
} frame;

void encode(uint8_t *dst, frame *src);
void decode(frame *dst, uint8_t *src);
