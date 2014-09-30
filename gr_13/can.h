/*
 * can.h
 *
 * Created: 30.09.2014 13:04:16
 *  Author: erlenhes
 */ 

#ifndef _MCP2515_H
#define _MCP2515_H

#include <stdint.h>

typedef struct{
	unsigned int id;
	uint8_t length;
	uint8_t data[8];
} can_message_t;

void CAN_msg_send(can_message_t *msg);
void CAN_init();
void CAN_error();
void CAN_transmit_complete();
void CAN_data_receive();
void CAN_int_vect();

#endif