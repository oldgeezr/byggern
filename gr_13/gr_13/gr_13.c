/*
 * gr_13.c
 *
 * Created: 09.09.2014 11:35:43
 *  Author: chriram
 */ 

#define F_CPU 16000000

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
		OLED_set_pages(0,0);
		OLED_set_columns(0,127);
		OLED_write_string("christoffer");
		_delay_ms(1000);
    }
}
