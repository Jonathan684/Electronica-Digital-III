/*
===============================================================================
 Name        : Teclado.c
 Author      : $Jonathan Patiño
 Version     :
 Copyright   : $(copyright)
 Description : main definition
===============================================================================
*/

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#include "LPC17XX_pinsel.h"
#include "LPC17XX_gpio.h"
#endif

#include <cr_section_macros.h>
#include <stdio.h>

# define PUERTO_0 (uint8_t)0
# define PUERTO_1 (uint8_t)1
# define PUERTO_2 (uint8_t)2
# define PUERTO_3 (uint8_t)3

# define SALIDA   (uint8_t)1
# define ENTRADA  (uint8_t)0

# define PIN_22   ((uint32_t)(1<<22))

# define PIN_0   ((uint32_t)(1<<0))
# define PIN_4   ((uint32_t)(1<<4))

uint32_t pinesFilas[] = {0,1,2,3};
uint32_t pinesColumnas[] = {(1<<6),(1<<5),(1<<4)};
char teclas[4][3] = {{'1','2','3'},
                     {'4','5','6'},
                     {'7','8','9'},
                     {'*','0','#'}};

char contraseña[] = {'1','2','3'} ;
uint32_t completa = 0;
uint8_t  Incrementar = 0;

void confGPIO(void);
void confIntGPIO(void);
void antirebote(void);
int main(void) {

    printf("Teclado Matricial \n");
    confGPIO();
    confIntGPIO();
    //GPIO_SetValue(PUERTO_2, PIN_0);

    while(1) {

    	if(completa == 3){
    		printf("Acceso correcto\n");
    		completa = 0;
    		Incrementar = 0;
          }

    }
    return 0 ;
}
void confGPIO(void){

    LPC_GPIO2->FIODIR    |= ((1<<0)|(1<<1)|(1<<2)|(1<<3)); //Pines 0,1,2,3 como salidas 4 Filas
    LPC_GPIO2->FIOCLR   |= ((1<<0)|(1<<1)|(1<<2)|(1<<3));  //Pongo un cero en la salida
    LPC_GPIO2->FIODIR    &= ~((1<<4)|(1<<5)|(1<<6));          //Pines 4,5,6,7 como entradas    3 Columnas
	LPC_PINCON->PINMODE4 &= ~((1<<4)|(1<<5)|(1<<6)); 	   //HABILITO LAS PULLUP para las columnas
}
/*
 * Configuracion de las interrupciones por GPIO.
 */
void confIntGPIO(void){
   LPC_GPIOINT -> IO2IntEnF |= ((1 << 4)|(1 << 5)|(1 << 6)); //Selecciono la interrupcion por flanco de bajada
   LPC_GPIOINT -> IO2IntClr |= ((1 << 4)|(1 << 5)|(1 << 6)); //Limpia la bandera
   NVIC_SetPriority(EINT3_IRQn, 0); 	 //Prioridad para esta interrupcion
   NVIC_EnableIRQ(EINT3_IRQn); 			 //Habilita las interrupciones por GPIO
}
/*
 * Manejo de la rutina de interrupcion
 * Interrupciones de GPIO
 * P[0]0
 */
void EINT3_IRQHandler(void){
	Incrementar ++ ;
	if((LPC_GPIOINT->IO2IntStatF) & (1<<4)){ //Pregunto si se interrumpio por el flanco descendente en el pin 4.
	  for (int nL = 0; nL <= 3; nL++)
           {
    	    LPC_GPIO2->FIOSET |= (1<<nL);
    	    //Barrido en columnas buscando un LOW
             if (((LPC_GPIO2->FIOPIN) & (1<<4))) //Deberia entrar cada vez que esta en 1
               {
            	 printf("Tecla: %c\n",teclas[nL][0]);
            	 if(contraseña[nL] == teclas[nL][0]){
            		 printf("Coincidencia \n");
            		 if(Incrementar == 1)
            		 {
            			 completa = 1;
            		 }
            		 else Incrementar = 0;
            	 }
            	 antirebote();
            	 break;
               }
             }
   }
   else if((LPC_GPIOINT->IO2IntStatF) & (1<<5)){ //Pregunto si se interrumpio por el flanco descendente en el pin 4.
         for (int nL = 0; nL <= 3; nL++)
                    {
             	    LPC_GPIO2->FIOSET |= (1<<nL);
             	    //Barrido en columnas buscando un LOW
                      if (((LPC_GPIO2->FIOPIN) & (1<<5))) //Deberia entrar cada vez que esta en 1
                        {
                     	 printf("Tecla: %c\n",teclas[nL][1]);
                     	if(contraseña[nL] == teclas[nL][0]){
                     	      printf("Coincidencia \n");
                     	      if(completa == 1 && Incrementar == 2){
                     	      	completa = 2;
                     	       antirebote();

                     	      }
                     	     else Incrementar = 0;
                     	 }
                     	break;

               }
           }
    }
   else if((LPC_GPIOINT->IO2IntStatF) & (1<<6)){ //Pregunto si se interrumpio por el flanco descendente en el pin 4.
        for (int nL = 0; nL <= 3; nL++)
                 {
                       	    LPC_GPIO2->FIOSET |= (1<<nL);
                       	    //Barrido en columnas buscando un LOW
                                if (((LPC_GPIO2->FIOPIN) & (1<<6))) //Deberia entrar cada vez que esta en 1
                                  {
                               	 printf("Tecla: %c\n",teclas[nL][2]);
                               	if(contraseña[nL] == teclas[nL][0]){
                               	    printf("Coincidencia \n");
                               	      if(completa == 2 && Incrementar == 3){
                               		      	completa = 3;
                               	         }
                               	 }
                               	 antirebote();
                          	 break;
                         }
                     }
   }
   LPC_GPIOINT->IO2IntClr |= (1<<4);   //Bajo la bandera
   LPC_GPIOINT->IO2IntClr |= (1<<5);
   LPC_GPIOINT->IO2IntClr |= (1<<6);
   LPC_GPIO2->FIOCLR   |= ((1<<0)|(1<<1)|(1<<2)|(1<<3));

}
void antirebote(void){
	for (int i;i<1000000;i++){}
}
/*
 * Configuracion de Filas
 */
//Salida P2[0] Filas
/*uint8_t i;
int aux = pinesFilas[0];
//for(i=0 ;i<4;i++){
	    conf_pin->Portnum = PINSEL_PORT_2;  // Puerto 2
		conf_pin->Pinnum  = PINSEL_PIN_0;   // Funcion pin 0
		conf_pin->Funcnum = PINSEL_FUNC_0;  // Funcion GPIO
		conf_pin->Pinmode = PINSEL_PINMODE_PULLUP; //Modo pull-up por defecto
		conf_pin->OpenDrain = PINSEL_PINMODE_NORMAL; // Sin open drain
		PINSEL_ConfigPin(conf_pin);
		GPIO_SetDir(PUERTO_2, pinesFilas[i] , SALIDA);
		GPIO_ClearValue(PUERTO_2, pinesFilas[i] ); // Todas las filas le ponemos un 0
//}
*/

//uint32_t pinesColumnas[] = {6,5,4};
//Entrada P2[4] Columnas
/*	for(i=0;i<3;i++){
	conf_pin->Portnum = PINSEL_PORT_2;	// Puerto 2
	conf_pin->Pinnum  = ((uint32_t)(1<<pinesColumnas[i]));	// Funcion pin 4
	conf_pin->Funcnum = PINSEL_FUNC_0;  // Funcion GPIO
	conf_pin->Pinmode = PINSEL_PINMODE_PULLUP; //Modo pull-up
	conf_pin->OpenDrain = PINSEL_PINMODE_NORMAL; // Sin open drain
	PINSEL_ConfigPin(conf_pin);
	GPIO_SetDir(PUERTO_2, pinesColumnas[i] , ENTRADA);
*/
//}

/*PINSEL_CFG_Type *conf_pin;

	conf_pin->Portnum = PINSEL_PORT_0;
	conf_pin->Pinnum  = PINSEL_PIN_22;
	conf_pin->Funcnum = PINSEL_FUNC_0;
	conf_pin->Pinmode = PINSEL_PINMODE_PULLUP;
	conf_pin->OpenDrain = PINSEL_PINMODE_NORMAL;

	PINSEL_ConfigPin(conf_pin);
	GPIO_SetDir(PUERTO_0, PIN_22 , SALIDA);

	conf_pin->Portnum = PINSEL_PORT_2;
	conf_pin->Pinnum  = PINSEL_PIN_0;
	conf_pin->Funcnum = PINSEL_FUNC_0;
	conf_pin->Pinmode = PINSEL_PINMODE_PULLUP;
	conf_pin->OpenDrain = PINSEL_PINMODE_NORMAL;

	PINSEL_ConfigPin(conf_pin);
	GPIO_SetDir(PUERTO_2, PIN_0 , SALIDA);*/
/*void Delay(){
	int i=0;
	for(i=0;i<100;i++){}
}*/
/*GPIO_SetValue(PUERTO_2, pines_Filas); // Se colocan todos los bit de filas en 1
	   for(){
		   GPIO_ClearValue(PUERTO_2, pines_Filas); // Luego voy poniendo uno por uno en 0 hasta encontarla la fila y columna que necesito.
	   }


	   uint32_t aux = (LPC_GPIO2->FIOPIN) & (1<<4);
	       	if(((LPC_GPIO2->FIOPIN) & (1<<4)) == 0) // Pregunto si en el cuarto bit entro un cero
	       	{
	       		printf("Presionada \n");
	       	}
	       	GPIO_SetValue(PUERTO_2, PIN_0);
	       	/*GPIO_ClearValue(PUERTO_0, PIN_22);
	       	Delay();
	       	GPIO_SetValue(PUERTO_0, PIN_22);
	       	Delay();*/

	       	//Delay();*/
