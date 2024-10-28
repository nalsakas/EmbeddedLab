#include <avr/io.h>
#include "led.h"

void led_init()
{
    DDRB |= (1 << DDB0);
}

void led_toggle()
{
    PORTB ^= (1 << PB0);
}