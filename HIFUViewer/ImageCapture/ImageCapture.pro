#-------------------------------------------------
#
# Project created by QtCreator 2015-11-08T08:16:16
#
#-------------------------------------------------

QT       += gui

TARGET = ImageCapture
TEMPLATE = lib

DEFINES += IMAGECAPTURE_LIBRARY

INCLUDEPATH += ../lib/ImageCapture/Inc

LIBS += ../../bin/OKAPI64.lib

SOURCES += \
    imagecapture.cpp

HEADERS +=\
        imagecapture_global.h \
    imagecapture.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
