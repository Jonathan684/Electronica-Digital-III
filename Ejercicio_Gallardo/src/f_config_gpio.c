/*
 * f_config_gpio.c
 *
 *  Created on: 25 ago. 2021
 *      Author: Administrador
 */


#include "f_config_gpio.h"

LPC_GPIO_TypeDef *f_get_gpiop(uint8_t portNum) {

	LPC_GPIO_TypeDef 	*pGPIO  = 0;

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

	return pGPIO;
}

void f_pin_conf(uint8_t portNum, uint8_t pinNum , uint8_t dir) {

	LPC_PINCON_TypeDef  *pPINCON= LPC_PINCON;
	LPC_GPIO_TypeDef 	*pGPIO  = f_get_gpiop(portNum);

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

void f_pin_set(uint8_t portNum, uint8_t pinNum , uint8_t state) {

	LPC_GPIO_TypeDef 	*pGPIO  = f_get_gpiop(portNum);

	if (pGPIO != NULL) {
			if(state) pGPIO ->FIOSET |= (1<<pinNum);
			else	  pGPIO ->FIOCLR |= (1<<pinNum);
		}
}
uint8_t f_pin_read(uint8_t portNum, uint8_t pinNum){
	LPC_GPIO_TypeDef 	*pGPIO  = f_get_gpiop(portNum);
	uint32_t value;
	uint32_t masked_n,mask;
	uint8_t thebit;
	mask =  1 << pinNum;
	if (pGPIO != NULL) {
			value = pGPIO ->FIOPIN;  //32
			masked_n = value & mask;
			thebit = masked_n >> pinNum;
     }
	return thebit;
}


