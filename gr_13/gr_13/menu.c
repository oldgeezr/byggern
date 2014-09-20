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
	
	static MENU_options option = PLAY;	
	static MENU_options previous_option = PLAY;	
	
	JOYSTICK_control direction = JOYSTICK_get_direction();
	
	if (direction == UP) {
		previous_option = option;
		option = (option + 1) % (N_OPTIONS);
		MENU_select(option);
		OLED_scroll_left(previous_option+3, previous_option+3);
		_delay_ms(500);
	}
	if (direction == DOWN) {
		previous_option = option;
		option = ((option - 1) + N_OPTIONS) % N_OPTIONS;
		MENU_select(option);
		OLED_scroll_left(previous_option+3, previous_option+3);
		_delay_ms(500);
	}
	if (direction == RIGHT) {
		if (option == PLAY) {
			OLED_clear();
		}
	}
}

void MENU_select(MENU_options option) {
	
	switch(option) {
		case PLAY:
		OLED_scroll_right(PLAY+3,PLAY+3);
		break;
		case OPTIONS:
		OLED_scroll_right(OPTIONS+3,OPTIONS+3);
		break;
		case INFO:
		OLED_scroll_right(INFO+3,INFO+3);
		break;
		case END:
		break;
	}
}

void MENU_draw() {
	
	uint8_t align = 36;
	
	OLED_clear();
	
	OLED_set_pages(1,1);
	OLED_set_columns(64-16,127);
	OLED_write_string("MENU");
	
	OLED_set_pages(3,3);
	OLED_set_columns(align,127);
	OLED_write_string("PLAY");
	OLED_scroll_right(3,3);
	
	OLED_set_pages(4,4);
	OLED_set_columns(align,127);
	OLED_write_string("OPTIONS");
	
	OLED_set_pages(5,5);
	OLED_set_columns(align,127);
	OLED_write_string("INFO");
	
}