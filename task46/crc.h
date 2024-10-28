#ifndef __CRC_H__
#define __CRC_H__

#include <avr/io.h>

uint32_t crc(uint8_t *data, uint8_t size);

#endif