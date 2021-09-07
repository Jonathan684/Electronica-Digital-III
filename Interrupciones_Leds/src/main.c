

/*
===============================================================================
 Name        SECUENCIAS CON ARREGLO IAN.c
 Author      : $(author)ERIK CONCEPCION ALVARADO
 Version     :
 Copyright   : $(copyright)
 Description : main definition
===============================================================================

Realizar un programa que configure el puerto P2.0 y P2.1 para que provoquen una interrupción por flanco de subida para el primer pin y por flanco de bajada para el segundo.
Cuando la interrupción sea por P2.0 se enviará por el pin P0.0
la secuencia de bits 010011010. Si la interrupción es por P2.1 se enviará por el pin P0.1 la secuencia 011100110.
Las secuencias se envían únicamente cuando se produce una interrupción, en caso contrario la salida de los pines tienen valores 1 lógicos.
¿que prioridad tienen configuradas por defecto estas interrupciones?
*/

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#include <cr_section_macros.h>


int retardo(uint32_t temp);

uint32_t *puntero = 0x10003000;
uint32_t *puntero2 = 0x10003010;
uint32_t *aux =0x10004000;
uint32_t k = 1000000;
uint32_t inte=0;

int main(void) {

	 LPC_GPIO0->FIODIR = 0XFFF;
     LPC_GPIO0->FIOMASK = 0XFFFFFFFF &~(0XFF)&(1<<8)&(1<<9);

	 *puntero = 0b1010101010;
	 *puntero2 =0b1111111111;
	 uint32_t i,j,pos =0;
	 LPC_GPIO2->FIODIR &= ~(1<<0);// P2[0] entrada
	 LPC_GPIO2->FIODIR &= ~(1<<1);//P2[1] ENTRADA
	 LPC_PINCON->PINMODE4 |= (3<<0);//pulldown P2[0]  //
	 LPC_PINCON->PINMODE4 &= ~(3<<1);// PULLUP P2[1] va con RISE Flanco de subida

	 LPC_GPIOINT->IO2IntEnR |= 1<<0;//SE LE HABILITA LA INTERRUPCION POR FLANCO DE SUBIDA AL P2[0] esdecir envio un 1
	 LPC_GPIOINT->IO2IntEnF |= 1<<1;// por flanco de bajada al P2[1] es decir "0"
	 NVIC_EnableIRQ(EINT3_IRQn);// habilita las interrupciones
	 LPC_GPIOINT->IO2IntClr |= 1<<0;

    while(1){

        //*************************************************************************************
    	// SECUENCIA CON PUNTERO Y FIOPIN

       if(inte%2){
    	  k = 1000000;
    		*aux = *puntero2;
    	}
    	else{
            k = 8000000;
    		*aux = *puntero;
    	}

       for(i=0;i<10;i++){
    		if(*aux&(1<<i)){
    			LPC_GPIO0->FIOPIN = 1<<i;//se corren lugare a la izquierda
    			retardo(k);
    		}
         }
       for(i=10;i>0;i--){
    		if(*aux&(i>>1)){//se corren lugares a la derecha forma de testear y el fiopin los extrae el bit del puntero
    		    			LPC_GPIO0->FIOPIN = (1<<i);
    		    			retardo(k);
    		    		}
         }

    }
    return 0 ;
}


int retardo(uint32_t temp){
	uint32_t conta;
	for(conta=0;conta<temp;conta++){};

	return conta;

}


void EINT3_IRQHandler(void){

	if((LPC_GPIOINT->IO2IntStatR&(1<<0))){
     inte++;
	 inte = inte%10;
	}
    LPC_GPIOINT->IO2IntClr |= 1<<0;//LIMPIO LA BANDERA DE INTERRUPCION DEL P2[0]
    if((LPC_GPIOINT->IO2IntStatF&(1<<1))){

    	inte++;
		inte = inte%10;//cuando llega a 10 se hacce cro

	}

		LPC_GPIOINT->IO2IntClr |= 1<<1;//LIMPIO LA BANDERA DE INTERRUPCION DEL P2[1]//limpio las banderas de interrupcion
		LPC_GPIOINT->IO2IntClr |= 1<<0;//LIMPIO LA BANDERA DE INTERRUPCION DEL P2[0]
}
