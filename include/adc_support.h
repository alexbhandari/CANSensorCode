/*
 * adc_support.h
 *
 *  Created on: Jan 31, 2016
 *      Author: alexbhandari
 */

#ifndef INCLUDE_ADC_SUPPORT_H_
#define INCLUDE_ADC_SUPPORT_H_

/* ADC Interface Functions */
void adcGetSingleData(adcBASE_t *adc, unsigned group, adcData_t *data);
void adcStartConversion_selChn(adcBASE_t *adc, unsigned channel, unsigned fifo_size, unsigned group);

#endif /* INCLUDE_ADC_SUPPORT_H_ */
