#ifndef __RASPNETL1_H__
#define __RASPNETL1_H__

#include <avr/io.h>
#include <avr/interrupt.h>
#include "raspnetL2.h"

#ifndef PREAMBLE
#define PREAMBLE 0b01111110
#endif

#ifndef F_CPU
#define F_CPU 12000000UL
#endif

#ifndef RASPNET_DELAY_MS
#define RASPNET_DELAY_MS 10
#endif

#define RASPNET_COUNTER (RASPNET_DELAY_MS * (F_CPU / 256) / 1000 - 1)


/* Transmit */
#define TX_STATE_IDLE 0
#define TX_STATE_SEND_PREAMBLE 1
#define TX_STATE_SEND_CRC 2
#define TX_STATE_SEND_SIZE 3
#define TX_STATE_SEND_PAYLOAD 4
#define TX_STATE_END 5
extern RASPNET_FRAME TX_BUFFER;
extern volatile uint8_t TX_BUSY;

/* Receive */
#define RX_STATE_IDLE 0
#define RX_STATE_READ_CRC 1
#define RX_STATE_READ_SIZE 2
#define RX_STATE_READ_PAYLOAD 3
#define RX_STATE_END 4
extern RASPNET_FRAME RX_BUFFER;
extern volatile uint8_t RX_READY;

void raspnet_init();
void raspnet_send();
void raspnet_receive();
void raspnet_clock();
void raspnet_timerreset();
uint8_t is_rx_ready();
uint8_t is_tx_complete();

#endif
