#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "commandwindow.h"

#include <list>
#include <vector>

#include <QMessageBox>
#include <QObject>
#include <QDateTime>
#include <QSerialPortInfo>

#define GYRO_FACTOR         0.00762939453125    // Max. Scale: 250 deg/s
#define ACC_FACTOR          0.00048828125       // Max. Scale: 16g
//#define ACC_FACTOR          0.00006103515625
#define TEMP_FACTOR         0.0625
#define PRES_HIGH_FACTOR	0.009695752453125 // bar/bit
#define PRES_HIGH_OFFSET	24.841810875 // bar
#define PRES_LOW_FACTOR		0.0008079796875 // bar/bit
#define PRES_LOW_OFFSET		2.1546125 // bar

typedef vector<int> vint;
typedef list <int>  lint;
typedef list <double> ldouble;
typedef list< pair<double,double> > lpair;

lpair position;
vint  velocity;
vint  times;
ldouble rates;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowState(windowState() | Qt::WindowMaximized);

    // menuSendCommand = ui->menuBar->addAction("Send Command");
    // connect(menuSendCommand, SIGNAL(triggered()), this, SLOT(sendCommand()));

    menuSendCommand = ui->menuBar->addMenu("Send Command");

    QList<QString> teleCommands;
    teleCommands.append("DELETE_SD");
    teleCommands.append("SHUTDOWN");
    teleCommands.append("RESTART");
    teleCommands.append("MODE_STANDBY");
    teleCommands.append("MODE_FLIGHT");
    teleCommands.append("MODE_SECURE");
    teleCommands.append("MODE_BD");
    teleCommands.append("TEST_ACTIVATE");
    teleCommands.append("TEST_LEAVE");

    foreach (const QString &name, teleCommands)
    {
       QAction *tmp = menuSendCommand->addAction(name);
       connect(tmp, SIGNAL(triggered()), this, SLOT(sendCommand()));
    }

    menuSelectCOM = ui->menuBar->addMenu("Serial COM");
    menuConnect = menuSelectCOM->addMenu("Connect");

    menuDisconnect = menuSelectCOM->addAction("Disconnect");
    connect(menuDisconnect, SIGNAL(triggered()), this, SLOT(serialDisconnect()));

    QList<QString> serialPortNames = getSerialPortNames();
    foreach (const QString &name, serialPortNames)
    {
        menuConnectCOMS.append(menuConnect->addAction(name));
        connect(menuConnectCOMS.last(), SIGNAL(triggered()), this, SLOT(serialConnect()));
        menuConnectCOMS.last()->setCheckable(true);
    }

    status = new QLabel;
    ui->statusBar->addWidget(status);
    currentPort = -1;
    status->setText("Disconnected");

    serial = new QSerialPort(this);

    connect(serial, SIGNAL(error(QSerialPort::SerialPortError)), this, SLOT(handleError(QSerialPort::SerialPortError)));
    connect(serial, SIGNAL(readyRead()), this, SLOT(readData()));
    // connect(console, SIGNAL(getData(QByteArray)), this, SLOT(writeData(QByteArray)));


    ui->menuBar->addAction("Load file");

    //initActionsConnections();
    initGUIWidgets();

    /*QMessageBox msgBox;
    msgBox.setText(QString::asprintf("%d", sizeof(dpIMU) ));
    msgBox.exec();*/


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::sendCommand()
{
    //commandWindow *w = new commandWindow(this);
    //w->show();
    static int counter = 0;

    if (!serial->isOpen())
        return;

    QAction *command = (QAction*)sender();

    dpCommand comPack;
    comPack.sync = SYNC_COMM;

    if (command->text() == "DELETE_SD")
    {
        comPack.id = DELETE_SD;
    }
    else if (command->text() == "SHUTDOWN")
    {
        comPack.id = SHUTDOWN;
    }
    else if (command->text() == "RESTART")
    {
        comPack.id = RESTART;
    }
    else if (command->text() == "MODE_STANDBY")
    {
        comPack.id = MODE_STANDBY;
    }
    else if (command->text() == "MODE_FLIGHT")
    {
        comPack.id = MODE_FLIGHT;
    }
    else if (command->text() == "MODE_SECURE")
    {
        comPack.id = MODE_SECURE;
    }
    else if (command->text() == "MODE_BD")
    {
        comPack.id = MODE_BD;
    }
    else if (command->text() == "TEST_ACTIVATE")
    {
        comPack.id = TEST_ACTIVATE;
    }
    else if (command->text() == "TEST_LEAVE")
    {
        comPack.id = TEST_LEAVE;
    }

    comPack.counter = counter;
    counter++;

    comPack.check = Murmur::mm_hash_32((uint8_t*)&comPack, 4); //TODO

    QByteArray commArray;
    QDataStream stream(&commArray, QIODevice::WriteOnly);
    stream << comPack.sync << comPack.id << comPack.counter << comPack.check;

    qDebug("Sync: %X\nID: %X\n", commArray.at(0) & 0xFF, commArray.at(1) & 0xFF);

    writeData(commArray);

}

void MainWindow::serialConnect()
{
    QAction *com = (QAction*)sender();
    int port = menuConnectCOMS.indexOf(com);

    if (port == currentPort)
        return;

    if (currentPort != -1)
    {
       closeSerialPort();
       menuConnectCOMS.at(currentPort)->setChecked(false);
       if(file.isOpen())
           file.close();
    }

    openSerialPort(com->text());

    currentPort = port;

    status->setText("Connected to Port " + com->text());

    QDateTime curTime = QDateTime::currentDateTime();


    file.setFileName("telemetry_" + com->text() + "_" + curTime.toString("dd-MM-yyyy-hh-mm-ss") + ".bin");

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QMessageBox msgBox;
        msgBox.setText("File " + file.fileName() + " not opened.");
        msgBox.exec();
    }
    else
    {
        QMessageBox msgBox;
        msgBox.setText("File " + file.fileName() + " opened.");
        msgBox.exec();
    }

}

void MainWindow::serialDisconnect()
{
    if (!serial->isOpen())
        return;
    closeSerialPort();
    menuConnectCOMS.at(currentPort)->setChecked(false);
    currentPort = -1;
    file.close();
    status->setText("Disconnected");
}

void MainWindow::openSerialPort(const QString &port)
{
    if (serial->isOpen())
        return;
    serial->setPortName(port);
    serial->setBaudRate(QSerialPort::Baud115200);
    serial->open(QIODevice::ReadWrite);
}

void MainWindow::closeSerialPort()
{
    if (serial->isOpen())
        serial->close();
    // TODO
}

void MainWindow::writeData(const QByteArray &data)
{
    serial->write(data);
}

void MainWindow::decodeIMU(const dpIMU *dataIMU)
{
    static int graph_index = 0;

    uint32_t checksum = Murmur::mm_hash_32((uint8_t*)dataIMU, sizeof(dpIMU) - 4);

    if (checksum != dataIMU->check)
        return;

    ui->lcdIMU1roll->display(dataIMU->imu1_roll * GYRO_FACTOR);
    ui->lcdIMU1yaw->display(dataIMU->imu1_yaw * GYRO_FACTOR);
    ui->lcdIMU1pitch->display(dataIMU->imu1_pitch * GYRO_FACTOR);
    ui->lcdIMU2roll->display(dataIMU->imu2_roll * GYRO_FACTOR);
    ui->lcdIMU2yaw->display(dataIMU->imu2_yaw * GYRO_FACTOR);
    ui->lcdIMU2pitch->display(dataIMU->imu2_pitch * GYRO_FACTOR);
    ui->lcdIMU1accx->display(dataIMU->imu1_accx * ACC_FACTOR);
    ui->lcdIMU1accy->display(dataIMU->imu1_accy * ACC_FACTOR);
    ui->lcdIMU1accz->display(dataIMU->imu1_accz * ACC_FACTOR);
    ui->lcdIMU2accx->display(dataIMU->imu2_accx * ACC_FACTOR);
    ui->lcdIMU2accy->display(dataIMU->imu2_accy * ACC_FACTOR);
    ui->lcdIMU2accz->display(dataIMU->imu2_accz * ACC_FACTOR);

    double roll_rate = ((double)dataIMU->imu1_roll + (double)dataIMU->imu2_roll)*GYRO_FACTOR/2;

    times.push_back(graph_index++);
    rates.push_back(roll_rate);

    if (file.isOpen())
    {
        QDataStream out(&file);
        out << dataIMU->sync
        //<< ((*(uint32_t*)dataIMU->time) & 0xFFFFFF);
        << dataIMU->time[0]
        << dataIMU->time[1]
        << dataIMU->time[2]
        << dataIMU->counter
        << dataIMU->imu1_roll
        << dataIMU->imu1_yaw
        << dataIMU->imu1_pitch
        << dataIMU->imu1_accx
        << dataIMU->imu1_accy
        << dataIMU->imu1_accz
        << dataIMU->imu2_roll
        << dataIMU->imu2_yaw
        << dataIMU->imu2_pitch
        << dataIMU->imu2_accx
        << dataIMU->imu2_accy
        << dataIMU->imu2_accz
        << dataIMU->check
        << "\n";
    }
}

void MainWindow::decodePT(const dpPresTemp *dataPT)
{
    uint32_t checksum = Murmur::mm_hash_32((uint8_t*)dataPT, sizeof(dpPresTemp) - 4);

    if (checksum != dataPT->check)
        return;

    ui->lcdTankPres->display(dataPT->presTank * PRES_HIGH_FACTOR - PRES_HIGH_OFFSET);
    wTankPressure->setValue(dataPT->presTank * PRES_HIGH_FACTOR - PRES_HIGH_OFFSET);

    ui->lcdValvePres->display(dataPT->presValves * PRES_LOW_FACTOR - PRES_LOW_OFFSET);
    wValvePressure->setValue(dataPT->presValves * PRES_LOW_FACTOR - PRES_LOW_OFFSET);

    wTankTemperature->setValue(dataPT->tempTank * TEMP_FACTOR);
    wNozzle1Temperature->setValue(dataPT->tempNoz1 * TEMP_FACTOR);
    wNozzle2Temperature->setValue(dataPT->tempNoz2 * TEMP_FACTOR);
    wNozzle3Temperature->setValue(dataPT->tempNoz3 * TEMP_FACTOR);
    wNozzle4Temperature->setValue(dataPT->tempNoz4 * TEMP_FACTOR);
    wPDUTemperature->setValue(dataPT->tempPDU * TEMP_FACTOR);

    if (file.isOpen())
    {
        QDataStream out(&file);
        out << dataPT->sync
        << dataPT->time[0]
        << dataPT->time[1]
        << dataPT->time[2]
        << dataPT->counter
        << dataPT->presTank
        << dataPT->presValves
        << dataPT->tempTank
        << dataPT->tempNoz1
        << dataPT->tempNoz2
        << dataPT->tempNoz3
        << dataPT->tempNoz4
        << dataPT->tempPDU
        << dataPT->check;
    }
}

void MainWindow::decodeCalc(const dpCalc *dataCalc)
{
    uint32_t checksum = Murmur::mm_hash_32((uint8_t*)dataCalc, sizeof(dpCalc) - 4);

    if (checksum != dataCalc->check)
        return;

    if (dataCalc->valveState & 0b1)
    {
        wValveRM->setColor(Qt::green);
    }
    else
    {
        wValveRM->setColor(Qt::red);
    }

    if (dataCalc->valveState & 0b10)
    {
        wValveRP->setColor(Qt::green);
    }
    else
    {
        wValveRP->setColor(Qt::red);
    }

    switch (dataCalc->mode & 0x7F)
    {
    case MODE_STANDBY:
        ui->txtMode->setPlainText("Stand-by-mode");
        break;
    case MODE_FLIGHT:
        ui->txtMode->setPlainText("Flight-mode");
        break;
    case MODE_SECURE:
        ui->txtMode->setPlainText("Secure-mode");
        break;
    case MODE_BD:
        ui->txtMode->setPlainText("Blowdown-mode");
        break;
    }

    if (dataCalc->mode & 0x80)
        wTestMode->setColor(Qt::green);
    else
        wTestMode->setColor(Qt::red);

    if (file.isOpen())
    {
        QDataStream out(&file);
        out << dataCalc->sync
        << dataCalc->time[0]
        << dataCalc->time[1]
        << dataCalc->time[2]
        << dataCalc->counter
        << dataCalc->mode
        << dataCalc->modulStates
        << dataCalc->vot1
        << dataCalc->vot2
        << dataCalc->valveState
        << dataCalc->check;
    }
}

void MainWindow::readData()
{
    //static int graph_index = 0;
    //QByteArray data = serial->readAll();
    /*QMessageBox msgBox;
    msgBox.setText(data);
    msgBox.exec();*/


    static dpIMU dataIMU;
    static dpPresTemp dataPT;
    static dpCalc dataCalc;

    //qDebug("AB: %i\n", serial->bytesAvailable());

    static int package_selected = -1;
    static int package_index = 0;

    uint8_t c;
    while(serial->getChar((char*)&c))
    {
        //qDebug("%X ", c);
        if (package_selected == -1)
        {
            //qDebug("Nothing selected");
            if (c == SYNC_IMU)
            {
                package_selected = SYNC_IMU;
                ((uint8_t*)&dataIMU)[package_index++] = c;
            }
            else if (c == SYNC_PT)
            {
                package_selected = SYNC_PT;
                ((uint8_t*)&dataPT)[package_index++] = c;
            }
            else if (c == SYNC_CALC)
            {
                package_selected = SYNC_CALC;
                ((uint8_t*)&dataCalc)[package_index++] = c;
            }
        }
        else if (package_selected == SYNC_IMU)
        {
            ((uint8_t*)&dataIMU)[package_index++] = c;
            if (package_index >= sizeof(dpIMU))
            {
                decodeIMU(&dataIMU);
                package_selected = -1;
                package_index = 0;
            }
        }
        else if (package_selected == SYNC_PT)
        {
            ((uint8_t*)&dataPT)[package_index++] = c;
            if (package_index >= sizeof(dpPresTemp))
            {
                decodePT(&dataPT);
                package_selected = -1;
                package_index = 0;
            }
        }
        else if (package_selected == SYNC_CALC)
        {
            ((uint8_t*)&dataCalc)[package_index++] = c;
            if (package_index >= sizeof(dpCalc))
            {
                decodeCalc(&dataCalc);
                package_selected = -1;
                package_index = 0;
            }
        }
        else
        {
            qDebug("Hier sollte man nie hinkommen...\n");
            package_selected = -1;
            package_index = 0;
        }
    }


    /*
    static int noti = 0;

    qDebug("%i\n",data.size());

    //qDebug("Sizes: %i, %i, %i\n", sizeof(dpIMU), sizeof(dpPresTemp), sizeof(dpCalc));

    for (int i = 0; i < data.size(); i++)
    {
        //qDebug("%X ", (uint8_t)data.at(i));
        if ((uint8_t)data.at(i) == SYNC_IMU && data.size() - i  >= sizeof(dpIMU))
        {
            //qDebug("IMU received %i\n", noti++);
            const dpIMU* dataIMU = (const dpIMU*)(data.constData() + i);

            decodeIMU(dataIMU);

            i += sizeof(dpIMU);
        }
        else if ((uint8_t)data.at(i) == SYNC_PT && data.size() - i >= sizeof(dpPresTemp))
        {
            qDebug("presdata\n");
            const dpPresTemp* dataPT = (const dpPresTemp*)(data.constData() + i);

            decodePT(dataPT);

            i += sizeof(dpPresTemp);
        }
        else if ((uint8_t)data.at(i) == SYNC_CALC && data.size() - i >= sizeof(dpCalc))
        {
            const dpCalc* dataCalc = (const dpCalc*)(data.constData() + i);

            decodeCalc(dataCalc);

            i += sizeof(dpCalc);
        }  

    }
    */

}

void MainWindow::handleError(QSerialPort::SerialPortError error)
{
    if (error == QSerialPort::ResourceError) {
        QMessageBox::critical(this, tr("Critical Error"), serial->errorString());
        closeSerialPort();
    }
}

QList<QString> MainWindow::getSerialPortNames()
{
    QList<QSerialPortInfo> ports = QSerialPortInfo::availablePorts();
    QList<QString> portNames;
    foreach (const QSerialPortInfo &portInfo, ports)
    {
        portNames.append(portInfo.portName());
    }
    return portNames;
}

void MainWindow::initActionsConnections()
{
    /*connect(ui->actionConnect, &QAction::triggered, this, &MainWindow::openSerialPort);
    connect(ui->actionDisconnect, &QAction::triggered, this, &MainWindow::closeSerialPort);
    connect(ui->actionQuit, &QAction::triggered, this, &MainWindow::close);
    connect(ui->actionConfigure, &QAction::triggered, settings, &MainWindow::show);
    connect(ui->actionClear, &QAction::triggered, console, &Console::clear);
    connect(ui->actionAbout, &QAction::triggered, this, &MainWindow::about);
    connect(ui->actionAboutQt, &QAction::triggered, qApp, &QApplication::aboutQt);*/
}

void MainWindow::initGUIWidgets()
{
    // Test Values
    /*const double IMU1 = 12.23;
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
    const double TEMP_PDU = 33.4;*/

    wTankPressure = new QNeedleIndicator(this);
    wValvePressure = new QNeedleIndicator(this);
    wTankTemperature = new ThermoMeter(this);
    wNozzle1Temperature = new ThermoMeter(this);
    wNozzle2Temperature = new ThermoMeter(this);
    wNozzle3Temperature = new ThermoMeter(this);
    wNozzle4Temperature = new ThermoMeter(this);
    wPDUTemperature = new ThermoMeter(this);
    wValveRP = new Led(this);
    wValveRM = new Led(this);
    wTestMode = new Led(this);
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
    wTankPressure->setValue(0);

    wValvePressure->setRange(0, 10);
    wValvePressure->setMajorTicks(11);
    wValvePressure->setMinorTicks(4);
    wValvePressure->setLabel(QString("Prev Pressure"));
    wValvePressure->setLabelOffset(0.75);
    wValvePressure->setLabelFont(QFont("Fixed", 10, QFont::Bold));
    wValvePressure->setDigitFont(QFont("Fixed", 10));
    wValvePressure->setDigitFormat("%.1f");
    wValvePressure->setAnimated(false);
    wValvePressure->setValue(0);

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

    //wValveRM->setColor(Qt::green);


    //fillChartData();
    initCharts(wChart);
    wChart->setSize(450);
    //wChart->setBaseSize(160, 60);
    //wChart->adjustSize();

    //wChart->baseSize().setHeight(500);

    //char test[256];
    //sprintf(test, "%d", wChart->isEnabled());

    //MessageBoxA(0, test, "Msg", MB_OK)


    /*ui->lcdNumber->display(IMU1);
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
    wValvePressure->setValue(PRES_PREV);

    wTankTemperature->setValue(TEMP_TANK);
    wNozzle1Temperature->setValue(TEMP_NOZ1);
    wNozzle2Temperature->setValue(TEMP_NOZ2);
    wNozzle3Temperature->setValue(TEMP_NOZ3);
    wNozzle4Temperature->setValue(TEMP_NOZ4);
    wPDUTemperature->setValue(TEMP_PDU);*/

    ui->gridTankPres->addWidget(wTankPressure);
    ui->gridValvePres->addWidget(wValvePressure);
    ui->gridTankTemp->addWidget(wTankTemperature);
    ui->gridGraph->addWidget(wChart);
    ui->gridNoz1Temp->addWidget(wNozzle1Temperature);
    ui->gridNoz2Temp->addWidget(wNozzle2Temperature);
    ui->gridNoz3Temp->addWidget(wNozzle3Temperature);
    ui->gridNoz4Temp->addWidget(wNozzle4Temperature);
    ui->gridPDUTemp->addWidget(wPDUTemperature);
    ui->gridValveRP->addWidget(wValveRP);
    ui->gridValveRM->addWidget(wValveRM);
    ui->gridTestMode->addWidget(wTestMode);

    ui->txtMode->setDisabled(true);
    ui->txtMode->zoomIn(4);
    ui->txtMode->setPlainText("Stand-by-mode");

    ui->txtIMUState->setDisabled(true);
    ui->txtIMUState->zoomIn(4);
    ui->txtIMUState->setPlainText("ok");

    ui->txtHkState->setDisabled(true);
    ui->txtHkState->zoomIn(4);
    ui->txtHkState->setPlainText("ok");

    ui->txtControlState->setDisabled(true);
    ui->txtControlState->zoomIn(4);
    ui->txtControlState->setPlainText("ok");

    ui->txtStorageState->setDisabled(true);
    ui->txtStorageState->zoomIn(4);
    ui->txtStorageState->setPlainText("ok");

    ui->txtTMState->setDisabled(true);
    ui->txtTMState->zoomIn(4);
    ui->txtTMState->setPlainText("ok");
}

void MainWindow::initCharts(Chart* widget)
{
    QPen vPen;
    //vPen.setColor(Qt::red);
    vPen.setWidthF(2.0);
    vPen.setColor(Qt::green);
    Channel cAngRates (-50,50,new DoubleDataContainer<vint,ldouble>(times,rates), "Angular rate (°/s)",vPen);
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

void MainWindow::fillChartData()
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
