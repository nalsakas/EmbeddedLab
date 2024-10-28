#ifndef __RASPNETL3_H__
#define __RASPNETL3_H__

#include <stdint.h>

#pragma pack(1)
/// @brief Level 3 packet format / structure
typedef struct raspnet_L3_packet_t
{
    uint8_t destination;
    uint8_t source;
    uint8_t payload[253];
} RPL3_PACKET;

void rpl3_send(uint8_t source, uint8_t destination, uint8_t *data);
int rpl3_receive(RPL3_PACKET *packet);

#endif
