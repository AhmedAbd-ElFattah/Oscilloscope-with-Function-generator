#ifndef COMMUNICATION_H
#define COMMUNICATION_H
/*
 * File Name --> communication.h
 * Author	 --> Ahmed Mohamed Abd-Elfattah
 * Version	 --> 1.0.0
 * Brief	 --> Communication with board.
 * */
#include <QObject>
#include <QSerialPort>
#include <QDebug>
#include <QPlainTextEdit>
#include <QTextCursor>
#include <QThread>
#include "plottingdatacontainer.h"
#include "functiongeneratorchannel.h"
#include "common.h"

class Communication : public QObject
{
    Q_OBJECT
private:
    QSerialPort Board_Serial_Port;
    int CurrentVoltageData = 0;
    int Received_data_Counter = 0;
    char CurrentReceivedData=0;

private slots:
    void Read_Data();
public:
    bool IsConnected(void);
    Communication();
    void Connect(QString Arg_PortName);
    void SendData(quint8 Arg_Command, quint8 Arg_Channel, quint16 Arg_Value);
    void SendData(FunctionGeneratorChannel &func_chx);
    void DisConnect();
    ~Communication();
signals:


};

#endif // COMMUNICATION_H

