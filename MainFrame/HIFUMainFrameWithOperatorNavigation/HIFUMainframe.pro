 #-------------------------------------------------
#
# Project created by QtCreator 2016-06-16T13:29:47
#
#-------------------------------------------------
#RC_FILE+= favicon.rc
QT       += core gui network serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
QT += qml quick widgets
QT += sql
QT += opengl
QT += printsupport
QT += multimedia multimediawidgets websockets network
#QT += widgets opengl
RC_ICONS = favicon.ico


TARGET = HIFUMainFrame
TEMPLATE = app

CONFIG += C++11 force_debug_info

VERSION = 0.1.180413

QMAKE_TARGET_PRODUCT = HIFUMainframe.exe
QMAKE_TARGET_COMPANY = zhonghui medical
QMAKE_TARGET_DESCRIPTION = hifu mainframe
QMAKE_TARGET_COPYRIGHT = zhonghui medical(shanghai)
QMAKE_CXXFLAGS_EXCEPTIONS_ON = /EHa
QMAKE_CXXFLAGS_STL_ON = /EHa

LIBS += -lDbgHelp
QMAKE_CXXFLAGS_RELEASE = $$QMAKE_CXXFLAGS_RELEASE_WITH_DEBUGINFO
QMAKE_CFLAGS_RELEASE = $$QMAKE_CFLAGS_RELEASE_WITH_DEBUGINFO
QMAKE_LFLAGS_RELEASE = $$QMAKE_LFLAGS_RELEASE_WITH_DEBUGINFO

INCLUDEPATH += ../../lib/common/ \
               ../../lib/Client/ \
               ../../lib/Plan/ \
               ../../lib/ServiceBuffer/ \
               ../../lib/Message/ \
               ../../lib/RealtimeScene/ \
               ../../lib/HifuImageAlgo/ \
               ../../lib/DrawScene/ \
               ../../lib/LayerScene/ \
               ../../lib/ImageGrabber/QCap \
               ../../lib/ImageGrabber \
#               ../../lib/ImageCapture \
               ../../lib/Location \
               ../../lib/Water/ \
               ../../lib/Append/ \
               ../../lib/Hifu3d/ \
               ../../lib/GL \
               ../../lib/HIFUNavigation \
               ../../dependency/VTK/include/vtk-8.2 \
               ../../lib/HifuRegist/ \
               ../../lib/NewScene3D/ \
               ../../lib/PlanScene3D/ \
               ../../lib/CrashReport/ \
               ../../lib/HifuHttpServer/ \
               ../../lib/PhaseController/ \
               ../../lib/PowerAmp/ \
#               ../../dependency/opencv/include \
#               ../../dependency/opencv/include/opencv \
#               ../../dependency/opencv/include/opencv2 \



LIBS += ../../bin/Client.lib \
        ../../bin/Plan.lib \
        ../../bin/ServiceBuffer.lib \
        ../../bin/Message.lib \
        ../../bin/RealtimeScene.lib \
        ../../bin/DrawScene.lib \
        ../../bin/LayerScene.lib \
        ../../bin/ImageGrabberTc.lib \
#        ../../bin/ImageCapture.lib \
        ../../bin/Location.lib \
        ../../bin/HIFUWater.lib \
        ../../bin/Q_Water.lib \
        ../../bin/Append.lib \
        ../../bin/Hifu3d.lib \
        ../../bin/PowerAmp.lib\
        ../../bin/PhaseController.lib\
        ../../bin/HifuNavigation.lib \
        ../../bin/HifuRegist.lib \
        ../../bin/NewScene3D.lib \
        ../../bin/PlanScene3D.lib \
#        ../../bin/exchndl.lib \
        ../../bin/HifuHttpServer.lib \
#        ../../bin/libvtkRenderingCore-6.3.lib \
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
        ../../bin/glut32.Lib \
        ../../bin/glut.Lib \
#        -L../../dependency/VTK/bin \
#        -llibvtkRenderingCore-6.3 \
#        -llibvtkRenderingOpenGL-6.3 \
#        -llibvtkCommonCore-6.3 \
#        -llibvtkImagingCore-6.3 \
#        -llibvtkInteractionStyle-6.3 \
#        -llibvtkInteractionImage-6.3 \
#        -llibvtkIOImage-6.3 \
#        -llibvtkCommonExecutionModel-6.3 \
#        -llibvtkCommonDataModel-6.3 \
#        -llibvtkGUISupportQt-6.3 \
#        -llibvtkImagingSources-6.3 \
#        -llibvtkRenderingImage-6.3 \
#        -llibvtkFiltersSources-6.3 \
#        -llibvtkFiltersCore-6.3 \
#        -llibvtkRenderingAnnotation-6.3 \
#        -llibvtkInteractionWidgets-6.3 \
#        -llibvtkIOGeometry-6.3/ \
#LIBS += ../../bin/HifuImageAlgo.dll \
#        ../../bin/Client.dll \
#        ../../bin/Plan.dll \
#        ../../bin/ServiceBuffer.dll \
#        ../../bin/Message.dll \
#        ../../bin/RealtimeScene.dll \
#        ../../bin/DrawScene.dll \
#        ../../bin/LayerScene.dll \
#        ../../bin/ImageGrabber.dll \
#        ../../bin/ImageCapture.dll \
#        ../../bin/Location.dll \
#        ../../bin/HIFUWater.dll \
#        ../../bin/Append.dll \
#        ../../bin/Hifu3d.dll \
#        ../../bin/HifuNavigation.dll \
#        ../../bin/HifuRegist.dll \
#        ../../bin/NewScene3D.dll \
#        ../../bin/PlanScene3D.dll \
#        ../../bin/exchndl.dll \
#        ../../bin/HifuHttpServer.dll \
#        -L../../dependency/VTK/bin \
#        -llibvtkRenderingCore-6.3 \
#        -llibvtkRenderingOpenGL-6.3 \
#        -llibvtkCommonCore-6.3 \
#        -llibvtkImagingCore-6.3 \
#        -llibvtkInteractionStyle-6.3 \
#        -llibvtkInteractionImage-6.3 \
#        -llibvtkIOImage-6.3 \
#        -llibvtkCommonExecutionModel-6.3 \
#        -llibvtkCommonDataModel-6.3 \
#        -llibvtkGUISupportQt-6.3 \
#        -llibvtkImagingSources-6.3 \
#        -llibvtkRenderingImage-6.3 \
#        -llibvtkFiltersSources-6.3 \
#        -llibvtkFiltersCore-6.3 \
#        -llibvtkRenderingAnnotation-6.3 \
#        -llibvtkInteractionWidgets-6.3 \
#        -llibvtkIOGeometry-6.3 \

#LIBS +=  -L../opencv/bin \
#         -llibopencv_core320 \
#         -llibopencv_core320 \
#         -llibopencv_highgui320 \
#         -llibopencv_imgproc320 \
#         -llibopencv_features2d320 \
#         -llibopencv_calib3d320 \
#         -llibopencv_imgcodecs320 \
#         -lopencv_ffmpeg320 \
#         -llibopencv_flann320 \
#         -llibopencv_ml320 \
#         -llibopencv_objdetect320 \
#         -llibopencv_objdetect320 \
#         -llibopencv_shape320 \
#         -llibopencv_stitching320 \
#         -llibopencv_superres320 \

#LIBS +=  ../../dependency/opencv/bin/libopencv_core320.dll \
#         ../../dependency/opencv/bin/libopencv_core320.dll \
#         ../../dependency/opencv/bin/libopencv_highgui320.dll \
#         ../../dependency/opencv/bin/libopencv_imgproc320.dll \
#         ../../dependency/opencv/bin/libopencv_features2d320.dll \
#         ../../dependency/opencv/bin/libopencv_calib3d320.dll \
#         ../../dependency/opencv/bin/libopencv_imgcodecs320.dll \


SOURCES += main.cpp\
    crashreporter.cpp\
    choseimage.cpp \
    draintankform.cpp \
    logindialog.cpp \
    tablemodel.cpp \
    irradiateplan.cpp \
    modelmanager.cpp \
    smartplan.cpp \
    newmainwindow.cpp \
    research.cpp \
    irradiatespot.cpp \
    irradiatecore.cpp \
    adduser.cpp \
    logininfoedit.cpp \
    usermanager.cpp \
    printdialog.cpp \
    parametertable.cpp \
    selftestdlg.cpp \
    qcw_thermometer.cpp \
    dragtable.cpp \
    dragmodel.cpp \
    filesavedialog.cpp \
    patientmanager.cpp \
    histogram.cpp \
    commondefine.cpp \
    headercontrol.cpp \
    irradiateblock.cpp \
    irradiateblockplan.cpp \
    printdialogclinic.cpp \
    dupumpcontrol.cpp \
    heart_thread.cpp \
    quitform.cpp \
    websocketclient.cpp

HEADERS  += \
    crashreporter.h\
    choseimage.h \
    draintankform.h \
    logindialog.h \
    tablemodel.h \
    irradiateplan.h \
    modelmanager.h \
    smartplan.h \
    newmainwindow.h \
    commondefine.h \
    research.h \
    irradiatespot.h \
    irradiatecore.h \
    adduser.h \
    logininfoedit.h \
    usermanager.h \
    usermanager_global.h \
    printdialog.h \
    parametertable.h \
    selftestdlg.h \
    qcw_thermometer.h \
    sampletotemperature.h \
    dragtable.h \
    dragmodel.h \
    filesavedialog.h \
    patientmanager.h \
    histogram.h \
    headercontrol.h \
    irradiateblockplan.h \
    printdialogclinic.h \
    dupumpcontrol.h \
    heart_thread.h \
    quitform.h \
    websocketclient.h

FORMS    += \
    choseimage.ui \
    draintankform.ui \
    logindialog.ui \
    viewer3d.ui \
    newmainwindow.ui \
    research.ui \
    adduser.ui \
    logininfoedit.ui \
    usermanager.ui \
    printdialog.ui \
    selftestdlg.ui \
    filesavedialog.ui \
    headercontrol.ui \
    printdialogclinic.ui \
    quitform.ui \
    dupumpcontrol.ui

RESOURCES += \
    resource.qrc \
    qml.qrc \

DISTFILES +=

#if(!include(../../HifuCrashReport/HifuCrashReport.pri)){
#    message(Can not add Crash Report Model.)
#}
