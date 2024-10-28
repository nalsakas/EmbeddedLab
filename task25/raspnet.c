#include "raspnet.h"
#include "uart.h"

/* Transmit */
uint8_t TX_BUFFER = 0;
uint8_t TX_SR = 0;
uint8_t TX_COUNTER = 0;
volatile uint8_t TX_STATE = STATE_IDLE;

/* Receive */
uint8_t RX_BUFFER = 0;
uint8_t RX_SR = 0;
uint8_t RX_COUNTER = 0;
volatile uint8_t RX_STATE = STATE_IDLE;
volatile uint8_t RX_READY = 0;

void raspnet_timerreset()
{
    TCNT1 = 0;
}

void raspnet_clock()
{
    PORTB ^= (1 << PB4);
}

void raspnet_receive()
{
    uint8_t val = PIND & (1 << PD5);
    val = val ? 1 : 0; // normalize pin value    
    
    switch (RX_STATE)
    {
    case STATE_IDLE:
        RX_SR = RX_SR << 1;
        RX_SR |= val;
        
        if (RX_SR == 1)
        {
            RX_COUNTER = 7;
            RX_STATE = STATE_RECEIVE;
        }
        break;
    case STATE_RECEIVE:
        RX_SR = RX_SR << 1;
        RX_SR |= val;
        RX_COUNTER--;

        if (!RX_COUNTER)
        {
            RX_STATE = STATE_END;
        }
        break;
    case STATE_END:
        RX_BUFFER = RX_SR;
        RX_SR = 0;
        RX_READY = 1;
        RX_STATE = STATE_IDLE;
        break;
    }
}

void raspnet_send()
{
    uint8_t val = 0;

    switch (TX_STATE)
    {
    case STATE_IDLE:
        if (TX_BUFFER)
        {
            // Move buffer into SR
            TX_SR = TX_BUFFER;
            
            // Clear Buffer
            TX_BUFFER = 0;

            // Count bits sent over wire
            TX_COUNTER = 8;

            TX_STATE = STATE_SEND;
        }
        break;
    case STATE_SEND:
        val = TX_SR & (1 << 7);
        
        if (val)
        {
            PORTB |= (1 << PB5);
        }
        else
        {
            PORTB &= ~(1 << PB5);
        }

        TX_SR = TX_SR << 1;
        
        TX_COUNTER--;

        if (!TX_COUNTER)
        {
            TX_STATE = STATE_END;
        }
        break;
    case STATE_END:
        TX_STATE = STATE_IDLE;
        TX_SR = 0;
        PORTB &= ~(1 << PB5); // clear pin state
        break;
    }
}

void raspnet_init()
{
    /* Enable global interrupts */
    sei();

    /* Timer/Counter Control Register */
    /* Prescaler 256 */
    TCCR1B = (1 << CS12) | (1 << WGM12);

    /* Output compare register*/
    OCR1A = RASPNET_COUNTER;

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

    /* Configure Pin change interrrupt on PD4
     * PD4 ==> PCINT20, PCIE2 for PCINT[23:16]
     * PD5 ==> PCINT21, PCIE2 for PCINT[23:16]
     */
    PCICR |= (1 << PCIE2);
    PCMSK2 |= (1 << PCINT20);
}
