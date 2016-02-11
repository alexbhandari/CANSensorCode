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
//int adc_convert_all_channels() { //NOT WORKING
//	adcData_t adc_data[16];
//	adcData_t *adc_data_ptr = &adc_data;
//
//	adcStartConversion(adcREG1, adcGROUP1);
//
// 	/** - Wait for ADC Group1 conversion to complete */
// 	while(!adcIsConversionComplete(adcREG1, adcGROUP1));
//
//	/** - Read the conversion result
//	*     The data contains the Ambient Light sensor data
//    */
//	int count = 0;
//	count = adcGetData(adcREG1, adcGROUP1, adc_data_ptr);
//	adcData_t *ptr = adc_data_ptr;
//	char str[1600];
//	//printf("data: %ld. voltage: %f. id: %ld. Count: %d\r\n",adc_data_ptr->value,adc_decode(adcREG1, adc_data_ptr->value),adc_data_ptr->id,count);
//	int i;
//	for(i = 9U; i < 10U; i++) {
//		uint16 id = adc_data[i].id;
//		unsigned int value = (unsigned int)adc_data[i].value;
//		adc_data_ptr++;
//		//printf("data: %ld. voltage: %f. id: %ld.\r\n", value, adc_decode(adcREG1, value), id);
//		//string_concat(str, " %d \r\n", adc_decode(value));
//	}
//	printf(str);
//	return count;
//}
void adc_convert_all_channels(adcData_t *ptr) {
	int i;
	int AD_CHANNELS[16] = {0U,1U,2U,3U,4U,5U,6U,7U,8U,9U,10U,11U,16U,17U,20U,21U};
	for(i = 0U; i < sizeof(AD_CHANNELS)/sizeof(AD_CHANNELS[0]); i++) {
		adc_convert_channel(AD_CHANNELS[i], ptr);
		ptr ++;
	}
}


/**
 * @fn int adc_conversion_helper(uint16_t channel)
 * @brief Runs an ADC conversion and prints the result
 */
int adc_convert_channel(uint16_t channel, adcData_t *adc_data_ptr) {

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
	//printf("channel: %d. data: %d. voltage: %f. id: %d. Count: %d\r\n", channel, adc_data_ptr->value, adc_decode(adcREG1, adc_data_ptr->value), adc_data_ptr->id, count);
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

float adc_decode(adcBASE_t *adc, int value)
{
	uint32 mode = (adc->OPMODECR & ADC_12_BIT_MODE);
	int divisions = 1024U;
	if(mode == ADC_12_BIT_MODE) {
		divisions = 4096U;
	}
	return (value*(AD_REF_HIGH - AD_REF_LOW) + 0.5F)/divisions + AD_REF_LOW;
}

void string_concat(char* str1, const char* format, ...)
{
    char       str2[100];
    va_list    args;

    va_start(args, format);
    vsnprintf(str2, sizeof(str2), format, args); // do check return value
    va_end(args);

    strcat(str1,str2);
}
