/*
 * oled.c
 *
 * Created: 16.09.2014 14:26:01
 *  Author: chriram
 */ 

#include "oled.h"
#include "font_8x8.h"
#include <string.h>

#ifndef F_CPU
#define F_CPU 4915200
#endif

#include <util/delay.h>

volatile char *command = (char *) 0x1000;
volatile char *data = (char *) 0x1200;

void OLED_init(void) {
	*command = SET_DISPLAY_POWER_OFF;
	
	*command = SET_SEGMENT_REMAP_127;
	*command = 0xda; //common pads hardware: alternative
	
	*command = 0x12;
	*command = 0xc8; //commonoutput scan direction:com63~com0
	
	*command = SET_MULTIPLEX_RATIO; //multiplex ration mode:63
	*command = 0x3f;
	
	*command = SET_DISPLAY_CLOCK; //display divide ratio/osc. freq. mode
	*command = 0x80;
	
	*command = SET_CONTRAST; //contrast control
	*command = 0x50;
	
	*command = SET_PRECHARGE_PERIOD; //set pre-charge period
	*command = 0x21;
	
	*command = SET_MEMORY_ADDRESSING_MODE;
	*command = 0x02; //Select page addressing mode
	
	*command = SET_VCOMH_DESELECT_LEVEL;
	*command = 0x30;
	
	*command = SET_IREF_SELECTION;
	*command = 0x00; //Use external
	
	*command = SET_DISPLAY_GDDRAM;
	*command = SET_NORMAL_DISPLAY;
	*command = SET_DISPLAY_POWER_ON;
}

void OLED_start(void) {
	*command = SET_MEMORY_ADDRESSING_MODE;
	*command = 0x02; //Select page addressing mode
	
	*command = SET_COLUMN_ADDRESS;
	*command = 0x00; //Start address
	*command = 0x7f; //Stop address
	
	*command = SET_PAGE_ADDRESS;
	*command = 0x00; //Start address
	*command = 0x07; //Stop address
}

void OLED_clear(void) {
	*command = DATA_MODE; // Set display start line 0 (to 63)
	
	char start; // Set page start address of target display location
	
	for (uint8_t i = 0; i < PAGES; i++) {
		OLED_start();
		start = 0xB0 | i;
		*command = start;
		for (uint8_t j = 0; j < COLUMNS; j++) {
			*data = 0x00;
		}
	}
}

void OLED_fill(void) {
	*command = DATA_MODE; // Set display start line 0 (to 63)
	
	char start; // Set page start address of target display location
	
	for (uint8_t i = 0; i < PAGES; i++) {
		OLED_start();
		start = 0xB0 | i;
		*command = start;
		for (uint8_t j = 0; j < COLUMNS; j++) {
			*data = 0xFF;
		}
	}
}

void OLED_write_char(uint8_t character) {
	uint8_t i = 0;
	for (i = 0; i < PAGES; i++) {
		*data = pgm_read_byte(&font8[character-32][i]);
	}
}

void OLED_write_string(char *str) {
	while(*str) {
		OLED_write_char(*str++);
	}
}

static inline void OLED_set_pages(uint8_t page_start, uint8_t page_end) {
	*command = SET_PAGE_ADDRESS;
	*command = 0xB0 | page_start; //Start address
	*command = 0xB0 | page_end; //End address
}

static inline void OLED_set_columns(uint8_t col_start, uint8_t col_end) {
	*command = SET_COLUMN_ADDRESS;
	*command = col_start; //Start address
	*command = col_end; //End address
}

void OLED_write_align_left(uint8_t x, uint8_t y, char *str) {
	
	//Guards
	x %= COLUMNS;
	y %= PAGES;
	
	//uint8_t offset = strlen(str)*4;
	
	OLED_set_pages(y,y);
	OLED_set_columns(x,127);
	OLED_write_string(str);
}

void OLED_write_align_center(uint8_t x, uint8_t y, char *str) {
	
	//Guards
	x %= COLUMNS;
	y %= PAGES;
	
	uint8_t offset = strlen(str)*4;
	
	OLED_set_pages(y,y);
	OLED_set_columns(x-offset,127);
	OLED_write_string(str);
}

void OLED_scroll_page_right(uint8_t page,uint8_t offset) {
	*command = SET_DEACTIVATE_SCROLL;
	
	*command = SET_RIGHT_HORIZONTAL_SCROLL;
	*command = 0x00; //Dummy byte A
	*command = page + offset;
	*command = 0b100; //Speed
	*command = page + offset;
	*command = 0x00;
	*command = 0xFF;
	
	*command = SET_ACTIVATE_SCROLL;
	_delay_ms(100);
	*command = SET_DEACTIVATE_SCROLL;
}

void OLED_scroll_page_left(uint8_t page,uint8_t offset) {
	
	*command = SET_DEACTIVATE_SCROLL;
	
	*command = SET_LEFT_HORIZONTAL_SCROLL;
	*command = 0x00; //Dummy byte A
	*command = page + offset;
	*command = 0b100; //Speed
	*command = page + offset;
	*command = 0x00;
	*command = 0xFF;
	
	*command = SET_ACTIVATE_SCROLL;
	_delay_ms(100);
	*command = SET_DEACTIVATE_SCROLL;
}