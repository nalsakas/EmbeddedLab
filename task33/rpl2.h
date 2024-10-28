#ifndef __RASPNETL2_H__
#define __RASPNETL2_H__

#include <stdint.h>
#include "rpl3.h"

#ifndef PREAMBLE
#define PREAMBLE 0b01111110
#endif

#pragma pack(1)
/// @brief Level 2 frame format / structure
typedef struct raspnet_frame_t
{
    uint8_t preamble;
    uint32_t crc;
    uint8_t size;
    uint8_t payload[255];
} RPL2_FRAME;

extern RPL2_FRAME RPL2_RESEND_FRAME; //< Level 2 resend cache. Packets not destined to this machine will be forwarded.

void rpl2_send(RPL3_PACKET *packet);
void rpl2_resend();
void rpl2_relay(RPL2_FRAME *frame);
int rpl2_receive(RPL2_FRAME *frame);
int checkCRC(RPL2_FRAME *pFrame);

#endif
