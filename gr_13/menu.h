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
	INFO
} menu_options;

typedef enum {
	PLAY_NEWGAME,
	PLAY_CONTINUE,
} loose_options;

typedef enum {
	MANUAL,
	AUTO,
} game_options;

typedef enum {
	NEW_GAME = 'N',
	CONTINUE = 'C',
	NOC = '0'
} game_command;

typedef enum {
	MENU,
	GAME,
	LOOSE,
	INGAME
} menu_state_t;

void MENU_run(void);
void MENU_root(void);
void MENU_init(void);
void MENU_draw(menu_options option);
void MENU_select(menu_options option);
void MENU_draw_play(void);
void MENU_draw_options(void);
void MENU_draw_info(void);
void MENU_game(void);
void MENU_draw_mode_select(void);
void GAME_select(game_options option);
void MENU_loose(void);
void LOOSE_select(loose_options option);
void MENU_draw_ingame(void);
void MENU_ingame(void);
void MENU_draw_loose(void);

#endif