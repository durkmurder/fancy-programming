#-------------------------------------------------
#
# Project created by QtCreator 2014-10-09T19:26:26
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = FEM
TEMPLATE = app


SOURCES += src/main.cpp\
        src/mainwindow.cpp \
    src/integral.cpp \
    src/rectintegration.cpp \
    src/trapintegration.cpp \
    src/gaussintegration.cpp

HEADERS  += src/mainwindow.h \
    src/integral.h \
    src/rectintegration.h \
    src/trapintegration.h \
    src/gaussintegration.h

FORMS    += src/mainwindow.ui

CONFIG += c++11
