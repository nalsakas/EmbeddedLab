#define F_CPU 12000000UL
#define BAUD 9600

#include <avr/io.h>
#include <util/delay.h>
#include <util/setbaud.h>

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

    UDR0 = ch;
}

void configureLEDs()
{
    DDRD |= (1 << DDD4);
    DDRD |= (1 << DDD5);
    DDRD |= (1 << DDD6);
    DDRD |= (1 << DDD7);
    DDRB |= (1 << DDB0);
    DDRB |= (1 << DDB1);
    DDRB |= (1 << DDB2);
    DDRB |= (1 << DDB3);
    DDRB |= (1 << DDB4);
    DDRB |= (1 << DDB5);
}

void delay(unsigned char ch) {
    switch (ch) {
        case '0':
            _delay_ms(100);
            break;
        case '1':
            _delay_ms(150);
            break;
        case '2':
            _delay_ms(200);
            break;
        case '3':
            _delay_ms(250);
            break;
        case '4':
            _delay_ms(300);
            break;
        case '5':
            _delay_ms(350);
            break;
        case '6':
            _delay_ms(400);
            break;
        case '7':
            _delay_ms(450);
            break;
        case '8':
            _delay_ms(500);
            break;
        case '9':
            _delay_ms(550);
            break;
        default:
            _delay_ms(100);
    }
}

/**
 * Set all PINs on
 */
void turnOnLEDs(unsigned char ch)
{
    PORTD |= (1 << PD4);
    delay(ch);

    PORTD |= (1 << PD5);
    delay(ch);

    PORTD |= (1 << PD6);
    delay(ch);

    PORTD |= (1 << PD7);
    delay(ch);

    PORTB |= (1 << PB0);
    delay(ch);

    PORTB |= (1 << PB1);
    delay(ch);

    PORTB |= 1 << (PB2);
    delay(ch);

    PORTB |= (1 << PB3);
    delay(ch);

    PORTB |= (1 << PB4);
    delay(ch);

    PORTB |= (1 << PB5);
    delay(ch);
}

/**
 * Sets all PINs off
 */
void turnOffLEDs(unsigned char ch)
{
    PORTB &= ~(1 << PB5);
    delay(ch);

    PORTB &= ~(1 << PB4);
    delay(ch);

    PORTB &= ~(1 << PB3);
    delay(ch);

    PORTB &= ~(1 << PB2);
    delay(ch);

    PORTB &= ~(1 << PB1);
    delay(ch);

    PORTB &= ~(1 << PB0);
    delay(ch);

    PORTD &= ~(1 << PD7);
    delay(ch);

    PORTD &= ~(1 << PD6);
    delay(ch);

    PORTD &= ~(1 << PD5);
    delay(ch);

    PORTD &= ~(1 << PD4);
    delay(ch);
}

void sayHello() {
    char *str = "LED aminatuon speed adjustable\n"
    "Use 0-9 from keypad in order to increase or decrease animation speed\n";
    
    char *pStr = str;
        
    while (*pStr) {
        transmit(*pStr);
        pStr++;
    }
}


int main()
{
    configureLEDs();
    configureUART();
    sayHello();

    unsigned char ch = '0';
    
    while (1)
    {
        // check receive complete flag
        if ((UCSR0A & (1 << RXC0))) {
            ch = UDR0;
            UCSR0A &=  ~(1 << RXC0);
        }
        
        turnOnLEDs(ch);
        turnOffLEDs(ch);
    }
    return 0;
}
