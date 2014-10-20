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
	LEFT = 'L',
	RIGHT = 'R',
	UP = 'U',
	DOWN = 'D',
	NEUTRAL = 'N',
	BTN_DOWN = 'B'
} joystick_control;

typedef struct {
	uint8_t left_pos;
	uint8_t right_pos;
} slider_position;

void JOYSTICK_calibrate();
void JOYSTICK_init();
int8_t JOYSTICK_get_position(joystick_control axis);
joystick_control JOYSTICK_get_direction();
void SLIDER_init();
slider_position SLIDER_get_position();

#endif