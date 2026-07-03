#-------------------------------------------------
#
# Project created by QtCreator 2016-07-18T10:29:21
#
#-------------------------------------------------

QT       -= gui

TARGET = Message
TEMPLATE = lib

INCLUDEPATH += ../../lib/common

DEFINES += MESSAGE_LIBRARY

SOURCES += message.cpp

HEADERS += message.h\
        message_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
