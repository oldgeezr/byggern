/*
 * spi.c
 *
 * Created: 30.09.2014 13:03:01
 *  Author: erlenhes
 */ 

#include "spi.h"
#include <avr/io.h>

void SPI_init(void) {
	//Set MOSI and SCK as output, all others input
	DDRB |= (1 << PB5) | (1 << PB7) | (1 << PB4);
	DDRB &= ~(1 << PB6);
	//Enable SPI, Master
	SPCR |= (1 << SPE) | (1 << MSTR) | (1 << SPR0);
	//SPSR |= (1 << SPI2X);
}
void SPI_write(char data) {
	SPDR = data;
	while(!(SPSR & (1 << SPIF)));
}

void SPI_slave_init(void) {
	//Set MISO output, all others input
	DDRB = (1 << PB6);
	//Enable SPI
	SPCR = (1 << SPE);
}
char SPI_read(void) {
	SPI_write(0); //Send dummy byte to receive
	while(!(SPSR & (1 << SPIF)));
	return SPDR;
}

void SPI_select(void) {
	PORTB &= ~(1 << PB4);
}

void SPI_deselect(void) {
	PORTB |= (1 << PB4);
}