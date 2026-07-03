#-------------------------------------------------
#
# Project created by QtCreator 2016-02-23T10:02:24
#
#-------------------------------------------------

QT       -= gui
QT       += serialport

TARGET = PowerAmp
TEMPLATE = lib

INCLUDEPATH += ../lib/common

DEFINES += POWERAMP_LIBRARY

SOURCES += poweramp.cpp

HEADERS += poweramp.h\
        poweramp_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
