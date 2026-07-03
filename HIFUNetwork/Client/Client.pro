#-------------------------------------------------
#
# Project created by QtCreator 2016-05-31T13:52:13
#
#-------------------------------------------------

QT       += network

QT       -= gui
QT       += widgets

CONFIG += C++11

TARGET = Client
TEMPLATE = lib

DEFINES += CLIENT_LIBRARY

INCLUDEPATH += ../../lib/common

SOURCES += client.cpp

HEADERS += client.h\
        client_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
