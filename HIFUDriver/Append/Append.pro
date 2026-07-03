#-------------------------------------------------
#
# Project created by QtCreator 2017-07-27T14:18:36
#
#-------------------------------------------------

QT       -= gui
QT       += serialport

TARGET = Append
TEMPLATE = lib

INCLUDEPATH += $$PWD/../../lib/common

DEFINES += APPEND_LIBRARY

SOURCES += append.cpp \
    serialworker.cpp

HEADERS += append.h\
        append_global.h \
    serialworker.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
