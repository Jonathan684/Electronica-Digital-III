#ifndef LPC17XX_FLEX_H_
#define LPC17XX_FLEX_H_
#include "lpc17xx_i2c.h"
void lcd_backlight_led(uint32_t bl);
void i2c_send_nibble(uint32_t data, uint32_t type);
void lcd_send_byte(uint32_t data, uint32_t type);
void lcd_clear();
void lcd_init(uint32_t ADDR, uint32_t col, uint32_t row);
void lcd_gotoxy( uint32_t x, uint32_t y);
void LCD_PUTC(char in_data);
#endif
