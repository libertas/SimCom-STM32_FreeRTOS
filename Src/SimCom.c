#include <string.h>

#include "FIFO.h"
#include "PhysicalLayer.h"
#include "ServiceLayer.h"
#include "SimCom.h"

#include "cmsis_os.h"


extern void ph_send_intr();


/*
 * Test
 */
void callback0(char from, char to, const char* data, SIMCOM_LENGTH_TYPE length)
{
  sl_send(to,from,data,length);
}

void StartSendTask(void const * argument)
{
  for(;;)
  {
	  ph_send_intr();
  }
}

void StartReceiveTask(void const * argument)
{
  for(;;)
  {
	  char c;

	  if(out_fifo(&ph_receive_fifo, &c)) {
		  in_char_queue(&ph_receive_queue, c);
		  sl_receive_intr();

	  } else {
		  osDelay(1);
	  }
  }
}

bool simcom_init(UART_HandleTypeDef *device)
{
	sl_config(0, callback0);

	bool state = sl_init(device);

	if(state) {
	}

	return state;
}
