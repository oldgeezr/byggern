/*
 * oled.h
 *
 * Created: 16.09.2014 14:26:20
 *  Author: chriram
 */ 

#ifndef OLED_H_
#define OLED_H_

#include <stdint.h>

void OLED_init();
void OLED_start();
void OLED_clear();
void OLED_fill();
void OLED_set_pages(uint8_t y0, uint8_t y1);
void OLED_set_columns(uint8_t x0, uint8_t x1);
static inline void OLED_write_char(uint8_t character);
void OLED_write_string(char *str);
void OLED_scroll_right(uint8_t page_start,uint8_t page_stop);
void OLED_scroll_left(uint8_t page_start,uint8_t page_stop);

#endif