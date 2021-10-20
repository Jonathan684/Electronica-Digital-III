

/*
===============================================================================
 Name        : TECLADO_IAN_TP_FINAL.c
 Author      : $(author)
 Version     :
 Copyright   : $(copyright)
 Description : main definition
===============================================================================
*/

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#include "../Drivers/inc/lpc17xx_adc.h"
#include "../Drivers/inc/lpc17xx_pinsel.h"
#include "../Drivers/inc/lpc17xx_gpio.h"
#include "core_cm3.h"
#include "LPC17xx_timer.h"
#include "LPC17xx_uart.h"
#endif

#include <cr_section_macros.h>



void PinConfig();
uint8_t ScanearTeclado();
void Set_UART3(void);
void enviar(void);


void retardo();
uint32_t scan_data;
uint8_t  dato_uart;


static const uint8_t  keyboard[4][4]= {{'1','2','3','A'},//uint8_t para poder enviar los el UART
				                   {'4','5','6','B'},
								   {'7','8','9','C'},
								   {'*','0','#','D'}};


int main(void) {

	PinConfig();
	ScanearTeclado();
	 retardo();
	  Set_UART3();
	  enviar();



    while(1) {

    }
    return 0 ;
}


void PinConfig(){

	LPC_GPIO2->FIOPIN = 0X0F; //PINN DEL PUERTO 2  0,1,2,3 COMO SALIDAS Y PIN 4,5,6,7 COMO ENTRADAS
	LPC_GPIO2->FIOMASK = 0XFFFFFFFF&~(0XFF);//Se enmascara los pines que no van a usar


	//LPC_PINCON->PINSEL ;
	//LPC_PINCON ->PINMODE ; //VER QUE POR DEFECTO ESTA EN GPIO Y LAS ENTRADA EN PULLUP ASI QUE NO ES NECESARIO AGREGARLES




	return;


}


uint8_t ScanearTeclado(void){
	uint32_t fila;
	uint32_t columna;



	for(fila=0;fila<4;fila++){//recorre las cuatro filas que son las salidad enviando "0"

		switch(fila){
		case(0): LPC_GPIO2->FIOCLR|=1<<0; break;//envio un cero al P2.0
		case(1): LPC_GPIO2->FIOCLR|=1<<1; break;//envio un cero al P2.1
		case(2): LPC_GPIO2->FIOCLR|=1<<2; break;//envio un cero al P2.2
		case(3): LPC_GPIO2->FIOCLR|=1<<3; break;//envio un cero al P2.3

		}
		retardo();

 scan_data = LPC_GPIO2->FIOPIN>>8 &(0XF);//verifica si se ha presionado alguna tecla encia un cero porque son entradas
 columna = -1;//es un CONTROL para saber si se presiono una tecla
 switch(scan_data){
 case(0xE): columna =0;break;
 case(0xD): columna =1;break;
 case(0xB): columna =2;break;
 case(0x7): columna =3;break;

 }

 if(columna!=-1){//este algoritmo con break hace que salga del bucle for si es que presiona una tecla
	 break;
 }
/*
 *otambien se podria poner este algoritmp
 *  if(columna==0{
 *   break;  //si se pesiono una tecla sale del for
 *  }
 *
 *  if(columna==1{
 *   break;
 *  }
 *   if(columna==2{
 *   break;
 *  }
 *   if(columna==3{
 *   break;
 *  }
 *
 */









	}//pertece al for  filai


	if(columna ==-1){//es porque no se presiono ninguna tecla nada retorna 0

		return 0;
	}

	else{



		return keyboard[fila][columna];//la funcion uint8_t ScanearTeclado() retorna un elememto del arreglo


	}

	dato_uart = keyboard[fila][columna];// dato_uart es un dato globa; listo para emviar a un uart


}

void retardo(){

	uint32_t i;
	for(i=0;i<1000000;i++){};//se purde ir reajustado conforme a la necesidad
}


void Set_UART3(void){
	UART_CFG_Type UARTconfig;
	UART_FIFO_CFG_Type UARTFIFOconfig;
	PINSEL_CFG_Type PINCFG;
	PINCFG.Funcnum=2; //configuro al pin como funcion TXD3   P0[0] == H[9]
	PINCFG.OpenDrain =0;
	PINCFG.Pinmode = 0; //como tris-state, osea sin pull up
	PINCFG.Pinnum = 0; // elijo al pin 0 para TXD3
	PINSEL_ConfigPin(&PINCFG);

	PINCFG.Pinnum = 1; // pin 1 para RXD3 P0[1]== H[10]
	PINSEL_ConfigPin(&PINCFG); // primero declaro el transmisor y luego el receptor

	UART_ConfigStructInit(&UARTconfig); // usamos config por default
	UART_Init(LPC_UART3,&UARTconfig); //inicializo el uart segun los parametros
	UART_FIFOConfigStructInit(&UARTFIFOconfig);
	UART_FIFOConfig(LPC_UART3,&UARTFIFOconfig);
	UART_TxCmd(LPC_UART3,ENABLE); //Habilito la funcion tx

}


void enviar(void){

	//dato_uart
	//uint8_t str[]={"holacordoba\n"};
//for(int i = 0; i<sizeof(str); i++){
	UART_SendByte(LPC_UART3,dato_uart);
}



