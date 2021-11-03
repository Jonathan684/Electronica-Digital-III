#ifndef LPC17XX_LCD_H_
#define LPC17XX_LCD_H_

#include "librerias.h"
#define ON                    1
#define OFF                   0

void Activar_alarma();
void Desactivar_alarma();
void Disparar_alarma();
void conf_I2C_2();
void I2C2_enable(void);
void I2C2_Start(void);
void I2C2_Stop(void);
unsigned char	I2C2_Address(unsigned char add);
void I2C2_Write(char dat);
void delay_ms(int );
void delay_us(int us);
void delay_cycles(int c);
void lcd_send_byte(uint32_t data, uint32_t type);
void lcd_init(uint32_t ADDR, uint32_t col, uint32_t row);
void lcd_backlight_led(uint32_t bl);
void lcd_clear();
void LCD_PUTC(char in_data);
void lcd_gotoxy( uint32_t x, uint32_t y);

#endif