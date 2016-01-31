/** @file sys_main.c 
*   @brief Application main file
*   @date 28.Aug.2015
*   @version 04.05.01
*
*   This file contains an empty main function,
*   which can be used for the application.
*/

/* 
* Copyright (C) 2009-2015 Texas Instruments Incorporated - www.ti.com 
* 
* 
*  Redistribution and use in source and binary forms, with or without 
*  modification, are permitted provided that the following conditions 
*  are met:
*
*    Redistributions of source code must retain the above copyright 
*    notice, this list of conditions and the following disclaimer.
*
*    Redistributions in binary form must reproduce the above copyright
*    notice, this list of conditions and the following disclaimer in the 
*    documentation and/or other materials provided with the   
*    distribution.
*
*    Neither the name of Texas Instruments Incorporated nor the names of
*    its contributors may be used to endorse or promote products derived
*    from this software without specific prior written permission.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
*  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
*  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
*  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT 
*  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
*  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
*  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
*  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
*  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
*  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
*  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
*/


/* USER CODE BEGIN (0) */
/* USER CODE END */

/* Include Files */

#include "sys_common.h"

/* USER CODE BEGIN (1) */
#include "can.h"
#include "esm.h"
#include "sys_core.h"
#include "sci.h"
#include "stdlib.h"
#include "adc.h"
#include "gio.h"
#include <stdio.h>

#define D_COUNT  8

uint32 cnt=0, error =0, tx_done =0;
uint8 tx_data[D_COUNT][8] = {0,1,2,3,4,5,6,7};
uint8 rx_data[D_COUNT][8] = {0};
uint8 *tx_ptr = &tx_data[0][0];
uint8 *rx_ptr = &rx_data[0][0];
uint8 *dptr=0;

void dumpSomeData();
/* USER CODE END */

/** @fn void main(void)
*   @brief Application main function
*   @note This function is empty by default.
*
*   This function is called after startup.
*   The user can use this function to implement the application.
*/

/* USER CODE BEGIN (2) */
/* USER CODE END */

void main(void)
{
/* USER CODE BEGIN (3) */

	/* Initialize sci for communication over USB */
	sciInit();
	sciSend(scilinREG,19,(unsigned char *)"CAN code start...\r\n");
	printf("CAN code start...\r\n");

	/* enable irq interrupt in Cortex R4 */
    _enable_interrupt_();

    /** - writing a random data in RAM - to transmit */
    //dumpSomeData();

    /** - configuring CAN1 MB1,Msg ID-1 to transmit and CAN2 MB1 to receive */
    canInit();

    /** - enabling error interrupts */
    canEnableErrorNotification(canREG1);
	canEnableErrorNotification(canREG2);


//    /** - starting transmission */
//    for(cnt=0;cnt<D_COUNT;cnt++)
//    {
//      canTransmit(canREG1, canMESSAGE_BOX1, tx_ptr); /* transmitting 8 different chunks 1 by 1 */
//      unsigned int value = (unsigned int)*tx_ptr;
//      char buffer[8];
//      unsigned int num_char = ltoa(value,(char *)buffer);
//      sciSend(scilinREG, 14, (unsigned char *)"Transmitting: ");
//      sciSend(scilinREG, 1, (unsigned char *)*tx_ptr);
//      printf("Transmitting: %d",*tx_ptr);
//      //while(tx_done == 0){};                 /* ... wait until transmit request is through        */
//      sciSend(scilinREG, 7, (unsigned char *)" - done");
//      sciSend(scilinREG, 2, (unsigned char *)"\r\n");
//      printf(" - done\r\n");
//      tx_done=0;
//      tx_ptr +=8;    /* next chunk ...*/
//    }
//
//    /** - check the received data with the one that was transmitted */
//    tx_ptr = &tx_data[0][0];
//    rx_ptr = &rx_data[0][0];
//    sciSend(scilinREG, 10, (unsigned char *)"Recieved: ");
//    for(cnt=0;cnt<63;cnt++)
//    {
//          if(*tx_ptr++ != *rx_ptr++)
//          {
//               error++; /* data error */
//               sciSend(scilinREG, 7, (unsigned char *)"error\r\n");
//          }
//          else {
//              unsigned int value = (unsigned int)*rx_ptr;
//              char buffer[8];
//              unsigned int num_char = ltoa(value,(char *)buffer);
//              sciSend(scilinREG, 1, (unsigned char *)*rx_ptr);
//          }
//    }
//    sciSend(scilinREG, 2, (unsigned char *)"\r\n");
//    sciSend(scilinREG, 10, (unsigned char *)"Finished\r\n");

//    while(1){}; /* wait forever after tx-rx complete. */



	//ADC stuff
	//while(1) {
		adcStartConversion(adcREG1, 1U);
		//adcStartConversion_selChn(adcREG1, 9U, 1, adcGROUP1);
		while(!adcIsConversionComplete(adcREG1, adcGROUP1));
		adcData_t adc_data;
		adcData_t *adc_data_ptr = &adc_data;
		int count = adcGetData(adcREG1,adcGROUP0,adc_data_ptr);
		printf("data: %d. Count: %d\r\n",adc_data,count);
	//}


    exit(0);
/* USER CODE END */
}

/* USER CODE BEGIN (4) */
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

//sciSend text method
void sciSendText(char * string) {
  unsigned int value = (unsigned int)*tx_ptr;
  char buffer[8];
  unsigned int num_char = ltoa(value,(char *)buffer);
  sciSend(scilinREG, 14, (unsigned char *)"Transmitting: ");
  sciSend(scilinREG, 1, (unsigned char *)*tx_ptr);
}

/* can interrupt notification */
/* Note-You need to remove canMessageNotification from notification.c to avoid redefinition */

/* USER CODE END */
