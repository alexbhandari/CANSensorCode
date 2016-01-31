/*
 * adc_support.c
 *
 *  Created on: Jan 31, 2016
 *      Author: alexbhandari
 */

#include "adc_support.h"

/**
 * @fn int adc_conversion_helper(uint16_t channel)
 * @brief Runs an ADC conversion and prints the result
 */
int adc_conversion_helper(uint16_t channel) {
	adcData_t adc_data;
	adcData_t *adc_data_ptr = &adc_data;

 	/** - Start Group1 ADC Conversion
 	*     Select Channel
 	*/
	adcStartConversion_selChn(adcREG1, channel, 1, adcGROUP1);

 	/** - Wait for ADC Group1 conversion to complete */
 	while(!adcIsConversionComplete(adcREG1, adcGROUP1));

	/** - Read the conversion result
	*     The data contains the Ambient Light sensor data
    */
	int count = 0;
	adcGetSingleData(adcREG1, adcGROUP1, adc_data_ptr);
	printf("data: %d. Count: %d\r\n",adc_data_ptr->value,count);
	return count;
}

/** @fn void adcGetSingleData(adcBASE_t *adc, unsigned group, adcData_t *data)
*   @brief Get single converted ADC value
*   @param[in] adc Pointer to ADC module:
*              - adcREG1: ADC1 module pointer
*              - adcREG2: ADC2 module pointer
*              - adcREG3: ADC3 module pointer
*   @param[in] group Hardware group of ADC module:
*              - adcGROUP0: ADC event group
*              - adcGROUP1: ADC group 1
*              - adcGROUP2: ADC group 2
*   @param[out] data Pointer to store ADC converted data
*
*/
void adcGetSingleData(adcBASE_t *adc, unsigned group, adcData_t *data)
{
    unsigned  buf;
    adcData_t *ptr = data;

    /** -  Get conversion data and channel/pin id */
    buf        = adc->GxBUF[group].BUF0;
    ptr->value = (unsigned short)(buf & 0xFFFU);
    ptr->id    = (unsigned short)((buf >> 16U) & 0x1FU); // int to unsigned short

    adc->GxINTFLG[group] = 9U;

    /**   @note The function canInit has to be called before this function can be used.\n
    *           The user is responsible to initialize the message box.
    */
}

/** @fn void adcStartConversion_selChn(adcBASE_t *adc, unsigned channel, unsigned fifo_size, unsigned group)
*   @brief Starts an ADC conversion
*   @param[in] adc Pointer to ADC module:
*              - adcREG1: ADC1 module pointer
*              - adcREG2: ADC2 module pointer
*   @param[in] channel ADC channel to be selected for conversion
*   @param[in] fifo_size ADC fifo size to be configured.
*   @param[in] group Hardware group of ADC module:
*              - adcGROUP0: ADC event group
*              - adcGROUP1: ADC group 1
*              - adcGROUP2: ADC group 2
*
*   This function Starts the convesion of the ADC selected group for the selected channel
*
*/
void adcStartConversion_selChn(adcBASE_t *adc, unsigned channel, unsigned fifo_size, unsigned group)
{
    /** - Setup FiFo size */
    adc->GxINTCR[group] = fifo_size;

    /** - Start Conversion */
    adc->GxSEL[group] = 1 << channel;
}
