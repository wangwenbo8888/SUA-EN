#-------------------------------------------------
#
# Project created by QtCreator 2017-10-23T22:11:27
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Service
TEMPLATE = app

INCLUDEPATH += ../../lib/ImageGrabber/Inc \
               ../../lib/ImageGrabber \
               ../../lib/common/ \

LIBS += ../../bin/ImageGrabber.dll

SOURCES += main.cpp\
        servewidget.cpp \

HEADERS  += servewidget.h \

FORMS    += servewidget.ui
CONFIG += C++11
