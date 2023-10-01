#include "plottingdatacontainer.h"

double PlottingDataContainer::ConvertDataToVoltage(int val)
{
    return (double)val*ReferenceVoltage / Resolution;
}

PlottingDataContainer::PlottingDataContainer() :dataContainer(new QCPGraphDataContainer())
{

}

QSharedPointer<QCPGraphDataContainer> PlottingDataContainer::getDataContainer()
{
    return dataContainer;
}

void PlottingDataContainer::addData(int voltage_sample_read)
{
    idx = idx%BufferLength;
    if (0 == idx){
        dataContainer->clear();
    }
    dataContainer->add(QCPGraphData(idx, ConvertDataToVoltage(voltage_sample_read)));
    ++idx;
}

PlottingDataContainer VoltagePlottingData;
