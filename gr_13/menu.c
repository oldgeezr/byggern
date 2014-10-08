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
const uint8_t N_OPTIONS = MENU - PLAY;

#include "oled.h"
#include "joystick.h"
#include <stdlib.h>
#include <util/delay.h>

MENU_options state = MENU;

void MENU_root(void) {
	
	static MENU_options option = PLAY;	
	static MENU_options previous_option = PLAY;	
	
	JOYSTICK_control direction = JOYSTICK_get_direction();
	
	if (direction == UP) {
		previous_option = option;
		option = (option + 1) % (N_OPTIONS);
		MENU_select(option);
		OLED_scroll_left(previous_option+3, previous_option+3);
		_delay_ms(250);
	}
	if (direction == DOWN) {
		previous_option = option;
		option = ((option - 1) + N_OPTIONS) % N_OPTIONS;
		MENU_select(option);
		OLED_scroll_left(previous_option+3, previous_option+3);
		_delay_ms(250);
	}
	if (direction == RIGHT) {
		state = option;
		MENU_draw();
	}
	if (direction == BTN_DOWN) {
		state = MENU;
		option = PLAY;
		previous_option = PLAY;
		MENU_draw();
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
		case MENU:
			break;
	}
}

void MENU_draw_menu(void) {
	
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

void MENU_draw_play(void) {
	
	OLED_clear();
	
	OLED_set_pages(3,3);
	OLED_set_columns(64-40,127);
	OLED_write_string("PLAYING...");
}

void MENU_draw_options(void) {
	
	OLED_clear();
	
	OLED_set_pages(3,3);
	OLED_set_columns(64-48,127);
	OLED_write_string("There are no");
	OLED_set_pages(4,4);
	OLED_set_columns(64-56,127);
	OLED_write_string("options at the");
	OLED_set_pages(5,5);
	OLED_set_columns(64-24,127);
	OLED_write_string("moment");
}

void MENU_draw_info(void) {
	
	uint8_t align = 36;
	
	OLED_clear();
	
	OLED_set_pages(2,2);
	OLED_set_columns(align,127);
	OLED_write_string("Made by:");
	OLED_set_pages(4,4);
	OLED_set_columns(64-64,127);
	OLED_write_string("Christoffer RE &");
	OLED_set_pages(5,5);
	OLED_set_columns(64-32,127);
	OLED_write_string("Erlend H");
}

void MENU_draw(void) {
	
	switch (state) {
		case MENU:
			MENU_draw_menu();
			break;
		case PLAY:
			MENU_draw_play();
			break;
		case OPTIONS:
			MENU_draw_options();
			break;
		case INFO:
			MENU_draw_info();
			break;
	}
}