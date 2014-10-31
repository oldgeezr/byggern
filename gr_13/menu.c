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
const uint8_t P_OPTIONS = PLAY_INGAME - PLAY_NEWGAME;

#include "oled.h"
#include "joystick.h"
#include "can.h"
#include "uart.h"
#include <stdlib.h>
#include <util/delay.h>

uint8_t state = MENU;
uint8_t ingame = 0;
uint8_t lives = 3;

void MENU_run(void) {
	
	if (ingame){
		PLAY_root();
	} else {
		MENU_root();
	}
}

void MENU_root(void) {
	
	static menu_options option = PLAY;
	static menu_options previous_option = PLAY;
	
	joystick_control direction = JOYSTICK_get_direction();
	
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
		if (option == PLAY) {;
			ingame = 1;
		} 
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

void PLAY_root(void) {
	
	static play_options option = PLAY_NEWGAME;
	static play_options previous_option = PLAY_NEWGAME;
	static can_message_t carriage_msg;
	
	joystick_control direction = JOYSTICK_get_direction();
	
	carriage_msg = CAN_msg_receive();
	printf("Carriage_msg: %d\n", carriage_msg.data[0]);
	
	if (direction == UP || direction == DOWN) {
		previous_option = option;
		if (option == PLAY_NEWGAME) {
			option = PLAY_CONTINUE;
		} else {
			option = PLAY_NEWGAME;
		}
		PLAY_select(option);
		OLED_scroll_left(previous_option, previous_option);
		_delay_ms(250);
	}
	if (direction == RIGHT) {
		if (!(option == PLAY_CONTINUE && lives != 0)) { // Can only continue if lives left
			lives = 3;
			CAN_send_command('N'); // New game
		}
		CAN_send_command('C'); // Continue
		state = PLAY_INGAME;
		MENU_draw();
	}
	if (carriage_msg.id != EMPTY) {
		lives -= carriage_msg.data[0];
		state = PLAY_STOP;
		option = PLAY_NEWGAME;
		MENU_draw();
	}
	if (direction == BTN_DOWN) {
		// have to do some resets here
		state = MENU;
		ingame = 0; // Go back to menu root
		CAN_send_command('S'); // STOP game
		option = PLAY;
		previous_option = PLAY;
		MENU_draw();
	}
}

void MENU_select(menu_options option) {
	
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

void PLAY_select(play_options option) {
	
	switch(option) {
		case PLAY_CONTINUE:
			OLED_scroll_right(PLAY_CONTINUE,PLAY_CONTINUE);
			break;
		case PLAY_NEWGAME:
			OLED_scroll_right(PLAY_NEWGAME,PLAY_NEWGAME);
			break;
		case PLAY_INGAME:
			break;
		case PLAY_STOP:
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

void PLAY_draw_ingame(void) {
	
	OLED_clear();
	
	OLED_set_pages(1,1);
	OLED_set_columns(64-28,127);
	OLED_write_string("IN GAME");
	
	OLED_set_pages(4,4);
	OLED_set_columns(64-36,127);
	OLED_write_string("LIVES : ");
	OLED_write_char((lives + '0'));
	
}

void PLAY_draw_stop(void) {
	
	OLED_clear();
	
	OLED_set_pages(1,1);
	OLED_set_columns(64-40,127);
	OLED_write_string("YOU LOOSE!");
	
	OLED_set_pages(2,2);
	OLED_set_columns(64-52,127);
	OLED_write_string("LIVES LEFT: ");
	OLED_write_char((lives + '0'));
	
	OLED_set_pages(4,4);
	OLED_set_columns(64-32,127);
	OLED_write_string("NEW GAME");
	OLED_scroll_right(4,4);
	
	OLED_set_pages(5,5);
	OLED_set_columns(64-32,127);
	OLED_write_string("CONTINUE");
	
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
		case PLAY_INGAME:
			PLAY_draw_ingame();
			break;
		case PLAY_STOP:
			PLAY_draw_stop();
			break;
	}
}