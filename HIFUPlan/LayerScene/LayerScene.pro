A#-------------------------------------------------
#
# Project created by QtCreator 2016-06-22T10:53:16
#
#-------------------------------------------------

QT       += widgets

TARGET = LayerScene
TEMPLATE = lib

INCLUDEPATH += ../../lib/common \
               ../../lib/DrawScene/ \

DEFINES += LAYERSCENE_LIBRARY
SOURCES += layerscene.cpp

HEADERS += layerscene.h\
        layerscene_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

if(!include(../../HifuCrashReport/HifuCrashReport.pri)){
    message(Can not add Crash Report Model.)
}
