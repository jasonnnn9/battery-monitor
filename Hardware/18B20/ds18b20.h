#ifndef __DS18B20_H
#define __DS18B20_H

#include "main.h"

#define BS18B20_GPIO_PORT GPIOA
#define BS18B20_GPIO_DATA GPIO_PIN_8
#define DS18B20_DQ_OUT(x) HAL_GPIO_WritePin(BS18B20_GPIO_PORT, BS18B20_GPIO_DATA, (x) ? GPIO_PIN_SET : GPIO_PIN_RESET)
#define DS18B20_DQ_IN HAL_GPIO_ReadPin(BS18B20_GPIO_PORT, BS18B20_GPIO_DATA)

void DS18B20_Init(void);
uint8_t DS18B20_Update(int16_t *temperature);

#endif
