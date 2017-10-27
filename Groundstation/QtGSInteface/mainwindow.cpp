#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <list>
#include <vector>

#include <QMessageBox>
#include <QObject>
#include <QDateTime>
#include <QSerialPortInfo>
#include <QSizePolicy>
#include <QInputDialog>

//#define ACTIVATE_DEBUG_DL

#define GYRO_FACTOR250      0.00762939453125
#define GYRO_FACTOR500		0.01525878906
#define GYRO_FACTOR1000		0.03051757812
#define GYRO_FACTOR2000		0.06103515625

#define BITS_FS_250DPS              0x00
#define BITS_FS_500DPS              0x08
#define BITS_FS_1000DPS             0x10
#define BITS_FS_2000DPS             0x18

#define ACC_FACTOR          0.00048828125       // Max. Scale: 16g
//#define ACC_FACTOR          0.00006103515625
#define TEMP_FACTOR         0.0625
#define PRES_HIGH_FACTOR	0.15513203925 // bar/bit
#define PRES_HIGH_OFFSET	24.841810875 // bar
#define PRES_LOW_FACTOR		0.012927675 // bar/bit
#define PRES_LOW_OFFSET		2.1546125 // bar
#define DELTA_OMEGA_DIVIDOR 800.0

#define GRAPH_X         100
#define GRAPH_Y_MIN     -50//-150
#define GRAPH_Y_MAX     50//100

QFont font10("Ubuntu Mono", 10, QFont::Bold, false);
QFont font12("Ubuntu Mono", 12, QFont::Bold, false);
QFont font14("Ubuntu Mono", 14, QFont::Bold, false);
QFont font18("Ubuntu Mono", 18, QFont::Bold, false);

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
    teleCommands.append("FORMAT_SD");
    teleCommands.append("MODE_STANDBY");
    teleCommands.append("MODE_FLIGHT");
    teleCommands.append("MODE_SECURE");
    teleCommands.append("MODE_BD");
    teleCommands.append("TEST_ACTIVATE");
    teleCommands.append("TEST_LEAVE");
    teleCommands.append("DOWNLINK_SD");
    teleCommands.append("RESET_SD");
    teleCommands.append("CALIBRATE_IMU");
    teleCommands.append("MODE_SPINUP - Roll-");
    teleCommands.append("MODE_SPINUP - Roll+");
    teleCommands.append("RESET_CONTROL");

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
    status->setFont(font12);
    status->setStyleSheet("QLabel { color : rgb(200, 200, 255) }");
    lbPackLoss = new QLabel;
    lbPackLoss->setFont(font12);
    lbPackLoss->setStyleSheet("QLabel { color : rgb(200, 200, 255) }");
    QWidget *statWidget = new QWidget();
    QGridLayout *gridStatLayout = new QGridLayout(statWidget);
    gridStatLayout->addWidget(status,0,0,1,1,Qt::AlignVCenter | Qt::AlignLeft);
    gridStatLayout->addWidget(lbPackLoss,0,1,1,1,Qt::AlignVCenter | Qt::AlignRight);
    //ui->statusBar->addWidget(status);
    //ui->statusBar->addWidget(lbPackLoss);
    ui->statusBar->addWidget(statWidget, 1);
    currentPort = -1;
    status->setText("Disconnected");
    gsMode = IDLE;
    sdDLinProgress = false;

    serial = new QSerialPort(this);

    connect(serial, SIGNAL(error(QSerialPort::SerialPortError)), this, SLOT(handleError(QSerialPort::SerialPortError)));
    connect(serial, SIGNAL(readyRead()), this, SLOT(readData()));
    // connect(console, SIGNAL(getData(QByteArray)), this, SLOT(writeData(QByteArray)));


    menuLoad = ui->menuBar->addMenu("Load file");
    loadGSFile = menuLoad->addAction("GS-File");
    //loadOBCFile = menuLoad->addAction("OBC-File");
    connect(loadGSFile, SIGNAL(triggered()), this, SLOT(openFileDialog()));
    //connect(loadOBCFile, SIGNAL(triggered()), this, SLOT(openFileDialog()));

    temperatureMinimum = 0;

    countdownOffset = 600;
    lo_received = false;

    rbIMUPos = 0;
    rbIMUSize = 0;
    rbPTPos = 0;
    rbPTSize = 0;
    rbCalcPos = 0;
    rbCalcSize = 0;
    lostPacksIMU = 0;
    lostPacksPT = 0;
    lostPacksCalc = 0;


    //initActionsConnections();
    initGUIWidgets();

    /*QMessageBox msgBox;
    msgBox.setText(QString::asprintf("%d", sizeof(dpIMU) ));
    msgBox.exec();*/


}

MainWindow::~MainWindow()
{
    delete view;
    delete container;
    delete ui;
}

void MainWindow::resetStateList()
{
    states.clear();
    ui->stateSilder->setEnabled(false);
}

void MainWindow::saveState()
{
    states.append(currentState);
}

void MainWindow::loadState(int index)
{
    if (index >= states.length())
        return;
    currentState = states[index];
    applyCurrentState();
}

void MainWindow::setCriticalColor(QLineEdit * txtEdit)
{
    if (txtEdit->text() == "CRITICAL_ERROR"  || txtEdit->text() == "INIT_FAILED")
        txtEdit->setStyleSheet("background-color: rgb(255, 0, 0);color: rgb(255, 255, 255);");
    else if (txtEdit->text() == "MINOR_ERROR")
        txtEdit->setStyleSheet("background-color: rgb(255,255,51);color: rgb(0, 0, 0);");
    else if (txtEdit->text() == "NO_INIT")
        txtEdit->setStyleSheet("background-color: rgb(255,165,0);color: rgb(255, 255, 255);");
    else
        txtEdit->setStyleSheet("background-color: rgb(0, 200, 0);color: rgb(0, 0, 0);");
}

void MainWindow::applyCurrentState()
{
    QString tmp;
    ui->lcdIMU1roll->display(tmp.sprintf("%.1f", currentState.imu1_roll));
    ui->lcdIMU1yaw->display(tmp.sprintf("%.1f", currentState.imu1_yaw));
    ui->lcdIMU1pitch->display(tmp.sprintf("%.1f", currentState.imu1_pitch));
    ui->lcdIMU1accx->display(tmp.sprintf("%.1f", currentState.imu1_accx));
    ui->lcdIMU1accy->display(tmp.sprintf("%.1f", currentState.imu1_accy));
    ui->lcdIMU1accz->display(tmp.sprintf("%.1f", currentState.imu1_accz));
    ui->lcdIMU2roll->display(tmp.sprintf("%.1f", currentState.imu2_roll));
    ui->lcdIMU2yaw->display(tmp.sprintf("%.1f", currentState.imu2_yaw));
    ui->lcdIMU2pitch->display(tmp.sprintf("%.1f", currentState.imu2_pitch));
    ui->lcdIMU2accx->display(tmp.sprintf("%.1f", currentState.imu2_accx));
    ui->lcdIMU2accy->display(tmp.sprintf("%.1f", currentState.imu2_accy));
    ui->lcdIMU2accz->display(tmp.sprintf("%.1f", currentState.imu2_accz));
    ui->lcdIMUfiltered_roll->display(tmp.sprintf("%.1f", currentState.imufiltered_roll));
    ui->lcdIMUfiltered_yaw->display(tmp.sprintf("%.1f", currentState.imufiltered_yaw));
    ui->lcdIMUfiltered_pitch->display(tmp.sprintf("%.1f", currentState.imufiltered_pitch));

    ui->lcdTankPres->display(currentState.presTank);
    wTankPressure->setValue(currentState.presTank);

    ui->lcdValvePres->display(currentState.presValves);
    wValvePressure->setValue(currentState.presValves);

    temperatureMinimum = fmin(temperatureMinimum,
                              fmin(currentState.tempPDU, fmin(currentState.tempValveRP,
                              fmin(currentState.tempValveRM, fmin(currentState.tempValveEC,
                              fmin(currentState.tempNoz1, currentState.tempNoz2))))));

    wTankTemperature->setMinimum(temperatureMinimum);
    wPDUTemperature->setMinimum(temperatureMinimum);
    wValveRPTemperature->setMinimum(temperatureMinimum);
    wValveRMTemperature->setMinimum(temperatureMinimum);
    wValveECTemperature->setMinimum(temperatureMinimum);
    wNozzle1Temperature->setMinimum(temperatureMinimum);
    wNozzle2Temperature->setMinimum(temperatureMinimum);

    wTankTemperature->setValue(currentState.tempTank);
    wPDUTemperature->setValue(currentState.tempPDU);
    wValveRPTemperature->setValue(currentState.tempValveRP);
    wValveRMTemperature->setValue(currentState.tempValveRM);
    wValveECTemperature->setValue(currentState.tempValveEC);
    wNozzle1Temperature->setValue(currentState.tempNoz1);
    wNozzle2Temperature->setValue(currentState.tempNoz2);

    wValveRM->setColor(currentState.valveRM);
    wValveRP->setColor(currentState.valveRP);
    wValveEC->setColor(currentState.valveEC);
    wLO->setColor(currentState.LO);
    wSOE->setColor(currentState.SOE);
    wSODS->setColor(currentState.SODS);
    wDCDC->setColor(currentState.DCDC);

    ui->lcdVOT->display((int)currentState.vot);

    ui->txtMode->setText(currentState.mode);
    wTestMode->setColor(currentState.testMode);

    ui->txtIMUState->setText(currentState.imuState);
    setCriticalColor(ui->txtIMUState);
    ui->txtHkState->setText(currentState.hkState);
    setCriticalColor(ui->txtHkState);
    ui->txtControlState->setText(currentState.controlState);
    setCriticalColor(ui->txtControlState);
    ui->txtStorageState->setText(currentState.storageState);
    setCriticalColor(ui->txtStorageState);
    ui->txtTMState->setText(currentState.tmState);
    setCriticalColor(ui->txtTMState);

    ui->lcdDOStart->display(currentState.deltaOmegaStart);
    ui->lcdDOOperation->display(currentState.deltaOmegaOperation);
    ui->lcdDOShutdown->display(currentState.deltaOmegaShutdown);

    if (lo_received == false && currentState.LO == Qt::green)
    {
        lo_received = true;
        countdownOffset = currentState.time - 1;
    }

    ui->lcdTime->display(currentState.time);
    char sign = currentState.time < countdownOffset ? '-' : '+';
    ui->labelTime->setText(tmp.sprintf("Time: T%c%d", sign, abs((int)(currentState.time - countdownOffset))));

    int packLoss = lostPacksIMU + lostPacksPT + lostPacksCalc;
    QString strPck;
    strPck.sprintf("Package loss: %d packets/s", packLoss);
    lbPackLoss->setText(strPck);

    //if (times.size() != currentState.used_plotpoints - currentState.start_point && currentState.used_plotpoints < times_all.size() && currentState.used_plotpoints >= 0 && currentState.start_point < currentState.used_plotpoints && currentState.start_point >= 0)
    //{
    if (times_all.size() > 0 && rates_all.size() > 0)
    {
        times = vector<double>(times_all.begin() + currentState.start_point, times_all.begin() + currentState.used_plotpoints);
        rates = vector<double>(rates_all.begin() + currentState.start_point, rates_all.begin() + currentState.used_plotpoints);
    }
        //qDebug("%d\n%d\n%d\n\n", times.size(), times_all.size(), currentState.used_plotpoints);
    //}

    if (times.size() > 0)
        wChart->setPosition(*times.begin());

    cAngRates->setMaximum(currentState.plot_max);
    cAngRates->setMinimum(currentState.plot_min);

    wChart->adjustSize();
    wChart->update();

    //qDebug("Duration: %f\n", *(float*)rollAnimation->currentValue().constData());

    //float curRoll = *(float*)rollAnimation->currentValue().constData();
    bool positiveDir = *(float*)rollAnimation->startValue().constData() == 0.0f ? true : false;

    //qDebug("Ani %f, %d, %d\n", curRoll, rollAnimation->duration(), rollAnimation->currentTime());

    float curPercentage = (float)(rollAnimation->currentTime() % rollAnimation->duration()) / (float)rollAnimation->duration();

    if (currentState.imufiltered_roll >= 0 && !positiveDir)
    {
        rollAnimation->setStartValue(0.0f);
        rollAnimation->setEndValue(360.0f);
        curPercentage = 1.0f - curPercentage;
    }
    else if (currentState.imufiltered_roll < 0 && positiveDir)
    {
        rollAnimation->setStartValue(360.0f);
        rollAnimation->setEndValue(0.0f);
        curPercentage = 1.0f - curPercentage;
    }

    if (abs(currentState.imufiltered_roll) > 0.0001)
    {
        //rollAnimation->resume();
        //int time = rollAnimation->currentTime();
        rollAnimation->setDuration(1000.0/(abs(currentState.imufiltered_roll)/360.0));
        rollAnimation->setCurrentTime(curPercentage * rollAnimation->duration());
        //if (positiveDir)
         //   rollAnimation->setCurrentTime(abs(curRoll)/360.f * rollAnimation->duration());
        //else
         //   rollAnimation->setCurrentTime((1.0f - abs(curRoll)/360.0f) * rollAnimation->duration());
        //rollAnimation->setCurrentTime(time);
    }
    else
    {
        //rollAnimation->pause();
        //int time = rollAnimation->currentTime();
        rollAnimation->setDuration(INT_MAX);
        //rollAnimation->setCurrentTime(curRoll/360.f * rollAnimation->duration());
        rollAnimation->setCurrentTime(curPercentage * rollAnimation->duration());
        //rollAnimation->setCurrentTime(time);
    }


}

void MainWindow::sendCommand()
{
    //commandWindow *w = new commandWindow(this);
    //w->show();

    if (!serial->isOpen())
        return;

    QAction *command = (QAction*)sender();

    bool ok;
    QString text = QInputDialog::getText(this, tr("Confirm command"), tr("Repeat command (") + command->text() + "):", QLineEdit::Normal, tr(""), &ok);
    if (!ok || text != command->text())
    {
        ui->leCommandConfirm->setText("Invalid Command!");
        ui->leCommandConfirm->setStyleSheet("background-color: rgb(255, 0, 0);color: rgb(255, 255, 255);");
        return;
    }

    dpCommand comPack;
    comPack.sync = SYNC_COMM;

    if (command->text() == "FORMAT_SD")
    {
        comPack.id = FORMAT_SD;
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
    else if (command->text() == "DOWNLINK_SD")
    {
        comPack.id = DOWNLINK_SD;
        currentSDPack = 0;

        QDateTime curTime = QDateTime::currentDateTime();
        QString dirName = "SD_Backup_" + curTime.toString("dd-MM-yyyy-hh-mm-ss");
        curSDDir = QDir();
        curSDDir.mkdir(dirName);
        curSDDir.setPath(dirName);
        sdDLinProgress = true;
    }
    else if (command->text() == "RESET_SD")
    {
        comPack.id = RESET_SD;
    }
    else if (command->text() == "CALIBRATE_IMU")
    {
        comPack.id = CALIBRATE_IMU;
    }
    else if (command->text() == "MODE_SPINUP - Roll-")
    {
        comPack.id = MODE_SPINUP;
        comPack.argument = -1;
    }
    else if (command->text() == "MODE_SPINUP - Roll+")
    {
        comPack.id = MODE_SPINUP;
        comPack.argument = 1;
    }
    else if (command->text() == "RESET_CONTROL")
    {
        comPack.id = RESET_CONTROL;
    }
    else
    {
        ui->leCommandConfirm->setText("Invalid Command!");
        ui->leCommandConfirm->setStyleSheet("background-color: rgb(255, 0, 0);color: rgb(255, 255, 255);");
        return;
    }

    comPack.check = Murmur::mm_hash_32((uint8_t*)&comPack, sizeof(comPack) - sizeof(comPack.check)); //TODO

    QByteArray commArray;
    QDataStream stream(&commArray, QIODevice::WriteOnly);
    //stream << comPack.sync << comPack.id << comPack.counter << comPack.check;

    for (uint32_t i = 0; i < sizeof(comPack); i++)
        stream << ((uint8_t*)(&comPack))[i];

    qDebug("Sync: %X\nID: %X\n", commArray.at(0) & 0xFF, commArray.at(1) & 0xFF);
    ui->leCommandConfirm->setText("Command: " + command->text() + " sent!");
    ui->leCommandConfirm->setStyleSheet("background-color: rgb(0, 200, 0);color: rgb(0, 0, 0);");

    writeData(commArray);

}

void MainWindow::sendCommand(QString &str)
{
    if (!serial->isOpen())
        return;

    dpCommand comPack;
    comPack.sync = SYNC_COMM;

    if (str == "FORMAT_SD")
    {
        comPack.id = FORMAT_SD;
    }
    else if (str == "MODE_STANDBY")
    {
        comPack.id = MODE_STANDBY;
    }
    else if (str == "MODE_FLIGHT")
    {
        comPack.id = MODE_FLIGHT;
    }
    else if (str == "MODE_SECURE")
    {
        comPack.id = MODE_SECURE;
    }
    else if (str == "MODE_BD")
    {
        comPack.id = MODE_BD;
    }
    else if (str == "TEST_ACTIVATE")
    {
        comPack.id = TEST_ACTIVATE;
    }
    else if (str == "TEST_LEAVE")
    {
        comPack.id = TEST_LEAVE;
    }
    else if (str == "DOWNLINK_SD")
    {
        comPack.id = DOWNLINK_SD;
        currentSDPack = 0;

        QDateTime curTime = QDateTime::currentDateTime();
        QString dirName = "SD_Backup_" + curTime.toString("dd-MM-yyyy-hh-mm-ss");
        curSDDir = QDir();
        curSDDir.mkdir(dirName);
        curSDDir.setPath(dirName);
        sdDLinProgress = true;
    }
    else if (str == "RESET_SD")
    {
        comPack.id = RESET_SD;
    }
    else if (str == "CALIBRATE_IMU")
    {
        comPack.id = CALIBRATE_IMU;
    }
    else if (str == "MODE_SPINUP - Roll-")
    {
        comPack.id = MODE_SPINUP;
        comPack.argument = -1;
    }
    else if (str == "MODE_SPINUP - Roll+")
    {
        comPack.id = MODE_SPINUP;
        comPack.argument = 1;
    }
    else if (str == "RESET_CONTROL")
    {
        comPack.id = RESET_CONTROL;
    }
    else
    {
        ui->leCommandConfirm->setText("Invalid Command!");
        ui->leCommandConfirm->setStyleSheet("background-color: rgb(255, 0, 0);color: rgb(255, 255, 255);");
        return;
    }

    comPack.check = Murmur::mm_hash_32((uint8_t*)&comPack, sizeof(comPack) - sizeof(comPack.check)); //TODO

    QByteArray commArray;
    QDataStream stream(&commArray, QIODevice::WriteOnly);
    //stream << comPack.sync << comPack.id << comPack.counter << comPack.check;

    for (uint32_t i = 0; i < sizeof(comPack); i++)
        stream << ((uint8_t*)(&comPack))[i];

    qDebug("Sync: %X\nID: %X\n", commArray.at(0) & 0xFF, commArray.at(1) & 0xFF);
    ui->leCommandConfirm->setText("Command: " + str + " sent!");
    ui->leCommandConfirm->setStyleSheet("background-color: rgb(0, 200, 0);color: rgb(0, 0, 0);");

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

    if (!openSerialPort(com->text()))
    {
        menuConnectCOMS.at(port)->setChecked(false);
        QMessageBox msgBox;
        msgBox.setText("Connecting to Port " + com->text() + " failed.");
        msgBox.exec();
        return;
    }

    currentPort = port;

    status->setText("Connected to Port " + com->text());
    gsMode = COM_CONNECTED;

    QDateTime curTime = QDateTime::currentDateTime();


    file.setFileName("telemetry_" + curTime.toString("dd-MM-yyyy-hh-mm-ss") + "_" + com->text() + ".bin");


    if (!file.open(QIODevice::WriteOnly))
    {
        QMessageBox msgBox;
        msgBox.setText("File " + file.fileName() + " not opened.");
        msgBox.exec();
    }
    else
    {
        /*QMessageBox msgBox;
        msgBox.setText("File " + file.fileName() + " opened.");
        msgBox.exec();*/
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
    gsMode = IDLE;

}

bool MainWindow::openSerialPort(const QString &port)
{
    if (serial->isOpen())
        return false;
    serial->setPortName(port);
    serial->setBaudRate(QSerialPort::Baud38400);
    return serial->open(QIODevice::ReadWrite);
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

void MainWindow::readGSFile(QByteArray &input)
{
    const dpIMU *dataIMU;
    const dpPresTemp *dataPT;
    const dpCalc *dataCalc;

    resetStateList();

    for (int i = 0; i < input.size(); i++)
    {
        //qDebug("%X: %X\n", i, input.at(i) & 0xFF);

        if ((uint8_t)input.at(i) == SYNC_IMU)
        {
            //qDebug("CheckIMU, %X\n", i);
            dataIMU = (const dpIMU *)(input.constData() + i);
            if (!decodeIMU(dataIMU))
                continue;
            else
            {
                //times_all.push_back(times[times.size() - 1]);
                //rates_all.push_back(rates[rates.size() - 1]);
                saveState();
                i += sizeof(dpIMU) - 1;
            }

        }
        else if ((uint8_t)input.at(i) == SYNC_PT)
        {
            //qDebug("CheckPT, %X\n", i);
            dataPT = (const dpPresTemp *)(input.constData() + i);
            if (!decodePT(dataPT))
                continue;
            else
            {
                saveState();
                i += sizeof(dpPresTemp) - 1;
            }
        }
        else if ((uint8_t)input.at(i) == SYNC_CALC)
        {
            //qDebug("CheckCalc, %X\n", i);
            dataCalc = (const dpCalc *)(input.constData() + i);
            if (!decodeCalc(dataCalc))
                continue;
            else
            {
                saveState();
                i += sizeof(dpCalc) - 1;
            }
        }

    }

    qDebug() << "State count: " << states.size() << "\n";

    ui->stateSilder->setMinimum(0);
    ui->stateSilder->setMaximum(states.size() - 1);
    ui->stateSilder->setTickInterval(1);
    ui->stateSilder->setEnabled(true);
    ui->stateSilder->setValue(ui->stateSilder->maximum());
}

/*
void MainWindow::readOBCFile(QByteArray &input)
{
    const ObcIMUdata *dataIMU;
    const ObcHkData *dataPT;
    const ObcCmdData *dataCalc;

    resetStateList();

    for (int i = 0; i < input.size(); i++)
    {
        //qDebug("%X: %X\n", i, input.at(i) & 0xFF);

        if ((uint8_t)input.at(i) == SYNC_IMU)
        {
            dataIMU = (const ObcIMUdata *)(input.constData() + i);
            if (!decodeObcIMU(dataIMU))
                continue;
            else
            {
                //times_all.push_back(times[times.size() - 1]);
                //rates_all.push_back(rates[rates.size() - 1]);
                saveState();
                i += sizeof(ObcIMUdata) - 1;
            }

        }
        else if ((uint8_t)input.at(i) == SYNC_PT)
        {
            dataPT = (const ObcHkData *)(input.constData() + i);
            if (!decodeObcHk(dataPT))
                continue;
            else
            {
                saveState();
                i += sizeof(ObcHkData) - 1;
            }
        }
        else if ((uint8_t)input.at(i) == SYNC_CALC)
        {
            dataCalc = (const ObcCmdData *)(input.constData() + i);
            if (!decodeObcCmd(dataCalc))
                continue;
            else
            {
                saveState();
                i += sizeof(ObcCmdData) - 1;
            }
        }

    }

    qDebug() << "State count: " << states.size() << "\n";

    ui->stateSilder->setMinimum(0);
    ui->stateSilder->setMaximum(states.size() - 1);
    ui->stateSilder->setTickInterval(1);
    ui->stateSilder->setEnabled(true);
    ui->stateSilder->setValue(ui->stateSilder->maximum());
}
*/

void MainWindow::openFileDialog()
{
    if (serial->isOpen() || gsMode == COM_CONNECTED)
   {
       // show message box
       return;
   }

    if (file.isOpen())
        file.close();

    QString fileName = QFileDialog::getOpenFileName(this, tr("Open history data"), "/", tr("All Files (*)"));
    file.setFileName(fileName);

    if (!file.open(QIODevice::ReadOnly))
   {
       QMessageBox msgBox;
       msgBox.setText("File " + file.fileName() + " not opened.");
       msgBox.exec();
       return;
   }

    status->setText("File " + file.fileName() + " opened.");

    QByteArray input = file.readAll();

    //QAction *command = (QAction*)sender();

    //if (command == loadOBCFile)
    //    readOBCFile(input);
    //else
    readGSFile(input);

}

bool MainWindow::decodeIMU(const dpIMU *dataIMU)
{
    //static int graphIndex = 0; // Remove later, should be time when working

    uint32_t checksum = Murmur::mm_hash_32((uint8_t*)dataIMU, sizeof(dpIMU) - 4);

    if (checksum != dataIMU->check)
        return false;

    double gyro_factor;
    switch (dataIMU->imu_config)
    {
    case BITS_FS_250DPS:
        gyro_factor = GYRO_FACTOR250;
        break;
    case BITS_FS_500DPS:
        gyro_factor = GYRO_FACTOR500;
        break;
    case BITS_FS_1000DPS:
        gyro_factor = GYRO_FACTOR1000;
        break;
    case BITS_FS_2000DPS:
        gyro_factor = GYRO_FACTOR2000;
        break;
    default:
        gyro_factor = GYRO_FACTOR2000;
        break;
    }

    currentState.imu1_pitch = dataIMU->imu1_pitch * gyro_factor;
    currentState.imu1_yaw = dataIMU->imu1_yaw * gyro_factor;
    currentState.imu1_roll = dataIMU->imu1_roll * gyro_factor;
    currentState.imu2_pitch = dataIMU->imu2_pitch * gyro_factor;
    currentState.imu2_yaw = dataIMU->imu2_yaw * gyro_factor;
    currentState.imu2_roll = dataIMU->imu2_roll * gyro_factor;
    currentState.imu1_accx = dataIMU->imu1_accx * ACC_FACTOR;
    currentState.imu1_accy = dataIMU->imu1_accy * ACC_FACTOR;
    currentState.imu1_accz = dataIMU->imu1_accz * ACC_FACTOR;
    currentState.imu2_accx = dataIMU->imu2_accx * ACC_FACTOR;
    currentState.imu2_accy = dataIMU->imu2_accy * ACC_FACTOR;
    currentState.imu2_accz = dataIMU->imu2_accz * ACC_FACTOR;
    currentState.imufiltered_pitch = dataIMU->imufiltered_pitch * gyro_factor;
    currentState.imufiltered_yaw = dataIMU->imufiltered_yaw * gyro_factor;
    currentState.imufiltered_roll = dataIMU->imufiltered_roll * gyro_factor;

    uint32_t currentTime = dataIMU->time[0] | (dataIMU->time[1] << 8) | (dataIMU->time[2] << 16);
    times_all.push_back(/*graphIndex++*/currentTime*0.001);
    rates_all.push_back(currentState.imufiltered_roll);
    currentState.used_plotpoints = times_all.size();

    if (currentState.imufiltered_roll < currentState.plot_min)
        currentState.plot_min = currentState.imufiltered_roll;

    if (currentState.imufiltered_roll > currentState.plot_max)
        currentState.plot_max = currentState.imufiltered_roll;

    if (times_all.size() > 1)
    {
        if (currentTime*0.001 < times_all[times_all.size() - 2])
        {
            currentState.start_point = times_all.size() - 1;
        }
        else
        {
            if (currentState.start_point >= 0 && currentState.start_point < times_all.size())
            {
                while (currentTime*0.001 - times_all[currentState.start_point] > GRAPH_X && currentState.start_point < times_all.size() - 1)
                {
                    currentState.start_point++;
                }
            }
        }
    }

    currentState.time = (double)currentTime / 1000.0;
    //qDebug("CT: %d; %f\n", currentTime, currentState.time);

    ringBufIMU[rbIMUPos++] = dataIMU->counter;
    if (rbIMUSize < 20) rbIMUSize++;
    rbIMUPos %= 20;

    lostPacksIMU = ringBufIMU[(rbIMUPos + rbIMUSize - 1) % 20]  - ringBufIMU[rbIMUPos] - rbIMUSize + 1;

    applyCurrentState();

    /*
    if (currentState.imufiltered_roll >= 0)
        rollAnimation->setDirection(QAbstractAnimation::Forward);
    else
        rollAnimation->setDirection(QAbstractAnimation::Backward);

    if (abs(currentState.imufiltered_roll) > 0.0001)
        rollAnimation->setDuration(1000.0/(abs(currentState.imufiltered_roll)/360.0));
    else
        rollAnimation->setDuration(INT_MAX);
*/

    /*for (int i = 0; i < sizeof(dataIMU); i++)
        qDebug("%x", ((unsigned char*)dataIMU)[i]);

    qDebug("\n");*/

    if (gsMode == COM_CONNECTED && file.isOpen())
    {
        QDataStream out(&file);
        for (int i = 0; i < sizeof(dpIMU); i++)
            out << ((unsigned char*)dataIMU)[i];
    }

    return true;
}

bool MainWindow::decodePT(const dpPresTemp *dataPT)
{
    uint32_t checksum = Murmur::mm_hash_32((uint8_t*)dataPT, sizeof(dpPresTemp) - 4);

    if (checksum != dataPT->check)
        return false;

    if (ui->cbPressureSwap->isChecked())
    {
        currentState.presTank = dataPT->presValves * PRES_HIGH_FACTOR - PRES_HIGH_OFFSET;
        currentState.presValves = dataPT->presTank * PRES_LOW_FACTOR - PRES_LOW_OFFSET;
    }
    else
    {
        currentState.presTank = dataPT->presTank * PRES_HIGH_FACTOR - PRES_HIGH_OFFSET;
        currentState.presValves = dataPT->presValves * PRES_LOW_FACTOR - PRES_LOW_OFFSET;
    }

    if (ui->cbAddATM->isChecked())
    {
        currentState.presTank -= 1.0232;
        currentState.presValves -= 1.0232;
    }

    //currentState.presTank = dataPT->presValves * PRES_HIGH_FACTOR - PRES_HIGH_OFFSET - 	1.0232;
    //currentState.presValves = dataPT->presTank * PRES_LOW_FACTOR - PRES_LOW_OFFSET - 	1.0232;

    currentState.tempTank = dataPT->tempTank * TEMP_FACTOR;
    currentState.tempPDU = dataPT->tempPDU * TEMP_FACTOR;
    currentState.tempValveRP = dataPT->tempValveRP * TEMP_FACTOR;
    currentState.tempValveRM = dataPT->tempValveRM * TEMP_FACTOR;
    currentState.tempValveEC = dataPT->tempValveEC * TEMP_FACTOR;
    currentState.tempNoz1 = dataPT->tempNoz1 * TEMP_FACTOR;
    currentState.tempNoz2 = dataPT->tempNoz2 * TEMP_FACTOR;


    uint32_t currentTime = dataPT->time[0] | (dataPT->time[1] << 8) | (dataPT->time[2] << 16);
    currentState.time = (double)currentTime / 1000.0;

    ringBufPT[rbPTPos++] = dataPT->counter;
    if (rbPTSize < 5) rbPTSize++;
    rbPTPos %= 5;

    lostPacksPT = ringBufPT[(rbPTPos + rbPTSize - 1) % 5]  - ringBufPT[rbPTPos] - rbPTSize + 1;

    applyCurrentState();

    if (gsMode == COM_CONNECTED && file.isOpen())
    {
        QDataStream out(&file);
        for (int i = 0; i < sizeof(dpPresTemp); i++)
            out << ((unsigned char*)dataPT)[i];
    }

    return true;
}

QString MainWindow::decodeModulState(uint16_t code)
{
    if (code == OK)
        return QString("OK");
    if (code == NO_INIT)
        return QString("NO_INIT");
    if (code == INIT_FAILED)
        return QString("INIT_FAILED");
    if (code == COMM_ERROR)
        return QString("COMM_ERROR");
    if (code == MINOR_ERROR)
        return QString("MINOR_ERROR");
    if (code == CRITICAL_ERROR)
        return QString("CRITICAL_ERROR");
    if (code == SD_DL_PROGRESS)
        return QString("SD_DL_PROGRESS");

    return QString("NO_CODE");
}

QString MainWindow::decodeMode(uint8_t code)
{
    switch (code & 0x7F)
    {
    case MODE_STANDBY:
        return QString("Stand-by-mode");
    case MODE_FLIGHT:
        return QString("Flight-mode");
    case MODE_SECURE:
        return QString("Secure-mode");
    case MODE_BD:
        return QString("Blowdown-mode");
    case MODE_SPINUP:
        return QString("Spinup-mode");
    }
    return QString("INVALID_MODE");
}

bool MainWindow::decodeCalc(const dpCalc *dataCalc)
{
    //qDebug("%d", sizeof(dpCalc));
    //qDebug("Received CMD\n");
    uint32_t checksum = Murmur::mm_hash_32((uint8_t*)dataCalc, sizeof(dpCalc) - 4);
    //qDebug("%d\n", sizeof(dpCalc));
    //qDebug("Checksum: %x\n", checksum);

    if (checksum != dataCalc->check)
        return false;

    //qDebug("Checksum correct");

    if ((dataCalc->valveState & 0b1) == 0b1)
    {
        currentState.valveRP = Qt::green;
    }
    else
    {
        currentState.valveRP = Qt::red;
    }

    if ((dataCalc->valveState & 0b10) == 0b10)
    {
        currentState.valveRM = Qt::green;
    }
    else
    {
        currentState.valveRM = Qt::red;
    }

    if ((dataCalc->valveState & 0b100) == 0b100)
    {
        currentState.valveEC = Qt::green;
    }
    else
    {
        currentState.valveEC = Qt::red;
    }

    if (dataCalc->rocket_signals & 0b1)
    {
        currentState.LO = Qt::green;
    }
    else
    {
        currentState.LO = Qt::red;
    }

    if (dataCalc->rocket_signals & 0b10)
    {
        currentState.SOE = Qt::green;
    }
    else
    {
        currentState.SOE = Qt::red;
    }


    if (dataCalc->rocket_signals & 0b100)
    {
        currentState.SODS = Qt::green;
    }
    else
    {
        currentState.SODS = Qt::red;
    }


    if (dataCalc->rocket_signals & 0b1000)
    {
        currentState.DCDC = Qt::green;
    }
    else
    {
        currentState.DCDC = Qt::red;
    }

    //qDebug("%d", dataCalc->vot);

    currentState.vot = dataCalc->vot;

    currentState.mode = decodeMode(dataCalc->mode);

    if (dataCalc->mode & 0x80)
        currentState.testMode = Qt::green;
    else
        currentState.testMode = Qt::red;

    uint16_t status = dataCalc->modulStates;

    currentState.imuState = decodeModulState(status & 0b111);

    status >>= 3;
    currentState.hkState = decodeModulState(status & 0b111);

    status >>= 3;
    currentState.controlState = decodeModulState(status & 0b111);

    status >>= 3;
    currentState.storageState = decodeModulState(status & 0b111);

    status >>= 3;
    currentState.tmState = decodeModulState(status & 0b111);

    currentState.deltaOmegaStart = dataCalc->deltaOmegaStart / DELTA_OMEGA_DIVIDOR;
    currentState.deltaOmegaOperation = dataCalc->deltaOmegaOperation / DELTA_OMEGA_DIVIDOR;
    currentState.deltaOmegaShutdown = dataCalc->deltaOmegaShutdown / DELTA_OMEGA_DIVIDOR;

    uint32_t currentTime = dataCalc->time[0] | (dataCalc->time[1] << 8) | (dataCalc->time[2] << 16);
    currentState.time = (double)currentTime / 1000.0;

    ringBufCalc[rbCalcPos++] = dataCalc->counter;
    if (rbCalcSize < 2) rbCalcSize++;
    rbCalcPos %= 2;

    lostPacksCalc = ringBufCalc[(rbCalcPos + rbCalcSize - 1) % 2]  - ringBufCalc[rbCalcPos] - rbCalcSize + 1;

    applyCurrentState();

    if (gsMode == COM_CONNECTED && file.isOpen())
    {
        QDataStream out(&file);
        for (int i = 0; i < sizeof(dpCalc); i++)
            out << ((unsigned char*)dataCalc)[i];
    }

    return true;
}

bool MainWindow::decodeSDDownlink(const void *dataPck)
{
    DpFinfo *fInfo = (DpFinfo*)(dataPck);
    DpFdata *fData = (DpFdata*)(dataPck);
    DpFend *fEnd = (DpFend*)(dataPck);
    DpDLcomplete *dlComp = (DpDLcomplete*)(dataPck);

    qDebug("Package %d received\n", fInfo->counter);

    //qDebug("Bytes: ");
    //for (int i = 0; i < sizeof(*fData); i++)
    //    qDebug("%d; ", ((uint8_t*)fData)[i]);

    bool ret = true;

    do
    {
        if (fInfo->counter != currentSDPack)
        {
            ret = false;
            break;
        }

        if (fInfo->sync == SYNC_DL_NF)
        {
            qDebug("Sync NF\n");
            uint32_t checksum = Murmur::mm_hash_32((uint8_t*)fInfo, sizeof(*fInfo) - 4);
            if (checksum != fInfo->check)
            {
                ret = false;
                break; //return false;
            }

            if (curSDFile.isOpen())
                curSDFile.close();

            curSDFile.setFileName(curSDDir.dirName() + "/" + QString(fInfo->fname));
            if (!curSDFile.open(QIODevice::WriteOnly))
            {
                //qDebug("schlecht...\n");
                ret = false;
                break; //return false;
            }
            //qDebug("File opened\n");
            currentSDPack++;

        }
        else if (fData->sync == SYNC_DL_FD)
        {
            qDebug("Sync FD\n");

            qDebug("\n");
            uint32_t checksum = Murmur::mm_hash_32((uint8_t*)fData, sizeof(*fData) - 4);
            qDebug("C1: %08X, C2: %08X\n", checksum, fData->check);
            if (checksum != fData->check)
            {
                ret = false;
                break; //return false;
            }

            //qDebug("Checksum correct\n");

            if (!curSDFile.isOpen())
            {
                ret = false;
                break; //return false;
            }

            //qDebug("File opened\n");

            QDataStream out(&curSDFile);

            for (int i = 0; i < sizeof(fData->fdata); i++)
                out << ((unsigned char*)fData->fdata)[i];

            currentSDPack++;
        }
        else if (fEnd->sync == SYNC_DL_FC)
        {
            qDebug("Sync FC\n");
            uint32_t checksum = Murmur::mm_hash_32((uint8_t*)fEnd, sizeof(*fEnd) - 4);
            if (checksum != fEnd->check)
            {
                ret = false;
                break; //return false;
            }

            if (!curSDFile.isOpen())
            {
                ret = false;
                break; //return false;
            }

            QDataStream out(&curSDFile);
            for (int i = 0; i < fEnd->remaining_bytes; i++)
                out << ((unsigned char*)fEnd->fdata)[i];

            curSDFile.close();

            currentSDPack++;
        }
        else if (dlComp->sync == SYNC_DL_SC)
        {
            qDebug("Sync SC\n");
            uint32_t checksum = Murmur::mm_hash_32((uint8_t*)dlComp, sizeof(*dlComp) - 4);
            if (checksum != dlComp->check)
            {
                ret = false;
                break; //return false;
            }

            // TODO change state
            sdDLinProgress = false;
            qDebug("Downlink complete!\n");

            currentSDPack++;
        }
    } while(0);

    dpCommand comPack;
    comPack.sync = SYNC_COMM;
    comPack.id = REQUEST_PCK;
    comPack.argument = currentSDPack;
    comPack.check = Murmur::mm_hash_32((uint8_t*)&comPack, sizeof(comPack) - sizeof(comPack.check));

    QByteArray commArray;
    QDataStream stream(&commArray, QIODevice::WriteOnly);

    for (uint32_t i = 0; i < sizeof(comPack); i++)
        stream << ((uint8_t*)(&comPack))[i];

    writeData(commArray);

    qDebug("ComPack sent: %d\n", currentSDPack);

    return ret; //TODO
}

bool MainWindow::decodeDLDebug(const DpDebug *dataDebug)
{
    uint32_t checksum = Murmur::mm_hash_32((uint8_t*)dataDebug, sizeof(DpDebug) - 4);

    qDebug("DL_DEBUG: %s\n", dataDebug->text);

    if (checksum != dataDebug->check)
        return false;


    return true;
}

void copyToByteArr(QByteArray &arr, void *data, uint32_t len)
{
    arr.clear();
    QDataStream stream(&arr, QIODevice::WriteOnly);
    for (uint32_t i = 1; i < len; i++)
    {
        stream << ((uint8_t*)data)[i];
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
    static DpFinfo fInfo;
    static DpFdata fData;
    //static DpFend &fEnd = *(DpFend*)(&fData);
    static DpFend fEnd;
    static DpDLcomplete dlComp;
    static DpDebug dlDebug;

    //qDebug("AB: %i\n", serial->bytesAvailable());

    static int package_selected = -1;
    static int package_index = 0;

    QByteArray data = serial->readAll();
    //int lastValidIndex = 0;

    QByteArray oldData;
    bool reReadData = false;
    uint32_t reReadIndex = 0;

    static bool dlSD = false;

    if (dlSD != sdDLinProgress)
    {
        package_selected = -1;
        package_index = 0;
        dlSD = sdDLinProgress;
    }

    //uint8_t c;
    for(int index = 0; index < data.length(); /*index++*/)
    {
        uint8_t c;
        if (reReadData == true)
        {
            if (reReadIndex < oldData.size())
                c = oldData.at(reReadIndex++);
            else
            {
                reReadData = false;
                c = data.at(index++);
            }
        }
        else
        {
            c = data.at(index++);
        }
        //qDebug("%X ", c);
        if (dlSD == false)
        {
            if (package_selected == -1)
            {
                //qDebug("Nothing selected");
                if (c == SYNC_IMU)
                {
                    package_selected = SYNC_IMU;
                    ((uint8_t*)&dataIMU)[package_index++] = c;
                    //lastValidIndex = index;
                }
                else if (c == SYNC_PT)
                {
                    package_selected = SYNC_PT;
                    ((uint8_t*)&dataPT)[package_index++] = c;
                    //lastValidIndex = index;
                }
                else if (c == SYNC_CALC)
                {
                    package_selected = SYNC_CALC;
                    ((uint8_t*)&dataCalc)[package_index++] = c;
                    //lastValidIndex = index;
                }
                #ifdef ACTIVATE_DEBUG_DL
                else if (c == DL_DEBUG_ID)
                {
                    package_selected = DL_DEBUG_ID;
                    ((uint8_t*)&dlDebug)[package_index++] = c;
                    //lastValidIndex = index;
                }
                #endif


            }
            else if (package_selected == SYNC_IMU)
            {
                ((uint8_t*)&dataIMU)[package_index++] = c;
                if (package_index >= sizeof(dpIMU))
                {
                    bool suc = decodeIMU(&dataIMU);
                    package_selected = -1;
                    package_index = 0;
                    if (!suc)
                    {
                        copyToByteArr(oldData, &dataIMU, sizeof(dpIMU));
                        reReadData = true;
                        reReadIndex = 0;
                        //index = lastValidIndex;
                    }
                }
            }
            else if (package_selected == SYNC_PT)
            {
                ((uint8_t*)&dataPT)[package_index++] = c;
                if (package_index >= sizeof(dpPresTemp))
                {
                    bool suc = decodePT(&dataPT);
                    package_selected = -1;
                    package_index = 0;
                    if (!suc)
                    {
                        copyToByteArr(oldData, &dataPT, sizeof(dpPresTemp));
                        reReadData = true;
                        reReadIndex = 0;
                        //index = lastValidIndex;
                    }
                }
            }
            else if (package_selected == SYNC_CALC)
            {
                ((uint8_t*)&dataCalc)[package_index++] = c;
                if (package_index >= sizeof(dpCalc))
                {
                    bool suc = decodeCalc(&dataCalc);
                    package_selected = -1;
                    package_index = 0;
                    if (!suc)
                    {
                        copyToByteArr(oldData, &dataCalc, sizeof(dpCalc));
                        reReadData = true;
                        reReadIndex = 0;
                        //index = lastValidIndex;
                    }
                }
            }
            #ifdef ACTIVATE_DEBUG_DL
            else if (package_selected == DL_DEBUG_ID)
            {
                ((uint8_t*)&dlDebug)[package_index++] = c;
                if (package_index >= sizeof(dlDebug))
                {
                    bool suc = decodeDLDebug(&dlDebug);
                    package_selected = -1;
                    package_index = 0;
                    if (!suc)
                    {
                        copyToByteArr(oldData, &dlDebug, sizeof(dlDebug));
                        reReadData = true;
                        reReadIndex = 0;
                        //index = lastValidIndex;
                    }
                }
            }
            #endif
            else
            {
                qDebug("Hier sollte man nie hinkommen...\n");
                package_selected = -1;
                package_index = 0;
            }
        }
        else
        {
            if (package_selected == -1)
            {
                if (c == SYNC_DL_NF)
                {
                    package_selected = SYNC_DL_NF;
                    ((uint8_t*)&fInfo)[package_index++] = c;
                    //lastValidIndex = index;
                }
                else if (c == SYNC_DL_FD)
                {
                    package_selected = SYNC_DL_FD;
                    ((uint8_t*)&fData)[package_index++] = c;
                    //lastValidIndex = index;
                }
                else if (c == SYNC_DL_FC)
                {
                    package_selected = SYNC_DL_FC;
                    ((uint8_t*)&fEnd)[package_index++] = c;
                    //lastValidIndex = index;
                }
                else if (c == SYNC_DL_SC)
                {
                    package_selected = SYNC_DL_SC;
                    ((uint8_t*)&dlComp)[package_index++] = c;
                    //lastValidIndex = index;
                }
            }
            else if (package_selected == SYNC_DL_NF)
            {
                ((uint8_t*)&fInfo)[package_index++] = c;
                if (package_index >= sizeof(fInfo))
                {
                    bool suc = decodeSDDownlink(&fInfo);
                    package_selected = -1;
                    package_index = 0;
                    if (!suc)
                    {
                        copyToByteArr(oldData, &fInfo, sizeof(fInfo));
                        reReadData = true;
                        reReadIndex = 0;
                        //index = lastValidIndex;
                    }
                }
            }
            else if (package_selected == SYNC_DL_FD)
            {
                ((uint8_t*)&fData)[package_index++] = c;
                if (package_index >= sizeof(fData))
                {
                    bool suc = decodeSDDownlink(&fData);
                    package_selected = -1;
                    package_index = 0;
                    if (!suc)
                    {
                        copyToByteArr(oldData, &fData, sizeof(fData));
                        reReadData = true;
                        reReadIndex = 0;
                        //index = lastValidIndex;
                    }
                }
            }
            else if (package_selected == SYNC_DL_FC)
            {
                ((uint8_t*)&fEnd)[package_index++] = c;
                if (package_index >= sizeof(fEnd))
                {
                    bool suc = decodeSDDownlink(&fEnd);
                    package_selected = -1;
                    package_index = 0;
                    if (!suc)
                    {
                        copyToByteArr(oldData, &fEnd, sizeof(fEnd));
                        reReadData = true;
                        reReadIndex = 0;
                        //index = lastValidIndex;
                    }
                }
            }
            else if (package_selected == SYNC_DL_SC)
            {
                ((uint8_t*)&dlComp)[package_index++] = c;
                if (package_index >= sizeof(dlComp))
                {
                    bool suc = decodeSDDownlink(&dlComp);
                    package_selected = -1;
                    package_index = 0;
                    if (!suc)
                    {
                        copyToByteArr(oldData, &dlComp, sizeof(dlComp));
                        reReadData = true;
                        reReadIndex = 0;
                        //index = lastValidIndex;
                    }
                }
            }
            else
            {
                qDebug("Hier sollte man nie hinkommen...\n");
                package_selected = -1;
                package_index = 0;
            }
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

/*
bool MainWindow::decodeObcIMU(const ObcIMUdata *dataIMU)
{

    double gyro_factor;
    switch (dataIMU->imu_config)
    {
    case BITS_FS_250DPS:
        gyro_factor = GYRO_FACTOR250;
        break;
    case BITS_FS_500DPS:
        gyro_factor = GYRO_FACTOR500;
        break;
    case BITS_FS_1000DPS:
        gyro_factor = GYRO_FACTOR1000;
        break;
    case BITS_FS_2000DPS:
        gyro_factor = GYRO_FACTOR2000;
        break;
    default:
        gyro_factor = GYRO_FACTOR2000;
        break;
    }

    currentState.imu1_roll = dataIMU->imu1_roll * gyro_factor;
    currentState.imu1_yaw = dataIMU->imu1_yaw * gyro_factor;
    currentState.imu1_pitch = dataIMU->imu1_pitch * gyro_factor;
    currentState.imu2_roll = dataIMU->imu2_roll * gyro_factor;
    currentState.imu2_yaw = dataIMU->imu2_yaw * gyro_factor;
    currentState.imu2_pitch = dataIMU->imu2_pitch * gyro_factor;
    currentState.imu1_accx = dataIMU->imu1_accx * ACC_FACTOR;
    currentState.imu1_accy = dataIMU->imu1_accy * ACC_FACTOR;
    currentState.imu1_accz = dataIMU->imu1_accz * ACC_FACTOR;
    currentState.imu2_accx = dataIMU->imu2_accx * ACC_FACTOR;
    currentState.imu2_accy = dataIMU->imu2_accy * ACC_FACTOR;
    currentState.imu2_accz = dataIMU->imu2_accz * ACC_FACTOR;
    currentState.imufiltered_roll = dataIMU->gyroFiltered[0];
    currentState.imufiltered_yaw = dataIMU->gyroFiltered[1];
    currentState.imufiltered_pitch = dataIMU->gyroFiltered[2];

    uint32_t currentTime = dataIMU->sysTime / 1000000LL;
    times.push_back(currentTime*0.001);
    rates.push_back(currentState.imufiltered_roll);
    currentState.used_plotpoints = times.size();

    currentState.time = (double)currentTime / 1000.0;

    applyCurrentState();

    return true;
}

bool MainWindow::decodeObcHk(const ObcHkData *dataHk)
{

    currentState.presTank = dataHk->presTank * PRES_HIGH_FACTOR - PRES_HIGH_OFFSET;
    currentState.presValves = dataHk->presValves * PRES_LOW_FACTOR - PRES_LOW_OFFSET;

    currentState.tempTank = dataHk->tempTank * TEMP_FACTOR;
    currentState.tempPDU = dataHk->tempPDU * TEMP_FACTOR;
    currentState.tempValveRP = dataHk->tempValveRP * TEMP_FACTOR;
    currentState.tempValveRM = dataHk->tempValveRM * TEMP_FACTOR;
    currentState.tempValveEC = dataHk->tempValveEC * TEMP_FACTOR;
    currentState.tempNoz1 = dataHk->tempNoz1 * TEMP_FACTOR;
    currentState.tempNoz2 = dataHk->tempNoz2 * TEMP_FACTOR;

    uint32_t currentTime = dataHk->sysTime / 1000000LL;
    currentState.time = (double)currentTime / 1000.0;

    applyCurrentState();

    return true;
}

bool MainWindow::decodeObcCmd(const ObcCmdData *dataCmd)
{

    if ((dataCmd->valveState & 0b1) == 0b1)
    {
        currentState.valveRP = Qt::green;
    }
    else
    {
        currentState.valveRP = Qt::red;
    }

    if ((dataCmd->valveState & 0b10) == 0b10)
    {
        currentState.valveRM = Qt::green;
    }
    else
    {
        currentState.valveRM = Qt::red;
    }

    if ((dataCmd->valveState & 0b100) == 0b100)
    {
        currentState.valveEC = Qt::green;
    }
    else
    {
        currentState.valveEC = Qt::red;
    }

    currentState.vot = dataCmd->vot;

    applyCurrentState();

    return true;
}
*/

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
    wPDUTemperature = new ThermoMeter(this);
    wValveRPTemperature = new ThermoMeter(this);
    wValveRMTemperature = new ThermoMeter(this);
    wValveECTemperature = new ThermoMeter(this);
    wNozzle1Temperature = new ThermoMeter(this);
    wNozzle2Temperature = new ThermoMeter(this);


    wValveRP = new Led(this);
    wValveRM = new Led(this);
    wValveEC = new Led(this);
    wTestMode = new Led(this);
    wLO = new Led(this);
    wSOE = new Led(this);
    wSODS = new Led(this);
    wDCDC = new Led(this);
    wChart = new Chart(this);

    wTankPressure->setRange(0, 300);
    wTankPressure->setMajorTicks(7);
    wTankPressure->setMinorTicks(4);
    wTankPressure->setLabel(QString("Tank"));
    wTankPressure->setLabelOffset(0.75);
    wTankPressure->setLabelFont(font10);
    wTankPressure->setDigitFont(QFont("Fixed", 10));
    wTankPressure->setDigitFormat("%.0f");
    wTankPressure->setAnimated(false);
    //wTankPressure->setGapAngle(200);
    wTankPressure->setValue(0);

    wValvePressure->setRange(0, 10);
    wValvePressure->setMajorTicks(11);
    wValvePressure->setMinorTicks(4);
    wValvePressure->setLabel(QString("Valves"));
    wValvePressure->setLabelOffset(0.75);
    wValvePressure->setLabelFont(font10);
    wValvePressure->setDigitFont(QFont("Fixed", 10));
    wValvePressure->setDigitFormat("%.1f");
    wValvePressure->setAnimated(false);
    wValvePressure->setValue(0);

    wTankTemperature->setSuffix(QString("°C"));
    wTankTemperature->setMaximum(50);
    wTankTemperature->setMinimum(-30);
    wTankTemperature->setFont(font10);
    wTankTemperature->setDigitFont(font10);
    wTankTemperature->setValueFont(font14);
    wTankTemperature->setValueOffset(wTankTemperature->valueOffset() + 10);
    wTankTemperature->setStyleSheet("ThermoMeter { color : rgb(200, 200, 255) }");

    wPDUTemperature->setSuffix(QString("°C"));
    wPDUTemperature->setMaximum(50);
    wPDUTemperature->setMinimum(-30);
    wPDUTemperature->setFont(font10);
    wPDUTemperature->setDigitFont(font10);
    wPDUTemperature->setValueFont(font14);
    wPDUTemperature->setValueOffset(wPDUTemperature->valueOffset() + 10);
    wPDUTemperature->setStyleSheet("ThermoMeter { color : rgb(200, 200, 255) }");

    wValveRPTemperature->setSuffix(QString("°C"));
    wValveRPTemperature->setMaximum(50);
    wValveRPTemperature->setMinimum(-30);
    wValveRPTemperature->setFont(font10);
    wValveRPTemperature->setDigitFont(font10);
    wValveRPTemperature->setValueFont(font14);
    wValveRPTemperature->setValueOffset(wValveRPTemperature->valueOffset() + 10);
    wValveRPTemperature->setStyleSheet("ThermoMeter { color : rgb(200, 200, 255) }");

    wValveRMTemperature->setSuffix(QString("°C"));
    wValveRMTemperature->setMaximum(50);
    wValveRMTemperature->setMinimum(-30);
    wValveRMTemperature->setFont(font10);
    wValveRMTemperature->setDigitFont(font10);
    wValveRMTemperature->setValueFont(font14);
    wValveRMTemperature->setValueOffset(wValveRMTemperature->valueOffset() + 10);
    wValveRMTemperature->setStyleSheet("ThermoMeter { color : rgb(200, 200, 255) }");

    wValveECTemperature->setSuffix(QString("°C"));
    wValveECTemperature->setMaximum(50);
    wValveECTemperature->setMinimum(-30);
    wValveECTemperature->setFont(font10);
    wValveECTemperature->setDigitFont(font10);
    wValveECTemperature->setValueFont(font14);
    wValveECTemperature->setValueOffset(wValveECTemperature->valueOffset() + 10);
    wValveECTemperature->setStyleSheet("ThermoMeter { color : rgb(200, 200, 255) }");

    wNozzle1Temperature->setSuffix(QString("°C"));
    wNozzle1Temperature->setMaximum(50);
    wNozzle1Temperature->setMinimum(-30);
    wNozzle1Temperature->setFont(font10);
    wNozzle1Temperature->setDigitFont(font10);
    wNozzle1Temperature->setValueFont(font14);
    wNozzle1Temperature->setValueOffset(wNozzle1Temperature->valueOffset() + 10);
    wNozzle1Temperature->setStyleSheet("ThermoMeter { color : rgb(200, 200, 255) }");

    wNozzle2Temperature->setSuffix(QString("°C"));
    wNozzle2Temperature->setMaximum(50);
    wNozzle2Temperature->setMinimum(-30);
    wNozzle2Temperature->setFont(font10);
    wNozzle2Temperature->setDigitFont(font10);
    wNozzle2Temperature->setValueFont(font14);
    wNozzle2Temperature->setValueOffset(wNozzle2Temperature->valueOffset() + 10);
    wNozzle2Temperature->setStyleSheet("ThermoMeter { color : rgb(200, 200, 255) }");

    //wValveRM->setColor(Qt::green);


    //fillChartData();
    initCharts(wChart);
    wChart->setSize(GRAPH_X);
    wChart->setPosition(0);
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

    wChart->setSizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::MinimumExpanding);

    ui->gridGraph->addWidget(wChart);
    ui->gridTankPres->addWidget(wTankPressure);
    ui->gridValvePres->addWidget(wValvePressure);
    ui->gridTankTemp->addWidget(wTankTemperature);
    ui->gridPDUTemp->addWidget(wPDUTemperature);
    ui->gridValveRPTemp->addWidget(wValveRPTemperature);
    ui->gridValveRMTemp->addWidget(wValveRMTemperature);
    ui->gridValveECTemp->addWidget(wValveECTemperature);
    ui->gridNoz1Temp->addWidget(wNozzle1Temperature);
    ui->gridNoz2Temp->addWidget(wNozzle2Temperature);
    ui->gridValveRP->addWidget(wValveRP);
    ui->gridValveRM->addWidget(wValveRM);
    ui->gridValveEC->addWidget(wValveEC);
    ui->gridLO->addWidget(wLO);
    ui->gridSOE->addWidget(wSOE);
    ui->gridSODS->addWidget(wSODS);
    ui->gridDCDC->addWidget(wDCDC);
    ui->gridTestMode->addWidget(wTestMode);

    ui->txtMode->setDisabled(true);
    ui->txtMode->setText("Stand-by-mode");
    ui->txtMode->setStyleSheet("background-color: rgb(255, 255, 255);color: rgb(0, 0, 0);");

    ui->txtIMUState->setDisabled(true);
    //ui->txtIMUState->zoomIn(4);
    ui->txtIMUState->setText("NO_INIT");

    ui->txtHkState->setDisabled(true);
    //ui->txtHkState->zoomIn(4);
    ui->txtHkState->setText("NO_INIT");

    ui->txtControlState->setDisabled(true);
    //ui->txtControlState->zoomIn(4);
    ui->txtControlState->setText("NO_INIT");

    ui->txtStorageState->setDisabled(true);
    //ui->txtStorageState->zoomIn(4);
    ui->txtStorageState->setText("NO_INIT");

    ui->txtTMState->setDisabled(true);
    //ui->txtTMState->zoomIn(4);
    ui->txtTMState->setText("NO_INIT");

    lbTankTemp = new VerticalLabel("Tank", this);
    lbTankTemp->setFont(font12);
    ui->gridLabelTankTemp->addWidget(lbTankTemp);

    lbPDUTemp = new VerticalLabel("PDU", this);
    lbPDUTemp->setFont(font12);
    ui->gridLabelPDUTemp->addWidget(lbPDUTemp);

    lbValveRPTemp = new VerticalLabel("Valve RP", this);
    lbValveRPTemp->setFont(font12);
    ui->gridLabelValveRPTemp->addWidget(lbValveRPTemp);

    lbValveRMTemp = new VerticalLabel("Valve RM", this);
    lbValveRMTemp->setFont(font12);
    ui->gridLabelValveRMTemp->addWidget(lbValveRMTemp);

    lbValveECTemp = new VerticalLabel("Valve EC", this);
    lbValveECTemp->setFont(font12);
    ui->gridLabelValveECTemp->addWidget(lbValveECTemp);

    lbNoz1Temp = new VerticalLabel("Nozzle 1", this);
    lbNoz1Temp->setFont(font12);
    ui->gridLabelNozzle1Temp->addWidget(lbNoz1Temp);

    lbNoz2Temp = new VerticalLabel("Nozzle 2", this);
    lbNoz2Temp->setFont(font12);
    ui->gridLabelNozzle2Temp->addWidget(lbNoz2Temp);

    // 3D-Stuff



    view = new Qt3DExtras::Qt3DWindow();
    view->defaultFramegraph()->setClearColor(QColor(QRgb(0x505050)));
    container = QWidget::createWindowContainer(view);
    ui->gridRocket->addWidget(container);
    //QSize screenSize = view->screen()->size();
    //container->setMinimumSize(QSize(200, 100));
    //container->setMaximumSize(screenSize);

    input = new Qt3DInput::QInputAspect;
    view->registerAspect(input);

    // Root entity
    rootEntity = new Qt3DCore::QEntity();

    // Camera
    cameraEntity = view->camera();

    // OLD_ROCKET
    cameraEntity->lens()->setPerspectiveProjection(45.0f, 16.0f/9.0f, 0.1f, 1000.0f);
    cameraEntity->setPosition(QVector3D(0, 0, 10.0f));
    //cameraEntity->setUpVector(QVector3D(0, 1, 1));
    cameraEntity->setViewCenter(QVector3D(0, 3.0f, 0));
    // OLD_ROCKET_END

    /* NEW_ROCKET
    cameraEntity->lens()->setPerspectiveProjection(45.0f, 16.0f/9.0f, 0.1f, 1000.0f);
    cameraEntity->setPosition(QVector3D(10.0f, 10.0f, 20.0f));
    cameraEntity->setUpVector(QVector3D(0.0f, 0.0f, 1.0f));
    cameraEntity->setViewCenter(QVector3D(0, 3.0f, 0));
    // NEW_ROCKET_END */

    // Set root object of the scene
    view->setRootEntity(rootEntity);


    // Create rocket
    rocket = new Rocket(rootEntity);
    //Qt3DRender::QDirectionalLight *light = new Qt3DRender::QDirectionalLight(rootEntity);
    //light->setIntensity(10000);
    //light->setWorldDirection(QVector3D(0, 0, -1));


    rollAnimation = new QPropertyAnimation(rocket);
    rollAnimation->setDuration(INT_MAX);
    rollAnimation->setStartValue(360.0f);
    rollAnimation->setEndValue(0.0f);
    rollAnimation->setLoopCount(-1);
    //rollAnimation->setDirection(QAbstractAnimation::Forward);
    rollAnimation->setTargetObject(rocket);
    rollAnimation->setPropertyName("roll");
    rollAnimation->start();
/*
    yawAnimation = new QPropertyAnimation(rocket);
    yawAnimation->setDuration(0);
    yawAnimation->setStartValue(-90.0f);
    yawAnimation->setEndValue(270.0f);
    yawAnimation->setLoopCount(-1);
    yawAnimation->setDirection(QAbstractAnimation::Forward);
    yawAnimation->setTargetObject(rocket);
    yawAnimation->setPropertyName("yaw");
    yawAnimation->start();

    pitchAnimation = new QPropertyAnimation(rocket);
    pitchAnimation->setDuration(0);
    pitchAnimation->setStartValue(0.0f);
    pitchAnimation->setEndValue(360.0f);
    pitchAnimation->setLoopCount(-1);
    pitchAnimation->setDirection(QAbstractAnimation::Forward);
    pitchAnimation->setTargetObject(rocket);
    pitchAnimation->setPropertyName("pitch");
    pitchAnimation->start();

*/

}

void MainWindow::initCharts(Chart* widget)
{
    QPen vPen;
    //vPen.setColor(Qt::red);
    vPen.setWidthF(2.0);
    vPen.setColor(QColor(0,200,0));
    cAngRates = new Channel(GRAPH_Y_MIN,GRAPH_Y_MAX,new DoubleDataContainer<vector<double>,vector<double>>(times,rates), "Angular rate (°/s)",vPen);
    cAngRates->setShowScale(true);
    //cAngRates->setMaximum(50);
    //cAngRates->setMinimum(-50);
    //cisnienie3.setShowScale(false);
    //pozycja.showScale = predkosc.showScale = false ;
    //chart->scaleGrid().showScale=false;
    //chart->scaleGrid().showGrid=false;
    widget->addChannel(*cAngRates);
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
        times.push_back(0.1*i);
        randf = 0.001 * ((rand() % 101) - 50);
        if (i > 50 && i < 300)
            randf -= 0.1;
        curRate += randf;
        rates.push_back(curRate);
    }
}

void MainWindow::on_stateSilder_valueChanged(int value)
{
    if (value >= states.size() || value < 0)
        return;

    currentState = states.at(value);
    applyCurrentState();

}

void MainWindow::on_buttonGraphReset_clicked()
{
    //resetStateList();
    times_all.clear();
    rates_all.clear();
    times.clear();
    rates.clear();

    wChart->update();
}

void MainWindow::on_leCommand_returnPressed()
{
    QString command = ui->leCommand->text();
    sendCommand(command);
    ui->leCommand->clear();
}

void MainWindow::on_pbSendCommand_clicked()
{
    on_leCommand_returnPressed();
}
