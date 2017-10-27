#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QSerialPort>
#include <QFileDialog>
#include <QLineEdit>

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
#include "verticallabel.h"

enum Mode {IDLE, COM_CONNECTED, HISFILE_OPENED};

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    bool openSerialPort(const QString &port);
    void closeSerialPort();

    ~MainWindow();

private:
    uint16_t ringBufIMU[20];
    int rbIMUPos, rbIMUSize;
    uint16_t ringBufPT[5];
    int rbPTPos, rbPTSize;
    uint16_t ringBufCalc[2];
    int rbCalcPos, rbCalcSize;
    int lostPacksIMU;
    int lostPacksPT;
    int lostPacksCalc;
    void initActionsConnections();
    void initGUIWidgets();
    void initCharts(Chart* widget);
    void fillChartData();
    void setCriticalColor(QLineEdit *textEdit);
    QString decodeModulState(uint16_t code);
    QString decodeMode(uint8_t code);
    void setTime(uint32_t time);

    QDir curSDDir;
    QFile curSDFile;
    uint32_t currentSDPack;
    bool sdDLinProgress;
    bool decodeSDDownlink(const void *dataPck);

    //curSDDir.mkdir("test");
    //curSDDir.

    bool decodeIMU(const dpIMU *dataIMU);
    bool decodePT(const dpPresTemp *dataPT);
    bool decodeCalc(const dpCalc *dataCalc);
    //bool decodeObcIMU(const ObcIMUdata *dataIMU);
    //bool decodeObcHk(const ObcHkData *dataHk);
    //bool decodeObcCmd(const ObcCmdData *dataCmd);
    bool decodeDLDebug(const DpDebug *dataDebug);
    void readGSFile(QByteArray &input);
    //void readOBCFile(QByteArray &input);
    void resetStateList();
    void saveState();
    void loadState(int index);
    void applyCurrentState();

    QList<QString> getSerialPortNames();

    Ui::MainWindow *ui;

    vector<double> times;
    vector<double> rates;

    vector<double> times_all;
    vector<double> rates_all;

    Mode gsMode;
    State currentState;
    QList<State> states;

    // Widgets
    QNeedleIndicator *wTankPressure;
    QNeedleIndicator *wValvePressure;
    ThermoMeter *wTankTemperature;
    ThermoMeter *wPDUTemperature;
    ThermoMeter *wValveRPTemperature;
    ThermoMeter *wValveRMTemperature;
    ThermoMeter *wValveECTemperature;
    ThermoMeter *wNozzle1Temperature;
    ThermoMeter *wNozzle2Temperature;
    Led *wValveRP;
    Led *wValveRM;
    Led *wValveEC;
    Led *wTestMode;
    Led *wLO;
    Led *wSOE;
    Led *wSODS;
    Led *wDCDC;
    Chart *wChart;
    Channel *cAngRates;

    // 3d stuff
    Qt3DExtras::Qt3DWindow *view;
    QWidget *container;
    Qt3DInput::QInputAspect *input;
    Qt3DCore::QEntity *rootEntity;
    Qt3DRender::QCamera *cameraEntity;
    Rocket *rocket;
    QPropertyAnimation *rollAnimation;
    QPropertyAnimation *yawAnimation;
    QPropertyAnimation *pitchAnimation;

    // QAction *menuSendCommand;
    QMenu *menuSendCommand;
    QMenu *menuSelectCOM;
    QMenu *menuConnect;
    QList<QAction*> menuConnectCOMS;
    QAction *menuDisconnect;
    QMenu *menuLoad;
    QAction *loadGSFile;
    QAction *loadOBCFile;

    // SerialSettings *settings;
    QSerialPort *serial;
    QLabel *status;
    QLabel *lbPackLoss;

    VerticalLabel *lbTankTemp;
    VerticalLabel *lbPDUTemp;
    VerticalLabel *lbValveRPTemp;
    VerticalLabel *lbValveRMTemp;
    VerticalLabel *lbValveECTemp;
    VerticalLabel *lbNoz1Temp;
    VerticalLabel *lbNoz2Temp;

    int currentPort;

    QFile file;
    double temperatureMinimum;

    double countdownOffset;
    bool lo_received;



public slots:
    void serialConnect();
    void serialDisconnect();
    void sendCommand();
    void sendCommand(QString &str);
    void writeData(const QByteArray &data);
    void readData();
    void handleError(QSerialPort::SerialPortError error);
    void openFileDialog();

private slots:
    void on_stateSilder_valueChanged(int value);
    void on_buttonGraphReset_clicked();
    void on_leCommand_returnPressed();
    void on_pbSendCommand_clicked();
};

#endif // MAINWINDOW_H
