/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
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
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QWidget *gridLayoutWidget;
    QGridLayout *gridLayout;
    QLabel *label;
    QLCDNumber *lcdNumber;
    QLCDNumber *lcdNumber_2;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *label_4;
    QLabel *label_5;
    QLabel *label_6;
    QLCDNumber *lcdNumber_3;
    QLabel *label_7;
    QLabel *label_8;
    QLCDNumber *lcdNumber_4;
    QLabel *label_9;
    QLabel *label_10;
    QLabel *label_11;
    QLabel *label_12;
    QLabel *label_13;
    QLCDNumber *lcdNumber_5;
    QLCDNumber *lcdNumber_6;
    QLCDNumber *lcdNumber_7;
    QLabel *label_14;
    QLCDNumber *lcdNumber_8;
    QLabel *label_15;
    QLabel *label_16;
    QLabel *label_17;
    QLabel *label_18;
    QLabel *label_19;
    QLCDNumber *lcdNumber_9;
    QLabel *label_20;
    QLCDNumber *lcdNumber_10;
    QWidget *gridLayoutWidget_2;
    QGridLayout *gridLayout_2;
    QWidget *gridLayoutWidget_3;
    QGridLayout *gridLayout_3;
    QWidget *gridLayoutWidget_4;
    QGridLayout *gridLayout_4;
    QWidget *gridLayoutWidget_5;
    QGridLayout *gridLayout_5;
    QLabel *label_21;
    QWidget *gridLayoutWidget_6;
    QGridLayout *gridLayout_6;
    QLabel *label_22;
    QLCDNumber *lcdNumber_11;
    QLCDNumber *lcdNumber_12;
    QLabel *label_23;
    QLabel *label_24;
    QLabel *label_25;
    QLabel *label_26;
    QWidget *gridLayoutWidget_7;
    QGridLayout *gridLayout_7;
    QLabel *label_27;
    QWidget *gridLayoutWidget_8;
    QGridLayout *gridLayout_8;
    QLabel *label_28;
    QWidget *gridLayoutWidget_9;
    QGridLayout *gridLayout_9;
    QLabel *label_29;
    QWidget *gridLayoutWidget_10;
    QGridLayout *gridLayout_10;
    QLabel *label_30;
    QWidget *gridLayoutWidget_11;
    QGridLayout *gridLayout_11;
    QMenuBar *menuBar;
    QMenu *menuRaCoS_Groundstation;
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
        gridLayoutWidget->setGeometry(QRect(590, 110, 231, 201));
        gridLayout = new QGridLayout(gridLayoutWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        label = new QLabel(centralWidget);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(20, 10, 71, 31));
        QFont font;
        font.setPointSize(12);
        font.setBold(true);
        font.setWeight(75);
        label->setFont(font);
        lcdNumber = new QLCDNumber(centralWidget);
        lcdNumber->setObjectName(QStringLiteral("lcdNumber"));
        lcdNumber->setGeometry(QRect(150, 60, 81, 31));
        lcdNumber->setStyleSheet(QLatin1String("background-color: rgb(255, 255, 255);\n"
"color: rgb(0, 0, 0);"));
        lcdNumber->setDigitCount(6);
        lcdNumber->setSegmentStyle(QLCDNumber::Flat);
        lcdNumber_2 = new QLCDNumber(centralWidget);
        lcdNumber_2->setObjectName(QStringLiteral("lcdNumber_2"));
        lcdNumber_2->setGeometry(QRect(150, 100, 81, 31));
        lcdNumber_2->setStyleSheet(QLatin1String("background-color: rgb(255, 255, 255);\n"
"color: rgb(0, 0, 0);"));
        lcdNumber_2->setDigitCount(6);
        lcdNumber_2->setSegmentStyle(QLCDNumber::Flat);
        label_2 = new QLabel(centralWidget);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(240, 60, 31, 30));
        QFont font1;
        font1.setPointSize(10);
        label_2->setFont(font1);
        label_3 = new QLabel(centralWidget);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(20, 60, 101, 31));
        label_3->setFont(font1);
        label_4 = new QLabel(centralWidget);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(20, 100, 121, 31));
        label_4->setFont(font1);
        label_5 = new QLabel(centralWidget);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setGeometry(QRect(240, 100, 41, 30));
        label_5->setFont(font1);
        label_6 = new QLabel(centralWidget);
        label_6->setObjectName(QStringLiteral("label_6"));
        label_6->setGeometry(QRect(20, 140, 121, 31));
        label_6->setFont(font1);
        lcdNumber_3 = new QLCDNumber(centralWidget);
        lcdNumber_3->setObjectName(QStringLiteral("lcdNumber_3"));
        lcdNumber_3->setGeometry(QRect(150, 140, 81, 31));
        lcdNumber_3->setStyleSheet(QLatin1String("background-color: rgb(255, 255, 255);\n"
"color: rgb(0, 0, 0);"));
        lcdNumber_3->setDigitCount(6);
        lcdNumber_3->setSegmentStyle(QLCDNumber::Flat);
        label_7 = new QLabel(centralWidget);
        label_7->setObjectName(QStringLiteral("label_7"));
        label_7->setGeometry(QRect(240, 140, 41, 30));
        label_7->setFont(font1);
        label_8 = new QLabel(centralWidget);
        label_8->setObjectName(QStringLiteral("label_8"));
        label_8->setGeometry(QRect(20, 180, 121, 31));
        label_8->setFont(font1);
        lcdNumber_4 = new QLCDNumber(centralWidget);
        lcdNumber_4->setObjectName(QStringLiteral("lcdNumber_4"));
        lcdNumber_4->setGeometry(QRect(150, 180, 81, 31));
        lcdNumber_4->setStyleSheet(QLatin1String("background-color: rgb(255, 255, 255);\n"
"color: rgb(0, 0, 0);"));
        lcdNumber_4->setDigitCount(6);
        lcdNumber_4->setSegmentStyle(QLCDNumber::Flat);
        label_9 = new QLabel(centralWidget);
        label_9->setObjectName(QStringLiteral("label_9"));
        label_9->setGeometry(QRect(240, 180, 41, 30));
        label_9->setFont(font1);
        label_10 = new QLabel(centralWidget);
        label_10->setObjectName(QStringLiteral("label_10"));
        label_10->setGeometry(QRect(160, 10, 71, 31));
        QFont font2;
        font2.setPointSize(12);
        font2.setBold(false);
        font2.setWeight(50);
        label_10->setFont(font2);
        label_11 = new QLabel(centralWidget);
        label_11->setObjectName(QStringLiteral("label_11"));
        label_11->setGeometry(QRect(300, 10, 71, 31));
        label_11->setFont(font2);
        label_12 = new QLabel(centralWidget);
        label_12->setObjectName(QStringLiteral("label_12"));
        label_12->setGeometry(QRect(380, 60, 31, 30));
        label_12->setFont(font1);
        label_13 = new QLabel(centralWidget);
        label_13->setObjectName(QStringLiteral("label_13"));
        label_13->setGeometry(QRect(380, 180, 41, 30));
        label_13->setFont(font1);
        lcdNumber_5 = new QLCDNumber(centralWidget);
        lcdNumber_5->setObjectName(QStringLiteral("lcdNumber_5"));
        lcdNumber_5->setGeometry(QRect(290, 100, 81, 31));
        lcdNumber_5->setStyleSheet(QLatin1String("background-color: rgb(255, 255, 255);\n"
"color: rgb(0, 0, 0);"));
        lcdNumber_5->setDigitCount(6);
        lcdNumber_5->setSegmentStyle(QLCDNumber::Flat);
        lcdNumber_6 = new QLCDNumber(centralWidget);
        lcdNumber_6->setObjectName(QStringLiteral("lcdNumber_6"));
        lcdNumber_6->setGeometry(QRect(290, 140, 81, 31));
        lcdNumber_6->setStyleSheet(QLatin1String("background-color: rgb(255, 255, 255);\n"
"color: rgb(0, 0, 0);"));
        lcdNumber_6->setDigitCount(6);
        lcdNumber_6->setSegmentStyle(QLCDNumber::Flat);
        lcdNumber_7 = new QLCDNumber(centralWidget);
        lcdNumber_7->setObjectName(QStringLiteral("lcdNumber_7"));
        lcdNumber_7->setGeometry(QRect(290, 60, 81, 31));
        lcdNumber_7->setStyleSheet(QLatin1String("background-color: rgb(255, 255, 255);\n"
"color: rgb(0, 0, 0);"));
        lcdNumber_7->setDigitCount(6);
        lcdNumber_7->setSegmentStyle(QLCDNumber::Flat);
        label_14 = new QLabel(centralWidget);
        label_14->setObjectName(QStringLiteral("label_14"));
        label_14->setGeometry(QRect(380, 140, 41, 30));
        label_14->setFont(font1);
        lcdNumber_8 = new QLCDNumber(centralWidget);
        lcdNumber_8->setObjectName(QStringLiteral("lcdNumber_8"));
        lcdNumber_8->setGeometry(QRect(290, 180, 81, 31));
        lcdNumber_8->setStyleSheet(QLatin1String("background-color: rgb(255, 255, 255);\n"
"color: rgb(0, 0, 0);"));
        lcdNumber_8->setDigitCount(6);
        lcdNumber_8->setSegmentStyle(QLCDNumber::Flat);
        label_15 = new QLabel(centralWidget);
        label_15->setObjectName(QStringLiteral("label_15"));
        label_15->setGeometry(QRect(380, 100, 41, 30));
        label_15->setFont(font1);
        label_16 = new QLabel(centralWidget);
        label_16->setObjectName(QStringLiteral("label_16"));
        label_16->setGeometry(QRect(600, 10, 111, 31));
        label_16->setFont(font);
        label_17 = new QLabel(centralWidget);
        label_17->setObjectName(QStringLiteral("label_17"));
        label_17->setGeometry(QRect(630, 70, 101, 31));
        label_17->setFont(font1);
        label_18 = new QLabel(centralWidget);
        label_18->setObjectName(QStringLiteral("label_18"));
        label_18->setGeometry(QRect(890, 70, 81, 31));
        label_18->setFont(font1);
        label_19 = new QLabel(centralWidget);
        label_19->setObjectName(QStringLiteral("label_19"));
        label_19->setGeometry(QRect(1070, 70, 31, 30));
        label_19->setFont(font1);
        lcdNumber_9 = new QLCDNumber(centralWidget);
        lcdNumber_9->setObjectName(QStringLiteral("lcdNumber_9"));
        lcdNumber_9->setGeometry(QRect(680, 70, 81, 31));
        lcdNumber_9->setStyleSheet(QLatin1String("background-color: rgb(255, 255, 255);\n"
"color: rgb(0, 0, 0);"));
        lcdNumber_9->setDigitCount(6);
        lcdNumber_9->setSegmentStyle(QLCDNumber::Flat);
        label_20 = new QLabel(centralWidget);
        label_20->setObjectName(QStringLiteral("label_20"));
        label_20->setGeometry(QRect(770, 70, 31, 30));
        label_20->setFont(font1);
        lcdNumber_10 = new QLCDNumber(centralWidget);
        lcdNumber_10->setObjectName(QStringLiteral("lcdNumber_10"));
        lcdNumber_10->setGeometry(QRect(980, 70, 81, 31));
        lcdNumber_10->setStyleSheet(QLatin1String("background-color: rgb(255, 255, 255);\n"
"color: rgb(0, 0, 0);"));
        lcdNumber_10->setDigitCount(6);
        lcdNumber_10->setSegmentStyle(QLCDNumber::Flat);
        gridLayoutWidget_2 = new QWidget(centralWidget);
        gridLayoutWidget_2->setObjectName(QStringLiteral("gridLayoutWidget_2"));
        gridLayoutWidget_2->setGeometry(QRect(890, 110, 231, 201));
        gridLayout_2 = new QGridLayout(gridLayoutWidget_2);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        gridLayout_2->setContentsMargins(0, 0, 0, 0);
        gridLayoutWidget_3 = new QWidget(centralWidget);
        gridLayoutWidget_3->setObjectName(QStringLiteral("gridLayoutWidget_3"));
        gridLayoutWidget_3->setGeometry(QRect(870, 450, 141, 341));
        gridLayout_3 = new QGridLayout(gridLayoutWidget_3);
        gridLayout_3->setSpacing(6);
        gridLayout_3->setContentsMargins(11, 11, 11, 11);
        gridLayout_3->setObjectName(QStringLiteral("gridLayout_3"));
        gridLayout_3->setContentsMargins(0, 0, 0, 0);
        gridLayoutWidget_4 = new QWidget(centralWidget);
        gridLayoutWidget_4->setObjectName(QStringLiteral("gridLayoutWidget_4"));
        gridLayoutWidget_4->setGeometry(QRect(40, 400, 731, 451));
        gridLayout_4 = new QGridLayout(gridLayoutWidget_4);
        gridLayout_4->setSpacing(6);
        gridLayout_4->setContentsMargins(11, 11, 11, 11);
        gridLayout_4->setObjectName(QStringLiteral("gridLayout_4"));
        gridLayout_4->setContentsMargins(0, 0, 0, 0);
        gridLayoutWidget_5 = new QWidget(centralWidget);
        gridLayoutWidget_5->setObjectName(QStringLiteral("gridLayoutWidget_5"));
        gridLayoutWidget_5->setGeometry(QRect(1490, 110, 231, 201));
        gridLayout_5 = new QGridLayout(gridLayoutWidget_5);
        gridLayout_5->setSpacing(6);
        gridLayout_5->setContentsMargins(11, 11, 11, 11);
        gridLayout_5->setObjectName(QStringLiteral("gridLayout_5"));
        gridLayout_5->setContentsMargins(0, 0, 0, 0);
        label_21 = new QLabel(centralWidget);
        label_21->setObjectName(QStringLiteral("label_21"));
        label_21->setGeometry(QRect(1370, 70, 31, 30));
        label_21->setFont(font1);
        gridLayoutWidget_6 = new QWidget(centralWidget);
        gridLayoutWidget_6->setObjectName(QStringLiteral("gridLayoutWidget_6"));
        gridLayoutWidget_6->setGeometry(QRect(1190, 110, 231, 201));
        gridLayout_6 = new QGridLayout(gridLayoutWidget_6);
        gridLayout_6->setSpacing(6);
        gridLayout_6->setContentsMargins(11, 11, 11, 11);
        gridLayout_6->setObjectName(QStringLiteral("gridLayout_6"));
        gridLayout_6->setContentsMargins(0, 0, 0, 0);
        label_22 = new QLabel(centralWidget);
        label_22->setObjectName(QStringLiteral("label_22"));
        label_22->setGeometry(QRect(1530, 70, 71, 31));
        label_22->setFont(font1);
        lcdNumber_11 = new QLCDNumber(centralWidget);
        lcdNumber_11->setObjectName(QStringLiteral("lcdNumber_11"));
        lcdNumber_11->setGeometry(QRect(1280, 70, 81, 31));
        lcdNumber_11->setStyleSheet(QLatin1String("background-color: rgb(255, 255, 255);\n"
"color: rgb(0, 0, 0);"));
        lcdNumber_11->setDigitCount(6);
        lcdNumber_11->setSegmentStyle(QLCDNumber::Flat);
        lcdNumber_12 = new QLCDNumber(centralWidget);
        lcdNumber_12->setObjectName(QStringLiteral("lcdNumber_12"));
        lcdNumber_12->setGeometry(QRect(1580, 70, 81, 31));
        lcdNumber_12->setStyleSheet(QLatin1String("background-color: rgb(255, 255, 255);\n"
"color: rgb(0, 0, 0);"));
        lcdNumber_12->setDigitCount(6);
        lcdNumber_12->setSegmentStyle(QLCDNumber::Flat);
        label_23 = new QLabel(centralWidget);
        label_23->setObjectName(QStringLiteral("label_23"));
        label_23->setGeometry(QRect(1230, 70, 101, 31));
        label_23->setFont(font1);
        label_24 = new QLabel(centralWidget);
        label_24->setObjectName(QStringLiteral("label_24"));
        label_24->setGeometry(QRect(1670, 70, 31, 30));
        label_24->setFont(font1);
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
        gridLayout_7 = new QGridLayout(gridLayoutWidget_7);
        gridLayout_7->setSpacing(6);
        gridLayout_7->setContentsMargins(11, 11, 11, 11);
        gridLayout_7->setObjectName(QStringLiteral("gridLayout_7"));
        gridLayout_7->setContentsMargins(0, 0, 0, 0);
        label_27 = new QLabel(centralWidget);
        label_27->setObjectName(QStringLiteral("label_27"));
        label_27->setGeometry(QRect(1240, 410, 111, 31));
        label_27->setFont(font1);
        gridLayoutWidget_8 = new QWidget(centralWidget);
        gridLayoutWidget_8->setObjectName(QStringLiteral("gridLayoutWidget_8"));
        gridLayoutWidget_8->setGeometry(QRect(1220, 450, 141, 341));
        gridLayout_8 = new QGridLayout(gridLayoutWidget_8);
        gridLayout_8->setSpacing(6);
        gridLayout_8->setContentsMargins(11, 11, 11, 11);
        gridLayout_8->setObjectName(QStringLiteral("gridLayout_8"));
        gridLayout_8->setContentsMargins(0, 0, 0, 0);
        label_28 = new QLabel(centralWidget);
        label_28->setObjectName(QStringLiteral("label_28"));
        label_28->setGeometry(QRect(1410, 410, 111, 31));
        label_28->setFont(font1);
        gridLayoutWidget_9 = new QWidget(centralWidget);
        gridLayoutWidget_9->setObjectName(QStringLiteral("gridLayoutWidget_9"));
        gridLayoutWidget_9->setGeometry(QRect(1390, 450, 141, 341));
        gridLayout_9 = new QGridLayout(gridLayoutWidget_9);
        gridLayout_9->setSpacing(6);
        gridLayout_9->setContentsMargins(11, 11, 11, 11);
        gridLayout_9->setObjectName(QStringLiteral("gridLayout_9"));
        gridLayout_9->setContentsMargins(0, 0, 0, 0);
        label_29 = new QLabel(centralWidget);
        label_29->setObjectName(QStringLiteral("label_29"));
        label_29->setGeometry(QRect(1570, 410, 111, 31));
        label_29->setFont(font1);
        gridLayoutWidget_10 = new QWidget(centralWidget);
        gridLayoutWidget_10->setObjectName(QStringLiteral("gridLayoutWidget_10"));
        gridLayoutWidget_10->setGeometry(QRect(1550, 450, 141, 341));
        gridLayout_10 = new QGridLayout(gridLayoutWidget_10);
        gridLayout_10->setSpacing(6);
        gridLayout_10->setContentsMargins(11, 11, 11, 11);
        gridLayout_10->setObjectName(QStringLiteral("gridLayout_10"));
        gridLayout_10->setContentsMargins(0, 0, 0, 0);
        label_30 = new QLabel(centralWidget);
        label_30->setObjectName(QStringLiteral("label_30"));
        label_30->setGeometry(QRect(1740, 410, 101, 31));
        label_30->setFont(font1);
        gridLayoutWidget_11 = new QWidget(centralWidget);
        gridLayoutWidget_11->setObjectName(QStringLiteral("gridLayoutWidget_11"));
        gridLayoutWidget_11->setGeometry(QRect(1710, 450, 141, 341));
        gridLayout_11 = new QGridLayout(gridLayoutWidget_11);
        gridLayout_11->setSpacing(6);
        gridLayout_11->setContentsMargins(11, 11, 11, 11);
        gridLayout_11->setObjectName(QStringLiteral("gridLayout_11"));
        gridLayout_11->setContentsMargins(0, 0, 0, 0);
        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1920, 26));
        menuRaCoS_Groundstation = new QMenu(menuBar);
        menuRaCoS_Groundstation->setObjectName(QStringLiteral("menuRaCoS_Groundstation"));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        menuBar->addAction(menuRaCoS_Groundstation->menuAction());

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0));
        label->setText(QApplication::translate("MainWindow", "IMU", 0));
        label_2->setText(QApplication::translate("MainWindow", "\302\260/s", 0));
        label_3->setText(QApplication::translate("MainWindow", "Angular rate", 0));
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
        label_18->setText(QApplication::translate("MainWindow", "Pre Valves", 0));
        label_19->setText(QApplication::translate("MainWindow", "bar", 0));
        label_20->setText(QApplication::translate("MainWindow", "bar", 0));
        label_21->setText(QApplication::translate("MainWindow", "bar", 0));
        label_22->setText(QApplication::translate("MainWindow", "Roll-", 0));
        label_23->setText(QApplication::translate("MainWindow", "Roll+", 0));
        label_24->setText(QApplication::translate("MainWindow", "bar", 0));
        label_25->setText(QApplication::translate("MainWindow", "Tank Temp", 0));
        label_26->setText(QApplication::translate("MainWindow", "Nozzle 1 Temp", 0));
        label_27->setText(QApplication::translate("MainWindow", "Nozzle 2 Temp", 0));
        label_28->setText(QApplication::translate("MainWindow", "Nozzle 3 Temp", 0));
        label_29->setText(QApplication::translate("MainWindow", "Nozzle 4 Temp", 0));
        label_30->setText(QApplication::translate("MainWindow", "PDU Temp", 0));
        menuRaCoS_Groundstation->setTitle(QApplication::translate("MainWindow", "Info", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
