#-------------------------------------------------
#
# Project created by QtCreator 2018-01-05T08:47:59
#
#-------------------------------------------------

QT       += core gui
QT       += serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = debug
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    serial.cpp \
    cardcom.cpp

HEADERS  += mainwindow.h \
    serial.h \
    cardcom.h \
    data.h

FORMS    += mainwindow.ui
