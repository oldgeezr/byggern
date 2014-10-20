/*
 * adc.c
 *
 * Created: 09.09.2014 12:40:31
 *  Author: chriram
 */ 

#include "adc.h"

#ifndef F_CPU
#define F_CPU 4915200
#endif 

#include <util/delay.h>

uint8_t ADC_read(adc_channel ch) {
	volatile char *adc = (char *) 0x1400;
	adc[1] = ch;
	_delay_us(40);
	
	return adc[0];
}