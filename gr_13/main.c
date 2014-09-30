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
#include "spi.h"
#include "mcp2515.h"

int main(void)
{
	usart_setup();
	SRAM_init();
	OLED_init();
	MENU_draw_menu();
	SRAM_test();
	JOYSTICK_init();
	SLIDER_init();
	SPI_master_init();
	
	//PORTB = ~(1 << PB4); //Pull SS low
	
	for(;;) {
		
		//mcp2515_write(MCP_RX_STATUS);
		//mcp2515_read(0x00);
		
		MENU_root();
		//SPI_master_transmit(MCP_RX_STATUS);
		 //SLIDER_get_position();
		 //int8_t result = JOYSTICK_get_direction();
		 //printf("value: %d \n",result);
		//_delay_ms(100);
    }
}
