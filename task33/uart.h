#ifndef __UART_H__
#define __UART_H__

#include <stdint.h>

unsigned char uart_receive();
unsigned char uart_receive_nb();
void uart_send(uint8_t ch);
void uart_send_str(uint8_t *str);
void uart_send_bin(uint32_t num, uint8_t newline);
void uart_send_num(uint32_t num, uint8_t newline);
void uart_send_hex(uint32_t num, uint8_t newline);

#endif
