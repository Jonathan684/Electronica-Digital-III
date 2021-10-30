#ifndef LPC17XX_TECLADO_H_
#include "librerias.h"
#define LPC17XX_TECLADO_H_
# define PUERTO_0 (uint8_t)0
# define PUERTO_1 (uint8_t)1
# define PUERTO_2 (uint8_t)2
# define PUERTO_3 (uint8_t)3

# define SALIDA   (uint8_t)1
# define ENTRADA  (uint8_t)0

# define PIN_22   ((uint32_t)(1<<22))

# define PIN_0   ((uint32_t)(1<<0))
# define PIN_4   ((uint32_t)(1<<4))

void confGPIO(void);
void confIntGPIO(void);
void antirebote(void);


#endif

