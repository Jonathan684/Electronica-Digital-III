/*
===============================================================================
 Name        : Encendido_Led_22.c
 Author      : $(author)
 Version     :
 Copyright   : $(copyright)
 Description : main definition
===============================================================================
*/

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

void retardo(void);
#include <cr_section_macros.h>

// TODO: insert other include files here

// TODO: insert other definitions and declarations here

int main(void) {

	uint32_t verClk;
	verClk = SystemCoreClock;  //Se verifica la frecuencia del CPU
	LPC_GPIO0 -> FIODIR |= (1<<22);

	while(1) {
		LPC_GPIO0->FIOSET = (1<<22);// pone en alto al pin 22, encendiendo el led.
		retardo(); // Invoca a la funcion retardo.
		LPC_GPIO0-> FIOCLR = (1<<22); // pone en cero al pin 22, apagandp el led.
		retardo(); // Invoca a la funcion retarado.
	}// Fin del while
    return 0 ;
 } // Fin del main

void retardo(void){
	uint32_t conta,ver; // declara variable entera. conta ver.
	ver = sizeof(conta); //se verifica el tamaño, en bytes, de la variable conta
	for(conta=0; conta < 1000000 ; conta++){} // lazo for retardo
}
