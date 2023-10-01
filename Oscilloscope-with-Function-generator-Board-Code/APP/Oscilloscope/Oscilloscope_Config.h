/*
 * File Name --> Oscilloscope_Config.h
 * Author	 --> Ahmed Mohamed Abd-Elfattah
 * Version	 --> 1.0.0
 * Layer	 --> Application
 * Brief	 --> Oscilloscope APIs
 * */
#ifndef OSCILLOSCOPE_OSCILLOSCOPE_CONFIG_H_
#define OSCILLOSCOPE_OSCILLOSCOPE_CONFIG_H_

#include "HMCAL/DMA/DMA.h"
#include "HMCAL/TIM/TIM.h"

/*ADC Timer Configurations*/
#define CONFIG_ADC_TRIGGER_SRC_TIMER_INSTANCE_ID	5

/*ADC Configurations*/
#define CONFIG_ADC_INSTANCE_ID						1
#define CONFIG_ADC_EVENT_TRIGGER					ADC_REG_EVENT_TRIG_TIM5_CC1
/*DMA for Storing ADC output values*/
#define CONFIG_ADC_DMA_INSTANCE_ID					2
#define CONFIG_ADC_DMA_CHANNEL						0
#define CONFIG_ADC_DMA_STREAM						4
#define CONFIG_ADC_DMAx_CHANNELx_STREAMx			DMA2_ADC1_CH_0_S_4


#define CONFIG_ANALOG_VALUES_BUFFER_LENGTH			100
/*Use USART x*/
#define CONFIG_PC_USART_INSTANCE_ID					4
/*PC USART Instance Is*/
#define CONFIG_PC_USART_DMA_INSTANCE_ID				1
/*Configure Channel & Stream*/
#define CONFIG_PC_USART_DMAx_CHANNELx_STREAMx		DMA1_UART4_TX_CH_4_S_4

#define CONFIG_PC_USART_GPIO_Id						'C'
#define CONFIG_PC_USART_TxPin						10
#define CONFIG_PC_USART_RxPin						11
#define CONFIG_PC_USART_BAUDRATE					115200

/***********************************************************/

#endif /* OSCILLOSCOPE_OSCILLOSCOPE_CONFIG_H_ */
