/*
 * sram.h
 *
 * Created: 09.09.2014 12:03:44
 *  Author: chriram
 */ 

#ifndef SRAM_H_
#define SRAM_H_

#include <stdio.h>
#include <stdint.h>

static volatile char *ext_ram = (char *) 0x1800;

void SRAM_init(void);
void SRAM_test(void);

static inline void SRAM_write(uint16_t address, uint8_t data)  {
	ext_ram[address] = data;
}

static inline uint8_t SRAM_read(uint16_t address) {
	return ext_ram[address];
}

#endif