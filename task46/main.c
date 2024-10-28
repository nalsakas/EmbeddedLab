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

/// @brief Interrupt service routine for PIN change. Toggles LEDs and receives messages on layer 1.
/// @param Interrupt vector number.
ISR(PCINT2_vect)
{
    rpl1_receive();
}

/// @brief Interrupt service routine for Timer 0. Disables timer and relays messages that are not destined to this node.
/// @param Interrupt vector number.
ISR(TIMER0_COMPA_vect)
{
    pins_disable_relay();
    rpl2_resend();
}

/// @brief Interrupt service routine for Timer 1. Send ACK and Resend messages on layer 4.
/// @param Interrupt vector number.
ISR(TIMER1_COMPA_vect)
{
    rpl4_ack_resend();
}

uint8_t flag = 0; ///< This flag causes clock and data to take turns. Only one of them will execute when clock hits.
/// @brief Interrupt service routine for Timer 2. Generates clock signal and sends layer 1 messages.
/// @param Interrupt vector number.
ISR(TIMER2_COMPA_vect)
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

/// @brief Receives command lines from termnal and pass them to API.
/// This method is used inside main loop of the program.
void uart2rp()
{
    static uint8_t buffer[250];
    static uint8_t i = 0;

    char ch = uart_receive_nb();

    if (ch >= 0x20 && ch < 0x7F)
    {
        buffer[i] = ch;
        i++;
    }
    else if (ch == '\r')
    {
        if (strlen(buffer))
        {
            api_handle_line(buffer);
        }

        i = 0;
        memset(buffer, 0, 250);
    }
}

/// @brief Receives layer 4 messages and pass them to API.
/// This method is used inside main loop of the program.
void rp2uart()
{
    RPL4_PACKET packet;
    memset(&packet, 0, sizeof packet);
    int ok = rpl4_receive(&packet);
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
        /* SEND */
        uart2rp();

        /* RECEIVE */
        rp2uart();
    }
    return 0;
}
