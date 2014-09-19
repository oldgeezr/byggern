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
	END
} MENU_options;


void MENU_root();
void MENU_draw();
void MENU_select(MENU_options option);

#endif