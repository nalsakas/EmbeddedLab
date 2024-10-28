#define F_CPU 12000000UL
#define BAUD 9600

#include <avr/io.h>
#include <util/setbaud.h>
#include "uart.h"

void uart_init()
{
    // baudrate
    UBRR0H = UBRRH_VALUE;
    UBRR0L = UBRRL_VALUE;

    // Enable transmitter and receiver
    UCSR0B |= (1 << TXEN0) | (1 << RXEN0);

    // 8-bit, 1 stop bit, no parity, asynchronous UART
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00) | (0 << USBS0) | (0 << UPM01) | (0 << UPM00) | (0 << UMSEL01) | (0 << UMSEL00);
}

unsigned char uart_receive()
{
    // Wait until receive complete flag is set
    while (!(UCSR0A & (1 << RXC0)))
        ;

    return UDR0;
}

void uart_send(unsigned char ch)
{
    // wait untill data register is free
    while (!(UCSR0A & (1 << UDRE0)))
        ;

    // write data to transmit buffer
    UDR0 = ch;
}

/* Receive a character over raspnet and send it to terminal */
void uart_log8(uint8_t ch)
{
    for (int i = 0; i < 8; i++)
    {
        if (ch & 0x80)
        {
            uart_send('1');
        }
        else
        {
            uart_send('0');
        }
        ch = ch << 1;
    }
    uart_send('\n');
}

void uart_log16(uint16_t ch)
{
    for (int i = 0; i < 16; i++)
    {
        if (ch & 0x8000)
        {
            uart_send('1');
        }
        else
        {
            uart_send('0');
        }
        ch = ch << 1;
    }
    uart_send('\n');
}

void uart_log32(uint32_t ch)
{
    for (int i = 0; i < 32; i++)
    {
        if (ch & 0x80000000)
        {
            uart_send('1');
        }
        else
        {
            uart_send('0');
        }
        ch = ch << 1;
    }
    uart_send('\n');
}
