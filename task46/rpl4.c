#include <avr/io.h>
#include <string.h>
#include <stdlib.h>
#include "rpl2.h"
#include "rpl3.h"
#include "uart.h"
#include "pins.h"
#include "api.h"
#include "rpl4.h"

uint8_t PACKET_IDENTIFICATION = 1;       ///< L4 packet identification counter
RPL4_CACHE_ITEM PACKET_SEND_CACHE[0];    ///< L4 packet cache for sended packets
RPL4_CACHE_ITEM PACKET_RECEIVE_CACHE[0]; ///< L4 packet cache for received packets

/// @brief Adds message to the outgoing cache. This function used internally by rpl4_send() function.
/// @param item Message to be added to the cache.
/// @return On success returns 0.
int rpl4_scache_add(RPL4_CACHE_ITEM *item)
{
    int empty_slot = 0;
    int i = 0;

    // find an empy slot
    for (i = 0; i < sizeof PACKET_SEND_CACHE / sizeof PACKET_SEND_CACHE[0]; i++)
    {
        if (PACKET_SEND_CACHE[i].retry == 0)
        {
            empty_slot = 1;
            break;
        }
    }

    // use empy slot if there is one
    if (empty_slot == 1)
    {
        memcpy(&PACKET_SEND_CACHE[i], item, sizeof *item);
    }
    else
    {
        // clear all cache
        for (int i = 0; i < sizeof PACKET_SEND_CACHE / sizeof PACKET_SEND_CACHE[0]; i++)
        {
            memset(&PACKET_SEND_CACHE[i], 0, sizeof *item);
        }

        // add packet to the first slot
        if (sizeof PACKET_SEND_CACHE)
        {
            memcpy(&PACKET_SEND_CACHE[0], item, sizeof *item);
        }
    }

    // Reset timer1 counter
    pins_timer1_reset();
    return 0;
}

/// @brief Remove message from the outgoing cache whose id equals to identification.
/// @param identification id if the message to be removed.
void rpl4_scache_remove(uint8_t identification)
{
    for (int i = 0; i < sizeof PACKET_SEND_CACHE / sizeof PACKET_SEND_CACHE[0]; i++)
    {
        if (PACKET_SEND_CACHE[i].packet.identification == identification)
        {
            memset(&PACKET_SEND_CACHE[i], 0, sizeof PACKET_SEND_CACHE[i]);
        }
    }
}

/// @brief This method first sends ACK messages and after that sends resend messages.
/// It is used inside interrupt handler. Messages are removed from the cache after sending.
/// One message is sent at a time. Other cached messages has to wait their turn
/// on following timer interrupts.
void rpl4_ack_resend()
{
    // Process of received packets
    for (int i = 0; i < sizeof PACKET_RECEIVE_CACHE / sizeof PACKET_RECEIVE_CACHE[0]; i++)
    {
        if (PACKET_RECEIVE_CACHE[i].retry != 0)
        {
            rpl3_send(PACKET_RECEIVE_CACHE[i].source, PACKET_RECEIVE_CACHE[i].destination, &PACKET_RECEIVE_CACHE[i].packet);
            memset(&PACKET_RECEIVE_CACHE[i], 0, sizeof PACKET_RECEIVE_CACHE[i]);
            return;
        }
    }

    // Process all sent packets
    for (int i = 0; i < sizeof PACKET_SEND_CACHE / sizeof PACKET_SEND_CACHE[0]; i++)
    {
        if (PACKET_SEND_CACHE[i].retry != 0)
        {
            rpl3_send(PACKET_SEND_CACHE[i].source, PACKET_SEND_CACHE[i].destination, &PACKET_SEND_CACHE[i].packet);
            PACKET_SEND_CACHE[i].retry--;
            return;
        }
    }
}

/// @brief Finds the message/packet inside Layer4 incomming cache.
/// It is used to find out source address of the incoming message.
/// @param identification
/// @return Returns 0 on success, -1 on error
int rpl4_rcache_find(uint8_t identification)
{
    for (int i = 0; i < sizeof PACKET_RECEIVE_CACHE / sizeof PACKET_RECEIVE_CACHE[0]; i++)
    {
        if (PACKET_RECEIVE_CACHE[i].packet.identification == identification)
        {
            return PACKET_RECEIVE_CACHE[i].source;
        }
    }
    return -1;
}

/// @brief Adds message/packet to Layer4 imcomming cache
/// @param item
/// @return Returns 0 on success, -1 on error
int rpl4_rcache_add(RPL4_CACHE_ITEM *item)
{
    int empty_slot = 0;
    int i = 0;

    // find an empy slot
    for (i = 0; i < sizeof PACKET_RECEIVE_CACHE / sizeof PACKET_RECEIVE_CACHE[0]; i++)
    {

        if (PACKET_RECEIVE_CACHE[i].retry == 0)
        {
            empty_slot = 1;
            break;
        }
    }

    // use empy slot
    if (empty_slot == 1)
    {
        memcpy(&PACKET_RECEIVE_CACHE[i], item, sizeof *item);
    }
    else
    {
        // clear all cache
        for (int i = 0; i < sizeof PACKET_RECEIVE_CACHE / sizeof PACKET_RECEIVE_CACHE[0]; i++)
        {
            memset(&PACKET_RECEIVE_CACHE[i], 0, sizeof *item);
        }

        // add packet to the first slot
        if (sizeof PACKET_RECEIVE_CACHE)
        {
            memcpy(&PACKET_RECEIVE_CACHE[0], item, sizeof *item);
        }
    }

    return 0;
}

/// @brief Creates Layer4 packet, fills in structure members and then passes it to the lower levels.
/// @param source Source address
/// @param destination Destination address
/// @param flags Layer4 packet flags
/// @param str Data to be sent
void rpl4_send(uint8_t source, uint8_t destination, uint8_t flags, uint8_t *str)
{
    // prepare packet
    RPL4_PACKET packet;
    memset(&packet, 0, sizeof packet);

    packet.identification = PACKET_IDENTIFICATION++;
    packet.flags = flags;
    memcpy(packet.payload, str, strlen(str));

    // cache packet for resending later
    int dgram = flags & RPL4_FLAGS_DGRAM;
    int ack = flags & RPL4_FLAGS_ACK;

    if (!dgram && !ack)
    {
        RPL4_CACHE_ITEM item;
        memset(&item, 0, sizeof item);

        item.retry = 5;
        item.source = source;
        item.destination = destination;
        memcpy(&item.packet, &packet, sizeof packet);
        rpl4_scache_add(&item);
    }

    // send packet
    rpl3_send(source, destination, &packet);
}

/// @brief Receives Layer4 packet. Fills Layer4 packet structure pointer given in parameters.
/// @param packetL4 Address of the Layer4 packet structure.
/// @param str Data to be sent.
/// @return Return 0 on success, -1 otherwise.
int rpl4_receive(RPL4_PACKET *packetL4)
{
    RPL3_PACKET packetL3;
    memset(&packetL3, 0, sizeof packetL3);

    int ok = rpl3_receive(&packetL3);
    if (ok == 0)
    {
        RPL4_PACKET *ppacketL4 = (RPL4_PACKET *)packetL3.payload;

        // Check packet flags, decide whether to add it to the cache or not.
        if (ppacketL4->flags & RPL4_FLAGS_DGRAM)
        {
            memcpy(packetL4, ppacketL4, sizeof *ppacketL4);
            return 0;
        }
        else if (ppacketL4->flags & RPL4_FLAGS_ACK)
        {
            rpl4_scache_remove(ppacketL4->identification);
        }
        else
        {
            RPL4_CACHE_ITEM item;
            memset(&item, 0, sizeof item);

            item.retry = 5;
            item.source = packetL3.source;
            item.destination = packetL3.destination;
            item.packet.identification = ppacketL4->identification;
            item.packet.flags = RPL4_FLAGS_ACK;
            rpl4_rcache_add(&item);

            memcpy(packetL4, ppacketL4, sizeof *ppacketL4);

            return 0;
        }
    }

    return -1;
}
