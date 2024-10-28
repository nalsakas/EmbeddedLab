#include <avr/io.h>
#include <string.h>
#include <stdlib.h>
#include "uart.h"

/// @brief Receives uart serial character messages from console. This function blocks until a message comes.
/// @return Returns character
unsigned char uart_receive()
{
    // Wait until receive complete flag is set
    while (!(UCSR0A & (1 << RXC0)))
        ;

    return UDR0;
}

/// @brief Receives uart serial character messages from console. This function doesn't block.
/// @return Returns character if there is any.
unsigned char uart_receive_nb()
{
    char ch = '\0';

    // Wait until receive complete flag is set
    if (UCSR0A & (1 << RXC0))
    {
        ch = UDR0;
    }

    return ch;
}

/// @brief Sends character over uart serial communication to the console.
/// @param ch Character ro be send.
void uart_send(uint8_t ch)
{
    // wait untill data register is free
    while (!(UCSR0A & (1 << UDRE0)))
        ;

    // write data to transmit buffer
    UDR0 = ch;
}

/// @brief Sends character array (string) over uart to the console.
/// @param str String to be send.
void uart_send_str(uint8_t *str)
{
    for (uint8_t i = 0; i < strlen(str); i++)
    {
        uart_send(str[i]);
    }
}

/// @brief Send number to the console as binary number
/// @param num Number to send to hte console.
/// @param newline Whether to add a newline at the end
void uart_send_bin(uint32_t num, uint8_t newline)
{
    char buf[33];
    itoa(num, buf, 2);
    uart_send_str(buf);
    if (newline)
        uart_send('\n');
}

/// @brief Send number to the console as decimal number
/// @param num Number to send to hte console.
/// @param newline Whether to add a newline at the end
void uart_send_num(uint32_t num, uint8_t newline)
{
    char buf[33];
    itoa(num, buf, 10);
    uart_send_str(buf);
    if (newline)
        uart_send('\n');
}

/// @brief Send number to the console as hex number
/// @param num Number to send to hte console.
/// @param newline Whether to add a newline at the end
void uart_send_hex(uint32_t num, uint8_t newline)
{
    char buf[33];
    itoa(num, buf, 16);
    uart_send_str(buf);
    if (newline)
        uart_send('\n');
}
