#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QSerialPort>

#include <Qt3DCore>
#include <Qt3DRender>
#include <Qt3DInput>
#include <Qt3DLogic>
#include <Qt3DExtras>

#include "GUIWidgets/qneedleindicator.h"
#include "GUIWidgets/thermometer.h"
#include "GUIWidgets/led.h"
#include "GUIWidgets/chart.h"
#include "GUIWidgets/chart/datacontainers.h"

#include "packages.h"
#include "murmur.h"
#include "rocket.h"

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
    void decodeIMU(const dpIMU *dataIMU);
    void decodePT(const dpPresTemp *dataPT);
    void decodeCalc(const dpCalc *dataCalc);


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

    // 3d stuff
    Qt3DExtras::Qt3DWindow *view;
    QWidget *container;
    Qt3DInput::QInputAspect *input;
    Qt3DCore::QEntity *rootEntity;
    Qt3DRender::QCamera *cameraEntity;
    Rocket *rocket;

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
