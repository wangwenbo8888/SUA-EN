#-------------------------------------------------
#
# Project created by QtCreator 2022-07-15T13:30:03
#
#-------------------------------------------------

QT       -= gui
QT       += serialport

TARGET = Q_Water
TEMPLATE = lib

DEFINES += Q_WATER_LIBRARY

SOURCES += q_water.cpp

HEADERS += q_water.h\
        q_water_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
