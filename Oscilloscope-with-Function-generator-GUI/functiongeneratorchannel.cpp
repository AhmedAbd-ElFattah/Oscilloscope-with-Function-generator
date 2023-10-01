#include "functiongeneratorchannel.h"

FunctionGeneratorChannel::FunctionGeneratorChannel(short int Arg_ChannelIdx):ChannelIndex(Arg_ChannelIdx)
{

}

short FunctionGeneratorChannel::Get_ChannalNumber()
{
    return ChannelIndex;
}

void FunctionGeneratorChannel::Update_Phaseshift(short Arg_phaseShift)
{
    phaseShift = Arg_phaseShift;
}

short FunctionGeneratorChannel::Get_Phaseshift()
{
    return phaseShift;
}


void FunctionGeneratorChannel::Update_Frequency(short Arg_frequency)
{
    frequency = Arg_frequency;
}

short FunctionGeneratorChannel::Get_Frequency()
{
    return frequency;
}


void FunctionGeneratorChannel::Update_Amplitude(short Arg_amplitude)
{
    amplitude = Arg_amplitude;
}

short FunctionGeneratorChannel::Get_Amplitude()
{
    return amplitude;
}

void FunctionGeneratorChannel::Update_DutyCycle(short Arg_dutyCycle)
{
    dutyCycle = Arg_dutyCycle;
}

short FunctionGeneratorChannel::Get_DutyCycle()
{
    return dutyCycle;
}

void FunctionGeneratorChannel::Update_waveType(WaveType_enum Arg_waveType)
{
    waveType = Arg_waveType;
}

short FunctionGeneratorChannel::Get_waveType()
{
    return waveType;
}

