#-------------------------------------------------
#
# Project created by QtCreator 2017-09-29T15:36:46
#
#-------------------------------------------------

QT       += widgets opengl

TARGET = HIFUNavigation
TEMPLATE = lib

DEFINES += HIFUNAVIGATION_LIBRARY

SOURCES += hifunavigation.cpp \
    maindialog.cpp \
    OperationDialog/thresholddialog.cpp \
    OperationDialog/regiongrowingdialog.cpp \
    OperationDialog/surfacerenderingdialog.cpp \
    myvtkcontourwidget.cpp \
    fitalgorithm.cpp

HEADERS += hifunavigation.h\
        hifunavigation_global.h \
    maindialog.h \
    OperationDialog/thresholddialog.h \
    algobase.h \
    OperationDialog/regiongrowingdialog.h \
    OperationDialog/surfacerenderingdialog.h \
    myvtkcontourwidget.h \
    fitalgorighm.h

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
    OperationDialog/thresholddialog.ui \
    OperationDialog/regiongrowingdialog.ui \
    OperationDialog/surfacerenderingdialog.ui \
    maindialog.ui

INCLUDEPATH +=$$PWD/../HIFUAlgoProcessor/

LIBS += ../../bin/HIFUAlgoProcessor.lib \
        ../../bin/HIFUConnection.lib \

INCLUDEPATH += ../../dependency/VTK/include/vtk-8.2 \
               ../../dependency/opencv/include \
               ../../lib/common/ \
               ../../lib/HIFUConnection/ \




#LIBS += ../../bin/opencv_core320.lib \
#        ../../bin/opencv_core320.lib \
#        ../../bin/opencv_highgui320.lib \
#        ../../bin/opencv_imgproc320.lib \
#        ../../bin/opencv_features2d320.lib \
#        ../../bin/opencv_calib3d320.lib \
#        ../../bin/opencv_imgcodecs320.lib \
#        -L../../bin \
#        -lvtkRenderingCore-6.3 \
#        -lvtkCommonCore-6.3 \
#        -lvtkDICOMParser-6.3 \
#        -lvtkViewsCore-6.3 \
#        -lvtkImagingCore-6.3 \
#        -lvtkInteractionStyle-6.3 \
#        -lvtkRenderingFreeType-6.3 \
#        -lvtkInteractionImage-6.3 \
#        -lvtkIOImage-6.3 \
#        -lvtkCommonExecutionModel-6.3 \
#        -lvtkGUISupportQt-6.3 \
#        -lvtkImagingSources-6.3 \
#        -lvtkImagingGeneral-6.3 \
#        -lvtkImagingFourier-6.3 \
#        -lvtkImagingStatistics-6.3 \
#        -lvtkImagingColor-6.3 \
#        -lvtkImagingHybrid-6.3 \
#        -lvtkImagingMath-6.3 \
#        -lvtkImagingMorphological-6.3 \
#        -lvtkImagingStencil-6.3 \
#        -lvtkCommonDataModel-6.3 \
#        -lvtkRenderingLOD-6.3 \
#        -lvtkRenderingAnnotation-6.3 \
#        -lvtkRenderingLabel-6.3 \
#        -lvtkViewsInfovis-6.3 \
#        -lvtkfreetype-6.3 \
#        -lvtkInteractionWidgets-6.3 \
#        -lvtkFiltersModeling-6.3 \
#        -lvtkFiltersGeometry-6.3 \
#        -lvtkRenderingQt-6.3 \
#        -lvtkRenderingOpenGL-6.3 \
#        -lvtkIOCore-6.3 \
#        -lvtkIOImport-6.3 \
#        -lvtkIOGeometry-6.3 \
#        -lvtkFiltersGeneral-6.3 \
#        -lvtkFiltersCore-6.3 \
#        -lvtkCommonTransforms-6.3 \
#        -lvtkCommonMath-6.3 \
#        -lvtkFiltersSources-6.3 \
#        -lvtkCommonMisc-6.3 \
#        -lvtkCommonComputationalGeometry-6.3 \
#        -lvtkRenderingImage-6.3 \

#LIBS += ../../dependency/opencv/bin/libopencv_core320.dll \
#         ../../dependency/opencv/bin/libopencv_core320.dll \
#         ../../dependency/opencv/bin/libopencv_highgui320.dll \
#         ../../dependency/opencv/bin/libopencv_imgproc320.dll \
#         ../../dependency/opencv/bin/libopencv_features2d320.dll \
#         ../../dependency/opencv/bin/libopencv_calib3d320.dll \
#         ../../dependency/opencv/bin/libopencv_imgcodecs320.dll \
#        -L../../dependency/VTK/bin \
#        -llibvtkRenderingCore-6.3 \
#        -llibvtkCommonCore-6.3 \
#        -llibvtkDICOMParser-6.3 \
#        -llibvtkViewsCore-6.3 \
#        -llibvtkImagingCore-6.3 \
#        -llibvtkInteractionStyle-6.3 \
#        -llibvtkRenderingFreeType-6.3 \
#        -llibvtkInteractionImage-6.3 \
#        -llibvtkIOImage-6.3 \
#        -llibvtkCommonExecutionModel-6.3 \
#        -llibvtkGUISupportQt-6.3 \
#        -llibvtkImagingSources-6.3 \
#        -llibvtkImagingGeneral-6.3 \
#        -llibvtkImagingFourier-6.3 \
#        -llibvtkImagingStatistics-6.3 \
#        -llibvtkImagingColor-6.3 \
#        -llibvtkImagingHybrid-6.3 \
#        -llibvtkImagingMath-6.3 \
#        -llibvtkImagingMorphological-6.3 \
#        -llibvtkImagingStencil-6.3 \
#        -llibvtkCommonDataModel-6.3 \
#        -llibvtkRenderingLOD-6.3 \
#        -llibvtkRenderingAnnotation-6.3 \
#        -llibvtkRenderingLabel-6.3 \
#        -llibvtkViewsInfovis-6.3 \
#        -llibvtkfreetype-6.3 \
#        -llibvtkInteractionWidgets-6.3 \
#        -llibvtkFiltersModeling-6.3 \
#        -llibvtkFiltersGeometry-6.3 \
#        -llibvtkRenderingQt-6.3 \
#        -llibvtkRenderingOpenGL-6.3 \
#        -llibvtkIOCore-6.3 \
#        -llibvtkIOImport-6.3 \
#        -llibvtkIOGeometry-6.3 \
#        -llibvtkFiltersGeneral-6.3 \
#        -llibvtkFiltersCore-6.3 \
#        -llibvtkCommonTransforms-6.3 \
#        -llibvtkCommonMath-6.3 \
#        -llibvtkFiltersSources-6.3 \
#        -llibvtkCommonMisc-6.3 \
#        -llibvtkCommonComputationalGeometry-6.3 \

LIBS += ../../bin/opencv_core320.lib \
        ../../bin/opencv_core320.lib \
        ../../bin/opencv_highgui320.lib \
        ../../bin/opencv_imgproc320.lib \
        ../../bin/opencv_features2d320.lib \
        ../../bin/opencv_calib3d320.lib \
        ../../bin/opencv_imgcodecs320.lib \
        ../../bin/vtkRenderingCore-8.2.lib \
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


