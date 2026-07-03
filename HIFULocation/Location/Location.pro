#-------------------------------------------------
#
# Project created by QtCreator 2015-10-18T09:57:31
#
#-------------------------------------------------

QT       -= gui

TARGET = Location
TEMPLATE = lib

CONFIG += C++11

QMAKE_CXXFLAGS += /utf-8

DEFINES += LOCATION_LIBRARY

INCLUDEPATH += ../../lib/common/ \

SOURCES += location.cpp \
    loadsimcanfunc.cpp

HEADERS += location.h\
        location_global.h \
    loadsimcanfunc.h \
    uisimcanfunc_rt.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
