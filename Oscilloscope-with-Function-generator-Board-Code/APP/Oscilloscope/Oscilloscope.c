/*
 * File Name --> Oscilloscope.c
 * Author	 --> Ahmed Mohamed Abd-Elfattah
 * Version	 --> 1.0.0
 * Layer	 --> Application
 * Brief	 -->
 * */

/*
==================================================
  Start Section --> File Includes
==================================================
 */
#include "Oscilloscope_APIs.h"
#include "Oscilloscope_Private.h"
#include "math.h"
#include "HMCAL/DMA/DMA.h"
#include "LIB/utils.h"
#include "HMCAL/DAC/DAC.h"
#include "HMCAL/TIM/TIM.h"
#include "HMCAL/GPIO/GPIO.h"
#include "Oscilloscope_Config.h"
#include "HMCAL/RCC/RCC.h"

#include "HMCAL/ADC/ADC.h"
#include "HMCAL/TIM/TIM.h"
#include "HMCAL/DMA/DMA.h"
#include "HMCAL/USART/USART.h"

#include "common.h"
/*
==================================================
  End   Section --> File Includes
==================================================
 */

/*
 * PC USART DMA
 * Is the DMA, which has the responsibility to send analog values through DMA.
 * */

extern DMA_Cfg G_PCUSARTDMA_DMA_Cfg;
extern USART_Cfg G_PCUSART_USART_Cfg;
static DMA_Cfg GS_AnalogValuesDMA_DMA_Cfg;

u32 completed = 0;

volatile static u16 GS_AnalogValuesBuffer_u16[2][CONFIG_ANALOG_VALUES_BUFFER_LENGTH];

static void PC_DMA(void);

void DMA_Array_Completed(void);

extern union FunctionGeneratorDataFrame G_FunctionGeneratorDataFrame;
void APP_OscilloscopeInit_v(void){


	/*Configure USART that will send data to PC.*/
	G_PCUSARTDMA_DMA_Cfg.InstanceId = CONFIG_PC_USART_DMA_INSTANCE_ID;
	G_PCUSARTDMA_DMA_Cfg.DataSize =DMA_DATA_SIZE_MEM_BYTE_PERPH_BYTE;
	G_PCUSARTDMA_DMA_Cfg.CircularMode = DMA_CIRCULAR_MODE_DISABLE;
	G_PCUSARTDMA_DMA_Cfg.IncrementMode = DMA_INCREMENT_MODE_INCR_MEM_FIXED_PERPH;
	G_PCUSARTDMA_DMA_Cfg.TransferDir = DMA_TRANSFER_DIR_MEM_TO_PERPH;
	G_PCUSARTDMA_DMA_Cfg.Interrupt = DMA_INTERRUPT_TRANSFER_COMPLETE;
	G_PCUSARTDMA_DMA_Cfg.Size = 2*ARRAY_LENGTH(GS_AnalogValuesBuffer_u16[0]);
	G_PCUSARTDMA_DMA_Cfg.PeripheralAddress = USART_GetAdressForDMA_u32(&G_PCUSART_USART_Cfg);
	G_PCUSARTDMA_DMA_Cfg.MemoryAddress = (u32)&GS_AnalogValuesBuffer_u16[0];

	USART_Config_v(&G_PCUSART_USART_Cfg);
	DMA_Config_v(&G_PCUSARTDMA_DMA_Cfg, CONFIG_PC_USART_DMAx_CHANNELx_STREAMx);
	/*Configure Timer that samples / triggers ADC*/
	/*Timer*/
	TIM_Cfg L_ADCTimer_TIM_Cfg={0};
	L_ADCTimer_TIM_Cfg.AutoReloadValue = 16-1;//10-1
	L_ADCTimer_TIM_Cfg.Channel1Mode = TIM_CHANNEL_MODE_OUTPUT_PWM_ACTIVE_HIGH;
	L_ADCTimer_TIM_Cfg.ActiveChannels = TIM_CHANNEL_1_MASK;
	L_ADCTimer_TIM_Cfg.Prescaler = 1000-1;//80-1
	L_ADCTimer_TIM_Cfg.Channel1CaptureCompare = L_ADCTimer_TIM_Cfg.AutoReloadValue*.5;
	L_ADCTimer_TIM_Cfg.InstanceId = CONFIG_ADC_TRIGGER_SRC_TIMER_INSTANCE_ID;

	/*Configure ADC*/
	ADC_Cfg L_ADCOscilloscope_ADC_Cfg = {0};

	L_ADCOscilloscope_ADC_Cfg.ConversionMode = ADC_CONV_MODE_CONTINUOUS;
	L_ADCOscilloscope_ADC_Cfg.Resolution = ADC_RES_12_BIT;
	u32 L_Sequence_u32[] = {
			ADC1_A1,
	};

	L_ADCOscilloscope_ADC_Cfg.ConversionChannels = L_Sequence_u32;
	L_ADCOscilloscope_ADC_Cfg.ConversionLength = ARRAY_LENGTH(L_Sequence_u32);
	L_ADCOscilloscope_ADC_Cfg.InstanceId = CONFIG_ADC_INSTANCE_ID;

	L_ADCOscilloscope_ADC_Cfg.ExternalTriggerDetection = ADC_EXT_TRIG_DETECT_BOTH_EDGES;
	L_ADCOscilloscope_ADC_Cfg.SampleTime = ADC_SAMPLE_TIME_3_CYCLES;
	L_ADCOscilloscope_ADC_Cfg.RegularEventTrigger = CONFIG_ADC_EVENT_TRIGGER;

	L_ADCOscilloscope_ADC_Cfg.DMA = ADC_DMA_ENABLE;

	/*Configure DMA of ADC*/
	GS_AnalogValuesDMA_DMA_Cfg.Burst = DMA_BURST_MEM_SINGLE_TRANSFER_PERPH_SINGLE_TRANSFER;

	GS_AnalogValuesDMA_DMA_Cfg.CircularMode = DMA_CIRCULAR_MODE_ENABLE;
	GS_AnalogValuesDMA_DMA_Cfg.DataSize = DMA_DATA_SIZE_MEM_HALF_WORD_PERPH_HALF_WORD;
	GS_AnalogValuesDMA_DMA_Cfg.TransferDir = DMA_TRANSFER_DIR_PERPH_TO_MEM;

	GS_AnalogValuesDMA_DMA_Cfg.DirectMode = DMA_DIRECT_MODE_ENABLE;
	GS_AnalogValuesDMA_DMA_Cfg.IncrementMode = DMA_INCREMENT_MODE_INCR_MEM_FIXED_PERPH;


	GS_AnalogValuesDMA_DMA_Cfg.Size = ARRAY_LENGTH(GS_AnalogValuesBuffer_u16[0]);
	GS_AnalogValuesDMA_DMA_Cfg.PeripheralAddress = ADC_GetAdressForDMA_u32(&L_ADCOscilloscope_ADC_Cfg);
	GS_AnalogValuesDMA_DMA_Cfg.MemoryAddress = (u32)&GS_AnalogValuesBuffer_u16[0];
	GS_AnalogValuesDMA_DMA_Cfg.MemoryAddressSecondBuffer = (u32)&GS_AnalogValuesBuffer_u16[1];
	GS_AnalogValuesDMA_DMA_Cfg.InstanceId=CONFIG_ADC_DMA_INSTANCE_ID;
	GS_AnalogValuesDMA_DMA_Cfg.Interrupt = DMA_INTERRUPT_TRANSFER_COMPLETE;
	GS_AnalogValuesDMA_DMA_Cfg.DoubleBuffer = DMA_DOUBLE_BUFFER_ENABLE_START_BUFFER_0;
	GS_AnalogValuesDMA_DMA_Cfg.Priority = DMA_PRIORITY_VERY_HIGH;

	DMA_Config_v(&GS_AnalogValuesDMA_DMA_Cfg, PRIVATECONFIG_ADC_DMA_CHANNELx_STREAMx);
	InterruptHandler_SetDMACallback_v(CONFIG_ADC_DMA_INSTANCE_ID, CONFIG_ADC_DMA_STREAM, DMA_INTERRUPT_TRANSFER_COMPLETE, DMA_Array_Completed);
	DMA_EnableDMA_v(&GS_AnalogValuesDMA_DMA_Cfg, PRIVATECONFIG_ADC_DMA_CHANNELx_STREAMx);

	/*Function generator DMA*/
	TIM_Config_v(&L_ADCTimer_TIM_Cfg);
	TIM_StartTimer_v(&L_ADCTimer_TIM_Cfg);

	ADC_Config_v(&L_ADCOscilloscope_ADC_Cfg);

	ADC_EnableADC_v(&L_ADCOscilloscope_ADC_Cfg);
	ADC_StartSoftwareConversion_v(&L_ADCOscilloscope_ADC_Cfg);
}

void DMA_Array_Completed(void){
	if (0 == completed){
		DMA_ChangeBuffer0Address_v (&G_PCUSARTDMA_DMA_Cfg,CONFIG_PC_USART_DMAx_CHANNELx_STREAMx, (u32)&GS_AnalogValuesBuffer_u16[0]);

		InterruptHandler_SetDMACallback_v(1, 4, DMA_INTERRUPT_TRANSFER_COMPLETE, PC_DMA);

		DMA_EnableDMA_v(&G_PCUSARTDMA_DMA_Cfg, CONFIG_PC_USART_DMAx_CHANNELx_STREAMx);
		USART_EnableUSART_v(&G_PCUSART_USART_Cfg);
	}

	InterruptHandler_SetDMACallback_v(2, 4, DMA_INTERRUPT_TRANSFER_COMPLETE, NULL);
	completed++;
}

/*Change Buffer for ping-pong buffer*/
static void PC_DMA(void){
	u8 L_CurrentBuffer_u8 = DMA_GetCurrentBuffer_v(&GS_AnalogValuesDMA_DMA_Cfg, DMA2_ADC1_CH_0_S_4);
	switch(L_CurrentBuffer_u8){
	case 0:
		DMA_ChangeBuffer0Address_v (&G_PCUSARTDMA_DMA_Cfg,CONFIG_PC_USART_DMAx_CHANNELx_STREAMx, (u32)&GS_AnalogValuesBuffer_u16[1]);
		break;
	case 1:
		DMA_ChangeBuffer0Address_v (&G_PCUSARTDMA_DMA_Cfg,CONFIG_PC_USART_DMAx_CHANNELx_STREAMx, (u32)&GS_AnalogValuesBuffer_u16[0]);
		break;
	}
	DMA_EnableDMA_v(&G_PCUSARTDMA_DMA_Cfg, CONFIG_PC_USART_DMAx_CHANNELx_STREAMx);
}












