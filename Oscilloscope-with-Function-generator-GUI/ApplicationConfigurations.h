#ifndef APPLICATIONCONFIGURATIONS_H
#define APPLICATIONCONFIGURATIONS_H

/*
 * File Name --> Configurations.h
 * Author	 --> Ahmed Mohamed Abd-Elfattah
 * Version	 --> 1.0.0
 * Brief	 --> It includes configuration of application.
 * */

#define APP_Config_FunctionGenerator_CH1_MaxVal                 4065
#define APP_Config_FunctionGenerator_CH1_MaxOutputVoltage       3.2

#define APP_Config_FunctionGenerator_CH2_MaxVal                 4065
#define APP_Config_FunctionGenerator_CH2_MaxOutputVoltage       3.2

#define APP_CONFIG_OSCILLOSCOPE_SNAPSHOT_TIME_IN_MS             2

#define APP_CONFIG_OSCILLOSCOPE_Y_LENGTH                        1000
/*
 * USB Board configurations
 * To avoid confliction between target board & other connected devices.
*/
#define APP_CONFIG_Board_VendorId      4292
#define APP_CONFIG_Board_ProductId     60000
#define APP_CONFIG_Board_Manufacturer  "Silicon Labs"
#define APP_CONFIG_Board_Description   "CP2102 USB to UART Bridge Controller"

#endif // APPLICATIONCONFIGURATIONS_H
