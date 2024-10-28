#define F_CPU 12000000UL
#define BAUD 9600

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "led.h"
#include "uart.h"
#include "raspnet.h"

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
    led_toggle();
    raspnet_receive();
}

int main()
{
    raspnet_init();
    uart_init();
    led_init();

    while (1)
    {
        /* Send one byte over raspnet */
        _delay_ms(500);
        TX_BUFFER = 0b11100001;
        
        
        // wait until ready
        while (!RX_READY)
            ;
        uart_log(RX_BUFFER);
        

        // Set it false again
        RX_READY = 0;
    }
    return 0;
}
