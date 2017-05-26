#include "SimCom.h"
#include "ServiceLayer.h"

#include "cmsis_os.h"


extern void ph_send_intr();


void callback0(char from, char to, const char* data, SIMCOM_LENGTH_TYPE length)
{
  char msg[100];
  sprintf(msg, "callback %d, from %d, length %d\tdata:%s",\
    to, from, length, data);
  sl_send(to, from, msg, strlen(msg));
}

osThreadId sendTaskHandle;

void StartSendTask(void const * argument)
{
  for(;;)
  {
	  ph_send_intr();
  }
}

bool simcom_init(UART_HandleTypeDef *device)
{
	sl_config(0, callback0);

	bool state = sl_init(device);

	if(state) {
		osThreadDef(sendTask, StartSendTask, osPriorityNormal, 0, 128);
		sendTaskHandle = osThreadCreate(osThread(sendTask), NULL);
	}

	return state;
}
