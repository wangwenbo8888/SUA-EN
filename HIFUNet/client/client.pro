#-------------------------------------------------
#
# Project created by QtCreator 2017-10-23T22:11:27
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Client
TEMPLATE = app

INCLUDEPATH += ../../lib/common/

SOURCES += main.cpp\
    clientwidget.cpp

HEADERS  += clientwidget.h

FORMS    += clientwidget.ui
CONFIG += C++11
