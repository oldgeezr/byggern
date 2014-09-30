/*
 * mcp2515.c
 *
 * Created: 30.09.2014 13:03:29
 *  Author: erlenhes
 */ 

#include "mcp2515.h"
#include "spi.h"

#include <avr/io.h>

uint8_t mcp2515_read(uint8_t cmd, uint8_t addr)
{
	uint8_t result;
	PORTB &= ~(1 << PB4); //Select CAN-controller
	SPI_master_transmit(cmd); // Send read command
	SPI_master_transmit(addr); // Send address 
	result = SPI_slave_receive(); //Read result
	PORTB |= (1 << PB4); //Deselect CAN-controller
	return result;
}

void mcp2515_write(char data) {
	PORTB &= ~(1 << PB4); //Select CAN-controller
	SPI_master_transmit(cmd); // Send read command
	SPI_master_transmit(addr); // Send address
	SPI_master_transmit(addr); // Send address
	PORTB |= (1 << PB4); //Deselect CAN-controller
}

void mcp2515_reset(void) {
	mcp2515_write(MCP_RESET);
}


uint8_t mcp2515_init(void)
{
	uint8_t value;
	SPI_master_init(); //Initialize SPI
	mcp2515_reset(); // Send reset-command
	//Self-test
	mcp2515_read(MCP_CANSTAT, &value);
	if((value & MODE_MASK) != MODE_CONFIG) {
		printf("MCP2515 is NOT in configuration mode after reset!\n");
		return 1;
	}
	// More initialization
	return 0;
}
