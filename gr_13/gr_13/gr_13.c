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

int main(void)
{
	usart_setup();
	SRAM_init();
	JOYSTICK_init();
	
	char result;
	
	//SRAM_test();
	for(;;) {
		//result = ADC_read(JOY_X);
		//printf("X: %d ", result);
		
		//result = ADC_read(JOY_Y);
		//printf(" , Y: %d \n", result);
		
		//result = JOYSTICK_get_direction();
		result = ADC_read(JOY_X);
		printf(" , POS: %d \n", result);
		
		_delay_ms(1);
		
    }
}
