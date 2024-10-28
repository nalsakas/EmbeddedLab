#include "crc.h"

/**
 * CRC32b
 */
uint32_t crc(uint8_t *data, uint8_t size)
{
    uint32_t poly = 0x04C11DB7;
    uint32_t crc = 0xFFFFFFFF;
    int n = 0;

    while (n < size)
    {
        uint32_t ch = data[n];
        crc = crc ^ (ch << 24);

        for (int i = 0; i < 8; i++)
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
        n = n + 1;
    }

    return ~crc;
}

/*
uint32_t crc(char *str)
{
    uint32_t polynomial = 0xEDB88320;
    uint32_t crc = ~0;
    char *p = str;

    while (*p != '\0')
    {
        crc ^= *p++;

        for (int i = 0; i < 8; i++)
        {
            uint32_t t = ~((crc & 1) - 1);
            crc = (crc >> 1) ^ (polynomial & t);
        }
    }

    return ~crc;
}
*/