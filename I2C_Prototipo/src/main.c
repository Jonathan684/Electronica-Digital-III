/*
===============================================================================
 Name        : I2C_Prototipo.c
 Author      : $(author)
 Version     :
 Copyright   : $(copyright)
 Description : main definition
===============================================================================
*/

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#include "lpc17xx_i2c.h"
#include "lpc17xx_pinsel.h"
#include "debug_frmwrk.h"
#endif

#include <cr_section_macros.h>

#include <stdio.h>

// TODO: insert other include files here

// TODO: insert other definitions and declarations here
#define I2CDEV LPC_I2C0
#define BUFFER_SIZE 0x80
int Done = 0;
void confI2C(void);
void confGPIO(void);
uint8_t* menu1 = (uint8_t*)0;
uint32_t count = 6;
int main(void) {

    printf("Comunicacion I2C\n");
    confGPIO();
    confI2C();

    while(1){



    	LPC_I2C0 -> I2CONSET |=(1<<5) ;
    	LPC_I2C0 -> I2DAT = (0XEF);
    	LPC_I2C0 ->I2DAT = menu1;
        if( I2C_MasterTransferComplete(LPC_I2C0)==1){
        	printf("Funciona \n");
        }
        printf("NO Funciona %d\n");

    }
}
void confGPIO(void){

	 PINSEL_CFG_Type PinCfg;

     //Init I2C pin connect

	 PinCfg.Portnum = PINSEL_PORT_0;
	 PinCfg.Pinnum =   PINSEL_PIN_27;
	 PinCfg.Funcnum = 1;
	 PinCfg.Pinmode = 0; //PINSEL_PINMODE_TRISTATE
     PinCfg.OpenDrain = 0;

	 PINSEL_ConfigPin (& PinCfg); // SDA0
	 PinCfg.Pinnum = 28;
	 PINSEL_ConfigPin (& PinCfg); // SCL0
}

void confI2C(void){

//    // Inicializar periférico I2C


	I2C_Init(LPC_I2C0, 100000);
	LPC_I2C0 ->I2CONSET |=(1<<6);
	LPC_I2C0 ->I2CONCLR |= (1<<5);
	LPC_I2C0 ->I2CONCLR |= (1<<4);
	LPC_I2C0 ->I2CONCLR |= (1<<3);

//
//
//
  	I2C_M_SETUP_Type conf;
    conf.sl_addr7bit = 0x27;
  	conf.rx_data = NULL;
  	conf.rx_length = 0;
  	conf.tx_length = 40;
  	conf.tx_data = menu1;
  	conf.retransmissions_max=10;
  	I2C_MasterTransferData(LPC_I2C0,&conf , I2C_TRANSFER_INTERRUPT);
    I2C_Cmd(LPC_I2C0, ENABLE);


//    I2C_OWNSLAVEADDR_CFG_Type conf2;
//    conf2.SlaveAddrChannel = 0;
//    conf2.SlaveAddr_7bit = 0x27;
//    conf2.SlaveAddrMaskValue = 0x27;
//    I2C_SetOwnSlaveAddr(LPC_I2C0, &conf2);
//    // Configurar la interrupción para I2C en NVIC del núcleo ARM * /
//	// Desactivar interrupción I2C0 * /
//	NVIC_DisableIRQ (I2C0_IRQn);
//	// preferencia = 1, sub-prioridad = 0 * /
//	NVIC_SetPriority (I2C0_IRQn, ((0x01 << 3) | 0x01));
//	// habilitar interrupción I2C
	//I2C_IntCmd (LPC_I2C0, ENABLE);
//    // Habilita la operación I2C * /
//	I2C_Cmd (I2CDEV, ENABLE);
//	//NVIC_EnableIRQ (I2C0_IRQn);
//	/*
//	 * Status I2C_MasterTransferData(LPC_I2C_TypeDef *I2Cx, \
//        I2C_M_SETUP_Type *TransferCfg, I2C_TRANSFER_OPT_Type Opt);
//	 */

}

void  I2C0_IRQHandler ( void )
{
	I2C_MasterHandler(LPC_I2C0);

}

