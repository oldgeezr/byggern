/*
 * menu.h
 *
 * Created: 19.09.2014 14:12:21
 *  Author: chriram
 */ 

#ifndef MENU_H_
#define MENU_H_

#include <stdint.h>

typedef enum {
	PLAY,
	OPTIONS,
	INFO,
	MENU
} menu_options;

typedef enum {
	PLAY_NEWGAME,
	PLAY_CONTINUE,	
	PLAY_INGAME,
	PLAY_STOP
} play_options;

typedef enum {
	NEW_GAME = 'N',
	CONTINUE = 'C',
	NOP = '0'
} game_command;

void MENU_root(void);
void MENU_draw_menu(void);
void MENU_draw(void);
void MENU_select(menu_options option);
void MENU_draw_play(void);
void MENU_draw_options(void);
void MENU_draw_info(void);

#endif