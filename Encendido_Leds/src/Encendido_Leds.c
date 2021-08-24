/*
===============================================================================
 Name        : Encendido_Leds.c
 Author      : $(author)
 Version     :
 Copyright   : $(copyright)
 Description : main definition
===============================================================================
*/

/*
===============================================================================
 Name        : Encendio_3led.c
 Author      : $(author)
 Version     :
 Copyright   : $(copyright)
 Description : main definition
===============================================================================
*/

/*
===============================================================================
 Name        : LAD_SAUL.c
 Author      : $(author)
 Version     :
 Copyright   : $(copyright)
 Description : main definition
===============================================================================
*/



/*
===============================================================================
 Name        : CONCEPCION ALVARADO eRIK
 Author      : $(author)
 Version     :
 Copyright   : $(copyright)
 Description : main definition
===============================================================================
*/


/*

===============================================================================
 Name        : ejercicio1_led_2019.c
 Author      :
 Version     :
 Copyright   :
 Description : Electrónica digital 3 - miercoles 14 de Agosto de 2019
===============================================================================


*/

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#include <stdint.h>
#endif

#include <cr_section_macros.h>



volatile int32_t TimeDelay;
volatile int32_t No_Ok;

void SysTick_Handler(void);
void sysTick_Initialize(uint32_t ticks);

int main(void) {

	uint32_t verClk;
    verClk = SystemCoreClock;
    No_Ok =0;

    sysTick_Initialize(100000);//inicializo en 1 ms

    LPC_GPIO0->FIODIR |=(1<<22);//led rojo
    LPC_GPIO3->FIODIR |=(1<<25);//led verde
    LPC_GPIO3->FIODIR |=(1<<26);//led azul

    LPC_GPIO2->FIODIR &=~(1<<10);// P2[10]pulsador del lpc




    LPC_GPIO0->FIOSET |= (1<<22);// Apago todos los leds
    LPC_GPIO3->FIOSET |=(1<<25);
    LPC_GPIO3->FIOSET |=(1<<26);


    Delay(1000);//antes de comezar la secuencia espero un segundo
    Delay(1000);//enciendo en secuencia los leds, cada uno se prende un segundo
    Delay(1000);
    Delay(1000);


    LPC_GPIO0->FIOSET |= (1<<22);// Apago todos los leds
    LPC_GPIO3->FIOSET |=(1<<25);
    LPC_GPIO3->FIOSET |=(1<<26);
	//SysTick->CTRL &=~ 1; // SysTick_CTRL_ENABLE
    int pulsador = 0;// esta es la variable para el pulsador
	int control_ok = 0; // desde que enciende hasta que se presiona el pulsador hay un timpo maáximo
						// esta variable controla que no se haya excedido ese tiempo
	//LPC_GPIO3->FIOSET |=(1<<26);
	while(1){

			//LPC_GPIO0->FIOSET |= (1<<22);

			while((LPC_GPIO2->FIOPIN)&(1<<10)){  //se mantendrá en el bucle hasta que suelte el pulsador
				//LPC_GPIO0->FIOCLR |= (1<<22);
				pulsador = 1;
			}
			//else

			//LPC_GPIO0->FIOSET |= (1<<22);

			if (pulsador == 1){

				if (control_ok == 0){
				    LPC_GPIO3->FIOSET |=(1<<26);    // si presiono el pulsador la primera vez ingresará a este condicional
					LPC_GPIO0->FIOCLR |= (1<<22);	// encenderá el led correspondiente
					pulsador = 0;
					control_ok= control_ok+1;
					TimeDelay = 15000;
					No_Ok = 1;


				}else if(control_ok == 1){			// si presiono el pulsador la segunda vez ingresará a este condicional
					LPC_GPIO0->FIOSET |= (1<<22);	// encenderá el led correspondiente
					LPC_GPIO3->FIOCLR |=(1<<25);
					pulsador = 0;
					control_ok= control_ok+1;
					TimeDelay = 15000;
				}else if(control_ok == 2){			// si presiono el pulsador la tercera vez ingresará a este condicional
					LPC_GPIO0->FIOSET |= (1<<22);	// encenderá el led correspondiente
					LPC_GPIO3->FIOSET |=(1<<25);
					LPC_GPIO3->FIOCLR |=(1<<26);
					pulsador = 0;
					control_ok= control_ok+1;
					TimeDelay = 15000;
				}else if(control_ok == 3){			// si presiono el pulsador la cuarta vez ingresará a este condicional
					//SysTick->CTRL &=~ 1; // SysTick_CTRL_ENABLE	// parpadean los led simultáneamente una cantidad i veces
					No_Ok = 0;
					for(int i =0;i<3;i++){
						LPC_GPIO0->FIOCLR |= (1<<22);
						LPC_GPIO3->FIOCLR |=(1<<25);
						LPC_GPIO3->FIOCLR |=(1<<26);

						Delay(500);

						LPC_GPIO0->FIOSET |= (1<<22);
						LPC_GPIO3->FIOSET |=(1<<25);
						LPC_GPIO3->FIOSET |=(1<<26);

						Delay(500);
					}
					pulsador = 0;
					control_ok= control_ok+1;
					SysTick->CTRL &=~ 1; // SysTick_CTRL_ENABLE
					LPC_GPIO3->FIOSET |=(1<<26);
				}

			}

	}


    return 0 ;
}




void SysTick_Handler(void){
	static int control = 0;


	if (TimeDelay >0)
		TimeDelay--;

	if (TimeDelay == 0){

		if (control == 0){					//prendo los leds en secuencia de acuerdo al tiempo especificado
			LPC_GPIO0->FIOCLR |= (1<<22);
			LPC_GPIO3->FIOSET |=(1<<25);
			LPC_GPIO3->FIOSET |=(1<<26);
			control = control + 1;
		}else if(control == 1){
			LPC_GPIO0->FIOSET |= (1<<22);
			LPC_GPIO3->FIOCLR |=(1<<25);
			LPC_GPIO3->FIOSET |=(1<<26);
			control = control + 1;
		}else if(control == 2){
			LPC_GPIO0->FIOSET |=(1<<22);
			LPC_GPIO3->FIOSET |=(1<<25);
			LPC_GPIO3->FIOCLR |=(1<<26);
			control = control + 1;
		}else {
			LPC_GPIO0->FIOSET |= (1<<22);
			LPC_GPIO3->FIOSET |=(1<<25);
			LPC_GPIO3->FIOSET |=(1<<26);
		}

	}

	if (TimeDelay == 0 ){
		if (No_Ok == 1){						//	si el tiempo de espera especificado se excede el led rojo
			LPC_GPIO0->FIOSET |= (1<<22);	    //	parpadea una serie de veces
			LPC_GPIO3->FIOSET |=(1<<25);
			LPC_GPIO3->FIOSET |=(1<<26);

			for(int i = 0;i<5;i++ ){
				LPC_GPIO0->FIOCLR |= (1<<22);

				for(int i = 0;i<700000;i++ ){
					//
				}

				LPC_GPIO0->FIOSET |= (1<<22);

				for(int i = 0;i<700000;i++ ){
					//
				}

			}
			No_Ok = 0;


		}

	}


}


void Delay(uint32_t nTime){
	TimeDelay = nTime;
	while(TimeDelay != 0);

}


void sysTick_Initialize(uint32_t ticks){		//inicializo los registro del SysTick

	SysTick->CTRL = 0;//ENABLE EL CONTADOR DEL SYSTICK LIMPIA AL PONER CERO A TODOS EL REGISTRO
	SysTick->LOAD = ticks - 1;//TIEMPO DE INTERRUPCION = LOAD *(PERIODO DEL RELOJ USADO)

	NVIC_SetPriority(SysTick_IRQn, (1<<__NVIC_PRIO_BITS)-1);

	SysTick->VAL = 0;

	SysTick->CTRL |= (1<<2); // SysTick_CTRL_CLKSOURCE

	SysTick->CTRL |= (1<<1); // SysTick_CTRL_TICKINT HABILITA LAS INTERRUPCIONES POR SYSTICK

	SysTick->CTRL |= 1; 	 // SysTick_CTRL_ENABLE

}
