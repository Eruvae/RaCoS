/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.6.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLCDNumber>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QWidget *gridLayoutWidget;
    QGridLayout *gridTankPres;
    QLabel *label;
    QLCDNumber *lcdIMU1roll;
    QLCDNumber *lcdIMU1accx;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *label_4;
    QLabel *label_5;
    QLabel *label_6;
    QLCDNumber *lcdIMU1accy;
    QLabel *label_7;
    QLabel *label_8;
    QLCDNumber *lcdIMU1accz;
    QLabel *label_9;
    QLabel *label_10;
    QLabel *label_11;
    QLabel *label_12;
    QLabel *label_13;
    QLCDNumber *lcdIMU2accx;
    QLCDNumber *lcdIMU2accy;
    QLCDNumber *lcdIMU2roll;
    QLabel *label_14;
    QLCDNumber *lcdIMU2accz;
    QLabel *label_15;
    QLabel *label_16;
    QLabel *label_17;
    QLabel *label_18;
    QLabel *label_19;
    QLCDNumber *lcdTankPres;
    QLabel *label_20;
    QLCDNumber *lcdValvePres;
    QWidget *gridLayoutWidget_2;
    QGridLayout *gridValvePres;
    QWidget *gridLayoutWidget_3;
    QGridLayout *gridTankTemp;
    QWidget *gridLayoutWidget_4;
    QGridLayout *gridGraph;
    QLabel *label_25;
    QLabel *label_26;
    QWidget *gridLayoutWidget_7;
    QGridLayout *gridNoz1Temp;
    QLabel *label_27;
    QWidget *gridLayoutWidget_8;
    QGridLayout *gridNoz2Temp;
    QLabel *label_28;
    QWidget *gridLayoutWidget_9;
    QGridLayout *gridNoz3Temp;
    QLabel *label_29;
    QWidget *gridLayoutWidget_10;
    QGridLayout *gridNoz4Temp;
    QLabel *label_30;
    QWidget *gridLayoutWidget_11;
    QGridLayout *gridPDUTemp;
    QWidget *gridLayoutWidget_12;
    QGridLayout *gridValveRP;
    QWidget *gridLayoutWidget_13;
    QGridLayout *gridValveRM;
    QLabel *label_31;
    QLabel *label_32;
    QLabel *label_21;
    QLCDNumber *lcdIMU2yaw;
    QLabel *label_22;
    QLCDNumber *lcdIMU1yaw;
    QLabel *label_23;
    QLabel *label_24;
    QLCDNumber *lcdIMU2pitch;
    QLabel *label_33;
    QLCDNumber *lcdIMU1pitch;
    QLabel *label_34;
    QLabel *label_35;
    QPlainTextEdit *txtMode;
    QLabel *label_36;
    QWidget *gridLayoutWidget_14;
    QGridLayout *gridTestMode;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(1920, 1000);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        gridLayoutWidget = new QWidget(centralWidget);
        gridLayoutWidget->setObjectName(QStringLiteral("gridLayoutWidget"));
        gridLayoutWidget->setGeometry(QRect(870, 110, 231, 201));
        gridTankPres = new QGridLayout(gridLayoutWidget);
        gridTankPres->setSpacing(6);
        gridTankPres->setContentsMargins(11, 11, 11, 11);
        gridTankPres->setObjectName(QStringLiteral("gridTankPres"));
        gridTankPres->setContentsMargins(0, 0, 0, 0);
        label = new QLabel(centralWidget);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(50, 30, 71, 31));
        QFont font;
        font.setPointSize(12);
        font.setBold(true);
        font.setWeight(75);
        label->setFont(font);
        lcdIMU1roll = new QLCDNumber(centralWidget);
        lcdIMU1roll->setObjectName(QStringLiteral("lcdIMU1roll"));
        lcdIMU1roll->setGeometry(QRect(180, 80, 81, 31));
        lcdIMU1roll->setStyleSheet(QLatin1String("background-color: rgb(255, 255, 255);\n"
"color: rgb(0, 0, 0);"));
        lcdIMU1roll->setDigitCount(6);
        lcdIMU1roll->setSegmentStyle(QLCDNumber::Flat);
        lcdIMU1accx = new QLCDNumber(centralWidget);
        lcdIMU1accx->setObjectName(QStringLiteral("lcdIMU1accx"));
        lcdIMU1accx->setGeometry(QRect(180, 200, 81, 31));
        lcdIMU1accx->setStyleSheet(QLatin1String("background-color: rgb(255, 255, 255);\n"
"color: rgb(0, 0, 0);"));
        lcdIMU1accx->setDigitCount(6);
        lcdIMU1accx->setSegmentStyle(QLCDNumber::Flat);
        label_2 = new QLabel(centralWidget);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(270, 80, 31, 30));
        QFont font1;
        font1.setPointSize(10);
        label_2->setFont(font1);
        label_3 = new QLabel(centralWidget);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(50, 80, 101, 31));
        label_3->setFont(font1);
        label_4 = new QLabel(centralWidget);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(50, 200, 121, 31));
        label_4->setFont(font1);
        label_5 = new QLabel(centralWidget);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setGeometry(QRect(270, 200, 41, 30));
        label_5->setFont(font1);
        label_6 = new QLabel(centralWidget);
        label_6->setObjectName(QStringLiteral("label_6"));
        label_6->setGeometry(QRect(50, 240, 121, 31));
        label_6->setFont(font1);
        lcdIMU1accy = new QLCDNumber(centralWidget);
        lcdIMU1accy->setObjectName(QStringLiteral("lcdIMU1accy"));
        lcdIMU1accy->setGeometry(QRect(180, 240, 81, 31));
        lcdIMU1accy->setStyleSheet(QLatin1String("background-color: rgb(255, 255, 255);\n"
"color: rgb(0, 0, 0);"));
        lcdIMU1accy->setDigitCount(6);
        lcdIMU1accy->setSegmentStyle(QLCDNumber::Flat);
        label_7 = new QLabel(centralWidget);
        label_7->setObjectName(QStringLiteral("label_7"));
        label_7->setGeometry(QRect(270, 240, 41, 30));
        label_7->setFont(font1);
        label_8 = new QLabel(centralWidget);
        label_8->setObjectName(QStringLiteral("label_8"));
        label_8->setGeometry(QRect(50, 280, 121, 31));
        label_8->setFont(font1);
        lcdIMU1accz = new QLCDNumber(centralWidget);
        lcdIMU1accz->setObjectName(QStringLiteral("lcdIMU1accz"));
        lcdIMU1accz->setGeometry(QRect(180, 280, 81, 31));
        lcdIMU1accz->setStyleSheet(QLatin1String("background-color: rgb(255, 255, 255);\n"
"color: rgb(0, 0, 0);"));
        lcdIMU1accz->setDigitCount(6);
        lcdIMU1accz->setSegmentStyle(QLCDNumber::Flat);
        label_9 = new QLabel(centralWidget);
        label_9->setObjectName(QStringLiteral("label_9"));
        label_9->setGeometry(QRect(270, 280, 41, 30));
        label_9->setFont(font1);
        label_10 = new QLabel(centralWidget);
        label_10->setObjectName(QStringLiteral("label_10"));
        label_10->setGeometry(QRect(190, 30, 71, 31));
        QFont font2;
        font2.setPointSize(12);
        font2.setBold(false);
        font2.setWeight(50);
        label_10->setFont(font2);
        label_11 = new QLabel(centralWidget);
        label_11->setObjectName(QStringLiteral("label_11"));
        label_11->setGeometry(QRect(330, 30, 71, 31));
        label_11->setFont(font2);
        label_12 = new QLabel(centralWidget);
        label_12->setObjectName(QStringLiteral("label_12"));
        label_12->setGeometry(QRect(410, 80, 31, 30));
        label_12->setFont(font1);
        label_13 = new QLabel(centralWidget);
        label_13->setObjectName(QStringLiteral("label_13"));
        label_13->setGeometry(QRect(410, 280, 41, 30));
        label_13->setFont(font1);
        lcdIMU2accx = new QLCDNumber(centralWidget);
        lcdIMU2accx->setObjectName(QStringLiteral("lcdIMU2accx"));
        lcdIMU2accx->setGeometry(QRect(320, 200, 81, 31));
        lcdIMU2accx->setStyleSheet(QLatin1String("background-color: rgb(255, 255, 255);\n"
"color: rgb(0, 0, 0);"));
        lcdIMU2accx->setDigitCount(6);
        lcdIMU2accx->setSegmentStyle(QLCDNumber::Flat);
        lcdIMU2accy = new QLCDNumber(centralWidget);
        lcdIMU2accy->setObjectName(QStringLiteral("lcdIMU2accy"));
        lcdIMU2accy->setGeometry(QRect(320, 240, 81, 31));
        lcdIMU2accy->setStyleSheet(QLatin1String("background-color: rgb(255, 255, 255);\n"
"color: rgb(0, 0, 0);"));
        lcdIMU2accy->setDigitCount(6);
        lcdIMU2accy->setSegmentStyle(QLCDNumber::Flat);
        lcdIMU2roll = new QLCDNumber(centralWidget);
        lcdIMU2roll->setObjectName(QStringLiteral("lcdIMU2roll"));
        lcdIMU2roll->setGeometry(QRect(320, 80, 81, 31));
        lcdIMU2roll->setStyleSheet(QLatin1String("background-color: rgb(255, 255, 255);\n"
"color: rgb(0, 0, 0);"));
        lcdIMU2roll->setDigitCount(6);
        lcdIMU2roll->setSegmentStyle(QLCDNumber::Flat);
        label_14 = new QLabel(centralWidget);
        label_14->setObjectName(QStringLiteral("label_14"));
        label_14->setGeometry(QRect(410, 240, 41, 30));
        label_14->setFont(font1);
        lcdIMU2accz = new QLCDNumber(centralWidget);
        lcdIMU2accz->setObjectName(QStringLiteral("lcdIMU2accz"));
        lcdIMU2accz->setGeometry(QRect(320, 280, 81, 31));
        lcdIMU2accz->setStyleSheet(QLatin1String("background-color: rgb(255, 255, 255);\n"
"color: rgb(0, 0, 0);"));
        lcdIMU2accz->setDigitCount(6);
        lcdIMU2accz->setSegmentStyle(QLCDNumber::Flat);
        label_15 = new QLabel(centralWidget);
        label_15->setObjectName(QStringLiteral("label_15"));
        label_15->setGeometry(QRect(410, 200, 41, 30));
        label_15->setFont(font1);
        label_16 = new QLabel(centralWidget);
        label_16->setObjectName(QStringLiteral("label_16"));
        label_16->setGeometry(QRect(880, 10, 111, 31));
        label_16->setFont(font);
        label_17 = new QLabel(centralWidget);
        label_17->setObjectName(QStringLiteral("label_17"));
        label_17->setGeometry(QRect(910, 70, 101, 31));
        label_17->setFont(font1);
        label_18 = new QLabel(centralWidget);
        label_18->setObjectName(QStringLiteral("label_18"));
        label_18->setGeometry(QRect(1170, 70, 81, 31));
        label_18->setFont(font1);
        label_19 = new QLabel(centralWidget);
        label_19->setObjectName(QStringLiteral("label_19"));
        label_19->setGeometry(QRect(1350, 70, 31, 30));
        label_19->setFont(font1);
        lcdTankPres = new QLCDNumber(centralWidget);
        lcdTankPres->setObjectName(QStringLiteral("lcdTankPres"));
        lcdTankPres->setGeometry(QRect(960, 70, 81, 31));
        lcdTankPres->setStyleSheet(QLatin1String("background-color: rgb(255, 255, 255);\n"
"color: rgb(0, 0, 0);"));
        lcdTankPres->setDigitCount(6);
        lcdTankPres->setSegmentStyle(QLCDNumber::Flat);
        label_20 = new QLabel(centralWidget);
        label_20->setObjectName(QStringLiteral("label_20"));
        label_20->setGeometry(QRect(1050, 70, 31, 30));
        label_20->setFont(font1);
        lcdValvePres = new QLCDNumber(centralWidget);
        lcdValvePres->setObjectName(QStringLiteral("lcdValvePres"));
        lcdValvePres->setGeometry(QRect(1260, 70, 81, 31));
        lcdValvePres->setStyleSheet(QLatin1String("background-color: rgb(255, 255, 255);\n"
"color: rgb(0, 0, 0);"));
        lcdValvePres->setDigitCount(6);
        lcdValvePres->setSegmentStyle(QLCDNumber::Flat);
        gridLayoutWidget_2 = new QWidget(centralWidget);
        gridLayoutWidget_2->setObjectName(QStringLiteral("gridLayoutWidget_2"));
        gridLayoutWidget_2->setGeometry(QRect(1170, 110, 231, 201));
        gridValvePres = new QGridLayout(gridLayoutWidget_2);
        gridValvePres->setSpacing(6);
        gridValvePres->setContentsMargins(11, 11, 11, 11);
        gridValvePres->setObjectName(QStringLiteral("gridValvePres"));
        gridValvePres->setContentsMargins(0, 0, 0, 0);
        gridLayoutWidget_3 = new QWidget(centralWidget);
        gridLayoutWidget_3->setObjectName(QStringLiteral("gridLayoutWidget_3"));
        gridLayoutWidget_3->setGeometry(QRect(870, 450, 141, 341));
        gridTankTemp = new QGridLayout(gridLayoutWidget_3);
        gridTankTemp->setSpacing(6);
        gridTankTemp->setContentsMargins(11, 11, 11, 11);
        gridTankTemp->setObjectName(QStringLiteral("gridTankTemp"));
        gridTankTemp->setContentsMargins(0, 0, 0, 0);
        gridLayoutWidget_4 = new QWidget(centralWidget);
        gridLayoutWidget_4->setObjectName(QStringLiteral("gridLayoutWidget_4"));
        gridLayoutWidget_4->setGeometry(QRect(40, 400, 731, 451));
        gridGraph = new QGridLayout(gridLayoutWidget_4);
        gridGraph->setSpacing(6);
        gridGraph->setContentsMargins(11, 11, 11, 11);
        gridGraph->setObjectName(QStringLiteral("gridGraph"));
        gridGraph->setContentsMargins(0, 0, 0, 0);
        label_25 = new QLabel(centralWidget);
        label_25->setObjectName(QStringLiteral("label_25"));
        label_25->setGeometry(QRect(900, 410, 101, 31));
        label_25->setFont(font1);
        label_26 = new QLabel(centralWidget);
        label_26->setObjectName(QStringLiteral("label_26"));
        label_26->setGeometry(QRect(1070, 410, 111, 31));
        label_26->setFont(font1);
        gridLayoutWidget_7 = new QWidget(centralWidget);
        gridLayoutWidget_7->setObjectName(QStringLiteral("gridLayoutWidget_7"));
        gridLayoutWidget_7->setGeometry(QRect(1050, 450, 141, 341));
        gridNoz1Temp = new QGridLayout(gridLayoutWidget_7);
        gridNoz1Temp->setSpacing(6);
        gridNoz1Temp->setContentsMargins(11, 11, 11, 11);
        gridNoz1Temp->setObjectName(QStringLiteral("gridNoz1Temp"));
        gridNoz1Temp->setContentsMargins(0, 0, 0, 0);
        label_27 = new QLabel(centralWidget);
        label_27->setObjectName(QStringLiteral("label_27"));
        label_27->setGeometry(QRect(1240, 410, 111, 31));
        label_27->setFont(font1);
        gridLayoutWidget_8 = new QWidget(centralWidget);
        gridLayoutWidget_8->setObjectName(QStringLiteral("gridLayoutWidget_8"));
        gridLayoutWidget_8->setGeometry(QRect(1220, 450, 141, 341));
        gridNoz2Temp = new QGridLayout(gridLayoutWidget_8);
        gridNoz2Temp->setSpacing(6);
        gridNoz2Temp->setContentsMargins(11, 11, 11, 11);
        gridNoz2Temp->setObjectName(QStringLiteral("gridNoz2Temp"));
        gridNoz2Temp->setContentsMargins(0, 0, 0, 0);
        label_28 = new QLabel(centralWidget);
        label_28->setObjectName(QStringLiteral("label_28"));
        label_28->setGeometry(QRect(1410, 410, 111, 31));
        label_28->setFont(font1);
        gridLayoutWidget_9 = new QWidget(centralWidget);
        gridLayoutWidget_9->setObjectName(QStringLiteral("gridLayoutWidget_9"));
        gridLayoutWidget_9->setGeometry(QRect(1390, 450, 141, 341));
        gridNoz3Temp = new QGridLayout(gridLayoutWidget_9);
        gridNoz3Temp->setSpacing(6);
        gridNoz3Temp->setContentsMargins(11, 11, 11, 11);
        gridNoz3Temp->setObjectName(QStringLiteral("gridNoz3Temp"));
        gridNoz3Temp->setContentsMargins(0, 0, 0, 0);
        label_29 = new QLabel(centralWidget);
        label_29->setObjectName(QStringLiteral("label_29"));
        label_29->setGeometry(QRect(1570, 410, 111, 31));
        label_29->setFont(font1);
        gridLayoutWidget_10 = new QWidget(centralWidget);
        gridLayoutWidget_10->setObjectName(QStringLiteral("gridLayoutWidget_10"));
        gridLayoutWidget_10->setGeometry(QRect(1550, 450, 141, 341));
        gridNoz4Temp = new QGridLayout(gridLayoutWidget_10);
        gridNoz4Temp->setSpacing(6);
        gridNoz4Temp->setContentsMargins(11, 11, 11, 11);
        gridNoz4Temp->setObjectName(QStringLiteral("gridNoz4Temp"));
        gridNoz4Temp->setContentsMargins(0, 0, 0, 0);
        label_30 = new QLabel(centralWidget);
        label_30->setObjectName(QStringLiteral("label_30"));
        label_30->setGeometry(QRect(1740, 410, 101, 31));
        label_30->setFont(font1);
        gridLayoutWidget_11 = new QWidget(centralWidget);
        gridLayoutWidget_11->setObjectName(QStringLiteral("gridLayoutWidget_11"));
        gridLayoutWidget_11->setGeometry(QRect(1710, 450, 141, 341));
        gridPDUTemp = new QGridLayout(gridLayoutWidget_11);
        gridPDUTemp->setSpacing(6);
        gridPDUTemp->setContentsMargins(11, 11, 11, 11);
        gridPDUTemp->setObjectName(QStringLiteral("gridPDUTemp"));
        gridPDUTemp->setContentsMargins(0, 0, 0, 0);
        gridLayoutWidget_12 = new QWidget(centralWidget);
        gridLayoutWidget_12->setObjectName(QStringLiteral("gridLayoutWidget_12"));
        gridLayoutWidget_12->setGeometry(QRect(640, 80, 41, 41));
        gridValveRP = new QGridLayout(gridLayoutWidget_12);
        gridValveRP->setSpacing(6);
        gridValveRP->setContentsMargins(11, 11, 11, 11);
        gridValveRP->setObjectName(QStringLiteral("gridValveRP"));
        gridValveRP->setContentsMargins(0, 0, 0, 0);
        gridLayoutWidget_13 = new QWidget(centralWidget);
        gridLayoutWidget_13->setObjectName(QStringLiteral("gridLayoutWidget_13"));
        gridLayoutWidget_13->setGeometry(QRect(640, 140, 41, 41));
        gridValveRM = new QGridLayout(gridLayoutWidget_13);
        gridValveRM->setSpacing(6);
        gridValveRM->setContentsMargins(11, 11, 11, 11);
        gridValveRM->setObjectName(QStringLiteral("gridValveRM"));
        gridValveRM->setContentsMargins(0, 0, 0, 0);
        label_31 = new QLabel(centralWidget);
        label_31->setObjectName(QStringLiteral("label_31"));
        label_31->setGeometry(QRect(530, 85, 101, 31));
        label_31->setFont(font1);
        label_32 = new QLabel(centralWidget);
        label_32->setObjectName(QStringLiteral("label_32"));
        label_32->setGeometry(QRect(530, 146, 101, 30));
        label_32->setFont(font1);
        label_21 = new QLabel(centralWidget);
        label_21->setObjectName(QStringLiteral("label_21"));
        label_21->setGeometry(QRect(270, 120, 31, 30));
        label_21->setFont(font1);
        lcdIMU2yaw = new QLCDNumber(centralWidget);
        lcdIMU2yaw->setObjectName(QStringLiteral("lcdIMU2yaw"));
        lcdIMU2yaw->setGeometry(QRect(320, 120, 81, 31));
        lcdIMU2yaw->setStyleSheet(QLatin1String("background-color: rgb(255, 255, 255);\n"
"color: rgb(0, 0, 0);"));
        lcdIMU2yaw->setDigitCount(6);
        lcdIMU2yaw->setSegmentStyle(QLCDNumber::Flat);
        label_22 = new QLabel(centralWidget);
        label_22->setObjectName(QStringLiteral("label_22"));
        label_22->setGeometry(QRect(410, 120, 31, 30));
        label_22->setFont(font1);
        lcdIMU1yaw = new QLCDNumber(centralWidget);
        lcdIMU1yaw->setObjectName(QStringLiteral("lcdIMU1yaw"));
        lcdIMU1yaw->setGeometry(QRect(180, 120, 81, 31));
        lcdIMU1yaw->setStyleSheet(QLatin1String("background-color: rgb(255, 255, 255);\n"
"color: rgb(0, 0, 0);"));
        lcdIMU1yaw->setDigitCount(6);
        lcdIMU1yaw->setSegmentStyle(QLCDNumber::Flat);
        label_23 = new QLabel(centralWidget);
        label_23->setObjectName(QStringLiteral("label_23"));
        label_23->setGeometry(QRect(50, 120, 101, 31));
        label_23->setFont(font1);
        label_24 = new QLabel(centralWidget);
        label_24->setObjectName(QStringLiteral("label_24"));
        label_24->setGeometry(QRect(270, 160, 31, 30));
        label_24->setFont(font1);
        lcdIMU2pitch = new QLCDNumber(centralWidget);
        lcdIMU2pitch->setObjectName(QStringLiteral("lcdIMU2pitch"));
        lcdIMU2pitch->setGeometry(QRect(320, 160, 81, 31));
        lcdIMU2pitch->setStyleSheet(QLatin1String("background-color: rgb(255, 255, 255);\n"
"color: rgb(0, 0, 0);"));
        lcdIMU2pitch->setDigitCount(6);
        lcdIMU2pitch->setSegmentStyle(QLCDNumber::Flat);
        label_33 = new QLabel(centralWidget);
        label_33->setObjectName(QStringLiteral("label_33"));
        label_33->setGeometry(QRect(410, 160, 31, 30));
        label_33->setFont(font1);
        lcdIMU1pitch = new QLCDNumber(centralWidget);
        lcdIMU1pitch->setObjectName(QStringLiteral("lcdIMU1pitch"));
        lcdIMU1pitch->setGeometry(QRect(180, 160, 81, 31));
        lcdIMU1pitch->setStyleSheet(QLatin1String("background-color: rgb(255, 255, 255);\n"
"color: rgb(0, 0, 0);"));
        lcdIMU1pitch->setDigitCount(6);
        lcdIMU1pitch->setSegmentStyle(QLCDNumber::Flat);
        label_34 = new QLabel(centralWidget);
        label_34->setObjectName(QStringLiteral("label_34"));
        label_34->setGeometry(QRect(50, 160, 101, 31));
        label_34->setFont(font1);
        label_35 = new QLabel(centralWidget);
        label_35->setObjectName(QStringLiteral("label_35"));
        label_35->setGeometry(QRect(530, 230, 101, 31));
        label_35->setFont(font1);
        txtMode = new QPlainTextEdit(centralWidget);
        txtMode->setObjectName(QStringLiteral("txtMode"));
        txtMode->setGeometry(QRect(600, 230, 141, 31));
        label_36 = new QLabel(centralWidget);
        label_36->setObjectName(QStringLiteral("label_36"));
        label_36->setGeometry(QRect(530, 300, 101, 31));
        label_36->setFont(font1);
        gridLayoutWidget_14 = new QWidget(centralWidget);
        gridLayoutWidget_14->setObjectName(QStringLiteral("gridLayoutWidget_14"));
        gridLayoutWidget_14->setGeometry(QRect(640, 290, 41, 41));
        gridTestMode = new QGridLayout(gridLayoutWidget_14);
        gridTestMode->setSpacing(6);
        gridTestMode->setContentsMargins(11, 11, 11, 11);
        gridTestMode->setObjectName(QStringLiteral("gridTestMode"));
        gridTestMode->setContentsMargins(0, 0, 0, 0);
        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1920, 21));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "RaCoS GS", 0));
        label->setText(QApplication::translate("MainWindow", "IMU", 0));
        label_2->setText(QApplication::translate("MainWindow", "\302\260/s", 0));
        label_3->setText(QApplication::translate("MainWindow", "Roll rate", 0));
        label_4->setText(QApplication::translate("MainWindow", "Accelaration (x)", 0));
        label_5->setText(QApplication::translate("MainWindow", "m/s\302\262", 0));
        label_6->setText(QApplication::translate("MainWindow", "Accelaration (y)", 0));
        label_7->setText(QApplication::translate("MainWindow", "m/s\302\262", 0));
        label_8->setText(QApplication::translate("MainWindow", "Accelaration (z)", 0));
        label_9->setText(QApplication::translate("MainWindow", "m/s\302\262", 0));
        label_10->setText(QApplication::translate("MainWindow", "IMU-1", 0));
        label_11->setText(QApplication::translate("MainWindow", "IMU-2", 0));
        label_12->setText(QApplication::translate("MainWindow", "\302\260/s", 0));
        label_13->setText(QApplication::translate("MainWindow", "m/s\302\262", 0));
        label_14->setText(QApplication::translate("MainWindow", "m/s\302\262", 0));
        label_15->setText(QApplication::translate("MainWindow", "m/s\302\262", 0));
        label_16->setText(QApplication::translate("MainWindow", "Pressure", 0));
        label_17->setText(QApplication::translate("MainWindow", "Tank", 0));
        label_18->setText(QApplication::translate("MainWindow", "Valves", 0));
        label_19->setText(QApplication::translate("MainWindow", "bar", 0));
        label_20->setText(QApplication::translate("MainWindow", "bar", 0));
        label_25->setText(QApplication::translate("MainWindow", "Tank Temp", 0));
        label_26->setText(QApplication::translate("MainWindow", "Nozzle 1 Temp", 0));
        label_27->setText(QApplication::translate("MainWindow", "Nozzle 2 Temp", 0));
        label_28->setText(QApplication::translate("MainWindow", "Nozzle 3 Temp", 0));
        label_29->setText(QApplication::translate("MainWindow", "Nozzle 4 Temp", 0));
        label_30->setText(QApplication::translate("MainWindow", "PDU Temp", 0));
        label_31->setText(QApplication::translate("MainWindow", "Valve Roll+", 0));
        label_32->setText(QApplication::translate("MainWindow", "Valve Roll-", 0));
        label_21->setText(QApplication::translate("MainWindow", "\302\260/s", 0));
        label_22->setText(QApplication::translate("MainWindow", "\302\260/s", 0));
        label_23->setText(QApplication::translate("MainWindow", "Yaw rate", 0));
        label_24->setText(QApplication::translate("MainWindow", "\302\260/s", 0));
        label_33->setText(QApplication::translate("MainWindow", "\302\260/s", 0));
        label_34->setText(QApplication::translate("MainWindow", "Pitch rate", 0));
        label_35->setText(QApplication::translate("MainWindow", "Mode", 0));
        label_36->setText(QApplication::translate("MainWindow", "Test mode", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
