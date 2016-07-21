#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QSerialPort>

#include "GUIWidgets/qneedleindicator.h"
#include "GUIWidgets/thermometer.h"
#include "GUIWidgets/led.h"
#include "GUIWidgets/chart.h"
#include "GUIWidgets/chart/datacontainers.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    void openSerialPort(const QString &port);
    void closeSerialPort();

    ~MainWindow();

private:
    void initActionsConnections();
    void initGUIWidgets();
    void initCharts(Chart* widget);
    void fillChartData();
    QList<QString> getSerialPortNames();

    Ui::MainWindow *ui;

    // Widgets
    QNeedleIndicator *wTankPressure;
    QNeedleIndicator *wValvePressure;
    ThermoMeter *wTankTemperature;
    ThermoMeter *wNozzle1Temperature;
    ThermoMeter *wNozzle2Temperature;
    ThermoMeter *wNozzle3Temperature;
    ThermoMeter *wNozzle4Temperature;
    ThermoMeter *wPDUTemperature;
    Led *wValveRP;
    Led *wValveRM;
    Led *wTestMode;
    Chart *wChart;

    // QAction *menuSendCommand;
    QMenu *menuSendCommand;
    QMenu *menuSelectCOM;
    QMenu *menuConnect;
    QList<QAction*> menuConnectCOMS;
    QAction *menuDisconnect;

    // SerialSettings *settings;
    QSerialPort *serial;
    QLabel *status;

    int currentPort;

    QFile file;



public slots:
    void serialConnect();
    void serialDisconnect();
    void sendCommand();
    void writeData(const QByteArray &data);
    void readData();
    void handleError(QSerialPort::SerialPortError error);

};

#endif // MAINWINDOW_H
