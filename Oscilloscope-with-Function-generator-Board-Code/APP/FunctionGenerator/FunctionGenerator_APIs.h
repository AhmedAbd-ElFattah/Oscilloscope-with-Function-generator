/*
 * File Name --> FunctionGenerator_APIs.h
 * Author	 --> Ahmed Mohamed Abd-Elfattah
 * Version	 --> 1.0.0
 * Layer	 --> Application
 * Brief	 --> Function Generator APIs
 * */
#ifndef APP_FUNCTIONGENERATOR_FUNCTIONGENERATOR_APIS_H_
#define APP_FUNCTIONGENERATOR_FUNCTIONGENERATOR_APIS_H_




/*
==================================================
  Start Section --> File Includes
==================================================
*/
#include "LIB/STD_TYPES.h"
#include "LIB/BIT_MATH.h"

/*
==================================================
  End   Section --> File Includes
==================================================
*/

/*
==================================================
  Start Section --> APIs
==================================================
*/

void APP_FunctionGeneratorInit_v(void);

void APP_FuncGenSetSineWave_v(u8 Arg_ChannelNum_u8,f32 Arg_Vmax_f32,u32 Arg_Freguency_u32, s16 Arg_PhaseShift_s16);

void APP_FuncGenSetSawtoothWave_v(u8 Arg_ChannelNum_u8,f32 Arg_Vmax_f32,u32 Arg_Freguency_u32, s16 Arg_PhaseShift_s16);

void APP_FuncGenSetTriangleWave_v(u8 Arg_ChannelNum_u8,f32 Arg_Vmax_f32,u32 Arg_Freguency_u32, s16 Arg_PhaseShift_s16);

void APP_FuncGenSetSquareWave_v(u8 Arg_ChannelNum_u8,f32 Arg_Vmax_f32,f32 Arg_DutyCycle, u16 Arg_Freguency_u16, s16 Arg_PhaseShift_s16);


/*
==================================================
  End   Section --> APIs
==================================================
*/
#endif /* APP_FUNCTIONGENERATOR_FUNCTIONGENERATOR_APIS_H_ */
