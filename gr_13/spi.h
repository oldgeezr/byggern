/*
 * spi.h
 *
 * Created: 30.09.2014 13:03:08
 *  Author: erlenhes
 */ 

#ifndef _SPI_H
#define _SPI_H

void SPI_init(void);
void SPI_write(char data);
void SPI_slave_init(void);
char SPI_read(void);
void SPI_select(void);
void SPI_deselect(void);

#endif