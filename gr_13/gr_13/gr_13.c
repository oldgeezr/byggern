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

int main(void)
{
	//DDRC = (1 << PC0);
	//PORTC = (1 << PC0);
	usart_setup();
	SRAM_init();
	ADC_init();
	
	//int y = 0;
	//char str[10];
	
	for(;;) {
		//PORTC ^= (1 << PC0);
		// SRAM_test();
		printf("%d\n", ADC_read());
		// scanf("%s",str);
		// printf("HEI\n"); // "%s\n", str);
		// _delay_ms(250);
    }
}
