/*
 * spi.h
 *
 * Created: 30.09.2014 13:03:08
 *  Author: erlenhes
 */ 

#ifndef _SPI_H
#define _SPI_H

void SPI_master_init(void);
void SPI_master_transmit(char data);
void SPI_slave_init(void);
char SPI_slave_receive(void);

#endif