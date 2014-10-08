/*
 * can.h
 *
 * Created: 30.09.2014 13:04:16
 *  Author: erlenhes
 */ 

#ifndef _CAN_H_
#define _CAN_H_

#include <stdint.h>

#define MCP_ABTF 6
#define MCP_MLOA 5
#define MCP_TXERR 4
#define MCP_TXREQ 3

typedef struct{
	unsigned int id;
	uint8_t length;
	uint8_t data[8];
} can_message_t;

typedef enum {
	MSG_LOST,
	TX_ERROR,
	MSG_ABORT,
	NO_ERROR,
	MSG_SENT
} CAN_status_flag;

void CAN_init(void);
CAN_status_flag CAN_msg_send(can_message_t *msg);
CAN_status_flag CAN_error();
uint8_t CAN_transmit_complete();
can_message_t CAN_data_receive();
//void CAN_int_vect();

#endif