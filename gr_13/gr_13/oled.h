/*
 * oled.h
 *
 * Created: 16.09.2014 14:26:20
 *  Author: chriram
 */ 
#include <stdint.h>
#include <util/delay.h>

void OLED_write_data(char dat);
void OLED_write_command(char cmd);
void OLED_init();
void OLED_test();
void OLED_start();
void OLED_clear();
void OLED_set_pixel(uint8_t x, uint8_t y);