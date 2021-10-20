/*
===============================================================================
 Name        : ADC_4canales.c
 Author      : $ Jonathan Patiño
 Version     :
 Copyright   : $(copyright)
 Description : main definition
===============================================================================
Configurar 4 canales del ADC para que funcionando en modo burst
se obtenga una frecuencia de muestreo en cada uno de 50Kmuestras/seg.
Suponer un cclk = 100 Mhz
*/

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#include "lpc17xx_adc.h"
#endif

#include <cr_section_macros.h>

#include <stdio.h>

// TODO: insert other include files here

// TODO: insert other definitions and declarations here
void confADC(void);
__IO uint32_t valor_adc;
int main(void) {

    printf("ADC 4 canales\n");
    confADC();
    while(1) {

    }
    return 0 ;
}

void confADC(void){

	LPC_PINCON->PINSEL1   |=   (1<<14)|(1<<16)|(1<<18)|(1<<20);   //adc canal 0-1-2-3 entrada de la señal analogica a muestrear
	LPC_PINCON->PINMODE1  |=   (1<<15)|(1<<17)|(1<<19)|(1<<21);   //ni pull up ni pull down si o si para el adc

	LPC_SC  -> PCLKSEL0  |=  (3<<24);  //pclk = cclk // Configuro el divisor del clock
	LPC_SC  -> PCON 	 |= (1<<12);   //Enciendo el conversor

	LPC_ADC -> ADCR    |= ((1<<0)|(1<<1)|(1<<2)|(1<<3));       // Selecciona canal 0-1-2-3
	LPC_ADC -> ADCR    |= (1<<21);      //Habilita el adc

	LPC_ADC -> ADCR    &= ~(0xFF<<8);   //Divisor interno por 1

	LPC_ADC -> ADCR	   |= (1<<16);     //Modo Burst
	LPC_ADC -> ADINTEN |= 1 ;
	NVIC_EnableIRQ(ADC_IRQn);
}
void ADC_IRQHandler(void){

	if(LPC_ADC->ADSTAT & (1<<0)){ //Pregunto si termino la conversion el canal 0
		valor_adc = (LPC_ADC->ADDR0>>4)&0XFFF; //El valor siempre esta en el mismo lugar
	}
	else if(LPC_ADC->ADSTAT & (1<<1)){//Pregunto si termino la conversion el canal 0
		valor_adc = (LPC_ADC->ADDR1>>4)&0XFFF; //El valor siempre esta en el mismo lugar
	}
	else if(LPC_ADC->ADSTAT & (1<<2)){//Pregunto si termino la conversion el canal 0
		valor_adc = (LPC_ADC->ADDR2>>4)&0XFFF; //El valor siempre esta en el mismo lugar
	}
	else if(LPC_ADC->ADSTAT & (1<<3)){//Pregunto si termino la conversion el canal 0
		valor_adc = (LPC_ADC->ADDR3>>4)&0XFFF; //El valor siempre esta en el mismo lugar
	}
}
