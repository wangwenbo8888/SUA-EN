#ifndef MAINDIALOG_H
#define MAINDIALOG_H

#include <QDialog>
#include <QFileDialog>
#include <QMenu>
#include "algobase.h"
#include <myvtkcontourwidget.h>

#include <QLoggingCategory>

#include "constant.h"

#include <vtkAutoInit.h>
VTK_MODULE_INIT(vtkRenderingOpenGL2);
VTK_MODULE_INIT(vtkInteractionStyle);
VTK_MODULE_INIT(vtkRenderingFreeType);

#include <vtkSmartPointer.h>
#include <vtkDICOMImageReader.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkImageShiftScale.h>
#include <vtkImageActor.h>
//#include <QVTKWidget.h>
//#include <QVTKOpenGLWidget.h>
#include <vtkBorderWidget.h>
#include <vtkRenderingOpenGL2Module.h>
#include <vtkImageViewer2.h>
#include <vtkLookupTable.h>
#include <vtkImageMapToColors.h>
#include <vtkImageData.h>
#include <vtkImageMapper3D.h>
#include <vtkDoubleArray.h>
#include <vtkPointData.h>
#include <vtkEventQtSlotConnect.h>
#include <vtkCellPicker.h>
#include <vtkCell.h>
#include <vtkStructuredPoints.h>
#include <vtkImageMask.h>
#include <vtkImageImport.h>
#include <vtkImageResample.h>
#include <vtkAlgorithmOutput.h>
#include <vtkSTLReader.h>
#include <vtkPolyDataMapper.h>
#include <vtkInteractorStyleImage.h>
#include <vtkJPEGWriter.h>

#include <vtkExtractVOI.h>

#include <vtkCamera.h>
#include <vtkDataSetMapper.h>
#include <vtkPolyData.h>
#include <vtkActor.h>
#include <vtkProperty.h>
#include <vtkClipClosedSurface.h>
#include <vtkPlane.h>
#include <vtkPlaneCollection.h>
#include <vtkXMLPolyDataReader.h>

#include <vtkVersion.h>
#include <vtkMetaImageWriter.h>
#include <vtkPolyDataToImageStencil.h>
#include <vtkImageStencil.h>
#include <vtkImagePermute.h>
#include <vtkImageFlip.h>

#include <vtkCutter.h>
#include <vtkStripper.h>
#include <vtkLinearExtrusionFilter.h>
#include <vtkPNGWriter.h>
#include <vtkTransformPolyDataFilter.h>
#include <vtkTransform.h>
#include <vtkBMPWriter.h>
#include <vtkBMPReader.h>

#include <vtkImageCanvasSource2D.h>
#include <vtkImageProperty.h>
#include <vtkImageToStructuredPoints.h>

#include <vtkAxesActor.h>
#include <vtkOrientationMarkerWidget.h>
#include <vtkAnnotatedCubeActor.h>
#include <vtkMedicalImageReader2.h>
#include <vtkMedicalImageProperties.h>
#include <vtkStringArray.h>

#include "vtkPoints.h"
#include "vtkCellArray.h"
#include "vtkPolyData.h"
#include "vtkPolyDataMapper2D.h"
#include "vtkActor2D.h"
#include "vtkProperty2D.h"
#include "vtkRendererCollection.h"
#include <vtkTriangleFilter.h>
#include <vtkMassProperties.h>
#include <vtkRegularPolygonSource.h>
#include <vtkPolygon.h>
#include <vtkDemandDrivenPipeline.h>
#include <vtkActor2DCollection.h>

#include "gdcmReader.h"
#include "gdcmMediaStorage.h"
#include "gdcmStringFilter.h"
#include "gdcmAttribute.h"
#include "gdcmValue.h"
#include "gdcmType.h"
#include "gdcmVR.h"
#include "gdcmPersonName.h"
#include "gdcmString.h"

#include <OperationDialog/thresholddialog.h>
#include <OperationDialog/regiongrowingdialog.h>
#include <OperationDialog/surfacerenderingdialog.h>
#include "hifualgoprocessor.h"
#include <QBitArray>
#include <math.h>

#include "vtkImageThreshold.h"
#include "vtkContourFilter.h"
#include "vtkPolyDataConnectivityFilter.h"

#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>

#include <HIFUConnection.h>
#include <HIFUConnection_global.h>

#include <QVTKWidget.h>

class HIFUConnection;

const double pi=3.1415926535;
const QString STYLE_OPERATING = "background-color: rgb(0, 170, 0);color:rgb(0,0,0);border-radius:5px;";

const QString NOT_WORK = "color: rgb(146, 146, 146);border-radius:5px;";
const QString WORK = "color: rgb(4, 222, 230);border-radius:5px;";
// 背景象牙白
const QString SELECTED = "background-color: rgb(250, 255, 240);color:rgb(0,0,0);border-radius:5px;";

const QString HORIZON_TYPE = //滑条
        "QSlider::groove:horizontal{"
        " border: 1px solid #999999;"
        " height: 5px; "

        "position: absolute;"
        "border-radius :3px;"
        " background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #B1B1B1, stop:1 #c4c4c4);"
        " margin: 1px 0;"
        " left: 3px; right: 3px;"
        "}"
        //小滑块
        "QSlider::handle:horizontal {"
        " background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #b4b4b4, stop:1 #8f8f8f);"
       "border: 1px solid #5c5c5c;"
        "width: 12px;"
        " height: 12px; "
        "margin: -2px -1; "/* handle is placed by default on the contents rect of the groove. Expand outside the groove */
        "  border-radius: 4px;"
        "}"
        //滑块滑过改变样式
        " QSlider::add-page:horizontal {"
        "border-radius :2px;"
        " margin: 1px 1;"
        "background: white;"
        //  "position: absolute;"
        //  " left: 2px; right: 2px;"
        "}"
        "QSlider::sub-page:horizontal {"
        "border-radius :2px;"
        " margin: 1px 1;"//
        "  background-color:#86dfc6;"
        // "position: absolute;"
        // " left: 2px; right: 2px;"
        "}";

Q_DECLARE_LOGGING_CATEGORY(HIFUNAVIGATION)

namespace Ui {
class MainDialog;
}

class MainDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MainDialog(QWidget *parent = 0);
    ~MainDialog();
    void ClearAllData();
    void SetVtkContourWidget(MyVtkContourWidget* myContourWidget2);

protected:
    void closeEvent(QCloseEvent *event);

public slots:
    void on_ThresholdValueChanged(int min,int max);
    void SeedRegionGrowingProcess(bool bClear);
    void SeedRegionReGrowingProcess();
    void SurfaceRenderingProcess();

signals:
    // 发送切割的图像
    void sendImageData(QImage,int);

    // 发送切割的肌膜轮廓
    void sendContour(QImage,int);

    // 发送切割的肌瘤轮廓
    void sendContourNew(QImage,int);

    void sendSearchRadius(int);

    void MainDialogClose();

private slots:
    void on_pushButton_ImportMR_clicked();

    void on_verticalScrollBar_DicomChange_XY_valueChanged(int value);

    //void on_pushButton_quit_clicked();

    void on_qtVtkWidgetMouseMove(vtkObject*, unsigned long, void*, void*);
    void on_qtVtkWidgetMouseClicked(vtkObject*, unsigned long, void*, void*);
    void on_qtVtkWidgetMiddleButtonPressEvent(vtkObject*, unsigned long, void*, void*);
    void on_qtVtkWidgetMouseWheelForwardEvent(vtkObject*, unsigned long, void*, void*);
    void on_qtVtkWidgetMouseWheelBackwardEvent(vtkObject*, unsigned long, void*, void*);
    void on_qtVtkWiggetMouseRightButtonClicked(vtkObject * obj, unsigned long, void * client_data, void *, vtkCommand * command);
    void on_qtVtkWiggetRenderEvent(vtkObject*, unsigned long, void*, void*);

    void on_pushButton_GetContours_clicked();

    void on_pushButton_GetSlices_clicked();

    //void on_spinBox_Threshold_Min_valueChanged(int arg1);

    //void on_horizontalSlider_Threshold_Min_valueChanged(int value);

    //void on_horizontalSlider_Threshold_Max_valueChanged(int value);

    //void on_spinBox_Threshold_Max_valueChanged(int arg1);

    void on_pushButton_PickSeedPoint_clicked();

    void on_pushButton_Grow_clicked();

    void on_toolButton_SetRegionXY_clicked();

    void on_horizontalSlider_EraseSize_valueChanged(int value);

    void on_spinBox_Limit_z1_valueChanged(int arg1);

    void on_spinBox_Limit_z2_valueChanged(int arg1);

    void on_toolButton_Erase_clicked();

    void on_pushButton_ReGrow_clicked();

    void on_pushButton_SurfaceRendering_2_clicked();

    void on_toolButton_Fill_clicked();

    void on_horizontalSlider_FillSize_valueChanged(int value);

    void on_toolButton_ClearLimitRegion_clicked();

    void ContextAction_ThresholdMask();
    void ContextAction_RegionGrowMask();

    void ContexAction_ContourMask();

    void ContextMenu_Hide();

    void on_pushButton_ManualDrawOn_clicked();

    void on_tabWidget_tabBarClicked(int index);

    void on_pushButton_ManualDrawClearCur_clicked();

    void on_pushButton_ManualDrawClearAll_clicked();

    void on_toolButton_grow_confirm_clicked();

    void on_horizontalSlider_search_radius_valueChanged(int value);

    void on_pushButton_output_mask_clicked();

    void on_pushButton_input_mask_clicked();

    void setOrientation(int);

    void on_pushButton_generate_contour_clicked();

    void on_pushButton_generate_mask_clicked();

    void on_pushButton_reslice_contour_clicked();

    void sendGetCoutour();

    void on_pushButton_generate_ellips_clicked();

    void on_radioButton_draw_clicked();

    void on_radioButton_write_clicked();

private:
    Ui::MainDialog *ui;

    DicomSourceInfo DicomInfo;
    AlgoProcessInfo ProcessInfo;
    DicomPreProInfo DicomPreInfo;
    short *_curdata;

    vtkSmartPointer<vtkImageFlip> _flip;
    QImage vtkImageDataToQImage(vtkSmartPointer<vtkImageData> data);

    QImage vtkImageDataToResizeQImage(vtkSmartPointer<vtkImageData> data,int width,int height);

    //读取、显示和交互
    bool isReadingDicom;
    vtkSmartPointer<vtkDICOMImageReader> _dicomReader;
    vtkSmartPointer<vtkImageViewer2> _imageViewer;
    vtkSmartPointer<vtkRenderWindowInteractor> _renderWindowInteractor;
    vtkSmartPointer<vtkEventQtSlotConnect> _vtkQtSlotConnect;
    vtkSmartPointer<vtkCellPicker> _picker;
    vtkSmartPointer<vtkImageData> _inImageData;
    vtkSmartPointer<vtkAlgorithmOutput> _inImageOutputPort;
    vtkSmartPointer<vtkImageResample> _ImageResample;
    QVTKWidget* _qvtkWidget;
    bool GetPointInfoByMousePos(int posx,int posy,int posz,int& pointX,int& pointY,int& pointZ,short& value);
    bool GetDicomTagsInfo(QString dirPath);
    bool bInitView;
    int m_iPosX,m_iPosY,m_iPosZ;
    vtkSmartPointer<vtkRenderer> _emptyDicomRender;
    vtkSmartPointer<vtkRenderWindow> _emptyDicomRenderWindow;

    //预处理
    bool DicomSourceDirecJudge();
    void PreProcessDicomData();

    vtkSmartPointer<vtkImageData> _preProImageData;
    vtkSmartPointer<vtkImageData> _preProMaskData;
    vtkSmartPointer<vtkImageData> _preProContourData;

    bool m_bIsPreProcessed;
    bool m_Nc;//用于判断Nc找到的contour是否为空
    vtkSmartPointer<vtkAlgorithmOutput> _preProImageOutputPort;

    short* _pPreProData;
    uchar* m_pPreProMask;
    uchar* m_pPreProContour;

    vtkSmartPointer<vtkImageImport> PreProImageImport;

    vtkSmartPointer<vtkImageImport> PreProMaskImport;

    vtkSmartPointer<vtkImageImport> PreProContourImport;
//    vtkSmartPointer<vtkImageViewer2> _preProImageViewer;
//    QVTKWidget* _qvtkPreProWidget;

    //阈值分割
    vtkSmartPointer<vtkLookupTable> lut;
    vtkSmartPointer<vtkImageActor> thresholdMaskActor;
    vtkSmartPointer<vtkImageMapToColors> thresholdMasMapper;
    vtkSmartPointer<vtkImageImport> thresholdImageImport;

    void UpdateLUT(int min,int max,bool bUpdataMask);
    void InitThresholdMaskMap();
    void RemoveMaskActor(int maskNum);
    void AddMaskActor(int maskNum);
    void UpdateCtlValueRange(int min,int max);

    vtkSmartPointer<vtkMatrix4x4> _resliceAxes;
    vtkSmartPointer<vtkImageMapToColors> _colorMap;
    vtkSmartPointer<vtkImageReslice> _reslice;
    vtkSmartPointer<vtkLookupTable> _colorTable;
    vtkSmartPointer<vtkExtractVOI> _extraVOI;

    // 获取181幅和超声图像比照的核磁图像
    void GetSlices();

    vtkSmartPointer<vtkMatrix4x4> _contourResliceAxes;
    vtkSmartPointer<vtkImageMapToColors> _contourColorMap;
    vtkSmartPointer<vtkImageReslice> _contourReslice;
    vtkSmartPointer<vtkLookupTable> _contourColorTable;
    vtkSmartPointer<vtkExtractVOI> _contourExtraVOI;
    // 获取肌瘤轮廓图
    void GetContours();
    void GetContoursNew();

    // 获取肌瘤区域轮廓线
    void GetMaskPolyData(double spacex,double spacey,int slice,cv::Mat image);

    //区域增长
    vtkSmartPointer<vtkLookupTable> regionGrowinglut;
    vtkSmartPointer<vtkImageMapToColors> regionGrowingMapper;
    vtkSmartPointer<vtkImageActor> regionGrowingMaskActor;
    vtkSmartPointer<vtkImageImport> regionGrowingImageImport;

    uchar* RegionGrowImageDataArray;
    void InitRegionGrowMask();
    void SetSeedPos(int x,int y,int z);

//    uchar* RegionGrowImageArray_NewOri;
//    vtkSmartPointer<vtkImageImport> regionGrowingImageImport_NewOri;


    //肌瘤区域显示
    vtkSmartPointer<vtkLookupTable> contourlut;
    vtkSmartPointer<vtkImageMapToColors> contourMapper;
    vtkSmartPointer<vtkImageActor> contourActor;
    vtkSmartPointer<vtkImageImport> contourImport;
    void InitContourMask();
    bool isContourMaskInited;
    uchar* ContoursMaskArray;

    //区域增长修正
    void InitLimitedRegionMask();
    void UpdateLimitedRegion(int x2,int y2,int z2,bool isclear);
    void EraseRegion(int x,int y,int z,int size,uchar value);
    void SetLimitPoint1(int p1x,int p1y);
    void SetLimitPoint2(int p2x,int p2y,int w,int h);
    void SetLimitRegionZ(int z1,int z2);
    void UpdateGrowRegionPosInfo(short value,int posx,int posy,int posz);
    vtkSmartPointer<vtkPolyDataMapper2D> RTRect_Mapper;
    vtkSmartPointer<vtkActor2D> RTRect_Actor;
    vtkSmartPointer<vtkRegularPolygonSource> RTRect_Source;
    void InitRTRect();
    void UpdateRTRect(int x2,int y2,int z2,bool hiden);
    int rect_x1,rect_y1;
    double XRatio,YRatio;
    int CurDisplayX,CurDisplayY;
    void GetRatioWorldToDispaly(double& xr,double&yr);
    void LimitedRegionOperationSwitch(Operation operation);
    vtkSmartPointer<vtkRegularPolygonSource> LimitedRegionSource;
    vtkSmartPointer<vtkPolyDataMapper2D> LimitedRegionRect_Mapper;
    vtkSmartPointer<vtkActor2D> LimitedRegionRect_Actor;
    int LimitPointCache_x1,LimitPointCache_y1;

    //表面重建
    vtkSmartPointer<vtkRenderer> _emptySTLRender;
    vtkSmartPointer<vtkActor> _STLActor;
    vtkSmartPointer<vtkOrientationMarkerWidget> _STLOrientationWidget;
    vtkSmartPointer<vtkAnnotatedCubeActor> STLCubeAxes;
    vtkSmartPointer<vtkPolyData> _STLPolyData;
    QVTKWidget* _qvtkWidget_STLView;
    void Update3DModel();
    void UpdateTumorParam(double sizex,double sizey,double sizez,double volume);

    // 根据肌膜轮廓提取肌瘤轮廓
    vtkSmartPointer<vtkImageImport> importer;
    vtkSmartPointer<vtkImageThreshold> thresholdFilter;
    vtkSmartPointer<vtkContourFilter> contourFilter;
    // vtkSmartPointer<vtkPolyDataConnectivityFilter> connectFilter1;

    int _seeds[3];
    int m_iSearchRadius;
    int m_iPatientOrientation;

    // mask空洞填充算法
    void fillHole(const DicomSourceInfo& info,QVector<uchar>& mask);

    //算法
    HIFUAlgoProcessor Hap;

    //pacs连接
    HIFUConnection* _pConnection;

    //右键菜单
    QMenu* _qvtkWIdgetContextMenu;
    void InitqvtkWIdgetContextMenu();
    QAction* _ShowThresholdMask;
    QAction* _ShowRegionGrowMask;
    QAction* _ShowContourMask;

    //区域手动勾选
    MyVtkContourWidget* myContourWidget;
    void ManualDrawProcess(bool bClear);

    //清理数据
    void releaseData();

    bool isDicom(QString file);

    // 导出mask数据
    bool outputMask(QString file);

    // 导入mask数据
    bool inputMask(QString file);

    // 导入默认dicom图像
    bool defaultdicom=false;

    // 设置控件tips
    void setToolTip();



};

#endif // MAINDIALOG_H
