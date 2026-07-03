#-------------------------------------------------
#
# Project created by QtCreator 2016-07-01T14:43:53
#
#-------------------------------------------------

QT       += network

QT       -= gui

CONFIG += C++11

TARGET = Network
TEMPLATE = lib

INCLUDEPATH += ../../lib/common

DEFINES += NETWORK_LIBRARY

SOURCES += network.cpp

HEADERS += network.h\
        network_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
