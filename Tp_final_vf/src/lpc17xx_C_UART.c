/*
===============================================================================
   

===============================================================================
*/


#include "librerias.h"

void conf_pin(void);
void Set_UART3(void);
void retardo(void);
void apagar_luces();
void encender_luces();

//uint8_t info[1] = "";
__IO char arraybuffer[] = {"off"};
void UART3_IRQHandler(void){
	retardo();
	UART_Receive(LPC_UART3,&arraybuffer,sizeof(arraybuffer),NONE_BLOCKING);
	if((arraybuffer[0] =='o') && (arraybuffer[1]=='f') && (arraybuffer[2] == 'f')){
		//printf("Luces Apagadas \n");
		apagar_luces();
	}
	//UART_Send(LPC_UART3, str,sizeof(str),NONE_BLOCKING);
	if((arraybuffer[0] == 'o')&&(arraybuffer[1] == 'n')){
		//printf("Luces Encendidas \n");
		encender_luces();
	}
}


void conf_pin(void){
	LPC_GPIO2->FIODIR   |= (1<<12); //salida
	LPC_GPIO2->FIOSET   |= (1<<12); // Comienzan apagadas off
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
	PINCFG.Portnum = PINSEL_PORT_0;
	PINCFG.Funcnum = PINSEL_FUNC_2; //configuro al pin como funcion TXD3   P0[0] == H[9]
	PINCFG.OpenDrain = PINSEL_PINMODE_NORMAL;
	PINCFG.Pinmode = PINSEL_PINMODE_PULLUP; //como tris-state, osea sin pull up
	PINCFG.Pinnum = PINSEL_PIN_0; // elijo al pin 0 para TXD3
	PINSEL_ConfigPin(&PINCFG);

	PINCFG.Pinnum = PINSEL_PIN_1; // pin 1 para RXD3 P0[1]== H[10]
	PINSEL_ConfigPin(&PINCFG);    // primero declaro el transmisor y luego el receptor

	UART_ConfigStructInit(&UARTconfig); //usamos config por default
	UART_Init(LPC_UART3,&UARTconfig); //inicializo el uart segun los parametros
	UART_FIFOConfigStructInit(&UARTFIFOconfig);
	UART_FIFOConfig(LPC_UART3,&UARTFIFOconfig);
	UART_TxCmd(LPC_UART3,ENABLE); //Habilito la funcion tx
	LPC_UART3 -> IER|=(1<<0); // Interrumpe cuando recibe

}
void retardo (void){
uint32_t conta;
for(conta = 0; conta<500000;conta++){} //lazo for de retardo
return; // fin de la función retardo y regreso a main
}

