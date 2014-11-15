/*
 * can.c
 *
 * Created: 30.09.2014 13:04:09
 *  Author: erlenhes
 */ 

#include "can.h"
#include "mcp2515.h"
#include "joystick.h"
#include "uart.h"

#ifndef F_CPU
#define F_CPU 4915200
#endif

#include <util/delay.h>
#include <avr/interrupt.h>

uint8_t rx_flag = 0;

void CAN_init(void) {
	MCP2515_init();
}

can_status_flag CAN_msg_send(can_message_t *msg) {
	if (CAN_transmit_complete() == 1) {
		
		MCP2515_write(MCP_TXB0SIDH,(msg->id >> 3)); //Identifier (HIGH-bits)
		MCP2515_write(MCP_TXB0SIDL,(msg->id) << 5); //Identifier (LOW-bits)
		MCP2515_write(MCP_TXB0DLC,msg->length); //Data length
		
		for (uint8_t i = 0; i < msg->length; i++) {
			MCP2515_write(MCP_TXB0D0+i,msg->data[i]); //Data to send
		}
		
		MCP2515_rts(); //Request-to-send
		
	} else {
		if (CAN_error() == MSG_LOST || CAN_error() == TX_ERROR) {
			return MSG_ABORT;
		}
	}
	return MSG_SENT;
}

void CAN_send_command(uint8_t cmd) {
	can_message_t msg;
	
	msg.id = 100;
	msg.length = 1;
	msg.data[0] = cmd;
	
	CAN_msg_send(&msg);
}

void CAN_send_hid(void) {
	can_message_t msg;
	slider_position position;
	
	position = SLIDER_get_position();
	
	msg.id = 155;
	msg.length = 2;
	msg.data[0] = position.left_pos;
	msg.data[1] = position.right_pos;
	
	CAN_msg_send(&msg);
	
	_delay_ms(10);
}

can_message_t CAN_msg_receive(void) {
	can_message_t msg;
	
	if (rx_flag == 1) {
		msg.id = (MCP2515_read(MCP_RXB0SIDH) << 3) | (MCP2515_read(MCP_RXB0SIDL) >> 5); //Get ID
		msg.length = (0x0F) & (MCP2515_read(MCP_RXB0DLC)); //Get data length
		
		for (uint8_t i = 0; i < msg.length; i++) {
			msg.data[i] = MCP2515_read(MCP_RXB0D0 + i); //Get data
		}
		rx_flag = 0;
	} else {
		msg.id = EMPTY;
	}
	
	return msg;
}

uint8_t CAN_transmit_complete(void) {
	uint8_t status = MCP2515_read(MCP_TXB0CTRL);
	
	if (status & (1 << MCP_TXREQ)) {
		return 0;
	} else {
		return 1;
	}
}

can_status_flag CAN_error(void) {
	uint8_t status = MCP2515_read(MCP_TXB0CTRL);
	
	if (status & (1 << MCP_MLOA)) {
		return MSG_LOST;
	} else if (status & (1 << MCP_TXERR)) {
		return TX_ERROR;
	}
	return NO_ERROR;
}

ISR(INT2_vect) {
	MCP2515_bit_modify(MCP_CANINTF,0x01,0); //Clear flag
	rx_flag = 1;
}

/*
 * Test functions
*/

void CAN_test_loopback_msg(void) {
	can_message_t msg;
	can_message_t received_msg;
	slider_position position;
	
	position = SLIDER_get_position();
	
	msg.id = 155;
	msg.length = 2;
	msg.data[0] = position.left_pos;
	msg.data[1] = position.right_pos;
	
	CAN_msg_send(&msg);
	
	_delay_ms(10);
	
	received_msg = CAN_msg_receive();
	printf("CAN RX - ID: %d - LENGTH: %d - DATA: ", received_msg.id, received_msg.length);
	uint8_t i;
	for (i = 0; i < received_msg.length; i++)
	{
		printf("%d ,", received_msg.data[i]);
	}
	printf("\n");
}

void CAN_test_receive(void) {
	can_message_t received_msg;
	
	received_msg = CAN_msg_receive();
	printf("CAN RX - ID: %d - LENGTH: %d - DATA: ", received_msg.id, received_msg.length);
	uint8_t i;
	for (i = 0; i < received_msg.length; i++)
	{
		printf("%d ,", received_msg.data[i]);
	}
	printf("\n");
}

void CAN_test_msg_normal_mode(void) {
	can_message_t msg;
	
	joystick_control joy_control;
	slider_position position;
	slider_btn_state btn_state;
	
	position = SLIDER_get_position();
	joy_control = JOYSTICK_get_direction();
	btn_state = SLIDER_get_btn_state();
	
	msg.id = 155;
	msg.length = 8;
	
	msg.data[0] = '0'; //Command
	msg.data[1] = joy_control;
	msg.data[2] = position.right_pos;
	msg.data[3] = btn_state;
	msg.data[4] = 0;
	msg.data[5] = 0;
	msg.data[6] = 0;
	msg.data[7] = 0;
	
	// printf("CAN msg: %d, %d, %d \n", msg.data[1], msg.data[2], msg.data[3]);
	
	CAN_msg_send(&msg);
	
}