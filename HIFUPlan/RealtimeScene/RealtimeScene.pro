#-------------------------------------------------
#
# Project created by QtCreator 2016-06-22T10:13:43
#
#-------------------------------------------------

QT       += widgets

TARGET = RealtimeScene
TEMPLATE = lib

DEFINES += REALTIMESCENE_LIBRARY

INCLUDEPATH += ../DrawScene \
               ../../lib/common \

SOURCES += realtimescene.cpp \
    adjustwindow.cpp \
    adjcontrast.cpp

HEADERS += realtimescene.h\
        realtimescene_global.h \
    adjustwindow.h \
    adjcontrast.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
