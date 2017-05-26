#ifndef _SERVICELAYER_H
#define _SERVICELAYER_H

#include <stdbool.h>

#include "SimCom.h"

#include "stm32f1xx_hal.h"

#define SL_CALLBACK_NUM 10
#define SL_BUF_LEN 200

bool sl_init(UART_HandleTypeDef *device);
bool sl_config(char port, void (*callback)(char, char, const char*, SIMCOM_LENGTH_TYPE));
bool sl_send(char from_port, char to_port, const char *data, SIMCOM_LENGTH_TYPE length);

/*
  call it timely for your own project
*/
bool sl_receive_intr();

#endif
