#include <stdio.h>
#include <stdint.h>
#include <string.h>

// TEST CRC FUCTIONS

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

int main()
{
    uint8_t data1[] = {0x74, 0x65, 0x73, 0x74, 0x0}; // b70ed28
    uint8_t data2[] = {0x2b, 0x64, 0xe5, 0xe1, 0x0}; // 8c1436e0
    uint8_t data3[] = {0xab, 0xcd, 0x12, 0x03, 0x0}; // fa706aae

    uint32_t crc1 = crc(data1, strlen(data1));
    uint32_t crc2 = crc(data2, strlen(data2));
    uint32_t crc3 = crc(data3, strlen(data3));

    if (crc1 == 0xb70ed28)
    {
        printf("%x is passed\n", crc1);
    }
    else
    {
        printf("%x is failed\n", crc1);
    }

    if (crc2 == 0x8c1436e0)
    {
        printf("%x is passed\n", crc2);
    }
    else
    {
        printf("%x is failed\n", crc2);
    }

    if (crc3 == 0xfa706aae)
    {
        printf("%x is passed\n", crc3);
    }
    else
    {
        printf("%x is failed\n", crc3);
    }

    uint8_t *hello = "hello world";
    printf("CRC for string \"%s\" and length %d is %x", hello, strlen(hello), crc(hello, strlen(hello)));

    return 0;
}