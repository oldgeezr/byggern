/*
 * adc.c
 *
 * Created: 09.09.2014 12:40:31
 *  Author: chriram
 */ 

#include "adc.h"

void ADC_init()
{
	// Set interrupt pin 
	GICR |= (1 << INT2); 
	sei();
}

uint8_t ADC_read()
{
	volatile char *adc = (char *) 0x1400;
	
	// Wait for ADC ready interrupt
	while (!((1 << INTF2) & GIFR)){}
		
	return *adc;
}