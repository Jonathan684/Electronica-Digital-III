/*
===============================================================================
 Name        : Ejercicio_Franco.c
 Author      : $(author)
 Version     :
 Copyright   : $(copyright)
 Description : main definition
===============================================================================
*/

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#ifndef NULL
#define NULL ((void*) -1)
#endif
#include <cr_section_macros.h>

typedef struct
{
  __IO uint32_t FIODIR;
}LPC_GPIO_TypeDef_1;
void config_gpio(uint8_t portNum,  uint32_t bitValue,uint8_t dir);
//static LPC_GPIO_TypeDef *GPIO_GetPointer(uint8_t portNum);
void set_gpio(void);
void get_gpio(void);
void retardo(void);

int main(void){

	SystemInit();
	uint32_t verClk;
	verClk = SystemCoreClock; //tengo 100Mhz
	uint8_t portNum = 0;
	uint8_t dir = 1;
	uint32_t bitValue = (0<<22); // ->
	config_gpio(portNum,bitValue,dir);
	dir = 0;
	bitValue = (0<<22);
	config_gpio(portNum,bitValue,dir);
	set_gpio();
	get_gpio();

	while(1){
	}
	return 0;
}
  //{//uint8_t portNum,  uint32_t bitValue,uint8_t dir){
/*#define LPC_GPIO0_BASE        (LPC_GPIO_BASE + 0x00000)
  #define LPC_GPIO1_BASE        (LPC_GPIO_BASE + 0x00020)
  #define LPC_GPIO2_BASE        (LPC_GPIO_BASE + 0x00040)
  #define LPC_GPIO3_BASE        (LPC_GPIO_BASE + 0x00060)
 #define LPC_GPIO4_BASE        (LPC_GPIO_BASE + 0x00080)*/
	void config_gpio(uint8_t portNum,  uint32_t bitValue,uint8_t dir){

		switch(portNum){
		case 0:
				LPC_PINCON->PINSEL0 = (0x000000); //pongo el puerto 0 como GPIO
				LPC_PINCON->PINSEL1 = (0x000000);
				break;
		}
		//LPC_PINCON->PINSEL0 = (0x000000); //pongo el puerto 0 como GPIO
		//LPC_PINCON->PINSEL1 = (0x000000);

		LPC_PINCON->PINMODE0 =(0x55555555);//pongo todos los pines en repeater mode (01)
		if(dir){
			LPC_GPIO0->FIODIR = (0xFFFFFFFF);//configuro el puerto como salida
		}
		else{
			LPC_GPIO0->FIODIR = (0x000000);//configuro el puerto como salida
		}

				return;
}

	void set_gpio(void){

		LPC_GPIO0->FIOSET = (0xFFFFFFFF); //seteo todos los pines del puerto
		retardo();
		LPC_GPIO0->FIOCLR = (0XFFFFFFFF); //pongo en bajo los pines del puerto
		retardo();
		return;

}
	void get_gpio(void){

		if(LPC_GPIO0->FIOPIN == (0XFFFFFFFF)){ //leo si los pines estan en 1 y no hago nada

		}
			return;
	}

	void retardo(void){

		uint32_t contador;//declaro variable
		for(contador=0; contador< 6000000 ; contador++){

		}
		return;
	}
