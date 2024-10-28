#define F_CPU 12000000UL
#define BAUD 9600

#include <string.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "led.h"
#include "uart.h"
#include "raspnetL1.h"
#include "raspnetL2.h"

/**
 * Output clock ISR
 */
ISR(TIMER1_COMPA_vect)
{
    raspnet_clock();
    raspnet_send();
    raspnet_timerreset();
}

/**
 * Input clock ISR
 */
ISR(PCINT2_vect)
{
    /* Delay further program execution by 2 clock cycles */
    asm("nop");
    asm("nop");
    led_toggle();
    raspnet_receive();
}

int main()
{
    raspnet_init();
    uart_init();
    led_init();

    RASPNET_FRAME buffer;

    while (1)
    {
        _delay_ms(500);
        raspnetL2_send("Hello\n");

        raspnetL2_receive(&buffer);

        for (uint8_t i = 0; i < buffer.size; i++)
        {
            uart_send(buffer.payload[i]);
        }
    }
    return 0;
}
