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

typedef enum {
	SLIDER_BTN_NONE,
	SLIDER_BTN_LEFT,
	SLIDER_BTN_RIGHT,
	SLIDER_BTN_BOTH
} slider_btn_state;

void JOYSTICK_calibrate(void);
void JOYSTICK_init(void);
int8_t JOYSTICK_get_position(joystick_control axis);
joystick_control JOYSTICK_get_direction(void);
void SLIDER_init(void);
slider_position SLIDER_get_position(void);
slider_btn_state SLIDER_get_btn_state(void);

#endif