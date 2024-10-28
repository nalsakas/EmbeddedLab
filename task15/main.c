#define F_CPU 12000000UL // Speed of the microprocessor, in hertz (16 MHz)
#include <avr/io.h>      // provides PORT* and DDR* registers
#include <util/delay.h>  // provides _delay_ms(), and needs F_CPU

/**
 * Configure DataDirectionRegister for all PINs
 */
void configurePins()
{
    DDRD |= _BV(DDD4);
    DDRD |= _BV(DDD5);
    DDRD |= _BV(DDD6);
    DDRD |= _BV(DDD7);
    DDRB |= _BV(DDB0);
    DDRB |= _BV(DDB1);
    DDRB |= _BV(DDB2);
    DDRB |= _BV(DDB3);
    DDRB |= _BV(DDB4);
    DDRB |= _BV(DDB5);
}

/**
 * Set all PINs on
 */
void setPinsOn()
{
    PORTD |= _BV(PD4);
    _delay_ms(250);

    PORTD |= _BV(PD5);
    _delay_ms(250);

    PORTD |= _BV(PD6);
    _delay_ms(250);

    PORTD |= _BV(PD7);
    _delay_ms(250);

    PORTB |= _BV(PB0);
    _delay_ms(250);

    PORTB |= _BV(PB1);
    _delay_ms(250);

    PORTB |= _BV(PB2);
    _delay_ms(250);

    PORTB |= _BV(PB3);
    _delay_ms(250);

    PORTB |= _BV(PB4);
    _delay_ms(250);

    PORTB |= _BV(PB5);
    _delay_ms(250);
}

/**
 * Sets all PINs off
 */
void setPinsOff()
{
    PORTB &= ~_BV(PB5);
    _delay_ms(250);
    
    PORTB &= ~_BV(PB4);
    _delay_ms(250);
    
    PORTB &= ~_BV(PB3);
    _delay_ms(250);
    
    PORTB &= ~_BV(PB2);
    _delay_ms(250);
    
    PORTB &= ~_BV(PB1);
    _delay_ms(250);
    
    PORTB &= ~_BV(PB0);
    _delay_ms(250);
    
    PORTD &= ~_BV(PD7);
    _delay_ms(250);
    
    PORTD &= ~_BV(PD6);
    _delay_ms(250);
    
    PORTD &= ~_BV(PD5);
    _delay_ms(250);
    
    PORTD &= ~_BV(PD4);
    _delay_ms(250);
}

int main()
{
    configurePins();

    while (1)
    {
        setPinsOn();
        setPinsOff();
    }
    return 0;
}
