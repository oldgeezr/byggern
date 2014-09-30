/*
 * can.c
 *
 * Created: 30.09.2014 13:04:09
 *  Author: erlenhes
 */ 

#include "can.h"
#include "mcp2515.h"

void CAN_msg_send(can_message_t *msg) {
	/*while(*msg) {
		MCP2515_write(0x00,*msg++);	
	}*/
}

void CAN_init(void) {
	
}