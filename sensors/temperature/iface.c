#include "iface.h"
#include <string.h>

void encode(uint8_t *dst, frame *src){
    memcpy(dst, src, FRAME_SIZE);
}

void decode(frame *dst, uint8_t *src){
    memcpy(dst, src, FRAME_SIZE);
}
