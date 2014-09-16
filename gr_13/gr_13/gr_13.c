/*
 * gr_13.c
 *
 * Created: 09.09.2014 11:35:43
 *  Author: chriram
 */ 

#define F_CPU 4915200

#include <util/delay.h>
#include <avr/io.h>
#include <stdio.h>

#include "sram.h"
#include "uart.h"
#include "adc.h"
#include "joystick.h"
#include "oled.h"

int main(void)
{
	usart_setup();
	SRAM_init();
	JOYSTICK_init();
	OLED_init();
	
	// char result;
	
	SRAM_test();
	for(;;) {
		OLED_start();
		OLED_clear();
		uint8_t i;
		for (i = 0; i < 8; i++) {
			OLED_set_pixel(100, (50 + i));
			_delay_ms(500);
		}	
		// OLED_test();
		
		//result = ADC_read(JOY_X);
		//printf("X: %d ", result);
		
		//result = ADC_read(JOY_Y);
		//printf(" , Y: %d \n", result);
		
		//result = JOYSTICK_get_direction();
		//result = ADC_read(JOY_X);
		//printf(" , POS: %d \n", result);
		
		_delay_ms(10);
		
    }
}
