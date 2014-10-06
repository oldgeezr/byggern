/*
 * can.h
 *
 * Created: 30.09.2014 13:04:16
 *  Author: erlenhes
 */ 

#ifndef _CAN_H
#define _CAN_H

#include <stdint.h>

typedef struct{
	unsigned int id;
	uint8_t length;
	uint8_t data[8];
} can_message_t;

void CAN_msg_send(can_message_t *msg);
void CAN_init();
uint8_t CAN_error();
uint8_t CAN_transmit_complete();
can_message_t CAN_data_receive();
void CAN_int_vect();

#endif