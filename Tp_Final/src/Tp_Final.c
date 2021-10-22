/*
===============================================================================
 Name        : Tp_Final.c
 Author      : $(author)
 Version     :
 Copyright   : $(copyright)
 Description : main definition
===============================================================================
*/

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#include "lpc17xx_timer.h"
#endif

#include <cr_section_macros.h>

#include <stdio.h>

// TODO: insert other include files here
void delay_ms(int t);
// TODO: insert other definitions and declarations here
void confTimer0(uint32_t, uint32_t);
void configIntExt(void);


uint8_t Duty_Cycle = 0;
int main(void) {
	SystemInit();
    //delay_ms(100000000);
    printf("Hello World\n");
    configIntExt();
    
    while(1) {
    }
    return 0 ;
}

//SENSOR de la canilla
void configIntExt(void){
	LPC_GPIO0 -> FIODIR = (1<<22);
    LPC_GPIO2 -> FIODIR = (1<<8);

	LPC_GPIO2-> FIOCLR |= (1<<10);
    LPC_PINCON->PINSEL4 |=(1<<20);    //elegimos P2.10 como EINT0
    LPC_SC->EXTMODE  |=   (1<<0);     //Selecciono para que la interrupcion sea por nivel
    LPC_SC->EXTPOLAR &=  ~(1<<0);     //Interrumpe cuando el flanco es
    LPC_SC->EXTINT   |=   (1<<0);     //Limpio el flag de la interrupcion
    NVIC_SetPriority(TIMER0_IRQn, 3); //Menor prioridad
    NVIC_EnableIRQ(EINT0_IRQn);       //Habilito la interrupcion

    return;
}

void EINT0_IRQHandler(void){
    // generar un pwm con timer de ..... 
    // mg996r
	delay_ms(6000000);
	
    if(LPC_SC->EXTPOLAR &(1<<0)){ //Interrumpe cuando el nivel es alto
        //inicio el timer con una cierta frecuencia 20ms. giro derecha
        LPC_SC->EXTPOLAR &=  ~(1<<0);        //Interrumpe cuando el nivel es bajo
        //CERRAR CANILLA

        //confTimer0(12499, 249999);
        LPC_GPIO0-> FIOCLR = (1<<22);
    }
    else if((LPC_SC->EXTPOLAR &(1<<0)) == 0){ //Interrumpe cuando el flanco es bajo
        LPC_SC->EXTPOLAR |= (1<<0);        //Interrumpe cuando el flanco es ascendente
        //ABRIR LA CANILLA GIRO -60°
        //inicio el timer con una cierta frecuencia 40ms. giro izquierda
        // RESET
        // CARGAMOS TIEMPO 1 MS VG    
        LPC_GPIO0->FIOSET = (1<<22);
        confTimer0(12499, 249999);
    }
    LPC_SC->EXTINT |= (1<<0);//Limpio el flag de la interrupcion
}
void delay_ms(int t){
for(int i=0;i<t;i++){};
}

//Configuracion de timer para generar la señal PWM que maneje el servomotor y gire a la izquierda
void confTimer0(uint32_t match0, uint32_t match1){
    LPC_SC->PCONP       |= (1<<1);  //Habilito TMR0(prendo el periferico)
    LPC_SC->PCLKSEL0    |= (3<<2);  //Uso el CLK/8 para TIMER0 (12.5MHz)
    LPC_PINCON->PINSEL3 |= (3<<24); // Selecciona funcion MATCH0
    //prescaler por defecto en 0
    //No usamos la salida del match EMC
    LPC_TIM0->MR0        = match0;    //con este valor matchea a 1ms(12499)
    LPC_TIM0->MR1        = match1;   //con este valor matchea a 20ms (249999)
    
    //por defecto no se resetean cuando matchean ni detienen la cuenta
    LPC_TIM0->MCR       |= (1<<0);      //interrumpe el match0
    LPC_TIM0->MCR       &= ~(1<<1);     //No reset
    LPC_TIM0->MCR       |= (1<<3);      //interrumpe el match1
    LPC_TIM0->MCR       |= (1<<4);     // Reset

    LPC_TIM0->TCR        = 3;       //Habilita TC  y PC con el bit 0 =1 y resetea simultaneamente Tc y Pc en el siguiente pulso ascendente
	LPC_GPIO2 -> FIOSET |= (1<<8);					
    LPC_TIM0->TCR       &= ~(1<<1);	//los saco del reset asi empiezan a contar 
    NVIC_EnableIRQ(TIMER0_IRQn);
    
    return;
}
void TIMER0_IRQHandler(void){
    //vemos quien interrumpio (si match0 o match1)
    if(TIM_GetIntStatus(LPC_TIM0, TIM_MR0_INT)){ //Me fijo si interr el MATCH0
        LPC_GPIO2 -> FIOCLR |= (1<<8); //Pongo en bajo la salida  
        Duty_Cycle ++;// Incrementa hasta llegar a 50
        TIM_ClearIntPending(LPC_TIM0, TIM_MR0_INT);
        LPC_TIM0->TCR |= (1<<0);
        //TIM_ClearIntPending(LPC_TIM0, TIM_MR0_INT);
    }
    else if(TIM_GetIntStatus(LPC_TIM0, TIM_MR1_INT))//Me fijo si interr el MATCH1
    {
        LPC_GPIO2 -> FIOSET |= (1<<8); //Pongo en alto la salida
        TIM_ResetCounter(LPC_TIM0);
        TIM_ClearIntPending(LPC_TIM0, TIM_MR1_INT);
    }
    if(Duty_Cycle == 50){
        Duty_Cycle = 0;
        NVIC_DisableIRQ(TIMER0_IRQn); 
    }         
}
