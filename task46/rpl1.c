#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>
#include "rpl1.h"
#include "rpl2.h"
#include "uart.h"

/* Transmit */
RPL2_FRAME TX_BUFFER; ///< Send buffer used in Layer1
uint8_t TX_BIT_COUNTER = 0;
uint8_t TX_CRC_COUNTER = 0;
uint8_t TX_SIZE_COUNTER = 0;
uint8_t TX_PAYLOAD_COUNTER = 0;
uint8_t TX_SR = 0;
uint8_t TX_STATE = TX_STATE_IDLE;
volatile uint8_t TX_BUSY = 0;

/* Receive */
RPL2_FRAME RX_BUFFER; ///< Receive buffer used in Layer1
uint8_t RX_BIT_COUNTER = 0;
uint8_t RX_CRC_COUNTER = 0;
uint8_t RX_SIZE_COUNTER = 0;
uint8_t RX_PAYLOAD_COUNTER = 0;
uint8_t RX_SR = 0;
uint8_t RX_STATE = RX_STATE_IDLE;
volatile uint8_t RX_BUSY = 0;

/// @brief Receives messages on layer 1.
/// This method is called on each pin change interrupt.
void rpl1_receive()
{
    uint8_t pinVal = PIND & (1 << PD5);
    pinVal = pinVal ? 1 : 0;

    switch (RX_STATE)
    {
    case RX_STATE_IDLE:

        RX_SR = RX_SR << 1;
        RX_SR |= pinVal;

        if (RX_SR == PREAMBLE)
        {
            memset(&RX_BUFFER, 0, sizeof RX_BUFFER);
            RX_BUFFER.preamble = RX_SR;
            RX_SR = 0;
            RX_BIT_COUNTER = 0;
            RX_STATE = RX_STATE_READ_CRC;
            RX_BUSY = 1;
            RX_CRC_COUNTER = 4;
        }
        break;

    case RX_STATE_READ_CRC:
        RX_SR = RX_SR << 1;
        RX_SR |= pinVal;
        RX_BIT_COUNTER++;

        if (RX_BIT_COUNTER == 8)
        {
            RX_BUFFER.crc <<= 8;
            RX_BUFFER.crc |= RX_SR;
            RX_CRC_COUNTER--;
            RX_BIT_COUNTER = 0;
            RX_SR = 0;

            if (!RX_CRC_COUNTER)
            {
                RX_STATE = RX_STATE_READ_SIZE;
            }
        }
        break;

    case RX_STATE_READ_SIZE:
        RX_SR = RX_SR << 1;
        RX_SR |= pinVal;
        RX_BIT_COUNTER++;

        if (RX_BIT_COUNTER == 8)
        {
            RX_BUFFER.size = RX_SR;
            RX_BIT_COUNTER = 0;
            RX_SR = 0;
            RX_STATE = RX_STATE_READ_PAYLOAD;
        }
        break;
    case RX_STATE_READ_PAYLOAD:
        RX_SR = RX_SR << 1;
        RX_SR |= pinVal;
        RX_BIT_COUNTER++;

        if (RX_BIT_COUNTER == 8)
        {
            RX_BUFFER.payload[RX_PAYLOAD_COUNTER] = RX_SR;
            RX_PAYLOAD_COUNTER++;
            RX_BIT_COUNTER = 0;
            RX_SR = 0;

            if (RX_PAYLOAD_COUNTER >= RX_BUFFER.size)
            {
                RX_STATE = RX_STATE_END;
            }
        }
        break;
    case RX_STATE_END:
        RX_CRC_COUNTER = 0;
        RX_SIZE_COUNTER = 0;
        RX_PAYLOAD_COUNTER = 0;
        RX_BIT_COUNTER = 0;
        RX_BUSY = 0;
        RX_STATE = RX_STATE_IDLE;
    }
}

/// @brief Sends messages on layer 1.
/// This method is called on each clock timer interrupt.
void rpl1_send()
{
    switch (TX_STATE)
    {
    case TX_STATE_IDLE:

        if (TX_BUFFER.preamble)
        {
            TX_STATE = TX_STATE_SEND_PREAMBLE;
            TX_BUSY = 1;
        }
        break;

    case TX_STATE_SEND_PREAMBLE:
        if (!TX_BIT_COUNTER)
        {
            TX_SR = TX_BUFFER.preamble;
            TX_BIT_COUNTER = 8;
        }

        if (TX_BIT_COUNTER)
        {
            uint8_t carry = TX_SR & (1 << 7);
            TX_SR = TX_SR << 1;

            if (carry)
            {
                PORTB |= (1 << PB5);
            }
            else
            {
                PORTB &= ~(1 << PB5);
            }

            TX_BIT_COUNTER--;

            if (!TX_BIT_COUNTER)
            {
                TX_STATE = TX_STATE_SEND_CRC;
                TX_CRC_COUNTER = 4;
            }
        }
        break;

    case TX_STATE_SEND_CRC:
        if (TX_CRC_COUNTER && !TX_BIT_COUNTER)
        {
            TX_SR = (uint8_t)(TX_BUFFER.crc >> (8 * (TX_CRC_COUNTER - 1)));
            TX_BIT_COUNTER = 8;
            TX_CRC_COUNTER--;
        }

        if (TX_BIT_COUNTER)
        {
            uint8_t carry = TX_SR & (1 << 7);

            if (carry)
            {
                PORTB |= (1 << PB5);
            }
            else
            {
                PORTB &= ~(1 << PB5);
            }

            TX_SR = TX_SR << 1;

            TX_BIT_COUNTER--;

            if (!TX_CRC_COUNTER && !TX_BIT_COUNTER)
            {
                TX_STATE = TX_STATE_SEND_SIZE;
            }
        }
        break;
    case TX_STATE_SEND_SIZE:
        if (!TX_BIT_COUNTER)
        {
            TX_SR = TX_BUFFER.size;
            TX_BIT_COUNTER = 8;
        }

        if (TX_BIT_COUNTER)
        {
            uint8_t carry = TX_SR & (1 << 7);
            TX_SR = TX_SR << 1;

            if (carry)
            {
                PORTB |= (1 << PB5);
            }
            else
            {
                PORTB &= ~(1 << PB5);
            }

            TX_BIT_COUNTER--;

            if (!TX_BIT_COUNTER)
            {
                TX_STATE = TX_STATE_SEND_PAYLOAD;
                TX_PAYLOAD_COUNTER = TX_BUFFER.size;
            }
        }
        break;
    case TX_STATE_SEND_PAYLOAD:
        if (TX_PAYLOAD_COUNTER && !TX_BIT_COUNTER)
        {
            TX_SR = TX_BUFFER.payload[TX_BUFFER.size - TX_PAYLOAD_COUNTER];
            TX_BIT_COUNTER = 8;
            TX_PAYLOAD_COUNTER--;
        }

        if (TX_BIT_COUNTER)
        {
            uint8_t carry = TX_SR & (1 << 7);
            TX_SR = TX_SR << 1;

            if (carry)
            {
                PORTB |= (1 << PB5);
            }
            else
            {
                PORTB &= ~(1 << PB5);
            }

            TX_BIT_COUNTER--;

            if (!TX_PAYLOAD_COUNTER && !TX_BIT_COUNTER)
            {
                TX_STATE = TX_STATE_END;
            }
        }
        break;
    case TX_STATE_END:
        TX_BUFFER.preamble = 0;
        TX_SR = 0;
        TX_BUSY = 0;
        PORTB &= ~(1 << PB5);
        TX_STATE = TX_STATE_IDLE;
    }
}
