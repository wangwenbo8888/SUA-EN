#ifndef QVTKWRAP_H
#define QVTKWRAP_H
#include <QObject>
#include <QVBoxLayout>
#include <QMenu>
#include <QMap>

//vkt header file
#include <vtkAutoInit.h>
VTK_MODULE_INIT(vtkRenderingOpenGL2)
VTK_MODULE_INIT(vtkInteractionStyle)
VTK_MODULE_INIT(vtkRenderingFreeType)

#include <QVTKWidget.h>
#include <vtkRenderWindow.h>
#include <vtkSmartPointer.h>
#include <vtkRenderer.h>
#include <vtkImageViewer2.h>
#include <vtkDICOMImageReader.h>
#include <vtkImageData.h>
#include <vtkAlgorithmOutput.h>
#include <vtkImageResample.h>
#include <vtkCamera.h>
#include <vtkImageImport.h>
#include <vtkImageCanvasSource2D.h>
#include <vtkLookupTable.h>
#include <vtkImageMapToColors.h>
#include <vtkImageResliceMapper.h>
#include <vtkImageSlice.h>
#include <vtkImageActor.h>
#include <vtkImageImport.h>

#include "vtkSmartPointer.h"
#include "vtkPoints.h"
#include "vtkCellArray.h"
#include "vtkPolyData.h"
#include "vtkPolyDataMapper2D.h"
#include "vtkActor2D.h"
#include "vtkPolyDataMapper.h"
#include <vtkQuad.h>
#include <vtkImageMapper3D.h>
#include <vtkPicker.h>
#include <vtkOutlineSource.h>

#include <vtkAxesActor.h>
#include <vtkOrientationMarkerWidget.h>
#include <vtkAnnotatedCubeActor.h>
#include <vtkSTLReader.h>
#include <vtkProperty.h>
#include <vtkCylinderSource.h>
#include <vtkAppendPolyData.h>
#include <vtkEventQtSlotConnect.h>
#include <vtkInteractorStyle.h>
#include <vtkTransformPolyDataFilter.h>
#include <vtkTransform.h>
#include <vtkPlaneSource.h>
#include <vtkQImageToImageSource.h>
#include <vtkMath.h>

//#include <QVTKWidget2.h>
//#include <vtkGenericOpenGLRenderWindow.h>

#ifdef QT_DEBUG
const QString SPOT_FPATH="D:\\PatientSpot.stl";
const QString HUMANMODEL_FPATH="D:\\newHumanmodel.stl";
#else
const QString BLOCK_FPATH="E:\\USgHIFU\\lib\\block.stl";
const QString HUMANMODEL_FPATH="E:\\USgHIFU\\lib\\newHumanmodel.stl";
const QString TRANSDUCER_PATH="E:\\USgHIFU\\lib\\transducer.stl";
#endif

class QVTKWrap: public QObject
{
                           Q_OBJECT
public:
    explicit QVTKWrap(QObject *parent = 0);
    ~QVTKWrap();

signals:

public slots:

private slots:
    void Scene3d_qvtkWidget_ContextMenu(vtkObject * obj, unsigned long, void * client_data, void *, vtkCommand * command);
    void Scene3d_ContextAction_ShowTumor();
    void Scene3d_ContextAction_ShowIrraPoint();
    void Scene3d_ContextAction_ShowIrraPointAllLevel();
    void Scene3d_ContextAction_ShowIrraPointTopLevel();
    void Scene3d_ContextAction_ShowIrraPointMidlleLevel();
    void Scene3d_ContextAction_ShowIrraPointDownLevel();
    void Scene3d_ContextAction_ShowTumorNoOpacity();
    void Scene3d_ContextAction_ShowTumorLowOpacity();
    void Scene3d_ContextAction_ShowTumorMidlleOpacity();
    void Scene3d_ContextAction_ShowTumorHighOpacity();
    void Scene3d_ContextAction_LockHumanModel();
    void Scene3d_ContextMenu_Hide();

private:
    //Scene3d
    QVTKWidget* _qvtkWidget_Scene3d;
    vtkSmartPointer<vtkRenderer> _Scene3d_Renderer;

    // 换能器模型
    vtkSmartPointer<vtkSTLReader> _Scene3d_TransducerStlReader;
    vtkSmartPointer<vtkPolyDataMapper> _Scene3d_TransducerMapper;
    vtkSmartPointer<vtkActor> _Scene3d_TransducerActor;

    vtkSmartPointer<vtkSTLReader> _Scene3d_TumorStlReader;
    vtkSmartPointer<vtkPolyDataMapper> _Scene3d_TumorMapper;
    vtkSmartPointer<vtkActor> _Scene3d_TumorActor;
    vtkSmartPointer<vtkSTLReader> _Scene3d_HumanStlReader;
    vtkSmartPointer<vtkPolyDataMapper> _Scene3d_HumanMapper;
    vtkSmartPointer<vtkActor> _Scene3d_HumanActor;
    vtkSmartPointer<vtkOrientationMarkerWidget> _Scene3d_OrientationWidget;
    vtkSmartPointer<vtkAnnotatedCubeActor> _Scene3d_CubeAxes;
    QVector<vtkSmartPointer<vtkPolyDataMapper> > _Scene3d_IrradiationPointMapper;
    QMap<QString,vtkSmartPointer<vtkActor> >  _Scene3d_IrradiationPointActorMap;
    vtkSmartPointer<vtkSTLReader> _Scene3d_BlockStlReader;
    vtkSmartPointer<vtkTransformPolyDataFilter> _Scene3d_BlockTransformFilter;
    QVector<vtkSmartPointer<vtkPolyData> > _Scene3d_BlockPloyData;

    QString NowIrraPointKey;

    bool bInitIrraPoint; //是否初始化点

    //右键菜单
    vtkSmartPointer<vtkEventQtSlotConnect> _vtkQtSlotConnect;
    QMenu* _qvtkWIdgetContextMenu;
    void InitqvtkWIdgetContextMenu();
    QAction* _ShowTuborElement;
    QAction* _ShowIrraPointElement;
    QAction* _ShowIrraPointAllLevel;
    QAction* _ShowIrraPointTopLevel;
    QAction* _ShowIrraPointMidlleLevel;
    QAction* _ShowIrraPointDownLevel;    
    QAction* _ShowTumorNoOpacity;
    QAction* _ShowTumorLowOpacity;
    QAction* _ShowTumorMidlleOpacity;
    QAction* _ShowTumorHighOpacity;
    QAction* _LockHumanModel;

    //camera状态
    bool isInitCam;
    double CamInitFocalPoint[3];
    double CamInitPosition[3];
    double CamInitViewUp[3];

public:
    int PatientHeadDirection;

    void Wrap_Init_Scene3d(QVBoxLayout* frame,QWidget *parent = 0);
    void Wrap_LoadTumorStl(const char* path,double *centerOffset,float size);
    void Wrap_Init_IrradiationPoint(double *centerOffset);
    void Wrap_Init_IrradiationPoint(double *centerOffset,QMap<QString,QVector<double> > &SpotList,int HeadDirection);
    void SetNowIrraPoint(QString idBound);
    void Wrap_setBlockColor(QString id,QColor color);
    void Wrap_ClearSpot();
    void Wrap_ClearTumor();

    void Wrap_ChangeUsImg(double angle,QImage &img);
    void Wrap_ChangeUsImg(double angle,QPixmap &pix);
    void Wrap_ChangeUsImg(double angle);
};

#endif // QVTKWRAP_H
