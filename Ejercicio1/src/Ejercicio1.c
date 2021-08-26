/*
===============================================================================
 Name        : ED3_ej_gpio.c
 Author      : Gomez, Augusto Rodrigo
 Description :
	 Generar un archivo header y c para control de puerto GPIO.
	¿Cómo sería?
	1- Crear funciones de configuración. Paso el puerto, paso el pin, y entrada-salida.
	2- Crear funciones de seteo.
	3- Crear funciones de status.
	4- Probarlo.
===============================================================================
*/
#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#include <cr_section_macros.h>
#include <stdio.h>

//DEFINES
#define MID 16U

void f_pin_conf(uint8_t portNum, uint8_t pinNum , uint8_t dir);

int main(void) {

    return 0 ;
}

void f_pin_conf(uint8_t portNum, uint8_t pinNum , uint8_t dir) {

	LPC_PINCON_TypeDef  *pPINCON= LPC_PINCON;
	LPC_GPIO_TypeDef 	*pGPIO  = 0;

	/*Set pin of certain port as GPIO*/
	switch (portNum) {
		case 0:
			if(pinNum< MID) pPINCON-> PINSEL0 &= ~(0b11<<(2*pinNum));
			else pPINCON-> PINSEL1 &= ~(0b11<<(2*pinNum-16));
			break;
		case 1:
			if(pinNum< MID) pPINCON-> PINSEL2 &= ~(0b11<<(2*pinNum));
			else pPINCON-> PINSEL3 &= ~(0b11<<(2*pinNum-16));
			break;
		case 2:
			if(pinNum< MID) pPINCON-> PINSEL4 &= ~(0b11<<(2*pinNum));
			else pPINCON-> PINSEL5 &= ~(0b11<<(2*pinNum-16));
			break;
		case 3:
			if(pinNum< MID) pPINCON-> PINSEL6 &= ~(0b11<<(2*pinNum));
			else pPINCON-> PINSEL7 &= ~(0b11<<(2*pinNum-16));
			break;
		case 4:
			if(pinNum< MID) pPINCON-> PINSEL8 &= ~(0b11<<(2*pinNum));
			else pPINCON-> PINSEL9 &= ~(0b11<<(2*pinNum-16));
			break;
		default:
			break;
	}

	/*Get GPIO pionter*/
	switch (portNum) {
		case 0:
			pGPIO = LPC_GPIO0;
			break;
		case 1:
			pGPIO = LPC_GPIO1;
			break;
		case 2:
			pGPIO = LPC_GPIO2;
			break;
		case 3:
			pGPIO = LPC_GPIO3;
			break;
		case 4:
			pGPIO = LPC_GPIO4;
			break;
		default:
			break;
	}

	/*Set as Input or Output*/
	if (pGPIO != NULL) {
		// Enable Output
		if (dir) {
			pGPIO->FIODIR |= (1<<pinNum);
		}
		// Enable Input
		else {
			pGPIO->FIODIR &= ~(1<<pinNum);
		}
	}
}
