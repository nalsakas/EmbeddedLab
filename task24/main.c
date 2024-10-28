#define F_CPU 12000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

void configureLED()
{
    DDRB |= (1 << DDB0);
}

void toggleLED()
{
    PORTB ^= (1 << PB0);
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

    /* Clock output PIN PB4 */
    DDRB |= (1 << DDB4);

    /* Data output pin PB5 */
    DDRB |= (1 << DDB5);

    /* Clock input pin PD4 */
    DDRD &= ~(1 << DDD4);

    /* Data input pin PD5 */
    DDRD &= ~(1 << DDD5);

    /* Configure Pin change interrrupt on PD4 */
    /* PD4 ==> PCINT20, PCIE2 for PCINT[23:16] */
    /* */
    PCICR |= (1 << PCIE2);
    PCMSK2 |= (1 << PCINT20);
}

/**
 * This routine will be executed with every timer overflow.
 */
ISR(TIMER1_COMPA_vect)
{
    /* Generate clock signal on PIN PB4 */
    PORTB ^= (1 << PB4);

    /* Timer/Counter Register */
    /* Clear counter to restart counting */
    TCNT1 = 0;
}

/**
 * This routine will be executed when PD4 pin logic toggles.
 */
ISR(PCINT2_vect)
{
    toggleLED();
}

int main()
{
    configureTimer();
    configureLED();

    while (1)
    {
        // do nothing
    }
    return 0;
}
