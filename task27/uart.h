#ifndef __UART_H__
#define __UART_H__

void uart_init();
unsigned char uart_receive();
void uart_send(unsigned char ch);
void uart_log8(uint8_t ch);
void uart_log16(uint16_t ch);
void uart_log32(uint32_t ch);

#endif
