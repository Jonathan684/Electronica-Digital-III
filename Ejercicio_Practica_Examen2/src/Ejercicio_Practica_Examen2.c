
/*
===============================================================================
 Name        : ej_integracion_2.c
 Author      : Juan Manuel Gil
 Version     :
 Copyright   : $(copyright)

 Description : Realizar un código en C que permita filtrar las muestras provenientes del ADC con un
 filtro promediador móvil de N muestras, donde  N inicialmente es igual a 2 y puede ser incrementado en
 dos unidades cada vez que se presiona un pulsador conectado en P0.6 hasta llegar a un valor de 600.

 A su vez, utilizando el bit de overrun y un timer, complete el código realizado para que en caso de
 producirse el evento de pérdida de datos por parte del ADC, se deje de muestrear y se saque por el
 pin MATCH2.1 una señal cuadrada de 440 Hz. Considerar una frecuencia de cclk de 60 Mhz y configurar
 el ADC para obtener una frecuencia de muestreo de 5 Kmuestras/seg.


===============================================================================
*/

#include "LPC17xx.h"

uint16_t N = 2;
float prom_muestras = 0;

void confGPIO();
void confIntGPIO();
void confTimer1();
void confTimer2();
void confADC();
void delay(uint32_t);

int main(void) {
	confGPIO();
	confIntGPIO();
	confTimer1();
	confADC();
    while(1) {
    	if (LPC_ADC->ADDR0 & (0x1<<30)) {
    		//se produjo un overrun
    		LPC_ADC->ADCR &= ~(0x1<<21); //desactivo el ADC
    		LPC_SC->PCONP &= ~(0X1<<12); //le saco el power al ADC
    		confTimer2(); //llamo a la conf del Timer2 para que me genere la onda cuadrada
    	}
    }
    return 0 ;
}

void confGPIO() {
	//por defecto son gpio y pull up
	LPC_GPIO0->FIODIR0 &= ~(0x1<<6);//entrada pin P0.6
	LPC_GPIO0->FIOMASK0 = 0xFF;
	LPC_GPIO0->FIOMASK0 &= ~(0x1<<6);
	return;
}

void confIntGPIO() {
	//Usando DRIVERS
	//GPIO_IntCmd(uint8_t portNum, uint32_t bitValue, uint8_t edgeState)
	//GPIO_ClearInt(uint8_t portNum, uint32_t bitValue)
	LPC_GPIOINT -> IO0IntEnF |= (0x1<<6); //seteo para que interrumpa por flanco de bajada
	LPC_GPIOINT -> IO0IntClr |= (0x1<<6); //limpio la flag

	NVIC_EnableIRQ(EINT3_IRQn); //habilito las interrupciones externas
}

void confTimer1() {
	//este timer se usa para realizar las conversiones
	LPC_SC->PCLKSEL0 |= (0x1<<24); //pclk = cclk
	LPC_SC->PCONP |= (0x1<<2);//habilito el power timer 1
	LPC_PINCON->PINSEL3 |= (0x3<<12); //pongo el mat1.0
	LPC_TIM1->EMR |= (0x3<<4);//realiza el toggle
	LPC_TIM1->PR = 0;
	LPC_TIM1->MR0 = 6E3;//cargo el registro para el match0
	LPC_TIM1->TCR = 0x3; //habilito la cuenta.
	LPC_TIM1->TCR &= ~(0x1<<1); //resetro el timer.
	return;
}

void confTimer2() {
	//este timer se utiliza para realizar la onda cuadrada de 440Hz en caso de un overrun.
	LPC_SC->PCLKSEL1 |= (0x1<<12);//pclk = cclk
	LPC_SC->PCONP |= (0x1<<22);//Habilito el power del Timer2
	LPC_PINCON->PINSEL0 |= (0x3<<14); //selecciono el match 1 del timer 2
	LPC_TIM2->EMR |= (0x3<<6);//pongo el toggle
	LPC_TIM2->PR = 0;
	LPC_TIM2->MR1 = 136199;
	LPC_TIM2->TCR = 0x3; //habilito la cuenta.
	LPC_TIM2->TCR &= ~(0x1<<1); //resetro el timer.
	return;
}

void confADC() {
	LPC_SC->PCLKSEL0 &= ~(0x3<<24);//pclk = cclk/4
	LPC_SC->PCONP |= (0x1<<12); //habilito el power del adc
	LPC_ADC -> ADCR |= (1<<21); //Pag. 588 Habilita el adc
	LPC_PINCON->PINSEL1 |= (0x1<<14); //ad canal 0
	LPC_PINCON->PINMODE1 |= (0x1<<15);//o pull down o up
	LPC_ADC->ADCR |= 0x1; //canal 0
	LPC_ADC->ADCR &= ~(0xFF<<8);//divisor interno por 1.
	LPC_ADC->ADCR &= ~(0x1<<16); //no burst
	LPC_ADC->ADCR |= (0x1<<27); //la conversión comienza con un flanco de bajada
	LPC_ADC->ADCR |= (0x3<<25); //convierte cuando se de el flanco por el timer 1.0
	return;
}

void EINT3_IRQHandler() {
	delay(10E6);
	if (LPC_GPIOINT->IntStatus & 0x1) {
		//generó la int el port 0?
		if (LPC_GPIOINT->IO0IntStatF & (0x1<<6)) {
			//fue el pin 6?
			uint32_t adc_value = (LPC_ADC->ADDR0>>4) & 0xFFF;
			N = (N == 600) ? 2 : N + 2;
			prom_muestras = adc_value / N;
		}
		LPC_GPIOINT -> IO0IntClr |= (0x1<<6); //Limpio la flag
	}
	return;
}

void delay(uint32_t count) {
	for(int cont=0; cont<count;cont++){};
}
