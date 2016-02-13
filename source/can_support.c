/*
 * can_support.c
 *
 *  Created on: Jan 31, 2016
 *      Author: alexbhandari
 */

#include "can_support.h"

uint32 cnt=0, error =0, tx_done =0;
uint8 tx_data[D_COUNT][8] = {0,1,2,3,4,5,6,7};
uint8 rx_data[D_COUNT][8] = {0};
uint8 *tx_ptr = &tx_data[0][0];
uint8 *rx_ptr = &rx_data[0][0];
uint8 *dptr=0;

void can_transmit_recieve() {
    /** - starting transmission */
	int cnt;
    for(cnt=0;cnt<D_COUNT;cnt++)
    {
      canTransmit(canREG1, canMESSAGE_BOX1, tx_ptr); /* transmitting 16 different chunks 1 by 1 */
      unsigned int value = (unsigned int)*tx_ptr;
      char buffer[8];
      unsigned int num_char = ltoa(value,(char *)buffer);
      sciSend(scilinREG, 14, (unsigned char *)"Transmitting: ");
      sciSend(scilinREG, 1, (unsigned char *)*tx_ptr);
      printf("Transmitting: %d",*tx_ptr);
      while(tx_done == 0){};  /* ... wait until transmit request is through        */
      tx_dome = 0;
      sciSend(scilinREG, 7, (unsigned char *)" - done");
      sciSend(scilinREG, 2, (unsigned char *)"\r\n");
      printf(" - done\r\n");
      tx_done=0;
      tx_ptr +=8;    /* next chunk ...*/
    }

    /** - check the received data with the one that was transmitted */
    tx_ptr = &tx_data[0][0];
    rx_ptr = &rx_data[0][0];
    sciSend(scilinREG, 10, (unsigned char *)"Recieved: ");
    for(cnt=0;cnt<63;cnt++)
    {
          if(*tx_ptr++ != *rx_ptr++)
          {
               error++; /* data error */
               sciSend(scilinREG, 7, (unsigned char *)"error\r\n");
          }
          else {
              unsigned int value = (unsigned int)*rx_ptr;
              char buffer[8];
              unsigned int num_char = ltoa(value,(char *)buffer);
              sciSend(scilinREG, 1, (unsigned char *)*rx_ptr);
          }
    }
    sciSend(scilinREG, 2, (unsigned char *)"\r\n");
    sciSend(scilinREG, 10, (unsigned char *)"Finished\r\n");
}

/* writing some data to ram  */
void dumpSomeData()
{
     uint32 tmp = 0x11;

     cnt = (D_COUNT*8)-1;
     dptr = &tx_data[0][0];
     *dptr = tmp;

     while(cnt--)
     {
        tmp = *dptr++;
        *dptr = tmp + 0x11;
     }
}

/* can interrupt notification */
void canMessageNotification(canBASE_t *node, uint32 messageBox)
{
     /* node 1 - transfer request */
     if(node==canREG1)
     {
       tx_done=1; /* confirm transfer request */
     }

     /* node 2 - receive complete */
     if(node==canREG2)
     {
      while(!canIsRxMessageArrived(canREG2, canMESSAGE_BOX1));
      canGetData(canREG2, canMESSAGE_BOX1, rx_ptr); /* copy to RAM */
      rx_ptr +=8;
     }

    /* Note: since only message box 1 is used on both nodes we dont check it here.*/
}
