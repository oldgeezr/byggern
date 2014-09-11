/*
 * uart.c
 *
 * Created: 09.09.2014 12:07:32
 *  Author: chriram
 */ 

#include "uart.h"

void usart_setup() 
{
	//Enable receive and transmit
	UCSR0B |= (1 << RXEN0) | (1 << TXEN0);
	UCSR0C |= (1 << UCSZ01) | (1 << UCSZ00); //8bit. no-parity, 1 stop bit
	
	UBRR0H = 0;
	UBRR0L = 31;
	
	uart = fdevopen(&usart_putchar, &usart_getchar);
}

int usart_putchar(char c, FILE *f) 
{
	while (!(UCSR0A & (1 << UDRE0)));
	UDR0 = c;
	return 0;
}

int usart_getchar(FILE *f) 
{
	while (!(UCSR0A & (1 << RXC0)));
	return UDR0;
}
