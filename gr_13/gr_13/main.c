/*
 * gr_13.c
 *
 * Created: 09.09.2014 11:35:43
 *  Author: chriram
 */ 

#ifndef F_CPU
#define F_CPU 4915200
#endif

#include <util/delay.h>
#include <avr/io.h>
#include <stdio.h>

#include "sram.h"
#include "uart.h"
#include "oled.h"
#include "menu.h"
#include "adc.h"
#include "joystick.h"

int main(void)
{
	usart_setup();
	SRAM_init();
	OLED_init();
	MENU_draw();
	SRAM_test();
	JOYSTICK_init();
	
	for(;;) {
		MENU_root();
		//uint8_t result = ADC_read(JOY_X);
		//printf("value: %d \n",result);
		_delay_ms(100);
    }
}