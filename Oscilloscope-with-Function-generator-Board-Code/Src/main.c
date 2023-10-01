#include "LIB/utils.h"

#include "FunctionGenerator/FunctionGenerator_APIs.h"
#include "Oscilloscope/Oscilloscope_APIs.h"

#include "Communication/Communication.h"


int main(void)
{
	APP_CommunicationInit_v();
	APP_OscilloscopeInit_v();
	APP_FunctionGeneratorInit_v();

	while(1){

	}
}



















