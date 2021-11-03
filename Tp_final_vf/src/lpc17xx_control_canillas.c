/*
 *===============================================================================
 *
 *===============================================================================
 */

#include "librerias.h"
# define  VERDADERO  1
# define  FALSO  0
# define  BAJO 	 0
# define  ALTO   1
# define  PIN22 ( uint32_t ) ( 1 << 22 )
# define  PORT0 ( uint8_t ) (( 0 ))
# define  OUTPUT ( uint8_t ) (( 1 ))

void configTIMERS();
void configGPIO ();
void configIntExt();
void delay_ext0();
int duty = 0;

void  TIMER0_IRQHandler ( void ) {
	if(TIM_GetIntStatus(LPC_TIM0, TIM_MR0_INT)) {  //Me fijo si interrumpio el Match0
		//LPC_TIM0->MCR &=  ~(1<<0);
		LPC_TIM0->IR |= (1<<0);
        LPC_GPIO0->FIOCLR |= (1<<4);
        duty ++;
		//TIM_ClearIntPending(LPC_TIM0, TIM_MR0_INT);
	}
	if(TIM_GetIntStatus(LPC_TIM0, TIM_MR1_INT)) { //Me fijo si interrumpio el Match1
		//TIM_ClearIntPending(LPC_TIM0,s TIM_MR1_INT);
		//LPC_TIM0->MCR &=  ~(1<<1);
		LPC_TIM0->IR |= (1<<1);
        LPC_GPIO0->FIOSET |= (1<<4);
	}
	if(duty == 40){
		LPC_GPIO0->FIOCLR |= (1<<4);
		NVIC_DisableIRQ (TIMER0_IRQn);
		duty = 0;

	}
}
void  configTIMERS() {
	TIM_Cmd (LPC_TIM0, DISABLE);
	LPC_TIM0->IR |= (0XF);

    TIM_TIMERCFG_Type struct_timer;
    TIM_MATCHCFG_Type struct_match;

    struct_timer. PrescaleOption = TIM_PRESCALE_USVAL;
    struct_timer. PrescaleValue = 100 ; // 100us

    struct_match. MatchChannel = 0 ;      //match0
    struct_match. IntOnMatch = ENABLE;
    struct_match. StopOnMatch = DISABLE; // Que no se detenga luego
    struct_match. ResetOnMatch = DISABLE;// No se resetea
    struct_match. ExtMatchOutputType = TIM_EXTMATCH_NOTHING;
    struct_match. MatchValue =  10; //canilla cerrada con el valor 10

    TIM_ConfigMatch (LPC_TIM0, &struct_match);

    struct_match. MatchChannel = 1 ;      //match1
    struct_match. IntOnMatch = ENABLE;    //interrumpe
    struct_match. StopOnMatch = DISABLE;  // Que no se detenga luego
    struct_match. ResetOnMatch = ENABLE;  //se resetea el TC cuando interrumpe
    struct_match. ExtMatchOutputType = TIM_EXTMATCH_NOTHING;
    struct_match. MatchValue = 200 ;       // 20 ms(periodo del servo)

    TIM_ConfigMatch (LPC_TIM0, &struct_match);

    TIM_Init(LPC_TIM0, TIM_TIMER_MODE,&struct_timer);

    TIM_Cmd (LPC_TIM0, DISABLE);
    NVIC_DisableIRQ (TIMER0_IRQn);

    return ;
}
void  configGPIO () {
	LPC_GPIO0 -> FIODIR |= (1<<4);  //Salida
    LPC_GPIO0 -> FIOCLR |= (1<<4);  
	return ;
}
//SENSOR de la canilla
void configIntExt(void){
	
    LPC_PINCON->PINSEL4 |= (1<<20);    //elegimos P2.10 como EINT0
    LPC_SC->EXTMODE     |= (1<<0);     //Selecciono para que la interrupcion sea por FLANCO
    LPC_SC->EXTPOLAR    &= ~(1<<0);    //Interrumpe cuando el flanco es ASCENDENTE
    LPC_SC->EXTINT      |=  (1<<0);    //Limpio el flag de la interrupcion
    NVIC_SetPriority(EINT0_IRQn, 3);   //Menor prioridad
    NVIC_EnableIRQ(EINT0_IRQn);        //Habilito la interrupcion

    return;
}
/*
 * Generar un pwm con Timer0
 * Servo : MG996R
 */ 
void EINT0_IRQHandler(void){
	
    //duty=0;
	LPC_SC->EXTINT |= (1<<0);//Limpio el flag de la interrupcion
	delay_ext0(3000000); // Sensor de movimiento rebote de 330ms 
	if(LPC_SC->EXTPOLAR &(1<<0)){ //Interrumpe cuando el nivel es alto
        // ABRIR LA CANILLA GIRO 60° aprox horario
        LPC_SC->EXTPOLAR &=  ~(1<<0);        //Interrumpe cuando el nivel es bajo
        //CERRAR CANILLA
        TIM_Cmd(LPC_TIM0, DISABLE);            //Deshabilito el Timers 
        TIM_UpdateMatchValue(LPC_TIM0, 0, 10); //Cambio el Match
        TIM_ResetCounter(LPC_TIM0);            //Reset TC
        TIM_Cmd(LPC_TIM0, ENABLE);             //Habilito el timers
        NVIC_SetPriority(TIMER0_IRQn, 3);      // 
        NVIC_EnableIRQ (TIMER0_IRQn);
        LPC_GPIO2 -> FIOSET |= (1<<4);         //Comienza el pwm salida en la P[2].4
        //LPC_SC->EXTINT |= (1<<0);//Limpio el flag de la interrupcion 
    }
    else if((LPC_SC->EXTPOLAR &(1<<0)) == 0){  //Interrumpe cuando el flanco es bajo
        LPC_SC->EXTPOLAR |= (1<<0);        //Interrumpe cuando el flanco es ascendente
        // ABRIR LA CANILLA GIRO -60° aprox antihorario
        TIM_Cmd(LPC_TIM0, DISABLE);
        TIM_UpdateMatchValue(LPC_TIM0, 0, 30); // 3 milisegundos
        TIM_ResetCounter(LPC_TIM0);
        TIM_Cmd(LPC_TIM0, ENABLE);
        NVIC_SetPriority(TIMER0_IRQn, 3);
        NVIC_EnableIRQ (TIMER0_IRQn);
        LPC_GPIO2 -> FIOSET |= (1<<4);
    }
    //while(duty > 0){} 
    
}
/*
 * Retardo antirebote
 */
void delay_ext0(int t){
  for(int i=0;i<t;i++){};
}
//Configuracion de timer para generar la señal PWM que maneje el servomotor y gire a la izquierda

