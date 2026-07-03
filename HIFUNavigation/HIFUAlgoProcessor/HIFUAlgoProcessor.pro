#-------------------------------------------------
#
# Project created by QtCreator 2017-09-29T17:23:41
#
#-------------------------------------------------

QT       -= gui

TARGET = HIFUAlgoProcessor
TEMPLATE = lib

DEFINES += HIFUALGOPROCESSOR_LIBRARY

INCLUDEPATH += ../../dependency/opencv/include \

#LIBS += ../../dependency/opencv/bin/libopencv_core320.dll \
#         ../../dependency/opencv/bin/libopencv_core320.dll \
#         ../../dependency/opencv/bin/libopencv_highgui320.dll \
#         ../../dependency/opencv/bin/libopencv_imgproc320.dll \
#         ../../dependency/opencv/bin/libopencv_features2d320.dll \
#         ../../dependency/opencv/bin/libopencv_calib3d320.dll \
#         ../../dependency/opencv/bin/libopencv_imgcodecs320.dll \
LIBS += ../../bin/opencv_core320.lib \
        ../../bin/opencv_core320.lib \
        ../../bin/opencv_highgui320.lib \
        ../../bin/opencv_imgproc320.lib \
        ../../bin/opencv_features2d320.lib \
        ../../bin/opencv_calib3d320.lib \
        ../../bin/opencv_imgcodecs320.lib \


SOURCES += hifualgoprocessor.cpp \
    SeededGrow/seededgrow3d.cpp \
    SurfaceReconstruction/smc_adsmcprocessor.cpp

HEADERS += hifualgoprocessor.h\
        hifualgoprocessor_global.h \
        SeededGrow/seededgrow3d.h \
        base3d.h \
        SurfaceReconstruction/smc_adsmcprocessor.h \
        smc_mesh.h \
        SurfaceReconstruction/smc_mesh.h \
        SurfaceReconstruction/smc_table.h \
        SurfaceReconstruction/smc_cube.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

#DLLDESTDIR += $$PWD/../build-HIFUNavigationTest-Desktop_Qt_5_4_1_MinGW_32bit-Debug/debug/
#DLLDESTDIR += $$PWD/../build-HIFUNavigation-Desktop_Qt_5_4_1_MinGW_32bit-Debug/debug/
