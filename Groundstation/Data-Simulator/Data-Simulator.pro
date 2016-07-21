#-------------------------------------------------
#
# Project created by QtCreator 2016-06-10T20:34:41
#
#-------------------------------------------------

QT       += core gui
QT       += serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Data-Simulator
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    datasimulation.cpp

HEADERS  += mainwindow.h \
    datasimulation.h \
    packages.h

FORMS    += mainwindow.ui
