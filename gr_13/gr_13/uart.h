/*
 * uart.h
 *
 * Created: 09.09.2014 12:08:48
 *  Author: chriram
 */ 

#include <avr/io.h>
#include <stdio.h>

int usart_putchar(char c, FILE *f);
int usart_getchar(FILE *f);

FILE *uart;

void usart_setup();

int usart_putchar(char c, FILE *f);

int usart_getchar(FILE *f);