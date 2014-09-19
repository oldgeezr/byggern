/*
 * uart.h
 *
 * Created: 09.09.2014 12:08:48
 *  Author: chriram
 */ 

#ifndef UART_H_
#define UART_H_

#include <stdio.h>

FILE *uart;

int usart_putchar(char c, FILE *f);
int usart_getchar(FILE *f);

void usart_setup();

#endif