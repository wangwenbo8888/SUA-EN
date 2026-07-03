#-------------------------------------------------
#
# Project created by QtCreator 2016-06-22T10:53:16
#
#-------------------------------------------------

QT       += widgets
QT       += opengl

TARGET = HIFUViewer3D
TEMPLATE = lib

INCLUDEPATH += ../../lib/QGLViewer/ \
               ../../lib/GL/ \

LIBS += ../../bin/QGLViewer2.lib \

DEFINES += HIFUVIEWER3D_LIBRARY

SOURCES += \
    frameTransform.cpp \
    plane.cpp \
    spot.cpp

HEADERS +=\
    HIFUViewer3D_global.h \
    frameTransform.h \
    plane.h \
    spot.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

DISTFILES += \
    HIFUViewer3D.pro.user
