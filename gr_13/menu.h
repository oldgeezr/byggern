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

void MENU_root(void);
void MENU_draw_menu(void);
void MENU_draw(void);
void MENU_select(MENU_options option);
void MENU_draw_play(void);
void MENU_draw_options(void);
void MENU_draw_info(void);
#endif