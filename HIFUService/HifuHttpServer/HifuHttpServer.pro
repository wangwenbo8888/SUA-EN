#-------------------------------------------------
#
# Project created by QtCreator 2018-03-14T15:12:46
#
#-------------------------------------------------

QT       += network concurrent

QT       -= gui

TARGET = HifuHttpServer
TEMPLATE = lib

include( $$PWD/JQLibrary/JQLibrary.pri )

DEFINES += HIFUHTTPSERVER_LIBRARY

SOURCES += hifuhttpserver.cpp

HEADERS += hifuhttpserver.h\
        hifuhttpserver_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
