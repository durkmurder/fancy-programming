#-------------------------------------------------
#
# Project created by QtCreator 2015-07-01T21:01:32
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = FuturesTry
TEMPLATE = app

CONFIG += c++11

SOURCES += main.cpp\
        mainwindow.cpp \
    taskmodel.cpp \
    datareader.cpp

HEADERS  += mainwindow.h \
    taskmodel.h \
    datareader.h \
    task.hpp

FORMS    += mainwindow.ui

DISTFILES += \
    data.txt
