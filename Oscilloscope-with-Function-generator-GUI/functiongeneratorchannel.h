/*
 * File Name --> functiongeneratorchannel.h
 * Author	 --> Ahmed Mohamed Abd-Elfattah
 * Version	 --> 1.0.0
 * Brief	 --> FunctionGeneratorChannel stores function generator channel properities.
 */

#ifndef FUNCTIONGENERATORCHANNEL_H
#define FUNCTIONGENERATORCHANNEL_H

#include "common.h"

class FunctionGeneratorChannel
{
private:
    short int phaseShift;
    short int frequency;
    short int amplitude;
    short int dutyCycle;
    const short int ChannelIndex;
    WaveType_enum waveType;
public:
    FunctionGeneratorChannel(short int Arg_ChannelIdx);
    short int Get_ChannalNumber();

    void Update_Phaseshift(short int Arg_phaseShift);
    short int Get_Phaseshift();

    void Update_Frequency(short int Arg_frequency);
    short int Get_Frequency();

    void Update_Amplitude(short int Arg_amplitude);
    short int Get_Amplitude();

    void Update_DutyCycle(short int Arg_dutyCycle);
    short int Get_DutyCycle();

    void Update_waveType(WaveType_enum Arg_waveType);
    short int Get_waveType();

};

#endif // FUNCTIONGENERATORCHANNEL_H
