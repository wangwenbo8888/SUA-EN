#-------------------------------------------------
#
# Project created by QtCreator 2016-06-22T10:53:16
#
#-------------------------------------------------

QT       += widgets

TARGET = DrawScene
TEMPLATE = lib

INCLUDEPATH += ../../lib/common

DEFINES += DRAWSCENE_LIBRARY

SOURCES += drawscene.cpp \
    measureline.cpp \
    spline.cpp \
    ellipsefit.cpp \
    vect2f.cpp

HEADERS += drawscene.h\
        drawscene_global.h \
    measureline.h \
    spline.h \
    ellipsefit.h \
    vect2f.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

if(!include(../../HifuCrashReport/HifuCrashReport.pri)){
    message(Can not add Crash Report Model.)
}
