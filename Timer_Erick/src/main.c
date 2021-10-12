/*
===============================================================================
 Name        : TIMER_DELAY_PROTOTIPO.c

 Author      : $(author)
 Version     :
 Copyright   : $(copyright)
 Description : main definition
===============================================================================
VARIAMOS LA FRECUENCIA DEL LED 22 CON RETARDO GENERADO POR EL TIMER0;
*/

#ifdef __USE_CMSIS
#include "LPC17xx.h"

#include "lpc17xx_pinsel.h"
#include "lpc17xx_timer.h"
#include "lpc17xx_gpio.h"

#endif

#include <cr_section_macros.h>


void PinCfg();
void EINT0Cgf();
void retardo(uint32_t tiempo);

uint32_t timeDelay;//=timeDelay1;
//uint32_t timeDelay1 = 0 ;

uint32_t contador = 0;


//timeDelay = timeDelay1;
uint32_t frecuencia[5] ={300,500,700,800,1000};
uint32_t i = 0;

void togglear();
int main(void) {
	EINT0Cgf();
	LPC_GPIO0->FIODIR|=1<<22;
	LPC_GPIO0->FIOMASK = 0XFFFFFFFF&~(1<<22);
	PinCfg();
//	timeDelay1=500;
//	timeDelay = timeDelay1;

	while(1) {
		togglear();
    }
	return 0 ;
}
void togglear(){

    if(contador == 0){
    	LPC_GPIO0->FIOCLR |= 1<<22;  //Este prende
    }
    //retardo(timeDelay);
    if(contador == 1)
    {
    	LPC_GPIO0->FIOSET |= 1<<22;	//Este apaga
    }
}
//void retardo(uint32_t tiempo){
//	contador = 0;
//	TIM_Cmd(LPC_TIM0, DISABLE);
//	uint32_t timematch = tiempo;
//    TIM_UpdateMatchValue(LPC_TIM0,0, timematch);
//	TIM_Cmd(LPC_TIM0, ENABLE);
//	//while(contador == 0);//esta en 0 indefididamente hasta que interrumpa el timer0
//	//TIM_Cmd(LPC_TIM0, DISABLE);//INABILITO EL EL TIMER SOLO CUANDO DESEO USAR EL RETARDO UNA VEZ
//}
void PinCfg(){
	PINSEL_CFG_Type PinCfg;
	PinCfg.Portnum = PINSEL_PORT_1;
	PinCfg.Pinnum =PINSEL_PIN_28;//MATCH0.0   P1.28
	PinCfg.Funcnum =PINSEL_FUNC_3;
	PinCfg.Pinmode = PINSEL_PINMODE_TRISTATE;
	PinCfg.OpenDrain = PINSEL_PINMODE_NORMAL;

	PINSEL_ConfigPin(&PinCfg);

	TIM_TIMERCFG_Type TIM_ConfigStruct;
	TIM_ConfigStruct.PrescaleOption =TIM_PRESCALE_USVAL;
	TIM_ConfigStruct.PrescaleValue = 1000;

	TIM_Init(LPC_TIM0,TIM_TIMER_MODE, &TIM_ConfigStruct);//VALIDACION DE ESTRUCTURA

	TIM_MATCHCFG_Type TIM_MatchConfigStruct;
	TIM_MatchConfigStruct.ExtMatchOutputType =TIM_EXTMATCH_TOGGLE;
	TIM_MatchConfigStruct.IntOnMatch =ENABLE;
	TIM_MatchConfigStruct.MatchChannel = 0;
	TIM_MatchConfigStruct.MatchValue=  frecuencia[4];//1ms x 1000 = 1seg
	TIM_MatchConfigStruct.ResetOnMatch =ENABLE;
	TIM_MatchConfigStruct.StopOnMatch = DISABLE;

	TIM_ConfigMatch(LPC_TIM0,&TIM_MatchConfigStruct);//validacion de la estructura del match
	//HABILITA EL TC Y EL PR COUNTER
	//TIM_ResetCounter(LPC_TIM0);
	TIM_ClearIntPending(LPC_TIM0, TIM_MR0_INT);

	NVIC_SetPriority(TIMER0_IRQn, 1);
	NVIC_EnableIRQ(TIMER0_IRQn);

	TIM_Cmd(LPC_TIM0, ENABLE);

return;
}
void TIMER0_IRQHandler(void){
    contador ^= 1;  // Cambia de "1 a 0" o de "1 a 0"
    TIM_ResetCounter(LPC_TIM0);
    TIM_ClearIntPending(LPC_TIM0, TIM_MR0_INT);
}

void EINT0Cgf(){

	LPC_PINCON->PINSEL4|= 0b01<<20; //SE SUPONE QUE PINMODE ESTA POR DEFECTO EN PULLUP  P2.10---H[51]
	LPC_SC->EXTINT |= 1<<0;
	LPC_SC->EXTMODE|= 1<<0;
	LPC_SC->EXTPOLAR &= ~(1<<0); //FLANCO DE BAJADA
	//LPC_SC->EXTPOLAR = 1;//..0X01
	NVIC_SetPriority(TIMER0_IRQn, 0);
	NVIC_EnableIRQ(EINT0_IRQn);
	//NVIC_SetPriority(EINT2_IRQn,2);
}

void EINT0_IRQHandler(void){
	//static uint32_t i ;
	//i = 0;
	//Recorrido con rueda o escalera
	//uint32_t frecuencia[5] ={300,500,700,800,1000};
    timeDelay = frecuencia[i];

	//LPC_TIM0->MR0 = timeDelay1;
	TIM_Cmd(LPC_TIM0, DISABLE);
	TIM_UpdateMatchValue(LPC_TIM0,0, timeDelay);
	TIM_Cmd(LPC_TIM0, ENABLE);
	TIM_ResetCounter(LPC_TIM0);
	i++;
	i = i%5;
	LPC_SC->EXTINT |= 1<<0;
}


