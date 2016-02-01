/*
 * adc_support.h
 *
 *  Created on: Jan 31, 2016
 *      Author: alexbhandari
 */

#ifndef INCLUDE_ADC_SUPPORT_H_
#define INCLUDE_ADC_SUPPORT_H_

#include "adc.h"
#include "stdio.h"

#define AD_REF_HIGH		3.3F
#define AD_REF_LOW		0U

/* ADC Interface Functions */
int adc_convert_all_channels();
int adc_convert_channel(uint16_t channel);
void adcGetSingleData(adcBASE_t *adc, unsigned group, adcData_t *data);
void adcStartConversion_selChn(adcBASE_t *adc, unsigned channel, unsigned fifo_size, unsigned group);
float decode_10_bit(int value);
float decode_12_bit(int value);

#endif /* INCLUDE_ADC_SUPPORT_H_ */
