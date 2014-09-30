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

#include "sram.h"
#include "uart.h"
#include "oled.h"
#include "menu.h"
#include "adc.h"
#include "joystick.h"
#include "MCP2515.h"

int main(void)
{
	usart_setup();
	SRAM_init();
	OLED_init();
	MENU_draw_menu();
	//SRAM_test();
	JOYSTICK_init();
	SLIDER_init();
	
	MCP2515_init();
	
	for(;;) {
		
		MENU_root();
		
		uint8_t result;
		//result = MCP2515_read_status();
		result = MCP2515_read(MCP_CNF3);
		printf("from SPI: %d \n",result);
		
		
		 //SLIDER_get_position();
		 //int8_t result = JOYSTICK_get_direction();
		 //printf("value: %d \n",result);
		_delay_ms(100);
    }
}
