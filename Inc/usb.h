#ifndef USB_H_
#define USB_H_

#include "stm32f1xx_hal.h"
#include "usb_device.h"
#include "usbd_cdc_if.h"

int usbRead(int fd, char *instring, uint count);
int usbWrite(int,char*,int);
uint8_t USB_Receive_FS (uint8_t* Buf, uint32_t *Len);


#endif /* USB_H_ */
