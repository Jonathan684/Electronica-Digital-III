

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
#include "lpc17xx_timer.h"


#endif

#include <cr_section_macros.h>


void PinCfg();
void ADCCfg();
void TimerCfg();
void retardo();
__IO uint32_t ADCVALUE = 0 ;
uint32_t volts;
int main(void) {

    LPC_GPIO0->FIODIR|= 1<<22;
    LPC_GPIO0->FIODIR|= 1<<9;
    LPC_GPIO0->FIODIR = 0XFF;//SALIDAS DESDE P0.0 .....P0.7
    LPC_GPIO0->FIOMASK =0XFFFFFFFF&~(0XFF)&~(1<<9)&~(1<<22);//ENMASCARO LOS PINES QUE NO VOY A USAR


    ADCCfg();
    PinCfg();
    TimerCfg();
     retardo();

    while(1) {

    	LPC_GPIO0->FIOSET|= 1<<22;
    	retardo();
    	LPC_GPIO0->FIOCLR|= 1<<22;
    	 retardo();




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
	//ADC_BurstCmd(LPC_ADC,ENABLE);
	ADC_StartCmd(LPC_ADC, ADC_START_ON_MAT01);
	ADC_IntConfig (LPC_ADC, ADC_ADINTEN0,ENABLE);

	NVIC_EnableIRQ(ADC_IRQn);
	return;

}

void ADC_IRQHandler(void){
	static uint16_t i;

	ADCVALUE = ADC_ChannelGetData(LPC_ADC, 0);//SOLO 8 BITS
	//ADCVALUE = ADCVALUE&0XFF;
	volts = (3.3/4095)*ADCVALUE;
	//ADCVALUE = (volts/2)*256;

//	for(i=0;i<8;i++){
//		if(ADCVALUE&(1<<i)){
//
//			LPC_GPIO0->FIOSET|= 1<<i;
//		}
//		else{
//			LPC_GPIO0->FIOCLR|= 1<<i;
//
//		}
//
//
//
//	}



	 if(ADCVALUE>2024){

		 LPC_GPIO0->FIOCLR |= 1<<9;
	 }

	 else{
		 LPC_GPIO0->FIOSET |= 1<<9;

	 }

return;

}

void TimerCfg(){
	PINSEL_CFG_Type PinCfg;
		PinCfg.Portnum = PINSEL_PORT_1;
		PinCfg.Pinnum =PINSEL_PIN_29;//MATCH0.1   P1.29
		PinCfg.Funcnum =PINSEL_FUNC_3;
		PinCfg.Pinmode = PINSEL_PINMODE_TRISTATE;
		PinCfg.OpenDrain = PINSEL_PINMODE_NORMAL;

		PINSEL_ConfigPin(&PinCfg);

		TIM_TIMERCFG_Type TIM_ConfigStruct;
		TIM_ConfigStruct.PrescaleOption = TIM_PRESCALE_USVAL;
		TIM_ConfigStruct.PrescaleValue =1000;

		TIM_Init(LPC_TIM0, TIM_TIMER_MODE, &TIM_ConfigStruct);

		TIM_MATCHCFG_Type TIM_MatchConfigStruct;
		TIM_MatchConfigStruct.ExtMatchOutputType =TIM_EXTMATCH_TOGGLE;
		TIM_MatchConfigStruct.IntOnMatch = DISABLE;
		TIM_MatchConfigStruct.MatchChannel = 1;
		TIM_MatchConfigStruct.MatchValue = 1000;
		TIM_MatchConfigStruct.ResetOnMatch =ENABLE;
		TIM_MatchConfigStruct.StopOnMatch = DISABLE;

		TIM_ConfigMatch(LPC_TIM0, &TIM_MatchConfigStruct);

		TIM_Cmd(LPC_TIM0, ENABLE);


 return;

}

void retardo(){
uint32_t i;

for(i=0;i<1000000;i++){};
return;

}




