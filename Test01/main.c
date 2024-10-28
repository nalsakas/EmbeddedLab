#define F_CPU 12000000UL
#define BAUD 9600

#include <string.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "uart.h"
#include "api.h"
#include "pins.h"


/// @brief Receives command lines from termnal and pass them to API.
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

/// @brief Receives layer 4 messages and pass them to API.
/// This method is used inside main loop of the program.
void rp2uart()
{

}

int main()
{
    pins_init();
    uart_send_str("System restarted.\n");

    while (1)
    {
        /* SEND */
        uart2rp();

        /* RECEIVE */
        rp2uart();
    }
    return 0;
}
