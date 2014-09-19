/*
 * joystick.h
 *
 * Created: 15.09.2014 14:31:37
 *  Author: chriram
 */ 

#ifndef JOYSTICK_H_
#define JOYSTICK_H_

typedef enum {
	LEFT = 0,
	RIGHT = 1,
	UP = 2,
	DOWN = 3,
	NEUTRAL = 4,
	BTN_DOWN = 5
} JOYSTICK_control;

void JOYSTICK_init();
JOYSTICK_control JOYSTICK_get_direction();

#endif