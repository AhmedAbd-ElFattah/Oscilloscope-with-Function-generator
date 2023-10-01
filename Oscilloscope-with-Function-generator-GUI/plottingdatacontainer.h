#ifndef PLOTTINGDATACONTAINER_H
#define PLOTTINGDATACONTAINER_H

#include <QSharedPointer>
#include <QVector>
#include "qcustomplot.h"
#include "common.h"

class PlottingDataContainer
{
private:
    short int idx=0;
    const short int BufferLength = APP_CONFIG_OSCILLOSCOPE_Y_LENGTH;
    const double Resolution = APP_CONFIG_OSCILLOSCOPE_RESOLUTION;
    const double ReferenceVoltage = APP_CONFIG_OSCILLOSCOPE_Voltage_REFERENCE;

    QSharedPointer<QCPGraphDataContainer> dataContainer;
    double ConvertDataToVoltage(int val);
public:
    PlottingDataContainer();
    QSharedPointer<QCPGraphDataContainer> getDataContainer();
    void addData(int voltage_sample_read);
};

extern PlottingDataContainer VoltagePlottingData;

#endif // PLOTTINGDATACONTAINER_H
