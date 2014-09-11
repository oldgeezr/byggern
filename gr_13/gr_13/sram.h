/*
 * sram.h
 *
 * Created: 09.09.2014 12:03:44
 *  Author: chriram
 */ 

#include <avr/io.h>
#include <stdio.h>
#include <stdint.h>

void SRAM_init();

void SRAM_test(void);

void SRAM_gal_cs_test();