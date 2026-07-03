#-------------------------------------------------
#
# Project created by QtCreator 2016-06-22T10:53:16
#
#-------------------------------------------------

QT       += widgets

TARGET = HifuRegist
TEMPLATE = lib

INCLUDEPATH += ../../lib/common/ \
               ../../lib/DrawScene/ \
               ../../dependency/opencv/include \

LIBS += ../../dependency/opencv/lib/opencv_core320.lib \
         ../../dependency/opencv/lib/opencv_core320.lib \
         ../../dependency/opencv/lib/opencv_highgui320.lib \
         ../../dependency/opencv/lib/opencv_imgproc320.lib \
         ../../dependency/opencv/lib/opencv_features2d320.lib \
         ../../dependency/opencv/lib/opencv_calib3d320.lib \
         ../../dependency/opencv/lib/opencv_imgcodecs320.lib \
         ../../bin/DrawScene.lib \

DEFINES += HIFUREGIST_LIBRARY

SOURCES += regist.cpp \
    image_algo.cpp \
    geometry_match.cpp

HEADERS += regist.h\
        regist_global.h \
    image_algo.h \
    geometry_match.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
