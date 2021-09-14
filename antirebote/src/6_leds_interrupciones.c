/*
===============================================================================
 Name        : 6_leds_interrupciones.c
 Author      : $(author)
 Version     :
 Copyright   : $(copyright)
 Description : main definition
===============================================================================
*/

//
//leds_Erik


//JOE 6 LED QUE SE ENCIENDEN UNO A UNO
/*
 Name        : clase4_ejercicio8.c
 Author      : $jwayar
 Version     :
 Copyright   : $FCEFyN-UNC
 Description : 6Leds+Pulsador c/ y s/ rebote
===============================================================================
*/




/* ----------------------------------------------
 * @Output:	P0[8][7][6][0][1][18] : En ése orden!
 * 			-------------------------------------
 * ----------------------------------------------
 * @Input:	P0[9]  (Pulsador)
 * 			-------------------------------------
 */

/*
===============================================================================
 Name        : clase4_ejercicio8.h
 Author      : $jwayar
 Version     :
 Copyright   : $FCEFyN-UNC
 Description : 6Leds+Pulsador c/ y s/ rebote
===============================================================================
*/

/* ----------------------------------------------
 * @Output:	P0[8][7][6][0][1][18] : En ése orden!
 * 			-------------------------------------
 * ----------------------------------------------
 * @Input:	P0[9]  (Pulsador)
 * 			-------------------------------------
 */
#define posLED0 8
#define posLED1 7
#define posLED2 6
#define posLED3 0
#define posLED4 1
#define posLED5 18
#define posPULS 9
/*-------------------------------------------
    Posiciones de memoria para config GPIO0
 *------------------------------------------*/
#define AddrFIO0DIR  0X2009C000
#define AddrFIO0SET  0X2009C018
#define AddrFIO0CLR  0X2009C01C
#define AddrFIO0PIN  0X2009C014
#define AddrFIO0MASK 0X2009C010
#define AddrPINMODE0 0X2009C040 // Config PinMode (to Pull-UP P0[9]) del GPIO0, pines del 0 al 15
/* Definir Puertos a las direcciones
 * de memoria por las correspondientes constantes:
 */
unsigned int volatile *const FIO0DIR  = (unsigned int*) AddrFIO0DIR;
unsigned int volatile *const FIO0SET  = (unsigned int*) AddrFIO0SET;
unsigned int volatile *const FIO0CLR  = (unsigned int*) AddrFIO0CLR;
unsigned int volatile *const FIO0PIN  = (unsigned int*) AddrFIO0PIN;
unsigned int volatile *const FIO0MASK = (unsigned int*) AddrFIO0MASK;
unsigned int volatile *const PINMODE0 = (unsigned int*) AddrPINMODE0;

// Prototipo de Funciones:
void configuracion(void ); 	//-- Config Port
void init(void );			//-- Init Output
int retardo(unsigned int ); //-- Function delay
int debounce(int );         //-- Function Debounce

//#include "Ej8Wayar.h"

//-- Config Port
void configuracion(void){
	//-------------------------------------
	//--PINSEL (Select GPIO0 or other)
		/* by default 00 to all -> "GPIO0"
		 * PINSEL0 para P0[15:0]
		 * PINSEL1 para P0[31:16]
		 */
	//-------------------------------------
	//--FIO0DIR (Select Input/Output)
		/* by default All Input
		 */
	// Output (Set 1):
	*FIO0DIR |= (
					(1<<posLED0) |
					(1<<posLED1) |
					(1<<posLED2) |
					(1<<posLED3) |
					(1<<posLED4) |
					(1<<posLED5)
				);
	// Input (Set 0):
	*FIO0DIR &= ~(1<<posPULS);

	//-------------------------------------
	//-- FIOMASK (enmascared to pins unused)
		/* Set 1: enmascarar pines no usados!
		 * Set 0: a los pines utilizados (In/Out)
		 */
	*FIO0MASK = ( 0xFFFF
						& ~(1<<posLED0) & ~(1<<posLED1)
						& ~(1<<posLED2) & ~(1<<posLED3)
						& ~(1<<posLED4) & ~(1<<posLED5)
						& ~(1<<posPULS)
				);
	//-------------------------------------
	//-- PINMODE
		/* PullUp (to INPUT) Enable by default
		 * only one Input: PULSADOR P0[9]->PINMODE0
		 */
	*PINMODE0 &= ~(0x03<<18); // Set "00" to enable Pul-Up in P0[9]
}

//-- Init Output
void init(void){
	*FIO0SET|= (
						(1<<posLED0) |
						(1<<posLED1) |
						(1<<posLED2) |
						(1<<posLED3) |
						(1<<posLED4) |
						(1<<posLED5)
					);
}

//-- Function delay
int retardo(unsigned int time){
	unsigned int i;
	for (i=0; i<time; i++); // lazo de demora
	return 0;
}

//------------------------------------
//-- Function Debounce
//-------------------------------------
/* debounce "Eliminador de rebote"
 * @Input: SampleA (la primer muestra de la señal, estado del pin leído por primera vez)
 * @Output LastDebounceResult (Resultado, interpretación del pin leído)
 */
int debounce(int SampleA){
	//-- Set static variables:
	static int SampleB=0;
	static int SampleC=0;
	static int LastDebounceResult =0;

	//-- Logical Function:
	LastDebounceResult = (LastDebounceResult &&
						(SampleA || SampleB || SampleC)) ||
						(SampleA && SampleB && SampleC);
	//-- Update Sample
	SampleC=SampleB;
	SampleB=SampleA;

	return LastDebounceResult;
}
//-- main
int main(void){
	// Configuración:
	configuracion();

	//--- Parameters:
	unsigned time_antiRebote= 2000; // tiempo para inhibir el rebote (fijado a prueba y error)
	unsigned int vector_leds[6]={
									posLED0,
									posLED1,
									posLED2,
									posLED3,
									posLED4,
									posLED5
								};
	unsigned int pos=0; // posicion del vector de leds
	unsigned int pulsador_status;
	unsigned int j;

	//-- Acción principal:
	init();
	retardo(800000); // aprox 2 segundos (900mil aprox 2.5 seg)
	while (1)
	{
		//-- prender sólo el LED correspondiente
		*FIO0PIN = (1<<vector_leds[pos]);

		//--Pregunto!
		pulsador_status = !(*FIO0PIN & (1<<posPULS));//EL pulsador status es un control que me indica
		//FIOPIN en este caso lo utilizo como lectura porque no le estoy dando ningun valor
		// que la entrada posPuls esta siendo presionada y me da valores de 1==v   0===f
		//si es falso no entrara al ciclo o bucle if() si es verdadero ejecutara la instruccion
		if( pulsador_status ) // se pulsó?
		{
			//Antirebote
			for( j=0; j<3; j++)
			{
				retardo(time_antiRebote); //si!, espero a que deje de rebotar y vuelvo a preguntar
				pulsador_status = !(*FIO0PIN & (1<<posPULS));
				pulsador_status=debounce(pulsador_status);
			}

			if( pulsador_status ) // si! se pulsó!vuelve a replantear la condicion de verdadero
			{
				// desplazar posicion del LED
				pos++;

				//prender LED correspondiente
				*FIO0PIN = (1<<vector_leds[pos]);//FIOPIN SIRVE PARA LECTURA Y ESCRITURA IGUAL QUE FIODIR;
         //aqui FIOPIN SE USA COMO ESCRITURA WRITTEN
				// mientras se mantiene pulsado, (se queda en espera)
				while(!(*FIO0PIN & (1<<posPULS)));
				//AQUI MIENTRAS ESTE PULSADO ESTA EN EL CICLO WHILE DE MODO INFINITO ES COMO SI SE PUSIERA
				//    while(!(*FIO0PIN & (1<<posPULS)))
				//        {

			//	          }
				//cuando dejo de pulsar sale del ciclo infinito

				// se dejó de pulsar (sale del while)
				//pos++;
				pos = pos%6; // "si sobrepasa de 6 vuelve a empezar de 0"
			}
		}
	}
}
