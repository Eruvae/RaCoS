#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "qneedleindicator.h"
#include "thermometer.h"
#include "led.h"
#include "chart.h"
#include "chart/datacontainers.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QNeedleIndicator *wTankPressure;
    QNeedleIndicator *wPreValvesPressure;
    QNeedleIndicator *wNozzlesRPPressure;
    QNeedleIndicator *wNozzlesRMPressure;
    ThermoMeter *wTankTemperature;
    ThermoMeter *wNozzle1Temperature;
    ThermoMeter *wNozzle2Temperature;
    ThermoMeter *wNozzle3Temperature;
    ThermoMeter *wNozzle4Temperature;
    ThermoMeter *wPDUTemperature;
    Led *valve1;
    Led *valve2;
    Chart *wChart;

    QAction *menuSend_Command;

public slots:
    void sendCommand();

};

#endif // MAINWINDOW_H
