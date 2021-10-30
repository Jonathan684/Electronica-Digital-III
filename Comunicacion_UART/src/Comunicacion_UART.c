/*
===============================================================================
Alumno: Cristian Velazquez
Configuracion del ADC
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

//uint8_t buffer[] ="hola";
void conf_pin(void);
void Set_UART3(void);
void enviar(void);
void enviar2(void);
void recibira(void);
void retardo(void);
void recibir(void);
void recibir2(void);
__IO char arraybuffer[] = {"off"};
void apagar_luces();
void encender_luces();

int main(void) {
	conf_pin();
	Set_UART3();
    LPC_PINCON->PINSEL0 |= 0b1010;//configuramos PUERTO0 pin 0 es tx, pin 1 rx
    UART_TxCmd(LPC_UART3,ENABLE);
	NVIC_EnableIRQ(UART3_IRQn);
	while(1){
   //recibir y enviar
		if((arraybuffer[0] =='o') && (arraybuffer[1]=='f') && (arraybuffer[2] == 'f')){
						printf("Luces Apagadas \n");
						apagar_luces();
			}
			printf("Contenido de  arraybuffer ->%s<-\n",arraybuffer);
			retardo();
			//UART_Send(LPC_UART3, str,sizeof(str),NONE_BLOCKING);
			if((arraybuffer[0] == 'o')&&(arraybuffer[1] == 'n')){
			      	printf("Luces Encendidas \n");
			      	encender_luces();
			}

    }
	return 0;
}
void conf_pin(void){
	LPC_GPIO2->FIODIR   |= (1<<12); //salida
	LPC_GPIO2->FIOCLR   |= (1<<12);
	LPC_GPIO2->FIOSET   |= (1<<12);
}
void apagar_luces(){
	LPC_GPIO2->FIOSET   |= (1<<12);

}
void encender_luces(){
	LPC_GPIO2->FIOCLR   |= (1<<12);
}
void Set_UART3(void){
	UART_CFG_Type UARTconfig;
	UART_FIFO_CFG_Type UARTFIFOconfig;
	PINSEL_CFG_Type PINCFG;
	PINCFG.Funcnum = 2; //configuro al pin como funcion TXD3   P0[0] == H[9]
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
	LPC_UART3 -> IER|=(1<<0); //

}
void UART3_IRQHandler(void){
	//NVIC_DisableIRQ(UART3_IRQn);

	UART_Receive(LPC_UART3,&arraybuffer,sizeof(arraybuffer),NONE_BLOCKING);
	retardo();

			//UART_TxCmd(LPC_UART3,DISABLE);
}
void enviar(void){

	//uint8_t str[]={"holaa "};
	char str[]={"h\n"};
	for(int i = 0; i<1; i++){
		UART_SendByte(LPC_UART3,str[i]);
	}
}
void enviar2(void){
	//uint8_t buffer[]={0x30,0x31,65,64};
	uint8_t message;
	uint8_t buffer[]={"Digitales3 \n"      };

	//UART_Send(LPC_UART3,buffer,sizeof(buffer),NONE_BLOCKING);
//message = UART_Receive(LPC_UART3,buffer,4,NONE_BLOCKING);

	//UART_Send(LPC_UART3,message;,sizeof(message),NONE_BLOCKING);
	//UART_SendByte(LPC_UART3,message);
	UART_Send(LPC_UART3,buffer,sizeof(buffer),NONE_BLOCKING);

}




void retardo (void){
uint32_t conta;
for(conta = 0; conta<10000000;conta++){} //lazo for de retardo
return; // fin de la función retardo y regreso a main
}
void recibir(void){

	uint8_t arraybuffer[20];
	UART_TxCmd(LPC_UART3,ENABLE);
			  UART_Receive(LPC_UART3,&arraybuffer,sizeof(arraybuffer),NONE_BLOCKING);
				UART_Send(LPC_UART3, arraybuffer,sizeof( arraybuffer),NONE_BLOCKING);
				UART_TxCmd(LPC_UART3,DISABLE);



}


void recibira(void){//funcion espejo
	uint8_t mensaje;
	mensaje=UART_ReceiveByte(LPC_UART3);


	UART_SendByte(LPC_UART3,mensaje);
}

