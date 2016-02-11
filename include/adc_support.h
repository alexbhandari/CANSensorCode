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
#include "string.h"

#define AD_REF_HIGH		3.3F
#define AD_REF_LOW		0U
//#define AD_CHANNELS		{0U,1U,2U,3U,4U,5U,6U,7U,8U,9U,10U,11U,16U,17U,20U,21U}

/* ADC Interface Functions */
void adc_convert_all_channels();
int adc_convert_channel(uint16_t channel, adcData_t *adc_data_ptr);
void adcGetSingleData(adcBASE_t *adc, unsigned group, adcData_t *data);
void adcStartConversion_selChn(adcBASE_t *adc, unsigned channel, unsigned fifo_size, unsigned group);
float adc_decode(adcBASE_t *adc, int value);
void string_concat(char* str1, const char* format, ...);

#endif /* INCLUDE_ADC_SUPPORT_H_ */
