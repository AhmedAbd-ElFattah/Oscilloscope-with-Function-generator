#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    BoardSelector = new AvailableBoards(ui->AvailableBoards_comboBox);

    PlottingInit();
    connect(SnapshotPlottingTimer, &QTimer::timeout, this, &MainWindow::UpdatePlotting);
    SnapshotPlottingTimer->setInterval(APP_CONFIG_OSCILLOSCOPE_SNAPSHOT_TIME_IN_MS);
    SnapshotPlottingTimer->start();
}

MainWindow::~MainWindow()
{
    if (nullptr != SnapshotPlottingTimer){
        delete SnapshotPlottingTimer;
        SnapshotPlottingTimer = nullptr;
    }
    delete ui;
}


void MainWindow::on_connect_pushButton_clicked()
{
    if (!CommuncationWithBoard.IsConnected()){
        QString PortName = BoardSelector->GetPortName();
        if ("" == PortName){
            return;
        }

        CommuncationWithBoard.Connect(PortName);
        if (!CommuncationWithBoard.IsConnected()){
            QMessageBox::critical(nullptr, "Connection failed !",
                                  "Connection failed !, please try to refresh and connection again");
            on_RefreshAvailableBoards_pushButton_clicked();
            return ;
        }

        ui->connect_pushButton->setText("Dis-Connect");

    }
    else{

        CommuncationWithBoard.DisConnect();
        ui->connect_pushButton->setText("Connect");


    }
}


void MainWindow::on_RefreshAvailableBoards_pushButton_clicked()
{
    BoardSelector->Refresh();
}

void MainWindow::UpdatePlotting()
{
    ui->graphPlot_QCustomPlot->graph(0)->setData(VoltagePlottingData.getDataContainer());
    ui->graphPlot_QCustomPlot->replot();
    ui->graphPlot_QCustomPlot->update();
}

void MainWindow::PlottingInit()
{
    ui->graphPlot_QCustomPlot->addGraph();
    ui->graphPlot_QCustomPlot->graph(0)->setLineStyle(QCPGraph::lsLine);
    ui->graphPlot_QCustomPlot->xAxis->setRange(0, APP_CONFIG_OSCILLOSCOPE_Y_LENGTH);
    ui->graphPlot_QCustomPlot->yAxis->setRange(0, APP_CONFIG_OSCILLOSCOPE_Voltage_REFERENCE);

    for (int i =0; i<1000; i++){
        VoltagePlottingData.addData(i*4);
    }
    ui->graphPlot_QCustomPlot->graph(0)->setData(VoltagePlottingData.getDataContainer());

    ui->graphPlot_QCustomPlot->replot();
    ui->graphPlot_QCustomPlot->update();
}


void MainWindow::on_Wave_Ch1_comboBox_currentIndexChanged(int index)
{
    func_ch1.Update_waveType((WaveType_enum) index);
}


void MainWindow::on_Wave_Ch2_comboBox_currentIndexChanged(int index)
{
    func_ch2.Update_waveType((WaveType_enum) index);
}


void MainWindow::on_Amplitude_Ch1_doubleSpinBox_valueChanged(double arg1)
{
    func_ch1.Update_Amplitude((arg1/APP_Config_FunctionGenerator_CH1_MaxOutputVoltage)*APP_Config_FunctionGenerator_CH1_MaxVal);
}

void MainWindow::on_Amplitude_Ch2_doubleSpinBox_valueChanged(double arg1)
{
    func_ch2.Update_Amplitude((arg1/APP_Config_FunctionGenerator_CH2_MaxOutputVoltage)*APP_Config_FunctionGenerator_CH2_MaxVal);
}


void MainWindow::on_Frequency_Ch1_spinBox_valueChanged(int arg1)
{
    func_ch1.Update_Frequency(arg1);
}


void MainWindow::on_Frequency_Ch2_spinBox_valueChanged(int arg1)
{
    func_ch2.Update_Frequency(arg1);
}


void MainWindow::on_PhaseShift_Ch1_spinBox_valueChanged(int arg1)
{
    func_ch1.Update_Phaseshift(arg1);
}


void MainWindow::on_PhaseShift_Ch2_spinBox_valueChanged(int arg1)
{
    func_ch2.Update_Phaseshift(arg1);
}



void MainWindow::on_DutyCycle_Ch1_spinBox_valueChanged(int arg1)
{
    func_ch1.Update_DutyCycle(arg1);
}



void MainWindow::on_DutyCycle_Ch2_spinBox_valueChanged(int arg1)
{
    func_ch2.Update_DutyCycle(arg1);
}


void MainWindow::on_ApplyFunctionGeneratorConfigurations_pushButton_clicked()
{
    if (CommuncationWithBoard.IsConnected()){
        CommuncationWithBoard.SendData(func_ch1);
        QThread::msleep(100);
        CommuncationWithBoard.SendData(func_ch2);

        /*Change applied configure value for channel 1*/
        ui->WaveCurrentSetting_Ch1_label->setText("Wave = " + ui->Wave_Ch1_comboBox->currentText());
        ui->AmplitudeCurrentSetting_Ch1_label->setText("Amplitude = " + QString::number(ui->Amplitude_Ch1_doubleSpinBox->value()));
        ui->FrequencyCurrentSetting_Ch1_label->setText("Frequency = " + QString::number(ui->Frequency_Ch1_spinBox->value()));
        ui->PhaseShiftCurrentSetting_Ch1_label->setText("Phase Shift = " + QString::number(ui->PhaseShift_Ch1_spinBox->value()));
        ui->DutyCycleCurrentSetting_Ch1_label->setText("Duty Cycle = " + QString::number(ui->DutyCycle_Ch1_spinBox->value()));

        /*Change applied configure value for channel 2*/
        ui->WaveCurrentSetting_Ch2_label->setText("Wave = " + ui->Wave_Ch2_comboBox->currentText());
        ui->AmplitudeCurrentSetting_Ch2_label->setText("Amplitude = " + QString::number(ui->Amplitude_Ch2_doubleSpinBox->value()));
        ui->FrequencyCurrentSetting_Ch2_label->setText("Frequency = " + QString::number(ui->Frequency_Ch2_spinBox->value()));
        ui->PhaseShiftCurrentSetting_Ch2_label->setText("Phase Shift = " + QString::number(ui->PhaseShift_Ch2_spinBox->value()));
        ui->DutyCycleCurrentSetting_Ch2_label->setText("Duty Cycle = " + QString::number(ui->DutyCycle_Ch2_spinBox->value()));

    }
}


void MainWindow::on_Vmax_doubleSpinBox_valueChanged(double arg1)
{
    ui->graphPlot_QCustomPlot->yAxis->setRange(0, arg1);
}

