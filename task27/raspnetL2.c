#include <avr/io.h>
#include "raspnetL2.h"
#include "raspnetL1.h"
#include "uart.h"
#include "crc.h"
#include <stdio.h>
#include <string.h>

void raspnetL2_send(char *str)
{

    while (TX_BUSY)
        ;

    /* Copy over to TX */
    TX_BUFFER.preamble = PREAMBLE;
    TX_BUFFER.crc = crc(str, strlen(str));
    TX_BUFFER.size = strlen(str);
    memcpy(TX_BUFFER.payload, str, strlen(str));
}

void raspnetL2_receive(RASPNET_FRAME *buffer)
{
    while (!RX_READY)
        ;

    /* Check Frame */
    if (RX_BUFFER.crc == crc(RX_BUFFER.payload, RX_BUFFER.size))
    {
        buffer->preamble = RX_BUFFER.preamble;
        buffer->crc = RX_BUFFER.crc;
        buffer->size = RX_BUFFER.size;
        memcpy(buffer->payload, RX_BUFFER.payload, RX_BUFFER.size);
    }
}
