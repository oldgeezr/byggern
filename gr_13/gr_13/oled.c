/*
 * oled.c
 *
 * Created: 16.09.2014 14:26:01
 *  Author: chriram
 */ 

#include "oled.h"

volatile char *command = (char *) 0x1000;
volatile char *data = (char *) 0x1200;

void OLED_init() {
	*command = 0xae; // display off
	*command = 0xa1; //segment remap
	*command = 0xda; //common pads hardware: alternative
	*command = 0x12;
	*command = 0xc8; //commonoutput scan direction:com63~com0
	*command = 0xa8; //multiplex ration mode:63
	*command = 0x3f;
	*command = 0xd5; //display divide ratio/osc. freq. mode
	*command = 0x80;
	*command = 0x81; //contrast control
	*command = 0x50;
	*command = 0xd9; //set pre-charge period
	*command = 0x21;
	*command = 0x20; //Set Memory Addressing Mode
	*command = 0x02;
	*command = 0xdb; //VCOM deselect level mode
	*command = 0x30;
	*command = 0xad; //master configuration
	*command = 0x00;
	*command = 0xa4; //out follows RAM content
	*command = 0xa6; //set normal display
	*command = 0xaf; // display on

	// *command = 0x40; // Set display start line 0 (to 63)
}

void OLED_start() {
	
	//Set memory addressing mode (page)
	*command = 0x20;
	*command = 0x10;
	//*command = 0x0b);
	
	//Set column address
	*command = 0x21;
	*command = 0x00;
	*command = 0x7f;
	
	//Set page address
	*command = 0x22;
	*command = 0x00;
	*command = 0x07;
	
}

void OLED_clear() {
	
	*command = 0x40; // Set display start line 0 (to 63)
	
	char start; // Set page start address of target display location
	
	for (uint8_t i = 0; i < 8; i++) {
		OLED_start();
		start = 0xB0 | i;
		*command = start;
		for (uint8_t j = 0; j < 128; j++) {
			*data = 0x00;
		}
	}
}

void OLED_set_pixel(uint8_t x, uint8_t y) {
	/* 128*64*/
	
	//Set column address
	*command = 0x21;
	*command = 0x00 | x;
	*command = 0x00 | (x + 1);
	
	uint8_t rest = y % 8;
	uint8_t page = (y - rest)/8;
	*command = (0xB0 | page);
	*data |= (1 << rest);
}

void OLED_test() {
	
	OLED_clear();
	
	char start;
	
	for (uint8_t i = 0; i < 8; i++) {
		OLED_start();
		start = 0xB0 | i;
		*command = start;
		for (uint8_t j = 0; j < 128; j++) {
			*data = 0x08;
			_delay_ms(10);
		}
	}
}