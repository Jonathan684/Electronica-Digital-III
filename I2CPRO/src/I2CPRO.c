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
//#define      WAIT_SI      while (!(LPC_I2C2->I2CONSET & (1<<3)))
#define      CLEAR_SI       LPC_I2C2->I2CONCLR = 1<< 3
#define      ACTION         LPC_GPIO0->FIOSET0 = 1
#define      ACTION_E       LPC_GPIO0->FIOCLR0 = 1
#define      ADDRESS        LPC_GPIO0->FIOSET0 = 2
#define      ADDRESS_E      LPC_GPIO0->FIOCLR0 = 2
#define      WRITE	        LPC_GPIO0->FIOSET0 = 1<<4
#define      WRITE_E       	LPC_GPIO0->FIOCLR0 = 1<<4
#define      READ	LPC_GPIO0->FIOSET0 = 1<<5
#define      READ_E	LPC_GPIO0->FIOCLR0 = 1<<5
void	conf();
void I2C2_enable(void);
void I2C2_Start(void);
void I2C2_Stop(void);

#define ENABLE                0b00000100

uint32_t  BACKLIGHT_LED = 0b00001000;
unsigned char	I2C2_Address(unsigned char add);
void I2C2_Write(char dat);
int * a = 4;
uint32_t aux ;
void delay()
{
	for(int i=0;i<30000;i++){}
}
unsigned char	enviar(unsigned char add) {
 	//ADDRESS;	//signal for MSO
 	LPC_I2C2->I2DAT = add;	//the address
 	//LPC_I2C2->I2CONCLR = 1<<5;	//clear start
 	//LPC_I2C2->I2CONCLR = 1<< 3;	//clear SI
 	while (!(LPC_I2C2->I2CONSET & (1<<3)));//esperar hasta que cambie de estado

 	LPC_I2C2 -> I2CONSET = 0X04;
 	return (LPC_I2C2->I2STAT);
 	}
int main(void) {
	conf();
	printf("Hello World\n");
	const uint32_t  lcd_type=2;
	const uint32_t LCD_INIT_STRING[4] = {0x20 | (lcd_type << 2), 0xC, 1, 6};
	unsigned	char st;

	//LPC_I2C2->I2ADR0 = 0x4E;

			LPC_I2C2->I2CONSET |= 1<< 5;
			LPC_I2C2->I2CONSET |= 1<< 6;

	//I2C2_enable();	//Refer to I2CONSET register
	//I2C2_Start();	//Refer to I2CONSET register
		// Valor que necesito para escritura 0x27 + 1
    while(1) {
		st = I2C2_Address(0x4E);//0x4E
        if ((st == 0x18)  )	//Consulte el registro I2STATUS
    	{ //OTRAS ACCIONES
    		//I2C2_Start();
        	//for(int i=0;i<1000000;i++){}//0.00000001

        	st = enviar(0x0); //ESCRIBO EN EL ESCLAVO
        	delay();
        	LPC_I2C2->I2CONCLR |= (1<<3);
        	LPC_I2C2->I2CONSET |= 1<< 5;
        	LPC_I2C2->I2CONSET |= 1<< 6;

        	st = enviar(0b00110000); //ESCRIBO EN EL ESCLAVO
        	delay();
        	LPC_I2C2->I2CONCLR |= (1<<3);
        	LPC_I2C2->I2CONSET |= 1<< 5;
        	LPC_I2C2->I2CONSET |= 1<< 6;
        	st = enviar(0b00110000); //ESCRIBO EN EL ESCLAVO
        	delay();

        	LPC_I2C2->I2CONCLR |= (1<<3);
        	LPC_I2C2->I2CONSET |= 1<< 5;
        	LPC_I2C2->I2CONSET |= 1<< 6;

        	st = enviar(0x03);
        	delay();
        	st = enviar(0x03);
        	delay();
        	st = enviar(0x03);
        	delay();
        	//LPC_I2C2->I2DAT = 0xffffff;
        	//LPC_I2C2->I2DAT = 0b00110000;
//        	while (!(LPC_I2C2->I2CONSET & (1<<3)));
//            LPC_I2C2 -> I2CONSET = 0X04;
//
//            LPC_I2C2->I2DAT = 0x00;
//            while (!(LPC_I2C2->I2CONSET & (1<<3)));
//            LPC_I2C2 -> I2CONSET = 0X04;
//            LPC_I2C2->I2DAT = 0x03<<4;
//            while (!(LPC_I2C2->I2CONSET & (1<<3)));
//            LPC_I2C2 -> I2CONSET = 0X04;
//            LPC_I2C2->I2DAT = 0x03<<4;
//            while (!(LPC_I2C2->I2CONSET & (1<<3)));
//
//            LPC_I2C2 -> I2CONSET = 0X04;
//            LPC_I2C2 -> I2CONCLR = 0x08;
//
//        	LPC_I2C2->I2DAT = 0x30;
//        	LPC_I2C2->I2CONCLR = 1<< 3;
//        	while (!(LPC_I2C2->I2CONSET & (1<<3)));
//
//        	LPC_I2C2 -> I2CONSET = 0X04;
//
//        	LPC_I2C2 -> I2CONCLR = 0x08;
//        	LPC_I2C2->I2DAT = 0x30;
//        	//LPC_I2C2->I2CONCLR = 1<< 3
//        	while (!(LPC_I2C2->I2CONSET & (1<<3)));
//
//        	LPC_I2C2 -> I2CONSET = 0X04;
//
//        	LPC_I2C2 -> I2CONCLR = 0x08;
//        	LPC_I2C2->I2DAT = 0x30;
//
//        	while (!(LPC_I2C2->I2CONSET & (1<<3)));
//
//        	LPC_I2C2 -> I2CONSET = 0X04;
//
//        	LPC_I2C2 -> I2CONCLR = 0x08;

    	}
//
//    			I2C2_Write(0x55);
//    			I2C2_Write(0xAA);
//    			I2C2_Stop();
    		//}
//    		if (st == 0x30||(st == 0x28))	//Consulte el registro I2STATUS
//    		{ //OTRAS ACCIONES
//    			//LPC_I2C2->I2DAT = 5;
//    			//LPC_I2C2->I2CONCLR = 1<< 3
//    			//while (!(LPC_I2C2->I2CONSET & (1<<3)));
//    			LPC_I2C2 -> I2CONSET = 0X14;
//    			LPC_I2C2 -> I2CONCLR = 0x08;
//
//    		}
//    		if (st == 0x38)	//Consulte el registro I2STATUS
//    		{ //OTRAS ACCIONES
//    			//LPC_I2C2->I2DAT = 5;
//    			//LPC_I2C2->I2CONCLR = 1<< 3
//    			//while (!(LPC_I2C2->I2CONSET & (1<<3)));
//    			LPC_I2C2 -> I2CONSET = 0X24;
//    			LPC_I2C2 -> I2CONCLR = 0x08;
//    		}
//    		if (st == 0x28)	//Consulte el registro I2STATUS
//    		{ //OTRAS ACCIONES
//    			LPC_I2C2->I2DAT = 5;
//    			//LPC_I2C2->I2CONCLR = 1<< 3
//    			//while (!(LPC_I2C2->I2CONSET & (1<<3)));
//    			LPC_I2C2 -> I2CONSET = 0X14;
//    			LPC_I2C2 -> I2CONCLR = 0x08;
////    		}
    		if (st == 0x20) I2C2_Stop( );	//Slave address but NO ACK
     }
    return 0 ;
}
void I2C2_enable (void) {
 	//LPC_I2C2->I2CONSET |= 1<<6;	//enable I2C2 // bien
 	LPC_I2C2 -> I2CONCLR |= (1<<3)|(1<<4)|(1<<5);
 	LPC_I2C2->I2CONSET |= 1<<6;

 	}

void I2C2_Start(void) {
	//ACTION;	//for MSO display
	LPC_I2C2->I2CONSET |= 1<< 5;	//START I2C2 // bien

	while (!(LPC_I2C2->I2CONSET & (1<<3)));//esperar hasta que cambie de estado
	LPC_I2C2->I2CONCLR |= (1<<3);
		LPC_I2C2->I2CONSET |= 1<< 5;
		LPC_I2C2->I2CONSET |= 1<< 6;
	//necesita cargar la dirección del esclavo
	//luego borre el bit de inicio en el registro I2CLR
}
void I2C2_Stop(void) {
LPC_I2C2->I2CONSET |= 1<<4;	//STOP I2C
CLEAR_SI;	//clear SI
while (LPC_I2C2->I2CONSET & (1<<4));	//wait until H/w stops I2c
ACTION_E;	//turn off MSO signa
}

void	conf(void) {
//LPC_GPIO0->FIODIR0 |= 0x33;	//P0.5 P0.4 P0.1 P0.0 ext pins 29,30,10,9
//LPC_SC->PCONP |= 1 << 26;	//re-enable POWER to I2C_2 if required
//LPC_SC->PCLKSEL1 |= 1<<20;	//pclk = cclk
//LPC_PINCON->PINSEL0 |= 0x02<<20;	//Pin P0.10 allocated to alternate function 2
//LPC_PINCON->PINSEL0 |= 0x02<<22;	//Pin P0.11 allocated to alternate function 2
//LPC_GPIO0->FIODIR |= 1<<10;	//Bit P0.10 an output
//LPC_GPIO0->FIODIR |= 1<<11;	//Bit P0.11 an output
//LPC_PINCON->PINMODE0 &= ~(3<<20);	//P0.10 has pull up/down resistor
//LPC_PINCON->PINMODE0 |= (2<<20);	//omit to use internal pull up
//LPC_PINCON->PINMODE0 &= ~(3<<22);	//P0.11 has pull up/down resistor
//LPC_PINCON->PINMODE0 |= (2<<22);	//omit to use internal pull up
//LPC_PINCON->PINMODE_OD0 |= 1<<10;	//Bit P0.10 is open drain
//LPC_PINCON->PINMODE_OD0 |= 1<<11;	//Bit P0.11 is open drain
//LPC_I2C2->I2SCLH = 60;	//100kHz from 12MHz
//LPC_I2C2->I2SCLL = 60;	//100kHz from 12MHz

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
unsigned char	I2C2_Address(unsigned char add) {
 	ADDRESS;	//signal for MSO
 	LPC_I2C2->I2DAT = add;	//the address
 	//LPC_I2C2->I2CONCLR = 1<<5;	//clear start
 	LPC_I2C2->I2CONCLR = 1<< 3;	//clear SI
 	while (!(LPC_I2C2->I2CONSET & (1<<3)));//esperar hasta que cambie de estado
 	ADDRESS_E;
 	return (LPC_I2C2->I2STAT);
 	}
void	I2C2_Write(char dat) {
 	WRITE;	//for display only
 	LPC_I2C2->I2DAT = dat ;	//new data
 	CLEAR_SI;
 	//WAIT_SI;	//wait until change in status
    while (!(LPC_I2C2->I2CONSET & (1<<3)));//esperar hasta que cambie de estado
 	WRITE_E;
 	}
