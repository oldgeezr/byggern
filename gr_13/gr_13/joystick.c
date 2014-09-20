/*
 * joystick.c
 *
 * Created: 15.09.2014 14:31:28
 *  Author: chriram
 */ 

#include "joystick.h"
#include "adc.h"
#include <avr/io.h>

void JOYSTICK_init() {
	PORTE |= (1 << PE0); //Enable pullup
	//GICR |= (1 << INTF2);
	//sei();
}

JOYSTICK_control JOYSTICK_get_direction() {
	uint8_t x_pos = ADC_read(JOY_X);
	uint8_t y_pos = ADC_read(JOY_Y);
	//printf("x: %d , y: %d \n",x_pos,y_pos);
	
	if (x_pos < 70) {
		return LEFT;
	}
	if (x_pos > 200) {
		return RIGHT;
	}
	if (y_pos < 90) {
		return UP;
	}
	if (y_pos > 200) {
		return DOWN;
	}
	/*if (!((1 << PE0) & PINE)) {
		//GIFR |= (1 << INTF2);
		return BTN_DOWN;
	}*/
	return NEUTRAL;
}

uint8_t JOYSTICK_get_position() {
	uint8_t x_pos = ADC_read(JOY_X);
	uint8_t y_pos = ADC_read(JOY_Y);
	
	return 0;
}

uint8_t SLIDER_get_position() {
	//Button left -> JTAG PF7
	//Button right -> JTAG PF6
	//Slider left -> JTAG PF5
	//Slider right -> JTAG PF4
	return 0;
}

