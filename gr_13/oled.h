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
void OLED_write_char(uint8_t character);
void OLED_write_string(char *str);
void OLED_scroll_right(uint8_t page_start,uint8_t page_stop);
void OLED_scroll_left(uint8_t page_start,uint8_t page_stop);

#define SSD1308_ADDRESS_1 0x3C
#define SSD1308_ADDRESS_2 0x3D
#define SSD1308_DEFAULT_ADDRESS SSD1308_ADDRESS_1

#define ROWS 64
#define COLUMNS 128
#define PAGES 8
#define PAGE_WIDTH (ROWS / 8)
#define FONT_WIDTH 8
#define CHARS (COLUMNS / FONT_WIDTH)
#define MAX_PAGE (PAGES - 1)
#define MAX_COL (COLUMNS - 1)

#define HORIZONTAL_ADDRESSING_MODE 0x00
#define VERTICAL_ADDRESSING_MODE   0x01
#define PAGE_ADDRESSING_MODE       0x02

#define SET_MEMORY_ADDRESSING_MODE 0x20 // takes one byte

#define SET_COLUMN_ADDRESS 0x21 // takes two bytes, start address and end address of display data RAM
#define SET_PAGE_ADDRESS   0x22 // takes two bytes, start address and end address of display data RAM

#define SET_CONTRAST 0x81 // takes one byte, 0x00 - 0xFF

#define SET_SEGMENT_REMAP_0   0xA0 // column address 0 is mapped to SEG0
#define SET_SEGMENT_REMAP_127 0xA1 // column address 127 is mapped to SEG0

#define SET_ENTIRE_DISPLAY_ON 0xA5 // turns all pixels on, does not affect RAM
#define SET_DISPLAY_GDDRAM    0xA4 // restores display to contents of RAM

#define SET_NORMAL_DISPLAY  0xA6 // a data of 1 indicates 'ON'
#define SET_INVERSE_DISPLAY 0xA7 // a data of 0 indicates 'ON'

#define SET_MULTIPLEX_RATIO 0xA8 // takes one byte, from 16 to 63 (0x

#define EXTERNAL_IREF 0x10
#define INTERNAL_IREF 0x00
#define SET_IREF_SELECTION 0xAD // sets internal or external Iref

#define SET_DISPLAY_POWER_OFF 0xAE
#define SET_DISPLAY_POWER_ON  0xAF

#define COMMAND_MODE 0x80
#define DATA_MODE 0x40

#define PAGE0 0x00
#define PAGE1 0x01
#define PAGE2 0x02
#define PAGE3 0x03
#define PAGE4 0x04
#define PAGE5 0x05
#define PAGE6 0x06
#define PAGE7 0x07

#define SET_PAGE_START_ADDRESS 0xB0 // | with a page number to get start address

#define SET_DISPLAY_OFFSET 0xD3

#define SET_DISPLAY_CLOCK 0xD5

#define VCOMH_DESELECT_0_65_CODE 0x00
#define VCOMH_DESELECT_0_77_CODE 0x20
#define VCOMH_DESELECT_0_83_CODE 0x30
#define SET_VCOMH_DESELECT_LEVEL 0xDB

#define NOP 0xE3

#define SET_RIGHT_HORIZONTAL_SCROLL 0x26
#define SET_LEFT_HORIZONTAL_SCROLL  0x27
#define SET_VERTICAL_RIGHT_HORIZONTAL_SCROLL 0x29
#define SET_VERTICAL_LEFT_HORIZONTAL_SCROLL  0x2A

#define SET_DEACTIVATE_SCROLL 0x2E
#define SET_ACTIVATE_SCROLL 0x2F

#define SET_VERTICAL_SCROLL_AREA 0xA3

#endif