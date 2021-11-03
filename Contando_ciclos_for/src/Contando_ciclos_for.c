/*
===============================================================================
 Name        : Contando_ciclos_for.c
 Author      : $(author)
 Version     :
 Copyright   : $(copyright)
 Description : main definition
===============================================================================
*/

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif
/*
 * Tiempo en milisegundos
 */
void dealy_ms(int tiempo);
int main(void) {
	int tiempo;
	tiempo = 3000000; // 500 ms
	dealy_ms(tiempo);
    while(1) {

    }
    return 0 ;
}
void dealy_ms(int tiempo){

	for(int i=0;i<tiempo;i++){}

}
