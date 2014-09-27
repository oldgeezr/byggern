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
} MENU_options;

/*typedef enum {
	M_MENU,
	M_PLAY,
	M_OPTIONS,
	M_INFO
} MENU_states;*/

void MENU_root();
void MENU_draw_menu();
void MENU_draw();
void MENU_select(MENU_options option);
void MENU_draw_play();
void MENU_draw_options();
void MENU_draw_info();
#endif