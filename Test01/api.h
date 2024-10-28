#ifndef __API_H__
#define __API_H__

extern uint8_t API_ADDRESS;
extern uint8_t API_BIND;
extern uint8_t API_BROADCAST;
extern uint8_t API_DATA[250];

void api_handle_line(uint8_t *line);
void api_handle_packet();

#endif
