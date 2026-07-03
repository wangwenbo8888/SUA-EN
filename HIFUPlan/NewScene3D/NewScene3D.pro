#-------------------------------------------------
#
# Project created by QtCreator 2017-11-07T14:19:46
#
#-------------------------------------------------

QT       -= gui
QT       += widgets opengl


TARGET = NewScene3D
TEMPLATE = lib

DEFINES += NEWSCENE3D_LIBRARY

SOURCES += newscene3d.cpp \
    qvtkwrap.cpp

HEADERS += newscene3d.h\
        newscene3d_global.h \
    qvtkwrap.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

INCLUDEPATH += $$PWD/../../dependency/VTK/include/vtk-8.2 \

LIBS += -L$$PWD/../../dependency/VTK/lib \
        -lvtkRenderingCore-8.2 \
        -lvtkRenderingOpenGL2-8.2 \
        -lvtkCommonCore-8.2 \
        -lvtkImagingCore-8.2 \
        -lvtkInteractionStyle-8.2 \
        -lvtkInteractionImage-8.2 \
        -lvtkIOImage-8.2 \
        -lvtkCommonExecutionModel-8.2 \
        -lvtkCommonDataModel-8.2 \
        -lvtkGUISupportQt-8.2 \
        -lvtkImagingSources-8.2 \
        -lvtkRenderingImage-8.2 \
        -lvtkFiltersSources-8.2 \
        -lvtkFiltersCore-8.2 \
        -lvtkRenderingAnnotation-8.2 \
        -lvtkInteractionWidgets-8.2 \
        -lvtkIOGeometry-8.2 \
#        -lvtkGUISupportQtOpenGL-8.2 \
        -lvtkFiltersGeneral-8.2 \
        -lvtkFiltersModeling-8.2 \
        -lvtkCommonTransforms-8.2 \
        -lvtkRenderingQt-8.2 \
        -lvtkRenderingFreeType-8.2 \

RESOURCES += \
    img.qrc

