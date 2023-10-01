/*
 * File Name --> Oscilloscope_Private.h
 * Author	 --> Ahmed Mohamed Abd-Elfattah
 * Version	 --> 1.0.0
 * Layer	 --> Application
 * Brief	 -->
 * */

#ifndef OSCILLOSCOPE_OSCILLOSCOPE_PRIVATE_H_
#define OSCILLOSCOPE_OSCILLOSCOPE_PRIVATE_H_




/*
==================================================
  Start Section --> File Includes
==================================================
*/
#include "Oscilloscope_Config.h"
/*
==================================================
  End   Section --> File Includes
==================================================
*/
#define PRIVATECONFIG_USART_GPIO_PINs (1<<CONFIG_PC_USART_TxPin)|(1<<CONFIG_PC_USART_RxPin)
#define PRIVATECONFIG_ADC_DMA_CHANNELx_STREAMx	((CONFIG_ADC_DMA_CHANNEL<<8) | CONFIG_ADC_DMA_STREAM)

#if (PRIVATECONFIG_ADC_DMA_CHANNELx_STREAMx != CONFIG_ADC_DMAx_CHANNELx_STREAMx)
#error Error in setting one or more in the following configurations CONFIG_ADC_DMA_CHANNEL, CONFIG_ADC_DMA_STREAM, or CONFIG_ADC_DMAx_CHANNELx_STREAMx
#endif

#endif /* OSCILLOSCOPE_OSCILLOSCOPE_PRIVATE_H_ */
