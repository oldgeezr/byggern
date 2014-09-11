/*
 * adc.h
 *
 * Created: 09.09.2014 12:40:52
 *  Author: chriram
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>

void ADC_init();

uint8_t ADC_read();