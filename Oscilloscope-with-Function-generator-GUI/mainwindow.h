#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "availableboards.h"
#include "communication.h"
#include "plottingdatacontainer.h"
#include "functiongeneratorchannel.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_connect_pushButton_clicked();

    void on_RefreshAvailableBoards_pushButton_clicked();

    void on_Wave_Ch1_comboBox_currentIndexChanged(int index);

    void on_Wave_Ch2_comboBox_currentIndexChanged(int index);

    void on_Amplitude_Ch1_doubleSpinBox_valueChanged(double arg1);

    void on_Amplitude_Ch2_doubleSpinBox_valueChanged(double arg1);

    void on_Frequency_Ch1_spinBox_valueChanged(int arg1);

    void on_Frequency_Ch2_spinBox_valueChanged(int arg1);

    void on_PhaseShift_Ch1_spinBox_valueChanged(int arg1);

    void on_PhaseShift_Ch2_spinBox_valueChanged(int arg1);

    void on_DutyCycle_Ch1_spinBox_valueChanged(int arg1);

    void on_DutyCycle_Ch2_spinBox_valueChanged(int arg1);

    void on_ApplyFunctionGeneratorConfigurations_pushButton_clicked();

    void on_Vmax_doubleSpinBox_valueChanged(double arg1);

private:
    FunctionGeneratorChannel func_ch1 = FunctionGeneratorChannel(1);
    FunctionGeneratorChannel func_ch2 = FunctionGeneratorChannel(2);
    /*This timer is responsible
     * for taking snapshot for PlottingDataContainer
     * And plot samples.
*/
    QTimer *SnapshotPlottingTimer = new QTimer(this);
    void PlottingInit();
    void UpdatePlotting();
    Communication CommuncationWithBoard;
    AvailableBoards *BoardSelector = nullptr;
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
