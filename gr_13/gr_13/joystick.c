/*
 * joystick.c
 *
 * Created: 15.09.2014 14:31:28
 *  Author: chriram
 */ 

#include "joystick.h"
#include "adc.h"
#include <avr/io.h>
#include <avr/interrupt.h>

void JOYSTICK_init() {
	PORTB |= (1 << PB0); //Enable pullup
	GICR |= (1 << INTF2);
	sei();
	
	JOYSTICK_calibrate();
}

void JOYSTICK_calibrate() {
	mid_x = ADC_read(JOY_X);
	mid_y = ADC_read(JOY_Y);
	
	// printf("midx: %d, midy: %d\n", mid_x, mid_y); // Typical 151, 151
}

int8_t JOYSTICK_get_position(JOYSTICK_control axis) {
	
	uint8_t mid;
	
	uint8_t pos = ADC_read(axis);
	
	// printf("pos: %d\n", pos);
	
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

JOYSTICK_control JOYSTICK_get_direction() {
	
	/*
	* NOTE! When getting the direction of the y-axis the program 
	* runs very slow! Maybe it's just the printf func?
	*/
	
	int8_t x_percent, y_percent;
	
	x_percent = JOYSTICK_get_position(JOY_X);
	y_percent = JOYSTICK_get_position(JOY_Y);
	
	// printf("xpercent: %d, ypercent: %d\n", x_percent, y_percent);
	
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

SLIDER_position SLIDER_get_position() {
	
	SLIDER_position position;

	uint8_t l_pos, r_pos;
	
	l_pos = ADC_read(SLIDER_L);
	r_pos = ADC_read(SLIDER_R);
	
	// Convert to percentage
	position.left_pos = 100*l_pos/255;
	position.right_pos = 100*r_pos/255;
	
	// printf("x: %d, y: %d\n", position.left_pos, position.right_pos);
	
	return position;
}

// Execute when Right button is pressed
ISR (INT0_vect) {
	printf("btnR\n");
}

// Execute when Left button is pressed
ISR (INT1_vect) {
	printf("btnL\n");
}