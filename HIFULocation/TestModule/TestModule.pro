#-------------------------------------------------
#
# Project created by QtCreator 2015-10-23T08:44:11
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TestModule
TEMPLATE = app

INCLUDEPATH += ../../lib/common \
               ../../lib/Location \
               ../../lib/Location/Inc

LIBS += ../bin/Location.dll

SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h
