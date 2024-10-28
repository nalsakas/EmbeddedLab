#define F_CPU 12000000UL
#define BAUD 9600

#include <avr/io.h>
#include <util/delay.h>
#include <util/setbaud.h>
#include <avr/interrupt.h>

void configureUART()
{

    // baudrate
    UBRR0H = UBRRH_VALUE;
    UBRR0L = UBRRL_VALUE;

    // Enable transmitter and receiver
    UCSR0B |= (1 << TXEN0) | (1 << RXEN0);

    // 8-bit, 1 stop bit, no parity, asynchronous UART
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00) | (0 << USBS0) | (0 << UPM01) | (0 << UPM00) | (0 << UMSEL01) | (0 << UMSEL00);
}

unsigned char receive()
{
    // Wait until receive complete flag is set
    while (!(UCSR0A & (1 << RXC0)))
        ;

    return UDR0;
}

void transmit(unsigned char ch)
{
    // wait untill data register is free
    while (!(UCSR0A & (1 << UDRE0)))
        ;

    // write data to transmit buffer
    UDR0 = ch;
}
/**
 * To get 1sec timer interval we need
 * to count until 46874
 * this number is only possible with a
 * 16 bit register. It is therefore
 * only possible woth TIMER1
 */
void configureTimer()
{
    /* Enable global interrupts */
    sei();

    /* Timer/Counter Control Register */
    /* Prescaler 256 */
    TCCR1B = (1 << CS12) | (1 << WGM12);

    /* Output compare register*/
    OCR1A = 46874;
    
    /* Timer/Counter Interrupt Mask */
    /* Enable Overflow Interrupt */
    TIMSK1 = 1 << OCIE1A;

    /* Initialize Counter */
    TCNT1 = 0;
}

/**
 * This routine will be executed with every timer overflow.
 */
ISR(TIMER1_COMPA_vect)
{
    /* Timer/Counter Register */
    transmit('I');
    /* Clear counter to restart counting */
    TCNT1 = 0;
}

int main()
{
    configureUART();
    configureTimer();

    while (1)
    {
        transmit('M');
        _delay_ms(250);
    }
    return 0;
}
