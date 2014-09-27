/*
 * joystick.h
 *
 * Created: 15.09.2014 14:31:37
 *  Author: chriram
 */ 

#ifndef JOYSTICK_H_
#define JOYSTICK_H_

#include <stdint.h>

uint8_t mid_x, mid_y;

typedef enum {
	LEFT = 0,
	RIGHT = 1,
	UP = 2,
	DOWN = 3,
	NEUTRAL = 4,
	BTN_DOWN = 5
} JOYSTICK_control;

typedef struct {
	uint8_t left_pos;
	uint8_t right_pos;
} SLIDER_position;

void JOYSTICK_calibrate();
void JOYSTICK_init();
int8_t JOYSTICK_get_position(JOYSTICK_control axis);
JOYSTICK_control JOYSTICK_get_direction();
void SLIDER_init();
SLIDER_position SLIDER_get_position();

#endif