/*
===============================================================================
 Name        : Interrupciones_externas2.c
 Author      : $(author)
 Version     :
 Copyright   : $(copyright)
 Description : main definition
===============================================================================
*/
/*
Interrupción externa_2
Escribir un programa en C que permita sacar por los pines P1.16 al
P1.23 el equivalente en ascii de "a" si la interrupción se ha realizado
 por interrupción de EINT1 y no hay interrupción pendiente de EINT0.
Para el caso en el cual se produzca una interrupción por EINT1
y exista una interrupción pendiente de EINT0 sacar por el  puerto el equivalente en ascii de "A".
La interrupción de EINT1 es por el flanco de subida producido por un pulsador identificado como "tecla a/A",
 mientras que la interrupción EINT0 es por el nivel alto de un pulsador etiquetado como "Activación de Mayusculas".

Nota: Valerse de la configuración de los niveles de prioridad para que la pulsación conjunta de
"Activación de Mayúsculas" con "tecla a/A" de como resultado a la salida el equivalente en ascii "A".
*/
#include "LPC17xx.h"

void configGPIO(void);
void configEINT(void);

int main(void){
    configGPIO();
    configEINT();
    while(1)
    {

    }
}
void configGPIO(){
    LPC_PINCON->PINSEL3 &= ~(0XFFFF);//P1.16 al P1.23 como GPIO (GPIO viene por defecto)

    LPC_GPIO1->FIODIR   |= (0X00FF0000);//los pongo como salida
    LPC_GPIO2->FIODIR   |= (1<<0);

    return;
}
/*
 * La interrupción de EINT1 es por el flanco de subida
 * EINT0 =
 */
void configEINT(void){
	///// CONFIG EINT0 ////
	LPC_PINCON->PINSEL4 |= (1<<20); // Habilita la interrupcion por EINT0
	LPC_SC->EXTMODE     &= ~(1<<0); // Seleciono EINT0 por nivel
	LPC_SC->EXTPOLAR    |= (1<<0);	// Nivel alto en EINT0
	LPC_SC->EXTINT      |= (1<<0);  // 000.....01 EINT0

	NVIC_EnableIRQ(EINT0_IRQn);     // Habilita la interrupcion














    //LPC_PINCON->PINSEL4 |= (1<<22); // Habilita la interrupcion por EINT1

    //LPC_SC->EXTMODE |= (1<<1);    // Selecciono EINT1 por flanco
    //LPC_SC->EXTPOLAR  &= ~(1<<0); // Flanco de subida en EINT1

    //LPC_SC -> EXTINT  |= (1<<1);  // 000.....10 EINT1
    //NVIC_SetPriority(EINT1_IRQn,3);
    //NVIC_SetPriority(EINT0_IRQn,5);
    // Habilita la interrupcion
    //NVIC_EnableIRQ(EINT1_IRQn);
}
/*
 * PULSADOR EN EL P2[11] H[52]
 */
void EINT1_IRQHandler(void){
    uint8_t letra; // 97 = a y 65 = A
    if(NVIC_GetPendingIRQ(EINT0_IRQn)){
        letra = 65;
        LPC_GPIO1 -> FIOSET2 = letra;
    }
    else{
        letra = 97;
        LPC_GPIO1 -> FIOSET2 = letra;
    }
    LPC_SC ->EXTINT |= 2;//Bajo la bandera de la interrupcion de EINT1
}
/*
 * PULSADOR EN EL P2[10] H[51]
 */
void EINT0_IRQHandler(void){
	LPC_SC ->EXTINT |= (1<<0);//Bajo la bandera de la interrupcion
}
//NVIC_GetPendingIRQ(EINT1_IRQn) -> dEVU
/** \brief  Get Pending Interrupt

    This function reads the pending register in the NVIC and returns the pending bit
    for the specified interrupt.

    \param [in]      IRQn  Number of the interrupt for get pending
    \return             0  Interrupt status is not pending
    \return             1  Interrupt status is pending
 */
