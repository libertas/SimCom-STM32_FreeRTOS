#include "usb.h"

#define RxBufferSize 1024

char RxBuffer[RxBufferSize];
volatile uint readIndex = RxBufferSize;
volatile uint RxFifoIndex = 0;


void dummy(void);

void dummy(void){
     volatile static int i;

     for (i=0;i<50;i++);

     i++;
}

int usbWrite(int FD, char * outstr, int len){

     while(CDC_Transmit_FS((uint8_t*)outstr,len) == USBD_BUSY);
     dummy();
     return len;
}


int usbRead(int fd, char *instring, uint count){
     uint32_t bytesread = 0;

     if(count > bytesread){
          while(readIndex == (RxBufferSize - RxFifoIndex)){}
          {
               while((count > bytesread) & (readIndex !=(RxBufferSize - RxFifoIndex ))){
                    instring[bytesread] = RxBuffer[RxBufferSize - readIndex];
                    if(readIndex == (0))
                         readIndex = RxBufferSize;
                    else readIndex--;
                    bytesread++;
               }
          }
     }
     return (int)bytesread;
}

uint8_t USB_Receive_FS(uint8_t* Buf, uint32_t *Len)
{
     volatile uint32_t counter = 0;

     while(counter < *Len){
          RxBuffer[RxFifoIndex ] = Buf[counter];
          counter++, RxFifoIndex++;
          if(RxFifoIndex  == RxBufferSize)
               RxFifoIndex  = 0;
          }

  return (USBD_OK);
}
