/*
 * adc.h
 *
 * Created: 09.09.2014 12:40:52
 *  Author: chriram
 */ 

#ifndef ADC_H_
#define ADC_H_

#include <stdint.h>

typedef enum {
	JOY_X = 4,
	JOY_Y = 5,
	SLIDER_L = 6,
	SLIDER_R = 7
} ADC_channel;

uint8_t ADC_read(ADC_channel ch);

#endif