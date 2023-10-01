#ifndef COMMON_H
#define COMMON_H
/*
 * File Name --> common.h
 * Author	 --> Ahmed Mohamed Abd-Elfattah
 * Version	 --> 1.0.0
 * Brief	 --> Contain common structure of send/receive data of serial communication and
 *               order of shown elements in gui
 * Note      --> This file is common between GUI & Microcontroller.It's portable file between C & C++.
 * */

#ifdef __cplusplus
#include <QtGlobal>
#include <array>
#include "ApplicationConfigurations.h"

#else
#include "LIB/STD_TYPES.h"
#endif

/*
==================================================
  Start Section --> Common configurations
==================================================
*/
/*
 * Configure buffer size of function generator serial port.
*/
#define APP_CONFIG_FUNCTION_GENERATOR_BUFFER_SIZE           4

#define APP_CONFIG_OSCILLOSCOPE_Voltage_REFERENCE           3.2
#define APP_CONFIG_OSCILLOSCOPE_RESOLUTION                  4095
/*
==================================================
  End Section --> Common configurations
==================================================
*/

/*
==================================================
  Start Section --> Common datatypes
==================================================
*/
/*Note For portability of file,
 * any primitive datatype used in this file must use these only*/

#ifdef __cplusplus

#define COMMON_u8   quint8
#define COMMON_u16  quint16
#define COMMON_u32  quint32
#define COMMON_u64  quint64

#define COMMON_s8   qint8
#define COMMON_s16  qint16
#define COMMON_s32  qint32
#define COMMON_s64  qint64

#else
#include "LIB/STD_TYPES.h"

#define COMMON_u8   u8
#define COMMON_u16  u16
#define COMMON_u32  u32
#define COMMON_u64  u64

#define COMMON_s8   s8
#define COMMON_s16  s16
#define COMMON_s32  s32
#define COMMON_s64  s64

#endif

/*
==================================================
  End Section --> Common datatypes
==================================================
*/

/*
==================================================
  Start Section --> Communication structure
==================================================
*/

typedef enum WaveType_enum{
    Wave_DisableChannel,
    Wave_SineWave,
    Wave_TriangleWave,
    Wave_SawToothWave,
    Wave_SquareWave,
}WaveType_enum;

typedef enum UI_Send_Commands{
    UI_Send_CMD_NO_CMD,
    UI_Send_CMD_Configure_Phaseshift,
    UI_Send_CMD_Configure_Frequency,
    UI_Send_CMD_Configure_Amplitude,
    UI_Send_CMD_Configure_DutyCycle,
    UI_Send_CMD_Configure_WaveType,
    UI_Send_CMD_Apply_Configurations,
    UI_Send_CMD_Disconnect,
}UI_Send_Commands;

union FunctionGeneratorDataFrame{
    struct {
        COMMON_u8   Command;
        COMMON_u8   ChannelNumber;
        COMMON_u16  Value;
    };
#ifdef __cplusplus
    std::array<COMMON_u8, APP_CONFIG_FUNCTION_GENERATOR_BUFFER_SIZE> Buffer;
#else
    COMMON_u8 Buffer [APP_CONFIG_FUNCTION_GENERATOR_BUFFER_SIZE];
#endif
};
/*
==================================================
  End Section --> Communication structure
==================================================
*/
#endif // COMMON_H
