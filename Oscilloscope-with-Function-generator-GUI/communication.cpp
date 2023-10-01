#include "communication.h"

Communication::Communication()
{
    Board_Serial_Port.setBaudRate(QSerialPort::BaudRate::Baud115200);
    Board_Serial_Port.setParity(QSerialPort::Parity::NoParity);
    Board_Serial_Port.setDataBits(QSerialPort::DataBits::Data8);
    Board_Serial_Port.setFlowControl(QSerialPort::FlowControl::NoFlowControl);
    Board_Serial_Port.setStopBits(QSerialPort::StopBits::OneStop);

}

void Communication::Connect(QString Arg_PortName)
{
    if (Board_Serial_Port.isOpen()){
        Board_Serial_Port.close();
    }
    connect(&Board_Serial_Port, SIGNAL(readyRead()), this, SLOT(Read_Data()));
    Board_Serial_Port.setPortName(Arg_PortName);
    Board_Serial_Port.open(QIODevice::ReadWrite);

}


void Communication::SendData(FunctionGeneratorChannel &func_chx)
{
    FunctionGeneratorDataFrame dataFrame;
    /*Send wave type*/
    dataFrame.Command = UI_Send_CMD_Configure_WaveType;
    dataFrame.ChannelNumber = func_chx.Get_ChannalNumber();
    dataFrame.Value = func_chx.Get_waveType();
    Board_Serial_Port.write((char *)&dataFrame.Buffer, sizeof(dataFrame.Buffer));
    Board_Serial_Port.waitForBytesWritten();

    /*Send Duty Cycle*/
    dataFrame.Command = UI_Send_CMD_Configure_DutyCycle;
    dataFrame.ChannelNumber = func_chx.Get_ChannalNumber();
    dataFrame.Value = func_chx.Get_DutyCycle();
    Board_Serial_Port.write((char *)&dataFrame.Buffer, sizeof(dataFrame.Buffer));
    Board_Serial_Port.waitForBytesWritten();

    /*Send Amplitude*/
    dataFrame.Command = UI_Send_CMD_Configure_Amplitude;
    dataFrame.ChannelNumber = func_chx.Get_ChannalNumber();
    dataFrame.Value = func_chx.Get_Amplitude();
    Board_Serial_Port.write((char *)&dataFrame.Buffer, sizeof(dataFrame.Buffer));
    Board_Serial_Port.waitForBytesWritten();

    /*Send Frequency*/
    dataFrame.Command = UI_Send_CMD_Configure_Frequency;
    dataFrame.ChannelNumber = func_chx.Get_ChannalNumber();
    dataFrame.Value = func_chx.Get_Frequency();
    Board_Serial_Port.write((char *)&dataFrame.Buffer, sizeof(dataFrame.Buffer));
    Board_Serial_Port.waitForBytesWritten();

    /*Send Phaseshift*/
    dataFrame.Command = UI_Send_CMD_Configure_Phaseshift;
    dataFrame.ChannelNumber = func_chx.Get_ChannalNumber();
    dataFrame.Value = func_chx.Get_Phaseshift();
    Board_Serial_Port.write((char *)&dataFrame.Buffer, sizeof(dataFrame.Buffer));
    Board_Serial_Port.waitForBytesWritten();


    /*Apply configurations*/
    dataFrame.Command = UI_Send_CMD_Apply_Configurations;
    dataFrame.ChannelNumber = func_chx.Get_ChannalNumber();
    dataFrame.Value = 0;
    Board_Serial_Port.write((char *)&dataFrame.Buffer, sizeof(dataFrame.Buffer));
    Board_Serial_Port.waitForBytesWritten();

}

void Communication::SendData(quint8 Arg_Command, quint8 Arg_Channel, quint16 Arg_Value)
{
    Board_Serial_Port.write((char *)&Arg_Command, sizeof(Arg_Command));
    Board_Serial_Port.waitForBytesWritten();
    Board_Serial_Port.write((char *)&Arg_Channel,    sizeof(Arg_Channel));
    Board_Serial_Port.waitForBytesWritten();
    Board_Serial_Port.write((char *)&Arg_Value,    sizeof(Arg_Value));
    Board_Serial_Port.waitForBytesWritten();
}

void Communication::DisConnect()
{
    if (Board_Serial_Port.isOpen()){
        SendData((quint8)UI_Send_CMD_Disconnect, 0, 0);
        Board_Serial_Port.close();
    }
}

void Communication::Read_Data()
{
    qint16 CurrentVoltageData;
    while (Board_Serial_Port.bytesAvailable() >= 2)
    {
        QByteArray data = Board_Serial_Port.read(2);
        if (data.size() == 2)
        {
            CurrentVoltageData = static_cast<qint16>((static_cast<quint8>(data[0])) | (static_cast<quint8>(data[1]) << 8));
            VoltagePlottingData.addData(CurrentVoltageData);
        }
    }
}


bool Communication::IsConnected()
{
    return Board_Serial_Port.isOpen();
}

Communication::~Communication()
{
    if (Board_Serial_Port.isOpen()){
        Board_Serial_Port.close();
    }
}




