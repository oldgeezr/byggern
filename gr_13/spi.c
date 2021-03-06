/*
 * spi.c
 *
 * Created: 30.09.2014 13:03:01
 *  Author: erlenhes
 */ 

#include "spi.h"

void SPI_init(void) {
	//Set MOSI, SCK and SS as output, all others input
	DDRB |= (1 << MOSI_PIN) | (1 << SCK_PIN) | (1 << SS_PIN);
	DDRB &= ~(1 << MISO_PIN);
	SPCR |= (1 << SPE) | (1 << MSTR) | (1 << SPR0); //Enable SPI Master mode
}

void SPI_slave_init(void) {
	DDRB = (1 << MISO_PIN);
	SPCR = (1 << SPE);
}

void SPI_write(char data) {
	SPDR = data;
	while(!(SPSR & (1 << SPIF)));
}

char SPI_read(void) {
	SPI_write(0); //Send dummy byte to receive
	while(!(SPSR & (1 << SPIF)));
	return SPDR;
}