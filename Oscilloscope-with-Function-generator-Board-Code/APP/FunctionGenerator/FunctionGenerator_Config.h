/*
 * File Name --> FunctionGenerator_Config.h
 * Author	 --> Ahmed Mohamed Abd-Elfattah
 * Version	 --> 1.0.0
 * Layer	 --> Application
 * Brief	 --> Function Generator Configurations
 * */

#ifndef APP_FUNCTIONGENERATOR_FUNCTIONGENERATOR_CONFIG_H_
#define APP_FUNCTIONGENERATOR_FUNCTIONGENERATOR_CONFIG_H_




/*
==================================================
  Start Section --> File Includes
==================================================
*/
#include "common.h"
/*
==================================================
  End   Section --> File Includes
==================================================
*/


/*
==================================================
  Start Section --> Function Generator & DAC Configurations
==================================================
*/
#define CONFIG_FUNC_GEN_MAXIMUM_VOLTAGE			3.2
#define CONFIG_FUNC_GEN_DAC_RESOLUTION			12
#define CONFIG_FUNC_GEN_SAMPLE_POINTS_NUM		360

#define CONFIG_DAC_CHANNEL_1_TIMER_INSTANCE		6
#define CONFIG_DAC_CHANNEL_2_TIMER_INSTANCE		7


#define CONFIG_DAC_GPIO_Id						'A'
#define CONFIG_DAC_CH1_Pin						4
#define CONFIG_DAC_CH2_Pin						5


#define CONFIG_FUNC_GEN_DMA_INSTANCE_ID					1
#define CONFIG_FUNC_GEN_DMA_CHANNEL						4
#define CONFIG_FUNC_GEN_DMA_STREAM						2
#define CONFIG_FUNC_GEN_USART_DMAx_CHANNELx_STREAMx		DMA1_UART4_RX_CH_4_S_2
/*
==================================================
  End   Section --> Function Generator & DAC Configurations
==================================================
*/


#endif /* APP_FUNCTIONGENERATOR_FUNCTIONGENERATOR_CONFIG_H_ */
