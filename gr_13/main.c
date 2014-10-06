/*
 * gr_13.c
 *
 * Created: 09.09.2014 11:35:43
 *  Author: chriram
 */ 

#ifndef F_CPU
#define F_CPU 4915200
#endif

#include <util/delay.h>
#include <avr/io.h>
#include <stdio.h>
#include <avr/interrupt.h>

#include "sram.h"
#include "uart.h"
#include "oled.h"
#include "menu.h"
#include "adc.h"
#include "joystick.h"
#include "can.h"

int main(void)
{
	usart_setup();
	//SRAM_init();
	//OLED_init();
	//MENU_draw_menu();
	//SRAM_test();
	//JOYSTICK_init();
	//SLIDER_init();
	CAN_init();
	
	GICR |= (1 << INT2);
	sei();
	
	for(;;) {
		can_message_t msg;
		can_message_t received_msg;
		
		msg.id = 155;
		msg.length = 3;
		msg.data[0] = 0x06;
		msg.data[1] = 0x06;
		msg.data[2] = 0x06;
		
		printf("Can msg sent: %d , %d , %d , %d , %d \n",msg.id, msg.length, msg.data[0],msg.data[1],msg.data[2]);
		
		CAN_msg_send(&msg);
		
		received_msg = CAN_data_receive();
		
		printf("Can msg received: %d , %d , %d , %d , %d \n",received_msg.id, received_msg.length, received_msg.data[0],received_msg.data[1],received_msg.data[2]);
		
		_delay_ms(500);
		
    }
}
