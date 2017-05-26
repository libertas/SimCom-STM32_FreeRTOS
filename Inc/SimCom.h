#ifndef _SIMCOM_H
#define _SIMCOM_H

#include <stdbool.h>
#include <stdint.h>

#include "stm32f1xx_hal.h"

/*
  You can modify this for your own project
*/
#define SIMCOM_LENGTH_TYPE uint16_t
#define SIMCOM_DLENGTH_TYPE uint32_t

bool sl_init(UART_HandleTypeDef *device);

#endif
