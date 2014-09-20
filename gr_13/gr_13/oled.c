/*
 * oled.c
 *
 * Created: 16.09.2014 14:26:01
 *  Author: chriram
 */ 

#include "oled.h"
#include "font_8x8.h"

#ifndef F_CPU
#define F_CPU 4915200
#endif

#include <util/delay.h>

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
	*command = 0x02;
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


void OLED_write_char(uint8_t character) {
	
	uint8_t i = 0;
	for (i = 0; i < 8; i++) {
		*data = pgm_read_byte(&font8[character-32][i]);
	}
	
}

void OLED_set_pages(uint8_t y0, uint8_t y1) {
	*command = 0x22;
	*command = 0xB0 | y0;
	*command = 0xB0 | y1;
}

void OLED_set_columns(uint8_t x0, uint8_t x1) {
	*command = 0x21;
	*command = x0;
	*command = x1;
}

void OLED_scroll_right(uint8_t page_start,uint8_t page_stop) {
	*command = 0x2E; //Deactivate scroll
	
	*command = 0x26; //Vertical scroll setup
	*command = 0x00; //Dummy byte A
	*command = page_start; //Start: PAGE0
	*command = 0b100; //Speed
	*command = page_stop; //End: PAGE7
	*command = 0x00;
	*command = 0xFF;
	
	*command = 0x2F; //Start scroll
	_delay_ms(100);
	*command = 0x2E; //Deactivate scroll
}

void OLED_scroll_left(uint8_t page_start,uint8_t page_stop) {
	*command = 0x2E; //Deactivate scroll
	
	*command = 0x27; //Vertical scroll setup
	*command = 0x00; //Dummy byte A
	*command = page_start; //Start: PAGE0
	*command = 0b100; //Speed
	*command = page_stop; //End: PAGE7
	*command = 0x00;
	*command = 0xFF;
	
	*command = 0x2F; //Start scroll
	_delay_ms(100);
	*command = 0x2E; //Deactivate scroll
}

void OLED_write_string(char *str) {
	while(*str) {
		OLED_write_char(*str++);
	}
}