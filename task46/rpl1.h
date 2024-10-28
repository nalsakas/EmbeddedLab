#ifndef __RASPNETL1_H__
#define __RASPNETL1_H__

#include "rpl2.h"

/* Transmit */
#define TX_STATE_IDLE 0
#define TX_STATE_SEND_PREAMBLE 1
#define TX_STATE_SEND_CRC 2
#define TX_STATE_SEND_SIZE 3
#define TX_STATE_SEND_PAYLOAD 4
#define TX_STATE_END 5
extern RPL2_FRAME TX_BUFFER;     //< Used as Level 1 cache for transmitted frame
extern volatile uint8_t TX_BUSY; //< Flag used to indicate Level 1 transmission is busy or not

/* Receive */
#define RX_STATE_IDLE 0
#define RX_STATE_READ_CRC 1
#define RX_STATE_READ_SIZE 2
#define RX_STATE_READ_PAYLOAD 3
#define RX_STATE_END 4
extern RPL2_FRAME RX_BUFFER;     //< Used as Level 1 cache for received frame
extern volatile uint8_t RX_BUSY; //< Flag used to indicate Level 1 receiving is busy or not

void rpl1_send();
void rpl1_receive();

#endif
