#ifndef __RASPNETL4_H__
#define __RASPNETL4_H__

#include <stdint.h>

#pragma pack(1)
/// @brief Level 4 packet format / structure
typedef struct L4_packet
{
    uint8_t identification;
    uint8_t flags;
    uint8_t payload[251];
} RPL4_PACKET;

/// @brief Level 4 cache packet format / structure
typedef struct L4_cache_item
{
    uint8_t retry;
    uint8_t source;
    uint8_t destination;
    RPL4_PACKET packet;
} RPL4_CACHE_ITEM;

#define RPL4_FLAGS_ACK 0x1
#define RPL4_FLAGS_DGRAM 0x2
#define RPL4_FLAGS_FC 0xFC

void rpl4_send(uint8_t source, uint8_t destination, uint8_t flags, uint8_t *str);
int rpl4_receive(RPL4_PACKET *packetL4);
int rpl4_rcache_find(uint8_t identification);
void rpl4_ack_resend();

#endif
