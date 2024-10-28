#include <avr/io.h>
#include <stdio.h>
#include <string.h>
#include "rpl1.h"
#include "rpl2.h"
#include "crc.h"
#include "uart.h"

RPL2_FRAME RPL2_RESEND_FRAME; ///< L2 frame cache for redirecting them.

/// @brief Check CRC of the frame given in parameter. On Error logs to the console.
/// @param pFrame Pointer to the frame.
/// @return Return 0 on success, -1 otherwise
int checkCRC(RPL2_FRAME *pFrame)
{
    if (pFrame->crc == crc(pFrame->payload, pFrame->size))
    {
        return 0;
    }
    else
    {
        return -1;
    }
}

/// @brief Sends messages on layer 2.
/// @param packet Layer 3 packet to be send.
void rpl2_send(RPL3_PACKET *packet)
{
    /* Spin lock */
    while (TX_BUSY)
        ;

    memset(&TX_BUFFER, 0, sizeof TX_BUFFER);

    uint8_t size = strlen(packet->payload + 2) + 4;

    TX_BUFFER.crc = crc((uint8_t *)packet, size);
    TX_BUFFER.size = size;
    memcpy(TX_BUFFER.payload, (uint8_t *)packet, size);

    TX_BUFFER.preamble = PREAMBLE;
}

/// @brief Resends relay messages.
/// Relay messages are messages which aren't directed to this node on network.
void rpl2_resend()
{
    /* Spin lock */
    while (TX_BUSY)
        ;

    memset((uint8_t *)&TX_BUFFER, 0, sizeof TX_BUFFER);
    memcpy(&TX_BUFFER, &RPL2_RESEND_FRAME, sizeof RPL2_RESEND_FRAME);
}

/// @brief Receives messages on layer 2.
/// @param frame Pointer to the frame structure. Incoming message goes in this frame.
/// @return Returns 0 on success, -1 otherwise.
int rpl2_receive(RPL2_FRAME *frame)
{
    /* Spin lock */
    while (RX_BUSY)
        ;

    /* Check Frame */
    if (!RX_BUFFER.preamble)
    {
        return -1;
    }

    memcpy(frame, &RX_BUFFER, sizeof RX_BUFFER);

    // Clear receiving buffer
    memset(&RX_BUFFER, 0, sizeof RX_BUFFER);
    return 0;
}

// @brief Resends relay messages.
/// Relay messages are messages which aren't directed to this node on network.
void rpl2_relay(RPL2_FRAME *pframe)
{
    /* Spin lock */
    while (TX_BUSY)
        ;

    memset((uint8_t *)&TX_BUFFER, 0, sizeof TX_BUFFER);
    memcpy((uint8_t *)&TX_BUFFER, (uint8_t *)pframe, sizeof *pframe);
}