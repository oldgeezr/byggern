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

void SRAM_init() 
{
	MCUCR |= (1<<SRE); 
}

void SRAM_test2()
{
	volatile char *ext_ram = (char *) 0x1800;
	unsigned char testvalue = 0;
	ext_ram[0] = 0;
	if (ext_ram[0] != testvalue) {
		printf("SRAM error (write phase): ext_ram[0] = %02X (should be %02X)\r\n", ext_ram[0], testvalue);
	}
	
}

void SRAM_test(void)
{
	// Start address for the SRAM
	volatile char *ext_ram = (char *) 0x1800;

	uint16_t i, werrors, rerrors;
	werrors = 0;
	rerrors = 0;
	unsigned char testvalue;

	printf("Starting SRAM test...\r\n");

	for (i = 0; i < 0x800; i++) {
		testvalue = ~(i % 256);
		ext_ram[i] = testvalue;
		if (ext_ram[i] != testvalue) {
			printf("SRAM error (write phase): ext_ram[%d] = %02X (should be %02X)\r\n", i, ext_ram[i], testvalue);
			werrors++;
		}
	}

	for (i = 0; i < 0x800; i++) {
		testvalue = ~(i % 256);
		if (ext_ram[i] != testvalue) {
			printf("SRAM error (read phase): ext_ram[%d] = %02X (should be %02X)\r\n", i, ext_ram[i], testvalue);
			rerrors++;
		}
	}

	printf("SRAM test completed with %d errors in write phase and %d errors in read phase\r\n", werrors, rerrors);
}

int main(void)
{
	//DDRC = (1 << PC0);
	//PORTC = (1 << PC0);
	usart_setup();
	SRAM_init();
	
	int y = 0;
	char str[10];
	
	for(int i = 0; ;i++) {
		//PORTC ^= (1 << PC0);
		SRAM_test();
		// scanf("%s",str);
		// printf("HEI\n"); // "%s\n", str);
		// _delay_ms(250);
    }
}
