#include "crc.h"
/// @brief Calculates 32 bit CRC of the data pointed by first parameter.
/// @param data Pointer to the data array.
/// @param size Size of the data array.
/// @return Returns CRC of the data as 32-bit number.
uint32_t crc(uint8_t *data, uint8_t size)
{
    uint32_t poly = 0x04C11DB7;
    uint32_t crc = 0;

    for (int i = 0; i < size; i++)
    {
        uint32_t ch = data[i];
        crc = crc ^ (ch << 24);

        for (int j = 0; j < 8; j++)
        {
            if (crc & (1UL << 31))
            {
                crc = (crc << 1) ^ poly;
            }
            else
            {
                crc <<= 1;
            }
        }
    }

    return crc;
}

/// @brief Standart implementation of CRC algorithm.
/// @param data Pointer to the data array.
/// @param size Size of the data array.
/// @return Returns CRC of the data as 32-bit number.
uint32_t crc32b(uint8_t *data, uint8_t size)
{
    uint32_t poly = 0x04C11DB7;
    uint32_t crc = 0xFFFFFFFF;

    for (int i = 0; i < size; i++)
    {
        uint32_t ch = data[i];
        crc = crc ^ (ch << 24);

        for (int j = 0; j < 8; j++)
        {
            if (crc & (1UL << 31))
            {
                crc = (crc << 1) ^ poly;
            }
            else
            {
                crc <<= 1;
            }
        }
    }

    return ~crc;
}