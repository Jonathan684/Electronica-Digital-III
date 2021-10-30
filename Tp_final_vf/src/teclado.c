/*
===============================================================================

===============================================================================
 */


#include "librerias.h"
//#include "LPC17xx.h"
//#include "LPC17XX_pinsel.h"
//#include "LPC17XX_gpio.h"


/*
# define PUERTO_0 (uint8_t)0
# define PUERTO_1 (uint8_t)1
# define PUERTO_2 (uint8_t)2
# define PUERTO_3 (uint8_t)3

# define SALIDA   (uint8_t)1
# define ENTRADA  (uint8_t)0

# define PIN_22   ((uint32_t)(1<<22))

# define PIN_0   ((uint32_t)(1<<0))
# define PIN_4   ((uint32_t)(1<<4))
*/
uint8_t a = 0;
uint32_t pinesFilas[] = {0,1,2,3};
uint32_t pinesColumnas[] = {(1<<6),(1<<5),(1<<4)};
char teclas[4][3] = {{'1','2','3'},
                     {'4','5','6'},
                     {'7','8','9'},
                     {'*','0','#'}};

char contraseña_activar[] = {'1','1','1'} ;
char contraseña_desactivar[] = {'4','5','6'} ;
char contraseña_aux[3];



uint32_t completa = 0;
uint8_t  Incrementar = 0;
uint8_t match = 0;
void confGPIO(void);
void confIntGPIO(void);
void antirebote(void);

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
 * Manejo de la rutina de interrupcion]
 *
 * Interrupciones de GPIO
 * P[0]0
 */
void EINT3_IRQHandler(void){
	// COLUMNA 0
	antirebote();
	if((LPC_GPIOINT->IO2IntStatF) & (1<<4)){ //Pregunto si se interrumpio por el flanco descendente en el pin 4.
		for (int nL = 0; nL < 4; nL++) // For para buscar la fila
		{
			LPC_GPIO2->FIOSET |= (1<<nL);
			//Barrido en columnas buscando un LOW
			if (((LPC_GPIO2->FIOPIN) & (1<<4))) //Deberia entrar cada vez que esta en 1
			{
				//printf("Tecla: %c\n",teclas[nL][0]);
				lcd_clear();
				LCD_PUTC(teclas[nL][0]);
                contraseña_aux[Incrementar] = teclas[nL][0];
				Incrementar ++;
				antirebote();
				break;
			}
		}
   }
	// COLUMNA 1
   else if((LPC_GPIOINT->IO2IntStatF) & (1<<5)){ //Pregunto si se interrumpio por el flanco descendente en el pin 4.
         for (int nL = 0; nL < 4; nL++)
                    {
             	    LPC_GPIO2->FIOSET |= (1<<nL);
             	    //Barrido en columnas buscando un LOW
                      if (((LPC_GPIO2->FIOPIN) & (1<<5))) //Deberia entrar cada vez que esta en 1
                        {
                    	  //printf("Tecla: %c\n",teclas[nL][1]);
                    	  lcd_clear();
                    	  LCD_PUTC(teclas[nL][1]);
                    	  contraseña_aux[Incrementar] = teclas[nL][1];
                    	  Incrementar ++;
                    	  antirebote();
                     	break;

               }
           }
    }
	// COLUMNA 2
   else if((LPC_GPIOINT->IO2IntStatF) & (1<<6)){ //Pregunto si se interrumpio por el flanco descendente en el pin 4.
        for (int nL = 0; nL < 4; nL++)
                 {
                       	    LPC_GPIO2->FIOSET |= (1<<nL);
                       	    //Barrido en columnas buscando un LOW
                                if (((LPC_GPIO2->FIOPIN) & (1<<6))) //Deberia entrar cada vez que esta en 1
                                  {
                                	//printf("Tecla: %c\n",teclas[nL][2]);
                                	lcd_clear();
                                	LCD_PUTC(teclas[nL][2]);
                                	contraseña_aux[Incrementar] = teclas[nL][2];
                                	Incrementar ++;
                                	antirebote();
                               	break;
                        		 }
                     }
   }
	//printf("Verificacion Incrementar: %d\n",Incrementar);
	if(Incrementar == 3){
		
		if(a == 0){
			
			for (int i=0 ; i <Incrementar ; i++){

			   if (contraseña_activar[i] == contraseña_aux[i]) match ++;
			   else{
				   //printf("Incorrecto \n");
				   break;
			   }
			}
			if (match == 3){
				//printf("Correcto \n");
				Activar_alarma();
				EXTI_ClearEXTIFlag(EXTI_EINT1);
				NVIC_EnableIRQ(EINT1_IRQn);
				//lcd.AlarmaActivada
				a = 1;
			}
			
		}
		else if(a == 1){
			for (int i=0 ; i <Incrementar ; i++){

			   if (contraseña_desactivar[i] == contraseña_aux[i]) match ++;
			   else{
				   //lcd.AlarmaDesactivada
				   //printf("Incorrecto \n");
				   break;
			   }
			}
			if (match == 3){
				//printf("Correcto \n");
				Desactivar_alarma();
				
				NVIC_DisableIRQ(EINT1_IRQn);
				EXTI_ClearEXTIFlag(EXTI_EINT1);
				LPC_TIM1->EMR |= (1<<4); 
				LPC_TIM1->EMR &=  ~(1<<5);
                //lcd.AlarmaActivada
			    a = 0;
			}
			
		}
		Incrementar = 0;
		match = 0;
	}

   LPC_GPIOINT->IO2IntClr |= (1<<4);   //Bajo la bandera
   LPC_GPIOINT->IO2IntClr |= (1<<5);
   LPC_GPIOINT->IO2IntClr |= (1<<6);
   LPC_GPIO2->FIOCLR   |= ((1<<0)|(1<<1)|(1<<2)|(1<<3));

   }
void antirebote(void){
	for (int i;i<24000000;i++){}
}
