/*
 * File Name --> Communication.h
 * Author	 --> Ahmed Mohamed Abd-Elfattah
 * Version	 --> 1.0.0
 * Layer	 --> Application
 * Brief	 --> Communication Implementation
 * */

#include "Communication.h"

DMA_Cfg G_PCUSARTDMA_DMA_Cfg;
USART_Cfg G_PCUSART_USART_Cfg;
extern union FunctionGeneratorDataFrame G_FunctionGeneratorDataFrame;

void APP_CommunicationInit_v(void){
	/*
	 * Configure USART that will send data to PC.
	 * */
	/*
	 * For example if you need to use pin C10 in transmitting configure
	 * CONFIG_PC_USART_GPIO_Id --> 'C
	 *
	 * */
	G_PCUSART_USART_Cfg.GPIOInstanceId = CONFIG_PC_USART_GPIO_Id;
	G_PCUSART_USART_Cfg.GPIOPins = PRIVATECONFIG_USART_GPIO_PINs;

	G_PCUSART_USART_Cfg.BaudRate = CONFIG_PC_USART_BAUDRATE;
	G_PCUSART_USART_Cfg.DMA = USART_DMA_TRANSMIT | USART_DMA_RECEIVE;

	G_PCUSART_USART_Cfg.InstanceId = CONFIG_PC_USART_INSTANCE_ID;
	G_PCUSART_USART_Cfg.TransferDirection = USART_TRANSMIT | USART_RECEIVE;

	/*Configure DMA for function generator*/
	DMA_Cfg L_ReceiveDMA_Configurations = {0};

	L_ReceiveDMA_Configurations.CircularMode = DMA_CIRCULAR_MODE_ENABLE;
	L_ReceiveDMA_Configurations.DataSize = DMA_DATA_SIZE_MEM_BYTE_PERPH_BYTE;
	L_ReceiveDMA_Configurations.IncrementMode = DMA_INCREMENT_MODE_INCR_MEM_FIXED_PERPH;
	L_ReceiveDMA_Configurations.InstanceId = CONFIG_FUNC_GEN_DMA_INSTANCE_ID;

	L_ReceiveDMA_Configurations.Interrupt = DMA_INTERRUPT_TRANSFER_COMPLETE;
	L_ReceiveDMA_Configurations.MemoryAddress = (u32)&G_FunctionGeneratorDataFrame.RxVal;

	L_ReceiveDMA_Configurations.PeripheralAddress = USART_GetAdressForDMA_u32(&G_PCUSART_USART_Cfg);
	L_ReceiveDMA_Configurations.Size = 4;
	L_ReceiveDMA_Configurations.TransferDir = DMA_TRANSFER_DIR_PERPH_TO_MEM;
	L_ReceiveDMA_Configurations.Priority = DMA_PRIORITY_LOW;
	L_ReceiveDMA_Configurations.PeripheralIncrementOffset = DMA_PERPH_INCR_AS_PERPH_SIZE;
	DMA_Config_v(&L_ReceiveDMA_Configurations, CONFIG_FUNC_GEN_USART_DMAx_CHANNELx_STREAMx);
	DMA_EnableDMA_v(&L_ReceiveDMA_Configurations, CONFIG_FUNC_GEN_USART_DMAx_CHANNELx_STREAMx);

}
