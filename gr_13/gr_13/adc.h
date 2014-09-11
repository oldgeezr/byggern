/*
 * adc.h
 *
 * Created: 09.09.2014 12:40:52
 *  Author: chriram
 */ 

#define F_CPU 4915200

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

typedef enum {
	JOY_X = 4,
	JOY_Y = 5,
	SLIDER_L = 6,
	SLIDER_R = 7
} ADC_channel;

uint8_t ADC_read(ADC_channel ch);