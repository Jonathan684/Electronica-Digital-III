#include "librerias.h"
/*
 * Configuracion del sensor para la alarma.
 */
void config_sensor(){
	LPC_GPIO1->FIOCLR |=(1<<22);  //SALIDA PARA EL BUZZ
	PINSEL_CFG_Type pin_configuration;
	//// Salida del Match Timer1
	pin_configuration.Portnum 	=	PINSEL_PORT_1;
	pin_configuration.Pinnum	=	PINSEL_PIN_22;
	pin_configuration.Pinmode	=	PINSEL_PINMODE_PULLUP;
	pin_configuration.Funcnum	= 	PINSEL_FUNC_3;
	pin_configuration.OpenDrain	=	PINSEL_PINMODE_NORMAL;

	PINSEL_ConfigPin(&pin_configuration);


	PINSEL_CFG_Type pinEintConf;

	//P2.10 -> EINT0
	pinEintConf.Pinnum  = PINSEL_PIN_11;
	pinEintConf.Portnum = PINSEL_PORT_2;
	pinEintConf.Pinmode = PINSEL_PINMODE_PULLUP;
	pinEintConf.Funcnum = PINSEL_FUNC_1;


	PINSEL_ConfigPin(&pinEintConf);

	//EXTI conf
	EXTI_InitTypeDef eint0Conf;
	eint0Conf.EXTI_Line     = EXTI_EINT1;
	eint0Conf.EXTI_Mode     = EXTI_MODE_EDGE_SENSITIVE;
	eint0Conf.EXTI_polarity = EXTI_POLARITY_HIGH_ACTIVE_OR_RISING_EDGE;

	EXTI_Config(&eint0Conf);
	EXTI_ClearEXTIFlag(EXTI_EINT1);
	NVIC_SetPriority(TIMER0_IRQn, 0);
	NVIC_DisableIRQ(EINT1_IRQn);
    //NVIC_EnableIRQ(EINT1_IRQn);
}

void EINT1_IRQHandler (void) {
	if(LPC_SC->EXTINT &(1<<1)){
	Disparar_alarma();
    LPC_TIM1->EMR |=(3<<4); //TOGGLE
    //LPC_TIM1->EMR &= ~((1<<4)|(1<<5)); //LOW
     
	//TIM_Cmd(LPC_TIM1, ENABLE);
	//LPC_SC -> PCONP |= (1<<1);
	}
    EXTI_ClearEXTIFlag(EXTI_EINT1);
}

void confTimer1(void) {
	TIM_TIMERCFG_Type	timer_config;
	TIM_MATCHCFG_Type	timer_match;

	timer_config.PrescaleOption	    =	TIM_PRESCALE_USVAL;
	timer_config.PrescaleValue		=	500; //

	timer_match.MatchChannel 		=	0;
	timer_match.IntOnMatch			=	DISABLE;
	timer_match.StopOnMatch		    =	DISABLE;
	timer_match.ExtMatchOutputType	=	TIM_EXTMATCH_LOW; //TIM_EXTMATCH_TOGGLE;
	timer_match.ResetOnMatch 		=	ENABLE;
	timer_match.MatchValue 			=   1000;
	TIM_ConfigMatch(LPC_TIM1, &timer_match);

	TIM_Init(LPC_TIM1, TIM_TIMER_MODE, &timer_config);

    TIM_Cmd(LPC_TIM1, ENABLE);
	TIM_ResetCounter(LPC_TIM1);
	//LPC_SC -> PCONP &= ~(1<<1); // Apago el timers
    //NVIC_DisableIRQ(TIMER1_IRQn);
	//NVIC_EnableIRQ(TIMER1_IRQn);
}
