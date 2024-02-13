/*
===============================================================================

===============================================================================
 */
/*
================================================== =============================
 Nombre:  FinalProject.c
 Autores: $ (autor)
 Versión :
 Copyright: $ (copyright)
 Descripción: Timer0 con Match para parpadear un led en PIN0.22
 	       El led se encuentra 40ms en alto y 40ms en bajo.
Cálculo del temporizador:
CLK = 100MHz => PCLK = CLK / 4 = 25MHz => 1 ciclo = 40ns
		1 TC ---> (PR + 1) ciclos = (100 + 1) ciclos = 4.04us
	    10000 TC ---> 40.4ms
Los partidos se producen cada 40.4ms. Y cambian el estado del LED.
(Ej MatchValue = 250000 y PR = 100 entonces interrumpe cada 1seg)
================================================== =============================
*/
#include "librerias.h"
# define  VERDADERO  1
# define  FALSO  0
# define  BAJO 	0
# define  ALTO  1
# define  PIN22 ( uint32_t ) ( 1 << 22 )
# define  PORT0 ( uint8_t ) (( 0 ))
# define  OUTPUT ( uint8_t ) (( 1 ))

void configTIMERS();
void  configGPIO ();
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
    struct_match. ResetOnMatch = DISABLE;
    struct_match. ExtMatchOutputType = TIM_EXTMATCH_NOTHING;
    struct_match. MatchValue =  10; //giro hacia un lado 24 y 5

    TIM_ConfigMatch (LPC_TIM0, &struct_match);

    struct_match. MatchChannel = 1 ;      //match1
    struct_match. IntOnMatch = ENABLE;      //interrumpe
    struct_match. StopOnMatch = DISABLE; // Que no se detenga luego
    struct_match. ResetOnMatch = ENABLE;  //se resetea el TC cuando interrumpe
    struct_match. ExtMatchOutputType = TIM_EXTMATCH_NOTHING;
    struct_match. MatchValue = 200 ;  // 20 ms

    TIM_ConfigMatch (LPC_TIM0, &struct_match);

    TIM_Init(LPC_TIM0, TIM_TIMER_MODE,&struct_timer);

    TIM_Cmd (LPC_TIM0, DISABLE);
    LPC_GPIO2 -> FIOCLR |= (1<<8);
    //LPC_GPIO2 -> FIOSET |= (1<<8);
    NVIC_DisableIRQ (TIMER0_IRQn);

    return ;
}
void  configGPIO () {
	LPC_GPIO0 -> FIODIR |= (1<<4);
    LPC_GPIO0 -> FIOCLR |= (1<<4);
	return ;
}
//SENSOR de la canilla
void configIntExt(void){
	

    LPC_PINCON->PINSEL4 |= (1<<20);    //elegimos P2.10 como EINT0
    LPC_SC->EXTMODE     |= (1<<0);     //Selecciono para que la interrupcion sea por FLANCO
    LPC_SC->EXTPOLAR    &= ~(1<<0);     //Interrumpe cuando el flanco es ASCENDENTE
    LPC_SC->EXTINT      |=  (1<<0);     //Limpio el flag de la interrupcion
    NVIC_SetPriority(TIMER0_IRQn, 4); //Menor prioridad
    NVIC_EnableIRQ(EINT0_IRQn);       //Habilito la interrupcion

    return;
}
void EINT0_IRQHandler(void){
	LPC_SC->EXTINT |= (1<<0);//Limpio el flag de la interrupcion
    // generar un pwm con Timer0
    // MG996R
	delay_ext0(3000000); // PRUEBA 6000000 , 3000000
	
    if(LPC_SC->EXTPOLAR &(1<<0)){ //Interrumpe cuando el nivel es alto
        //inicio el timer con una cierta frecuencia 20ms. giro derecha
        LPC_SC->EXTPOLAR &=  ~(1<<0);        //Interrumpe cuando el nivel es bajo
        //CERRAR CANILLA
        TIM_Cmd(LPC_TIM0, DISABLE);
        TIM_UpdateMatchValue(LPC_TIM0, 0, 10);
        TIM_ResetCounter(LPC_TIM0);
        TIM_Cmd(LPC_TIM0, ENABLE);
        NVIC_SetPriority(TIMER0_IRQn, 3);
        NVIC_EnableIRQ (TIMER0_IRQn);
        LPC_GPIO2 -> FIOSET |= (1<<4);
    }
    else if((LPC_SC->EXTPOLAR &(1<<0)) == 0){ //Interrumpe cuando el flanco es bajo
        LPC_SC->EXTPOLAR |= (1<<0);        //Interrumpe cuando el flanco es ascendente
        //ABRIR LA CANILLA GIRO -60° aprox
        //inicio el timer con una cierta frecuencia 40ms. giro izquierda
        // RESET
        // CARGAMOS TIEMPO 1 MS VG
        TIM_Cmd(LPC_TIM0, DISABLE);
        TIM_UpdateMatchValue(LPC_TIM0, 0, 30);
        TIM_ResetCounter(LPC_TIM0);
        TIM_Cmd(LPC_TIM0, ENABLE);
        NVIC_SetPriority(TIMER0_IRQn, 3);
        NVIC_EnableIRQ (TIMER0_IRQn);
        LPC_GPIO2 -> FIOSET |= (1<<4);
    }

}
void delay_ext0(int t){
  for(int i=0;i<t;i++){};
}
//Configuracion de timer para generar la señal PWM que maneje el servomotor y gire a la izquierda

