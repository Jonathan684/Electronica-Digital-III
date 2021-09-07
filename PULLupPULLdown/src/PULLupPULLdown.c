

/*
===============================================================================
 Name        SECUENCIAS CON ARREGLO IAN.c
 Author      : $(author)ERIK CONCEPCION ALVARADO
 Version     :
 Copyright   : $(copyright)
 Description : main definition
===============================================================================
*/

#include "LPC17xx.h"

int retardo(uint32_t temp);
uint8_t f_pin_read(uint8_t portNum, uint8_t pinNum);
LPC_GPIO_TypeDef *f_get_gpiop(uint8_t portNum);
uint32_t frecuencia = 1000000;
uint32_t pulsador=0;

int main(void){

	LPC_GPIO0->FIODIR =0XFFF; //P[0] como salida
    LPC_GPIO2->FIODIR &= ~(1<<0);// Entrada pulsador
	LPC_PINCON->PINMODE4 &= ~(3<<0); //COMO PULLup al P2[0] ojo PullUp va con Flanco de bajada Quiere decir que el pin esta siempre en 1
	//LPC_PINCON->PINMODE4 |= (3<<1);// PullDown P2[0] va con RISE Flanco de subida . Quiere decir que el pin esta siempre en 0
    //LPC_GPIOINT->IO2IntEnR |= 1<<0;//SE LE HABILITA LA INTERRUPCION POR FLANCO DE SUBIDA AL P2[0]
	LPC_GPIOINT->IO2IntEnR |= 1<<0;// por flanco de bajada al P2[0]
	NVIC_EnableIRQ(EINT3_IRQn);// habilita las interrupciones
	LPC_GPIOINT->IO2IntClr |= 1<<0;
    while(1) {

        if(pulsador%2){//inte

    		frecuencia = 1000000;
    	}
    	else{

    		frecuencia = 8000000;
    	}
        LPC_GPIO0->FIOSET|=(1<<0);
    	    			retardo(frecuencia);
    	LPC_GPIO0->FIOCLR|=(1<<0);
    	    			 retardo(frecuencia);



    }
    return 0 ;
}


int retardo(uint32_t temp){
	uint32_t conta;
	for(conta=0;conta<temp;conta++){};
    return conta;
}

void EINT3_IRQHandler(void){

	if ((LPC_GPIOINT->IO2IntStatR) & (1<<0)){	//Si la interrupcion es por P0.15, sumar 1 a inte.

		  pulsador++;
          pulsador = pulsador%10;

	}

    LPC_GPIOINT->IO2IntClr |= 1<<0;//LIMPIO LA BANDERA DE INTERRUPCION DEL P2[0]
}

