#include "PhysicalLayer.h"

char ph_send_queue_buf[PH_BUF_LEN];
char ph_receive_queue_buf[PH_BUF_LEN];

char ph_send_dma_buf[PH_BUF_LEN];
char ph_receive_it_buf[1];

char_queue ph_send_queue;
char_queue ph_receive_queue;
bool ph_initialized = false;

UART_HandleTypeDef *uart_device;

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	char *p = huart->pRxBuffPtr - 1;

	ph_receive_intr(*p);

	sl_receive_intr();

	HAL_UART_Receive_IT(huart, p, 1);
}

bool ph_init(UART_HandleTypeDef *device)
{
  if(ph_initialized) {
    return false;
  }

  init_char_queue(&ph_send_queue, ph_send_queue_buf, PH_BUF_LEN);
  init_char_queue(&ph_receive_queue, ph_receive_queue_buf, PH_BUF_LEN);

  uart_device = device;

  HAL_UART_Receive_IT(uart_device, ph_receive_it_buf, 1);

  ph_initialized = true;
  return true;
}

bool ph_send(char data)
{
  if(!ph_initialized) {
    return false;
  }

  return in_char_queue(&ph_send_queue, data);
}

bool ph_receive(char *data)
{
  if(!ph_initialized) {
    return false;
  }

  return out_char_queue(&ph_receive_queue, data);
}

bool ph_receive_intr(char data)
{
  if(!ph_initialized) {
    return false;
  }

  return in_char_queue(&ph_receive_queue, data);
}

void ph_send_intr()
{
  /*
    You must call this function timely to send the data in the queue
    This function must be modified to use different types of physical devices
  */
  char c;
  SIMCOM_LENGTH_TYPE count = 0;

  while(uart_device->State != HAL_UART_STATE_READY || uart_device->State != HAL_UART_STATE_BUSY_RX) {
	  osDelay(1);
  }

  while(out_char_queue(&ph_send_queue, &c)) {
    ph_send_dma_buf[count] = c;
    count++;
  }

  HAL_UART_Transmit_DMA(uart_device, ph_send_dma_buf, count);
}
