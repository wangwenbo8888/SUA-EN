#-------------------------------------------------
#
# Project created by QtCreator 2016-03-01T14:04:35
#
#-------------------------------------------------

QT       -= gui
QT       += serialport

CONFIG   += c++11

TARGET = Driver
TEMPLATE = lib

INCLUDEPATH += ../../lib/common \
               ../../lib/PowerAmp \
               ../../lib/PhaseController \
               ../../lib/Plan

LIBS += ../../bin/PowerAmp.lib \
        ../../bin/PhaseController.lib \
        ../../bin/Plan.lib

DEFINES += DRIVER_LIBRARY

SOURCES += driver.cpp \
    phasecalc.cpp

HEADERS += driver.h\
        driver_global.h \
    phasecalc.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
