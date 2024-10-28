#define F_CPU 12000000UL
#define BAUD 9600

#include <string.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "uart.h"
#include "rpl1.h"
#include "rpl2.h"
#include "rpl3.h"
#include "api.h"
#include "pins.h"

/// @brief Interrupt service routine for PIN change. Reacts to received clock signal and receives messages on incoming data pins on layer 1.
/// @param Interrupt vector number.
ISR(PCINT2_vect)
{
    rpl1_receive();
}

/// @brief Interrupt service routine for Timer 0. Disables timer and relays messages that are not destinated to this node.
/// @param Interrupt vector number.
ISR(TIMER0_COMPA_vect)
{
    pins_disable_relay();
    rpl2_resend();
}

uint8_t flag = 0; ///< This flag causes clock and data to take turns. Only one of them will execute when clock hits.
/// @brief Interrupt service routine for Timer 1. Generates clock signal and sends layer 1 messages.
/// @param Interrupt vector number.
ISR(TIMER1_COMPA_vect)
{
    if (flag)
    {
        rpl1_send();
    }
    else
    {
        pins_clock();
    }
    flag ^= 1;
}

/// @brief Receive commands from termnal and pass them to API.
/// This method is used inside main loop of the program.
void uart2rp()
{
    static uint8_t uart_buffer[250];
    static uint8_t i = 0;

    char ch = uart_receive_nb();

    if (ch >= 0x20 && ch < 0x7F)
    {
        uart_buffer[i] = ch;
        i++;
    }
    else if (ch == '\r')
    {
        if (strlen(uart_buffer))
        {
            api_handle_line(uart_buffer);
        }

        i = 0;
        memset(uart_buffer, 0, 250);
    }
}

/// @brief Receive Layer 3 messages and pass them to API.
/// This method is used inside main loop of the program.
void rp2uart()
{
    RPL3_PACKET packet;
    memset((uint8_t *)&packet, 0, sizeof packet);

    int ok = rpl3_receive(&packet);
    if (ok == 0)
    {
        api_handle_packet(&packet);
    }
}

int main()
{
    pins_init();
    uart_send_str("\nSystem restarted.\n");

    while (1)
    {
        /* SENDING part*/
        uart2rp();

        /* RECEIVING part */
        rp2uart();
    }
    return 0;
}
