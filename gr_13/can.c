/*
 * can.c
 *
 * Created: 30.09.2014 13:04:09
 *  Author: erlenhes
 */ 

#include "can.h"
#include "mcp2515.h"

#ifndef F_CPU
#define F_CPU 4915200
#endif

#include <util/delay.h>

uint8_t rx_flag = 0;

#include <avr/interrupt.h>

void CAN_msg_send(can_message_t *msg) {
	if (CAN_transmit_complete()) {
		
		MCP2515_write(MCP_TXB0SIDH,(msg->id >> 3)); //Identifier (HIGH-bits)
		MCP2515_write(MCP_TXB0SIDL,(msg->id) << 5); //Identifier (LOW-bits)
		MCP2515_write(MCP_TXB0DLC,msg->length); //Data length
		
		for (uint8_t i = 0; i < msg->length; i++) {
			MCP2515_write(MCP_TXB0D0+i,msg->data[i]); //Data to send
		}
		
		MCP2515_rts(); //Request-to-send
	}
}

void CAN_init(void) {
	MCP2515_init();
}

uint8_t CAN_error() {
	uint8_t error = MCP2515_read(MCP_TXB0CTRL);
	
	if (error & 0x05) {
		return 1;
	} else if (error & 0x05) {
		return 2;
	}
	return 0;
}

uint8_t CAN_transmit_complete() {
	if (MCP2515_read(MCP_TXB0CTRL) & 0x04) {
		return 0;
	} else {
		return 1;
	}
}

can_message_t CAN_data_receive() {
	can_message_t msg;
	
	if (rx_flag == 1) {
		msg.id = (MCP2515_read(MCP_RXB0SIDH) << 3) | (MCP2515_read(MCP_RXB0SIDL) >> 5); //Get ID
		msg.length = (0x0F) & (MCP2515_read(MCP_RXB0DLC)); //Get data length
		
		for (uint8_t i = 0; i < msg.length; i++) {
			msg.data[i] = MCP2515_read(MCP_RXB0D0 + i); //Get data
		}
		
		rx_flag = 0;
	}
	
	return msg;
}

ISR(INT2_vect) {
	_delay_ms(10);
	MCP2515_bit_modify(MCP_CANINTF,0x01,0); //Clear flag
	rx_flag = 1; 
}