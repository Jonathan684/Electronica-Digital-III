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
#endif

#include <cr_section_macros.h>

#include <stdio.h>

// TODO: insert other include files here

// TODO: insert other definitions and declarations here
void configIntExt(void);

int main(void) {
	SystemInit();
    printf("Hello World\n");
    configIntExt();
    while(1) {
    }
    return 0 ;
}

//SENSOR de la canilla
void configIntExt(void){
	LPC_GPIO0 -> FIODIR = (1<<22);
	LPC_GPIO2-> FIOCLR |= (1<<10);
    LPC_PINCON->PINSEL4 |=(1<<20);  //elegimos P2.10 como EINT0
    LPC_SC->EXTMODE  |=   (1<<0);    //Selecciono para que la interrupcion sea por nivel
    LPC_SC->EXTPOLAR &=  ~(1<<0);      //Interrumpe cuando el flanco es
    LPC_SC->EXTINT   |=   (1<<0);      //Limpio el flag de la interrupcion
                                    //Menor prioridad
    NVIC_EnableIRQ(EINT0_IRQn);     //Habilito la interrupcion
    return;
}

void EINT0_IRQHandler(void){
    // generar un pwm con timer de ..... 
    // mg996r

	for(int i=0;i<100000;i++){};
    if(LPC_SC->EXTPOLAR &(1<<0)){ //Interrumpe cuando el nivel es alto
        //inicio el timer con una cierta frecuencia 20ms. giro derecha
        LPC_SC->EXTPOLAR &=  ~(1<<0);        //Interrumpe cuando el nivel es bajo
        LPC_GPIO0-> FIOCLR = (1<<22);
    }
    else if((LPC_SC->EXTPOLAR &(1<<0)) == 0){ //Interrumpe cuando el flanco es bajo
        //inicio el timer con una cierta frecuencia 40ms. giro izquierda
        LPC_SC->EXTPOLAR |= (1<<0);        //Interrumpe cuando el flanco es ascendente
        LPC_GPIO0->FIOSET = (1<<22);
    }
    LPC_SC->EXTINT |= (1<<0);//Limpio el flag de la interrupcion
}
