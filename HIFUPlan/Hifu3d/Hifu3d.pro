#-------------------------------------------------
#
# Project created by QtCreator 2016-06-22T10:53:16
#
#-------------------------------------------------

QT       += widgets

TARGET = Hifu3d
TEMPLATE = lib

DESTDIR += E:\USgHIFU\HIFUPlan\build-Hifu3dTest-Desktop_Qt_5_4_1_MinGW_32bit-Release\release \

INCLUDEPATH += ../../lib/common/ \
               ../../lib/DrawScene/ \

LIBS += ../../bin/DrawScene.lib \

DEFINES += HIFU3D_LIBRARY

SOURCES += hifu3d.cpp \
    MarchingCubes.cpp \
    MCHashTable.cpp \
    vector3d.cpp \
    mesh.cpp \
    region_growing.cpp \
    region_growing_data_struct.cpp \
    smooth.cpp \
    createmodel.cpp

HEADERS += hifu3d.h\
        hifu3d_global.h \
    MCHasTable.h \
    MCTablethin.h \
    vector3d.h \
    marchingcubes.h \
    mesh.h \
    region_growing_data_struct.h \
    region_growing.h \
    smooth.h \
    createmodel.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

DISTFILES += \
    Hifu3d.pro.user
