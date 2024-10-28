#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

void uart_send_bin(uint32_t num, uint8_t *buf)
{
    itoa(num, buf, 2);
}

void uart_send_num(uint32_t num, uint8_t *buf)
{
    itoa(num, buf, 10);
}

void uart_send_hex(uint32_t num, uint8_t *buf)
{
    itoa(num, buf, 16);
}

// TEST INTEGER TO ACSII FUNCTIONS
int main()
{
    uint8_t buf[33];
    memset(buf, 0, 33);

    uint32_t num = 0xCAFFEE;

    uart_send_bin(num, buf);
    printf("%s\n", buf);

    uart_send_num(num, buf);
    printf("%s\n", buf);

    uart_send_hex(num, buf);
    printf("%s\n", buf);

    return 0;
}