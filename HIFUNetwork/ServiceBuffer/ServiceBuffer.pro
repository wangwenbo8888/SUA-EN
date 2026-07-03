#-------------------------------------------------
#
# Project created by QtCreator 2016-06-02T14:46:58
#
#-------------------------------------------------

QT       -= gui

TARGET = ServiceBuffer
TEMPLATE = lib

DEFINES += SERVICEBUFFER_LIBRARY

SOURCES += servicebuffer.cpp

HEADERS += servicebuffer.h\
        servicebuffer_global.h

INCLUDEPATH += ../../lib/Plan \
               ../../lib/common \
               ../../lib/Driver

unix {
    target.path = /usr/lib
    INSTALLS += target
}
