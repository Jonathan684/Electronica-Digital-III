/*
===============================================================================
 Name        : ADC_gallardo.c
 Author      : $(author)
 Version     :
 Copyright   : $(copyright)
 Description : main definition
===============================================================================
1- Dada una señal de 100khz de componente espectral máxima
 que ingresa por un pin del ADC, se necesita procesar.
 El rango dinámico de amplitud es de 3.3v.
 Proponer otra señal de frecuencia definida para que samplee
 a la señal conectada al ADC y poder reconstruirla. 300 samples.

 Escriba el programa de la manera más conveniente.
 Definir si burst o software, tiempos de sampleo, tipo de variables.
 */

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#include <cr_section_macros.h>

#include <stdio.h>

// TODO: insert other include files here
#include "lpc17xx_adc.h"
#include "lpc17xx_gpio.h"
#include "lpc17xx_pinsel.h"
// TODO: insert other definitions and declarations here
#define ADC_Channel ADC_CHANNEL_0

__IO uint32_t adc_value; // Necesario para ir cambiando el valor en cada conversion



void confADC(void);
void confPin(void);


int main(void) {

    printf("CONVERSOR\n");
    confADC();
    confPin();
    while(1) {
    	ADC_StartCmd(LPC_ADC, ADC_START_NOW);
    	NVIC_EnableIRQ(ADC_IRQn);
    	for(int tmp=0 ; tmp<100000000;tmp++);
    }
    return 0 ;
}

void confADC(void){




}


void confPin(void){
	PINSEL_CFG_Type PinConf;
	PinConf.Portnum =  PINSEL_PORT_0;
	PinConf.Funcnum =  PINSEL_FUNC_1;
	PinConf.Pinmode =  PINSEL_PINMODE_TRISTATE; //Obligatorio para el ADC
	PinConf.Pinnum  =  PINSEL_PIN_23;
	PinConf.OpenDrain =PINSEL_PINMODE_NORMAL;
	PINSEL_ConfigPin(&PinConf);
}

void confADC(void){
	uint32_t rate = 200000; // 200 Khz
	ADC_Init(LPC_ADC, rate);
	ADC_IntConfig(LPC_ADC, ADC_CHANNEL_0, ENABLE);
	ADC_ChannelCmd(LPC_ADC, ADC_Channel, ENABLE);

	ADC_StartCmd(LPC_ADC,ADC_START_ON_MAT01);
	NVIC_SetPriority(ADC_IRQn,(9));


//	LPC_SC->PCONP |= (1 << 12);
//	LPC_ADC->ADCR |= (1 << 21);   //enable ADC
//	LPC_SC->PCLKSEL0 |= (3<<24);  //CCLK/8
//	LPC_ADC->ADCR &=~(255 << 8);  //[15:8] CLKDIV
//	LPC_ADC->ADCR |= (1 << 0);    // channel
//	LPC_ADC->ADCR &= ~(1 << 16);  // no burst
//	LPC_ADC->ADCR |= (1 << 26);   // match
//	LPC_ADC->ADCR &= ~(3 << 24);  // match
//	LPC_ADC->ADCR |= (1 << 27);   //edge
//	//LPC_PINCON->PINSEL4 |= (1<<20);
//	LPC_PINCON->PINMODE1 |= (1<<15); //neither pull-up nor pull-down.
//	LPC_PINCON->PINSEL1 |= (1<<14);
//	LPC_ADC->ADINTEN |= (1<<0);
//	NVIC_EnableIRQ(ADC_IRQn);

	return;
}


void ADC_HandlerIRQ(void){
	adc_value = 0;
	if(ADC_ChannelGetStatus(LPC_ADC, ADC_Channel, ADC_DATA_DONE)){
		adc_value = ADC_ChannelGetData(LPC_ADC, ADC_Channel);
		NVIC_DisableIRQ(ADC_IRQn);
	}
}

