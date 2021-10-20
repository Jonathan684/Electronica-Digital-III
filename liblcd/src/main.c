/*
===============================================================================
 Name        : liblcd.c
 Author      : $(author)
 Version     :
 Copyright   : $(copyright)
 Description : main definition
===============================================================================
*/

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#include "lpc17xx_i2c.h"
#include "i2c_Flex_LCD.h"
#endif

#include <cr_section_macros.h>

#include <stdio.h>

// TODO: insert other include files here
void I2C2_enable (void);
// TODO: insert other definitions and declarations here
void conf(void);
int main(void) {

    printf("Hello World\n");
    conf();
    I2C2_enable();
    lcd_init(0x4E,16,2);
    lcd_backlight_led(1);
    while(1) {
    	LCD_PUTC("H");
    	for(int i=0;i<1000000;i++){}
    }
    return 0 ;
}

void I2C2_enable (void) {
 	//LPC_I2C2->I2CONSET |= 1<<6;	//enable I2C2 // bien
 	LPC_I2C2 -> I2CONCLR |= (1<<3)|(1<<4)|(1<<5);
 	LPC_I2C2->I2CONSET |= 1<<6;
}
void conf(void) {
	LPC_GPIO0->FIODIR0 |= 0x33;	//P0.5 P0.4 P0.1 P0.0 ext pins 29,30,10,9
	LPC_SC->PCONP |= 1 << 26;	//re-enable POWER to I2C_2 if required
	LPC_SC->PCLKSEL1 |= 1<<20;	//pclk = cclk
	LPC_PINCON->PINSEL0 |= 0x02<<20;	//Pin P0.10 allocated to alternate function 2
	LPC_PINCON->PINSEL0 |= 0x02<<22;	//Pin P0.11 allocated to alternate function 2
	LPC_GPIO0->FIODIR |= 1<<10;	//Bit P0.10 an output
	LPC_GPIO0->FIODIR |= 1<<11;	//Bit P0.11 an output
	LPC_PINCON->PINMODE0 &= ~(3<<20);	//P0.10 has pull up/down resistor
	LPC_PINCON->PINMODE0 |= (2<<20);	//omit to use internal pull up
	LPC_PINCON->PINMODE0 &= ~(3<<22);	//P0.11 has pull up/down resistor
	LPC_PINCON->PINMODE0 |= (2<<22);	//omit to use internal pull up
	LPC_PINCON->PINMODE_OD0 |= 1<<10;	//Bit P0.10 is open drain
	LPC_PINCON->PINMODE_OD0 |= 1<<11;	//Bit P0.11 is open drain
	LPC_I2C2->I2SCLH = 60;	//100kHz from 12MHz
	LPC_I2C2->I2SCLL = 60;	//100kHz from 12MHz
}
