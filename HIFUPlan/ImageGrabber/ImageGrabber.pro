#-------------------------------------------------
#
# Project created by QtCreator 2016-06-21T09:24:32
#
#-------------------------------------------------

QT       += gui

TARGET = ImageGrabberTc
TEMPLATE = lib

DEFINES -= UNICODE
DEFINES += UMBCS
QMAKE_CXXFLAGS -= -Zc:strictStrings

DEFINES += IMAGEGRABBER_LIBRARY

INCLUDEPATH += ../../lib/ImageGrabber/QCap/ \
               ../../lib/common/

LIBS += ../../bin/QCAP.X64.LIB \
        ole32.lib

SOURCES += imagegrabber.cpp

HEADERS += imagegrabber.h\
        imagegrabber_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
