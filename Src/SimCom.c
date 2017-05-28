#include <string.h>

#include "FIFO.h"
#include "PhysicalLayer.h"
#include "ServiceLayer.h"
#include "SimCom.h"

#include "cmsis_os.h"


/*
 * Test
 */
void callback0(char from, char to, const char* data, SIMCOM_LENGTH_TYPE length)
{
  sl_send(to,from,data,length);
}

osThreadId sendTaskHandle;
osThreadId receiveTaskHandle;

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
	  char buf[1];

	  uint8_t count = usbRead(0, buf, 1);
	  for(uint8_t i = 0; i < count; i++) {
		  ph_receive_intr(buf[i]);
	  }
	  sl_receive_intr();
	  osThreadYield();
  }
}

bool simcom_init()
{
	sl_config(0, callback0);

	bool state = sl_init();

	if(state) {
		osThreadDef(sendTask, StartSendTask, osPriorityNormal, 0, 128);
		osThreadDef(receiveTask, StartReceiveTask, osPriorityNormal, 0, 128);

		sendTaskHandle = osThreadCreate(osThread(sendTask), NULL);
		receiveTaskHandle = osThreadCreate(osThread(receiveTask), NULL);
	}

	return state;
}
