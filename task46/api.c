#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "rpl1.h"
#include "rpl4.h"
#include "uart.h"
#include "api.h"

uint8_t API_ADDRESS = 0;   ///< Source Address
uint8_t API_BIND = 0;      ///< Destination Address
uint8_t API_BROADCAST = 0; ///< Broadcast Address
uint8_t API_DATA[100] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; ///< Data to send

/// @brief Format and sends incoming massage to console.
/// @param packet Message to be showed.
void api_handle_packet(RPL4_PACKET *packet)
{
    uart_send_str("R ");

    uart_send_num(rpl4_rcache_find(packet->identification), 0);
    uart_send(' ');

    uart_send_num(strlen(packet->payload), 0);
    uart_send(' ');

    uart_send_str(packet->payload);
    uart_send('\n');
}

/// @brief Sends current transmit status of the node to the console.
/// Status is either BUSY or OK.
void api_status()
{
    uart_send_str("Data: ");
    uart_send_str(API_DATA);
    uart_send('\n');

    uart_send_str("Address: ");
    uart_send_num(API_ADDRESS, 1);
    uart_send_str("Bind: ");
    uart_send_num(API_BIND, 1);

    if (TX_BUSY)
    {
        uart_send_str("BUSY\n");
    }
    else
    {
        uart_send_str("OK\n");
    }
}

/// @brief Sends the message through layer 4 and sends result to the console.
void api_push()
{
    if (!TX_BUSY)
    {
        rpl4_send(API_ADDRESS, API_BIND, 0, API_DATA);
        uart_send_str("Y\n");
    }
    else
    {
        uart_send_str("N\n");
    }
}

/// @brief Parses the line and extracts the command. Sends result of the action to the console.
/// @param line Line to be parsed.
/// @param command Pointer to the command variable. Extracted value will be put inside this variable.
void api_parse_command(uint8_t **line, uint8_t *command)
{
    while (!isalnum(**line) && **line != 0)
    {
        (*line)++;
    }

    if (**line == 'A' || **line == 'S' || **line == 'D' || **line == 'P' || **line == 'B')
    {
        *command = **line;
        (*line)++;
    }
}

/// @brief Parses the line and extracts the integer value. Sends result of the action to the console.
/// @param line Line to be parsed.
/// @param pval Pointer to the integer variable. Extracted value will be put inside this variable.
void api_parse_int(uint8_t **line, uint8_t *pval)
{
    uint8_t buffer[] = {0, 0, 0, 0};

    while (!isdigit(**line) && **line != 0)
    {
        (*line)++;
    }

    if (isdigit(**line))
    {
        uint8_t *start = *line;
        while (isdigit(**line) && **line != 0)
        {
            (*line)++;
        }

        uint8_t *end = *line;
        memcpy(buffer, start, end - start);
        *pval = atoi(buffer);
        uart_send_str("Y\n");
        return;
    }
    uart_send_str("N\n");
}

/// @brief Parses the line and extracts the string value. Sends result of the action to the console.
/// @param line Line to be parsed.
/// @param data Address of the buffer. Extracted string will be put inside this buffer.
void api_parse_data(uint8_t **line, uint8_t data[])
{
    memset(API_DATA, 0, sizeof API_DATA / sizeof API_DATA[0]);

    while (!isalnum(**line) && **line != 0)
    {
        (*line)++;
    }

    if (**line != 0)
    {
        uint8_t *start = *line;
        while (**line != 0)
        {
            (*line)++;
        }
        uint8_t *end = *line;

        memcpy(data, start, MIN(end - start, 100));
        uart_send_str("Y\n");
        return;
    }
    uart_send_str("N\n");
}

/// @brief Parses the line and depending on the command it contains calls appropriate methods.
/// Sends result of the action to the console.
/// @param line Line to be parsed.
void api_handle_line(uint8_t *line)
{
    uint8_t **pline = &line;
    uint8_t command = '\0';
    api_parse_command(pline, &command);

    switch (command)
    {
    case 'A':
        api_parse_int(pline, &API_ADDRESS);
        break;
    case 'S':
        api_status();
        break;
    case 'D':
        api_parse_data(pline, &API_DATA[0]);
        break;
    case 'B':
        api_parse_int(pline, &API_BIND);
        break;
    case 'P':
        api_push();
        break;
    default:
        uart_send_str("N\n");
        break;
    }
}
