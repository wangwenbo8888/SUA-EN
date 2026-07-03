#-------------------------------------------------
#
# Project created by QtCreator 2016-06-22T10:53:16
#
#-------------------------------------------------

QT       += widgets

TARGET = HifuImageAlgo
TEMPLATE = lib

INCLUDEPATH += ../../lib/common/ \
               ../../lib/DrawScene/ \
               ../../lib/RealtimeScene/ \
               ../../dependency/opencv/include \



LIBS += ../../bin/RealtimeScene.lib \
        ../../bin/DrawScene.lib \

LIBS +=  ../../bin/opencv_core320.lib \
         ../../bin/opencv_core320.lib \
         ../../bin/opencv_highgui320.lib \
         ../../bin/opencv_imgproc320.lib \
         ../../bin/opencv_features2d320.lib \
         ../../bin/opencv_calib3d320.lib \
         ../../bin/opencv_imgcodecs320.lib \



DEFINES += HIFUIMAGEALGO_LIBRARY

SOURCES += \
    image_algo.cpp \
    geometry_match.cpp \
    hifuimagealgo.cpp

HEADERS +=\
    image_algo.h \
    geometry_match.h \
    hifuimagealgo.h \
    hifuimagealgo_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
