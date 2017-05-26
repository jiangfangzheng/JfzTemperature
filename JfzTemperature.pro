#-------------------------------------------------
#
# Project created by QtCreator 2016-12-13T15:04:01
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = JfzTemperature
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui
RC_ICONS = poi.ico
QT+= serialport
QT += network
