/*
 * menu.c
 *
 * Created: 19.09.2014 14:12:12
 *  Author: chriram
 */ 

#ifndef F_CPU
#define F_CPU 4915200
#endif

#include "menu.h"
const uint8_t N_OPTIONS = END - PLAY;

#include "oled.h"
#include "joystick.h"
#include <stdlib.h>
#include <util/delay.h>

void MENU_root() {
	
	_delay_ms(100);
	
	uint8_t init = 1;
	
	static MENU_options option = PLAY;	
	static MENU_options previous = PLAY;	
	
	JOYSTICK_control direction = JOYSTICK_get_direction();
	
	previous = option;	
	if (direction == UP) {
		option = (option + 1) % (N_OPTIONS);
		MENU_select(option);
		if (!init) {
			OLED_scroll_left(previous+2,previous+2);
		}
		init = 0;
		_delay_ms(500);
	}
	if (direction == DOWN) {
		option = ((option - 1) + N_OPTIONS) % N_OPTIONS;
		MENU_select(option);
		if (!init) {
			OLED_scroll_left(previous+2,previous+2);
		}
		init = 0;
		_delay_ms(500);
	}
}

void MENU_select(MENU_options option) {
	
	switch(option) {
		case PLAY:
		OLED_scroll_right(PLAY+2,PLAY+2);
		break;
		case OPTIONS:
		OLED_scroll_right(OPTIONS+2,OPTIONS+2);
		break;
		case INFO:
		OLED_scroll_right(INFO+2,INFO+2);
		break;
		case END:
		break;
	}
}

void MENU_draw() {
	OLED_clear();
	
	OLED_set_pages(1,1);
	OLED_set_columns(64-16,127);
	OLED_write_string("MENU");
	
	OLED_set_pages(3,3);
	OLED_set_columns(64-16,127);
	OLED_write_string("PLAY");
	
	OLED_set_pages(4,4);
	OLED_set_columns(64-28,127);
	OLED_write_string("OPTIONS");
	
	OLED_set_pages(5,5);
	OLED_set_columns(64-16,127);
	OLED_write_string("INFO");
	
}