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
	PLAY = 0,
	OPTIONS = 1,
	INFO = 2,
	MENU = 3
} menu_options;

typedef enum {
	PLAY_NEWGAME = 4,
	PLAY_CONTINUE = 5,	
	PLAY_INGAME = 6,
	PLAY_STOP = 7
} play_options;

typedef enum {
	NEW_GAME = 'N',
	CONTINUE = 'C',
	NOC = '0'
} game_command;

void MENU_run(void);
void MENU_root(void);
void MENU_draw_menu(void);
void MENU_draw(void);
void MENU_select(menu_options option);
void MENU_draw_play(void);
void MENU_draw_options(void);
void MENU_draw_info(void);
void PLAY_root(void);
void PLAY_select(play_options option);
void PLAY_draw_ingame(void);
void PLAY_draw_stop(void);

#endif