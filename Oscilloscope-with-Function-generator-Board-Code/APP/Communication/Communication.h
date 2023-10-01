/*
 * File Name --> Communication.h
 * Author	 --> Ahmed Mohamed Abd-Elfattah
 * Version	 --> 1.0.0
 * Layer	 --> Application
 * Brief	 --> Communication APIs
 * */

#ifndef COMMUNICATION_COMMUNICATION_H_
#define COMMUNICATION_COMMUNICATION_H_

#include "HMCAL/DMA/DMA.h"
#include "HMCAL/USART/USART.h"
#include "Oscilloscope/Oscilloscope_Config.h"
#include "FunctionGenerator/FunctionGenerator_Config.h"
#include "Oscilloscope/Oscilloscope_Private.h"
#include "common.h"

extern DMA_Cfg G_PCUSARTDMA_DMA_Cfg;
extern USART_Cfg G_PCUSART_USART_Cfg;

void APP_CommunicationInit_v(void);

#endif /* COMMUNICATION_COMMUNICATION_H_ */
