/*
 * test.c
 *
 * Created: 26.08.2014 13:24:21
 *  Author: erlenhes
 */ 


#define F_CPU 4915200

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdint.h>

int usart_putchar(char c, FILE *f);
int usart_getchar(FILE *f);

FILE *uart;

void usart_setup() {
	//Enable receive and transmit
	UCSR0B |= (1 << RXEN0) | (1 << TXEN0);
	UCSR0C |= (1 << UCSZ01) | (1 << UCSZ00); //8bit. no-parity, 1 stop bit
	
	UBRR0H = 0;
	UBRR0L = 31;
	
	uart = fdevopen(&usart_putchar, &usart_getchar);
}

int usart_putchar(char c, FILE *f) {
	while (!(UCSR0A & (1 << UDRE0)));
	UDR0 = c;
	return 0;
}

int usart_getchar(FILE *f) {
	while (!(UCSR0A & (1 << RXC0)));
	return UDR0;
} 

int main(void)
{
	DDRC = (1 << PC0);
	PORTC = (1 << PC0);
	usart_setup();
	
	int y = 0;
	char str[10];
	
	while(1) {
		PORTC ^= (1 << PC0);
		scanf("%s",str);
		printf("%s\n", str);
		_delay_ms(250);
    }
}
