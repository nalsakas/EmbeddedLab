#ifndef __API_H__
#define __API_H__

#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define MAX(a, b) (((a) > (b)) ? (a) : (b))

extern uint8_t API_ADDRESS;   //< Holds source address
extern uint8_t API_BIND;      //< Holds destination address
extern uint8_t API_BROADCAST; //< Holds broadcast address
extern uint8_t API_DATA[100]; //< Holds data to be sent

void api_handle_line(uint8_t *line);
void api_handle_packet(RPL4_PACKET *packet);

#endif
