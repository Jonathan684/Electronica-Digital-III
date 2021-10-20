/*
 * libreria.c
 *
 *  Created on: 19 oct. 2021
 *      Author: Administrador
 */
//#include "lpc17xx.h"
//#include "lcd.h"
//void Lcd_CmdWrite(unsigned char cmd);
//void Lcd_DataWrite(unsigned char dat);
//#define LCDRS	 9
//#define LCDRW	10
//#define LCDEN	11
//
//#define LCD_D4  19
//#define LCD_D5  20
//#define LCD_D6  21
//#define LCD_D7  22
//#define LcdData       LPC_GPIO0->FIOPIN
//#define LcdControl    LPC_GPIO0->FIOPIN
//#define LcdDataDirn   LPC_GPIO0->FIODIR
//#define LcdCtrlDirn   LPC_GPIO0->FIODIR
//#define LCD_ctrlMask   ((1<<LCDRS)|(1<<LCDRW)|(1<<LCDEN))
//#define LCD_dataMask   ((1<<LCD_D4)|(1<<LCD_D5)|(1<<LCD_D6)|(1<<LCD_D7))
//unsigned int delay_gen ;
//
///*
// *sendNibble = enviar nibles
// */
//
// void sendNibble(char nibble)
//{
//    LcdData&=~(LCD_dataMask);                      // Clear previous data (Borrado de los datos anteriores)
//    LcdData|= (((nibble >>0x00) & 0x01) << LCD_D4);
//    LcdData|= (((nibble >>0x01) & 0x01) << LCD_D5);
//    LcdData|= (((nibble >>0x02) & 0x01) << LCD_D6);
//    LcdData|= (((nibble >>0x03) & 0x01) << LCD_D7);
//}
//
// void Lcd_CmdWrite(unsigned char cmd)
//{
//    sendNibble((cmd >> 0x04) & 0x0F);  //Send higher nibble
//    LcdControl &= ~(1<<LCDRS); // Send LOW pulse on RS pin for selecting Command register
//    LcdControl &= ~(1<<LCDRW); // Send LOW pulse on RW pin for Write operation
//    LcdControl |= (1<<LCDEN);  // Generate a High-to-low pulse on EN pin
//    for(delay_gen=0;delay_gen<=30000;delay_gen++);
//    LcdControl &= ~(1<<LCDEN);
//
//    for(delay_gen=0;delay_gen<=30000;delay_gen++);
//
//    sendNibble(cmd & 0x0F);            //Send Lower nibble
//    LcdControl &= ~(1<<LCDRS); // Send LOW pulse on RS pin for selecting Command register
//    LcdControl &= ~(1<<LCDRW); // Send LOW pulse on RW pin for Write operation
//    LcdControl |= (1<<LCDEN);  // Generate a High-to-low pulse on EN pin
//    for(delay_gen=0;delay_gen<=30000;delay_gen++);
//    LcdControl &= ~(1<<LCDEN);
//
//    for(delay_gen=0;delay_gen<=30000;delay_gen++);
//}
//
//
//void Lcd_DataWrite(unsigned char dat)
//{
//    sendNibble((dat >> 0x04) & 0x0F);  //Send higher nibble
//    LcdControl |= (1<<LCDRS);  // Send HIGH pulse on RS pin for selecting data register
//    LcdControl &= ~(1<<LCDRW); // Send LOW pulse on RW pin for Write operation
//    LcdControl |= (1<<LCDEN);  // Generate a High-to-low pulse on EN pin
//    for(delay_gen=0;delay_gen<=30000;delay_gen++);
//    LcdControl &= ~(1<<LCDEN);
//
//    for(delay_gen=0;delay_gen<=30000;delay_gen++);
//
//    sendNibble(dat & 0x0F);            //Send Lower nibble
//    LcdControl |= (1<<LCDRS);  // Send HIGH pulse on RS pin for selecting data register
//    LcdControl &= ~(1<<LCDRW); // Send LOW pulse on RW pin for Write operation
//    LcdControl |= (1<<LCDEN);  // Generate a High-to-low pulse on EN pin
//    for(delay_gen=0;delay_gen<=30000;delay_gen++);
//    LcdControl &= ~(1<<LCDEN);
//
//   for(delay_gen=0;delay_gen<=30000;delay_gen++);
//}
//void lcd_clear( void)
//{
//  Lcd_CmdWrite( 0x01 );
//}
//int lcd_gotoxy( unsigned char x, unsigned char y)
//{
//  unsigned char retval = TRUE;
//
//  if( (x > 1) && (y > 15) )
//   {
//     retval = FALSE;
//   }
//  else
//   {
//     if( x == 0 ) Lcd_CmdWrite( 0x80 + y );		//column
//  	 else if( x==1 ) Lcd_CmdWrite( 0xC0 + y );
//   }
//   return retval;
//}
//void lcd_putchar( unsigned char c )
//{
//  Lcd_DataWrite( c );
//}
//void lcd_putstring( char *string )
//{
//   while(*string != '\0')
//  {
//    lcd_putchar( *string );
//    string++;
//  }
//}
//void lcd_putstring16( unsigned char line, char *string )
//{
//  unsigned char len = 16;
//
//  lcd_gotoxy( line, 0 );//which row , column number
//  while(*string != '\0' && len--)
//  {
//    lcd_putchar( *string );
//    string++;
//  }
//}
//  void init_lcd( void )
//  {
//   LcdDataDirn |= LCD_dataMask;  // Configure all the LCD pins as output
//   LcdCtrlDirn |= LCD_ctrlMask;
//
//
//                     // Initialize Lcd in 4-bit mode
//  Lcd_CmdWrite(0x03);
//   for(delay_gen=0;delay_gen<=30000;delay_gen++);
//  Lcd_CmdWrite(0x03);
//   for(delay_gen=0;delay_gen<=30000;delay_gen++);
//  Lcd_CmdWrite(0x03);
//   for(delay_gen=0;delay_gen<=30000;delay_gen++);
//  Lcd_CmdWrite(0x2);
//  Lcd_CmdWrite(0x28);
//   Lcd_CmdWrite(0x0e);// display on cursor blinking
//   Lcd_CmdWrite(0x06);
//  Lcd_CmdWrite(0x01);
//   for(delay_gen=0;delay_gen<=30000;delay_gen++);                   // display on
//
//	}
//
//
