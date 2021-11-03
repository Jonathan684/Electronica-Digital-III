
/*
===============================================================================
 Name        : Tp_Final.c
 Author      : $(author)
 Version     :
 Copyright   : $(copyright)
 Description : main definition
===============================================================================
*/

#include "librerias.h"

int main(void) {
    //Control servo
	configGPIO ();
	configTIMERS();
	configIntExt();
	//Sensor de puerta
	confGPIO();
	confIntGPIO();
    config_sensor();
    ///lcd
	conf_I2C_2();
	confTimer1();
	I2C2_enable();	//Refer to I2CONSET register
	I2C2_Start();
	lcd_init(0x4E, 16, 2);
	lcd_backlight_led(ON);
	Desactivar_alarma();
    ///uart
	conf_pin();
	Set_UART3();
	UART_TxCmd(LPC_UART3,ENABLE);
	NVIC_EnableIRQ(UART3_IRQn);

	while(1) {
    }
    return 0 ;
}

