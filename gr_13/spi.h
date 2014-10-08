/*
 * spi.h
 *
 * Created: 30.09.2014 13:03:08
 *  Author: erlenhes
 */ 

#ifndef _SPI_H_
#define _SPI_H_

#define SS   PB4
#define MOSI PB5
#define MISO PB6
#define SCK	 PB7

#include <avr/io.h>

void SPI_init(void);
void SPI_slave_init(void);
void SPI_write(char data);
char SPI_read(void);

static inline void SPI_select(void) {
	PORTB &= ~(1 << SS);
}
static inline void SPI_deselect(void) {
	PORTB |= (1 << SS);
}

#endif