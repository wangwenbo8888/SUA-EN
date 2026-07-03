#-------------------------------------------------
#
# Project created by QtCreator 2020-12-01T10:36:46
#
#-------------------------------------------------

QT       += widgets opengl

TARGET = HIFUConnection
TEMPLATE = lib

DEFINES += HIFUCONNECTION_LIBRARY

SOURCES += \
    maindialog.cpp \
    #OperationDialog/thresholddialog.cpp \
    #OperationDialog/regiongrowingdialog.cpp \
    #OperationDialog/surfacerenderingdialog.cpp \
    HIFUConnection.cpp


HEADERS +=\
        HIFUConnection_global.h \
    maindialog.h \
    #OperationDialog/thresholddialog.h \
    #OperationDialog/regiongrowingdialog.h \
    #OperationDialog/surfacerenderingdialog.h \
    HIFUConnection.h


unix {
    target.path = /usr/lib
    INSTALLS += target
}

win32-g++* {
    QMAKE_LFLAGS += -Wl,--large-address-aware
    QMAKE_CFLAGS_RELEASE += -g
    QMAKE_CXXFLAGS_RELEASE += -g
    QMAKE_LFLAGS_RELEASE =
} else:win32 {
    QMAKE_LFLAGS += /LARGEADDRESSAWARE
}

FORMS += \
    #OperationDialog/thresholddialog.ui \
    #OperationDialog/regiongrowingdialog.ui \
    #OperationDialog/surfacerenderingdialog.ui \
    maindialog.ui

#INCLUDEPATH +=$$PWD/../HIFUAlgoProcessor/

#LIBS += ../../bin/HIFUAlgoProcessor.dll

INCLUDEPATH += ../../dependency/dcmtk/include \
               ../../lib/common/ \

#LIBS +=  ../../dependency/dcmtk/bin/libdcmqrdb.dll \
#         ../../dependency/dcmtk/bin/libdcmnet.dll \
#         #../../dependency/opencv/bin/libopencv_core320.dll \
#         #../../dependency/opencv/bin/libopencv_    core320.dll \
#         #../../dependency/opencv/bin/libopencv_highgui320.dll \
#         #../../dependency/opencv/bin/libopencv_imgproc320.dll \
#         #../../dependency/opencv/bin/libopencv_features2d320.dll \
#         #../../dependency/opencv/bin/libopencv_calib3d320.dll \
#         #../../dependency/opencv/bin/libopencv_imgcodecs320.dll \



INCLUDEPATH +=../../dependency/GDCM/include/gdcm-2.8


LIBS += ../../dependency/GDCM/lib/gdcmCommon.lib \
        ../../dependency/GDCM/lib/gdcmcharls.lib \
        ../../dependency/GDCM/lib/gdcmDICT.lib \
        ../../dependency/GDCM/lib/gdcmIOD.lib \
        ../../dependency/GDCM/lib/gdcmDSED.lib \
        ../../dependency/GDCM/lib/gdcmexpat.lib \
        ../../dependency/GDCM/lib/gdcmgetopt.lib \
        ../../dependency/GDCM/lib/gdcmMEXD.lib \
        ../../dependency/GDCM/lib/gdcmMSFF.lib \
        ../../dependency/GDCM/lib/gdcmopenjp2.lib \
        ../../dependency/GDCM/lib/gdcmzlib.lib \
        ../../dependency/GDCM/lib/socketxx.lib \
        ../../dependency/GDCM/lib/gdcmopenjp2.lib \

#LIBS += -L../../dependency/GDCM/bin \
#        -llibgdcmCommon \
#        -llibgdcmcharls \
#        -llibgdcmDICT \
#        -llibgdcmIOD \
#        -llibgdcmDSED \
#        -llibgdcmexpat \
#        -llibgdcmgetopt \
#        -llibgdcmMEXD \
#        -llibgdcmMSFF \
#        -llibgdcmopenjp2 \
#        -llibgdcmzlib \
#        -llibsocketxx \
#        -llibgdcmopenjp2 \



DLLDESTDIR += ../../bin \

RESOURCES += \
    resource.qrc \

include(../../dependency/Armadillo/armad.pri)


