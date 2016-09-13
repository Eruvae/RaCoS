#-------------------------------------------------
#
# Project created by QtCreator 2016-01-10T14:15:05
#
#-------------------------------------------------

QT += core gui
QT += 3dcore 3drender 3dinput 3dlogic 3dextras
QT += serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QtGSInteface
TEMPLATE = app


SOURCES += main.cpp \
    murmur.cpp \
    mainwindow.cpp \
    commandwindow.cpp \
    GUIWidgets/chart/standard/channeldecorator.cpp \
    GUIWidgets/chart/standard/glassdecorator.cpp \
    GUIWidgets/chart/standard/labelsdecorator.cpp \
    GUIWidgets/chart/standard/legenddecorator.cpp \
    GUIWidgets/chart/standard/scalegriddecorator.cpp \
    GUIWidgets/chart/channel.cpp \
    GUIWidgets/chart/chartdecorator.cpp \
    GUIWidgets/chart/scalegrid.cpp \
    GUIWidgets/abstractmeter.cpp \
    GUIWidgets/chart.cpp \
    GUIWidgets/functions.cpp \
    GUIWidgets/led.cpp \
    GUIWidgets/qneedleindicator.cpp \
    GUIWidgets/thermometer.cpp \
    GUIWidgets/widgetwithbackground.cpp \
    rocket.cpp

HEADERS  += mainwindow.h \
    murmur.h \
    commandwindow.h \
    GUIWidgets/chart/standard/channeldecorator.h \
    GUIWidgets/chart/standard/glassdecorator.h \
    GUIWidgets/chart/standard/labelsdecorator.h \
    GUIWidgets/chart/standard/legenddecorator.h \
    GUIWidgets/chart/standard/scalegriddecorator.h \
    GUIWidgets/chart/channel.h \
    GUIWidgets/chart/channeldata.h \
    GUIWidgets/chart/chartdecorator.h \
    GUIWidgets/chart/datacontainers.h \
    GUIWidgets/chart/scalegrid.h \
    GUIWidgets/abstractmeter.h \
    GUIWidgets/chart.h \
    GUIWidgets/functions.h \
    GUIWidgets/led.h \
    GUIWidgets/qneedleindicator.h \
    GUIWidgets/thermometer.h \
    GUIWidgets/widgetwithbackground.h \
    packages.h \
    rocket.h

FORMS    += mainwindow.ui \
    commandwindow.ui

RESOURCES += \
    rocket.qrc
