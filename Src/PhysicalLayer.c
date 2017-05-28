#include "PhysicalLayer.h"

#include "cmsis_os.h"
#include "usb.h"

char ph_send_queue_buf[PH_BUF_LEN];
char ph_receive_queue_buf[PH_BUF_LEN];

char ph_send_dma_buf[PH_BUF_LEN];
char ph_receive_it_buf[1];

char_queue ph_send_queue;
char_queue ph_receive_queue;
fifo ph_receive_fifo;
bool ph_initialized = false;

osMutexId ph_send_lock;
osMutexDef(ph_send_lock);


bool ph_init()
{
  if(ph_initialized) {
    return false;
  }

  ph_send_lock = osMutexCreate(osMutex(ph_send_lock));

  init_char_queue(&ph_send_queue, ph_send_queue_buf, PH_BUF_LEN);
  init_char_queue(&ph_receive_queue, ph_receive_queue_buf, PH_BUF_LEN);

  ph_initialized = true;
  return true;
}

bool ph_send(char data)
{
  if(!ph_initialized) {
    return false;
  }


  osMutexWait(ph_send_lock, osWaitForever);

  bool result = in_char_queue(&ph_send_queue, data);

  osMutexRelease(ph_send_lock);
  osThreadYield();

  return result;
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

  return in_char_queue(&ph_receive_queue, data);;
}

void ph_send_intr()
{
  /*
    You must call this function timely to send the data in the queue
    This function must be modified to use different types of physical devices
  */
  char c;
  SIMCOM_LENGTH_TYPE count = 0;

  osMutexWait(ph_send_lock, osWaitForever);

  while(out_char_queue(&ph_send_queue, &c)) {
    ph_send_dma_buf[count] = c;
    count++;
  }

  osMutexRelease(ph_send_lock);
  usbWrite(0, ph_send_dma_buf, count);
  osThreadYield();
  osDelay(1);
}
