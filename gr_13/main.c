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
	SRAM_init();
	OLED_init();
	MENU_draw_menu();
	//SRAM_test();
	JOYSTICK_init();
	SLIDER_init();
	CAN_init();
	
	GICR |= (1 << INT2); //For CAN rxbuf
	sei();
	
	for(;;) {
		
		//MENU_root();
		
		//SLIDER_get_position();
		
		CAN_test_msg_normal_mode();
		//CAN_test_receive();
	
		//_delay_ms(10);
    }
}
