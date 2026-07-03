#-------------------------------------------------
#
# Project created by QtCreator 2016-02-26T10:27:48
#
#-------------------------------------------------

QT       -= gui
QT       += serialport

TARGET = PhaseController
TEMPLATE = lib

INCLUDEPATH += ../../lib/common

DEFINES += PHASECONTROLLER_LIBRARY

SOURCES += phasecontroller.cpp

HEADERS += phasecontroller.h\
        phasecontroller_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
