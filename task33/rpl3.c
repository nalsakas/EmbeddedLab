#include <avr/io.h>
#include <string.h>
#include <stdlib.h>
#include "rpl3.h"
#include "rpl2.h"
#include "uart.h"
#include "crc.h"
#include "pins.h"
#include "api.h"

/// @brief Send packages on layer 3. This method requires source address, destination address
///  and pointer to the data buffer.
/// @param source Source address.
/// @param destination Destination address.
/// @param packetL4 Layer 4 packet to be sent.
void rpl3_send(uint8_t source, uint8_t destination, uint8_t *data)
{
    RPL3_PACKET packetL3;
    memset(&packetL3, 0, sizeof packetL3);

    packetL3.source = source;
    packetL3.destination = destination;
    memcpy(packetL3.payload, data, strlen(data));
    rpl2_send(&packetL3);
}

/// @brief Receive packeges on layer 3. Checks source and destination addreses of incoming messages.
/// Depending on the situation either packets accepted or rejected.
/// @param packet Ponter to L3 message packet structure. Incoming message goes in this packet.
/// @return Returns 0 on success, -1 otherwise.
int rpl3_receive(RPL3_PACKET *packet)
{
    RPL2_FRAME frame;
    memset((uint8_t *)&frame, 0, sizeof frame);

    int ok = rpl2_receive(&frame);
    if (ok == 0)
    {
        RPL3_PACKET *ppacket = (RPL3_PACKET *)frame.payload;

        if (ppacket->source == API_ADDRESS)
        {
            // reject
            uart_send_str("Same source packet received. Rejecting...\n");
            return -1;
        }

        if (checkCRC(&frame) != 0)
        {
            // reject
            uart_send_str("CRC check failed: ");
            uart_send_hex(crc(frame.payload, frame.size), 1);
            return -1;
        }

        if (ppacket->destination == API_ADDRESS)
        {
            // accept
            memcpy((uint8_t *)packet, (uint8_t *)ppacket, frame.size);
            return 0;
        }

        if (ppacket->destination == API_BROADCAST)
        {
            // accept
            memcpy((uint8_t *)packet, (uint8_t *)ppacket, frame.size);

            // relay
            uart_send_str("Relaying...\n");
            rpl2_relay(&frame);

            // memcpy((uint8_t)&RPL2_RESEND_FRAME, (uint8_t)&frame, sizeof frame);
            // pins_enable_relay();

            return 0;
        }
        else
        {
            // relay
            uart_send_str("Relaying...\n");
            rpl2_relay(&frame);

            // memcpy((uint8_t)&RPL2_RESEND_FRAME, (uint8_t)&frame, sizeof frame);
            // pins_enable_relay();

            return -1;
        }
    }
}
