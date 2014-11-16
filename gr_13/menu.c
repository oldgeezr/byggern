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
#include "uart.h"
const uint8_t N_OPTIONS = 3;
const uint8_t G_OPTIONS = 2;

#include "oled.h"
#include "joystick.h"
#include "can.h"
#include <stdlib.h>
#include <util/delay.h>

menu_state_t menu_state = MENU;
uint8_t lives = 3;

// Menu state machine
void MENU_run(void) {
	
	switch (menu_state) {
		case MENU:
			MENU_root();
			break;
		case GAME:
			MENU_game();
			break;
		case INGAME:
			MENU_ingame();
			break;
		case LOOSE:
			MENU_loose();
			break;
	}
}

// Main menu
void MENU_root(void) {
	
	static menu_options option = PLAY;
	static menu_options previous_option = PLAY;
	
	joystick_control direction = JOYSTICK_get_direction();
	// printf("Joystick btn = %d\n", direction);
	
	if (direction == UP) {
		previous_option = option;
		option = (option + 1) % (N_OPTIONS);
		MENU_select(option);
		OLED_scroll_page_left(previous_option,3);
		// printf("MENU_root option = %d, n_option = %d\n", option, N_OPTIONS);
		_delay_ms(250);
	}
	if (direction == DOWN) {
		previous_option = option;
		option = ((option - 1) + N_OPTIONS) % N_OPTIONS;
		// printf("MENU_root option = %d, n_option = %d\n", option, N_OPTIONS);
		MENU_select(option);
		OLED_scroll_page_left(previous_option,3);
		_delay_ms(250);
	}
	if (direction == RIGHT) {
		if (option == PLAY) {;
			menu_state = GAME;
			MENU_draw_mode_select();
			_delay_ms(250);
		} else {
			MENU_draw(option);
		}
	}
	if (direction == BTN_DOWN) {
		menu_state = MENU;
		option = PLAY;
		previous_option = PLAY;
		MENU_draw(MENU);
	}
}

// Game select menu
void MENU_game(void) {
	
	static game_options option = MANUAL;
	static game_options previous_option = MANUAL;
	
	joystick_control direction = JOYSTICK_get_direction();
	
	if (direction == UP || direction == DOWN) {
		previous_option = option;
		if (option == MANUAL) {
			option = AUTO;
		} else {
			option = MANUAL;
		}
		GAME_select(option);
		OLED_scroll_page_left(previous_option,3);
		_delay_ms(250);
	}
	if (direction == RIGHT) {
		if (option == MANUAL) {
			// Send manual command CAN
			CAN_send_command('M');
			// printf("M\n");
			option = MANUAL;
			previous_option = MANUAL;
		} else if (AUTO) {
			// Send auto command CAN
			CAN_send_command('A');
			// printf("A\n");
			option = MANUAL;
			previous_option = MANUAL;
		}
		menu_state = INGAME;
		MENU_draw_ingame();
		_delay_ms(250); // Delay for double click
	}
	if (direction == BTN_DOWN) {
		menu_state = MENU;
		option = MANUAL;
		previous_option = MANUAL;
		MENU_draw(MENU);
	}
}

// Ingame menu 
void MENU_ingame(void) {
	
	static can_message_t carriage_msg;
	carriage_msg = CAN_msg_receive();
	printf("CAN msg from node 2 = %d\n", carriage_msg.data[0]);
	
	// Receive score
	if (carriage_msg.id == 'L') {
		lives -= carriage_msg.data[0];
		menu_state = LOOSE;
		MENU_draw_loose();
	} else {
		if (CAN_send_controls()) {
			CAN_send_command('S'); // STOP game
			CAN_send_command('S'); // STOP game
			CAN_send_command('S'); // STOP game
			menu_state = MENU;
			MENU_draw(MENU);
		}
	}
}

// Select new game or continue 
void MENU_loose(void) {
	
	static loose_options option = PLAY_NEWGAME;
	static loose_options previous_option = PLAY_NEWGAME;
	
	joystick_control direction = JOYSTICK_get_direction();
	
	if (direction == UP || direction == DOWN) {
		previous_option = option;
		if (option == PLAY_NEWGAME) {
			option = PLAY_CONTINUE;
		} else {
			option = PLAY_NEWGAME;
		}
		LOOSE_select(option);
		OLED_scroll_page_left(previous_option,4);
		_delay_ms(250);
	}
	if (direction == RIGHT) {
		if (!(option == PLAY_CONTINUE && lives != 0)) { // Can only continue if lives left
			lives = 3;
			CAN_send_command('N'); // New game
		}
		CAN_send_command('C'); // Continue
		menu_state = INGAME;
		option = PLAY_NEWGAME;
		previous_option = PLAY_NEWGAME;
		MENU_draw_ingame();
		_delay_ms(250); // Delay for double click
	}
	if (direction == BTN_DOWN) {
		menu_state = MENU;
		option = PLAY_NEWGAME;
		previous_option = PLAY_NEWGAME;
		CAN_send_command('S'); // STOP game
		MENU_draw(MENU);
	}
}

void MENU_select(menu_options option) {
	
	switch(option) {
		case PLAY:
			OLED_scroll_page_right(PLAY,3);
			break;
		case OPTIONS:
			OLED_scroll_page_right(OPTIONS,3);
			break;
		case INFO:
			OLED_scroll_page_right(INFO,3);
			break;
	}
}

void GAME_select(game_options option) {
	
	switch(option) {
		case MANUAL:
			OLED_scroll_page_right(MANUAL,3);
			break;
		case AUTO:
			OLED_scroll_page_right(AUTO,3);
			break;
	}
}

void LOOSE_select(loose_options option) {
	
	switch(option) {
		case PLAY_CONTINUE:
			OLED_scroll_page_right(PLAY_CONTINUE,4);
			break;
		case PLAY_NEWGAME:
			OLED_scroll_page_right(PLAY_NEWGAME,4);
			break;
	}
}

void MENU_init(void) {
	OLED_clear();
	OLED_write_align_center(64,1,"MENU");
	OLED_write_align_left(36,3,"PLAY");
	OLED_scroll_page_right(3,0);
	OLED_write_align_left(36,4,"OPTIONS");
	OLED_write_align_left(36,5,"INFO");
}

void MENU_draw_play(void) {
	OLED_clear();
	OLED_write_align_center(64,3,"PLAYING...");
}

void MENU_draw_options(void) {
	OLED_clear();
	OLED_write_align_center(64,3,"There are no");
	OLED_write_align_center(64,4,"options at the");
	OLED_write_align_center(64,5,"moment");
}

void MENU_draw_info(void) {
	OLED_clear();
	OLED_write_align_left(36,2,"Made by:");
	OLED_write_align_center(64,4,"Christoffer RE &");
	OLED_write_align_center(64,5,"Erlend H");
}

void MENU_draw_mode_select(void) {
	OLED_clear();
	OLED_write_align_left(36,3,"MANUAL");
	OLED_scroll_page_right(3,0);
	OLED_write_align_left(36,4,"AUTO");
}

void MENU_draw_ingame(void) {
	OLED_clear();
	OLED_write_align_center(64,1,"IN GAME");
	OLED_write_align_center(64,4,"LIVES : ");
	OLED_write_char((lives + '0'));
}

void MENU_draw_loose(void) {
	OLED_clear();
	OLED_write_align_center(64,1,"YOU LOOSE!");
	OLED_write_align_center(64,2,"LIVES LEFT: ");
	OLED_write_char((lives + '0'));
	OLED_write_align_center(64,4,"NEW GAME");
	OLED_scroll_page_right(4,0);
	OLED_write_align_center(64,5,"CONTINUE");
}

void MENU_draw(menu_options option) {
	
	switch (option) {
		case MENU:
			MENU_init();
			break;
		case OPTIONS:
			MENU_draw_options();
			break;
		case INFO:
			MENU_draw_info();
			break;
		default:
			MENU_init();
			break;
	}
}