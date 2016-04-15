#-------------------------------------------------
#
# Project created by QtCreator 2016-01-10T14:15:05
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QtGSInteface
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    qneedleindicator.cpp \
    thermometer.cpp \
    abstractmeter.cpp \
    widgetwithbackground.cpp \
    functions.cpp \
    chart.cpp \
    chart/channel.cpp \
    chart/chartdecorator.cpp \
    chart/scalegrid.cpp \
    chart/standard/channeldecorator.cpp \
    chart/standard/glassdecorator.cpp \
    chart/standard/labelsdecorator.cpp \
    chart/standard/legenddecorator.cpp \
    chart/standard/scalegriddecorator.cpp \
    led.cpp \
    commandwindow.cpp

HEADERS  += mainwindow.h \
    qneedleindicator.h \
    thermometer.h \
    abstractmeter.h \
    widgetwithbackground.h \
    functions.h \
    chart.h \
    chart/channel.h \
    chart/channeldata.h \
    chart/chartdecorator.h \
    chart/datacontainers.h \
    chart/scalegrid.h \
    chart/standard/channeldecorator.h \
    chart/standard/glassdecorator.h \
    chart/standard/labelsdecorator.h \
    chart/standard/legenddecorator.h \
    chart/standard/scalegriddecorator.h \
    led.h \
    commandwindow.h

FORMS    += mainwindow.ui \
    commandwindow.ui
