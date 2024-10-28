#ifndef __RASPNETL2_H__
#define __RASPNETL2_H__

#include <avr/interrupt.h>
#include <string.h>

#ifndef PREAMBLE
#define PREAMBLE 0b01111110
#endif

typedef struct raspnet_frame_t
{
    uint8_t preamble;
    uint32_t crc;
    uint8_t size;
    uint8_t payload[255];
} RASPNET_FRAME;

void raspnetL2_send(char *str);
void raspnetL2_receive(RASPNET_FRAME *buffer);

#endif
