/*
===============================================================================
 Name        : ED3_ej_gpio.c
 Author      : Jonathan Patiño
 Description :
	 Generar un archivo header y c para control de puerto GPIO.
	¿Cómo sería?
	1- Crear funciones de configuración. Paso el puerto, paso el pin, y entrada-salida.
	2- Crear funciones de seteo.
	3- Crear funciones de status.
	4- Probarlo.
===============================================================================
*/

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#include <cr_section_macros.h>
#include <stdio.h>
#include <f_config_gpio.h>
//DEFINES



void retardo(void);

int main(void) {
	uint8_t portNum = 3;
	uint8_t pinNum = 25;
	uint8_t dir = 1;
	f_pin_conf(portNum, pinNum , dir);
	uint8_t state;
	while(1){

    	retardo();
    	state = 0;
    	f_pin_set( portNum,  pinNum , state);
    	retardo();
    	printf("Read 1 Pin %d \n",f_pin_read(portNum,pinNum));
    	state = 1;
    	f_pin_set( portNum,  pinNum , state);
    	printf("Read 2 Pin %d \n",f_pin_read(portNum,pinNum));
    }
	return 0 ;
}

void retardo(void){
	uint32_t conta;//,ver; // declara variable entera. conta ver.
	//ver = sizeof(conta); //se verifica el tamaño, en bytes, de la variable conta
	for(conta=0; conta < 1000000 ; conta++){} // lazo for retardo
}


