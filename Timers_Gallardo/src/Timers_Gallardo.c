/*


===============================================================================
 Name        : Timers_Gallardo.c
 Author      : $(author)
 Version     :
 Copyright   : $(copyright)
 Description : main definition
===============================================================================
 */
#include "lpc17xx.h"
#include "lpc17xx_timer.h"
#include "lpc17xx_gpio.h"
#include "lpc17xx_pinsel.h"
#include "lpc17xx_clkpwr.h"


// TODO: insert other definitions and declarations here
//Generar con timer0 una señal de freq. variable.
//- Usando el capture “medir”el periodo usando otro timer.
//- Prender leds tipo vúmetro según la frecuencia.
//- Con un pulsador cambiar la frecuencia de pasos de 100khz. Actualizar el vúmetro.
#define	OUTPUT	    (uint8_t) 1
#define INPUT	    (uint8_t) 0

#define PIN_22		((uint32_t)(1<<22))
#define PIN_0		((uint32_t)(1<<0))
#define PIN_1		((uint32_t)(1<<1))
#define PORT_ZERO	(uint8_t)	0
#define PORT_ONE	(uint8_t)	1
#define PORT_TWO	(uint8_t)	2
#define PORT_THREE	(uint8_t)	3


void config_timer(void);
void confGPIO(void);

uint32_t aux1 = 0;
uint32_t aux2 = 0;
uint32_t aux3 = 0;
uint8_t i = 0;
uint8_t MatchChannel = 0;
uint8_t x = 0;
int main(void) {
	SystemInit();
	confGPIO();
	config_timer();

    while(1) {

    	if(x == 10){
    		TIM_UpdateMatchValue(LPC_TIM1,0, 9000);
    		LPC_TIM1 ->EMR &= ~(1<<0);
    		TIM_ResetCounter(LPC_TIM1);
    		x ++;
    	}
    	if(x == 20){
    		TIM_UpdateMatchValue(LPC_TIM1,0, 5000);
    		LPC_TIM1 ->EMR &= ~(1<<0);
    		TIM_ResetCounter(LPC_TIM1);
    		x ++;
    	}
    	if(x == 30){
    		TIM_UpdateMatchValue(LPC_TIM1,0, 1000);
    		LPC_TIM1 ->EMR &= ~(1<<0);
    		TIM_ResetCounter(LPC_TIM1);
    		x++;
    	}
    	if(x == 100){
    		x = 0;
    	}
    }
	return 0 ;
}
void confGPIO(void){
	 LPC_GPIO0 -> FIODIR |= (1<<22);
	 PINSEL_CFG_Type pin_configuration;
     //// Salida del Match
	 pin_configuration.Portnum 	=	PINSEL_PORT_1;
	 pin_configuration.Pinnum	=	PINSEL_PIN_22;
	 pin_configuration.Pinmode	=	PINSEL_PINMODE_TRISTATE;
	 pin_configuration.Funcnum	= 	PINSEL_FUNC_3;
	 pin_configuration.OpenDrain	=	PINSEL_PINMODE_NORMAL;

	 PINSEL_ConfigPin(&pin_configuration);

	 ////////  Modo captura ////////////////////////////////////
	 pin_configuration.Portnum 	=	PINSEL_PORT_1;
	 pin_configuration.Pinnum	=	PINSEL_PIN_26;
	 pin_configuration.Funcnum	= 	PINSEL_FUNC_3;
	 pin_configuration.Pinmode   =   PINSEL_PINMODE_PULLUP;
	 pin_configuration.OpenDrain =   PINSEL_PINMODE_NORMAL;

	 PINSEL_ConfigPin(&pin_configuration);

}
void config_timer(){

//	LPC_SC -> PCONP |= (1<<1);
//	LPC_SC -> PCLKSEL0 |= (1<<2);
//	LPC_PINCON -> PINSEL3 |= (3<<20);
//	LPC_TIM0 -> CCR |= (1<<1) | (1<<2);
//	LPC_TIM0 -> TCR = 3;
//	LPC_TIM0 -> TCR &= ~(1<<1);
//	NVIC_EnableIRQ(TIMER0_IRQn);




	TIM_CAPTURECFG_Type confCapture;

	confCapture.CaptureChannel = 0;
	confCapture.FallingEdge =  DISABLE; //Habilito la interrupcion por flanco de DESCENDENTE
	confCapture.RisingEdge  =  ENABLE;  //Habilito la interrupcion por flanco de ASCENDENTE
	confCapture.IntOnCaption = ENABLE; //Habilito la interrupcion por flanco

	TIM_ConfigCapture(LPC_TIM0, &confCapture);




//    ///////////////////////////////////////////////////////////////
    TIM_TIMERCFG_Type	struct_config;
    TIM_MATCHCFG_Type	struct_match;

    struct_config.PrescaleOption	=	TIM_PRESCALE_USVAL;
    struct_config.PrescaleValue		=	100;

    struct_match.MatchChannel		=	0;     	 //canal 0
    struct_match.IntOnMatch			=	ENABLE;	 //Habilita las interrupciones por Match
    struct_match.ResetOnMatch		=	ENABLE;	 //Resetea el Match
    struct_match.StopOnMatch		=	DISABLE;
    struct_match.ExtMatchOutputType	=	TIM_EXTMATCH_HIGH;
    struct_match.MatchValue			=	10000;

    TIM_Init(LPC_TIM1, TIM_TIMER_MODE, &struct_config);
    TIM_ConfigMatch(LPC_TIM1, &struct_match);


    TIM_ClearIntPending(LPC_TIM1, TIM_MR0_INT); //MR0 DEL TIMER 1
    TIM_ClearIntPending(LPC_TIM0, TIM_CR0_INT);

    TIM_Cmd(LPC_TIM0, ENABLE);
    TIM_Cmd(LPC_TIM1, ENABLE);

    NVIC_EnableIRQ(TIMER0_IRQn);
    NVIC_EnableIRQ(TIMER1_IRQn);
    aux1 = 0;
    aux2 = 0;
    aux3 = 0;
    i = 0;

}

void TIMER0_IRQHandler(void){

        if (i == 0){
		aux1 = TIM_GetCaptureValue(LPC_TIM0 , TIM_COUNTER_INCAP0);
		LPC_GPIO0-> FIOSET = (1<<22);
		aux3=0;
		i = 1;

	}
	else if(i == 1)
   {   aux2 = TIM_GetCaptureValue(LPC_TIM0 , TIM_COUNTER_INCAP0);
		LPC_GPIO0 -> FIOCLR = (1<<22);
		aux3 = aux2 - aux1;
		i = 0;
	}
    TIM_ClearIntCapturePending(LPC_TIM0, TIM_CR0_INT);
}
void TIMER1_IRQHandler(void){
	LPC_TIM1 ->EMR |= (0b01<<4);
	x++;
	TIM_ClearIntPending(LPC_TIM1, TIM_MR0_INT);

}

