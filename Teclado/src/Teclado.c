/*
===============================================================================
 Name        : Teclado.c
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
//Definición de los pines correspondientes al teclado
//estas dos variables facilitan el uso de bucles for, como se verá en algunas funciones
uint16_t FilasTeclado[4]={GPIO_Pin_0 , GPIO_Pin_1 , GPIO_Pin_2, GPIO_Pin_3};
uint16_t ColumnasTeclado[4]={GPIO_Pin_1 , GPIO_Pin_2, GPIO_Pin_3, GPIO_Pin_5 };
//esta matriz contiene cada uno de los caracteres correspondientes a las 16 teclas existentes, servirá para
//identificar correctamente la tecla pulsada
char matriz[4][4]={{'1','2','3', 'F'},{'4','5','6', 'E'},{'7','8','9', 'D'},{'A','0','B', 'C'}};

int main(void) {

    printf("Teclado Matricial \n");
    LPC_GPIO2->FIODIR &= ~(0xF<<1); //Se pone como entrada 4
    LPC_GPIO2->FIODIR |= (1<<5)|(1<<6)|(1<<7); //3 Como salida
    LPC_GPIO0->FIODIR |= 1<<0; //P0[0] como salida
    LPC_GPIOINT->IO2IntEnR |= 1<<1; //Por flanco de bajada al P2[0]

    while(1) {

    }
    return 0 ;
}
