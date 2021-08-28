//EJERCICIO
/*
 * Hacer funciones de configuracion .Paso el puerto, paso el pin,y
 * entrada-salida.
 * Crear funcion de seteo
 * Crear funcion de status
 */

#include"LPC17xx.h"


void config_gpio(void);
void set_gpio(void);
void get_gpio(void);
void retardo(void);

int main(void){

	SystemInit();
	uint32_t verClk;
	verClk = SystemCoreClock; //tengo 100Mhz

	config_gpio();
	set_gpio();
	get_gpio();

	while(1){
	}
	return 0;
}

	void config_gpio(void){

		LPC_PINCON->PINSEL0 = (0x000000); //pongo el puerto 0 como GPIO
		LPC_PINCON->PINSEL1 = (0x000000);

		LPC_PINCON->PINMODE0 =(0x55555555);//pongo todos los pines en repeater mode (01)

		LPC_GPIO0->FIODIR = (0xFFFFFFFF);//configuro el puerto como salida

		return;
}

	void set_gpio(void){

		LPC_GPIO0->FIOSET = (0xFFFFFFFF); //seteo todos los pines del puerto
		retardo();
		LPC_GPIO0->FIOCLR = (0XFFFFFFFF); //pongo en bajo los pines del puerto
		retardo();
		return;

}
	void get_gpio(void){

		if(LPC_GPIO0->FIOPIN == (0XFFFFFFFF)){ //leo si los pines estan en 1 y no hago nada

		}
			return;
	}

	void retardo(void){

		uint32_t contador;//declaro variable
		for(contador=0; contador< 6000000 ; contador++){

		}
		return;
	}
