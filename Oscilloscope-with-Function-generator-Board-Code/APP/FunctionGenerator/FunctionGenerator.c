/*
 * File Name --> FunctionGenerator.c
 * Author	 --> Ahmed Mohamed Abd-Elfattah
 * Version	 --> 1.0.0
 * Layer	 -->  
 * Github    --> github.com/AhmedAbd-ElFattah
 * Linkedin  --> https://www.linkedin.com/in/ahmed-mohamed-abd-elfattah-39705b238/
 * Brief 	 --> 
 */

/*
 * File Name --> FunctionGenerator.c
 * Author	 --> Ahmed Mohamed Abd-Elfattah
 * Version	 --> 1.0.0
 * Layer	 --> Application
 * Brief	 --> Function Generator Implementation
 * */
/*
==================================================
  Start Section --> File Includes
==================================================
 */
#include "FunctionGenerator_APIs.h"
#include "FunctionGenerator_Private.h"
#include "math.h"
#include "HMCAL/DMA/DMA.h"
#include "LIB/utils.h"
#include "HMCAL/DAC/DAC.h"
#include "HMCAL/TIM/TIM.h"
#include "HMCAL/GPIO/GPIO.h"
#include "FunctionGenerator_Config.h"
#include "HMCAL/RCC/RCC.h"
#include "common.h"

/*
==================================================
  End   Section --> File Includes
==================================================
 */

/*Union of data frame structure that USART will receive*/
union FunctionGeneratorDataFrame G_FunctionGeneratorDataFrame;

typedef struct {
	u16 phaseShift;
	u16 frequency;
	u16 dutyCycle;
	WaveType_enum WaveType;
	f32 amplitude;
}FunctionGeneratorChannel;

FunctionGeneratorChannel func_chx[2];
/*Array that store signal values*/
volatile u16 DACAPP_OutputValues_u16[2][CONFIG_FUNC_GEN_SAMPLE_POINTS_NUM];


static u16 APP_CalcAngleWithinRange_u16(s16 Arg_PhaseShift_s16);

static u16 APP_CalcCorrespondARRValToFreq(u32 Arg_Freq_u32);

static void APP_ChangeDACFreq_v(u8 Arg_ChannelNum_u8, u32 Arg_Freq_u32);

void ExecuteCommmand(void);

void APP_FunctionGeneratorInit_v(void){

	InterruptHandler_SetDMACallback_v(CONFIG_FUNC_GEN_DMA_INSTANCE_ID, CONFIG_FUNC_GEN_DMA_STREAM, DMA_INTERRUPT_TRANSFER_COMPLETE, ExecuteCommmand);

	/*Configure Function Generator Channel 1*/
	DAC_Cfg L_FunctionGeneratorCh1_DAC_Cfg = {0};
	L_FunctionGeneratorCh1_DAC_Cfg.GPIOInstanceId = CONFIG_DAC_GPIO_Id;
	L_FunctionGeneratorCh1_DAC_Cfg.GPIOPins = (1<<CONFIG_DAC_CH1_Pin);
	L_FunctionGeneratorCh1_DAC_Cfg.InstanceId=1;
	L_FunctionGeneratorCh1_DAC_Cfg.Channel = DAC_CHANNEL_1;
	L_FunctionGeneratorCh1_DAC_Cfg.Buffer = DAC_BUFFER_DISABLE;
	L_FunctionGeneratorCh1_DAC_Cfg.DMA = DAC_DMA_ENABLE;
	L_FunctionGeneratorCh1_DAC_Cfg.WaveGeneration = DAC_WAVE_GENERATION_DISABLE;
	L_FunctionGeneratorCh1_DAC_Cfg.Trigger = DAC_TRIGGER_TIM6_TRGO;

	/*Configure Function Generator Channel 2*/
	DAC_Cfg L_FunctionGeneratorCh2_DAC_Cfg = {0};
	L_FunctionGeneratorCh2_DAC_Cfg.GPIOInstanceId = CONFIG_DAC_GPIO_Id;
	L_FunctionGeneratorCh2_DAC_Cfg.GPIOPins = (1<<CONFIG_DAC_CH2_Pin);
	L_FunctionGeneratorCh2_DAC_Cfg.InstanceId=1;
	L_FunctionGeneratorCh2_DAC_Cfg.Channel = DAC_CHANNEL_2;
	L_FunctionGeneratorCh2_DAC_Cfg.Buffer = DAC_BUFFER_DISABLE;
	L_FunctionGeneratorCh2_DAC_Cfg.DMA = DAC_DMA_ENABLE;
	L_FunctionGeneratorCh2_DAC_Cfg.WaveGeneration = DAC_WAVE_GENERATION_DISABLE;
	L_FunctionGeneratorCh2_DAC_Cfg.Trigger = DAC_TRIGGER_TIM7_TRGO;

	/*Configure DMA of Function Generator Channel 1*/
	DMA_Cfg L_DMAFunctionGeneratorCh1_DMA_Cfg = {0};
	L_DMAFunctionGeneratorCh1_DMA_Cfg.InstanceId = 1;
	L_DMAFunctionGeneratorCh1_DMA_Cfg.DataSize = DMA_DATA_SIZE_MEM_HALF_WORD_PERPH_HALF_WORD;
	L_DMAFunctionGeneratorCh1_DMA_Cfg.PeripheralIncrementOffset = DMA_PERPH_INCR_AS_PERPH_SIZE;
	L_DMAFunctionGeneratorCh1_DMA_Cfg.Priority = DMA_PRIORITY_HIGH;
	L_DMAFunctionGeneratorCh1_DMA_Cfg.Burst = DMA_BURST_MEM_SINGLE_TRANSFER_PERPH_SINGLE_TRANSFER;
	L_DMAFunctionGeneratorCh1_DMA_Cfg.DoubleBuffer = DMA_DOUBLE_BUFFER_DISABLE;
	L_DMAFunctionGeneratorCh1_DMA_Cfg.DirectMode = DMA_DIRECT_MODE_DISABLE;
	L_DMAFunctionGeneratorCh1_DMA_Cfg.CircularMode = DMA_CIRCULAR_MODE_ENABLE;
	L_DMAFunctionGeneratorCh1_DMA_Cfg.FlowController = DMA_FLOW_Controller_DMA;
	L_DMAFunctionGeneratorCh1_DMA_Cfg.IncrementMode = DMA_INCREMENT_MODE_INCR_MEM_FIXED_PERPH;
	L_DMAFunctionGeneratorCh1_DMA_Cfg.PeripheralIncrementOffset = DMA_PERPH_INCR_AS_PERPH_SIZE;
	L_DMAFunctionGeneratorCh1_DMA_Cfg.Size = ARRAY_LENGTH(DACAPP_OutputValues_u16[0]);
	L_DMAFunctionGeneratorCh1_DMA_Cfg.TransferDir = DMA_TRANSFER_DIR_MEM_TO_PERPH;
	L_DMAFunctionGeneratorCh1_DMA_Cfg.PeripheralAddress = DAC_GetAddressForDMA_u32(&L_FunctionGeneratorCh1_DAC_Cfg);
	L_DMAFunctionGeneratorCh1_DMA_Cfg.MemoryAddress = (u32)&DACAPP_OutputValues_u16[L_FunctionGeneratorCh1_DAC_Cfg.Channel];



	/*Configure DMA of Function Generator Channel 2*/
	DMA_Cfg L_DMAFunctionGeneratorCh2_DMA_Cfg = {0};
	L_DMAFunctionGeneratorCh2_DMA_Cfg.InstanceId = 1;
	L_DMAFunctionGeneratorCh2_DMA_Cfg.DataSize = DMA_DATA_SIZE_MEM_HALF_WORD_PERPH_HALF_WORD;
	L_DMAFunctionGeneratorCh2_DMA_Cfg.PeripheralIncrementOffset = DMA_PERPH_INCR_AS_PERPH_SIZE;
	L_DMAFunctionGeneratorCh2_DMA_Cfg.Priority = DMA_PRIORITY_HIGH;
	L_DMAFunctionGeneratorCh2_DMA_Cfg.Burst = DMA_BURST_MEM_SINGLE_TRANSFER_PERPH_SINGLE_TRANSFER;
	L_DMAFunctionGeneratorCh2_DMA_Cfg.DoubleBuffer = DMA_DOUBLE_BUFFER_DISABLE;
	L_DMAFunctionGeneratorCh2_DMA_Cfg.DirectMode = DMA_DIRECT_MODE_DISABLE;
	L_DMAFunctionGeneratorCh2_DMA_Cfg.CircularMode = DMA_CIRCULAR_MODE_ENABLE;
	L_DMAFunctionGeneratorCh2_DMA_Cfg.FlowController = DMA_FLOW_Controller_DMA;
	L_DMAFunctionGeneratorCh2_DMA_Cfg.IncrementMode = DMA_INCREMENT_MODE_INCR_MEM_FIXED_PERPH;
	L_DMAFunctionGeneratorCh2_DMA_Cfg.PeripheralIncrementOffset = DMA_PERPH_INCR_AS_PERPH_SIZE;
	L_DMAFunctionGeneratorCh2_DMA_Cfg.Size = ARRAY_LENGTH(DACAPP_OutputValues_u16[1]);
	L_DMAFunctionGeneratorCh2_DMA_Cfg.TransferDir = DMA_TRANSFER_DIR_MEM_TO_PERPH;
	L_DMAFunctionGeneratorCh2_DMA_Cfg.PeripheralAddress = DAC_GetAddressForDMA_u32(&L_FunctionGeneratorCh2_DAC_Cfg);

	L_DMAFunctionGeneratorCh2_DMA_Cfg.MemoryAddress = (u32)&DACAPP_OutputValues_u16[L_FunctionGeneratorCh2_DAC_Cfg.Channel];


	/*DAC 1*/
	TIM_Cfg L_TIMTriggerCh1_TIM_Cfg = {0};
	L_TIMTriggerCh1_TIM_Cfg.AutoReloadValue = APP_CalcCorrespondARRValToFreq(2000);
	L_TIMTriggerCh1_TIM_Cfg.MasterMode = TIM_MASTER_MODE_UPDATE;
	L_TIMTriggerCh1_TIM_Cfg.Prescaler = 0;
	L_TIMTriggerCh1_TIM_Cfg.InstanceId = CONFIG_DAC_CHANNEL_1_TIMER_INSTANCE;

	TIM_Cfg L_TIMTriggerCh2_TIM_Cfg = {0};
	L_TIMTriggerCh2_TIM_Cfg.AutoReloadValue = APP_CalcCorrespondARRValToFreq(2000);
	L_TIMTriggerCh2_TIM_Cfg.MasterMode = TIM_MASTER_MODE_UPDATE;
	L_TIMTriggerCh2_TIM_Cfg.Prescaler = 0;
	L_TIMTriggerCh2_TIM_Cfg.InstanceId = CONFIG_DAC_CHANNEL_2_TIMER_INSTANCE;

	/*
	 * Note the following sequence must be i the same order.
	 * */
	/*Apply configuration for DMA Channel 1 and 2.*/
	DMA_Config_v(&L_DMAFunctionGeneratorCh1_DMA_Cfg, DMA1_DAC1_CH_7_S_5);
	DMA_Config_v(&L_DMAFunctionGeneratorCh2_DMA_Cfg, DMA1_DAC2_CH_7_S_6);

	TIM_Config_v(&L_TIMTriggerCh1_TIM_Cfg);
	TIM_Config_v(&L_TIMTriggerCh2_TIM_Cfg);

	DAC_Config_v(&L_FunctionGeneratorCh1_DAC_Cfg);
	DAC_Config_v(&L_FunctionGeneratorCh2_DAC_Cfg);



	DAC_EnableDAC_v(&L_FunctionGeneratorCh1_DAC_Cfg);
	DAC_EnableDAC_v(&L_FunctionGeneratorCh2_DAC_Cfg);

	DMA_EnableDMA_v(&L_DMAFunctionGeneratorCh1_DMA_Cfg, DMA1_DAC1_CH_7_S_5);
	DMA_EnableDMA_v(&L_DMAFunctionGeneratorCh2_DMA_Cfg, DMA1_DAC2_CH_7_S_6);

	TIM_StartTimer_v(&L_TIMTriggerCh1_TIM_Cfg);
	TIM_StartTimer_v(&L_TIMTriggerCh2_TIM_Cfg);
}

u16 APP_CalcAngleWithinRange_u16(s16 Arg_PhaseShift_s16){
	s16 L_OutVal_s16 = Arg_PhaseShift_s16;
	while(!((L_OutVal_s16>=0) && (L_OutVal_s16<CONFIG_FUNC_GEN_SAMPLE_POINTS_NUM))){
		if (L_OutVal_s16 >= CONFIG_FUNC_GEN_SAMPLE_POINTS_NUM){
			L_OutVal_s16 -= CONFIG_FUNC_GEN_SAMPLE_POINTS_NUM;
		}
		if (L_OutVal_s16 < 0){
			L_OutVal_s16 += CONFIG_FUNC_GEN_SAMPLE_POINTS_NUM;
		}
	}
	return (u16)L_OutVal_s16;
}

void APP_FuncGenSetSineWave_v(u8 Arg_ChannelNum_u8,f32 Arg_Vmax_f32,u32 Arg_Freguency_u32, s16 Arg_PhaseShift_s16){
	APP_ChangeDACFreq_v(Arg_ChannelNum_u8, Arg_Freguency_u32);

	u16 L_PhaseShift_u16 = APP_CalcAngleWithinRange_u16(Arg_PhaseShift_s16);
	u16 L_idx_u16 = L_PhaseShift_u16;
	u16 L_SamplePointValue_u16;
	for (u16 L_SamplePoint_u16 = 0; L_SamplePoint_u16<CONFIG_FUNC_GEN_SAMPLE_POINTS_NUM; L_SamplePoint_u16++){
		L_SamplePointValue_u16 = (u16)lroundf(DAC_MAX_OUTPUT_VAL*(Arg_Vmax_f32/CONFIG_FUNC_GEN_MAXIMUM_VOLTAGE)*.5*(sin(M_PI*(L_SamplePoint_u16)/180.0)+1));
		L_SamplePointValue_u16 = UTILS_MIN(L_SamplePointValue_u16, DAC_MAX_OUTPUT_VAL);
		DACAPP_OutputValues_u16[Arg_ChannelNum_u8][L_idx_u16]= L_SamplePointValue_u16;
		L_idx_u16++;
		if (CONFIG_FUNC_GEN_SAMPLE_POINTS_NUM == L_idx_u16){
			L_idx_u16 = 0;
		}
	}
}



void APP_FuncGenSetSawtoothWave_v(u8 Arg_ChannelNum_u8,f32 Arg_Vmax_f32,u32 Arg_Freguency_u32, s16 Arg_PhaseShift_s16){
	APP_ChangeDACFreq_v(Arg_ChannelNum_u8, Arg_Freguency_u32);
	u16 L_idx_u16= APP_CalcAngleWithinRange_u16(Arg_PhaseShift_s16);
	u16 L_SamplePointValue_u16;
	for (u16 L_SamplePoint_u16 = 0; L_SamplePoint_u16<CONFIG_FUNC_GEN_SAMPLE_POINTS_NUM; L_SamplePoint_u16++){
		L_SamplePointValue_u16 =lroundf((DAC_MAX_OUTPUT_VAL*L_SamplePoint_u16*CONFIG_FUNC_GEN_MAXIMUM_VOLTAGE*Arg_Vmax_f32)/DAC_MAX_OUTPUT_VAL);
		DACAPP_OutputValues_u16[Arg_ChannelNum_u8][L_idx_u16]= L_SamplePointValue_u16;
		L_idx_u16++;
		if (CONFIG_FUNC_GEN_SAMPLE_POINTS_NUM == L_idx_u16){
			L_idx_u16 = 0;
		}
		else{

		}
	}

}

void APP_FuncGenSetTriangleWave_v(u8 Arg_ChannelNum_u8,f32 Arg_Vmax_f32,u32 Arg_Freguency_u32, s16 Arg_PhaseShift_s16){
	APP_ChangeDACFreq_v(Arg_ChannelNum_u8, Arg_Freguency_u32);
	u16 L_idx_u16= APP_CalcAngleWithinRange_u16(Arg_PhaseShift_s16);
	u16 L_SamplePointValue_u16;
	for (u16 L_SamplePoint_u16 = 0; L_SamplePoint_u16<CONFIG_FUNC_GEN_SAMPLE_POINTS_NUM; L_SamplePoint_u16++){
		if (L_SamplePoint_u16<(CONFIG_FUNC_GEN_SAMPLE_POINTS_NUM/2)){
			L_SamplePointValue_u16 = (DAC_MAX_OUTPUT_VAL*L_SamplePoint_u16*CONFIG_FUNC_GEN_MAXIMUM_VOLTAGE*Arg_Vmax_f32)/DAC_MAX_OUTPUT_VAL;
		}
		else{
			L_SamplePointValue_u16 = DAC_MAX_OUTPUT_VAL;
			L_SamplePointValue_u16 -= (DAC_MAX_OUTPUT_VAL*L_SamplePoint_u16*CONFIG_FUNC_GEN_MAXIMUM_VOLTAGE*Arg_Vmax_f32)/DAC_MAX_OUTPUT_VAL;

		}
		L_SamplePointValue_u16 = UTILS_MIN(L_SamplePointValue_u16, DAC_MAX_OUTPUT_VAL);
		DACAPP_OutputValues_u16[Arg_ChannelNum_u8][L_idx_u16]= L_SamplePointValue_u16;
		L_idx_u16++;
		if (CONFIG_FUNC_GEN_SAMPLE_POINTS_NUM == L_idx_u16){
			L_idx_u16 = 0;
		}
		else{

		}
	}

}


void APP_FuncGenSetSquareWave_v(u8 Arg_ChannelNum_u8,f32 Arg_Vmax_f32,f32 Arg_DutyCycle, u16 Arg_Freguency_u16, s16 Arg_PhaseShift_s16){
	u16 L_OutRegVal_u16;
	u16 L_idx_u16;
	u16 L_FallingEdgeSamplePointIdx_u16;

	/*Calculate Voltage */
	L_OutRegVal_u16 = (u16)lroundf((Arg_Vmax_f32/CONFIG_FUNC_GEN_MAXIMUM_VOLTAGE)*DAC_MAX_OUTPUT_VAL);
	L_OutRegVal_u16 = UTILS_MIN(L_OutRegVal_u16, DAC_MAX_OUTPUT_VAL);
	/*Set Phase Shift index*/
	L_idx_u16 = APP_CalcAngleWithinRange_u16(Arg_PhaseShift_s16);


	L_FallingEdgeSamplePointIdx_u16 = (u16)lroundf(Arg_DutyCycle*CONFIG_FUNC_GEN_SAMPLE_POINTS_NUM);

	for (u16 L_SamplePoint_u16 = 0; L_SamplePoint_u16<360; L_SamplePoint_u16++){

		if (L_SamplePoint_u16 > L_FallingEdgeSamplePointIdx_u16){
			DACAPP_OutputValues_u16[Arg_ChannelNum_u8][L_idx_u16] = 0;
		}
		else{
			DACAPP_OutputValues_u16[Arg_ChannelNum_u8][L_idx_u16] = L_OutRegVal_u16;
		}
		L_idx_u16++;
		if (CONFIG_FUNC_GEN_SAMPLE_POINTS_NUM == L_idx_u16){
			L_idx_u16 = 0;
		}
	}
}

static u16 APP_CalcCorrespondARRValToFreq(u32 Arg_Freq_u32){
	return (u16)(1000000*RCC_GetAPB1TimerClockFrequency_f32()/((f32)CONFIG_FUNC_GEN_SAMPLE_POINTS_NUM*Arg_Freq_u32))-1;
}

static void APP_ChangeDACFreq_v(u8 Arg_ChannelNum_u8, u32 Arg_Freq_u32){
	TIM_Cfg L_TIMTriggerChx_TIM_Cfg = {0};
	L_TIMTriggerChx_TIM_Cfg.AutoReloadValue = APP_CalcCorrespondARRValToFreq(Arg_Freq_u32);
	L_TIMTriggerChx_TIM_Cfg.MasterMode = TIM_MASTER_MODE_UPDATE;
	L_TIMTriggerChx_TIM_Cfg.Prescaler = 0;
	if (DAC_CHANNEL_1 == Arg_ChannelNum_u8){
		L_TIMTriggerChx_TIM_Cfg.InstanceId = CONFIG_DAC_CHANNEL_1_TIMER_INSTANCE;
	}
	else if (DAC_CHANNEL_2 == Arg_ChannelNum_u8){
		L_TIMTriggerChx_TIM_Cfg.InstanceId = CONFIG_DAC_CHANNEL_2_TIMER_INSTANCE;
	}
	else{

	}
	TIM_StopTimer_v(&L_TIMTriggerChx_TIM_Cfg);
	TIM_Config_v(&L_TIMTriggerChx_TIM_Cfg);
	TIM_StartTimer_v(&L_TIMTriggerChx_TIM_Cfg);
}

void ExecuteCommmand(void){
	u8 L_Command_u8 = G_FunctionGeneratorDataFrame.Command;
	u8 L_ChannelNum_u8 = G_FunctionGeneratorDataFrame.ChannelNumber-1;

	switch (L_Command_u8){
	case UI_Send_CMD_NO_CMD:
		break;
	case UI_Send_CMD_Configure_Phaseshift:
		func_chx[L_ChannelNum_u8].phaseShift = G_FunctionGeneratorDataFrame.Value;
		break;
	case UI_Send_CMD_Configure_Frequency:
		func_chx[L_ChannelNum_u8].frequency = G_FunctionGeneratorDataFrame.Value;
		break;
	case UI_Send_CMD_Configure_Amplitude:
		func_chx[L_ChannelNum_u8].amplitude = G_FunctionGeneratorDataFrame.Value*APP_CONFIG_OSCILLOSCOPE_Voltage_REFERENCE/APP_CONFIG_OSCILLOSCOPE_RESOLUTION;
		break;
	case UI_Send_CMD_Configure_DutyCycle:
		func_chx[L_ChannelNum_u8].dutyCycle = G_FunctionGeneratorDataFrame.Value;
		break;
	case UI_Send_CMD_Configure_WaveType:
		func_chx[L_ChannelNum_u8].WaveType = G_FunctionGeneratorDataFrame.Value;
		break;
	case UI_Send_CMD_Apply_Configurations:
		switch(func_chx[L_ChannelNum_u8].WaveType){
		case Wave_DisableChannel:
			APP_FuncGenSetSquareWave_v(L_ChannelNum_u8, 0, 0, func_chx[L_ChannelNum_u8].frequency, func_chx[L_ChannelNum_u8].phaseShift);
			break;
		case Wave_SineWave:
			APP_FuncGenSetSineWave_v(L_ChannelNum_u8, func_chx[L_ChannelNum_u8].amplitude,func_chx[L_ChannelNum_u8].frequency, func_chx[L_ChannelNum_u8].phaseShift);
			break;
		case Wave_TriangleWave:
			APP_FuncGenSetTriangleWave_v(L_ChannelNum_u8, func_chx[L_ChannelNum_u8].amplitude,func_chx[L_ChannelNum_u8].frequency, func_chx[L_ChannelNum_u8].phaseShift);
			break;
		case Wave_SawToothWave:
			APP_FuncGenSetSawtoothWave_v(L_ChannelNum_u8, func_chx[L_ChannelNum_u8].amplitude,func_chx[L_ChannelNum_u8].frequency, func_chx[L_ChannelNum_u8].phaseShift);
			break;
		case Wave_SquareWave:
			APP_FuncGenSetSquareWave_v(L_ChannelNum_u8, func_chx[L_ChannelNum_u8].amplitude, func_chx[L_ChannelNum_u8].dutyCycle/100, func_chx[L_ChannelNum_u8].frequency, func_chx[L_ChannelNum_u8].phaseShift);
			break;
		}

		break;
	};
}
