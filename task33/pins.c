#include "pins.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/setbaud.h>

/// @brief Configures PINs and Timers
void pins_init()
{
    /* Enable global interrupts */
    sei();

    /* Configure PINs*/
    DDRB |= (1 << DDB4);  // Clock output PIN PB4
    DDRB |= (1 << DDB5);  // Data output pin PB5
    DDRD &= ~(1 << DDD4); // Clock input pin PD4
    DDRD &= ~(1 << DDD5); // Data input pin PD5

    /* Configure Pin change interrrupt on PD4 */
    PCICR |= (1 << PCIE2);    // PD4 ==> PCINT20, PCIE2 for PCINT[23:16]
    PCMSK2 |= (1 << PCINT20); // PD5 ==> PCINT21, PCIE2 for PCINT[23:16]

    /* Configure Timer0 for giving sending priority to relay messages */
    TCCR0A |= (1 << WGM01); // CTC Mode
    TCCR0B &= ~(1 << CS02); // No prescaler
    TIMSK0 |= (1 << OCIE0A);
    OCR0A = 1;
    TCNT0 = 0;

    /* Configure Timer1 for clock signal generation */
    TCCR1A = 0;                                        // CTC Mode
    TCCR1B = (1 << CS12) | (1 << CS10) | (1 << WGM12); // prescaler 1024
    TIMSK1 = (1 << OCIE1A);
    OCR1A = RASPNET_COUNTER;
    TCNT1 = 0;

    /* Configure uart */
    // baudrate
    UBRR0H = UBRRH_VALUE;
    UBRR0L = UBRRL_VALUE;

    // Enable transmitter and receiver
    UCSR0B |= (1 << TXEN0) | (1 << RXEN0);

    // 8-bit, 1 stop bit, no parity, asynchronous UART
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00) | (0 << USBS0) | (0 << UPM01) | (0 << UPM00) | (0 << UMSEL01) | (0 << UMSEL00);

    /* Configure LEDs */
    DDRB |= (1 << DDB0);
}

/// @brief Generates clock signal
void pins_clock()
{
    PORTB ^= (1 << PB4);
}

/// @brief Enables timer interrupt for relay messages
void pins_enable_relay()
{
    TCCR0B |= (1 << CS02);
}

/// @brief Disables timer interrupt for relay messages
void pins_disable_relay()
{
    TCCR0B &= ~(1 << CS02);
}

/// @brief Enables or Disables LEDs connected to Port B1.
void pins_led_toggle()
{
    PORTB ^= (1 << PB0);
}
