///*
//===============================================================================
// Name        : ED3_ej_TMR_match_4.c
// Author      : $(author)
// Version     :
// Copyright   : $(copyright)
// Description :
//
// Utilizando los 4 registros match del Timer 0 y las salidas P0.0, P0.1, P0.2
// y P0.3, realizar un programa en C que permita obtener las formas de ondas
// adjuntas, donde los pulsos en alto tienen una duración de 5 mseg. Un pulsador
// conectado a la entrada EINT3, permitirá elegir entre las dos secuencias mediante
// una rutina de servicio a la interrupción. La prioridad de la interrupción del
// Timer tiene que ser mayor que la del pulsador. Estas formas de ondas son muy
// útiles para controlar un motor paso a paso. Adjuntar el código en C.
//===============================================================================
//*/
//
//#ifdef __USE_CMSIS
//#include "LPC17xx.h"
//#include "LPC17XX_gpio.h"
//#include "LPC17XX_timer.h"
//#endif
//
//#include <cr_section_macros.h>
//
//void confGPIO();
//void confEint();
//void confTimer0();
//uint32_t  val_match [] = {49,99,149,199};
//
//int main(void) {
//	confGPIO();
//	confEint();
//	confTimer0();
//	while(1){
//
//	}
//    return 0 ;
//}
//
//void confGPIO (void) {
//	//Conf Basica
//	LPC_PINCON -> PINSEL0 &= ~(0xFF << 0); //P0.0 P0.1 P0.2 P0.3
//	LPC_GPIO0  -> FIODIR0 |= 0x0F; //pines como salida
//}
//
//void confEint (void) {
//	//Conf Basica
//	LPC_PINCON -> PINSEL4  |= (1 << 26); //P2.13 como EINT3
//	LPC_PINCON -> PINMODE4 &= ~(3 << 26);//P2.13 tiene PullUp
//	//Conf IntExt
//	LPC_SC -> EXTMODE 	   |= (1 << 3);  //EINT3 es Edge-sensitive
//	LPC_SC -> EXTPOLAR     &= ~(1 << 3); //EINT3 es fallingEdge-sensitive
//	LPC_SC -> EXTINT	   |= (1 << 3);  //Clear EINT3 flag
//	NVIC_EnableIRQ(EINT3_IRQn);
//    //NVIC_SetPriority(IRQn, priority);
//}
///*
// * Por defecto clock de 25 MHz
// */
//void confTimer0 (void) {
//
//	TIM_TIMERCFG_Type struct_config;
//	TIM_MATCHCFG_Type struct_match;
//
//	struct_config.PrescaleOption = TIM_PRESCALE_USVAL; //En vez de cuentas utilizamos el timepo en microsegundos
//	struct_config.PrescaleValue = 100; // 100 microsegundos = 100; // 100 microsegundos
//
//
//	//struct_match.MatchChannel = i;
//	    	//struct_match.MatchValue = val_match[i];
//	        struct_match.MatchChannel = 0; // Tipo de match
//	    	struct_match.IntOnMatch = ENABLE; // Interrupcion por match
//	    	struct_match.ResetOnMatch = ENABLE; // Reset el timer cuando llegue al match
//	    	struct_match.StopOnMatch = DISABLE; // Para el timer
//	    	struct_match.ExtMatchOutputType = TIM_EXTMATCH_NOTHING; //No hay nada en la salida
//	    	struct_match.MatchValue = 5000;//val_match[i]; // =100 mseg = 0.1 SEG son 1000 cuentas del timer y cada cuenta es 100 milisegudos
//	        TIM_ConfigMatch(LPC_TIM0, &struct_match);
//	//
//
//
//	/*for(int  i = 0 ; i<4 ; i++){
//    	//struct_match.MatchChannel = i;
//    	//struct_match.MatchValue = val_match[i];
//        struct_match.MatchChannel = i; // Tipo de match
//    	struct_match.IntOnMatch = ENABLE; // Interrupcion por match
//    	struct_match.ResetOnMatch = ENABLE; // Reset el timer cuando llegue al match
//    	struct_match.StopOnMatch = DISABLE; // Para el timer
//    	struct_match.ExtMatchOutputType = TIM_EXTMATCH_NOTHING; //No hay nada en la salida
//    	struct_match.MatchValue = 49 ;//val_match[i]; // =0.1 mseg son 1000 cuentas del timer y cada cuenta es 100 milisegudos
//        TIM_ConfigMatch(LPC_TIM0, &struct_match);
//
//    }*/
//
//    TIM_Init(LPC_TIM0,TIM_TIMER_MODE,&struct_config);
//    TIM_Cmd(LPC_TIM0, ENABLE);
//    //uint32_t aux =  LPC_SC -> PCLKSEL0;
//    TIM_ClearIntPending(LPC_TIM0, TIM_MR0_INT);
//    NVIC_EnableIRQ(TIMER0_IRQn);
//}
//
//void TIMER0_IRQHandler(void){
//	TIM_ClearIntPending(LPC_TIM0, TIM_MR0_INT);
//	GPIO_SetValue(0,0);
//}
///*
//===============================================================================
// Name        : Timer_and_GPIO.c
// Author      :
// Version     :
//===============================================================================
//*/
//
#include "LPC17xx.h"

/*!
 * Header files to project. Include library
 */

#include "lpc17xx_timer.h"
#include "lpc17xx_gpio.h"
#include "lpc17xx_pinsel.h"


// TODO: insert other definitions and declarations here

#define	OUTPUT	    (uint8_t) 1
#define INPUT	    (uint8_t) 0

#define PIN_22		((uint32_t)(1<<22))
#define PIN_0		((uint32_t)(1<<0))
#define PIN_1		((uint32_t)(1<<1))
#define PORT_ZERO	(uint8_t)	0
#define PORT_ONE	(uint8_t)	1
#define PORT_TWO	(uint8_t)	2
#define PORT_THREE	(uint8_t)	3

void config_GPIO(void);
void config_timer(void);
void confSystick(void);


int main(void) {

	config_GPIO();
	config_timer();
	confSystick();
	GPIO_SetValue(PORT_ZERO,PIN_22);

	while(1) {}
    return 0 ;
}

void config_GPIO(){
	PINSEL_CFG_Type pin_configuration;

	pin_configuration.Portnum 	=	PINSEL_PORT_2;
	pin_configuration.Pinnum	=	PINSEL_PIN_0;
	pin_configuration.Pinmode	=	PINSEL_PINMODE_PULLUP;
	pin_configuration.Funcnum	= 	PINSEL_FUNC_0;
	pin_configuration.OpenDrain	=	PINSEL_PINMODE_NORMAL;

	PINSEL_ConfigPin(&pin_configuration);

	GPIO_SetDir( PORT_TWO , PIN_0, OUTPUT );

	pin_configuration.Portnum 	=	PINSEL_PORT_2;
	pin_configuration.Pinnum	=	PINSEL_PIN_1;
	pin_configuration.Pinmode	=	PINSEL_PINMODE_PULLUP;
	pin_configuration.Funcnum	= 	PINSEL_FUNC_0;
	pin_configuration.OpenDrain	=	PINSEL_PINMODE_NORMAL;

	PINSEL_ConfigPin(&pin_configuration);

	GPIO_SetDir( PORT_TWO , PIN_1 , OUTPUT );

	return;
}

void config_timer(){
	TIM_TIMERCFG_Type	struct_config;
	TIM_MATCHCFG_Type	struct_match;

	struct_config.PrescaleOption	=	TIM_PRESCALE_USVAL;
	struct_config.PrescaleValue		=	100;

	struct_match.MatchChannel		=	0;
	struct_match.IntOnMatch			=	ENABLE;
	struct_match.ResetOnMatch		=	ENABLE;
	struct_match.StopOnMatch		=	DISABLE;
	struct_match.ExtMatchOutputType	=	TIM_EXTMATCH_NOTHING;
	struct_match.MatchValue			=	9999;

	TIM_Init(LPC_TIM0, TIM_TIMER_MODE, &struct_config);
	TIM_ConfigMatch(LPC_TIM0, &struct_match);

	TIM_Cmd(LPC_TIM0, ENABLE);

	NVIC_EnableIRQ(TIMER0_IRQn);

	return;
}
/*
 *Ojo funciona con clock de 100 Mhz
 *Interrumpir cada 1ms
 *Formula PERIODO_INT= (RECARGA+1)*PERIODO_CLK
          RECARGA= (PERIODO_INT/PERIODO_CLK)-1
 */
void confSystick(void){
    SysTick -> CTRL = 0; // Deshabilito el systick
    SysTick -> LOAD = 9999999UL; // Periodo del clock
    //NVIC_SetPriority(SysTick_IRQn, 0); // Setea prioridades, por defecto esta en -1
    SysTick ->VAL = 0; //Limpia el valor actual
    SysTick ->CTRL = 0x00000007; // Enable = 1 TickInt = 1 Clock Source = 1
    NVIC_SetPriority(SysTick_IRQn, 0);
    //NVIC_EnableIRQ(SysTick_IRQn); // Habilita la interrupcion
    //SysTick_Config(ticks)
}
/*
 * LED RAPIDO
 */
void SysTick_Handler(){
    if (GPIO_ReadValue(PORT_TWO)&PIN_0) {
    		GPIO_ClearValue(PORT_TWO, PIN_0);
    	} else {
    		GPIO_SetValue(PORT_TWO,PIN_0);
    	}
}
/*
 * LED LENTO
 */
void TIMER0_IRQHandler(void){

	TIM_ClearIntPending(LPC_TIM0, TIM_MR0_INT);
	if (GPIO_ReadValue(PORT_TWO)&PIN_1) {
	    		GPIO_ClearValue(PORT_TWO, PIN_1);
	   } else {
	   	GPIO_SetValue(PORT_TWO,PIN_1);
	   	}
//	if (GPIO_ReadValue(PORT_ZERO)&PIN_22) {
//		GPIO_ClearValue(PORT_ZERO, PIN_22);
//	} else {
//		GPIO_SetValue(PORT_ZERO,PIN_22);
//	}
//	return;
}
