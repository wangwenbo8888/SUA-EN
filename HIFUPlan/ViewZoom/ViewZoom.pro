#-------------------------------------------------
#
# Project created by QtCreator 2016-07-13T14:55:57
#
#-------------------------------------------------

QT       += widgets

QT       -= gui

TARGET = ViewZoom
TEMPLATE = lib

DEFINES += VIEWZOOM_LIBRARY

SOURCES += viewzoom.cpp

HEADERS += viewzoom.h\
        viewzoom_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
