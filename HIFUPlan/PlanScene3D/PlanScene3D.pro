#-------------------------------------------------
#
# Project created by QtCreator 2017-11-07T14:19:46
#
#-------------------------------------------------

QT       -= gui
QT       += widgets opengl


TARGET = PlanScene3D
TEMPLATE = lib

DEFINES += PLANSCENE3D_LIBRARY

SOURCES += planscene3d.cpp \
    qvtkwrap.cpp

HEADERS += planscene3d.h\
        planscene3d_global.h \
    qvtkwrap.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

INCLUDEPATH += $$PWD/../../dependency/VTK/include/vtk-8.2 \

#LIBS += -L$$PWD/../../dependency/VTK/bin \
#        -llibvtkRenderingCore-8.2 \
#        -llibvtkRenderingOpenGL-8.2 \
#        -llibvtkCommonCore-8.2 \
#        -llibvtkImagingCore-8.2 \
#        -llibvtkInteractionStyle-8.2 \
#        -llibvtkInteractionImage-8.2 \
#        -llibvtkIOImage-8.2 \
#        -llibvtkCommonExecutionModel-8.2 \
#        -llibvtkCommonDataModel-8.2 \
#        -llibvtkGUISupportQt-8.2 \
#        -llibvtkImagingSources-8.2 \
#        -llibvtkRenderingImage-8.2 \
#        -llibvtkFiltersSources-8.2 \
#        -llibvtkFiltersCore-8.2 \
#        -llibvtkRenderingAnnotation-8.2 \
#        -llibvtkInteractionWidgets-8.2 \
#        -llibvtkIOGeometry-8.2 \
#        -llibvtkGUISupportQtOpenGL-8.2 \
#        -llibvtkFiltersGeneral-8.2 \
#        -llibvtkFiltersModeling-8.2 \
#        -llibvtkCommonTransforms-8.2 \
#        -llibvtkRenderingQt-8.2 \

LIBS += ../../bin/vtkRenderingCore-8.2.lib \
        ../../bin/vtkRenderingOpenGL2-8.2.lib \
        ../../bin/vtkCommonCore-8.2.lib \
        ../../bin/vtkInteractionStyle-8.2.lib \
        ../../bin/vtkImagingCore-8.2.lib \
        ../../bin/vtkInteractionImage-8.2.lib \
        ../../bin/vtkIOImage-8.2.lib \
        ../../bin/vtkCommonExecutionModel-8.2.lib \
        ../../bin/vtkCommonDataModel-8.2.lib \
        ../../bin/vtkGUISupportQt-8.2.lib \
        ../../bin/vtkImagingSources-8.2.lib \
        ../../bin/vtkRenderingImage-8.2.lib \
        ../../bin/vtkFiltersSources-8.2.lib \
        ../../bin/vtkFiltersCore-8.2.lib \
        ../../bin/vtkRenderingAnnotation-8.2.lib \
        ../../bin/vtkInteractionWidgets-8.2.lib \
        ../../bin/vtkIOGeometry-8.2.lib \
        ../../bin/vtkDICOMParser-8.2.lib \
        ../../bin/vtkViewsCore-8.2.lib \
        ../../bin/vtkRenderingFreeType-8.2.lib \
        ../../bin/vtkImagingGeneral-8.2.lib \
        ../../bin/vtkImagingFourier-8.2.lib \
        ../../bin/vtkImagingStatistics-8.2.lib \
        ../../bin/vtkImagingColor-8.2.lib \
        ../../bin/vtkImagingHybrid-8.2.lib \
        ../../bin/vtkImagingMath-8.2.lib \
        ../../bin/vtkImagingMorphological-8.2.lib \
        ../../bin/vtkImagingStencil-8.2.lib \
        ../../bin/vtkRenderingLOD-8.2.lib \
        ../../bin/vtkRenderingLabel-8.2.lib \
        ../../bin/vtkViewsInfovis-8.2.lib \
        ../../bin/vtkFiltersModeling-8.2.lib \
        ../../bin/vtkFiltersGeometry-8.2.lib \
        ../../bin/vtkRenderingQt-8.2.lib \
        ../../bin/vtkIOCore-8.2.lib \
        ../../bin/vtkIOImport-8.2.lib \
        ../../bin/vtkFiltersGeneral-8.2.lib \
        ../../bin/vtkCommonTransforms-8.2.lib \
        ../../bin/vtkCommonMath-8.2.lib \
        ../../bin/vtkCommonMisc-8.2.lib \
        ../../bin/vtkCommonComputationalGeometry-8.2.lib \
        ../../bin/vtkfreetype-8.2.lib \
        ../../bin/vtkRenderingImage-8.2.lib \


#RESOURCES += \
#    img.qrc

