#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "commandwindow.h"

#include <list>
#include <vector>

#include <QMessageBox>
#include <QObject>

#include "windows.h"

typedef vector<int> vint;
typedef list <int>  lint;
typedef list <double> ldouble;
typedef list< pair<double,double> > lpair;

lpair position;
vint  velocity;
vint  times;
ldouble rates;

void fillChartData();
void initCharts(Chart* widget);

const double IMU1 = 12.23;
const double IMU2 = 13.12;
const double X1 = 3.41;
const double X2 = 3.32;
const double Y1 = 5.12;
const double Y2 = 4.99;
const double Z1 = 15.23;
const double Z2 = 15.35;
const double PRES_TANK = 196.6;
const double PRES_PREV = 3.04;
const double PRES_RP = 2.96;
const double PRES_RM = 2.95;
const double TEMP_TANK = 21.3;
const double TEMP_NOZ1 = 16.5;
const double TEMP_NOZ2 = 14.6;
const double TEMP_NOZ3 = 15.3;
const double TEMP_NOZ4 = 16.3;
const double TEMP_PDU = 33.4;


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    menuSend_Command = ui->menuBar->addAction("Send Command");
    connect(menuSend_Command, SIGNAL(triggered()), this, SLOT(sendCommand()));
    setWindowState(windowState() | Qt::WindowMaximized);
    wTankPressure = new QNeedleIndicator(this);
    wPreValvesPressure = new QNeedleIndicator(this);
    wNozzlesRPPressure = new QNeedleIndicator(this);
    wNozzlesRMPressure = new QNeedleIndicator(this);
    wTankTemperature = new ThermoMeter(this);
    wNozzle1Temperature = new ThermoMeter(this);
    wNozzle2Temperature = new ThermoMeter(this);
    wNozzle3Temperature = new ThermoMeter(this);
    wNozzle4Temperature = new ThermoMeter(this);
    wPDUTemperature = new ThermoMeter(this);
    valve1 = new Led(this);
    valve2 = new Led(this);
    wChart = new Chart(this);

    wTankPressure->setRange(0, 300);
    wTankPressure->setMajorTicks(7);
    wTankPressure->setMinorTicks(4);
    wTankPressure->setLabel(QString("Tank Pressure"));
    wTankPressure->setLabelOffset(0.75);
    wTankPressure->setLabelFont(QFont("Fixed", 10, QFont::Bold));
    wTankPressure->setDigitFont(QFont("Fixed", 10));
    wTankPressure->setDigitFormat("%.0f");
    wTankPressure->setAnimated(false);
    //wTankPressure->setGapAngle(200);

    wPreValvesPressure->setRange(0, 10);
    wPreValvesPressure->setMajorTicks(11);
    wPreValvesPressure->setMinorTicks(4);
    wPreValvesPressure->setLabel(QString("Prev Pressure"));
    wPreValvesPressure->setLabelOffset(0.75);
    wPreValvesPressure->setLabelFont(QFont("Fixed", 10, QFont::Bold));
    wPreValvesPressure->setDigitFont(QFont("Fixed", 10));
    wPreValvesPressure->setDigitFormat("%.1f");
    wPreValvesPressure->setAnimated(false);

    wNozzlesRPPressure->setRange(0, 10);
    wNozzlesRPPressure->setMajorTicks(11);
    wNozzlesRPPressure->setMinorTicks(4);
    wNozzlesRPPressure->setLabel(QString("Roll+ Pressure"));
    wNozzlesRPPressure->setLabelOffset(0.75);
    wNozzlesRPPressure->setLabelFont(QFont("Fixed", 10, QFont::Bold));
    wNozzlesRPPressure->setDigitFont(QFont("Fixed", 10));
    wNozzlesRPPressure->setDigitFormat("%.1f");
    wNozzlesRPPressure->setAnimated(false);

    wNozzlesRMPressure->setRange(0, 10);
    wNozzlesRMPressure->setMajorTicks(11);
    wNozzlesRMPressure->setMinorTicks(4);
    wNozzlesRMPressure->setLabel(QString("Roll- Pressure"));
    wNozzlesRMPressure->setLabelOffset(0.75);
    wNozzlesRMPressure->setLabelFont(QFont("Fixed", 10, QFont::Bold));
    wNozzlesRMPressure->setDigitFont(QFont("Fixed", 10));
    wNozzlesRMPressure->setDigitFormat("%.1f");
    wNozzlesRMPressure->setAnimated(false);

    wTankTemperature->setSuffix(QString("°C"));
    wTankTemperature->setMaximum(50);
    wTankTemperature->setMinimum(-30);

    wNozzle1Temperature->setSuffix(QString("°C"));
    wNozzle1Temperature->setMaximum(50);
    wNozzle1Temperature->setMinimum(-30);

    wNozzle2Temperature->setSuffix(QString("°C"));
    wNozzle2Temperature->setMaximum(50);
    wNozzle2Temperature->setMinimum(-30);

    wNozzle3Temperature->setSuffix(QString("°C"));
    wNozzle3Temperature->setMaximum(50);
    wNozzle3Temperature->setMinimum(-30);

    wNozzle4Temperature->setSuffix(QString("°C"));
    wNozzle4Temperature->setMaximum(50);
    wNozzle4Temperature->setMinimum(-30);

    wPDUTemperature->setSuffix(QString("°C"));
    wPDUTemperature->setMaximum(50);
    wPDUTemperature->setMinimum(-30);

    valve2->setColor(Qt::green);


    fillChartData();
    initCharts(wChart);
    wChart->setSize(450);
    //wChart->setBaseSize(160, 60);
    //wChart->adjustSize();

    //wChart->baseSize().setHeight(500);

    //char test[256];
    //sprintf(test, "%d", wChart->isEnabled());

    //MessageBoxA(0, test, "Msg", MB_OK)


    ui->lcdNumber->display(IMU1);
    ui->lcdNumber_7->display(IMU2);
    ui->lcdNumber_2->display(X1);
    ui->lcdNumber_5->display(X2);
    ui->lcdNumber_3->display(Y1);
    ui->lcdNumber_6->display(Y2);
    ui->lcdNumber_4->display(Z1);
    ui->lcdNumber_8->display(Z2);

    ui->lcdNumber_9->display(PRES_TANK);
    wTankPressure->setValue(PRES_TANK);

    ui->lcdNumber_10->display(PRES_PREV);
    wPreValvesPressure->setValue(PRES_PREV);

    ui->lcdNumber_11->display(PRES_RP);
    wNozzlesRPPressure->setValue(PRES_RP);

    ui->lcdNumber_12->display(PRES_RM);
    wNozzlesRMPressure->setValue(PRES_RM);

    wTankTemperature->setValue(TEMP_TANK);
    wNozzle1Temperature->setValue(TEMP_NOZ1);
    wNozzle2Temperature->setValue(TEMP_NOZ2);
    wNozzle3Temperature->setValue(TEMP_NOZ3);
    wNozzle4Temperature->setValue(TEMP_NOZ4);
    wPDUTemperature->setValue(TEMP_PDU);

    ui->gridLayout->addWidget(wTankPressure);
    ui->gridLayout_2->addWidget(wPreValvesPressure);
    ui->gridLayout_3->addWidget(wTankTemperature);
    ui->gridLayout_4->addWidget(wChart);
    ui->gridLayout_5->addWidget(wNozzlesRMPressure);
    ui->gridLayout_6->addWidget(wNozzlesRPPressure);
    ui->gridLayout_7->addWidget(wNozzle1Temperature);
    ui->gridLayout_8->addWidget(wNozzle2Temperature);
    ui->gridLayout_9->addWidget(wNozzle3Temperature);
    ui->gridLayout_10->addWidget(wNozzle4Temperature);
    ui->gridLayout_11->addWidget(wPDUTemperature);
    ui->gridLayout_12->addWidget(valve1);
    ui->gridLayout_13->addWidget(valve2);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::sendCommand()
{
    commandWindow *w = new commandWindow();
    w->show();
}


void fillChartData()
{
    double curRate = 26.7;
    double randf;
    for (int i = 0; i < 200; i++)
    {
        times.push_back(i);
        randf = 0.001 * ((rand() % 101) - 50);
        if (i > 50 && i < 300)
            randf -= 0.1;
        curRate += randf;
        rates.push_back(curRate);
    }
}


void initCharts(Chart* widget)
{
  QPen vPen;
  //vPen.setColor(Qt::red);
  vPen.setWidthF(2.0);
  vPen.setColor(Qt::green);
  Channel cAngRates (-50,50,new DoubleDataContainer<vint,ldouble>(times,rates), "Angular rate",vPen);
  cAngRates.setShowScale(true);
  //cAngRates.setMaximum(50);
  //cAngRates.setMinimum(-50);
  //cisnienie3.setShowScale(false);
//pozycja.showScale = predkosc.showScale = false ;
//chart->scaleGrid().showScale=false;
//chart->scaleGrid().showGrid=false;
  widget->addChannel(cAngRates);
  /*widget->addChannel(cisnienie3);
  widget->addChannel(cisnienie);*/
//    chart->addChannel(pozycja);
//    chart->addChannel(predkosc);

}
