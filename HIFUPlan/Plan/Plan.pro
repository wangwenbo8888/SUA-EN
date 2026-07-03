#-------------------------------------------------
#
# Project created by QtCreator 2016-06-02T10:49:45
#
#-------------------------------------------------

QT       -= gui

TARGET = Plan
TEMPLATE = lib

DEFINES += PLAN_LIBRARY

SOURCES += plan.cpp

HEADERS += plan.h\
        plan_global.h

INCLUDEPATH += ../../lib/common

unix {
    target.path = /usr/lib
    INSTALLS += target
}
