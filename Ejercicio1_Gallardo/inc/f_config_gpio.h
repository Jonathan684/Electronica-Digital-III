/*
 * f_config_gpio.h
 *
 *  Created on: 25 ago. 2021
 *      Author: Administrador
 */

#ifndef F_CONFIG_GPIO_H_
#define F_CONFIG_GPIO_H_

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#ifndef NULL
#define NULL ((void*) 0)
#endif

#define MID 16U

void f_pin_conf(uint8_t portNum, uint8_t pinNum , uint8_t dir);
void f_pin_set(uint8_t portNum, uint8_t pinNum , uint8_t state);
uint8_t f_pin_read(uint8_t portNum, uint8_t pinNum);
LPC_GPIO_TypeDef *f_get_gpiop(uint8_t portNum);

#endif /* F_CONFIG_GPIO_H_ */
