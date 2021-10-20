

/*
===============================================================================
 Name        : ADC_PRAXIS_PARCIAL2021.c
 Author      : $(author)
 Version     :
 Copyright   : $(copyright)
 Description : main definition
===============================================================================
*/

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#include "lpc17xx_pinsel.h"
#include "lpc17xx_adc.h"
#endif

#include <cr_section_macros.h>


void configPin();
void configADC();

__IO uint32_t valoradc; // Variable volatil donde se va a guardar el resultado de la conversion

int main(void) {


    configADC();
    configPin();

    while(1) {
    }
    return 0 ;
}
/*
 * Configuracion del PINSEL para el ADC P1[23]
 */

void configPin(){

	PINSEL_CFG_Type PinCfg;
	PinCfg.Portnum = PINSEL_PORT_0;
	PinCfg.Pinnum  = PINSEL_PIN_23;
	PinCfg.Funcnum = PINSEL_FUNC_1; // Funcion del ADC
	PinCfg.Pinmode = PINSEL_PINMODE_TRISTATE; // Obligatorio que no se ni pullup ni pulldown
	PinCfg.OpenDrain = PINSEL_PINMODE_NORMAL;

	PINSEL_ConfigPin(&PinCfg);

	LPC_GPIO0->FIODIR|= 1<<22;

 return;
}

void configADC(){

	ADC_Init(LPC_ADC, 200000); // Maximo frecuencia de muestreo 5us.
	ADC_ChannelCmd (LPC_ADC, 0, ENABLE); // Habilita el canal por donde entra la señal
	ADC_BurstCmd(LPC_ADC,ENABLE); // Habilita el modo burst
	ADC_IntConfig (LPC_ADC, ADC_ADINTEN0,ENABLE);//Interrupcion
    NVIC_EnableIRQ(ADC_IRQn);
	return;

}

void ADC_IRQHandler(void){

	 valoradc = ADC_ChannelGetData(LPC_ADC, 0);
     if(valoradc<2024){
          LPC_GPIO0->FIOCLR |= 1<<22;
	 }
    else{
		 LPC_GPIO0->FIOSET |= 1<<22; //Ojo logica cambiada
       }
}
