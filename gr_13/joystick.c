/*
 * joystick.c
 *
 * Created: 15.09.2014 14:31:28
 *  Author: chriram
 */ 

#define F_CPU 4915200

#include "joystick.h"
#include "adc.h"
#include "uart.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

volatile uint8_t slider_btn_left;
volatile uint8_t slider_btn_right;

void JOYSTICK_init() {
	PORTB |= (1 << PB0); //Enable pullup
	GICR |= (1 << INTF2);
	sei();
	
	JOYSTICK_calibrate();
}

void JOYSTICK_calibrate() {
	mid_x = ADC_read(JOY_X);
	mid_y = ADC_read(JOY_Y);
}

int8_t JOYSTICK_get_position(joystick_control axis) {
	
	uint8_t mid;
	uint8_t pos = ADC_read(axis);
	
	if (axis) {
		mid = mid_x;
	} else {
		mid = mid_y;
	}
	
	if (pos > mid) {
		return 100*(pos - mid)/(255 - mid);
	} 
	if (pos < mid) {
		return 100*(pos - mid)/mid;
	}
	
	return 0;
}

joystick_control JOYSTICK_get_direction() {
	
	int8_t x_percent, y_percent;
	
	x_percent = JOYSTICK_get_position(JOY_X);
	y_percent = JOYSTICK_get_position(JOY_Y);
	
	// Button
	if (!((1 << PB0) & PINB)) {
		GIFR |= (1 << INTF2);
		return BTN_DOWN;
	}
	if (x_percent < -50) {
		return LEFT;
	}
	if (x_percent > 50) {
		return RIGHT;
	}
	if (y_percent < -50) {
		return UP;
	}
	if (y_percent > 50) {
		return DOWN;
	}	

	return NEUTRAL;
}

void SLIDER_init() {
	// Enable interrupt request on rising edge for INTO & INT1
	// CAUTION: MCUCR is used to enable EXTERNAL SRAM as well
	MCUCR |= (1 << ISC00) | (1 << ISC01) | (1 << ISC10) | (1 << ISC11);
	GICR |= (1 << INT0) | (1 << INT1);
	sei();
}

slider_position SLIDER_get_position(void) {
	
	slider_position position;

	uint8_t r_pos; // l_pos
	uint16_t temp;
	
	temp = 0;
	temp += ADC_read(SLIDER_R);
	temp += ADC_read(SLIDER_R);
	temp += ADC_read(SLIDER_R);
	temp = temp / 3;
	r_pos = temp;
	
	// Convert to percentage
	// position.left_pos = 100*l_pos/255;
	position.right_pos = 100*r_pos/255;
	
	return position;
}

slider_btn_state SLIDER_get_btn_state(void) {
	if (slider_btn_right && slider_btn_left) {
		slider_btn_left = 0;
		slider_btn_right = 0;
		return SLIDER_BTN_BOTH;
	} else if (slider_btn_left) {
		slider_btn_left = 0;
		slider_btn_right = 0;
		return SLIDER_BTN_LEFT;
	} else if (slider_btn_right) {
		slider_btn_left = 0;
		slider_btn_right = 0;
		return SLIDER_BTN_RIGHT;
	} else {
		slider_btn_left = 0;
		slider_btn_right = 0;
		return SLIDER_BTN_NONE;
	}
}

// Execute when Right button is pressed
ISR (INT0_vect) {
	slider_btn_right = 1;
}

// Execute when Left button is pressed
ISR (INT1_vect) {
	slider_btn_left = 1;
}