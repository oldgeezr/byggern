/*
 * MCP2515.c
 *
 * Created: 30.09.2014 13:03:29
 *  Author: erlenhes
 */ 

#include "MCP2515.h"
#include "spi.h"
#include "uart.h"

#define F_CPU 4915200

#include <avr/io.h>
#include <util/delay.h>

uint8_t MCP2515_init(void)
{
	uint8_t value;
	SPI_init();
	MCP2515_reset();
	_delay_ms(1);
	
	//Self-test
	value = MCP2515_read(MCP_CANSTAT);
	if ((value & MODE_MASK) != MODE_CONFIG) {
		printf("MCP2515 is NOT in configuration mode after reset!");
		return 1;
		} else {
		printf("configured %d \n", value);
	}
	
	//Configure filters
	MCP2515_bit_modify(MCP_RXB0CTRL, 0b01100100, 0xFF);
	
	//Configure baudrate to 500kbps
	MCP2515_write(MCP_CNF1,0x00);
	MCP2515_write(MCP_CNF2,0xF0);
	MCP2515_write(MCP_CNF3,0x86);
	
	MCP2515_bit_modify(MCP_CANCTRL, MODE_MASK, MODE_NORMAL);
	//MCP2515_bit_modify(MCP_CANCTRL, MODE_MASK, MODE_LOOPBACK);
	
	MCP2515_bit_modify(MCP_CANINTE, 0x01, 1); //Enable receive interrupt
	
	return 0;
}

void MCP2515_reset(void) {
	SPI_select();
	SPI_write(MCP_RESET);
	SPI_deselect();
}

void MCP2515_write(uint8_t address, uint8_t data) {
	SPI_select();
	SPI_write(MCP_WRITE);
	SPI_write(address);
	SPI_write(data);
	SPI_deselect();
}

uint8_t MCP2515_read(uint8_t address)
{
	uint8_t result;
	SPI_select();
	SPI_write(MCP_READ);
	SPI_write(address);
	result = SPI_read();
	SPI_deselect();
	return result;
}

uint8_t MCP2515_read_rx_buffer(uint8_t instruction) {
	uint8_t result;
	SPI_select(); 
	SPI_write(instruction);
	result = SPI_read();
	SPI_deselect();
	return result;
}

uint8_t MCP2515_read_status(void) {
	uint8_t result;
	SPI_select(); 
	SPI_write(MCP_READ_STATUS);
	result = SPI_read(); 
	SPI_deselect(); 
	return result;
}

void MCP2515_load_tx_buffer(uint8_t data) {
	SPI_select();
	SPI_write(MCP_LOAD_TX0);
	SPI_write(data);
	SPI_deselect();
}

void MCP2515_rts(void) { //Request to send (RTS)
	SPI_select();
	SPI_write(MCP_RTS_TX0);
	SPI_deselect();
}

uint8_t MCP2515_rx_status(void) {
	uint8_t result;
	SPI_select();
	SPI_write(MCP_RX_STATUS);
	result = SPI_read();
	SPI_deselect();
	return result;
}

uint8_t MCP2515_read_RX0(void) {
	uint8_t result;
	SPI_select();
	SPI_write(MCP_READ_RX0);
	result = SPI_read();
	SPI_deselect();
	return result;
}

void MCP2515_bit_modify(uint8_t address, uint8_t mask_byte, uint8_t data) {
	SPI_select();
	SPI_write(MCP_BITMOD);
	SPI_write(address);
	SPI_write(mask_byte);
	SPI_write(data);
	SPI_deselect();
} 