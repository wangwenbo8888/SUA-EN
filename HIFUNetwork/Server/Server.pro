#-------------------------------------------------
#
# Project created by QtCreator 2016-05-31T09:13:40
#
#-------------------------------------------------

QT       += network

QT       -= gui

TARGET = Server
TEMPLATE = lib

DEFINES += SERVER_LIBRARY

SOURCES += server.cpp

HEADERS += server.h\
        server_global.h

INCLUDEPATH += ../../lib/common

unix {
    target.path = /usr/lib
    INSTALLS += target
}
