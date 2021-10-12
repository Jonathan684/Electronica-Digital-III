

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


void PinCfg();
void ADCCfg();

__IO uint32_t valoradc;
int main(void) {

    LPC_GPIO0->FIODIR|= 1<<22;
    ADCCfg();
    PinCfg();

    while(1) {




    }
    return 0 ;
}


void PinCfg(){

	PINSEL_CFG_Type PinCfg;
	PinCfg.Portnum = PINSEL_PORT_0;
	PinCfg.Pinnum =PINSEL_PIN_23;
	PinCfg.Funcnum =PINSEL_FUNC_1;
	PinCfg.Pinmode = PINSEL_PINMODE_TRISTATE;
	PinCfg.OpenDrain = PINSEL_PINMODE_NORMAL;

	PINSEL_ConfigPin(&PinCfg);

 return;
}

void ADCCfg(){

	ADC_Init(LPC_ADC, 200000);
	ADC_ChannelCmd (LPC_ADC, 0, ENABLE);
	ADC_BurstCmd(LPC_ADC,ENABLE);
	ADC_IntConfig (LPC_ADC, ADC_ADINTEN0,ENABLE);
    NVIC_EnableIRQ(ADC_IRQn);
	return;

}

void ADC_IRQHandler(void){


	valoradc = ADC_ChannelGetData(LPC_ADC, 0);

	 if(valoradc<2024){

		 LPC_GPIO0->FIOCLR |= 1<<22;
	 }

	 else{
		 LPC_GPIO0->FIOSET |= 1<<22;

	 }

return;

}
