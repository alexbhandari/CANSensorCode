/*
 * can_support.h
 *
 *  Created on: Jan 31, 2016
 *      Author: alexbhandari
 */

#ifndef INCLUDE_CAN_SUPPORT_H_
#define INCLUDE_CAN_SUPPORT_H_

#include "can.h"
#include <stdio.h>
#include "sci.h"
#include "stdlib.h"

#define D_COUNT  8

/* CAN Interface */
void can_transmit_recieve();
void dumpSomeData();

#endif /* INCLUDE_CAN_SUPPORT_H_ */
