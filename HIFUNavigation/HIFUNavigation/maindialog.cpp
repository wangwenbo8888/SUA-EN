#include "maindialog.h"
#include "ui_maindialog.h"
#include <QDebug>
#include <QDesktopWidget>
#include <QMessageBox>
#include <QMouseEvent>
#include <QTextCodec>
#include <QRegExp>

#include <QFile>
#include <QTextStream>

#include "vtkImageLuminance.h"
#include "vtkPointsProjectedHull.h"
#include "vtkPoints.h"
#include "vtkDataArray.h"
#include "vtkPolyLine.h"
#include "vtkCellData.h"
#include "vtkParametricSpline.h"

#include "vtkPolygonalSurfacePointPlacer.h"
#include "vtkPolygonalSurfaceContourLineInterpolator.h"
#include "vtkPolyDataCollection.h"

#include "fitalgorighm.h"
#include <QToolTip>
#include "HIFUConnection.h"


#define PI 3.1415916
using namespace cv;


QTextCodec *codec;

Q_LOGGING_CATEGORY(HIFUNAVIGATION,"HIFUNAVIGATION")

MainDialog::MainDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MainDialog),
    _curdata(NULL),
    _inImageData(NULL),
    _inImageOutputPort(NULL),
    _preProImageData(NULL),
    _pPreProData(NULL),
    _STLPolyData(NULL),
    m_pPreProMask(NULL),
    m_pPreProContour(NULL),
    ContoursMaskArray(NULL),
    RegionGrowImageDataArray(NULL)
    //RegionGrowImageArray_NewOri(NULL)
{
    ui->setupUi(this);

    ui->pushButton_output_mask->setVisible(false);
    ui->pushButton_input_mask->setVisible(false);

    //ui->tabWidget->setTabEnabled(0,false);

    setWindowFlags(windowFlags() | Qt::Widget);
    //    setWindowFlags(windowFlags() | Qt::FramelessWindowHint);
    //    QDesktopWidget* desktop = QApplication::desktop();
    //    move(desktop->width() - this->width(), 0);

    DicomInfo.isSourceRead=false;
    bInitView=false;
    m_iPosX=m_iPosY=m_iPosZ=-1;
    //读取显示初始化
    isReadingDicom=false;
    _dicomReader = vtkSmartPointer<vtkDICOMImageReader>::New();
    _imageViewer = vtkSmartPointer<vtkImageViewer2>::New();
    _renderWindowInteractor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
    _vtkQtSlotConnect=vtkSmartPointer<vtkEventQtSlotConnect>::New();
    _picker=vtkSmartPointer<vtkCellPicker>::New();
    _ImageResample=vtkSmartPointer<vtkImageResample>::New();
    _picker->SetTolerance(0.0005);
    _renderWindowInteractor->SetPicker(_picker);

    _qvtkWidget = new QVTKWidget(this);
    _emptyDicomRender=vtkSmartPointer<vtkRenderer>::New();
    _emptyDicomRenderWindow=vtkSmartPointer<vtkRenderWindow>::New();
    _emptyDicomRenderWindow->AddRenderer(_emptyDicomRender);
    ui->verticalLayout_vtktidget_dicom->addWidget(_qvtkWidget);
    _qvtkWidget->SetRenderWindow(_emptyDicomRenderWindow);
    PreProImageImport=vtkSmartPointer<vtkImageImport>::New();

    PreProMaskImport=vtkSmartPointer<vtkImageImport>::New();
    PreProContourImport = vtkSmartPointer<vtkImageImport>::New();

    _flip = vtkSmartPointer<vtkImageFlip>::New();

    setToolTip();//设置控件tips


    //阈值分割初始化
    lut = vtkSmartPointer<vtkLookupTable>::New();
    thresholdMasMapper = vtkSmartPointer<vtkImageMapToColors>::New();
    thresholdMaskActor = vtkSmartPointer<vtkImageActor>::New();
    thresholdImageImport = vtkSmartPointer<vtkImageImport>::New();
    UpdateLUT(0,0,false);

    //区域增长初始化
    regionGrowingMaskActor=vtkSmartPointer<vtkImageActor>::New();
    regionGrowingImageImport=vtkSmartPointer<vtkImageImport>::New();
    regionGrowinglut = vtkSmartPointer<vtkLookupTable>::New();
    regionGrowingMapper = vtkSmartPointer<vtkImageMapToColors>::New();
    if (RegionGrowImageDataArray!=NULL)
    {
       delete []RegionGrowImageDataArray;
        RegionGrowImageDataArray = NULL;
    }

//    if (RegionGrowImageArray_NewOri!=NULL)
//    {
//       delete []RegionGrowImageArray_NewOri;
//       RegionGrowImageArray_NewOri = NULL;
//    }

    // regionGrowingImageImport_NewOri=vtkSmartPointer<vtkImageImport>::New();

    contourlut= vtkSmartPointer<vtkLookupTable>::New();
    contourMapper= vtkSmartPointer<vtkImageMapToColors>::New();
    contourActor=vtkSmartPointer<vtkImageActor>::New();
    contourImport=vtkSmartPointer<vtkImageImport>::New();
    isContourMaskInited = false;

    //区域增长修正初始化
    LimitedRegionRect_Mapper=vtkSmartPointer<vtkPolyDataMapper2D>::New();
    LimitedRegionRect_Actor=vtkSmartPointer<vtkActor2D>::New();
    RTRect_Source=vtkSmartPointer<vtkRegularPolygonSource>::New();
    LimitedRegionSource=vtkSmartPointer<vtkRegularPolygonSource>::New();
    RTRect_Mapper=vtkSmartPointer<vtkPolyDataMapper2D>::New();
    RTRect_Actor=vtkSmartPointer<vtkActor2D>::New();

    //表面重建
    STLCubeAxes=vtkSmartPointer<vtkAnnotatedCubeActor>::New();
    _STLOrientationWidget=vtkSmartPointer<vtkOrientationMarkerWidget>::New();
    STLCubeAxes->SetXPlusFaceText("L");
    STLCubeAxes->SetXMinusFaceText("R");
    STLCubeAxes->SetYMinusFaceText("A");
    STLCubeAxes->SetYPlusFaceText("P");
    STLCubeAxes->SetZMinusFaceText("I");
    STLCubeAxes->SetZPlusFaceText("S");
    STLCubeAxes->GetTextEdgesProperty()->SetColor(1,1,0);
    STLCubeAxes->GetTextEdgesProperty()->SetLineWidth(2);
    STLCubeAxes->GetCubeProperty()->SetColor(0,0,1);
    _qvtkWidget_STLView=new QVTKWidget(this);
    _STLActor=vtkSmartPointer<vtkActor>::New();
    _emptySTLRender=vtkSmartPointer<vtkRenderer>::New();
    ui->verticalLayout_vtktidget_3d->addWidget(_qvtkWidget_STLView);
    _qvtkWidget_STLView->GetRenderWindow()->AddRenderer(_emptySTLRender);

    // 切割图像
    _colorMap = vtkSmartPointer<vtkImageMapToColors>::New();
    _resliceAxes = vtkSmartPointer<vtkMatrix4x4>::New();

    _reslice = vtkSmartPointer<vtkImageReslice>::New();
    _colorTable = vtkSmartPointer<vtkLookupTable>::New();
    _extraVOI = vtkSmartPointer<vtkExtractVOI>::New();

    _contourResliceAxes = vtkSmartPointer<vtkMatrix4x4>::New();
    _contourColorMap = vtkSmartPointer<vtkImageMapToColors>::New();
    _contourReslice = vtkSmartPointer<vtkImageReslice>::New();
    _contourColorTable = vtkSmartPointer<vtkLookupTable>::New();
    _contourExtraVOI = vtkSmartPointer<vtkExtractVOI>::New();

    // 根据肌膜轮廓提取肌瘤轮廓初始化
    importer = vtkSmartPointer<vtkImageImport>::New();
    thresholdFilter=vtkSmartPointer<vtkImageThreshold>::New();
    contourFilter = vtkSmartPointer<vtkContourFilter>::New();
    // connectFilter1=vtkSmartPointer<vtkPolyDataConnectivityFilter>::New();

    //右键菜单
    _qvtkWIdgetContextMenu=NULL;
    _ShowThresholdMask=NULL;
    _ShowRegionGrowMask=NULL;
    _ShowContourMask = NULL;
    InitqvtkWIdgetContextMenu();

    //pacs
    _pConnection = NULL;

//    m_iSearchRadius = ui->horizontalSlider_search_radius->value();
//    ui->label__search_radius->setText(QString::number(m_iSearchRadius));

//    ui->horizontalSlider_EraseSize->setStyleSheet(HORIZON_TYPE);
//    ui->horizontalSlider_FillSize->setStyleSheet(HORIZON_TYPE);
//    ui->horizontalSlider_search_radius->setStyleSheet(HORIZON_TYPE);

//    ui->toolButton_ClearLimitRegion->setStyleSheet(SELECTED);

//    ui->toolButton_Erase->setStyleSheet(SELECTED);
//    ui->toolButton_Fill->setStyleSheet(SELECTED);

//    ui->toolButton_grow_confirm->setStyleSheet(SELECTED);
//    ui->toolButton_SetRegionXY->setStyleSheet(SELECTED);

    ui->pushButton_GetContours->setEnabled(false);
    ui->pushButton_GetContours->setStyleSheet(NOT_WORK);

//    ui->pushButton_GetSlices->setEnabled(false);
//    ui->pushButton_GetSlices->setStyleSheet(NOT_WORK);

    ui->pushButton_output_mask->setEnabled(false);
    ui->pushButton_output_mask->setStyleSheet(NOT_WORK);

    ui->pushButton_input_mask->setEnabled(false);
    ui->pushButton_input_mask->setStyleSheet(NOT_WORK);

    ui->pushButton_SurfaceRendering_2->setEnabled(false);
    ui->pushButton_SurfaceRendering_2->setStyleSheet(NOT_WORK);

    ui->pushButton_ImportMR->setStyleSheet(SELECTED);
    ui->pushButton_ManualDrawClearAll->setStyleSheet(SELECTED);
    ui->pushButton_ManualDrawClearCur->setStyleSheet(SELECTED);
    ui->pushButton_ManualDrawOn->setEnabled(false);
    ui->pushButton_ManualDrawOn->setStyleSheet(NOT_WORK);
    ui->pushButton_generate_ellips->setStyleSheet(SELECTED);
//    ui->pushButton_PickSeedPoint->setStyleSheet(SELECTED);
//    ui->pushButton_ReGrow->setStyleSheet(SELECTED);

//    ui->horizontalSlider_Threshold_Min->setEnabled(false);
//    ui->horizontalSlider_Threshold_Min->setStyleSheet(NOT_WORK);

//    ui->horizontalSlider_Threshold_Max->setEnabled(false);
//    ui->horizontalSlider_Threshold_Max->setStyleSheet(NOT_WORK);

//    ui->spinBox_Threshold_Max->setEnabled(false);
//    ui->spinBox_Threshold_Min->setEnabled(false);

//    ui->label_EraseSize->setText(QString::number(ui->horizontalSlider_EraseSize->value()));
//    ui->label_FillSize->setText(QString::number(ui->horizontalSlider_FillSize->value()));

    ui->pushButton_ManualDrawClearAll->setEnabled(false);
    ui->pushButton_ManualDrawClearAll->setStyleSheet(NOT_WORK);

    ui->pushButton_ManualDrawClearCur->setEnabled(false);
    ui->pushButton_ManualDrawClearCur->setStyleSheet(NOT_WORK);

    //Update3DModel();
    if(_pConnection==NULL)
    {
        _pConnection = new HIFUConnection;
        //ui->verticalLayout_connection->addWidget(_pConnection->GetWindowWidget());
    }
}

MainDialog::~MainDialog()
{
    if (ContoursMaskArray!=NULL)
    {
        delete []ContoursMaskArray;
        ContoursMaskArray = NULL;
    }

    delete ui;
}


void MainDialog::setToolTip()
{
    ui->spinBox_long_axis->setStyleSheet("QToolTip{border:0px solid black; background: white; color:black;}");
    ui->spinBox_short_axis->setStyleSheet("QToolTip{border:0px solid black; background: white; color:black;}");
    ui->spinBox_thick_axis->setStyleSheet("QToolTip{border:0px solid black; background: white; color:black;}");
    ui->spinBox_long_axis->setToolTip(QStringLiteral("椭圆长轴半径，最大105mm"));
    ui->spinBox_short_axis->setToolTip(QStringLiteral("椭圆短轴半径，最大105mm"));
    ui->spinBox_thick_axis->setToolTip(QStringLiteral("椭圆厚度半径，最大105mm"));
}

void MainDialog::on_pushButton_ImportMR_clicked()
{
    isReadingDicom=true;
    QFileDialog pathDlg;
    pathDlg.setViewMode(QFileDialog::Detail);
    QDir dirtemp;
    QString rootpath=dirtemp.currentPath();
    rootpath=rootpath.left(rootpath.indexOf(":")+1);
    rootpath.append("/dicom/");
    pathDlg.setDirectory(rootpath);
    QString path=pathDlg.getExistingDirectory(this);
    QDir dir(path);
    if(path.isEmpty()||!dir.exists())
    {
        QMessageBox::warning(0,"tips","image import error !","OK",0);
        return;
    }

    qDebug()<<path;

    dir.setFilter(QDir::Files | QDir::NoSymLinks);
    QFileInfoList dicomlist = dir.entryInfoList();


    if(dicomlist.count()>=100)
    {
        codec = QTextCodec::codecForName("GBK");
        QString info1Str =  codec->toUnicode("提示");
        QString info2Str =  codec->toUnicode("读取的核磁图像超过100张，请重新读取");

        QMessageBox::warning(NULL,info1Str,info2Str);
        return;
    }

    if(dicomlist.count()<=3)
    {
        codec = QTextCodec::codecForName("GBK");
        QString info1Str =  codec->toUnicode("提示");
        QString info2Str =  codec->toUnicode("读取的核磁图像小于3张，请重新读取");
        QMessageBox::warning(NULL,info1Str,info2Str);
        return;
    }

    QStringList string_list;
    string_list.clear();
    QRegExp regx("[a-zA-Z0-9\/\\\.\_\\s\:\-\+]+$");
    for(int i=0; i<dicomlist.count();i++)
    {
        QFileInfo file_info = dicomlist.at(i);
        QString absolute_file_path = file_info.absoluteFilePath();
//        QTextCodec *code = QTextCodec::codecForName("GB2312");
//        std::string abs_filepath = code->fromUnicode(absolute_file_path).data();

//    absolute_file_path.
//    QString::iterator iter = absolute_file_path.begin();
//    for(i = 0;i<absolute_file_path.length();i++)
//    {
//        QString tempstring = absolute_file_path(i);
//        if(!(tempstring.contains(regx)))
//            return;

//    }
        QString temp_string = absolute_file_path;
        temp_string.remove(regx);
//        temp_string.remove(" ");
        if(temp_string.length())
        {
            codec = QTextCodec::codecForName("GBK");
            QString info1Str =  codec->toUnicode("提示");
            QString info2Str =  codec->toUnicode("路径包含中文及特殊字符,请使用英文和数字！");
            QMessageBox::warning(0,info1Str,info2Str,"ok",0);
            qDebug()<<"路径包含："<<absolute_file_path;
            return;
        }
        if(_dicomReader->CanReadFile(absolute_file_path.toLocal8Bit().data()))
            string_list.append(absolute_file_path);
        else
            return;



    }
    if(string_list.isEmpty())
    {
        codec = QTextCodec::codecForName("GBK");
        QString info1Str =  codec->toUnicode("提示");
        QString info2Str =  codec->toUnicode("没有可读取的核磁图像");
        QMessageBox::warning(NULL,info1Str,info2Str);
        return;
    }
    defaultdicom=false;
    ui->pushButton_ManualDrawOn->setEnabled(true);
    ui->pushButton_ManualDrawOn->setStyleSheet(SELECTED);

    if(DicomInfo.isSourceRead)
    {
        //清理原有读取的数据
        LimitedRegionOperationSwitch(NoOperation);
        UpdateTumorParam(0.0,0.0,0.0,0.00);
        _imageViewer->GetRenderer()->RemoveActor(thresholdMaskActor);
        _imageViewer->GetRenderer()->RemoveActor(regionGrowingMaskActor);
        _imageViewer->GetRenderer()->RemoveActor(contourActor);
        _imageViewer->GetRenderer()->RemoveActor2D(LimitedRegionRect_Actor);
        ProcessInfo.ClearParam();
        myContourWidget->ClearContour();

        releaseData();
    }

    _dicomReader->ReleaseDataFlagOn();
    _dicomReader->SetDataScalarTypeToShort();
    _dicomReader->SetDirectoryName(path.toLatin1().data());
    _dicomReader->Update();

    //清理信息和参数
    DicomInfo.ClearParam();
    ProcessInfo.ClearParam();
    SetSeedPos(-1,-1,-1);
//    SetLimitRegionZ(0,0);
//    SetLimitPoint1(-1,-1);
//    SetLimitPoint2(-1,-1,0,0);

    // 重采样
    int* originalDims = _dicomReader->GetOutput()->GetDimensions();
    double* originalSpace=_dicomReader->GetOutput()->GetSpacing();
    qDebug()<<"原图像信息\n"<<"Dims:"<<originalDims[0]<<originalDims[1]<<originalDims[2];
    qDebug()<<"Spacing:"<<originalSpace[0]<<originalSpace[1]<<originalSpace[2];
    if(originalSpace[0]<=0||originalSpace[1]<=0||originalSpace[2]<=0)
    {
        codec = QTextCodec::codecForName("GBK");
        QString info1Str =  codec->toUnicode("提示");
        QString info2Str =  codec->toUnicode("选择的核磁图像数据无效！");
        QMessageBox::warning(NULL,info1Str,info2Str);
        return;
    }


    if(GetDicomTagsInfo(path))
    {
        qDebug()<<"Get Dicom Tags Success";
        DicomSourceDirecJudge();
    }
    else
        qDebug()<<"Get Dicom Tags failed";
    qDebug()<<"Source Orientation:"<<DicomInfo.OrientationRow<<DicomInfo.OrientationCol<<DicomInfo.OrientationNum;
    //重采样
    double dNewZSpacing=originalSpace[2];
    if(originalSpace[2]>2)
    {
        dNewZSpacing=2;
    }
    if(_ImageResample->GetNumberOfInputConnections(0)==0)
    {
        _ImageResample->SetInputConnection(0,_dicomReader->GetOutputPort());
        _ImageResample->SetInterpolationModeToLinear();
    }

    _ImageResample->SetAxisOutputSpacing(0, originalSpace[0]);
    _ImageResample->SetAxisOutputSpacing(1, originalSpace[1]);
    _ImageResample->SetAxisOutputSpacing(2, dNewZSpacing);
    _ImageResample->Update();

    _inImageData=_ImageResample->GetOutput();
    _inImageOutputPort=_ImageResample->GetOutputPort();
    //显示
    if(DicomInfo.DicomTagInfoList.count()>0&&DicomInfo.DicomTagInfoList[0].dWindowCenter!=-1&&DicomInfo.DicomTagInfoList[0].dWindowWidth!=-1)
    {
        _imageViewer->SetColorLevel(DicomInfo.DicomTagInfoList[0].dWindowCenter);
        _imageViewer->SetColorWindow(DicomInfo.DicomTagInfoList[0].dWindowWidth);
    }
    else
    {
        _imageViewer->SetColorLevel(500);
        _imageViewer->SetColorWindow(2000);
    }
    if(!bInitView)
    {
        // _imageViewer->SetInputConnection(_ImageResample->GetOutputPort());
        _imageViewer->SetInputData(_inImageData);
        _qvtkWidget->SetRenderWindow(_imageViewer->GetRenderWindow());
        _imageViewer->SetupInteractor(_renderWindowInteractor);
        // _imageViewer->Render();

        _vtkQtSlotConnect->Connect(_qvtkWidget->GetRenderWindow()->GetInteractor(),vtkCommand::MouseMoveEvent,
                                   this,SLOT(on_qtVtkWidgetMouseMove(vtkObject*,ulong,void*,void*)));
        _vtkQtSlotConnect->Connect(_qvtkWidget->GetRenderWindow()->GetInteractor(),vtkCommand::LeftButtonPressEvent,
                                   this,SLOT(on_qtVtkWidgetMouseClicked(vtkObject*,ulong,void*,void*)));
        _vtkQtSlotConnect->Connect(_qvtkWidget->GetRenderWindow()->GetInteractor(),vtkCommand::MiddleButtonPressEvent,
                                   this,SLOT(on_qtVtkWidgetMiddleButtonPressEvent(vtkObject*,ulong,void*,void*)));
        _vtkQtSlotConnect->Connect(_qvtkWidget->GetRenderWindow()->GetInteractor(),vtkCommand::MouseWheelBackwardEvent,
                                   this,SLOT(on_qtVtkWidgetMouseWheelBackwardEvent(vtkObject*,ulong,void*,void*)));
        _vtkQtSlotConnect->Connect(_qvtkWidget->GetRenderWindow()->GetInteractor(),vtkCommand::MouseWheelForwardEvent,
                                   this,SLOT(on_qtVtkWidgetMouseWheelForwardEvent(vtkObject*,ulong,void*,void*)));
        _vtkQtSlotConnect->Connect(_qvtkWidget->GetRenderWindow()->GetInteractor(),vtkCommand::RightButtonPressEvent,
                                   this,SLOT(on_qtVtkWiggetMouseRightButtonClicked(vtkObject*,ulong,void*,void*,vtkCommand *)));
        _vtkQtSlotConnect->Connect(_qvtkWidget->GetRenderWindow()->GetInteractor(),vtkCommand::RenderEvent,
                                   this,SLOT(on_qtVtkWiggetRenderEvent(vtkObject*,ulong,void*,void*)));

        myContourWidget->GetImageActorPointPlacer()->SetImageActor(_imageViewer->GetImageActor());
        // myContourWidget->GetImageActorPointPlacer()->SetImageActor(contourActor);
        myContourWidget->SetInteractor(_renderWindowInteractor);
        myContourWidget->SetFollowCursor( true );
        myContourWidget->On();
        myContourWidget->EnabledOn();
        myContourWidget->ProcessEventsOff();

        InitRTRect();
        bInitView=true;
    }

    _imageViewer->GetRenderer()->ResetCamera();
    _imageViewer->GetRenderer()->GetActiveCamera()->Zoom(1.5);
    _imageViewer->Render();

    // 更新数据源信息
    double valuesRange[2];
    _inImageData->GetScalarRange(valuesRange);
    int* dims = _inImageData->GetDimensions();
    double* space =_inImageData->GetSpacing();
    qDebug()<<"预处理后图像信息\n"<<"Dims:"<<dims[0]<<dims[1]<<dims[2];
    qDebug()<<"Spacing:"<<space[0]<<space[1]<<space[2];
    DicomInfo.scalar_min=valuesRange[0];
    DicomInfo.scalar_max=valuesRange[1];
    DicomInfo.width=dims[0];
    DicomInfo.height=dims[1];
    DicomInfo.depth=dims[2];
    DicomInfo.m_iVoxelSize = dims[0]*dims[1]*dims[2];

    qDebug()<<DicomInfo.width<<DicomInfo.height<<DicomInfo.depth;

    DicomInfo.spacing_x=space[0];
    DicomInfo.spacing_y=space[1];
    DicomInfo.spacing_z=space[2];

    _curdata = static_cast<short *>(_inImageData->GetScalarPointer());

    //更新阈值分割
    UpdateLUT(0,0,false);
    // InitThresholdMaskMap();
    // UpdateCtlValueRange(DicomInfo.scalar_min,DicomInfo.scalar_max);

    // InitLimitedRegionMask();

    ui->verticalScrollBar_DicomChange_XY->setEnabled(true);
    ui->verticalScrollBar_DicomChange_XY->setMaximum(_imageViewer->GetSliceMax());
    ui->verticalScrollBar_DicomChange_XY->setMinimum(_imageViewer->GetSliceMin());
    ui->verticalScrollBar_DicomChange_XY->setValue((ui->verticalScrollBar_DicomChange_XY->maximum()+ui->verticalScrollBar_DicomChange_XY->minimum())/2);

    DicomInfo.isSourceRead=true;
    ProcessInfo.isCanThreshold=true;

    PreProcessDicomData();
    isReadingDicom=false;

//    ui->horizontalSlider_Threshold_Min->setEnabled(true);
//    ui->horizontalSlider_Threshold_Min->setStyleSheet(HORIZON_TYPE);

//    ui->horizontalSlider_Threshold_Max->setEnabled(true);
//    ui->horizontalSlider_Threshold_Max->setStyleSheet(HORIZON_TYPE);

//    ui->spinBox_Threshold_Max->setEnabled(true);
//    ui->spinBox_Threshold_Min->setEnabled(true);

    if (!ui->pushButton_input_mask->isEnabled())
    {
        ui->pushButton_input_mask->setEnabled(true);
        ui->pushButton_input_mask->setStyleSheet(SELECTED);
    }

    ui->pushButton_GetContours->setEnabled(false);
    ui->pushButton_GetContours->setStyleSheet(NOT_WORK);

}

bool MainDialog::isDicom(QString file)
{

    QFile file1(file);
    file1.open(QIODevice::ReadOnly);
    int num = 132;
    char * data = new char[num];
    int length = 0;
    if ((length=file1.read(data,num))!=0) // 一次读进10个字节（读进10个字节，length==10），当剩余字节数小于10时，length等于剩余字节数
    {
        int b0 = data[0] & 255;
        int b1 = data[1] & 255;
        int b2 = data[2] & 255;
        int b3 = data[3] & 255;

        if (data[128] == 68 && data[129] == 73 && data[130] == 67 && data[131] == 77)
        {
            file1.close();
            delete []data;
            return true;
        }
        else if ((b0 == 8 || b0 == 2) && b1 == 0 && b3 == 0)
        {
            file1.close();
            delete []data;
            return true;
        }
    }

    file1.close();

    delete []data;
    return false;
}

bool MainDialog::GetDicomTagsInfo(QString dirPath)
{
    qCDebug(HIFUNAVIGATION())<<HIFUNAVIGATION().categoryName()
                            <<"path: "<<dirPath
                           <<TRACE_CMH();
    QDir dir(dirPath);
    if(!dir.exists())
    {
        qCDebug(HIFUNAVIGATION())<<HIFUNAVIGATION().categoryName()
                                <<"dir not exist !"<<dirPath
                               <<TRACE_CMH();
        return false;
    }

    dir.setFilter(QDir::Files | QDir::NoSymLinks);
    QFileInfoList dicomlist = dir.entryInfoList();

    int file_count = dicomlist.count();
    if(file_count <= 1)
    {
        qCDebug(HIFUNAVIGATION())<<HIFUNAVIGATION().categoryName()
                                <<"file count less than 1 "
                               <<TRACE_CMH();

        return false;
    }

    QStringList string_list;
    for(int i=0; i<dicomlist.count();i++)
    {
        QFileInfo file_info = dicomlist.at(i);
        if(file_info.isDir())
            continue;

        QString absolute_file_path = file_info.absoluteFilePath();
        if (isDicom(absolute_file_path))
        {
            string_list.append(absolute_file_path);
        }
    }
    if(string_list.count()<=0)
        return false;

    DicomInfo.DicomTagInfoList.clear();
    int size=string_list.count();
    for(int i=0;i<size;++i)
    {
        // GDCM获取tag
        QString qsFileName=string_list[i];
        qDebug()<<"file name is "<<string_list[i];


        gdcm::Reader reader;
        reader.SetFileName(qsFileName.toStdString().c_str());

        if( !reader.Read() )
        {
            qDebug()<<"Could not read: " << qsFileName ;
            continue;
        }
        gdcm::File &file = reader.GetFile();

        gdcm::DataSet &ds = file.GetDataSet();
        //        std::stringstream strm;
        //        gdcm::FileMetaInformation &fmi = file.GetHeader();
        if(i==0)
        {
            if(ds.FindDataElement(gdcm::Tag(0x0010,0x0010)))
            {
                gdcm::Attribute<0x0010,0x0010> atPatientName;
                atPatientName.Set(ds);
                gdcm::PNComp gdcmPatientName=atPatientName.GetValue();
                DicomInfo.PatientName=QString(gdcmPatientName.c_str());
            }

            qCDebug(HIFUNAVIGATION())<<HIFUNAVIGATION().categoryName()
                                    <<"gdcm step1"
                                   <<TRACE_CMH();

            if(ds.FindDataElement(gdcm::Tag(0x0010,0x0020)))
            {
                gdcm::Attribute<0x0010,0x0020> atPatientID;
                atPatientID.Set(ds);
                gdcm::LOComp gdcmPatientID= atPatientID.GetValue();
                DicomInfo.PatientID=QString(gdcmPatientID.c_str());
            }

            qCDebug(HIFUNAVIGATION())<<HIFUNAVIGATION().categoryName()
                                    <<"gdcm step2"
                                   <<TRACE_CMH();

            if(ds.FindDataElement(gdcm::Tag(0x0020,0x000E)))
            {
                gdcm::Attribute<0x0020,0x000E> atSeriesUID;
                atSeriesUID.Set(ds);
                gdcm::LOComp gdcmSeriesUID= atSeriesUID.GetValue();
                DicomInfo.SeriesUID=QString(gdcmSeriesUID.c_str());
                qDebug()<<"series UID is "<<DicomInfo.SeriesUID;
            }

            qCDebug(HIFUNAVIGATION())<<HIFUNAVIGATION().categoryName()
                                    <<"gdcm step3"
                                   <<TRACE_CMH();
            if(ds.FindDataElement(gdcm::Tag(0x0010,0x0030)))
            {
                gdcm::Attribute<0x0010,0x0030> atPatientBirthDate;
                atPatientBirthDate.Set(ds);
                gdcm::DAComp gdcmPatientBirthDate=atPatientBirthDate.GetValue();
                DicomInfo.PatientBirthDate=QString(gdcmPatientBirthDate.c_str());
            }

            qCDebug(HIFUNAVIGATION())<<HIFUNAVIGATION().categoryName()
                                    <<"gdcm step4"
                                   <<TRACE_CMH();

            if(ds.FindDataElement(gdcm::Tag(0x0010,0x0040)))
            {
                gdcm::Attribute<0x0010,0x0040> atPatientSex;
                atPatientSex.Set(ds);
                gdcm::CSComp gdcmPatientSex=atPatientSex.GetValue();
                DicomInfo.PatientSex=QString(gdcmPatientSex.c_str());
            }

            qCDebug(HIFUNAVIGATION())<<HIFUNAVIGATION().categoryName()
                                    <<"gdcm step5"
                                   <<TRACE_CMH();
            if(ds.FindDataElement(gdcm::Tag(0x0010,0x1010)))
            {
                gdcm::Attribute<0x0010,0x1010> atPatientAge;
                atPatientAge.Set(ds);
                gdcm::ASComp gdcmPatientAge=atPatientAge.GetValue();
                DicomInfo.PatientAge=QString(gdcmPatientAge.c_str());
            }

            qCDebug(HIFUNAVIGATION())<<HIFUNAVIGATION().categoryName()
                                    <<"gdcm step6"
                                   <<TRACE_CMH();
            if(ds.FindDataElement(gdcm::Tag(0x0010,0x1030)))
            {
                gdcm::Attribute<0x0010,0x1030> atPatientWeight;
                atPatientWeight.Set(ds);
                DicomInfo.PatientWeight=QString("%1").arg(atPatientWeight.GetValue());
            }
            qCDebug(HIFUNAVIGATION())<<HIFUNAVIGATION().categoryName()
                                    <<"gdcm step7"
                                   <<TRACE_CMH();


            //            qDebug()<<DicomInfo.PatientName<<DicomInfo.PatientID<<DicomInfo.PatientBirthDate
            //                   <<DicomInfo.PatientSex<<DicomInfo.PatientAge<<DicomInfo.PatientWeight;
        }
        DicomImageTagInfo ntag;
        int iNum;
        if(ds.FindDataElement(gdcm::Tag(0x0020,0x0013)))
        {
            gdcm::Attribute<0x0020,0x0013> atInstanceNum;
            atInstanceNum.Set(ds);
            iNum=(int)atInstanceNum.GetValue();
            //        qDebug()<<"iNum"<<iNum;
            ntag.iInstanceNumber=iNum;
            ntag.sFilePath=qsFileName;
        }
        else
        {
            return false;
        }

        if(ds.FindDataElement(gdcm::Tag(0x0028,0x1050))&&ds.FindDataElement(gdcm::Tag(0x0028,0x1051)))
        {
            gdcm::Attribute<0x0028,0x1050> atWindowCenter;
            atWindowCenter.Set(ds);
            ntag.dWindowCenter=(double)atWindowCenter.GetValue();
            gdcm::Attribute<0x0028,0x1051> atWindowWidth;
            atWindowWidth.Set(ds);
            ntag.dWindowWidth=(double)atWindowWidth.GetValue();
        }


        if(ds.FindDataElement(gdcm::Tag(0x0020,0x0032)))
        {
            gdcm::Attribute<0x0020,0x0032> atImagePosition;
            atImagePosition.Set( ds );
            ntag.dImagePositionX=atImagePosition.GetValue(0);
            ntag.dImagePositionY=atImagePosition.GetValue(1);
            ntag.dImagePositionZ=atImagePosition.GetValue(2);
            //            qDebug()<<iNum<<atImagePosition.GetValue(0)
            //                   <<atImagePosition.GetValue(1)
            //                  <<atImagePosition.GetValue(2);
        }

        if(ds.FindDataElement(gdcm::Tag(0x0020,0x0037)))
        {
            gdcm::Attribute<0x0020,0x0037> atImageOrientation;
            atImageOrientation.Set( ds );
            ntag.dImageOrientationRowX=atImageOrientation.GetValue(0);
            ntag.dImageOrientationRowY=atImageOrientation.GetValue(1);
            ntag.dImageOrientationRowZ=atImageOrientation.GetValue(2);
            ntag.dImageOrientationColX=atImageOrientation.GetValue(3);
            ntag.dImageOrientationColY=atImageOrientation.GetValue(4);
            ntag.dImageOrientationColZ=atImageOrientation.GetValue(5);
            //        QString val20=QString("%1").arg(atImageOrientation.GetValue(0));
            //        QString val21=QString("%1").arg(atImageOrientation.GetValue(1));
            //        QString val22=QString("%1").arg(atImageOrientation.GetValue(2));
            //        QString val23=QString("%1").arg(atImageOrientation.GetValue(3));
            //        QString val24=QString("%1").arg(atImageOrientation.GetValue(4));
            //        QString val25=QString("%1").arg(atImageOrientation.GetValue(5));
            //        qDebug()<<val20<<val21<<val22<<val23<<val24<<val25;
        }
        else
        {
            return false;
        }
        DicomInfo.DicomTagInfoList.append(ntag);
    }
    if(DicomInfo.DicomTagInfoList.count()<2)
    {
        return false;
    }

    for(int j=0;j<DicomInfo.DicomTagInfoList.count()-1;j++)
    {
        for(int k=j+1;k<DicomInfo.DicomTagInfoList.count();k++)
        {
            if(DicomInfo.DicomTagInfoList[k].iInstanceNumber<DicomInfo.DicomTagInfoList[j].iInstanceNumber)
            {
                DicomImageTagInfo temp=DicomInfo.DicomTagInfoList[k];
                DicomInfo.DicomTagInfoList[k]=DicomInfo.DicomTagInfoList[j];
                DicomInfo.DicomTagInfoList[j]=temp;
            }
        }
    }
    return true;
}

bool MainDialog::DicomSourceDirecJudge()
{
    if(DicomInfo.DicomTagInfoList.count()<2)
    {
        DicomPreInfo.bNeedRotate=false;
        return false;
    }
    if(DicomInfo.DicomTagInfoList[0].iInstanceNumber!=-1&&DicomInfo.DicomTagInfoList[0].dImageOrientationRowX==-999)
    {
        DicomPreInfo.bNeedRotate=false;
        return false;
    }
    DicomPreInfo.bNeedRotate=true;
    bool haveXYZ[3];
    haveXYZ[0]=false;
    haveXYZ[1]=false;
    haveXYZ[2]=false;

    if(abs(DicomInfo.DicomTagInfoList[0].dImageOrientationRowX)>0.9)
    {
        if(DicomInfo.DicomTagInfoList[0].dImageOrientationRowX>0)
        {
            DicomInfo.OrientationRow=XPositive;
        }
        else
        {
            DicomInfo.OrientationRow=XNegative;
        }
        haveXYZ[0]=true;
    }
    else if(abs(DicomInfo.DicomTagInfoList[0].dImageOrientationRowY)>0.9)
    {
        if(DicomInfo.DicomTagInfoList[0].dImageOrientationRowY>0)
        {
            DicomInfo.OrientationRow=YPositive;
        }
        else
        {
            DicomInfo.OrientationRow=YNegative;
        }
        haveXYZ[1]=true;
    }
    else if(abs(DicomInfo.DicomTagInfoList[0].dImageOrientationRowZ)>0.9)
    {
        if(DicomInfo.DicomTagInfoList[0].dImageOrientationRowZ>0)
        {
            DicomInfo.OrientationRow=ZPositive;
        }
        else
        {
            DicomInfo.OrientationRow=ZNegative;
        }
        haveXYZ[2]=true;
    }

    if(abs(DicomInfo.DicomTagInfoList[0].dImageOrientationColX)>0.9)
    {
        if(DicomInfo.DicomTagInfoList[0].dImageOrientationColX>0)
        {
            DicomInfo.OrientationCol=XPositive;
        }
        else
        {
            DicomInfo.OrientationCol=XNegative;
        }
        haveXYZ[0]=true;
    }
    else if(abs(DicomInfo.DicomTagInfoList[0].dImageOrientationColY)>0.9)
    {
        if(DicomInfo.DicomTagInfoList[0].dImageOrientationColY>0)
        {
            DicomInfo.OrientationCol=YPositive;
        }
        else
        {
            DicomInfo.OrientationCol=YNegative;
        }
        haveXYZ[1]=true;
    }
    else if(abs(DicomInfo.DicomTagInfoList[0].dImageOrientationColZ)>0.9)
    {
        if(DicomInfo.DicomTagInfoList[0].dImageOrientationColZ>0)
        {
            DicomInfo.OrientationCol=ZPositive;
        }
        else
        {
            DicomInfo.OrientationCol=ZNegative;
        }
        haveXYZ[2]=true;
    }

    if(!haveXYZ[0])
    {
        if((DicomInfo.DicomTagInfoList[1].dImagePositionX-DicomInfo.DicomTagInfoList[0].dImagePositionX)>0)
        {
            DicomInfo.OrientationNum=XPositive;
        }
        else
        {
            DicomInfo.OrientationNum=XNegative;
        }
    }
    else if(!haveXYZ[1])
    {
        if((DicomInfo.DicomTagInfoList[1].dImagePositionY-DicomInfo.DicomTagInfoList[0].dImagePositionY)>0)
        {
            DicomInfo.OrientationNum=YPositive;
        }
        else
        {
            DicomInfo.OrientationNum=YNegative;
        }
    }
    else if(!haveXYZ[2])
    {
        if((DicomInfo.DicomTagInfoList[1].dImagePositionZ-DicomInfo.DicomTagInfoList[0].dImagePositionZ)>0)
        {
            DicomInfo.OrientationNum=ZPositive;
        }
        else
        {
            DicomInfo.OrientationNum=ZNegative;
        }
    }

    return true;
}

void MainDialog::PreProcessDicomData()
{
    _seeds[0] = ProcessInfo.SeedPoint_X;
    _seeds[1] = ProcessInfo.SeedPoint_Y;
    _seeds[2] = ProcessInfo.SeedPoint_Z;

    if (m_pPreProMask==NULL)
    {
        m_pPreProMask = new uchar[DicomInfo.m_iVoxelSize];
        memset(m_pPreProMask,0,DicomInfo.m_iVoxelSize);
    }
    else
    {
        delete []m_pPreProMask;
        m_pPreProMask = new uchar[DicomInfo.m_iVoxelSize];
        memset(m_pPreProMask,0,DicomInfo.m_iVoxelSize);
    }

    if (m_pPreProContour==NULL)
    {
        m_pPreProContour = new uchar[DicomInfo.m_iVoxelSize];
        memset(m_pPreProContour,0,DicomInfo.m_iVoxelSize);
    }
    else
    {
        delete []m_pPreProContour;
        m_pPreProContour = new uchar[DicomInfo.m_iVoxelSize];
        memset(m_pPreProContour,0,DicomInfo.m_iVoxelSize);
    }

    if(!DicomPreInfo.bNeedRotate)
    {


        qCDebug(HIFUNAVIGATION()) << "stop process dicom!!!";
        return;
    }

    int xos=2-(abs((int)DicomInfo.OrientationRow)-1);
    int yos=2-(abs((int)DicomInfo.OrientationCol)-1);
    int zos=2-(abs((int)DicomInfo.OrientationNum)-1);
    bool xflip=DicomInfo.OrientationRow<Unknown?true:false;
    bool yflip=DicomInfo.OrientationCol<Unknown?true:false;
    bool zflip=DicomInfo.OrientationNum<Unknown?true:false;
    int axeParm[3];
    double nspacing[3];
    int seedtemp[3];
    seedtemp[0] = ProcessInfo.SeedPoint_X;
    seedtemp[1] = ProcessInfo.SeedPoint_Y;
    seedtemp[2] = ProcessInfo.SeedPoint_Z;
    if(abs((int)DicomInfo.OrientationRow)!=1)
    {
        if(abs((int)DicomInfo.OrientationCol)==1)
        {
            axeParm[0]=DicomInfo.height;
            nspacing[0]=DicomInfo.spacing_y;
            xos=1;

        }
        else
        {
            axeParm[0]=DicomInfo.depth;
            nspacing[0]=DicomInfo.spacing_z;
            xos=2;
        }
    }
    else
    {
        axeParm[0]=DicomInfo.width;
        nspacing[0]=DicomInfo.spacing_x;
        xos=0;
    }
    if(abs((int)DicomInfo.OrientationCol)!=2)
    {
        if(abs((int)DicomInfo.OrientationRow)==2)
        {
            axeParm[1]=DicomInfo.width;
            nspacing[1]=DicomInfo.spacing_x;
            yos=0;
        }
        else
        {
            axeParm[1]=DicomInfo.depth;
            nspacing[1]=DicomInfo.spacing_z;
            yos=2;
        }
    }
    else
    {
        axeParm[1]=DicomInfo.height;
        nspacing[1]=DicomInfo.spacing_y;
        yos=1;
    }
    if(abs((int)DicomInfo.OrientationNum)!=3)
    {
        if(abs((int)DicomInfo.OrientationRow)==3)
        {
            axeParm[2]=DicomInfo.width;
            nspacing[2]=DicomInfo.spacing_x;
            zos=0;
        }
        else
        {
            axeParm[2]=DicomInfo.height;
            nspacing[2]=DicomInfo.spacing_y;
            zos=1;
        }
    }
    else
    {
        axeParm[2]=DicomInfo.depth;
        nspacing[2]=DicomInfo.spacing_z;
        zos=2;
    }

    qDebug()<<"axeParm"<<axeParm[0]<<axeParm[1]<<axeParm[2];
    qDebug()<<"sapcing"<<nspacing[0]<<nspacing[1]<<nspacing[2];
    qDebug()<<"os"<<xos<<yos<<zos;

    DicomPreInfo.width=axeParm[0];
    DicomPreInfo.height=axeParm[1];
    DicomPreInfo.depth=axeParm[2];
    DicomPreInfo.m_iVoxelSize = axeParm[0]*axeParm[1]*axeParm[2];


    qCDebug(HIFUNAVIGATION()) << "cycle step 1!";
    qCDebug(HIFUNAVIGATION()) << "m_iVoxelSize:"<<DicomPreInfo.m_iVoxelSize;
    qCDebug(HIFUNAVIGATION()) << "w:"<<DicomPreInfo.width;
    qCDebug(HIFUNAVIGATION()) << "h:"<<DicomPreInfo.height;
    qCDebug(HIFUNAVIGATION()) << "d:"<<DicomPreInfo.depth;


    DicomPreInfo.spacing_x=nspacing[0];
    DicomPreInfo.spacing_y=nspacing[1];
    DicomPreInfo.spacing_z=nspacing[2];
    DicomPreInfo.xyzos[0]=xos;
    DicomPreInfo.xyzos[1]=yos;
    DicomPreInfo.xyzos[2]=zos;

    if(_pPreProData!=NULL)
    {
        delete[] _pPreProData;
        _pPreProData=NULL;
    }
    _pPreProData=new short[DicomInfo.width*DicomInfo.height*DicomInfo.depth];
    int rcn[3];
    for(int r=0;r<DicomInfo.width;++r)
    {
        for(int c=0;c<DicomInfo.height;++c)
        {
            for(int n=0;n<DicomInfo.depth;++n)
            {
                rcn[0]=r;
                rcn[1]=c;
                rcn[2]=n;
                int x,y,z;
                if(xflip)
                {
                    //x=DicomInfo.width-rcn[xos]-1;
                    x=axeParm[0]-rcn[xos]-1;
                    _seeds[0]=axeParm[0]-seedtemp[xos]-1;
                }
                else
                {
                    x=rcn[xos];
                    _seeds[0]=seedtemp[xos];
                }
                if(yflip)
                {
                    //y=DicomInfo.height-rcn[yos]-1;
                    y=axeParm[1]-rcn[yos]-1;
                    _seeds[1]=axeParm[1]-seedtemp[yos]-1;
                }
                else
                {
                    y=rcn[yos];
                    _seeds[1]=seedtemp[yos];
                }
                if(zflip)
                {
                    //z=DicomInfo.depth-rcn[zos]-1;
                    z=axeParm[2]-rcn[zos]-1;
                    _seeds[2]=axeParm[2]-seedtemp[zos]-1;
                }
                else
                {
                    z=rcn[zos];
                    _seeds[2]=seedtemp[zos];
                }
                _pPreProData[x+y*axeParm[0]+z*axeParm[0]*axeParm[1]]=_curdata[r+c*DicomInfo.width+n*DicomInfo.width*DicomInfo.height];

                if (RegionGrowImageDataArray!=NULL)
                {
                    if (RegionGrowImageDataArray[r+c*DicomInfo.width+n*DicomInfo.width*DicomInfo.height]==1)
                    {
                        m_pPreProMask[x+y*axeParm[0]+z*axeParm[0]*axeParm[1]]=1;
                    }
                }

                if (ContoursMaskArray!=NULL)
                {
                    if (ContoursMaskArray[r+c*DicomInfo.width+n*DicomInfo.width*DicomInfo.height]==1)
                    {
                        m_pPreProContour[x+y*axeParm[0]+z*axeParm[0]*axeParm[1]]=1;
                    }
                }

            }
        }
    }

    if (!RegionGrowImageDataArray!=NULL)
    {
        PreProMaskImport->SetDataSpacing(nspacing);
        PreProMaskImport->SetDataOrigin(_inImageData->GetOrigin());
        PreProMaskImport->SetWholeExtent(0, axeParm[0]-1, 0, axeParm[1]-1, 0, axeParm[2]-1);
        PreProMaskImport->SetDataExtentToWholeExtent();
        PreProMaskImport->SetDataScalarTypeToUnsignedChar();
        PreProMaskImport->SetNumberOfScalarComponents(1);
        PreProMaskImport->SetImportVoidPointer(m_pPreProMask);
        PreProMaskImport->Update();
        _preProMaskData = PreProMaskImport->GetOutput();
    }

    if (ContoursMaskArray!=NULL)
    {

        PreProContourImport->SetDataSpacing(nspacing);
        PreProContourImport->SetDataOrigin(_inImageData->GetOrigin());
        PreProContourImport->SetWholeExtent(0, axeParm[0]-1, 0, axeParm[1]-1, 0, axeParm[2]-1);
        PreProContourImport->SetDataExtentToWholeExtent();
        PreProContourImport->SetDataScalarTypeToUnsignedChar();
        PreProContourImport->SetNumberOfScalarComponents(1);
        PreProContourImport->SetImportVoidPointer(m_pPreProContour);
        PreProContourImport->Update();

        _preProContourData = PreProContourImport->GetOutput();
    }

    PreProImageImport->SetDataSpacing(nspacing);
    PreProImageImport->SetDataOrigin(_inImageData->GetOrigin());
    PreProImageImport->SetWholeExtent(0, axeParm[0]-1, 0, axeParm[1]-1, 0, axeParm[2]-1);
    PreProImageImport->SetDataExtentToWholeExtent();
    PreProImageImport->SetDataScalarTypeToShort();
    PreProImageImport->SetNumberOfScalarComponents(1);
    PreProImageImport->SetImportVoidPointer(_pPreProData);
    PreProImageImport->Update();

    _preProImageData=PreProImageImport->GetOutput();
}

void MainDialog::on_verticalScrollBar_DicomChange_XY_valueChanged(int value)
{
    _imageViewer->SetSlice(value);
    int iSlice = _imageViewer->GetSlice();

    if(iSlice>=ProcessInfo.LimitRegion_z1&&iSlice<=ProcessInfo.LimitRegion_z2)
    {
        LimitedRegionRect_Actor->SetVisibility(1);
    }
    else
        LimitedRegionRect_Actor->SetVisibility(0);

    double* ori = _inImageData->GetOrigin();
    ori[2] = iSlice*DicomInfo.spacing_z;
    qDebug()<<"i slice is "<<iSlice;
    if(ProcessInfo.isThreshold)
    {
        thresholdImageImport->SetDataOrigin(ori);

        void* p = _inImageData->GetScalarPointer(0,0,iSlice);
        thresholdImageImport->SetImportVoidPointer(p);
        thresholdImageImport->Update();

        // thresholdMaskActor->SetZSlice(iSlice);
    }

    if(ProcessInfo.isRegionGrow)
    {
        void* p = RegionGrowImageDataArray+iSlice*DicomInfo.width*DicomInfo.height;
        regionGrowingImageImport->SetImportVoidPointer(p);

        regionGrowingImageImport->SetDataOrigin(ori);
        regionGrowingImageImport->Update();

        // regionGrowingMaskActor->SetZSlice(iSlice);
    }

     myContourWidget->SetCurSlice(iSlice);
//    if (contours.contains(iSlice)&&iSlice%2==0)
//    {
//        myContourWidget->Initialize(contours[iSlice]);
//    }
//    else
//    {
//        myContourWidget->Initialize();
//    }

    if (ProcessInfo.isShowContourMask)
    {
       contourActor->SetZSlice(iSlice);
    }

    myContourWidget->SetCurSlice(iSlice);
    _imageViewer->GetRenderWindow()->Render();

    ui->label_MouseInfo->setText(QString("X:%1  Y:%2  Z:%3").arg(m_iPosX).arg(m_iPosY).arg(iSlice));
}

void MainDialog::on_ThresholdValueChanged(int min,int max)
{
    UpdateLUT(min,max,true);
}

//QtVTKWidget鼠标移动事件
void MainDialog::on_qtVtkWidgetMouseMove(vtkObject*, unsigned long, void*, void*)
{
    if(isReadingDicom)
        return;

    int *pickPos=_renderWindowInteractor->GetEventPosition();
    int pointX=-1,pointY=-1,pointZ=-1;
    short pointValue=-1;
    if(GetPointInfoByMousePos(pickPos[0], pickPos[1], 0,pointX,pointY,pointZ,pointValue))
    {
        ui->label_MouseValue->setText(QString("%1").arg(pointValue));
        ui->label_MouseInfo->setText(QString("X:%1  Y:%2  Z:%3").arg(pointX).arg(pointY).arg(pointZ));
        if(ProcessInfo.isSettingLimitedRegion2)
        {
            UpdateRTRect(pickPos[0],pickPos[1],pointZ,false);
            int npx=0,npy=0,npz=0;
            short v;
            GetPointInfoByMousePos(rect_x1, rect_y1, 0,npx,npy,npz,v);
            LimitPointCache_x1=npx;
            LimitPointCache_y1=npy;
        }
        if(ProcessInfo.isRegionGrow)
        {
            if (pointX < 0 || pointX >=DicomInfo.width ||
                pointY < 0 || pointY >= DicomInfo.height ||
                pointZ < 0 || pointZ >= DicomInfo.depth)
            {
                qCDebug(HIFUNAVIGATION())<<HIFUNAVIGATION().categoryName()
                                        <<"update seed error "<<pointX <<" "<<pointY <<" "<<pointZ
                                       << TRACE_CMH();

                return;
            }

            UpdateGrowRegionPosInfo(RegionGrowImageDataArray[pointX+pointY*DicomInfo.width+pointZ*DicomInfo.width*DicomInfo.height]
                    ,pointX,pointY,pointZ);
        }
        if(ProcessInfo.isErasing||ProcessInfo.isFilling)
        {
            CurDisplayX=pickPos[0];
            CurDisplayY=pickPos[1];
            UpdateRTRect(pickPos[0],pickPos[1],pointZ,false);
        }
    }
    else
    {
        ui->label_MouseValue->setText(QString(""));
        ui->label_MouseInfo->setText(QString("No data"));
    }

    m_iPosX=pointX;
    m_iPosY=pointY;
    m_iPosZ=pointZ;
}

//QtVTKWidget鼠标点击事件获取种子点
void MainDialog::on_qtVtkWidgetMouseClicked(vtkObject *, unsigned long, void *, void *)
{
    if(isReadingDicom)
        return;

    int *pickPos=_renderWindowInteractor->GetEventPosition();
    int pointX=-1,pointY=-1,pointZ=-1;
    short pointValue=-1;
    qDebug()<<pickPos[0]<<pickPos[1]<<pickPos[2];
    if(!GetPointInfoByMousePos(pickPos[0], pickPos[1], 0,pointX,pointY,pointZ,pointValue))
    {
        if(ProcessInfo.isSetingSeedPoint)
        {
            SetSeedPos(-1,-1,-1);
        }
        return;
    }
    //设置种子点
    if(ProcessInfo.isSetingSeedPoint)
    {
        SetSeedPos(pointX,pointY,pointZ);
        ProcessInfo.SeedPoint_X=pointX;
        ProcessInfo.SeedPoint_Y=pointY;
        ProcessInfo.SeedPoint_Z=_imageViewer->GetSlice();
        LimitedRegionOperationSwitch(SetSeedPoint);
        qDebug()<<"Set Seed Point:"<<pointX<<pointY<<pointZ;
    }
    //设置限制区域
    if(ProcessInfo.isSettingLimitedRegion1)
    {
        LimitPointCache_x1=pointX;
        LimitPointCache_y1=pointY;
        //        ProcessInfo.LimitPoint1_x=pointX;
        //        ProcessInfo.LimitPoint1_y=pointY;
        //        ProcessInfo.LimitPoint1_z=pointZ;
        ProcessInfo.isSettingLimitedRegion1=false;
        ProcessInfo.isSettingLimitedRegion2=true;
//        SetLimitPoint1(LimitPointCache_x1,LimitPointCache_y1);
        rect_x1=pickPos[0];
        rect_y1=pickPos[1];
    }
    else if(ProcessInfo.isSettingLimitedRegion2)
    {
        ProcessInfo.LimitPoint1_x=LimitPointCache_x1;
        ProcessInfo.LimitPoint1_y=LimitPointCache_y1;
        ProcessInfo.LimitPoint1_z=pointZ;
        ProcessInfo.LimitPoint2_x=pointX;
        ProcessInfo.LimitPoint2_y=pointY;
        ProcessInfo.LimitPoint2_z=pointZ;
        UpdateLimitedRegion(pointX,pointY,pointZ,false);
        UpdateRTRect(0,0,0,true);
        int limitw=abs(ProcessInfo.LimitPoint2_x-ProcessInfo.LimitPoint1_x);
        int limith=abs(ProcessInfo.LimitPoint2_y-ProcessInfo.LimitPoint1_y);
//        SetLimitPoint1(ProcessInfo.LimitPoint1_x,ProcessInfo.LimitPoint1_y);
//        SetLimitPoint2(ProcessInfo.LimitPoint2_x,ProcessInfo.LimitPoint2_y,limitw,limith);
        LimitedRegionOperationSwitch(SetLimitedRegion);
    }
    //擦除区域
    if(ProcessInfo.isErasing)
    {
        EraseRegion(pointX,pointY,pointZ,ProcessInfo.EraseSize,0);
    }
    else if(ProcessInfo.isFilling)
    {
        EraseRegion(pointX,pointY,pointZ,ProcessInfo.FillSize,1);
    }
}

void MainDialog::on_qtVtkWidgetMiddleButtonPressEvent(vtkObject*, unsigned long, void*, void*)
{
    UpdateRTRect(0,0,0,true);
}

void MainDialog::on_qtVtkWidgetMouseWheelForwardEvent(vtkObject*, unsigned long, void*, void*)
{
    if(ProcessInfo.isErasing||ProcessInfo.isFilling)
    {
        GetRatioWorldToDispaly(XRatio,YRatio);
        UpdateRTRect(CurDisplayX,CurDisplayY,0,false);
    }
    if(ProcessInfo.isSettingLimitedRegion2)
    {
        int npx=0,npy=0,npz=0;
        short v;
        GetPointInfoByMousePos(rect_x1, rect_y1, 0,npx,npy,npz,v);
        LimitPointCache_x1=npx;
        LimitPointCache_y1=npy;
    }
}

void MainDialog::on_qtVtkWidgetMouseWheelBackwardEvent(vtkObject*, unsigned long, void*, void*)
{
    if(ProcessInfo.isErasing||ProcessInfo.isFilling)
    {
        GetRatioWorldToDispaly(XRatio,YRatio);
        UpdateRTRect(CurDisplayX,CurDisplayY,0,false);
    }
    if(ProcessInfo.isSettingLimitedRegion2)
    {
        int npx=0,npy=0,npz=0;
        short v;
        GetPointInfoByMousePos(rect_x1, rect_y1, 0,npx,npy,npz,v);
        LimitPointCache_x1=npx;
        LimitPointCache_y1=npy;
    }
}
//int ilog=0;
void MainDialog::on_qtVtkWiggetRenderEvent(vtkObject*, unsigned long, void*, void*)
{
    // qDebug()<<"My Event ******************************";
    //    ilog++;
    if(ProcessInfo.isShowLimitedRegion)
    {
        UpdateLimitedRegion(ProcessInfo.LimitPoint2_x,ProcessInfo.LimitPoint2_y,0,false);
    }
}

//获取鼠标位置的点信息
bool MainDialog::GetPointInfoByMousePos(int posx,int posy,int posz,int& pointX,int& pointY,int& pointZ,short& value)
{
    if(_picker==NULL)
        return false;

    qDebug()<<"pick pos is "<<posx<<" "<<posy<<" "<<posz;

    int i = _picker->Pick(posx, posy, posz, _imageViewer->GetRenderer());
    qDebug()<<"pick return value is "<<i;
    if(_picker->GetCellId() == -1)
    {
        return false;
    }
    //    qDebug()<<"eventposition:"<<posx<<posy<<posz;
    // double* worldPosition = _picker->GetPickPosition(); //世界坐标位置
    //    qDebug()<<"worldPosition"<<worldPosition[0]<<worldPosition[1]<<worldPosition[2];
    //    double displypt[3];
    //    vtkInteractorObserver::ComputeWorldToDisplay(_imageViewer->GetRenderer(),worldPosition[0],worldPosition[1],worldPosition[2],displypt);
    //    qDebug()<<"DisplayPosition:"<<displypt[0]<<displypt[1]<<displypt[2];
    int *posCoord=_picker->GetPointIJK();  //点索引位置

    int extent[6];
    _inImageData->GetExtent(extent);
    qDebug()<<"pos is "<<posCoord[0]<<" "<<posCoord[1]<<" "<<posCoord[2];
    qDebug()<<"extent is "<<extent[0]<<" "<<extent[1]<<" "<<extent[2]<<" "
           <<extent[3]<<" "<<extent[4]<<" "<<extent[5];


    if (posCoord[0]>=extent[0]&&posCoord[0]<=extent[1]
            &&posCoord[1]>=extent[2]&&posCoord[1]<=extent[3]
            &&posCoord[2]>=extent[4]&&posCoord[2]<=extent[5])
    {
        short* posValue=(short*)_inImageData->GetScalarPointer(posCoord[0],posCoord[1],posCoord[2]);
        pointX=posCoord[0];
        pointY=posCoord[1];
        pointZ=_imageViewer->GetSlice();
        value=posValue[0];
    }
    else
    {
        pointX=posCoord[0];
        pointY=posCoord[1];
        pointZ=_imageViewer->GetSlice();
        value=-1;
    }


    return true;
}

void MainDialog::UpdateLUT(int min,int max,bool bUpdataMask)
{
//#if 0
    if(lut==NULL||thresholdMasMapper==NULL||_imageViewer==NULL)
        return;
    lut->SetNumberOfTableValues(4096);
    lut->SetRange(0, 4095);
    for (double i = 0; i < 4096; i++)
    {
        if (i>min && i<=max)
        {
            lut->SetTableValue(i, 1.0, 0.0, 0.0, 0.5);
        }
        else
        {
            lut->SetTableValue(i, 0.0, 0.0, 0.0, 0.0);
        }
    }
    lut->Build();
    if(bUpdataMask)
    {
        thresholdMasMapper->Update();
        _imageViewer->GetRenderWindow()->Render();
    }

    ProcessInfo.threshold_min=min;
    ProcessInfo.threshold_max=max;
//#endif
}

void MainDialog::InitThresholdMaskMap()
{
    _imageViewer->GetRenderer()->RemoveActor(thresholdMaskActor);
    if(thresholdMasMapper->GetNumberOfInputConnections(0)==0)
    {
        thresholdImageImport->SetDataSpacing(DicomInfo.spacing_x, DicomInfo.spacing_y, DicomInfo.spacing_z);
        thresholdImageImport->SetDataOrigin(_inImageData->GetOrigin());
        thresholdImageImport->SetWholeExtent(0, DicomInfo.width-1, 0, DicomInfo.height-1, 0, 0);

        thresholdImageImport->SetDataExtentToWholeExtent();
        thresholdImageImport->SetDataScalarType(VTK_SHORT);
        thresholdImageImport->SetNumberOfScalarComponents(1);
        void* p = _inImageData->GetScalarPointer(0,0,_imageViewer->GetSlice());
        thresholdImageImport->SetImportVoidPointer(p);
        thresholdImageImport->Update();

        thresholdMasMapper->SetLookupTable(lut);
        thresholdMasMapper->PassAlphaToOutputOn(); //?
        thresholdMasMapper->SetInputData(0,thresholdImageImport->GetOutput());
    }
    thresholdMasMapper->Update();

    thresholdMaskActor->GetMapper()->RemoveAllInputs();
    thresholdMaskActor->GetMapper()->RemoveAllInputConnections(0);
    thresholdMaskActor->GetMapper()->SetInputConnection(0,thresholdMasMapper->GetOutputPort());
    // thresholdMaskActor->SetDisplayExtent(0, DicomInfo.width-1, 0, DicomInfo.height-1, 0, DicomInfo.depth-1);

    _imageViewer->GetRenderer()->AddActor(thresholdMaskActor);
    // thresholdMaskActor->SetZSlice(_imageViewer->GetSlice());
    _imageViewer->GetRenderWindow()->Render();

    ProcessInfo.isThreshold=true;
    ProcessInfo.isShowThresholdMask=true;
}

void MainDialog::RemoveMaskActor(int maskNum)
{
    if(maskNum==0)
    {
        _imageViewer->GetRenderer()->RemoveActor(thresholdMaskActor);
        ProcessInfo.isShowThresholdMask=false;
    }
    else if(maskNum==1)
    {
        _imageViewer->GetRenderer()->RemoveActor(regionGrowingMaskActor);
        ProcessInfo.isShowRegionGrowMask=false;
    }
    else if (maskNum==2)
    {
        _imageViewer->GetRenderer()->RemoveActor(contourActor);
        ProcessInfo.isShowContourMask=false;
    }

    _imageViewer->GetRenderWindow()->Render();
}

void MainDialog::AddMaskActor(int maskNum)
{
    if(maskNum==0)
    {
        _imageViewer->GetRenderer()->AddActor(thresholdMaskActor);
        // thresholdMaskActor->SetZSlice(_imageViewer->GetSlice());
        ProcessInfo.isShowThresholdMask=true;
    }
    else if(maskNum==1)
    {
        _imageViewer->GetRenderer()->AddActor(regionGrowingMaskActor);
        // regionGrowingMaskActor->SetZSlice(_imageViewer->GetSlice());
        ProcessInfo.isShowRegionGrowMask=true;
    }
    else if(maskNum==2)
    {
        _imageViewer->GetRenderer()->AddActor(contourActor);
        ProcessInfo.isShowContourMask=true;
    }

    _imageViewer->GetRenderWindow()->Render();
}

void MainDialog::GetContours()
{
    int extent[6];
    double spacing[3];
    double origin[3];

    if (_preProContourData==NULL)
    {
        qCDebug(HIFUNAVIGATION())<<HIFUNAVIGATION().categoryName()
                                <<"get contour zero !"
                               << TRACE_CMH();

        return;
    }

    _preProContourData->GetOrigin(origin);
    _preProContourData->GetSpacing(spacing);
    _preProContourData->GetExtent(extent);

    // 获取最小维度
    int* dims = _preProContourData->GetDimensions();

    float lengths[3];

    lengths[0] = dims[0]*spacing[0];
    lengths[1] = dims[1]*spacing[1];
    lengths[2] = dims[2]*spacing[2];

    float minSize = lengths[0]<lengths[1]?lengths[0]:lengths[1];
    minSize = minSize < lengths[2]?minSize:lengths[2];

    // qDebug()<<"min size is "<<minSize;

    int size[3];
    size[0] = minSize/spacing[0];
    size[1] = minSize/spacing[1];
    size[2] = minSize/spacing[2];

    //    qDebug()<<"size 0 is "<<size[0] << " size 1 is "<<size[1] << " size 2 is "<<size[2];
    //    qDebug()<<"seeds 0 is "<<_seeds[0] << " seed 1 is "<<_seeds[1] << " seed 2 is "<<_seeds[2];

    // 截取肌瘤附近的区域重新切片
    _extraVOI->SetInputData(_preProContourData);
    _extraVOI->SetVOI((_seeds[0]-0.5*size[0])<0?0:(_seeds[0]-0.5*size[0]),
            (_seeds[0]+0.5*size[0])>dims[0]-1?dims[0]-1:(_seeds[0]+0.5*size[0]),
        (_seeds[1]-0.5*size[1])<0?0:(_seeds[1]-0.5*size[1]),
        (_seeds[1]+0.5*size[1])>dims[1]-1?dims[1]-1:(_seeds[1]+0.5*size[1]),
        (_seeds[2]-0.5*size[2])<0?0:(_seeds[2]-0.5*size[2]),
        (_seeds[2]+0.5*size[2])>dims[2]-1?dims[2]-1:(_seeds[2]+0.5*size[2]));

    _extraVOI->Update();

    _extraVOI->GetOutput()->GetOrigin(origin);
    _extraVOI->GetOutput()->GetSpacing(spacing);
    _extraVOI->GetOutput()->GetExtent(extent);

     //qDebug()<<"Dimens is "<< dims[0] << " "<< dims[1] << " "<< dims[2];
     //qDebug()<<"extent is "<< extent[0] << " "<< extent[1] << " "<< extent[2]<< " "<< extent[3] << " "<< extent[4] << " "<< extent[5];

    double center[3];
    center[0] = origin[0]+(extent[0]+extent[1])*spacing[0]*0.5;
    center[1] = origin[1]+(extent[2]+extent[3])*spacing[1]*0.5;
    center[2] = origin[2]+(extent[4]+extent[5])*spacing[2]*0.5;
    //*****************************************************************//

    _reslice->SetInputConnection(_extraVOI->GetOutputPort());
    _reslice->SetOutputDimensionality(2);
    _reslice->SetInterpolationModeToLinear();

    //*****************************************************************//
    _colorTable->SetRange(0, 1);
    _colorTable->SetValueRange(0.0, 1.0);
    _colorTable->SetSaturationRange(0.0, 0.0);
    _colorTable->SetRampToLinear();
    _colorTable->Build();

    double dir = -1.0;
    if (m_iPatientOrientation==1)
    {
        dir = 1.0;
    }



    for (int i = 0; i < 181;++i)
    {
        double theta = dir*PI*(i-90)/180.0;
        double coronalElements[16] = {
            cos(theta), 0, -sin(theta), center[0],
            0, 1, 0, center[1],
            sin(theta), 0, cos(theta), center[2],
            0, 0, 0, 1 };

        _resliceAxes->DeepCopy(coronalElements);
        _reslice->SetResliceAxes(_resliceAxes);

        _colorMap->SetLookupTable(_colorTable);
        _colorMap->SetInputConnection(_reslice->GetOutputPort());
        _colorMap->Update();
        qCDebug(HIFUNAVIGATION())<<HIFUNAVIGATION().categoryName()
                                <<"--------------------1,count====="<<i
                               << TRACE_CMH();
        // _SliceImages.append(_colorMap->GetOutput());

        QImage image = vtkImageDataToQImage(_colorMap->GetOutput());
        QString path = "E:\\testimage\\";
        path.append(QString::number(i));
        path.append(".png");
        image.save(path);

         emit sendContour(vtkImageDataToQImage(_colorMap->GetOutput()),i);
        qCDebug(HIFUNAVIGATION())<<HIFUNAVIGATION().categoryName()
                                <<"--------------------2,count====="<<i
                               << TRACE_CMH();

    }



}

void MainDialog::GetContoursNew()
{
    PreProcessDicomData();

    int extent[6];
    double spacing[3];
    double origin[3];

    if (_preProMaskData==NULL)
    {
        qCDebug(HIFUNAVIGATION())<<HIFUNAVIGATION().categoryName()
                                <<"get contour zero !"
                               << TRACE_CMH();

        return;
    }

    _preProMaskData->GetOrigin(origin);
    _preProMaskData->GetSpacing(spacing);
    _preProMaskData->GetExtent(extent);

    // 获取最小维度
    int* dims = _preProMaskData->GetDimensions();

    float lengths[3];

    lengths[0] = dims[0]*spacing[0];
    lengths[1] = dims[1]*spacing[1];
    lengths[2] = dims[2]*spacing[2];

    float minSize = lengths[0]<lengths[1]?lengths[0]:lengths[1];
    minSize = minSize < lengths[2]?minSize:lengths[2];

    int size[3];
    size[0] = minSize/spacing[0];
    size[1] = minSize/spacing[1];
    size[2] = minSize/spacing[2];

    //    qDebug()<<"size 0 is "<<size[0] << " size 1 is "<<size[1] << " size 2 is "<<size[2];
    //    qDebug()<<"seeds 0 is "<<_seeds[0] << " seed 1 is "<<_seeds[1] << " seed 2 is "<<_seeds[2];

    // 截取肌瘤附近的区域重新切片
    _extraVOI->SetInputData(_preProMaskData);
    _extraVOI->SetVOI((_seeds[0]-0.5*size[0])<0?0:(_seeds[0]-0.5*size[0]),
            (_seeds[0]+0.5*size[0])>dims[0]-1?dims[0]-1:(_seeds[0]+0.5*size[0]),
        (_seeds[1]-0.5*size[1])<0?0:(_seeds[1]-0.5*size[1]),
        (_seeds[1]+0.5*size[1])>dims[1]-1?dims[1]-1:(_seeds[1]+0.5*size[1]),
        (_seeds[2]-0.5*size[2])<0?0:(_seeds[2]-0.5*size[2]),
        (_seeds[2]+0.5*size[2])>dims[2]-1?dims[2]-1:(_seeds[2]+0.5*size[2]));

    _extraVOI->Update();

    _extraVOI->GetOutput()->GetOrigin(origin);
    _extraVOI->GetOutput()->GetSpacing(spacing);
    _extraVOI->GetOutput()->GetExtent(extent);

    // qDebug()<<"Dimens is "<< dims[0] << " "<< dims[1] << " "<< dims[2];
    // qDebug()<<"extent is "<< extent[0] << " "<< extent[1] << " "<< extent[2]<< " "<< extent[3] << " "<< extent[4] << " "<< extent[5];

    double center[3];
    center[0] = origin[0]+(extent[0]+extent[1])*spacing[0]*0.5;
    center[1] = origin[1]+(extent[2]+extent[3])*spacing[1]*0.5;
    center[2] = origin[2]+(extent[4]+extent[5])*spacing[2]*0.5;
    //*****************************************************************//

    _reslice->SetInputConnection(_extraVOI->GetOutputPort());
    _reslice->SetOutputDimensionality(2);
    _reslice->SetInterpolationModeToLinear();

    //*****************************************************************//
    _colorTable->SetRange(0, 1);
    _colorTable->SetValueRange(0.0, 1.0);
    _colorTable->SetSaturationRange(0.0, 0.0);
    _colorTable->SetRampToLinear();
    _colorTable->Build();

    double dir = -1.0;
    if (m_iPatientOrientation==1)
    {
        dir = 1.0;
    }

    for (int i = 0; i < 181;i++)
    {
        if(i%4!=0)
        {
            continue;
        }

        double theta = dir*PI*(i-90)/180.0;
        double coronalElements[16] = {
            cos(theta), 0, -sin(theta), center[0],
            0, 1, 0, center[1],
            sin(theta), 0, cos(theta), center[2],
            0, 0, 0, 1 };

        _resliceAxes->DeepCopy(coronalElements);
        _reslice->SetResliceAxes(_resliceAxes);

        _colorMap->SetLookupTable(_colorTable);
        _colorMap->SetInputConnection(_reslice->GetOutputPort());
        _colorMap->Update();

        // _SliceImages.append(_colorMap->GetOutput());
        emit sendContour(vtkImageDataToQImage(_colorMap->GetOutput()),i);

    }

}

void MainDialog::sendGetCoutour()
{
    GetContours();
}

void MainDialog::GetSlices()
{
    if(m_bIsPreProcessed == false)
    {
    PreProcessDicomData();//计算contour和image数据
    m_bIsPreProcessed=true;
    //qDebug()<<"GetSlices里面_preProImageData是"<<_preProContourData;

    }

    int extent[6];
    double spacing[3];
    double origin[3];

    if (_preProImageData==NULL)
    {
        qCDebug(HIFUNAVIGATION())<<HIFUNAVIGATION().categoryName()
                                <<"get contour zero !"
                               << TRACE_CMH();

        return;
    }

    _preProImageData->GetOrigin(origin);
    _preProImageData->GetSpacing(spacing);
    _preProImageData->GetExtent(extent);

    // 获取最小维度
    int* dims = _preProImageData->GetDimensions();

    float lengths[3];

    lengths[0] = dims[0]*spacing[0];
    lengths[1] = dims[1]*spacing[1];
    lengths[2] = dims[2]*spacing[2];

    float minSize = lengths[0]<lengths[1]?lengths[0]:lengths[1];
    minSize = minSize < lengths[2]?minSize:lengths[2];

    // qDebug()<<"min size is "<<minSize;

    int size[3];
    size[0] = minSize/spacing[0];
    size[1] = minSize/spacing[1];
    size[2] = minSize/spacing[2];

    //    qDebug()<<"size 0 is "<<size[0] << " size 1 is "<<size[1] << " size 2 is "<<size[2];
    //    qDebug()<<"seeds 0 is "<<_seeds[0] << " seed 1 is "<<_seeds[1] << " seed 2 is "<<_seeds[2];

    // 截取肌瘤附近的区域重新切片
    _extraVOI->SetInputData(_preProImageData);
    _extraVOI->SetVOI((_seeds[0]-0.5*size[0])<0?0:(_seeds[0]-0.5*size[0]),
            (_seeds[0]+0.5*size[0])>dims[0]-1?dims[0]-1:(_seeds[0]+0.5*size[0]),
        (_seeds[1]-0.5*size[1])<0?0:(_seeds[1]-0.5*size[1]),
        (_seeds[1]+0.5*size[1])>dims[1]-1?dims[1]-1:(_seeds[1]+0.5*size[1]),
        (_seeds[2]-0.5*size[2])<0?0:(_seeds[2]-0.5*size[2]),
        (_seeds[2]+0.5*size[2])>dims[2]-1?dims[2]-1:(_seeds[2]+0.5*size[2]));

    _extraVOI->Update();

    _extraVOI->GetOutput()->GetOrigin(origin);
    _extraVOI->GetOutput()->GetSpacing(spacing);
    _extraVOI->GetOutput()->GetExtent(extent);

    // qDebug()<<"Dimens is "<< dims[0] << " "<< dims[1] << " "<< dims[2];
    // qDebug()<<"extent is "<< extent[0] << " "<< extent[1] << " "<< extent[2]<< " "<< extent[3] << " "<< extent[4] << " "<< extent[5];

    double center[3];
    center[0] = origin[0]+(extent[0]+extent[1])*spacing[0]*0.5;
    center[1] = origin[1]+(extent[2]+extent[3])*spacing[1]*0.5;
    center[2] = origin[2]+(extent[4]+extent[5])*spacing[2]*0.5;
    qDebug()<<"中心点是："<<center[0]<<center[1]<<center[2];
    //*****************************************************************//

    _reslice->SetInputConnection(_extraVOI->GetOutputPort());
    _reslice->SetOutputDimensionality(2);
    _reslice->SetInterpolationModeToCubic();

    //*****************************************************************//
    _colorTable->SetRange(0, 1000);
    _colorTable->SetValueRange(0.0, 1.0);
    _colorTable->SetSaturationRange(0.0, 0.0);
    _colorTable->SetRampToLinear();
    _colorTable->Build();

    double dir = -1.0;
    if (m_iPatientOrientation==1)
    {
        dir = 1.0;
    }

    for (int i = 0; i < 181;++i)
    {
        double theta = dir*PI*(i-90)/180.0;
        double coronalElements[16] = {
            cos(theta), 0, -sin(theta), center[0],
            0, 1, 0, center[1],
            sin(theta), 0, cos(theta), center[2],
            0, 0, 0, 1 };

        _resliceAxes->DeepCopy(coronalElements);
        _reslice->SetResliceAxes(_resliceAxes);

        _colorMap->SetLookupTable(_colorTable);
        _colorMap->SetInputConnection(_reslice->GetOutputPort());
        _colorMap->Update();

        // _SliceImages.append(_colorMap->GetOutput());
        emit sendImageData(vtkImageDataToResizeQImage(_colorMap->GetOutput(),320,320),i);

    }
}

//void MainDialog::on_pushButton_quit_clicked()
//{
//    this->close();
//}

void MainDialog::setOrientation(int ori)
{
    m_iPatientOrientation = ori;
}

// mask空洞填充
void MainDialog::fillHole(const DicomSourceInfo& info,QVector<uchar>& mask)
{
    // using namespace cv;

    for (int i = 0; i < info.depth;++i)
    {
        int index = info.width*info.height*i;
        cv::Mat srcBw(info.height, info.width, CV_8UC1, &mask[index]);

        Size m_Size = srcBw.size();
        cv::Mat Temp=cv::Mat::zeros(m_Size.height+2,m_Size.width+2,srcBw.type());//延展图像
        srcBw.copyTo(Temp(cv::Range(1, m_Size.height + 1), cv::Range(1, m_Size.width + 1)));

        cv::floodFill(Temp, Point(0, 0), Scalar(255));

        cv::Mat cutImg;//裁剪延展的图像
        Temp(cv::Range(1, m_Size.height + 1), cv::Range(1, m_Size.width + 1)).copyTo(cutImg);

        cv::Mat dstBw;
        dstBw = srcBw | (~cutImg);

        uchar* pDest = &mask[index];
        uchar* pSource = dstBw.data;
        for (int j = 0; j < info.height; ++j)
        {
            for (int i = 0; i < info.width; ++i)
            {
                if (*pSource==255)
                {
                    *pDest = 1;
                }
                else
                {
                    *pDest=*pSource;
                }

                ++pSource;
                ++pDest;
            }
        }

    }
}

void MainDialog::SeedRegionGrowingProcess(bool bClear)
{
    if (RegionGrowImageDataArray!=NULL)
    {
        delete []RegionGrowImageDataArray;
    }

    qDebug()<<"allocate memory size is "<<DicomInfo.m_iVoxelSize;
    RegionGrowImageDataArray = new uchar[DicomInfo.m_iVoxelSize];
    memset(RegionGrowImageDataArray,0,DicomInfo.m_iVoxelSize);

    if (ContoursMaskArray!=NULL)
    {
        delete []ContoursMaskArray;
        ContoursMaskArray = new uchar[DicomInfo.m_iVoxelSize];
        memset(ContoursMaskArray,0,DicomInfo.m_iVoxelSize);
    }

    if(!bClear)
    {
        Hap.SeededGrow3dProcess(_curdata, DicomInfo.width, DicomInfo.height, DicomInfo.depth,
                                ProcessInfo.SeedPoint_X, ProcessInfo.SeedPoint_Y, ProcessInfo.SeedPoint_Z,
                                ProcessInfo.threshold_min, ProcessInfo.threshold_max,RegionGrowImageDataArray);

        Hap.DataPostProcess(DicomInfo.width,DicomInfo.height,DicomInfo.depth,RegionGrowImageDataArray);
        // fillHole(DicomInfo,RegionGrowImageDataArray);
    }

    if(!ProcessInfo.isRegionGrow)
    {
        InitRegionGrowMask();
    }
    else
    {
        regionGrowingMapper->RemoveAllInputConnections(0);
        regionGrowingMapper->SetInputConnection(0,regionGrowingImageImport->GetOutputPort());
        regionGrowingMapper->Update();
        _imageViewer->GetRenderWindow()->Render();
        if(!ProcessInfo.isShowRegionGrowMask)
        {
            _imageViewer->GetRenderer()->AddActor(regionGrowingMaskActor);
            // regionGrowingMaskActor->SetZSlice(_imageViewer->GetSlice());
            _imageViewer->GetRenderWindow()->Render();
            ProcessInfo.isShowRegionGrowMask=true;
        }
    }

    if(bClear)
    {
        delete []RegionGrowImageDataArray;
        RegionGrowImageDataArray = NULL;
        ProcessInfo.isRegionGrow=false;
    }
    qDebug()<<"种子点区域生成操作 Clear:"<<bClear<<" Array size:"<<DicomInfo.m_iVoxelSize;
}

void MainDialog::ManualDrawProcess(bool bClear)
{
//#if 0
    if (RegionGrowImageDataArray!=NULL)
    {
        delete []RegionGrowImageDataArray;
        RegionGrowImageDataArray = NULL;
    }

    qDebug()<<"allocate memory size is "<<DicomInfo.m_iVoxelSize;
    RegionGrowImageDataArray = new uchar[DicomInfo.m_iVoxelSize];
    memset(RegionGrowImageDataArray,0,DicomInfo.m_iVoxelSize);


    if(!bClear)
    {
        QMap<int,vtkSmartPointer<vtkPolyData> >* ContourList=myContourWidget->GetConturList_Rep();

        qDebug()<<"Enter set contours !";
        qDebug()<<"contour list rep size is "<<myContourWidget->GetConturList_Rep()->size();
        QMap<int,vtkSmartPointer<vtkPolyData> >::const_iterator i = ContourList->constBegin();
        while (i != ContourList->constEnd())
        {
            int iz=i.key();
            vtkSmartPointer<vtkPolyData> contourdata=vtkSmartPointer<vtkPolyData>::New();
            contourdata->DeepCopy(i.value());
            //contourdata=i.value();
            vtkSmartPointer<vtkPolygon> polygon =
                    vtkSmartPointer<vtkPolygon>::New();
            for(int i=0;i<contourdata->GetNumberOfPoints();i++)
            {
                vtkIdType id(i);
                double *p;
                p=contourdata->GetPoint(id);
                polygon->GetPoints()->InsertNextPoint(p[0],p[1],0);
            }
            double n[3];
            polygon->ComputeNormal(polygon->GetPoints()->GetNumberOfPoints(),
                                   static_cast<double*>(polygon->GetPoints()->GetData()->GetVoidPointer(0)), n);

            double bounds[6];
            polygon->GetPoints()->GetBounds(bounds);

            double testInside[3] = {0.0, 0.0, 0.0};
            double *contourbound=contourdata->GetBounds();
            contourbound[0]=(contourbound[0]-DicomInfo.spacing_x)/DicomInfo.spacing_x;
            contourbound[1]=(contourbound[1]+DicomInfo.spacing_x)/DicomInfo.spacing_x;
            contourbound[2]=(contourbound[2]-DicomInfo.spacing_y)/DicomInfo.spacing_y;
            contourbound[3]=(contourbound[3]+DicomInfo.spacing_y)/DicomInfo.spacing_y;

            for(int ix=contourbound[0];ix<contourbound[1];++ix)
            {
                for(int iy=contourbound[2];iy<contourbound[3];++iy)
                {
                    testInside[0]=ix*DicomInfo.spacing_x;
                    testInside[1]=iy*DicomInfo.spacing_y;
                    if(polygon->PointInPolygon(testInside,
                                               polygon->GetPoints()->GetNumberOfPoints(), static_cast<double*>(
                                                   polygon->GetPoints()->GetData()->GetVoidPointer(0)), bounds, n))
                    {
                        RegionGrowImageDataArray[ix+iy*DicomInfo.width+iz*DicomInfo.width*DicomInfo.height]=1;
                    }
                }
            }
            ++i;
        }
    }

    if(!ProcessInfo.isRegionGrow)
        InitRegionGrowMask();
    else
    {
        regionGrowingMapper->RemoveAllInputConnections(0);
        regionGrowingMapper->SetInputConnection(0,regionGrowingImageImport->GetOutputPort());
        regionGrowingMapper->Update();
        _imageViewer->GetRenderWindow()->Render();
        if(!ProcessInfo.isShowRegionGrowMask)
        {
            _imageViewer->GetRenderer()->AddActor(regionGrowingMaskActor);
            // regionGrowingMaskActor->SetZSlice(_imageViewer->GetSlice());
            _imageViewer->GetRenderWindow()->Render();
            ProcessInfo.isShowRegionGrowMask=true;
        }
    }

    if(bClear)
    {
        delete []RegionGrowImageDataArray;
        RegionGrowImageDataArray = NULL;
        ProcessInfo.isRegionGrow=false;
    }
//#endif
    qDebug()<<"手动勾画区域生成操作 Clear:"<<bClear<<" Array size:"<<DicomInfo.m_iVoxelSize;
}

void MainDialog::SeedRegionReGrowingProcess()
{
    short* tempdata=new short[DicomInfo.width*DicomInfo.height*DicomInfo.depth];
    int px1,px2,py1,py2,pz1,pz2;
    bool bLimitCircle=false;
    double Circle_center[4]={0,0,0,0};
    double CirCle_r=0;
    if(ProcessInfo.ValidLimitRegion())
    {
        bLimitCircle=true;
        pz1=ProcessInfo.LimitRegion_z1;
        pz2=ProcessInfo.LimitRegion_z2;
        double* c2=LimitedRegionSource->GetCenter();
        double r2=LimitedRegionSource->GetRadius();
        double circle_top_point[4]={0,0,0};
        vtkInteractorObserver::ComputeDisplayToWorld(_imageViewer->GetRenderer(),c2[0],c2[1],c2[2],Circle_center);
        vtkInteractorObserver::ComputeDisplayToWorld(_imageViewer->GetRenderer(),c2[0],c2[1]+r2,c2[2],circle_top_point);
        CirCle_r=(circle_top_point[1]-Circle_center[1])/DicomInfo.spacing_y;
        Circle_center[0]=Circle_center[0]/DicomInfo.spacing_x;
        Circle_center[1]=Circle_center[1]/DicomInfo.spacing_y;
        px1=Circle_center[0]-CirCle_r-1;
        px2=Circle_center[0]+CirCle_r+1;
        py1=Circle_center[1]-CirCle_r-1;
        py2=Circle_center[1]+CirCle_r+1;
    }
    else
    {
        px1=0;
        px2=DicomInfo.width-1;
        py1=0;
        py2=DicomInfo.height-1;
        pz1=0;
        pz2=DicomInfo.depth-1;
    }

    if(RegionGrowImageDataArray==NULL)
    {
        return;
    }

    for(int ix=0;ix<DicomInfo.width;ix++)
    {
        for(int iy=0;iy<DicomInfo.height;iy++)
        {
            for(int iz=0;iz<DicomInfo.depth;iz++)
            {
                int index=ix+iy*DicomInfo.width+iz*DicomInfo.width*DicomInfo.height;
                if(ix>=px1&&ix<=px2&&iy>=py1&&iy<=py2&&iz>=pz1&&iz<=pz2)
                {
                    double lx=(double)ix-Circle_center[0];
                    double ly=(double)iy-Circle_center[1];
                    if((!bLimitCircle)||(bLimitCircle&&((lx*lx+ly*ly)<=(CirCle_r*CirCle_r))))
                    {
                        tempdata[index]=RegionGrowImageDataArray[index];
                    }
                    else
                    {
                        tempdata[index]=0;
                    }
                }
                else
                {
                    tempdata[index]=0;
                }
            }
        }
    }

    _imageViewer->GetRenderWindow()->Render();
    Hap.SeededGrow3dProcess(tempdata, DicomInfo.width, DicomInfo.height, DicomInfo.depth,
                            ProcessInfo.SeedPoint_X, ProcessInfo.SeedPoint_Y, ProcessInfo.SeedPoint_Z,
                            0, 2,RegionGrowImageDataArray);

    regionGrowingMapper->RemoveAllInputConnections(0);
    regionGrowingMapper->SetInputConnection(0,regionGrowingImageImport->GetOutputPort());
    regionGrowingMapper->Update();

    _imageViewer->GetRenderWindow()->Render();
    delete[] tempdata;
}

void MainDialog::InitContourMask()
{
//#if 0
    // _imageViewer->GetRenderer()->RemoveActor(contourActor);
    //初始化颜色表
    contourlut->SetNumberOfTableValues(2);
    contourlut->SetRange(0, 1);
    contourlut->SetTableValue(1, 0.0, 0.0, 1.0, 0.5);
    contourlut->SetTableValue(0, 0.0, 0.0, 0.0, 0.0);
    contourlut->Build();

    contourImport->SetDataSpacing(DicomInfo.spacing_x, DicomInfo.spacing_y, DicomInfo.spacing_z);
    contourImport->SetDataOrigin(_inImageData->GetOrigin());
    contourImport->SetWholeExtent(0, DicomInfo.width-1, 0, DicomInfo.height-1, 0, DicomInfo.depth-1);
    contourImport->SetDataExtentToWholeExtent();
    contourImport->SetDataScalarType(VTK_UNSIGNED_CHAR);
    contourImport->SetNumberOfScalarComponents(1);
    contourImport->SetImportVoidPointer(ContoursMaskArray);
    contourImport->Update();

    contourMapper->SetLookupTable(contourlut);
    contourMapper->PassAlphaToOutputOn(); //?
    contourMapper->SetInputConnection(0,contourImport->GetOutputPort());
    contourMapper->Update();

    contourActor->GetMapper()->SetInputConnection(0,contourMapper->GetOutputPort());
    int ext[6];
    _imageViewer->GetImageActor()->GetDisplayExtent(ext);
    contourActor->SetDisplayExtent(ext[0], ext[1], ext[2], ext[3], ext[4], ext[5]);
    // contourActor->SetDisplayExtent(0, DicomInfo.width-1, 0, DicomInfo.height-1, 0, DicomInfo.depth-1);

    _imageViewer->GetRenderer()->AddActor(contourActor);
    // regionGrowingMaskActor->SetZSlice(_imageViewer->GetSlice());
    myContourWidget->SetCurSlice(_imageViewer->GetSlice());
    _imageViewer->GetRenderWindow()->Render();

    isContourMaskInited = true;
    ProcessInfo.isShowContourMask = true;
//#endif
}

void MainDialog::InitRegionGrowMask()
{
//#if 0
    _imageViewer->GetRenderer()->RemoveActor(regionGrowingMaskActor);
    //初始化颜色表
    regionGrowinglut->SetNumberOfTableValues(2);
    regionGrowinglut->SetRange(0, 1);
    regionGrowinglut->SetTableValue(1, 0.0, 1.0, 0.0, 0.5);
    regionGrowinglut->SetTableValue(0, 0.0, 0.0, 0.0, 0.0);
    regionGrowinglut->Build();

    regionGrowingImageImport->SetDataSpacing(DicomInfo.spacing_x, DicomInfo.spacing_y, DicomInfo.spacing_z);
    double* ori = _inImageData->GetOrigin();
    ori[2] = _imageViewer->GetSlice()*DicomInfo.spacing_z;
    regionGrowingImageImport->SetDataOrigin(ori);
    regionGrowingImageImport->SetWholeExtent(0, DicomInfo.width-1, 0, DicomInfo.height-1, 0, 0);

    regionGrowingImageImport->SetDataExtentToWholeExtent();
    regionGrowingImageImport->SetDataScalarType(VTK_UNSIGNED_CHAR);
    regionGrowingImageImport->SetNumberOfScalarComponents(1);
    void* p = RegionGrowImageDataArray+_imageViewer->GetSlice()*DicomInfo.width*DicomInfo.height;
    regionGrowingImageImport->SetImportVoidPointer(p);
    regionGrowingImageImport->Update();

    regionGrowingMapper->SetLookupTable(regionGrowinglut);
    regionGrowingMapper->PassAlphaToOutputOn(); //?
    regionGrowingMapper->SetInputConnection(0,regionGrowingImageImport->GetOutputPort());
    regionGrowingMapper->Update();

    regionGrowingMaskActor->GetMapper()->SetInputConnection(0,regionGrowingMapper->GetOutputPort());

    _imageViewer->GetRenderer()->AddActor(regionGrowingMaskActor);
    _imageViewer->GetRenderWindow()->Render();

    ProcessInfo.isRegionGrow=true;
    ProcessInfo.isShowRegionGrowMask=true;
//#endif
}

void MainDialog::SurfaceRenderingProcess()
{
//    if (RegionGrowImageArray_NewOri!=NULL)
//    {
//        delete []RegionGrowImageArray_NewOri;
//        RegionGrowImageArray_NewOri = NULL;
//    }

    short* regionGrowDataTemp;
    int dims[3];
    double spacing[3];
    if(DicomPreInfo.bNeedRotate)
    {
        // regionGrowDataTemp.resize(DicomPreInfo.m_iVoxelSize);

        regionGrowDataTemp = new short[DicomPreInfo.m_iVoxelSize];

        //RegionGrowImageArray_NewOri= new uchar[DicomPreInfo.m_iVoxelSize];
        int rcn[3];

        for(int r=0;r<DicomInfo.width;++r)
        {
            for(int c=0;c<DicomInfo.height;++c)
            {
                for(int n=0;n<DicomInfo.depth;++n)
                {
                    rcn[0]=r;
                    rcn[1]=c;
                    rcn[2]=n;
                    int x,y,z;
                    if(DicomPreInfo.xyzflip[0])
                        x=DicomPreInfo.width-rcn[DicomPreInfo.xyzos[0]]-1;
                    else
                        x=rcn[DicomPreInfo.xyzos[0]];
                    if(DicomPreInfo.xyzflip[1])
                        y=DicomPreInfo.height-rcn[DicomPreInfo.xyzos[1]]-1;
                    else
                        y=rcn[DicomPreInfo.xyzos[1]];
                    if(DicomPreInfo.xyzflip[2])
                        z=DicomPreInfo.depth-rcn[DicomPreInfo.xyzos[2]]-1;
                    else
                        z=rcn[DicomPreInfo.xyzos[2]];
                    regionGrowDataTemp[x+y*DicomPreInfo.width+z*DicomPreInfo.width*DicomPreInfo.height]=(short)ContoursMaskArray[r+c*DicomInfo.width+n*DicomInfo.width*DicomInfo.height];
                    // RegionGrowImageArray_NewOri[x+y*DicomPreInfo.width+z*DicomPreInfo.width*DicomPreInfo.height]=ContoursMaskArray[r+c*DicomInfo.width+n*DicomInfo.width*DicomInfo.height];
                }
            }
        }

        dims[0]=DicomPreInfo.width;
        dims[1]=DicomPreInfo.height;
        dims[2]=DicomPreInfo.depth;
        spacing[0]=DicomPreInfo.spacing_x;
        spacing[1]=DicomPreInfo.spacing_y;
        spacing[2]=DicomPreInfo.spacing_z;

    }
    else
    {

#pragma mark 可能有问题
        //RegionGrowImageArray_NewOri = new uchar[DicomInfo.m_iVoxelSize];
        //memcpy(RegionGrowImageArray_NewOri,ContoursMaskArray,DicomInfo.m_iVoxelSize);
        // regionGrowDataTemp.resize(DicomInfo.m_iVoxelSize);
        regionGrowDataTemp = new short[DicomPreInfo.m_iVoxelSize];
        for(int i=0;i<DicomInfo.m_iVoxelSize;i++)
        {
            regionGrowDataTemp[i]=(short)ContoursMaskArray[i];
        }
        dims[0]=DicomInfo.width;
        dims[1]=DicomInfo.height;
        dims[2]=DicomInfo.depth;
        spacing[0]=DicomInfo.spacing_x;
        spacing[1]=DicomInfo.spacing_y;
        spacing[2]=DicomInfo.spacing_z;
    }

    Hap.SurfaceRenderingProcess(regionGrowDataTemp,dims[0],dims[1],dims[2],
            spacing[0],spacing[1],spacing[2]);

    Update3DModel();
    delete []regionGrowDataTemp;
    regionGrowDataTemp = NULL;

//    regionGrowingImageImport_NewOri->SetDataSpacing(spacing);
//    regionGrowingImageImport_NewOri->SetDataOrigin(_inImageData->GetOrigin());
//    regionGrowingImageImport_NewOri->SetWholeExtent(dims);
//    regionGrowingImageImport_NewOri->SetDataExtentToWholeExtent();
//    regionGrowingImageImport_NewOri->SetDataScalarTypeToUnsignedChar();
//    regionGrowingImageImport_NewOri->SetNumberOfScalarComponents(1);
//    regionGrowingImageImport_NewOri->SetImportVoidPointer(RegionGrowImageArray_NewOri);
//    regionGrowingImageImport_NewOri->Update();
}

void MainDialog::Update3DModel()
{
    vtkSmartPointer<vtkSTLReader> stlreader=vtkSmartPointer<vtkSTLReader>::New();
    stlreader->ReleaseDataFlagOn();
    QDir dir;
    QString path=dir.currentPath();
    path.append("/Tumor3d.stl");
    qDebug()<<"tumor path is "<<path;
    stlreader->SetFileName(path.toStdString().c_str());
    stlreader->Update();
    _STLPolyData=stlreader->GetOutput();
    vtkSmartPointer<vtkPolyDataMapper> mapper =
            vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->ReleaseDataFlagOn();
    mapper->SetInputData(stlreader->GetOutput());

    _STLActor->SetMapper(mapper);

    _emptySTLRender->RemoveActor(_STLActor);
    _emptySTLRender->AddActor(_STLActor);
    _emptySTLRender->SetBackground(.3, .6, .3); // Background color green
    _STLOrientationWidget->SetOutlineColor(0.9300, 0.5700, 0.1300);
    _STLOrientationWidget->SetOrientationMarker( STLCubeAxes );
    _STLOrientationWidget->SetInteractor(_qvtkWidget_STLView->GetRenderWindow()->GetInteractor());
    _STLOrientationWidget->SetViewport( 0.0, 0.0, 0.2, 0.2 );
    _STLOrientationWidget->SetEnabled( 1 );
    _STLOrientationWidget->InteractiveOn();

    _emptySTLRender->ResetCamera();
    _qvtkWidget_STLView->GetRenderWindow()->Render();

    //计算肌瘤参数
    double *bound;
    bound =_STLPolyData->GetBounds();
    double size[3];
    size[0]=bound[1]-bound[0];
    size[1]=bound[3]-bound[2];
    size[2]=bound[5]-bound[4];
    vtkSmartPointer<vtkTriangleFilter> triF= vtkSmartPointer<vtkTriangleFilter>::New();
    triF->SetReleaseDataFlag(1);
    triF->SetInputData(stlreader->GetOutput());
    triF->Update();
    vtkSmartPointer<vtkMassProperties> polygonProperties = vtkSmartPointer<vtkMassProperties>::New();
    polygonProperties->SetReleaseDataFlag(1);
    polygonProperties->SetInputData(triF->GetOutput());
    polygonProperties->Update();
    double volume = polygonProperties->GetVolume();
    UpdateTumorParam(size[0],size[1],size[2],volume);
}

//获取面切割轮廓图
void MainDialog::on_pushButton_GetContours_clicked()
{

    emit sendSearchRadius(m_iSearchRadius);
    // GetContoursNew();
    ui->pushButton_GetContours->setEnabled(false);
    ui->pushButton_GetContours->setStyleSheet(NOT_WORK);

    QCoreApplication::processEvents();

    GetSlices();
    GetContours();

    ui->pushButton_GetContours->setEnabled(true);
    ui->pushButton_GetContours->setStyleSheet(SELECTED);

    return;
}

void MainDialog::InitLimitedRegionMask()
{
    LimitedRegionSource->SetGeneratePolygon(0);
    LimitedRegionSource->SetGeneratePolyline(1);
    LimitedRegionSource->SetNumberOfSides(50);
    LimitedRegionSource->SetRadius(0.0);
    LimitedRegionSource->SetCenter(0, 0, 0);
    LimitedRegionSource->Update();

    LimitedRegionRect_Mapper->RemoveAllInputs();
    LimitedRegionRect_Mapper->RemoveAllInputConnections(0);
    LimitedRegionRect_Mapper->SetInputConnection(0,LimitedRegionSource->GetOutputPort());

    LimitedRegionRect_Actor->SetMapper( LimitedRegionRect_Mapper );
    LimitedRegionRect_Actor->GetProperty()->SetColor(0,1,1);
    LimitedRegionRect_Actor->GetProperty()->SetPointSize(1.5);

    _imageViewer->GetRenderer()->AddActor2D(LimitedRegionRect_Actor);
    _imageViewer->Render();

}

void MainDialog:: UpdateLimitedRegion(int x2,int y2,int z2,bool isclear)
{
    double dPtmin[3]={0,0,0};
    double dPtmax[3]={0,0,0};
    if(!isclear)
    {
        int px1=ProcessInfo.LimitPoint1_x<=x2?ProcessInfo.LimitPoint1_x:x2;
        int px2=ProcessInfo.LimitPoint1_x>=x2?ProcessInfo.LimitPoint1_x:x2;
        int py1=ProcessInfo.LimitPoint1_y<=y2?ProcessInfo.LimitPoint1_y:y2;
        int py2=ProcessInfo.LimitPoint1_y>=y2?ProcessInfo.LimitPoint1_y:y2;
        double wxmin=px1*DicomInfo.spacing_x,wxmax=px2*DicomInfo.spacing_x,wymin=py1*DicomInfo.spacing_y,wymax=py2*DicomInfo.spacing_y;
        vtkInteractorObserver::ComputeWorldToDisplay(_imageViewer->GetRenderer(),wxmin,wymin,0,dPtmin);
        vtkInteractorObserver::ComputeWorldToDisplay(_imageViewer->GetRenderer(),wxmax,wymax,0,dPtmax);
        ProcessInfo.isShowLimitedRegion=true;
    }
    else
    {
        ProcessInfo.isShowLimitedRegion=false;
    }
    //********修改
    ProcessInfo.LimitRegion_center[0]=(dPtmax[0]+dPtmin[0])/2.0;
    ProcessInfo.LimitRegion_center[1]=(dPtmax[1]+dPtmin[1])/2.0;
    ProcessInfo.LimitRegion_r=(sqrt((dPtmax[0]-dPtmin[0])*(dPtmax[0]-dPtmin[0])+(dPtmax[1]-dPtmin[1])*(dPtmax[1]-dPtmin[1])))/2.0;
    LimitedRegionSource->SetRadius(ProcessInfo.LimitRegion_r);
    LimitedRegionSource->SetCenter(ProcessInfo.LimitRegion_center[0], ProcessInfo.LimitRegion_center[1], 0);
    LimitedRegionSource->Update();
    //************
    LimitedRegionRect_Mapper->Update();
    if(_imageViewer->GetSlice()>=ProcessInfo.LimitRegion_z1&&_imageViewer->GetSlice()<=ProcessInfo.LimitRegion_z2)
    {
        LimitedRegionRect_Actor->SetVisibility(1);
    }
    else
        LimitedRegionRect_Actor->SetVisibility(0);
    _imageViewer->Render();
}

void MainDialog::EraseRegion(int x,int y,int z,int size,uchar value)
{
    if(size==0)
    {
        //qDebug()<<"RegionGrow array size:"<<RegionGrowImageDataArray.size()<<"erase pos:"<<(x+y*DicomInfo.width+z*DicomInfo.width*DicomInfo.height);
        int pos=x+y*DicomInfo.width+z*DicomInfo.width*DicomInfo.height;
        if(pos<0||pos>=DicomInfo.m_iVoxelSize){
            qDebug()<<"擦除填充操作失败 xyz:"<<x<<y<<z<<" value:"
                   <<value<<" Pos:"<<pos<<" array size:"<<DicomInfo.m_iVoxelSize;
            return;
        }
        RegionGrowImageDataArray[pos]=value;
    }
    else
    {
        int exmin=x-size;
        int exmax=x+size;
        int eymin=y-size;
        int eymax=y+size;
        for(int ix=exmin;ix<=exmax;ix++)
        {
            for(int iy=eymin;iy<=eymax;iy++)
            {
                if ((ix-x)*(ix-x)+(iy-y)*(iy-y)<size*size)
                {
                    int pos=ix+iy*DicomInfo.width+z*DicomInfo.width*DicomInfo.height;
                    if(pos<0||pos>=DicomInfo.m_iVoxelSize){
                        qDebug()<<"擦除填充操作失败 xyz:"<<x<<y<<z<<" value:"
                               <<value<<" Pos:"<<pos<<" array size:"<<DicomInfo.m_iVoxelSize;
                        return;
                    }
                    //qDebug()<<"RegionGrow array size:"<<RegionGrowImageDataArray.size()<<"erase pos:"<<(ix+iy*DicomInfo.width+z*DicomInfo.width*DicomInfo.height);
                    RegionGrowImageDataArray[pos]=value;
                }

            }
        }
    }
    regionGrowingMapper->RemoveAllInputConnections(0);
    regionGrowingMapper->SetInputConnection(0,regionGrowingImageImport->GetOutputPort());
    regionGrowingMapper->Update();
    _imageViewer->GetRenderWindow()->Render();
}

void MainDialog::on_pushButton_GetSlices_clicked()
{
    if(!DicomInfo.isSourceRead)
    {
        codec = QTextCodec::codecForName("GBK");
        QString info1Str =  codec->toUnicode("提示");
        QString info2Str =  codec->toUnicode("尚未导入数据，无法生成对应图像");
        QMessageBox::warning(NULL,info1Str,info2Str);
        return;
    }

//    ui->pushButton_GetSlices->setEnabled(false);
//    ui->pushButton_GetSlices->setStyleSheet(NOT_WORK);

    QCoreApplication::processEvents();
    GetSlices();

//    ui->pushButton_GetSlices->setEnabled(true);
//    ui->pushButton_GetSlices->setStyleSheet(SELECTED);
}

//void MainDialog::on_spinBox_Threshold_Min_valueChanged(int arg1)
//{
//    ui->horizontalSlider_Threshold_Min->setValue(arg1);
//}

//void MainDialog::on_spinBox_Threshold_Max_valueChanged(int arg1)
//{
//    ui->horizontalSlider_Threshold_Max->setValue(arg1);
//}

//void MainDialog::on_horizontalSlider_Threshold_Min_valueChanged(int value)
//{
//    if(ui->horizontalSlider_Threshold_Min->value()>=ui->horizontalSlider_Threshold_Max->value())
//    {
//        ui->horizontalSlider_Threshold_Min->setValue(ui->horizontalSlider_Threshold_Max->value());
//    }
//    ui->spinBox_Threshold_Min->setValue(ui->horizontalSlider_Threshold_Min->value());

////    int slice = _imageViewer->GetSlice();
////    _imageViewer->SetSlice(slice-1);
////    _imageViewer->SetSlice(slice);

//    if(ProcessInfo.isCanThreshold)
//    {
//        emit on_ThresholdValueChanged(ui->horizontalSlider_Threshold_Min->value(),
//                                      ui->horizontalSlider_Threshold_Max->value());
//    }
//}

//void MainDialog::on_horizontalSlider_Threshold_Max_valueChanged(int value)
//{
//    if(ui->horizontalSlider_Threshold_Max->value()<=ui->horizontalSlider_Threshold_Min->value())
//    {
//        ui->horizontalSlider_Threshold_Max->setValue(ui->horizontalSlider_Threshold_Min->value());
//    }
//    ui->spinBox_Threshold_Max->setValue(ui->horizontalSlider_Threshold_Max->value());

////    int slice = _imageViewer->GetSlice();
////    _imageViewer->SetSlice(slice-1);
////    _imageViewer->SetSlice(slice);

//    if(ProcessInfo.isCanThreshold)
//    {
//        emit on_ThresholdValueChanged(ui->horizontalSlider_Threshold_Min->value(),
//                                      ui->horizontalSlider_Threshold_Max->value());
//    }
//}

//void MainDialog::UpdateCtlValueRange(int min,int max)
//{
//    ui->horizontalSlider_Threshold_Max->setMinimum(min);
//    ui->horizontalSlider_Threshold_Max->setMaximum(max);
//    ui->spinBox_Threshold_Max->setMaximum(max);
//    ui->spinBox_Threshold_Max->setMinimum(min);
//    ui->horizontalSlider_Threshold_Min->setMinimum(min);
//    ui->horizontalSlider_Threshold_Min->setMaximum(max);
//    ui->spinBox_Threshold_Min->setMaximum(max);
//    ui->spinBox_Threshold_Min->setMinimum(min);
//    ui->spinBox_Threshold_Min->setValue(min);
//    ui->spinBox_Threshold_Max->setValue(min);
//}

void MainDialog::on_pushButton_PickSeedPoint_clicked()
{
    LimitedRegionOperationSwitch(SetSeedPoint);
}

void MainDialog::SetSeedPos(int x,int y,int z)
{
    // ui->label_SeedPos->setText(QString("(%1,%2,%3)").arg(x).arg(y).arg(z));
}

void MainDialog::on_pushButton_Grow_clicked()
{
    if(!DicomInfo.isSourceRead)
    {
        codec = QTextCodec::codecForName("GBK");
        QString info1Str =  codec->toUnicode("提示");
        QString info2Str =  codec->toUnicode("无可生成数据");
        QMessageBox::warning(NULL,info1Str,info2Str);
        return;
    }
    if(ProcessInfo.FecthTumorMethod==FetchThreshold)
    {
        if(!ProcessInfo.ValidThreshold())
        {
            codec = QTextCodec::codecForName("GBK");
            QString info1Str =  codec->toUnicode("提示");
            QString info2Str =  codec->toUnicode("未阈值分割或种子点未设置");
            QMessageBox::warning(NULL,info1Str,info2Str);
            return;
        }
        SeedRegionGrowingProcess(false);

        if (!ui->pushButton_output_mask->isEnabled())
        {
            ui->pushButton_output_mask->setEnabled(true);
            ui->pushButton_output_mask->setStyleSheet(SELECTED);
        }
    }
    else if(ProcessInfo.FecthTumorMethod==FetchManualDraw)
    {
        ManualDrawProcess(false);
    }
}

void MainDialog::on_toolButton_SetRegionXY_clicked()
{
    if(RegionGrowImageDataArray==NULL)
    {
        codec = QTextCodec::codecForName("GBK");
        QString info1Str =  codec->toUnicode("提示");
        QString info2Str =  codec->toUnicode("没有可标定的增长区域");
        QMessageBox::warning(NULL,info1Str,info2Str);
        return;
    }
    LimitedRegionOperationSwitch(SetLimitedRegion);
}

//void MainDialog::SetLimitPoint1(int p1x,int p1y)
//{
//    ui->label_LimitPoint1->setText(QString("(%1,%2)").arg(p1x).arg(p1y));
//}

//void MainDialog::SetLimitPoint2(int p2x,int p2y,int w,int h)
//{
//    ui->label_LimitPoint2->setText(QString("(%1,%2)").arg(p2x).arg(p2y));
//    ui->label__LimitWH->setText(QString("%1*%2").arg(w).arg(h));
//}

void MainDialog::UpdateGrowRegionPosInfo(short value,int posx,int posy,int posz)
{
    ui->label_MouseRegionGrowValue->setText(QString("%1").arg(value));
    //    ui->label_MouseRegionGrowInfo->setText(QString("X:%1  Y:%2  Z:%3").arg(posx).arg(posy).arg(posz));
}

void MainDialog::on_horizontalSlider_EraseSize_valueChanged(int value)
{
    ProcessInfo.EraseSize=value;
    //ui->label_EraseSize->setText(QString("%1").arg(ProcessInfo.EraseSize));
}

//void MainDialog::SetLimitRegionZ(int z1,int z2)
//{
//    ui->spinBox_Limit_z1->setValue(z1);
//    ui->spinBox_Limit_z2->setValue(z2);
//    ui->label__LimitZ->setText(QString("%1-%2").arg(z1).arg(z2));
//}

void MainDialog::on_spinBox_Limit_z1_valueChanged(int arg1)
{
    ProcessInfo.LimitRegion_z1=arg1;
    //ui->label__LimitZ->setText(QString("%1-%2").arg(ProcessInfo.LimitRegion_z1).arg(ProcessInfo.LimitRegion_z2));
}

void MainDialog::on_spinBox_Limit_z2_valueChanged(int arg1)
{
    ProcessInfo.LimitRegion_z2=arg1;
    //ui->label__LimitZ->setText(QString("%1-%2").arg(ProcessInfo.LimitRegion_z1).arg(ProcessInfo.LimitRegion_z2));
}

void MainDialog::on_toolButton_Erase_clicked()
{
    if(RegionGrowImageDataArray==NULL)
    {
        codec = QTextCodec::codecForName("GBK");
        QString info1Str =  codec->toUnicode("提示");
        QString info2Str =  codec->toUnicode("没有可擦除的增长区域");
        QMessageBox::warning(NULL,info1Str,info2Str);
        return;
    }
    LimitedRegionOperationSwitch(Erase);
}

void MainDialog::on_pushButton_ReGrow_clicked()
{
    if(RegionGrowImageDataArray==NULL)
    {
        codec = QTextCodec::codecForName("GBK");
        QString info1Str =  codec->toUnicode("提示");
        QString info2Str =  codec->toUnicode("没有可重新增长的区域");
        QMessageBox::warning(NULL,info1Str,info2Str);
        return;
    }
    SeedRegionReGrowingProcess();
}

void MainDialog::on_pushButton_SurfaceRendering_2_clicked()
{
    ui->pushButton_GetContours->setEnabled(true);
    ui->pushButton_GetContours->setStyleSheet(SELECTED);
//    ui->pushButton_GetSlices->setEnabled(true);
//    ui->pushButton_GetSlices->setStyleSheet(SELECTED);

    m_bIsPreProcessed=false;


    if(ContoursMaskArray==NULL)
    {
        codec = QTextCodec::codecForName("GBK");
        QString info1Str =  codec->toUnicode("提示");
        QString info2Str =  codec->toUnicode("肌瘤掩模为空，无法三维重建");
        QMessageBox::warning(NULL,info1Str,info2Str);
        return;
    }
    ui->pushButton_SurfaceRendering_2->setEnabled(false);
    ui->pushButton_SurfaceRendering_2->setStyleSheet(NOT_WORK);

    QCoreApplication::processEvents();
    SurfaceRenderingProcess();


    ui->pushButton_SurfaceRendering_2->setEnabled(true);
    ui->pushButton_SurfaceRendering_2->setStyleSheet(SELECTED);
}

QImage MainDialog::vtkImageDataToResizeQImage(vtkSmartPointer<vtkImageData> data,
                                              int width,int height)
{
    // 沿着y轴翻转,vtk图像方向和qt,opencv方向上下颠倒
    _flip->SetInputData(data);
    _flip->SetFilteredAxis(1);
    _flip->Update();

    data = _flip->GetOutput();

    int dim[3];
    data->GetDimensions(dim);
    int imgType=CV_8UC1;
    if(data->GetNumberOfScalarComponents()==1)
        imgType=CV_8UC1;
    if(data->GetNumberOfScalarComponents()==3)
        imgType=CV_8UC3;
    if(data->GetNumberOfScalarComponents()==4)
        imgType=CV_8UC4;

    cv::Mat srcMat(dim[1],dim[0],imgType,data->GetScalarPointer());

    //    double spacing[3];
    //    data->GetSpacing(spacing);
    //    cv::Size dsize;
    //    dsize.width = dim[0] * spacing[0];
    //    dsize.height = dim[1] * spacing[1];

    //    cv::Mat temp = srcMat;
    //    cv::resize(srcMat, temp, dsize);

    cv::Mat mat(width,height,imgType);
    cv::resize(srcMat, mat, mat.size(), 0.5, 0.5,
               cv::INTER_AREA);

    if(mat.type() == CV_8UC1)
    {
        QImage image(mat.cols, mat.rows, QImage::Format_Indexed8);
        // Set the color table (used to translate colour indexes to qRgb values)
        image.setColorCount(256);
        for(int i = 0; i < 256; i++)
        {
            image.setColor(i, qRgb(i, i, i));
        }
        // Copy input Mat
        uchar *pSrc = mat.data;
        for(int row = 0; row < mat.rows; row ++)
        {
            uchar *pDest = image.scanLine(row);
            memcpy(pDest, pSrc, mat.cols);
            pSrc += mat.step;
        }
        return image;
    }
    // 8-bits unsigned, NO. OF CHANNELS = 3
    else if(mat.type() == CV_8UC3)
    {
        // Copy input Mat
        const uchar *pSrc = (const uchar*)mat.data;
        // Create QImage with same dimensions as input Mat
        QImage image(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);
        return image.rgbSwapped();
    }
    else if(mat.type() == CV_8UC4)
    {
        // Copy input Mat
        const uchar *pSrc = (const uchar*)mat.data;
        // Create QImage with same dimensions as input Mat
        QImage image(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_ARGB32);
        return image.copy();
    }
    else
    {
        qDebug() << "ERROR: Mat could not be converted to QImage.";
        return QImage();
    }

}

QImage MainDialog::vtkImageDataToQImage(vtkSmartPointer<vtkImageData> data)
{
    // 沿着y轴翻转,vtk图像方向和qt,opencv方向上下颠倒
    _flip->SetInputData(data);
    _flip->SetFilteredAxis(1);
    _flip->Update();

    data = _flip->GetOutput();

    int dim[3];
    data->GetDimensions(dim);
    int imgType=CV_8UC1;
    if(data->GetNumberOfScalarComponents()==1)
        imgType=CV_8UC1;
    if(data->GetNumberOfScalarComponents()==3)
        imgType=CV_8UC3;
    if(data->GetNumberOfScalarComponents()==4)
        imgType=CV_8UC4;

    cv::Mat srcMat(dim[1],dim[0],imgType,data->GetScalarPointer());
    //    imshow("image data",srcMat);

    double spacing[3];
    data->GetSpacing(spacing);
    cv::Size dsize;
    dsize.width = dim[0] * spacing[0];
    dsize.height = dim[1] * spacing[1];

    cv::Mat mat = srcMat;
    cv::resize(srcMat, mat, dsize);

    if(mat.type() == CV_8UC1)
    {
        QImage image(mat.cols, mat.rows, QImage::Format_Indexed8);
        // Set the color table (used to translate colour indexes to qRgb values)
        image.setColorCount(256);
        for(int i = 0; i < 256; i++)
        {
            image.setColor(i, qRgb(i, i, i));
        }
        // Copy input Mat
        uchar *pSrc = mat.data;
        for(int row = 0; row < mat.rows; row ++)
        {
            uchar *pDest = image.scanLine(row);
            memcpy(pDest, pSrc, mat.cols);
            pSrc += mat.step;
        }
        return image;
    }
    // 8-bits unsigned, NO. OF CHANNELS = 3
    else if(mat.type() == CV_8UC3)
    {
        // Copy input Mat
        const uchar *pSrc = (const uchar*)mat.data;
        // Create QImage with same dimensions as input Mat
        QImage image(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);
        return image.rgbSwapped();
    }
    else if(mat.type() == CV_8UC4)
    {
        // Copy input Mat
        const uchar *pSrc = (const uchar*)mat.data;
        // Create QImage with same dimensions as input Mat
        QImage image(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_ARGB32);
        return image.copy();
    }
    else
    {
        qDebug() << "ERROR: Mat could not be converted to QImage.";
        return QImage();
    }

}

void MainDialog::InitRTRect()
{
    RTRect_Source->SetGeneratePolygon(0);
    RTRect_Source->SetGeneratePolyline(1);
    RTRect_Source->SetNumberOfSides(50);
    RTRect_Source->SetRadius(0.5);
    RTRect_Source->SetCenter(0, 0, 0);
    RTRect_Source->Update();
    RTRect_Mapper->RemoveAllInputs();
    RTRect_Mapper->SetInputConnection(RTRect_Source->GetOutputPort());

    RTRect_Actor->SetMapper( RTRect_Mapper );
    RTRect_Actor->GetProperty()->SetColor(1,1,0);
    RTRect_Actor->GetProperty()->SetLineWidth(1.5);

    _imageViewer->GetRenderer()->AddActor2D(RTRect_Actor);
    _imageViewer->Render();
}

void  MainDialog::UpdateRTRect(int x2,int y2,int z2,bool hiden)
{
    int px1,px2,py1,py2,nz2;
    double poly_r=0;
    double poly_center[3]={0,0,0};
    if(hiden)
    {
        px1=px2=py1=py2=0;
        RTRect_Source->SetRadius(0);
        RTRect_Source->SetCenter(0,0,0);
        RTRect_Source->Update();
    }
    else if(ProcessInfo.isSettingLimitedRegion1||ProcessInfo.isSettingLimitedRegion2)
    {

        px1=rect_x1<=x2?rect_x1:x2;
        px2=rect_x1>=x2?rect_x1:x2;
        py1=rect_y1<=y2?rect_y1:y2;
        py2=rect_y1>=y2?rect_y1:y2;
        double lx=(double)px2-(double)px1;
        double ly=(double)py2-(double)py1;
        poly_r=(sqrt(lx*lx+ly*ly))/2.0;
        poly_center[0]=(px2+px1)/2;
        poly_center[1]=(py2+py1)/2;
        poly_center[2]=z2;
    }
    else if(ProcessInfo.isErasing||ProcessInfo.isFilling)
    {
        int esize=0;
        if(ProcessInfo.isErasing)
            esize=ProcessInfo.EraseSize;
        else if(ProcessInfo.isFilling)
            esize=ProcessInfo.FillSize;
        esize=esize*XRatio;

        poly_r=esize;
        poly_center[0]=x2;
        poly_center[1]=y2;
        poly_center[2]=z2;

    }
    RTRect_Source->SetNumberOfSides(50);
    RTRect_Source->SetRadius(poly_r);
    RTRect_Source->SetCenter(poly_center[0], poly_center[1], poly_center[2]);
    RTRect_Source->Update();
    RTRect_Mapper->Update();
    _imageViewer->Render();
}

void MainDialog::GetRatioWorldToDispaly(double& xr,double&yr)
{
    double wxmin=1.0,wxmax=1.0+DicomInfo.spacing_x,wymin=1.0,wymax=1.0+DicomInfo.spacing_y;
    double dPtmin[3];
    vtkInteractorObserver::ComputeWorldToDisplay(_imageViewer->GetRenderer(),wxmin,wymin,0,dPtmin);
    double dPtmax[3];
    vtkInteractorObserver::ComputeWorldToDisplay(_imageViewer->GetRenderer(),wxmax,wymax,0,dPtmax);
    xr=dPtmax[0]-dPtmin[0];
    yr=dPtmax[1]-dPtmin[1];
}

void MainDialog::on_toolButton_Fill_clicked()
{
    if(RegionGrowImageDataArray==NULL)
    {
        codec = QTextCodec::codecForName("GBK");
        QString info1Str =  codec->toUnicode("提示");
        QString info2Str =  codec->toUnicode("没有可填充的增长区域");
        QMessageBox::warning(NULL,info1Str,info2Str);
        return;
    }
    LimitedRegionOperationSwitch(Fill);
}

void MainDialog::on_horizontalSlider_FillSize_valueChanged(int value)
{
    ProcessInfo.FillSize=value;
    //ui->label_FillSize->setText(QString("%1").arg(ProcessInfo.FillSize));
}

void MainDialog::on_toolButton_ClearLimitRegion_clicked()
{
    if(!DicomInfo.isSourceRead)
        return;
    UpdateLimitedRegion(0,0,0,true);
    ProcessInfo.ClearLimitRegion();
//    SetLimitRegionZ(0,0);
//    SetLimitPoint1(-1,-1);
//    SetLimitPoint2(-1,-1,0,0);
}

void MainDialog::LimitedRegionOperationSwitch(Operation operation)
{
    if(!DicomInfo.isSourceRead)
        return;

    bool isOperating=false;
    if(operation==SetSeedPoint&&!ProcessInfo.isSetingSeedPoint)
    {
        //ui->pushButton_PickSeedPoint->setStyleSheet(STYLE_OPERATING);
        ProcessInfo.isSetingSeedPoint=true;
        isOperating=true;
    }
    else if(ProcessInfo.isSetingSeedPoint)
    {
        //ui->pushButton_PickSeedPoint->setStyleSheet(SELECTED);
        ProcessInfo.isSetingSeedPoint=false;
    }

    if(operation==SetLimitedRegion&&!ProcessInfo.isSettingLimitedRegion1&&!ProcessInfo.isSettingLimitedRegion2)
    {
        //ui->toolButton_SetRegionXY->setStyleSheet(STYLE_OPERATING);
        GetRatioWorldToDispaly(XRatio,YRatio);
        ProcessInfo.isSettingLimitedRegion1=true;
        ProcessInfo.isSettingLimitedRegion2=false;
        isOperating=true;
    }
    else if(ProcessInfo.isSettingLimitedRegion1||ProcessInfo.isSettingLimitedRegion2)
    {
        //ui->toolButton_SetRegionXY->setStyleSheet(SELECTED);
        ProcessInfo.isSettingLimitedRegion1=false;
        ProcessInfo.isSettingLimitedRegion2=false;
    }

    if(operation==Erase&&!ProcessInfo.isErasing)
    {
        //ui->toolButton_Erase->setStyleSheet(STYLE_OPERATING);
        GetRatioWorldToDispaly(XRatio,YRatio);
        ProcessInfo.isErasing=true;
        isOperating=true;
    }
    else if(ProcessInfo.isErasing)
    {
        //ui->toolButton_Erase->setStyleSheet(SELECTED);
        UpdateRTRect(0,0,0,true);
        ProcessInfo.isErasing=false;
    }

    if(operation==Fill&&!ProcessInfo.isFilling)
    {
        //ui->toolButton_Fill->setStyleSheet(STYLE_OPERATING);
        GetRatioWorldToDispaly(XRatio,YRatio);
        ProcessInfo.isFilling=true;
        isOperating=true;
    }
    else if(ProcessInfo.isFilling)
    {
        //ui->toolButton_Fill->setStyleSheet(SELECTED);
        UpdateRTRect(0,0,0,true);
        ProcessInfo.isFilling=false;
    }

    if(operation==ManualDraw&&!ProcessInfo.isManualDrawing)
    {
        ui->pushButton_ManualDrawOn->setStyleSheet(STYLE_OPERATING);
        ProcessInfo.isManualDrawing=true;
        isOperating=true;
        myContourWidget->ProcessEventsOn();
    }
    else if(ProcessInfo.isManualDrawing)
    {
        ui->pushButton_ManualDrawOn->setStyleSheet(SELECTED);
        ProcessInfo.isManualDrawing=false;
        myContourWidget->ProcessEventsOff();
    }

    if(isOperating)
    {
        setCursor(Qt::CrossCursor);
    }
    else
    {
        setCursor(Qt::ArrowCursor);
    }
}

void MainDialog::closeEvent(QCloseEvent *event)
{
    emit MainDialogClose();
}

void MainDialog::on_qtVtkWiggetMouseRightButtonClicked(vtkObject * obj, unsigned long, void * client_data, void *, vtkCommand * command)
{
//    if(ProcessInfo.isThreshold)
//    {
//        _ShowThresholdMask->setEnabled(true);
//    }
//    else
//    {
//        _ShowThresholdMask->setEnabled(false);
//    }
//    if(ProcessInfo.isRegionGrow)
//    {
//        _ShowRegionGrowMask->setEnabled(true);
//    }
//    else
//    {
//        _ShowRegionGrowMask->setEnabled(false);
//    }

    if (ProcessInfo.isContourCreated)
    {
        _ShowContourMask->setEnabled(true);
    }
    else
    {
        _ShowContourMask->setEnabled(false);
    }

    vtkRenderWindowInteractor* iren = vtkRenderWindowInteractor::SafeDownCast(obj);
    command->AbortFlagOn();
    int* sz = iren->GetSize();
    int* position = iren->GetEventPosition();
    QPoint pt = QPoint(position[0], sz[1]-position[1]);
    QPoint global_pt = _qvtkWidget->parentWidget()->mapToGlobal(pt);
    _qvtkWIdgetContextMenu->exec(global_pt);
}

void MainDialog::InitqvtkWIdgetContextMenu()
{
    _qvtkWIdgetContextMenu= new QMenu();

    connect(_qvtkWIdgetContextMenu,SIGNAL(aboutToHide()),this,SLOT(ContextMenu_Hide()));

//    if (_ShowThresholdMask==NULL)
//    {
//        _ShowThresholdMask = _qvtkWIdgetContextMenu->addAction("隐藏阈值分割Mask");
//        connect(_ShowThresholdMask,SIGNAL(triggered(bool)),this,SLOT(ContextAction_ThresholdMask()));
//    }
//    if (_ShowRegionGrowMask==NULL)
//    {
//        _ShowRegionGrowMask = _qvtkWIdgetContextMenu->addAction("隐藏区域增长Mask");
//        connect(_ShowRegionGrowMask,SIGNAL(triggered(bool)),this,SLOT(ContextAction_RegionGrowMask()));
//    }

    if (_ShowContourMask==NULL)
    {

        codec = QTextCodec::codecForName("GBK");
        QString infoStr =  codec->toUnicode("隐藏肌瘤Mask");
        _ShowContourMask = _qvtkWIdgetContextMenu->addAction(infoStr);
        connect(_ShowContourMask,SIGNAL(triggered(bool)),this,SLOT(ContexAction_ContourMask()));
    }
}

void MainDialog::ContexAction_ContourMask()
{
    if(ProcessInfo.isShowContourMask)
    {
        codec = QTextCodec::codecForName("GBK");
        QString infoStr =  codec->toUnicode("显示肌瘤Mask");
        RemoveMaskActor(2);
        _ShowContourMask->setText(infoStr);
    }
    else
    {
        codec = QTextCodec::codecForName("GBK");
        QString infoStr =  codec->toUnicode("隐藏肌瘤Mask");
        AddMaskActor(2);
        _ShowContourMask->setText(infoStr);
    }
}

void MainDialog::ContextAction_ThresholdMask()
{
    if(ProcessInfo.isShowThresholdMask)
    {
        codec = QTextCodec::codecForName("GBK");
        QString infoStr =  codec->toUnicode("显示阈值分割Mask");
        RemoveMaskActor(0);
        _ShowThresholdMask->setText(infoStr);
    }
    else
    {
        codec = QTextCodec::codecForName("GBK");
        QString infoStr =  codec->toUnicode("隐藏阈值分割Mask");
        AddMaskActor(0);
        _ShowThresholdMask->setText(infoStr);
    }
}

void MainDialog::ContextAction_RegionGrowMask()
{
    if(ProcessInfo.isShowRegionGrowMask)
    {
        codec = QTextCodec::codecForName("GBK");
        QString infoStr =  codec->toUnicode("显示区域增长Mask");
        RemoveMaskActor(1);
        _ShowRegionGrowMask->setText(infoStr);
    }
    else
    {
        codec = QTextCodec::codecForName("GBK");
        QString infoStr =  codec->toUnicode("隐藏区域增长Mask");
        AddMaskActor(1);
        _ShowRegionGrowMask->setText(infoStr);
    }
}

void MainDialog::ContextMenu_Hide()
{
    _qvtkWidget->GetRenderWindow()->GetInteractor()->RightButtonReleaseEvent();
}

void MainDialog::UpdateTumorParam(double sizex,double sizey,double sizez,double volume)
{
    ProcessInfo.TumorSize[0]=sizex;
    ProcessInfo.TumorSize[1]=sizey;
    ProcessInfo.TumorSize[2]=sizez;
    ui->label__TumorSize->setText(QString("%1*%2*%3")
                                  .arg(QString::number(sizex/10.0,'f',2))
                                  .arg(QString::number(sizey/10.0,'f',2))
                                  .arg(QString::number(sizez/10.0,'f',2)));

    int radius = sizex>sizey?sizex:sizey;
    radius = radius > sizez?radius:sizez;

    radius = (radius)*0.5+10;

//    if (radius>ui->horizontalSlider_search_radius->minimum())
//    {
//        qDebug()<<"set radius "<<QString::number(radius);
//        ui->horizontalSlider_search_radius->setValue(radius);
//    }

    ProcessInfo.TumorVolume = volume;
    ui->label__TumorVolume->setText(QString::number(ProcessInfo.TumorVolume/1000.0,'f',3));
}
void MainDialog::releaseData()
{
    if(_ImageResample->GetOutput()!=NULL){
#ifdef QT_DEBUG
        qDebug()<<"_ImageResample->GetOutput()    "<<_ImageResample->GetOutput()<<_ImageResample->GetOutput()->GetActualMemorySize()/1024;
#endif
        //_ImageResample->GetOutput()->ReleaseData();
        //_ImageResample->GetOutput()->PrepareForNewData();
    }
    if(thresholdMaskActor->GetMapper()->GetInput()!=NULL){
#ifdef QT_DEBUG
        qDebug()<<"thresholdMaskActor->GetMapper()"<<thresholdMaskActor->GetMapper()->GetInput()<<thresholdMaskActor->GetMapper()->GetInput()->GetActualMemorySize()/1024;
#endif
        thresholdMaskActor->GetMapper()->GetInput()->ReleaseData();
    }
    if(LimitedRegionRect_Mapper->GetInput()!=NULL){
#ifdef QT_DEBUG
        qDebug()<<"LimitedRegionRect_Mapper       "<<LimitedRegionRect_Mapper->GetInput()<<LimitedRegionRect_Mapper->GetInput()->GetActualMemorySize()/1024;
#endif
        LimitedRegionRect_Mapper->GetInput()->ReleaseData();
    }
    if(PreProImageImport->GetOutput()!=NULL){
#ifdef QT_DEBUG
        qDebug()<<"PreProImageImport->GetOutput() "<<PreProImageImport->GetOutput()<<PreProImageImport->GetOutput()->GetActualMemorySize()/1024;
#endif
        PreProImageImport->GetOutput()->ReleaseData();
    }

    if (PreProMaskImport->GetOutput()!=NULL)
    {
        PreProMaskImport->GetOutput()->ReleaseData();
    }

    if (contourImport->GetOutput()!=NULL)
    {
        contourImport->GetOutput()->ReleaseData();
    }

    if (contourActor->GetMapper()->GetDataObjectInput()!=NULL)
    {
        contourActor->GetMapper()->GetDataObjectInput()->ReleaseData();
        myContourWidget->ProcessEventsOff();
    }

    if(regionGrowingImageImport->GetOutput()!=NULL){
#ifdef QT_DEBUG
        qDebug()<<"regionGrowingImageImport->GetOutput()"<<regionGrowingImageImport->GetOutput()<<regionGrowingImageImport->GetOutput()->GetActualMemorySize()/1024;
#endif
        regionGrowingImageImport->GetOutput()->ReleaseData();
    }

    if(regionGrowingMaskActor->GetMapper()->GetDataObjectInput()!=NULL)
    {
#ifdef QT_DEBUG
        qDebug()<<"regionGrowingMaskActor->GetMapper()   "<<regionGrowingMaskActor->GetMapper()->GetDataObjectInput()<<regionGrowingMaskActor->GetMapper()->GetDataObjectInput()->GetActualMemorySize()/1024;
#endif
        regionGrowingMaskActor->GetMapper()->GetDataObjectInput()->ReleaseData();
    }

//    if(regionGrowingImageImport_NewOri->GetOutput()!=NULL){
//#ifdef QT_DEBUG
//        qDebug()<<"regionGrowingImageImport_NewOri->GetOutput() "<<regionGrowingImageImport_NewOri->GetOutput()<<regionGrowingImageImport_NewOri->GetOutput()->GetActualMemorySize()/1024;
//#endif
//        regionGrowingImageImport_NewOri->GetOutput()->ReleaseData();
//    }

    if (RegionGrowImageDataArray!=NULL)
    {
        delete []RegionGrowImageDataArray;
        RegionGrowImageDataArray = NULL;
    }

//    if (RegionGrowImageArray_NewOri!=NULL)
//    {
//        delete []RegionGrowImageArray_NewOri;
//        RegionGrowImageArray_NewOri = NULL;
//    }

    if (ContoursMaskArray!=NULL)
    {
        delete []ContoursMaskArray;
        ContoursMaskArray = NULL;
    }

    isContourMaskInited = false;

    if(_pPreProData!=NULL)
    {
        delete[] _pPreProData;
        _pPreProData=NULL;
    }
    if(_STLPolyData!=NULL){
#ifdef QT_DEBUG
        qDebug()<<"_STLPolyData size:"<<_STLPolyData<<_STLPolyData->GetActualMemorySize()/1024.0;
#endif
        _STLPolyData->ReleaseData();
    }
}

void MainDialog::ClearAllData()
{
    myContourWidget->ClearContour();
    LimitedRegionOperationSwitch(NoOperation);
    if(bInitView)
    {
        _imageViewer->GetRenderer()->RemoveActor(thresholdMaskActor);
        _imageViewer->GetRenderer()->RemoveActor(regionGrowingMaskActor);
        _imageViewer->GetRenderer()->RemoveActor2D(LimitedRegionRect_Actor);

        _vtkQtSlotConnect->Disconnect(_qvtkWidget->GetRenderWindow()->GetInteractor(),vtkCommand::MouseMoveEvent,
                                      this,SLOT(on_qtVtkWidgetMouseMove(vtkObject*,ulong,void*,void*)));
        _vtkQtSlotConnect->Disconnect(_qvtkWidget->GetRenderWindow()->GetInteractor(),vtkCommand::LeftButtonPressEvent,
                                      this,SLOT(on_qtVtkWidgetMouseClicked(vtkObject*,ulong,void*,void*)));
        _vtkQtSlotConnect->Disconnect(_qvtkWidget->GetRenderWindow()->GetInteractor(),vtkCommand::MiddleButtonPressEvent,
                                      this,SLOT(on_qtVtkWidgetMiddleButtonPressEvent(vtkObject*,ulong,void*,void*)));
        _vtkQtSlotConnect->Disconnect(_qvtkWidget->GetRenderWindow()->GetInteractor(),vtkCommand::MouseWheelBackwardEvent,
                                      this,SLOT(on_qtVtkWidgetMouseWheelBackwardEvent(vtkObject*,ulong,void*,void*)));
        _vtkQtSlotConnect->Disconnect(_qvtkWidget->GetRenderWindow()->GetInteractor(),vtkCommand::MouseWheelForwardEvent,
                                      this,SLOT(on_qtVtkWidgetMouseWheelForwardEvent(vtkObject*,ulong,void*,void*)));
        _vtkQtSlotConnect->Disconnect(_qvtkWidget->GetRenderWindow()->GetInteractor(),vtkCommand::RightButtonPressEvent,
                                      this,SLOT(on_qtVtkWiggetMouseRightButtonClicked(vtkObject*,ulong,void*,void*,vtkCommand *)));
        _vtkQtSlotConnect->Disconnect(_qvtkWidget->GetRenderWindow()->GetInteractor(),vtkCommand::RenderEvent,
                                      this,SLOT(on_qtVtkWiggetRenderEvent(vtkObject*,ulong,void*,void*)));

        //        vtkSmartPointer<vtkRenderer> dicomRender=vtkSmartPointer<vtkRenderer>::New();
        //        vtkSmartPointer<vtkRenderWindow> dicomWindow=vtkSmartPointer<vtkRenderWindow>::New();
        //        dicomWindow->AddRenderer(dicomRender);
        _qvtkWidget->SetRenderWindow(_emptyDicomRenderWindow);
        _emptyDicomRenderWindow->Render();
        _emptySTLRender->RemoveActor(_STLActor);
        _qvtkWidget_STLView->GetRenderWindow()->Render();

        //        _imageViewer->GetRenderWindow()->InitializeFromCurrentContext();
        //        _qvtkWidget_STLView->GetRenderWindow()->InitializeFromCurrentContext();

        _imageViewer->GetRenderer()->RemoveActor2D(RTRect_Actor);
        bInitView=false;
        releaseData();

    }

    SetSeedPos(-1,-1,-1);
//    SetLimitRegionZ(0,0);
//    SetLimitPoint1(-1,-1);
//    SetLimitPoint2(-1,-1,0,0);
    UpdateTumorParam(0.0,0.0,0.0,0.00);

    if (RegionGrowImageDataArray!=NULL)
    {
        delete []RegionGrowImageDataArray;
        RegionGrowImageDataArray = NULL;
    }

    ui->verticalScrollBar_DicomChange_XY->setEnabled(false);
    DicomInfo.ClearParam();
    ProcessInfo.ClearParam();
}

void MainDialog::SetVtkContourWidget(MyVtkContourWidget* myContourWidget2)
{
    myContourWidget=myContourWidget2;
}

void MainDialog::on_pushButton_ManualDrawOn_clicked()
{
    if(!DicomInfo.isSourceRead)
        return;
    if(defaultdicom)
    {
        codec = QTextCodec::codecForName("GBK");
        QString info1Str =  codec->toUnicode("提示");
        QString info2Str =  codec->toUnicode("当前为默认图像，请重新导入dicom图像！");
        QMessageBox::warning(NULL,info1Str,info2Str);
        return;

    }
    ui->pushButton_ManualDrawClearAll->setEnabled(true);
    ui->pushButton_ManualDrawClearAll->setStyleSheet(SELECTED);
    ui->pushButton_ManualDrawClearCur->setEnabled(true);
    ui->pushButton_ManualDrawClearCur->setStyleSheet(SELECTED);
    LimitedRegionOperationSwitch(ManualDraw);
}

void MainDialog::on_tabWidget_tabBarClicked(int index)
{
    if(!DicomInfo.isSourceRead)
    {
        if(index==0)
        {
            ProcessInfo.FecthTumorMethod=FetchThreshold;
        }
        else
        {
            ProcessInfo.FecthTumorMethod=FetchManualDraw;
        }

        return;
    }
    if(index==0)
    {
        ProcessInfo.FecthTumorMethod=FetchThreshold;
        //ui->pushButton_PickSeedPoint->setEnabled(true);
        //ui->pushButton_PickSeedPoint->setStyleSheet(SELECTED);
        myContourWidget->ClearContour();
        LimitedRegionOperationSwitch(NoOperation);
        ManualDrawProcess(true);
    }
    else
    { 
        return;

        ProcessInfo.FecthTumorMethod=FetchManualDraw;
        //ui->pushButton_PickSeedPoint->setEnabled(false);
        //ui->pushButton_PickSeedPoint->setStyleSheet(NOT_WORK);
        SeedRegionGrowingProcess(true);
        LimitedRegionOperationSwitch(NoOperation);
        //ui->spinBox_Threshold_Min->setValue(0);
        //ui->spinBox_Threshold_Max->setValue(0);
    }
}

void MainDialog::on_pushButton_ManualDrawClearCur_clicked()
{
    myContourWidget->ClearContour(_imageViewer->GetSlice());
}

void MainDialog::on_pushButton_ManualDrawClearAll_clicked()
{
    myContourWidget->ClearContour();
    if (ContoursMaskArray!=NULL)
    {
        memset(ContoursMaskArray,0,DicomInfo.m_iVoxelSize*sizeof(uchar));
    }

    _imageViewer->GetRenderer()->RemoveActor(contourActor);
    _imageViewer->GetRenderWindow()->Render();

    ProcessInfo.isContourCreated = false;


    ui->pushButton_SurfaceRendering_2->setEnabled(false);
    ui->pushButton_SurfaceRendering_2->setStyleSheet(NOT_WORK);

    ui->pushButton_GetContours->setEnabled(false);
    ui->pushButton_GetContours->setStyleSheet(NOT_WORK);
}

void MainDialog::on_toolButton_grow_confirm_clicked()
{
    on_pushButton_Grow_clicked();
}

void MainDialog::on_horizontalSlider_search_radius_valueChanged(int value)
{
    m_iSearchRadius = value;
    //ui->label__search_radius->setText(QString::number(m_iSearchRadius));
    emit sendSearchRadius(m_iSearchRadius);
}

void MainDialog::on_pushButton_output_mask_clicked()
{
    QDir dirtemp;
    QString rootpath=dirtemp.currentPath();
    rootpath=rootpath.left(rootpath.indexOf(":")+1);
    QString maskFold = rootpath.append("/mask/");
    QDir fold(maskFold);
    if(fold.exists())
    {

    }
    else
    {
        bool ok = fold.mkpath(maskFold);
        if (!ok)
        {
            QString info("创建文件夹失败 ");
            info.append("maskFold");
            codec = QTextCodec::codecForName("GBK");
            QString info1Str =  codec->toUnicode("提示");
            QString info2Str =  codec->toUnicode("创建文件夹失败");

            QMessageBox::warning(NULL,info1Str,info2Str);

            return;
        }
    }

    QString file = maskFold;
    QFileDialog fd(this,tr("保存截图"),
                   file,
                   tr("ZH(*.zh)"));

    fd.setAcceptMode(QFileDialog::AcceptSave);//文件对话框为打开类型
    fd.setFileMode(QFileDialog::Directory);//文件夹名
    file.append("//mask.zh");
    fd.setDirectory(file);

    if (fd.exec()==QFileDialog::Accepted)
    {

        QStringList fileName = fd.selectedFiles();
        qCDebug(HIFUNAVIGATION())<<HIFUNAVIGATION().categoryName()
                                <<"Write mask to "<<file
                               << TRACE_CMH();

        if (!outputMask(*fileName.begin()))
        {
            codec = QTextCodec::codecForName("GBK");
            QString info1Str =  codec->toUnicode("提示");
            QString info2Str =  codec->toUnicode("写出掩模数据失败！");
            QMessageBox::warning(NULL,info1Str,info2Str);
        }
    }

}

void MainDialog::on_pushButton_input_mask_clicked()
{
    QFileDialog fd;
    fd.setAcceptMode(QFileDialog::AcceptOpen);//文件对话框为打开类型
    fd.setFileMode(QFileDialog::ExistingFiles);//文件夹名
    QDir dirtemp;
    QString rootpath=dirtemp.currentPath();
    rootpath=rootpath.left(rootpath.indexOf(":")+1);
    rootpath.append("/mask/");
    fd.setDirectory(rootpath);

    if (fd.exec()==QFileDialog::Accepted)
    {
        QStringList fileNames = fd.selectedFiles();
        if (!inputMask(*fileNames.begin()))
        {
            codec = QTextCodec::codecForName("GBK");
            QString info1Str =  codec->toUnicode("提示");
            QString info2Str =  codec->toUnicode("读入掩模数据失败！");
            QMessageBox::warning(NULL,info1Str,info2Str);
            return;
        }

        if(!ProcessInfo.isRegionGrow)
        {
            InitRegionGrowMask();
        }
        else
        {
            double* ori = _inImageData->GetOrigin();
            int i = _imageViewer->GetSlice();
            ori[2] = i*DicomInfo.spacing_z;

            void* p = RegionGrowImageDataArray+i*DicomInfo.width*DicomInfo.height;
            regionGrowingImageImport->SetImportVoidPointer(p);

            regionGrowingImageImport->SetDataOrigin(ori);
            regionGrowingImageImport->Update();

            _imageViewer->GetRenderWindow()->Render();
            if(!ProcessInfo.isShowRegionGrowMask)
            {
                _imageViewer->GetRenderer()->AddActor(regionGrowingMaskActor);
                _imageViewer->GetRenderWindow()->Render();
                ProcessInfo.isShowRegionGrowMask=true;
            }
        }

        if (!ui->pushButton_output_mask->isEnabled())
        {
            ui->pushButton_output_mask->setEnabled(true);
            ui->pushButton_output_mask->setStyleSheet(SELECTED);
        }
    }

}

// 导出mask数据
bool MainDialog::outputMask(QString filename)
{
    if (RegionGrowImageDataArray==NULL)
    {
        codec = QTextCodec::codecForName("GBK");
        QString info1Str =  codec->toUnicode("提示");
        QString info2Str =  codec->toUnicode("掩模数据为空，请先生成掩模！");
        QMessageBox::warning(NULL,info1Str,info2Str);
        return false;
    }

    QFile file(filename);
    //读二进制文件
    if (!file.open(QIODevice::WriteOnly))
    {
        QString info("不能打开写入文件");
        info.append(filename);
        codec = QTextCodec::codecForName("GBK");
        QString info1Str =  codec->toUnicode("提示");
        QString info2Str =  codec->toUnicode("不能打开写入文件");
        QMessageBox::warning(NULL,info1Str,info2Str);
        return false;
    }
    // 写SERIESUID长度
    int uidSize = DicomInfo.SeriesUID.size();
    if (sizeof(int)!=file.write((char*)&uidSize,sizeof(int)))
    {
        qCDebug(HIFUNAVIGATION()) << HIFUNAVIGATION().categoryName()
                                  << "写出SERIESUID 长度 "<< uidSize
                                  << TRACE_CMH();
        file.close();
        return false;
    }

    if (uidSize!=file.write(DicomInfo.SeriesUID.toStdString().c_str(),uidSize))
    {
        qCDebug(HIFUNAVIGATION()) << HIFUNAVIGATION().categoryName()
                                  << "写出SERIESUID失败 "
                                  << TRACE_CMH();
        file.close();
        return false;
    }

    // 写阈值
    if (sizeof(int)!=file.write((char*)&ProcessInfo.threshold_min,sizeof(ProcessInfo.threshold_min)))
    {
        qCDebug(HIFUNAVIGATION()) << HIFUNAVIGATION().categoryName()
                                  << "写出最小阈值失败 "<< ProcessInfo.threshold_min
                                  << TRACE_CMH();
        file.close();
        return false;
    }

    if (sizeof(int)!=file.write((char*)&ProcessInfo.threshold_max,sizeof(ProcessInfo.threshold_max)))
    {
        qCDebug(HIFUNAVIGATION()) << HIFUNAVIGATION().categoryName()
                                  << "写出最大阈值失败 "<< ProcessInfo.threshold_max
                                  << TRACE_CMH();
        file.close();
        return false;
    }

    // 写种子点
    if (sizeof(int)!=file.write((char*)&ProcessInfo.SeedPoint_X,sizeof(ProcessInfo.SeedPoint_X)))
    {
        qCDebug(HIFUNAVIGATION()) << HIFUNAVIGATION().categoryName()
                                  << "写出种子点x失败 "<< ProcessInfo.SeedPoint_X
                                  << TRACE_CMH();
        file.close();
        return false;
    }

    if (sizeof(int)!=file.write((char*)&ProcessInfo.SeedPoint_Y,sizeof(ProcessInfo.SeedPoint_Y)))
    {
        qCDebug(HIFUNAVIGATION()) << HIFUNAVIGATION().categoryName()
                                  << "写出种子点y失败 "<< ProcessInfo.SeedPoint_Y
                                  << TRACE_CMH();
        file.close();
        return false;
    }

    if (sizeof(int)!=file.write((char*)&ProcessInfo.SeedPoint_Z,sizeof(ProcessInfo.SeedPoint_Z)))
    {
        qCDebug(HIFUNAVIGATION()) << HIFUNAVIGATION().categoryName()
                                  << "写出种子点z失败 "<< ProcessInfo.SeedPoint_Z
                                  << TRACE_CMH();

        file.close();
        return false;
    }

    qint64 size = DicomInfo.m_iVoxelSize;
    if (size!=file.write((char*)RegionGrowImageDataArray,DicomInfo.m_iVoxelSize))
    {
        file.close();
        codec = QTextCodec::codecForName("GBK");
        QString info1Str =  codec->toUnicode("提示");
        QString info2Str =  codec->toUnicode("写入数据失败");
        QMessageBox::warning(NULL,info1Str,info2Str);
        return false;
    }

    file.close();
    QString info("写入数据成功 ");
    info.append(filename);
    codec = QTextCodec::codecForName("GBK");
    QString info1Str =  codec->toUnicode("提示");
    QString info2Str =  codec->toUnicode("写入数据成功");
    QMessageBox::warning(NULL,info1Str,info2Str);
    return true;
}

// 导入mask数据
bool MainDialog::inputMask(QString filename)
{
    QFile file(filename);
    if(!file.open(QIODevice::ReadOnly))
    {
        QString info("不能打开读入文件");
        info.append(filename);
        codec = QTextCodec::codecForName("GBK");
        QString info1Str =  codec->toUnicode("提示");
        QString info2Str =  codec->toUnicode("不能打开读入文件");
        QMessageBox::warning(NULL,info1Str,info2Str);

        return false;
    }

    int uidSize;
    if (sizeof(int)!=file.read((char*)&uidSize,sizeof(int)))
    {
        codec = QTextCodec::codecForName("GBK");
        QString info1Str =  codec->toUnicode("提示");
        QString info2Str =  codec->toUnicode("读入UIDSIZE 长度失败");
        QMessageBox::warning(NULL,info1Str,info2Str);

        file.close();
        return false;
    }
    qDebug()<<"read uid size is "<<QString::number(uidSize);
    qDebug()<<"series uid size is "<<DicomInfo.SeriesUID.size();

    size_t fileSize = file.size();
    qint64 size = DicomInfo.m_iVoxelSize;
    qDebug()<<"mask size is  "<<DicomInfo.m_iVoxelSize;
    if (fileSize!=size+6*sizeof(int)+uidSize)
    {
        QString info("读入文件长度不对");
        info.append(QString::number(size+6*sizeof(int)+uidSize));

        codec = QTextCodec::codecForName("GBK");
        QString info1Str =  codec->toUnicode("提示");
        QString info2Str =  codec->toUnicode("读入文件长度不对");
        QMessageBox::warning(NULL,info1Str,info2Str);

        file.close();
        return false;
    }

//    char seriesUID[uidSize+1];
    char *seriesUID = new char[uidSize];
    if (uidSize!=file.read(seriesUID,uidSize))
    {
        codec = QTextCodec::codecForName("GBK");
        QString info1Str =  codec->toUnicode("提示");
        QString info2Str =  codec->toUnicode("读入UIDSIZE失败");
        QMessageBox::warning(NULL,info1Str,info2Str);

        file.close();
        return false;
    }
    else
    {
        seriesUID[uidSize]='\0';
        if (QString(seriesUID)==DicomInfo.SeriesUID)
        {

        }
        else
        {
            codec = QTextCodec::codecForName("GBK");
            QString info1Str =  codec->toUnicode("提示");
            QString info2Str =  codec->toUnicode("读入seriesUID不匹配");
            QMessageBox::warning(NULL,info1Str,info2Str);
            qDebug()<<"mask series uid is "<<QString(seriesUID);
            qDebug()<<"dicom series uid is "<<DicomInfo.SeriesUID;
            QMessageBox::warning(NULL,info1Str,QString(seriesUID));
            QMessageBox::warning(NULL,info1Str,DicomInfo.SeriesUID);

            file.close();
            return false;
        }
    }

    int min;
    int max;
    if (sizeof(int)!=file.read((char*)&min,sizeof(int)))
    {
        codec = QTextCodec::codecForName("GBK");
        QString info1Str =  codec->toUnicode("提示");
        QString info2Str =  codec->toUnicode("读入阈值最小值失败");
        QMessageBox::warning(NULL,info1Str,info2Str);

        file.close();
        return false;
    }

    if (sizeof(int)!=file.read((char*)&max,sizeof(int)))
    {
        codec = QTextCodec::codecForName("GBK");
        QString info1Str =  codec->toUnicode("提示");
        QString info2Str =  codec->toUnicode("读入阈值最大值失败");
        QMessageBox::warning(NULL,info1Str,info2Str);

        file.close();
        return false;
    }

    //ui->horizontalSlider_Threshold_Max->setValue(max);
    //ui->horizontalSlider_Threshold_Min->setValue(min);

    int seedx;
    int seedy;
    int seedz;
    if (sizeof(int)!=file.read((char*)&seedx,sizeof(int)))
    {
        codec = QTextCodec::codecForName("GBK");
        QString info1Str =  codec->toUnicode("提示");
        QString info2Str =  codec->toUnicode("读入种子点x失败");
        QMessageBox::warning(NULL,info1Str,info2Str);

        file.close();
        return false;
    }

    if (sizeof(int)!=file.read((char*)&seedy,sizeof(int)))
    {
        codec = QTextCodec::codecForName("GBK");
        QString info1Str =  codec->toUnicode("提示");
        QString info2Str =  codec->toUnicode("读入种子点y失败");
        QMessageBox::warning(NULL,info1Str,info2Str);

        file.close();
        return false;
    }

    if (sizeof(int)!=file.read((char*)&seedz,sizeof(int)))
    {
        codec = QTextCodec::codecForName("GBK");
        QString info1Str =  codec->toUnicode("提示");
        QString info2Str =  codec->toUnicode("读入种子点z失败");
        QMessageBox::warning(NULL,info1Str,info2Str);

        file.close();
        return false;
    }

    ProcessInfo.SeedPoint_X=seedx;
    ProcessInfo.SeedPoint_Y=seedy;
    ProcessInfo.SeedPoint_Z=seedz;

    if (RegionGrowImageDataArray!=NULL)
    {
        delete []RegionGrowImageDataArray;
        RegionGrowImageDataArray = NULL;
    }

    RegionGrowImageDataArray = new uchar[DicomInfo.m_iVoxelSize];
    if (size!=file.read((char*)RegionGrowImageDataArray,size))
    {
        file.close();
        codec = QTextCodec::codecForName("GBK");
        QString info1Str =  codec->toUnicode("提示");
        QString info2Str =  codec->toUnicode("读入掩模失败");
        QMessageBox::warning(NULL,info1Str,info2Str);
        return false;
    }

    file.close();
    codec = QTextCodec::codecForName("GBK");
    QString info1Str =  codec->toUnicode("提示");
    QString info2Str =  codec->toUnicode("读入掩模成功");
    QMessageBox::warning(NULL,info1Str,info2Str);
    return true;
}

void MainDialog::on_pushButton_generate_contour_clicked()
{
//#if 0
    if (RegionGrowImageDataArray==NULL)
        return;

    int height = DicomInfo.height;
    int width = DicomInfo.width;
    int depth = DicomInfo.depth;
    for (int i = 0; i < depth;++i)
    {
        int index = width*height*i;
        cv::Mat raw(height, width, CV_8UC1, &RegionGrowImageDataArray[index]);

        GetMaskPolyData(DicomInfo.spacing_x,DicomInfo.spacing_y,i,raw);
    }

    int iSlice = _imageViewer->GetSlice();
    myContourWidget->SetCurSlice(iSlice);
    myContourWidget->On();
    myContourWidget->EnabledOn();
    myContourWidget->ProcessEventsOn();

    _imageViewer->GetRenderWindow()->Render();
//#endif
}

void MainDialog::GetMaskPolyData(double spacex,double spacey,int slice,cv::Mat image)
{
//#if 0
    // vtkSmartPointer<vtkImageImport> importer = vtkSmartPointer<vtkImageImport>::New();
    importer->SetWholeExtent(0, image.cols-1, 0, image.rows-1, 0, 0 );
    importer->SetDataExtentToWholeExtent();
    importer->SetDataScalarTypeToUnsignedChar();
    importer->SetNumberOfScalarComponents(1);
    importer->SetImportVoidPointer(image.data);
    importer->Update();

    //    //[3] --图像二值化
    // vtkSmartPointer<vtkImageThreshold> thresholdFilter=vtkSmartPointer<vtkImageThreshold>::New();
    thresholdFilter->SetInputConnection(importer->GetOutputPort());
    thresholdFilter->ThresholdByUpper(1);//大于该灰度范围的区间为有效范围
    thresholdFilter->SetInValue(255);//有效范围内的输出值
    thresholdFilter->SetOutValue(0);//无效范围的输出值
    thresholdFilter->UpdateDataObject();
    thresholdFilter->UpdateInformation();
    thresholdFilter->Update();
    //[3]

    //[4] --exactor contour
    //vtkSmartPointer<vtkContourFilter> contourFilter = vtkSmartPointer<vtkContourFilter>::New();
    contourFilter->SetInputConnection(thresholdFilter->GetOutputPort());
    contourFilter->ComputeNormalsOn();
    contourFilter->SetValue(0,100);
    contourFilter->Update();
    //[4]

    //[4] --连通区域分析
    vtkSmartPointer<vtkPolyDataConnectivityFilter> connectFilter1=vtkSmartPointer<vtkPolyDataConnectivityFilter>::New();
    connectFilter1->SetInputConnection(contourFilter->GetOutputPort());
    connectFilter1->SetExtractionModeToAllRegions();
    connectFilter1->ColorRegionsOn();
    connectFilter1->MarkVisitedPointIdsOn();
    connectFilter1->Update();
    int regionNum=connectFilter1->GetNumberOfExtractedRegions();
    // qDebug()<<"regionNum:"<<regionNum;
    //[4]

    if (regionNum==0)
    {
        return;
    }

    vtkSmartPointer<vtkPoints> point0=vtkSmartPointer<vtkPoints>::New();
    for(int i=0;i<connectFilter1->GetOutput()->GetNumberOfPoints();++i)
    {
//        double regionId=connectFilter1->GetOutput()->GetPointData()->GetScalars()->GetTuple1(i);
		int regionId = connectFilter1->GetOutput()->GetPointData()->GetScalars()->GetTuple1(i);
//        int regionId=connectFilter1->GetOutput()->GetPointData()->GetScalars()->GetTuple1(i);
        double *xyz=connectFilter1->GetOutput()->GetPoint(i);

        if(regionId==0)//选取区域标号为0的轮廓点集
            point0->InsertNextPoint(xyz);
    }

    //[6] --提取轮廓凸包
    vtkSmartPointer<vtkPointsProjectedHull> hull = vtkSmartPointer<vtkPointsProjectedHull>::New();
    hull->DeepCopy(point0);
    int zSize =hull->GetSizeCCWHullZ();

    double *pts=new double[zSize*2];
    hull->GetCCWHullZ(pts,zSize);

    vtkSmartPointer<vtkPoints> zHullPoints=vtkSmartPointer<vtkPoints>::New();
    for(int i=0;i<zSize;++i)
    {
        double xval=pts[2*i]*spacex;
        double yval=pts[2*i+1]*spacey;
        // qDebug()<<"(x,z)value of point"<<i<<":("<<xval<<yval<<")";
        zHullPoints->InsertPoint(static_cast<vtkIdType>(i), xval, yval, 0.0 );
    }

    // Create a cell array to connect the points into meaningful geometry
    vtkIdType* vertexIndices = new vtkIdType[zSize+1];
    for (int i = 0; i < zSize; i++)
    {
        vertexIndices[i] = static_cast<vtkIdType>(i);
    }
    // Set the last vertex to 0; this means the last line segment will join the 19th point (vertices[19])
    // with the first one (vertices[0]), thus closing the circle.
    vertexIndices[zSize] = 0;
    vtkSmartPointer<vtkCellArray> lines = vtkSmartPointer<vtkCellArray>::New();
    lines->InsertNextCell(zSize+1, vertexIndices);

    // Create polydata to hold the geometry just created, and populate it
    vtkSmartPointer<vtkPolyData> polydata = vtkSmartPointer<vtkPolyData>::New();
    polydata->SetPoints(zHullPoints);
    polydata->SetLines(lines);

    (*myContourWidget->GetConturList())[slice] = polydata;
    (*myContourWidget->GetConturList_Rep())[slice] = polydata;

//#endif
    // delete []pts;
}

// 生成肌瘤掩模
void MainDialog::on_pushButton_generate_mask_clicked()
{
//#if 0
    if (ContoursMaskArray==NULL)
    {
        ContoursMaskArray = new uchar[DicomInfo.m_iVoxelSize];
    }

    memset(ContoursMaskArray,0,DicomInfo.m_iVoxelSize);
    QMap<int,vtkSmartPointer<vtkPolyData> >& list = *myContourWidget->GetConturList();

    QMap<int,vtkSmartPointer<vtkPolyData> >::const_iterator i = list.constBegin();
    while (i != list.constEnd())
    {
        int iz=i.key();
        vtkSmartPointer<vtkPolyData> contourdata=vtkSmartPointer<vtkPolyData>::New();
        contourdata->DeepCopy(i.value());
        vtkSmartPointer<vtkPolygon> polygon = vtkSmartPointer<vtkPolygon>::New();

        for(int i=0;i<contourdata->GetNumberOfPoints();i++)
        {
            vtkIdType id(i);
            double *p;
            p=contourdata->GetPoint(id);
            polygon->GetPoints()->InsertNextPoint(p[0],p[1],0);
        }
        double n[3];
        polygon->ComputeNormal(polygon->GetPoints()->GetNumberOfPoints(),
                               static_cast<double*>(polygon->GetPoints()->GetData()->GetVoidPointer(0)), n);

        double bounds[6];
        polygon->GetPoints()->GetBounds(bounds);

        double testInside[3] = {0.0, 0.0, 0.0};
        double *contourbound=contourdata->GetBounds();
        contourbound[0]=(contourbound[0]-DicomInfo.spacing_x)/DicomInfo.spacing_x;
        contourbound[1]=(contourbound[1]+DicomInfo.spacing_x)/DicomInfo.spacing_x;
        contourbound[2]=(contourbound[2]-DicomInfo.spacing_y)/DicomInfo.spacing_y;
        contourbound[3]=(contourbound[3]+DicomInfo.spacing_y)/DicomInfo.spacing_y;

        for(int ix=contourbound[0];ix<contourbound[1];++ix)
        {
            for(int iy=contourbound[2];iy<contourbound[3];++iy)
            {
                testInside[0]=ix*DicomInfo.spacing_x;
                testInside[1]=iy*DicomInfo.spacing_y;
                if(polygon->PointInPolygon(testInside,
                                           polygon->GetPoints()->GetNumberOfPoints(), static_cast<double*>(
                                               polygon->GetPoints()->GetData()->GetVoidPointer(0)), bounds, n))
                {
                    ContoursMaskArray[ix+iy*DicomInfo.width+iz*DicomInfo.width*DicomInfo.height]=1;
                }
            }
        }

        ++i;
    }

    if (!isContourMaskInited)
    {
       InitContourMask();
    }
    else
    {
        contourMapper->RemoveAllInputConnections(0);
        contourMapper->SetInputConnection(0,contourImport->GetOutputPort());
        contourMapper->Update();

        _imageViewer->GetRenderWindow()->Render();
    }
//#endif
}

void MainDialog::on_pushButton_reslice_contour_clicked()
{
   GetContoursNew();
}


void MainDialog::on_pushButton_generate_ellips_clicked()
{
    int xmax;
    int xmin;
    int ymax;
    int ymin;
    int zmax;
    int zmin;
    double A;
    double x0;
    double B;
    double y0;
    double C;
    double z0;

    if (ui->radioButton_draw->isChecked())
    {
        QMap<int,vtkSmartPointer<vtkPolyData> >* ContourList=myContourWidget->GetConturList_Rep();

        std::vector<point3d> points;
        qDebug()<<"Enter set contours !";
        qDebug()<<"contour list rep size is "<<myContourWidget->GetConturList_Rep()->size();

        if (myContourWidget->GetConturList()->size()<3)
        {

            codec = QTextCodec::codecForName("GBK");
            QString info1Str =  codec->toUnicode("勾画张数");
            QString info2Str =  codec->toUnicode("请勾画三张以上");

            QMessageBox msgBox(info1Str,
                               info2Str,
                               QMessageBox::Warning,
                               QMessageBox::Yes | QMessageBox::Default,
                               QMessageBox::No | QMessageBox::Escape,
                               QMessageBox::NoButton);


            msgBox.setWindowFlags(Qt::WindowTitleHint|Qt::CustomizeWindowHint);

            msgBox.setGeometry(QRect(1400,400,msgBox.width(),msgBox.height()));
            int ret = msgBox.exec();
            if (ret == QMessageBox::Yes)
            {
                return;
            }
            else
            {
                return;
            }
        }

        QMap<int,vtkSmartPointer<vtkPolyData> >::const_iterator iter = ContourList->constBegin();
        while (iter != ContourList->constEnd())
        {
            int iz=iter.key();
            vtkSmartPointer<vtkPolyData> contourdata=vtkSmartPointer<vtkPolyData>::New();
            contourdata->DeepCopy(iter.value());
            for(int j=0;j<contourdata->GetNumberOfPoints();j++)
            {
                vtkIdType id(j);
                double *p;
                p=contourdata->GetPoint(id);
                point3d point;
                point.x = p[0];
                point.y = p[1];
                point.z = iz*DicomInfo.spacing_z;
                points.push_back(point);
            }

            ++iter;
        }

        CurveSurface algo;
        arma::mat x(points.size(),1);
        arma::mat y(points.size(),1);
        arma::mat z(points.size(),1);
        for (int i = 0; i < points.size();++i)
        {
            x(i,0) = points[i].x;
            y(i,0) = points[i].y;
            z(i,0) = points[i].z;
        }

        if (!algo.calcCurveSurface(x,y,z))
        {
            qDebug()<<"para A is "<<algo.A;
            qDebug()<<"para B is "<<algo.B;
            qDebug()<<"para C is "<<algo.C;
            qDebug()<<"para X0 is "<<algo.X0;
            qDebug()<<"para Y0 is "<<algo.Y0;
            qDebug()<<"para Z0 is "<<algo.Z0;

            codec = QTextCodec::codecForName("GBK");
            QString info1Str =  codec->toUnicode("拟合椭球失败");
            QString info2Str =  codec->toUnicode("请重新勾划");


            QMessageBox msgBox(info1Str,
                               info2Str,
                               QMessageBox::Warning,
                               QMessageBox::Yes | QMessageBox::Default,
                               QMessageBox::No | QMessageBox::Escape,
                               QMessageBox::NoButton);


            msgBox.setWindowFlags(Qt::WindowTitleHint|Qt::CustomizeWindowHint);

            msgBox.setGeometry(QRect(1400,400,msgBox.width(),msgBox.height()));
            int ret = msgBox.exec();
            if (ret == QMessageBox::Yes)
            {
                myContourWidget->ClearContour();
                return;
            }
            else
            {
                myContourWidget->ClearContour();
                return;
            }
        }

        xmax = (algo.X0+algo.A)/DicomInfo.spacing_x;
        xmin = (algo.X0-algo.A)/DicomInfo.spacing_x;
        ymax = (algo.Y0+algo.B)/DicomInfo.spacing_y;
        ymin = (algo.Y0-algo.B)/DicomInfo.spacing_y;
        zmax = (algo.Z0+algo.C)/DicomInfo.spacing_z;
        zmin = (algo.Z0-algo.C)/DicomInfo.spacing_z;

        A = algo.A/DicomInfo.spacing_x;
        x0 = algo.X0/DicomInfo.spacing_x;
        B = algo.B/DicomInfo.spacing_y;
        y0 = algo.Y0/DicomInfo.spacing_y;
        C = algo.C/DicomInfo.spacing_z;
        z0 = algo.Z0/DicomInfo.spacing_z;

        // 设置种子点
        ProcessInfo.SeedPoint_X = algo.X0/DicomInfo.spacing_x;
        ProcessInfo.SeedPoint_Y = algo.Y0/DicomInfo.spacing_y;
        ProcessInfo.SeedPoint_Z = algo.Z0/DicomInfo.spacing_z;


    }
    else if(ui->radioButton_write->isChecked())
    {


        qCDebug(HIFUNAVIGATION())<<HIFUNAVIGATION().categoryName()
                                <<"benin________"
                               <<TRACE_CMH();
        //导入默认dicom图像
        defaultdicom=true;
        //读入默认的dicom图像
        isReadingDicom=true;
        QDir dirtemp;
        QString rootpath=dirtemp.currentPath();
//        rootpath=rootpath.left(rootpath.indexOf(":")+1);//存储在当前项目的盘
        rootpath.append("/default_dcm");//默认的dicom图像存储地址
        qCDebug(HIFUNAVIGATION())<<HIFUNAVIGATION().categoryName()
                                <<"path________"<<rootpath
                               <<TRACE_CMH();
        QString path=rootpath;
        QDir dir(path);
        if(path.isEmpty()||!dir.exists()){
            return;
        }
        dir.setFilter(QDir::Files | QDir::NoSymLinks);
        QFileInfoList dicomlist = dir.entryInfoList();
        QStringList string_list;
        string_list.clear();

        for(int i=0; i<dicomlist.count();i++)
        {
            QFileInfo file_info = dicomlist.at(i);
            QString absolute_file_path = file_info.absoluteFilePath();
            if(_dicomReader->CanReadFile(absolute_file_path.toLatin1().data()))
                string_list.append(absolute_file_path);
        }

        if(string_list.isEmpty())
        {
            QMessageBox::warning(NULL,"提示","没有可读取的核磁图像");
            return;
        }

        if(DicomInfo.isSourceRead)
        {
            //清理原有读取的数据
            LimitedRegionOperationSwitch(NoOperation);
            UpdateTumorParam(0.0,0.0,0.0,0.00);
            _imageViewer->GetRenderer()->RemoveActor(thresholdMaskActor);
            _imageViewer->GetRenderer()->RemoveActor(regionGrowingMaskActor);
            _imageViewer->GetRenderer()->RemoveActor(contourActor);
            _imageViewer->GetRenderer()->RemoveActor2D(LimitedRegionRect_Actor);
            ProcessInfo.ClearParam();
            myContourWidget->ClearContour();

            releaseData();
        }

        _dicomReader->ReleaseDataFlagOn();
        _dicomReader->SetDataScalarTypeToShort();
        _dicomReader->SetDirectoryName(path.toLatin1().data());
        _dicomReader->Update();

        //清理信息和参数
        DicomInfo.ClearParam();
        ProcessInfo.ClearParam();
        SetSeedPos(-1,-1,-1);
//        SetLimitRegionZ(0,0);
//        SetLimitPoint1(-1,-1);
//        SetLimitPoint2(-1,-1,0,0);

        // 重采样
        int* originalDims = _dicomReader->GetOutput()->GetDimensions();
        double* originalSpace=_dicomReader->GetOutput()->GetSpacing();
        qDebug()<<"原图像信息\n"<<"Dims:"<<originalDims[0]<<originalDims[1]<<originalDims[2];
        qDebug()<<"Spacing:"<<originalSpace[0]<<originalSpace[1]<<originalSpace[2];
        if(originalSpace[0]<=0||originalSpace[1]<=0||originalSpace[2]<=0)
        {
            QMessageBox::warning(NULL,"提示","选择的核磁图像数据无效！");
            return;
        }

        if(GetDicomTagsInfo(path))
        {
            qDebug()<<"Get Dicom Tags Success";
            DicomSourceDirecJudge();
        }
        else
            qDebug()<<"Get Dicom Tags failed";
        qDebug()<<"Source Orientation:"<<DicomInfo.OrientationRow<<DicomInfo.OrientationCol<<DicomInfo.OrientationNum;
        //重采样
        double dNewZSpacing=originalSpace[2];
        if(originalSpace[2]>2)
        {
            dNewZSpacing=2;
        }
        if(_ImageResample->GetNumberOfInputConnections(0)==0)
        {
            _ImageResample->SetInputConnection(0,_dicomReader->GetOutputPort());
            _ImageResample->SetInterpolationModeToLinear();
        }

        _ImageResample->SetAxisOutputSpacing(0, originalSpace[0]);
        _ImageResample->SetAxisOutputSpacing(1, originalSpace[1]);
        _ImageResample->SetAxisOutputSpacing(2, dNewZSpacing);
        _ImageResample->Update();

        _inImageData=_ImageResample->GetOutput();
        _inImageOutputPort=_ImageResample->GetOutputPort();
        //显示
        if(DicomInfo.DicomTagInfoList.count()>0&&DicomInfo.DicomTagInfoList[0].dWindowCenter!=-1&&DicomInfo.DicomTagInfoList[0].dWindowWidth!=-1)
        {
            _imageViewer->SetColorLevel(DicomInfo.DicomTagInfoList[0].dWindowCenter);
            _imageViewer->SetColorWindow(DicomInfo.DicomTagInfoList[0].dWindowWidth);
        }
        else
        {
            _imageViewer->SetColorLevel(500);
            _imageViewer->SetColorWindow(2000);
        }
        if(!bInitView)
        {
            // _imageViewer->SetInputConnection(_ImageResample->GetOutputPort());
            _imageViewer->SetInputData(_inImageData);
            _qvtkWidget->SetRenderWindow(_imageViewer->GetRenderWindow());
            _imageViewer->SetupInteractor(_renderWindowInteractor);
            // _imageViewer->Render();

            _vtkQtSlotConnect->Connect(_qvtkWidget->GetRenderWindow()->GetInteractor(),vtkCommand::MouseMoveEvent,
                                       this,SLOT(on_qtVtkWidgetMouseMove(vtkObject*,ulong,void*,void*)));
            _vtkQtSlotConnect->Connect(_qvtkWidget->GetRenderWindow()->GetInteractor(),vtkCommand::LeftButtonPressEvent,
                                       this,SLOT(on_qtVtkWidgetMouseClicked(vtkObject*,ulong,void*,void*)));
            _vtkQtSlotConnect->Connect(_qvtkWidget->GetRenderWindow()->GetInteractor(),vtkCommand::MiddleButtonPressEvent,
                                       this,SLOT(on_qtVtkWidgetMiddleButtonPressEvent(vtkObject*,ulong,void*,void*)));
            _vtkQtSlotConnect->Connect(_qvtkWidget->GetRenderWindow()->GetInteractor(),vtkCommand::MouseWheelBackwardEvent,
                                       this,SLOT(on_qtVtkWidgetMouseWheelBackwardEvent(vtkObject*,ulong,void*,void*)));
            _vtkQtSlotConnect->Connect(_qvtkWidget->GetRenderWindow()->GetInteractor(),vtkCommand::MouseWheelForwardEvent,
                                       this,SLOT(on_qtVtkWidgetMouseWheelForwardEvent(vtkObject*,ulong,void*,void*)));
            _vtkQtSlotConnect->Connect(_qvtkWidget->GetRenderWindow()->GetInteractor(),vtkCommand::RightButtonPressEvent,
                                       this,SLOT(on_qtVtkWiggetMouseRightButtonClicked(vtkObject*,ulong,void*,void*,vtkCommand *)));
            _vtkQtSlotConnect->Connect(_qvtkWidget->GetRenderWindow()->GetInteractor(),vtkCommand::RenderEvent,
                                       this,SLOT(on_qtVtkWiggetRenderEvent(vtkObject*,ulong,void*,void*)));

            myContourWidget->GetImageActorPointPlacer()->SetImageActor(_imageViewer->GetImageActor());
            // myContourWidget->GetImageActorPointPlacer()->SetImageActor(contourActor);
            myContourWidget->SetInteractor(_renderWindowInteractor);
            myContourWidget->SetFollowCursor( true );
            myContourWidget->On();
            myContourWidget->EnabledOn();
            myContourWidget->ProcessEventsOff();

            InitRTRect();
            bInitView=true;
        }

        _imageViewer->GetRenderer()->ResetCamera();
        _imageViewer->GetRenderer()->GetActiveCamera()->Zoom(1.5);
        _imageViewer->Render();

        // 更新数据源信息
        double valuesRange[2];
        _inImageData->GetScalarRange(valuesRange);
        int* dims = _inImageData->GetDimensions();
        double* space =_inImageData->GetSpacing();
        qDebug()<<"预处理后图像信息\n"<<"Dims:"<<dims[0]<<dims[1]<<dims[2];
        qDebug()<<"Spacing:"<<space[0]<<space[1]<<space[2];
        DicomInfo.scalar_min=valuesRange[0];
        DicomInfo.scalar_max=valuesRange[1];
        DicomInfo.width=dims[0];
        DicomInfo.height=dims[1];
        DicomInfo.depth=dims[2];
        DicomInfo.m_iVoxelSize = dims[0]*dims[1]*dims[2];

        qDebug()<<DicomInfo.width<<DicomInfo.height<<DicomInfo.depth;

        DicomInfo.spacing_x=space[0];
        DicomInfo.spacing_y=space[1];
        DicomInfo.spacing_z=5;
//        DicomInfo.spacing_z=space[2];

        _curdata = static_cast<short *>(_inImageData->GetScalarPointer());

        //更新阈值分割
        UpdateLUT(0,0,false);
        // InitThresholdMaskMap();
        // UpdateCtlValueRange(DicomInfo.scalar_min,DicomInfo.scalar_max);

        // InitLimitedRegionMask();

        ui->verticalScrollBar_DicomChange_XY->setEnabled(true);
        ui->verticalScrollBar_DicomChange_XY->setMaximum(_imageViewer->GetSliceMax());
        ui->verticalScrollBar_DicomChange_XY->setMinimum(_imageViewer->GetSliceMin());
        ui->verticalScrollBar_DicomChange_XY->setValue((ui->verticalScrollBar_DicomChange_XY->maximum()+ui->verticalScrollBar_DicomChange_XY->minimum())/2);

        DicomInfo.isSourceRead=true;
        ProcessInfo.isCanThreshold=true;

        PreProcessDicomData();
        isReadingDicom=false;

        //ui->horizontalSlider_Threshold_Min->setEnabled(true);
        //ui->horizontalSlider_Threshold_Min->setStyleSheet(HORIZON_TYPE);

        //ui->horizontalSlider_Threshold_Max->setEnabled(true);
        //ui->horizontalSlider_Threshold_Max->setStyleSheet(HORIZON_TYPE);

        //ui->spinBox_Threshold_Max->setEnabled(true);
        //ui->spinBox_Threshold_Min->setEnabled(true);

        if (!ui->pushButton_input_mask->isEnabled())
        {
            ui->pushButton_input_mask->setEnabled(true);
            ui->pushButton_input_mask->setStyleSheet(SELECTED);
        }




        //计算椭圆模型
        A = ui->spinBox_long_axis->value()/DicomInfo.spacing_x;
        B = ui->spinBox_short_axis->value()/DicomInfo.spacing_y;
        C = ui->spinBox_thick_axis->value()/DicomInfo.spacing_z;


        x0 = DicomInfo.width*0.5;
        y0 = DicomInfo.height*0.5;
        z0 = DicomInfo.depth*0.5;

        xmax = x0+A;
        xmin = x0-A;
        ymax = y0+B;
        ymin = y0-B;
        zmax = z0+C;
        zmin = z0-C;

        // 设置种子点
        ProcessInfo.SeedPoint_X = x0;
        ProcessInfo.SeedPoint_Y = y0;
        ProcessInfo.SeedPoint_Z = z0;
    }
    else
    {

        codec = QTextCodec::codecForName("GBK");
        QString info1Str =  codec->toUnicode("选择一种轮廓生成方式");
        QString info2Str =  codec->toUnicode("请选择轮廓生成方式");

        QMessageBox msgBox(info1Str,
                           info2Str,
                           QMessageBox::Warning,
                           QMessageBox::Yes | QMessageBox::Default,
                           QMessageBox::No | QMessageBox::Escape,
                           QMessageBox::NoButton);


        msgBox.setWindowFlags(Qt::WindowTitleHint|Qt::CustomizeWindowHint);
        msgBox.setGeometry(QRect(1400,400,msgBox.width(),msgBox.height()));
        int ret = msgBox.exec();
        if (ret == QMessageBox::Yes)
        {
            return;
        }
        else
        {
            return;
        }

        return;
    }


    if (ContoursMaskArray==NULL)
    {
        ContoursMaskArray = new uchar[DicomInfo.m_iVoxelSize];
    }

    memset(ContoursMaskArray,0,DicomInfo.m_iVoxelSize*sizeof(uchar));
    for (int i = zmin;i <= zmax;++i)
    {
        for (int j = ymin; j <= ymax;++j)
        {
            for (int k = xmin; k<=xmax; ++k)
            {
                if ((i-z0)*(i-z0)/(C*C)+(j-y0)*(j-y0)/(B*B)+(k-x0)*(k-x0)/(A*A)<=1.0)
                {
                    if (k>=0&&k<DicomInfo.width&&j>=0&&j<DicomInfo.height&&i>=0&&i<DicomInfo.depth)
                    {
                        ContoursMaskArray[k+j*DicomInfo.width+i*DicomInfo.width*DicomInfo.height]=1;
                    }
                    else
                    {
                        m_Nc=false;//判断是否椭圆上所有点都位于图像内

                    }
                }
            }
        }

    }

//    if(!ProcessInfo.isRegionGrow)
//    {
//        InitRegionGrowMask();
//    }
//    else
//    {
//        regionGrowingMapper->RemoveAllInputConnections(0);
//        regionGrowingMapper->SetInputConnection(0,regionGrowingImageImport->GetOutputPort());
//        regionGrowingMapper->Update();
//        _imageViewer->GetRenderWindow()->Render();
//        if(!ProcessInfo.isShowRegionGrowMask)
//        {
//            _imageViewer->GetRenderer()->AddActor(regionGrowingMaskActor);
//            // regionGrowingMaskActor->SetZSlice(_imageViewer->GetSlice());
//            _imageViewer->GetRenderWindow()->Render();
//            ProcessInfo.isShowRegionGrowMask=true;
//        }
//    }

    if (!isContourMaskInited)
    {
       InitContourMask();

    }
    else
    {
        _imageViewer->GetRenderer()->RemoveActor(contourActor);
        contourMapper->RemoveAllInputConnections(0);
        contourMapper->SetInputConnection(0,contourImport->GetOutputPort());
        contourMapper->Update();

        _imageViewer->GetRenderer()->AddActor(contourActor);
        _imageViewer->GetRenderWindow()->Render();

    }

    ProcessInfo.isContourCreated = true;

    ui->pushButton_SurfaceRendering_2->setEnabled(true);
    ui->pushButton_SurfaceRendering_2->setStyleSheet(SELECTED);


}

void MainDialog::on_radioButton_draw_clicked()
{
    if (ui->radioButton_draw->isChecked())
    {
        ui->radioButton_write->setChecked(false);
    }
}

void MainDialog::on_radioButton_write_clicked()
{
    if (ui->radioButton_write->isChecked())
    {
        ui->radioButton_draw->setChecked(false);
    }
}
