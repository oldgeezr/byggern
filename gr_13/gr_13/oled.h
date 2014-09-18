/*
 * oled.h
 *
 * Created: 16.09.2014 14:26:20
 *  Author: chriram
 */ 

#define F_CPU 16000000

#include <stdint.h>
#include <util/delay.h>
#include <avr/pgmspace.h>

void OLED_write_data(char dat);
void OLED_write_command(char cmd);
void OLED_init();
void OLED_test();
void OLED_start();
void OLED_clear();
void OLED_set_pixel(uint8_t x, uint8_t y);
void OLED_fill();
void OLED_write_char(uint8_t character);
void OLED_set_pages(uint8_t y0, uint8_t y1);
void OLED_set_columns(uint8_t x0, uint8_t x1);
void OLED_write_string(char *str);