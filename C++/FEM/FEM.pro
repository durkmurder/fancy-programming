#-------------------------------------------------
#
# Project created by QtCreator 2014-10-09T19:26:26
#
#-------------------------------------------------

QT       += core

#greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = FEM
TEMPLATE = app


SOURCES += src/main.cpp\
        #src/mainwindow.cpp \
    src/integral.cpp \
    src/rectintegration.cpp \
    src/trapintegration.cpp \
    src/gaussintegration.cpp \
    src/parabintegral.cpp

HEADERS  += src/integral.h \
    #src/mainwindow.h \
    src/rectintegration.h \
    src/trapintegration.h \
    src/gaussintegration.h \
    src/parabintegral.h

#FORMS    += src/mainwindow.ui

CONFIG += c++11
