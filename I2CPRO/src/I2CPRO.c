/*
===============================================================================
 Name        : I2CPRO.c
 Author      : $(author)
 Version     :
 Copyright   : $(copyright)
 Description : main definition
===============================================================================
*/

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#include "lpc17xx_i2c.h"
#endif

#include <cr_section_macros.h>

#include <stdio.h>

// TODO: insert other include files here

// TODO: insert other definitions and declarations here
//#define      WAIT_SI         while (!(LPC_I2C2->I2CONSET & (1<<3)))
//#define      CLEAR_SI         LPC_I2C2->I2CONCLR = 1<< 3
#define      LCD_BACKLIGHT         0b00001000
#define      RS                    0b00000001  //P0 - PCF8574T Pin connected to RS
#define 	 ENABLE                0b00000100
#define 	 RW                    0b00000010  //P1 - PCF8574T Pin connected to RW
#define 	 addr_row_one          0x00        //LCD RAM address for row 1
#define 	 addr_row_two          0x40        //LCD RAM address for row 2
#define 	 addr_row_three        0x14        //LCD RAM address for row 3
#define 	 addr_row_four         0x54        //LCD RAM address for row 4

uint32_t new_row_request = 1;
int32_t LCD_ADDR ;//             0x4E        //I2C slave address for LCD module
int32_t lcd_total_rows ;//       2           //Number of rows: 1,2,3 or 4
int32_t lcd_total_columns ;//    16          //Number of columns: 1...20

void	conf();
void I2C2_enable(void);
void I2C2_Start(void);
void I2C2_Stop(void);


#define NOT                   ~
#define data_shifted          data<<4
uint32_t BACKLIGHT_LED = LCD_BACKLIGHT;
#define ON                    1
#define OFF                   0


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

char alarma_tex[] = {'A','L','A','R','M','A',' ','A','C','T','I','V','A','D','A'};

unsigned	char st;

int main(void) {

	printf("Control LCD\n");

	I2C2_Stop();
	conf();
	I2C2_enable();	//Refer to I2CONSET register
	I2C2_Start();
	lcd_init(0x4E, 16, 2);
	lcd_backlight_led(ON);
    //I2C2_Start();	//Refer to I2CONSET register
		            // Valor que necesito para escritura 0x27 + 1
    while(1) {
    	lcd_clear();
    	for(int i = 0; i<12000000;i++){};
    	//lcd_gotoxy(3, 1);
    	for(int i=0;i<15;i++){
    	LCD_PUTC(alarma_tex[i]);}

    	for(int i = 0; i<12000000;i++){};
    	//lcd_backlight_led(OFF);
    }
    return 0 ;
}
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
void	conf(void) {
	LPC_GPIO0->FIODIR |= (1<<0);
	LPC_GPIO0->FIOSET |= (1<<0); //Alimentacion para el modulo conversor de nieveles logicos

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
	for(int i = 0; i<t*70009;i++){};
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
