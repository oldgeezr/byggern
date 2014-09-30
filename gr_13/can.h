/*
 * can.h
 *
 * Created: 30.09.2014 13:04:16
 *  Author: erlenhes
 */ 


/* Names

#ifndef _MCP2515_H
#define _MCP2515_H

typedef struct{
	unsigned int id;
	uint8_t length;
	uint8_t data[8];
} can_message_t;

void can_init();
void can_message_send();
void can_error();
void can_transmit_complete();
void can_data_receive();
void can_int_vect();

#endif
*/