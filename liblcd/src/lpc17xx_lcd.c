/*
 * i2c_Flex_LCD.c
 *
 *  Created on: 18 oct. 2021
 *      Author: Administrador
 */
//-----------------------------------------------------------------------------
// Title:         i2c_Flex_LCD
// Description:   Driver for common LCD with 1/2/3 or 4 rows by 1...20 columns
//                using PCF8574T interface board with I2C protocol.
// Date:          Nov-2013
// Ver.Rev.:      1.1
// Author:        Hugo Silva (sergio-hugo@bol.com.br) #Based on the routines of
//                "20X4_LCD_I2C_DRIVER.h" from Pumrin S. and "lcd4_i2c.c" from XP8100
//-----------------------------------------------------------------------------
//
// lcd_init() Must be called before any other function.
//
// lcd_putc(c) Will display c on the next position of the LCD.
//
//     \f Clear LCD display
//     \n Set write position on next lcd line
//     \b LCD backspace
//     lcd_gotoxy(x,y) Set write position on LCD (upper left is 1,1)
//
// lcd_backlight_led(ON)/lcd_backlight_led(OFF) = Turn ON/OFF LCD Backlight LED
//
//-----------------------------------------------------------------------------
// LCD pins D0-D3 are not used.
//-----------------------------------------------------------------------------
//
// Comment   : Control of a compatible LCD (1...4 rows by 1...4 columns) from
//              a bus I2C with an EXPANDER of I/O with connection I2C.
//              The tests of these routines have been programmed using the IC
//              Phillips PCF8574T. I've used 4 bits mode programming.
//              The 8 bits mode programming is possible if you use 2 x PCF8574T.
//              RW Pin is not being used.
//
// As defined in the following structure the pin connection is as follows:
//
//  PCF8574P     LCD
//  ========     ======
//     P0        RS
//     P1        RW    (Not used!)
//     P2        Enable
//     P3        Led Backlight
//     P4        D4
//     P5        D5
//     P6        D6
//     P7        D7
//
//  The SCL and SDA pins should be pull-up resistor as shown below:
//
//             +5v
//               |
//               <
//               > 4.7K
//               <
//To PIC         |          To i2c slave
//pin xx ------------------ SDA pin
//(SDA)
//              +5v
//               |
//               <
//               > 4.7K
//               <
//To PIC         |          To i2c slave
//pin xx ------------------ SCL pin
//(SCL)
//
//To PIC                    To i2c slave
//Vss pin ----------------- Vss or ground pin
//                |
//              -----
//               ---  Ground
//                -
//
// THIS DOCUMENT IS PROVIDED TO THE USER "AS IS"
//-----------------------------------------------------------------------------
#include "LPC17xx.h"
#include "lpc17xx_i2c.h"
#include "lpc17xx_lcd.h"

#include "lpc17xx_pinsel.h"
#include "lpc17xx_gpio.h"
#include "lpc17xx_clkpwr.h"
#include "lpc17xx_exti.h"

#define      LCD_BACKLIGHT         0b00001000
#define      RS                    0b00000001  //P0 - PCF8574T Pin connected to RS
#define 	 ENABLE                0b00000100
#define 	 RW                    0b00000010  //P1 - PCF8574T Pin connected to RW
#define 	 addr_row_one          0x00        //LCD RAM address for row 1
#define 	 addr_row_two          0x40        //LCD RAM address for row 2
#define 	 addr_row_three        0x14        //LCD RAM address for row 3
#define 	 addr_row_four         0x54        //LCD RAM address for row 4


#define NOT                   ~
#define data_shifted          data<<4
uint32_t BACKLIGHT_LED = LCD_BACKLIGHT;
uint32_t new_row_request = 1;
int32_t LCD_ADDR ;//             0x4E        //I2C slave address for LCD module
int32_t lcd_total_rows ;//       2           //Number of rows: 1,2,3 or 4
int32_t lcd_total_columns ;//    16          //Number of columns: 1...20


char alarma[]  = {' ',' ',' ','A','L','A','R','M','A'};
char activada[]  = {'\n',' ',' ',' ','A','C','T','I','V','A','D','A'};
char desactivada[] = {'\n',' ',' ',' ','D','E','S','A','C','T','I','V','A','D','A'};
char disparada[] = {'\n',' ',' ',' ','D','I','S','P','A','R','A','D','A'};

unsigned	char st;

void	conf_I2C_2(void) {
	LPC_GPIO0->FIODIR |= (1<<18); //Configuracion como salida
	LPC_GPIO0->FIOSET |= (1<<18); //Alimentacion para el modulo conversor de nieveles logicos

    LPC_SC->PCONP |= 1 << 26;	//re-enable POWER to I2C_2 if required
	LPC_SC->PCLKSEL1 |= 1<<20;	//pclk = cclk
	LPC_PINCON->PINSEL0 |= 0x02<<20;	//Pin P0.10 allocated to alternate function 2
	LPC_PINCON->PINSEL0 |= 0x02<<22;	//Pin P0.11 allocated to alternate function 2
	LPC_GPIO0->FIODIR |= 1<<10;	       //Bit P0.10 an output
	LPC_GPIO0->FIODIR |= 1<<11;	       //Bit P0.11 an output
	LPC_PINCON->PINMODE0 &= ~(3<<20);	//P0.10 has pull up/down resistor
	LPC_PINCON->PINMODE0 |= (2<<20);	//omit to use internal pull up
	LPC_PINCON->PINMODE0 &= ~(3<<22);	//P0.11 has pull up/down resistor
	LPC_PINCON->PINMODE0 |= (2<<22);	//omit to use internal pull up
	LPC_PINCON->PINMODE_OD0 |= 1<<10;	//Bit P0.10 is open drain
	LPC_PINCON->PINMODE_OD0 |= 1<<11;	//Bit P0.11 is open drain
	LPC_I2C2->I2SCLH = 60;	//100kHz from 12MHz
	LPC_I2C2->I2SCLL = 60;	//100kHz from 12MHz
}



/*
 * Inicia el i2c
 */
void I2C2_enable (void) {
 	LPC_I2C2->I2CONSET |= 1<<6;	//enable I2C2
}

void I2C2_Start(void) {

		LPC_I2C2->I2CONSET |= 1<< 5;
		while (!(LPC_I2C2->I2CONSET & (1<<3)));
}
void I2C2_Stop(void) {
	LPC_I2C2->I2CONSET |= 1<<4;	//STOP I2C
	LPC_I2C2->I2CONCLR = 1<< 3;
	while (LPC_I2C2->I2CONSET & (1<<4));	//wait until H/w stops I2c
}
void lcd_clear()
{
        lcd_send_byte(0x01,0);
        delay_ms(8);
        new_row_request=1;
}

unsigned char	I2C2_Address(unsigned char add) {

 	LPC_I2C2->I2DAT = add;	//the address
 	LPC_I2C2->I2CONCLR = 1<<5;	//clear start
 	LPC_I2C2->I2CONCLR = 1<< 3;	//clear SI
 	while (!(LPC_I2C2->I2CONSET & (1<<3)));//esperar hasta que cambie de estado
    return (LPC_I2C2->I2STAT);
 }
void	I2C2_Write(char dat) {
 	LPC_I2C2->I2DAT = dat ;	//new data
 	LPC_I2C2->I2CONCLR = 1<< 3;
    while (!(LPC_I2C2->I2CONSET & (1<<3)));//esperar hasta que cambie de estado
}


void i2c_send_nibble(uint32_t data, uint32_t type)
{
    switch (type)
    {
        case 0 :
        I2C2_Write(data_shifted | BACKLIGHT_LED);
        delay_cycles(1);
        I2C2_Write(data_shifted | ENABLE |LCD_BACKLIGHT);
        delay_us(2);
        I2C2_Write((data_shifted & (NOT ENABLE)) | LCD_BACKLIGHT);
        break;

      case 1 :
    	  //0b00000001
    	  //I2C2_Write(0b11111100);
    	  I2C2_Write(data_shifted | RS | BACKLIGHT_LED);
    	  delay_cycles(1);
    	  I2C2_Write(data_shifted | RS | ENABLE | BACKLIGHT_LED );
    	  delay_us(2);
          I2C2_Write(data_shifted | RS | BACKLIGHT_LED);
          break;
      }
}
void lcd_send_byte(uint32_t data, uint32_t type)
{
	I2C2_Start();
	//I2C2_Write(0x4E);
	st = I2C2_Address(0x4E);
    i2c_send_nibble(data >> 4 , type);
	i2c_send_nibble(data & 0xf , type);
	I2C2_Stop();
}
void delay_cycles(int c){
	for(int i = 0; i<c;i++){};
}
void delay_us(int us)
{
	for(int i = 0; i<us*500;i++){};
}
void delay_ms(int t){
	t = t*70009;
	for(int i = 0; i<t;i++){};
}
void disable_interrupts(){
    LPC_I2C2 -> I2CONSET &=~(1<<3);
}
void lcd_init(uint32_t ADDR, uint32_t col, uint32_t row)
{
	uint32_t i=0;
	const uint32_t  lcd_type=2;  // 0=5x7, 1=5x10, 2=2 lines
	const uint32_t  LCD_INIT_STRING[4] = {0x20 | (lcd_type << 2), 0xc, 1, 6}; // These bytes need to be sent to the LCD to start it up.
    LCD_ADDR =ADDR;//             0x4E        //I2C slave address for LCD module
	lcd_total_rows =row;//       2           //Number of rows: 1,2,3 or 4
	lcd_total_columns= col ;
	disable_interrupts(); // Desahibilitar las interrupciones globales
	delay_ms(60); //LCD power up delay

	I2C2_Start();

	st = I2C2_Address(0x4E);
	i2c_send_nibble(0x00,0);
	delay_ms(25);
	I2C2_Write(0b00110000);
	for (i=1;i<=3;++i)
	{
		i2c_send_nibble(0x03,0);
		delay_ms(10);
	}
	i2c_send_nibble(0x02,0); //datasheet del lcd
	delay_ms(10);
	I2C2_Stop();

	for (i=0;i<=3;++i) {
		lcd_send_byte(LCD_INIT_STRING[i],0);
		delay_ms(10);
	}
	lcd_clear();  //Clear Display
	//enable_interrupts(GLOBAL);
}

void lcd_backlight_led(uint32_t bl)
{
	  if (bl) BACKLIGHT_LED=LCD_BACKLIGHT;
      else BACKLIGHT_LED=OFF;
}
void lcd_gotoxy( uint32_t x, uint32_t y)
{
 uint32_t row,column,row_addr,lcd_address;
 //static char data;

   if (y>lcd_total_rows) row=lcd_total_rows; else row=y;

   switch(row)
   {
      case 1:  row_addr=addr_row_one;     break;
      case 2:  row_addr=addr_row_two;     break;
      case 3:  row_addr=addr_row_three;   break;
      case 4:  row_addr=addr_row_four;    break;
      default: row_addr=addr_row_one;     break;
   }

   if (x>lcd_total_columns) column=lcd_total_columns; else column=x;
   lcd_address=(row_addr+(column-1));
   lcd_send_byte(0x80|lcd_address,0);
}
void LCD_PUTC(char in_data)
{
  switch(in_data)
   {
     case '\f': lcd_clear();                       break;

     case '\n':
     new_row_request++;
     if (new_row_request>lcd_total_rows) new_row_request=1;
     lcd_gotoxy(1, new_row_request);
     break;

     case '\b': lcd_send_byte(0x10,0);             break;

     default: lcd_send_byte(in_data,1);            break;

   }
}
void Activar_alarma(){
	lcd_clear();
	for(int i=0;i<9;i++){
		LCD_PUTC(alarma[i]);
	}
	for(int i=0;i<12;i++){
		LCD_PUTC(activada[i]);
	}
}

void Desactivar_alarma(){
	lcd_clear();
	for(int i=0;i<9;i++){
		LCD_PUTC(alarma[i]);
	}
	for(int i=0;i<15;i++){
		LCD_PUTC(desactivada[i]);
	}
}
void Disparar_alarma(){
	lcd_clear();
	for(int i=0;i<9;i++){
		LCD_PUTC(alarma[i]);
	}
	for(int i=0;i<13;i++){
		LCD_PUTC(disparada[i]);
	}
}

