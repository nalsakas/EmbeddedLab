#include <avr/io.h>
#include <avr/interrupt.h>

#ifndef F_CPU
#define F_CPU 12000000UL
#endif
#define RASPNET_DELAY_MS 100
#define RASPNET_COUNTER (RASPNET_DELAY_MS * (F_CPU / 256) / 1000 - 1)

#define STATE_IDLE 0
#define STATE_SEND 1
#define STATE_RECEIVE 2
#define STATE_END 3

/* Transmit */
extern uint8_t TX_BUFFER;
extern volatile uint8_t TX_STATE;

/* Receive */
extern uint8_t RX_BUFFER;
extern volatile uint8_t RX_STATE;
extern volatile uint8_t RX_READY;

/**
 * @brief: Init method for raspnet protocol. Sets Register and flag's values
 * @desc: To get 1sec timer interval we need to count until 46874.
 * This number is only possible with a 16 bit register.
 * It is therefore only possible with TIMER1 register.
 */
void raspnet_init();
void raspnet_send();
void raspnet_receive();

/**
 * Generate clock signal on PIN PB4
 */
void raspnet_clock();
void raspnet_timerreset();

void uart_printBits(uint8_t ch);
