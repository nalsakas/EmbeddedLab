#include "pins.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/setbaud.h>

/// @brief Configures PINs and Timers
void pins_init()
{
    /* Enable global interrupts */
    sei();

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

/// @brief Reset the counter of timer1. Timer1 is used for ACK, RESEND messages.
void pins_timer1_reset()
{
    TCNT1 = 0;
}
