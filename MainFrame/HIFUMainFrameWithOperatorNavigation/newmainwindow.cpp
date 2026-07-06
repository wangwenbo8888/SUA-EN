#include "newmainwindow.h"
#include "ui_newmainwindow.h"

#include <QDate>
#include <QTime>
#include <QMessageBox>
#include <QFileDialog>
#include <QTextDocument>
#include <QScreen>
#include <QDesktopWidget>

#include <QTableWidget.h>

//#include "scene3d.h"
#include "hifunavigation.h"
#include "layerscene.h"

#include "histogram.h"

#include "hifu3d.h"
#include "mesh.h"
#include "smooth.h"
#include "smartplan.h"
#include "realtimescene.h"
#include "append.h"
#include "irradiatecore.h"
#include "parametertable.h"
#include <QStandardItemModel>
#include <dragmodel.h>
#include "selftestdlg.h"
//#include "hifuimagealgo.h"


//#include "imagecapture.h"

//#include <QGLWidget>
//#include <QQuickView>
//#include <QQuickItem>
#include <QListWidget>

#include <QPainter>
#include <QtPrintSupport/qprinter.h>

// #include <qnamespace.h>

#include <QMenu>
#include <QAction>
#include <QToolTip>
#include <QQueue>
#include <QDir>
#include <QPair>

#include "stdio.h"
#include "time.h"

#include "regist.h"

#include "dragtable.h"
#include "filesavedialog.h"

#include "irradiateblockplan.h"
#include <tlhelp32.h>
//#include "opencv2/opencv.hpp"
//#include "opencv/cv.h"
//#include "opencv2/core.hpp"
//#include <opencv/cv.h>
//#include <opencv2/opencv.hpp>
//#include <opencv2/core.hpp>
//#include <opencv2/imgproc.hpp>
//#include <opencv2/highgui.hpp>

//using namespace cv;
using namespace std;

//double RESOLUTION = 2.07692;                           // unit: pixel/mm
double RESOLUTION = 2.07692;                           // unit: pixel/mm
double ZERO_DEVIATION = 2.2*RESOLUTION;
double X_OFFSET = 274;                                    // unit: pixel
double Y_OFFSET = 294;
// 换算成140mm应该是多少个像素
double FOCAL_PLANE_140MM = 140 * RESOLUTION+ZERO_DEVIATION;   // unit: pixel/mm
float delta = 7.2;
//double voltage_1;
//double voltage_2;
//double voltage_3;
//double voltage_4;
//double voltage_5;
//double voltage_6;
//double voltage_7;
//double voltage_8;
//double voltage_9;
//double voltage_10;
//double voltage_11;
//double voltage_12;
//double voltage_13;
//double voltage_14;

Q_LOGGING_CATEGORY(NEWMAINWINDOW,"NEWMAINWINDOW")

NewMainWindow::NewMainWindow(SelfTestDlg * selftestdlg,QWidget *parent) :
    _selfTestDlg(selftestdlg),
    QMainWindow(parent),
    ui(new Ui::NewMainWindow),
    _seriesAcquired(false)
{
    ui->setupUi(this);

    ui->horizontalScrollBar_contrast->setVisible(false);
    ui->pushButton->setVisible(false);
    ui->label_status->setVisible(false);

    m_msgBoxFont.setFamily("Microsoft YaHei");
    m_msgBoxFont.setPixelSize(16);

//    getSnapshot();
    qDebug()<<"main ID:" << QThread::currentThreadId();//获取当前线程ID
    setWindowFlags(windowFlags() | Qt::FramelessWindowHint);
    connect(this,SIGNAL(SendSubSysStatus(selfSubSys,bool)),_selfTestDlg,SLOT(updateStatus(selfSubSys,bool)));
    connect(this,SIGNAL(networkcheck()),_selfTestDlg,SLOT(networkreflash()));

//    ui->toolButton_dqa->setEnabled(false);
//    ui->toolButton_dqa->setStyleSheet(NOT_WORK);


    QSettings *settings = new QSettings(SETTINGS_PATH, QSettings::IniFormat);
    RESOLUTION = settings->value("UltrasoundImage/RESOLUTION").toString().toDouble();
    X_OFFSET = settings->value("UltrasoundImage/X_OFFSET").toString().toDouble();
    Y_OFFSET = settings->value("UltrasoundImage/Y_OFFSET").toString().toDouble();
    delta = settings->value("DrawScene/Focus_Delta").toFloat();


//    voltage_1 = settings->value("Voltage/Power22").toString().toDouble();
//    voltage_2 = settings->value("Voltage/Power54").toString().toDouble();
//    voltage_3 = settings->value("Voltage/Power72").toString().toDouble();
//    voltage_4 = settings->value("Voltage/Power90").toString().toDouble();
//    voltage_5 = settings->value("Voltage/Power99").toString().toDouble();
//    voltage_6 = settings->value("Voltage/Power108").toString().toDouble();
//    voltage_7 = settings->value("Voltage/Power117").toString().toDouble();
//    voltage_8 = settings->value("Voltage/Power126").toString().toDouble();
//    voltage_9 = settings->value("Voltage/Power135").toString().toDouble();
//    voltage_10 = settings->value("Voltage/Power144").toString().toDouble();
//    voltage_11 = settings->value("Voltage/Power153").toString().toDouble();
//    voltage_12 = settings->value("Voltage/Power162").toString().toDouble();
//    voltage_13 = settings->value("Voltage/Power171").toString().toDouble();
//    voltage_14 = settings->value("Voltage/Power180").toString().toDouble();


//    getVoltage()
    getsettingvoltage();
//    double vak = voltage_1;

    ui->label_temp2_value->setVisible(false);
    ui->label_temp2_unit->setVisible(false);
    ui->label_deepthmeasure->setVisible(false);
    ui->pushButton_2->setVisible(false);
//    ui->label_time_treatment->setVisible(false);

    ui->label_totalPoint->setStyleSheet(WORK);
    ui->label_totalTime->setStyleSheet(WORK);

    ui->commandLinkButton_water_circle->setEnabled(true);
    ui->commandLinkButton_water_circle->setStyleSheet(SELECTED);

    //ui->pushButton_generate_plan->setEnabled(true);
    //ui->pushButton_generate_plan->setStyleSheet(SELECTED);
    ui->tabWidget_operator->removeTab(2);
    ui->tabWidget_operator->removeTab(1);
    ui->tabWidget_operator->removeTab(0);


    ui->pushButton_block_confirm->setEnabled(true);
    ui->pushButton_block_confirm->setStyleSheet(SELECTED);
//    _SlicesViewer=vtkSmartPointer<vtkImageViewer2>::New();
//    _SlicesRenderWindowInteractor=vtkSmartPointer<vtkRenderWindowInteractor>::New();
    //_qvtkWidget_SlicesView = new QVTKOpenGLWidget(this);
    ui->pushButton_pump1speedup->setEnabled(false);
    ui->pushButton_pump1speedup->setStyleSheet("background-color: rgb(55, 55, 55)");
    ui->pushButton_pump1speeddown->setEnabled(false);
    ui->pushButton_pump1speeddown->setStyleSheet("background-color: rgb(55, 55, 55)");
    ui->pushButton_pump2speedup->setEnabled(false);
    ui->pushButton_pump2speedup->setStyleSheet("background-color: rgb(55, 55, 55)");
    ui->pushButton_pump2speeddown->setEnabled(false);
    ui->pushButton_pump2speeddown->setStyleSheet("background-color: rgb(55, 55, 55)");

    Heartthread = new heartthread;
    Heartthread_thread = new QThread;
    Heartthread->moveToThread(Heartthread_thread);
    connect(Heartthread_thread,SIGNAL(started()),Heartthread,SLOT(_start()));
    connect(Heartthread_thread,SIGNAL(finished()),Heartthread,SLOT(deleteLater()));
//    connect(Heartthread,SIGNAL(statusUpdated(DataFlag)),this,SLOT(on_status_updated(DataFlag)),Qt::QueuedConnection);
    connect(Heartthread,SIGNAL(GateWayFailed()),this,SLOT(GateWayConnectFailed()));

//    _pScene3d = NULL;
    _pHifu3d = NULL;
    _pMesh=NULL;
    _pCurrentModel = NULL;
    _pBlockModel = NULL;
    _pMessage = NULL;
    _location = NULL;
    _core = NULL;
    _pNavigation = NULL;
    _para = NULL;
    _printDialog=NULL;
    _realtimeScene = NULL;
    _newScene3d=NULL;
    _planScene3d=NULL;
    _hifuimagealgo = NULL;
    // _table = NULL;
    _refreshRealAngle = NULL;
    _destAngle = -100;

    _grabber = NULL;
    _pWidget = NULL;

    // Current Plan表格右键菜单对应的操作
    _tableMenu = NULL;
    _tableCopy = NULL;
    _tableDelete = NULL;
    _tableStop = NULL;
    _tableContinue = NULL;

    // Live Image窗口对应的右键菜单及操作
    _realTimeMenu =    NULL;
    _realTimeZoomOut = NULL;
    _realTimeZoomIn =  NULL;

    _realTimeShowPath = NULL;      // Live Image窗口Show Acoustic Window
    _realTimeHidePath = NULL;      // Live Image窗口Hide Acoustic Window

    _realTimeShowFocus = NULL;     // Live Image窗口Show Natural Focus
    _realTimeHideFocus = NULL;     // Live Image窗口Hide Natural Focus

    _realTimeShowRange = NULL;
    _realTimeHideRange = NULL;

    _realTimeResume =  NULL;
    _realTimeChange =  NULL;

    _targetMenu = NULL;
    _targetShowElement = NULL;
    _targetHideElement = NULL;
    _targetResumeImages = NULL;
    _targetZoomOut = NULL;         // Zoom In
    _targetZoomIn = NULL;          // Zoom Out
    _targetShowDeleted = NULL;     // 显示Delete图像
    _targetHideDeleted = NULL;     // 隐藏Delete图像

    _targetSelectInvert   = NULL;  // 反选
    _targetSmartSelect    = NULL;  // 自动Selectfibroid内的sonication cell
    _targetResumeDelected = NULL;  // RestoreDelete的sonication cell

    _targetShowContours = NULL;    // 显示fibroid轮廓
    _targetHideContours = NULL;    // 隐藏fibroid轮廓

    _targetShowDraw = NULL;         // Show Traced Contour
    _targetHideDraw = NULL;         // Hide Traced Contour

	_targetDrag = NULL;             //手动Edit轮廓
	
    
    _targetDragConfirm = NULL;      //Confirm拖动轮廓

    _layerMenu = NULL;
    _layerShowElement = NULL;
    _layerHideElement = NULL;
    _layerZoomOut = NULL;
    _layerZoomIn = NULL;
    _layerShowDeleted = NULL;     // 显示Delete图像
    _layerHideDeleted = NULL;     // 隐藏Delete图像

    _layerSelectInvert   = NULL;  // 反选
    _layerResumeDelected = NULL;  // RestoreDelete点

    _layerResumeImages  = NULL;

    _researchMenu = NULL;
    _researchShow = NULL;

    _append = NULL;
    _hifuHttpServer=NULL;

    _quitform = new quitform();
    _drainTankForm = new DrainTankForm();
    // Default为从内向外sonication
    m_eModel = Model_inner_to_outer;
    m_f3dDelta = 0.0f;

    _viewer2DInfo.scal = 1.0;    // 图像缩放比例
    _viewer2DInfo.rect = QRectF(0,0,574,574);

    _layerViewer2DInfo.scal = 1.0;
    _layerViewer2DInfo.rect = QRectF(0,0,200,200);
    _pChoseimage = new Choseimage(this);    // Generate任意旋转成像面窗口
    _pHeaderControl = new HeaderControl(this);    // GenerateProbe控制窗口
    connect(this,SIGNAL(updateHeadPos(float,float,float)),_pHeaderControl,SLOT(getHeadPos(float,float,float)));

    // 设置子系统Status均为故障
    for (int i = 0; i < 5; ++i)
    {
        _subSysStatus[i] = false;
    }
    _currentImages = 0;
    m_networkservice = 0;
    posZ=0;posA=0;posL=0;posF=0;
    m_bPump1Open = false;
    m_bPump2Open = false;
    connectflag = false;
    powerampready = true;
    waterready = true;
    networkready = true;
    locationready = true;
    appendready = true;
    flag1=false;
    BedZero=true;
    imagesignal = true;
    Auto_WaterLock = false;
    Image_WaterLock = false;

    init();

    // 只能放在前面，放在后面Patient information传不到报告里面去
    _printDialog = new PrintDialogClinic();
    //新Print窗口
    QObject::connect(_printDialog,SIGNAL(showRegistWindow()),this,SLOT(showRegistWindow()));
    QObject::connect(this,SIGNAL(setPatientInfo(QVariant,QVariant,QVariant,QVariant,QVariant,QVariant,QVariant)),
                     _printDialog,SLOT(setPatientInfo(QVariant,QVariant,QVariant,QVariant,QVariant,QVariant,QVariant)));
    QObject::connect(this,SIGNAL(setTerapyInfo(QVariant,QVariant,QVariant,QVariant,QVariant,QVariant,QVariant,QVariant,QVariant)),
                     _printDialog,SLOT(setTerapyInfo(QVariant,QVariant,QVariant,QVariant,QVariant,QVariant,QVariant,QVariant,QVariant)));
    QObject::connect(this,SIGNAL(saveReport()),_printDialog,SLOT(saveReport()));

    // 设置Treatment进度为0
    ui->progressBar_cooling_time->setValue(0);
    ui->label_remain_cooling_time_value->setText("0");

    // 设置当前TreatmentPlantableview右键菜单可用
    ui->tableView_current_plan->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->graphicsView_target_scene->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->graphicsView_realtime_image->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->graphicsView_treatment_layer->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->graphicsView_layer_plan->setContextMenuPolicy(Qt::CustomContextMenu);

    // 设置科研对应的右键菜单可用
    ui->label_target_image->setContextMenuPolicy(Qt::CustomContextMenu);

    // 设置单击选中一行
    ui->tableView_current_plan->setSelectionBehavior(QAbstractItemView::SelectRows);
    // ui->tableView_current_plan->horizontalHeader()->setSectionsMovable(true);

    ui->label_cooling_time_value->setText(QString("%1").arg(ui->horizontalSlider_cooling_time->value()));

    ui->tableView_current_plan->setMouseTracking(true);
    connect(ui->tableView_current_plan, SIGNAL(entered(QModelIndex)),
            this,SLOT(showTableViewToolTip(QModelIndex)));

    setWholePlanControlDisable();
    setToolTip();

    ui->toolButton_change_image_plane->setStyleSheet(SELECTED);
    ui->toolButton_save_image_2->setStyleSheet(SELECTED);
    ui->toolButton_save_video_2->setStyleSheet(SELECTED);
    ui->toolButton_header_zero->setStyleSheet(SELECTED);

    ui->horizontalSlider_left_right->setStyleSheet(HORIZON_TYPE);
    ui->horizontalSlider_forward_backward->setStyleSheet(HORIZON_TYPE);
    ui->horizontalSlider_up_down->setStyleSheet(HORIZON_TYPE);
    ui->horizontalSlider_left_right->setEnabled(false);
    ui->horizontalSlider_forward_backward->setEnabled(false);
    ui->horizontalSlider_up_down->setEnabled(false);

    ui->pushButton_histogram->setEnabled(false);
    ui->pushButton_histogram->setStyleSheet(NOT_WORK);

    ui->pushButton_measure_length->setEnabled(false);
    ui->pushButton_measure_length->setStyleSheet(NOT_WORK);

    ui->pushButton_cancel->setEnabled(false);
    ui->pushButton_cancel->setStyleSheet(NOT_WORK);

    ui->pushButton_acquire->setStyleSheet(SELECTED);

    ui->pushButton_draw->setEnabled(false);
    ui->pushButton_draw->setStyleSheet(NOT_WORK);

    ui->pushButton_drag->setEnabled(false);
    ui->pushButton_drag->setStyleSheet(NOT_WORK);

    ui->pushButton_draw_confirm->setEnabled(false);
    ui->pushButton_draw_confirm->setStyleSheet(NOT_WORK);

    ui->pushButton_drag_confirm->setEnabled(false);
    ui->pushButton_drag_confirm->setStyleSheet(NOT_WORK);

    ui->pushButton_set_order->setEnabled(false);
    ui->pushButton_set_order->setStyleSheet(NOT_WORK);

    ui->pushButton_confirm_order->setEnabled(false);
    ui->pushButton_confirm_order->setStyleSheet(NOT_WORK);

    // 设置进度条为圆角，底色为白杏仁
    ui->progressBar_cooling_time->setStyleSheet("background-color: rgb(255, 235, 205);border-radius:5px;");

    // Delete以前的TreatmentPlan，避免异常Exit造成异常
    deletePlanFold();

    _currentSpotId = "";
    _currentSpotIdBorn = "";
    _targetScene = NULL;
    _layerScene = NULL;
    _planLayerScene = NULL;

    _treatmentTimeRemain = NULL;

    _pixData = NULL;
    _tableList = NULL;

    stat._bDrawable = false;
    stat._beamVisiable =false;
    stat._bMeasureable =false;
    stat._bTargetDrawable = false;
    stat._transducerVisiable = false;
    m_bDrawOutline = false;
    _isIrradiating = false;
    _contoursVisiable = true;
    emit setIrradiating(_isIrradiating);

    _isOnSaveVideo = false;
    _irradiatePlan = NULL;
    _pBlockPlan = NULL;

    _newScene3d=new NewScene3D(ui->verticalLayout_NewScene3D,this);
    _planScene3d=new PlanScene3D(ui->verticalLayout_Scene3DPlan,this);
    initBlocksTable();

    _planLayerScene = new LayerScene(this);
    connect(this,SIGNAL(setBlockColor(QString,QColor)),_planLayerScene,SLOT(setBlockColor(QString,QColor)));
    connect(this,SIGNAL(setCurrentBlockId(QString)),_planLayerScene,SLOT(setCurrentBlockId(QString)));
    connect(_planLayerScene,SIGNAL(emitSetConfirmSetOrderEnable()),this,SLOT(setConfirmSetOrderEnable()));

    connect(this,SIGNAL(setBlockColor(QString,QColor)),_planScene3d,SLOT(setBlockColor(QString,QColor)));
    m_iRotate = 0;

    _playlist = new QMediaPlaylist;
    // _playlist->addMedia(QUrl::fromLocalFile("../resources/alarm.mp3"));
    _playlist->setCurrentIndex(1);
    _playlist->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);

//    _pMediaPlayer = new QMediaPlayer;
//    _pMediaPlayer->setPlaylist(_playlist);
    // _mediaPlayer.setVolume(30);
    // _mediaPlayer.play();

    if (_pNavigation==NULL)
    {
        _pNavigation = new HIFUNavigation;

        connect(_pNavigation,SIGNAL(sendQImage(QImage,int)),
                this,SLOT(receiveQImage(QImage,int)));

        connect(_pNavigation,SIGNAL(sendOuter(QImage,int)),
                this,SLOT(receiveOuter(QImage,int)));

        connect(_pNavigation,SIGNAL(sendContour(QImage,int)),
                this,SLOT(receiveContour(QImage,int)));

        connect(_pNavigation,SIGNAL(sendSearchRadius(int)),
                this,SLOT(setSearchRadius(int)));

        connect(this,SIGNAL(sendGetContour()),
                _pNavigation,SLOT(receiveGetContour()));

        connect(this,SIGNAL(sendPatientOrientation(int)),
                _pNavigation,SLOT(receivePatientOrientation(int)));
        qDebug()<<"navi窗口绑定前";

        ui->verticalLayout_Navigation->addWidget(_pNavigation->GetWindowWidget());

       qDebug()<<"navi窗口绑定后";
    }
    connect(qApp,SIGNAL(aboutToQuit()),this,SLOT(QuitBeforShow()));
}

void NewMainWindow::on_Drain_Water_Finished()
{

    if (_pChoseimage!=NULL)
    {
        _pChoseimage->accept();    // DeleteSelect成像面窗口
        delete _pChoseimage;
        _pChoseimage = NULL;
    }

    if (_pHeaderControl!=NULL)
    {
        _pHeaderControl->accept();
        delete _pHeaderControl;
        _pHeaderControl = NULL;
    }

//    if(_location!=NULL)
//    {
//        delete _location;
//        _location = NULL;
//    }

    emit MainClose();
    _drainTankForm->close();
    this->close();
}

void NewMainWindow::showTableViewToolTip(const QModelIndex &index)
{
    if (!index.isValid())
    {
        qDebug() << "Invalid index";
        return;
    }

    if (index.column()==2)
    {
       QToolTip::showText(QCursor::pos(), index.data().toString() + "°");
    }
    else if (index.column()==3)
    {
        QToolTip::showText(QCursor::pos(), index.data().toString() + "W");
    }
    else if (index.column()==4)
    {
        QToolTip::showText(QCursor::pos(), index.data().toString() + "pcs");
    }
    else
    {
        QToolTip::showText(QCursor::pos(), index.data().toString());
    }
}

void NewMainWindow::sendPatientInfo(QVariant id,QVariant name,
                                    QVariant age,QVariant mobile,
                                    QVariant x, QVariant y, QVariant z)
{
    emit setPatientInfo(id,name,age,mobile,x,y,z);

    _totalSpot = 0;
    _seriesSpot = 0;
    _recSpot = 0;
    treatedSpot = 0;
    _totalTime = 0;
}

void NewMainWindow::setUserModel()
{
    // 从配置文件中读取用户模式
    QSettings *settings = new QSettings(SETTINGS_PATH, QSettings::IniFormat);
    int model = settings->value("MainFrame/UserModel").toString().toUInt();
    m_f3dDelta = settings->value("MainFrame/3D_delta").toString().toFloat();
//    RESOLUTION = settings->value("UltrasoundImage/RESOLUTION").toString().toDouble();
//    X_OFFSET = settings->value("UltrasoundImage/X_OFFSET").toString().toUInt();
//    Y_OFFSET = settings->value("UltrasoundImage/Y_OFFSET").toString().toUInt();
//    qDebug()<<"resolution值："<<RESOLUTION;

    qCDebug(SELFCHECK()) << SELFCHECK().categoryName()
                         << "user model is "<< model
                         << TRACE_CMH();

    qCDebug(SELFCHECK()) << SELFCHECK().categoryName()
                         << "3d delta is "<< m_f3dDelta
                         << TRACE_CMH();
    switch (model)
    {
        case 0:
            ui->comboBox_irradiate_model->addItem(QWidget::tr("Outside In"));
            break;

        case 1:
            ui->comboBox_irradiate_model->addItem(QWidget::tr("Inside Out"));
//            ui->comboBox_irradiate_model->addItem(QWidget::tr("Outside In"));
            ui->horizontalSlider_cooling_time->setMaximum(180);
            break;
        case 2:
            ui->pushButton->setVisible(true);
    }

    delete settings;


}

NewMainWindow::~NewMainWindow()
{
    qDebug()<<"Enter deconstruction function !";
    clearTargetScenes();
    clearSliceScenes();
    // 关闭HIFUService
    // closeHIFUService();

    deletePlanFold();

    delete ui;
//    if (_water!=NULL)
//    {
//        delete _water;
//        _water = NULL;
//    }
//    if (_realtimeScene!=NULL)
//    {
//        delete _realtimeScene;
//        _realtimeScene = NULL;
//    }

//    if (_pScene3d!=NULL)
//    {
//        delete _pScene3d;
//        _pScene3d = NULL;
//    }

    if (_pMesh!=NULL)
    {
        delete _pMesh;
        _pMesh=NULL;
    }

//    if (_table!=NULL)
//    {
//        delete _table;
//        _table=NULL;
//    }

    if (_pHifu3d!=NULL)
    {
        delete _pHifu3d;
        _pHifu3d = NULL;
    }

    if (_pCurrentModel!=NULL)
    {
        delete _pCurrentModel;
        _pCurrentModel = NULL;
    }

    if (_pBlockModel!=NULL)
    {
        delete _pBlockModel;
        _pBlockModel = NULL;
    }

//    if (_pTotalModel!=NULL)
//    {
//        delete _pTotalModel;
//        _pTotalModel = NULL;
//    }

    if (_pNavigation!=NULL)
    {
        delete _pNavigation;
        _pNavigation = NULL;
    }

    if (_tableMenu!=NULL)
    {
        delete _tableMenu;
        _tableMenu = NULL;
    }

    if (_realTimeMenu!=NULL)
    {
        delete _realTimeMenu;
        _realTimeMenu = NULL;
    }

    if (_targetMenu!=NULL)
    {
        delete _targetMenu;
        _targetMenu = NULL;
    }

    if (_pMessage!=NULL)
    {
        delete _pMessage;
        _pMessage = NULL;
    }

    if (_layerMenu!=NULL)
    {
        delete _layerMenu;
        _layerMenu = NULL;
    }

    if (_researchMenu!=NULL)
    {
        delete _researchMenu;
        _researchMenu = NULL;
    }

    if (_pixData!=NULL)
    {
        delete []_pixData;
        _pixData = NULL;
    }

    if (_append != NULL)
    {
        delete _append;
        _append = NULL;
    }

//    if (_pMediaPlayer != NULL)
//    {
//        delete _pMediaPlayer;
//        _pMediaPlayer=NULL;
//    }

    if (_playlist!=NULL)
    {
        delete _playlist;
        _playlist=NULL;
    }

    if (_printDialog!=NULL)
    {
        delete _printDialog;
        _printDialog = NULL;
    }

    if (_core!=NULL)
    {
        delete _core;
        _core = NULL;
    }

    if (_para!=NULL)
    {
        delete _para;
        _para = NULL;
    }

    if (_grabber!=NULL)
    {
        delete _grabber;
        _grabber = NULL;
    }

    if (_pChoseimage!=NULL)
    {
        delete _pChoseimage;
        _pChoseimage = NULL;
    }

    if (_pHeaderControl!=NULL)
    {
        delete _pHeaderControl;
        _pHeaderControl = NULL;
    }

    if (_location!=NULL)
    {
//        _pSLocation=NULL;
        delete _location;
        _location = NULL;
    }
    if (_pWidget!=NULL)
    {
        delete _pWidget;
        _pWidget = NULL;
    }

//    if(_hifuHttpServer!=NULL)
//    {
//        delete _hifuHttpServer;
//        _hifuHttpServer = NULL;
//    }

    if (_planLayerScene!=NULL)
    {
        delete _planLayerScene;
        _planLayerScene = NULL;
    }

    if (_pBlockPlan!=NULL)
    {
        delete _pBlockPlan;
        _pBlockPlan = NULL;
    }

    if (_planScene3d!=NULL)
    {
        delete _planScene3d;
        _planScene3d=NULL;
    }

    if(_dupumpControl!=NULL)
    {
        delete _dupumpControl;
        _dupumpControl=NULL;
    }

    if (_quitform!=NULL)
    {
        delete _quitform;
        _quitform=NULL;
    }

    if (_drainTankForm!=NULL)
    {
        delete _drainTankForm;
        _drainTankForm=NULL;
    }

    qDebug(NEWMAINWINDOW())<<NEWMAINWINDOW().categoryName()
                           <<"main destruction success !"
                           <<TRACE_CMH();
}

void NewMainWindow::closeHIFUService()
{
    QProcess proc;
    proc.execute("taskkill",QStringList()<<"-im"<<"HIFUService.exe"<<"-f");
}

void NewMainWindow::setSearchRadius(int radius)
{
    m_movPos.clear();
    _contoursMoveMap.clear();
    m_iSearchRadius = radius;
}

void NewMainWindow::setToolTip()
{
    ui->checkBox_density_high->setToolTip("1 mm diameter");
    ui->checkBox_density_middle->setToolTip("1.4 mm diameter");
    ui->checkBox_density_low->setToolTip("1.8 mm diameter");

    ui->checkBox_layer_down->setToolTip("10 mm below natural focus");
    ui->checkBox_layer_middle->setToolTip("Same depth as natural focus");
    ui->checkBox_layer_up->setToolTip("10 mm above natural focus");

    ui->checkBox_circle_one->setToolTip("First ring around natural focus");
    ui->checkBox_circle_two->setToolTip("Second ring around natural focus");
    ui->checkBox_circle_three->setToolTip("Third ring around natural focus");
    ui->checkBox_circle_four->setToolTip("Fourth ring around natural focus");
    ui->checkBox_circle_five->setToolTip("Fifth ring around natural focus");

    ui->pushButton_start_treat->setToolTip("Start sonication for the selected point in the current plan");
    ui->pushButton_continue->setToolTip("Move to the next treatment plane for sonication");

    ui->pushButton_replan->setToolTip("Replan without acquiring images");
    ui->pushButton_modify_para->setToolTip("Keep sonication points and change parameters only");
}

// 清理Large Fibroid TreatmentPlan环境
void NewMainWindow::clearBigMyoma()
{
    if (_planLayerScene!=NULL)
    {
        _planLayerScene->clearAll();
    }

    if(_planScene3d!=NULL&&_planScene3d->isInit)
    {
        _planScene3d->ClearSpot();
        _planScene3d->ClearTumor();
    }

    if (_pBlockModel!=NULL)
    {
        _pBlockModel->clear();
    }

    initBlocksTable();
}

void NewMainWindow::sendPatientInfo(QString id,QString fname,
                                    QString age,QString mobile,
                                    QString x,QString y,QString z,int PatientOrientation)
{
    PID = id;
    _startTime = QTime::currentTime().toString("hh:mm:ss");
    startTime = QDateTime::currentMSecsSinceEpoch();
    _totalSpot = 0;
    _seriesSpot = 0;
    _totalTime = 0;
    _recSpot = 0;
    treatedSpot = 0;

    // 设置当前TreatmentPatient name
    ui->label_patient_text->setText(fname);
    ui->label_patient_text->setStyleSheet(WORK);

    ui->label_time_remain->setText(" ");
    ui->label_time_remain->setStyleSheet(WORK);

    ui->label_time_treatment->setText(" ");
    ui->label_time_treatment->setStyleSheet(WORK);

    m_iPatientOrientation=PatientOrientation;
    sendPatientOrientation(m_iPatientOrientation);

    clearBigMyoma();

    if(_pNavigation!=NULL)
    {
        _pNavigation->ClearData();
    }

    // Treatment盘归零
    if (!_location->setPos(AXIS_L,0))  // 左右
    {
        ui->label_status->setText("Axis left right zero error !");
    }

    if (!_location->setPos(AXIS_F,0))  // 前后
    {
        ui->label_status->setText("Axis front back zero error !");
    }

    ui->pushButton_draw->setEnabled(false);
    ui->pushButton_draw->setStyleSheet(NOT_WORK);

    ui->pushButton_drag->setEnabled(false);
    ui->pushButton_drag->setStyleSheet(NOT_WORK);

    ui->pushButton_draw_confirm->setEnabled(false);
    ui->pushButton_draw_confirm->setStyleSheet(NOT_WORK);

    ui->pushButton_drag_confirm->setEnabled(false);
    ui->pushButton_drag_confirm->setStyleSheet(NOT_WORK);

    ui->pushButton_block_confirm->setEnabled(false);
    ui->pushButton_block_confirm->setStyleSheet(NOT_WORK);

    outputTreatmentLog(id,fname,age,mobile,x,y,z);
    emit setPatientInfo(id,fname,age,mobile,x,y,z);
}

void NewMainWindow::initSmartPlanPanel()
{
    ui->checkBox_layer_up->setChecked(true);
    ui->checkBox_layer_middle->setChecked(true);
    ui->checkBox_layer_down->setChecked(true);
    ui->checkBox_circle_one->setChecked(true);
    ui->checkBox_circle_two->setChecked(true);
    ui->checkBox_circle_three->setChecked(true);
    ui->checkBox_circle_four->setChecked(true);
    ui->checkBox_circle_five->setChecked(true);
}

void NewMainWindow::changeView2D(viewer2D_info info)
{
    _viewer2DInfo.scal = info.scal;
    _viewer2DInfo.rect = info.rect;
}

void NewMainWindow::changeLayerViewer(viewer2D_info info)
{
    _layerViewer2DInfo.scal = info.scal;
    _layerViewer2DInfo.rect = info.rect;
}

void NewMainWindow::initReserarchMenu()
{
    if (_researchMenu==NULL)
    {
        _researchMenu = new QMenu(ui->label_target_image);
        _researchMenu->setStyleSheet("background-color: rgb(55, 55, 55);");
    }

    if (_researchShow==NULL)
    {
        _researchShow = _researchMenu->addAction("Show Research Dialog");
        // connect(_researchShow,SIGNAL(triggered(bool)),this,SLOT());
    }
}

void NewMainWindow::initLayerMenu()
{
    if (_layerMenu==NULL)
    {
        _layerMenu = new QMenu(ui->graphicsView_treatment_layer);
        _layerMenu->setStyleSheet("background-color: rgb(55, 55, 55);");
    }

    if (_layerShowElement==NULL)
    {
        _layerShowElement = _layerMenu->addAction("Show Sonication Cells");
        connect(_layerShowElement,SIGNAL(triggered(bool)),this,SLOT(layerShowElement()));
    }

    if (_layerHideElement==NULL)
    {
        _layerHideElement = _layerMenu->addAction("Hide Sonication Cells");
        connect(_layerHideElement,SIGNAL(triggered(bool)),this,SLOT(layerHideElement()));
    }

    if (_layerZoomOut==NULL)
    {
        _layerZoomOut = _layerMenu->addAction("Zoom In");
        connect(_layerZoomOut,SIGNAL(triggered(bool)),this,SLOT(layerZoomOut()));
    }

    if (_layerZoomIn==NULL)
    {
        _layerZoomIn = _layerMenu->addAction("Zoom Out");
        connect(_layerZoomIn,SIGNAL(triggered(bool)),this,SLOT(layerZoomIn()));
    }

    if (_layerShowDeleted == NULL)     // 显示Delete图像
    {
        _layerShowDeleted = _layerMenu->addAction("Show Deleted Cells");
        connect(_layerShowDeleted,SIGNAL(triggered(bool)),this,SLOT(layerShowDeleted()));
    }

    if (_layerHideDeleted == NULL)     // 隐藏Delete图像
    {
        _layerHideDeleted = _layerMenu->addAction("Hide Deleted Cells");
        connect(_layerHideDeleted,SIGNAL(triggered(bool)),this,SLOT(layerHideDeleted()));
    }

    if (_layerSelectInvert == NULL)    // 反选
    {
        _layerSelectInvert = _layerMenu->addAction("Invert Sonication Point Selection");
        connect(_layerSelectInvert,SIGNAL(triggered(bool)),this,SLOT(layerSelectInvert()));
    }

    if (_layerResumeDelected == NULL)
    {
        _layerResumeDelected = _layerMenu->addAction("Restore Deleted Cells");
        connect(_layerResumeDelected,SIGNAL(triggered(bool)),this,SLOT(layerResumeDeleted()));
    }

    if (_layerResumeImages==NULL)
    {
        _layerResumeImages = _layerMenu->addAction("Reset Image Size");
        connect(_layerResumeImages,SIGNAL(triggered(bool)),this,SLOT(layerResumeImages()));
    }
}

void NewMainWindow::layerShowElement()     // layer窗口Show Sonication Cells
{
    if (_layerScene!=NULL)
    {
        _layerScene->showElement();
    }
}

void NewMainWindow::layerHideElement()     // layer窗口Hide Sonication Cells
{
    if (_layerScene!=NULL)
    {
        _layerScene->hideElement();
    }
}

void NewMainWindow::layerZoomOut()         // layer窗口Zoom in图像
{
    if (_layerScene!=NULL)
    {
        _layerScene->zoomOut();
    }
}

void NewMainWindow::layerZoomIn()          // layer窗口Zoom out图像
{
    if (_layerScene!=NULL)
    {
        _layerScene->zoomIn();
    }
}

void NewMainWindow::layerResumeImages()    // layer窗口Reset to Original Image Size
{
    if (_layerScene!=NULL)
    {
        _layerViewer2DInfo.scal = 1.0;
        _layerViewer2DInfo.rect = QRectF(0,0,200,200);
        _layerScene->setScal(1.0);
        _layerScene->setSceneRect(0,0,200,200);
    }
}

// sonication开始后Delete不必要的右键菜单选项
void NewMainWindow::removeUnavailableMenu()
{
    _targetMenu->removeAction(_targetHideDeleted);
    _targetHideDeleted = NULL;
    _targetMenu->removeAction(_targetResumeDelected);
    _targetResumeDelected = NULL;
    _targetMenu->removeAction(_targetShowDeleted);
    _targetShowDeleted = NULL;
    _targetMenu->removeAction(_targetSelectInvert);
    _targetSelectInvert = NULL;
    _targetMenu->removeAction(_targetSmartSelect);
    _targetSmartSelect = NULL;

    _layerMenu->removeAction(_layerHideDeleted);
    _layerHideDeleted = NULL;
    _layerMenu->removeAction(_layerResumeDelected);
    _layerResumeDelected = NULL;
    _layerMenu->removeAction(_layerSelectInvert);
    _layerSelectInvert = NULL;
    _layerMenu->removeAction(_layerShowDeleted);
    _layerShowDeleted = NULL;

}

void NewMainWindow::inintBlockPlan()
{
    if (_pBlockPlan==NULL)
    {
        _pBlockPlan = new BlockPlan;
    }
}

void NewMainWindow::initIrradiatePlan()
{
    int pulseCount = ui->horizontalSlider_pulse_number->value();
    int pulseOff = ui->horizontalSlider_off_time->value();
    int pulseOn = ui->horizontalSlider_on_time->value();
    int coolingTime = ui->horizontalSlider_cooling_time->value();
    double voltage = getVoltage(ui->horizontalSlider_power->value());

    if (_irradiatePlan!=NULL)    // Delete旧Plan
    {
        delete _irradiatePlan;
        _irradiatePlan = NULL;
    }

    QQueue<double> x;
    QQueue<double> y;
    QQueue<double> z;
    QQueue<Circle> circles;
    QQueue<QString> idBorn;

    _3DStatus.clear();
    QMap<int,DrawScene*>::iterator iter = _targetSceneMap.begin();
    for (; iter != _targetSceneMap.end(); ++iter)
    {
        QQueue<double> xTemp = iter.value()->getFocusX();
        QQueue<double> yTemp = iter.value()->getFocusY();
        QQueue<double> zTemp = iter.value()->getFocusZ();
        QQueue<Circle> circleTemp = iter.value()->getCircles();
        QQueue<QString> idBornTemp = iter.value()->getIdBorn();
        for (int i = 0; i < xTemp.size();++i)
        {
            _3DStatus.insert(idBornTemp[i],Spot_3d_Status(xTemp[i],yTemp[i],zTemp[i],UNIRADIATE_COLOR));
            x << xTemp[i];
            y << yTemp[i];
            z << zTemp[i];
            circles<<circleTemp[i];
            idBorn << idBornTemp[i];
        }
    }

    m_totalTime.spots = idBorn.size();
    m_totalTime.spotsRemain = idBorn.size();

    m_totalTime.onTime = pulseOn;
    m_totalTime.offTime = pulseOff;
    m_totalTime.pulseNumber = pulseCount;
    m_totalTime.coolingTime = coolingTime;
    m_totalTime.delayTime = 5;
    refreshRemainTime();

    QQueue<int> pulseCounts;
    QQueue<double> voltages;
    QQueue<int> pulsesOn;
    QQueue<int> pulsesOff;
    QQueue<int> coolingTimes;
    int size = z.size();
    if (ui->comboBox_part->currentText()=="Custom Mode")
//    if (ui->comboBox_part->currentText()!="Myometrium")
    {
        for (int i = 0; i < size; ++i)
        {
            pulseCounts.push_back(pulseCount);
            voltages.push_back(voltage);
            pulsesOn.push_back(pulseOn);
            pulsesOff.push_back(pulseOff);
            coolingTimes.push_back(coolingTime);
        }
    }
    else
    {
        if (_para==NULL)
        {
            return;
        }

        int deep = ui->comboBox_deepth->currentText().toInt();
        Element ele;
        int deltaVol;
        int deltaOnTime;
        if (_para->getElement(deep,ele))
        {
            deltaVol = getVoltage(ui->horizontalSlider_power->value()) - ele.voltage;
            deltaOnTime = ui->horizontalSlider_on_time->value() - ele.on_time;
        }
        else
        {
            deltaVol = 0;
            deltaOnTime = 0;
        }

        for (int i = 0;i < size; ++i)
        {
            if (z[i]>0.145&&z[i]<0.155)
            {
                if (_para->getElement(deep+1,ele))
                {
                    // LowerTreatment平面
                    voltages.push_back(ele.voltage+deltaVol);
                    pulsesOn.push_back(ele.on_time+deltaOnTime);
                    pulsesOff.push_back(ele.off_time);
                    pulseCounts.push_back(ele.pulse_number);
                    coolingTimes.push_back(ele.cooling_time);
                }
                else
                {
                    qCDebug(NEWMAINWINDOW()) << "get deep para failed "<< deep + 1
                                             << TRACE_CMH();

                    pulseCounts.push_back(pulseCount);
                    voltages.push_back(voltage+deltaVol);
                    pulsesOn.push_back(pulseOn+deltaOnTime);
                    pulsesOff.push_back(pulseOff);
                    coolingTimes.push_back(coolingTime);
                }
            }
            else if (z[i]<0.145 && z[i]>0.135)
            {
                // MiddleTreatment平面
                pulseCounts.push_back(pulseCount);
                voltages.push_back(voltage);
                pulsesOn.push_back(pulseOn);
                pulsesOff.push_back(pulseOff);
                coolingTimes.push_back(coolingTime);

            }
            else if(z[i]<0.135 && z[i]>0.125)
            {
                // UpperTreatment平面
                Element ele;
                if (_para->getElement(deep-1,ele))
                {
                    // LowerTreatment平面
                    voltages.push_back(ele.voltage+deltaVol);
                    pulsesOn.push_back(ele.on_time+deltaOnTime);
                    pulsesOff.push_back(ele.off_time);
                    pulseCounts.push_back(ele.pulse_number);
                    coolingTimes.push_back(ele.cooling_time);
                }
                else
                {
                    qCDebug(NEWMAINWINDOW()) << "get deep para failed "<< deep - 1
                                             << TRACE_CMH();

                    pulseCounts.push_back(pulseCount);
                    voltages.push_back(voltage+deltaVol);
                    pulsesOn.push_back(pulseOn+deltaOnTime);
                    pulsesOff.push_back(pulseOff);
                    coolingTimes.push_back(coolingTime);
                }
            }

        }
    }

    _irradiatePlan = new IrradiatePlan(1,pulseCounts,x,y,z,circles,idBorn,
                                       voltages,pulsesOn,pulsesOff,coolingTimes,m_eModel);
    // 同步各项数值更改
//    connect(_pCurrentModel,SIGNAL(itemChanged(QStandardItem*)),
//            _irradiatePlan,SLOT(itemChanged(QStandardItem*)));

    if(_core!=NULL)
    {
        planrecount++;
        qDebug()<<"planrecount======="<<planrecount;
        _core->SetDisPlayPlan(_irradiatePlan);
    }
    connect(_pCurrentModel,SIGNAL(refreshDisplayPlan(QVector<QString>)),
            _irradiatePlan,SLOT(refreshDisplayPlan(QVector<QString>)));

    connect(this,SIGNAL(copySpots(QVector<QString>&)),_irradiatePlan,SLOT(copySpots(QVector<QString>&)));
    connect(this,SIGNAL(deleteSpots(QVector<QString>&)),_irradiatePlan,SLOT(deleteSpots(QVector<QString>&)));

    QMap<QString,QVector<double> > allSpotPos;
    QQueue<Irradiate_Spot> allSpot=_irradiatePlan->getTotalPlan();
    for(int i=0;i<allSpot.size();++i)
    {
        QString idBound=allSpot[i].idBorn;
        QVector<double> vpos;
        vpos.push_back(allSpot[i].position.x);
        vpos.push_back(allSpot[i].position.y);
        vpos.push_back(allSpot[i].position.z);
        allSpotPos[idBound]=vpos;
        // qDebug()<<"Spot Pos"<<vpos[0]<<vpos[1]<<vpos[2]<<allSpotPos.count();
    }
    double center[3]={0,0,0};
    _newScene3d->Init_IrradiationPoint(center,allSpotPos,m_iPatientOrientation);
}

void NewMainWindow::initRealtimeMenu()
{
    if (_realTimeMenu == NULL)
    {
        _realTimeMenu = new QMenu(ui->graphicsView_realtime_image);
        // _realTimeMenu->setWindowFlags(_tableMenu->windowFlags() | Qt::FramelessWindowHint);
        // menu白底黑字
//        _realTimeMenu->setStyleSheet("background-color: rgb(55, 55, 55);\ncolor: rgb(0, 0, 0);");
        _realTimeMenu->setStyleSheet("background-color:rgb(55,55,55);");
    }

    if (_realTimeZoomOut==NULL)
    {
        _realTimeZoomOut = _realTimeMenu->addAction("Zoom In");
        connect(_realTimeZoomOut,SIGNAL(triggered(bool)),this,SLOT(realTimeZoomOut()));
    }

    if (_realTimeZoomIn==NULL)
    {
        _realTimeZoomIn = _realTimeMenu->addAction("Zoom Out");
        connect(_realTimeZoomIn,SIGNAL(triggered(bool)),this,SLOT(realTimeZoomIn()));
    }

    if (_realTimeShowPath==NULL)
    {
        _realTimeShowPath = _realTimeMenu->addAction("Show Acoustic Window");
        connect(_realTimeShowPath,SIGNAL(triggered(bool)),this,SLOT(realTimeShowPath()));
    }

    if (_realTimeHidePath==NULL)
    {
        _realTimeHidePath = _realTimeMenu->addAction("Hide Acoustic Window");
        connect(_realTimeHidePath,SIGNAL(triggered(bool)),this,SLOT(realTimeHidePath()));
    }

    if (_realTimeShowFocus==NULL)
    {
        _realTimeShowFocus = _realTimeMenu->addAction("Show Natural Focus");
        connect(_realTimeShowFocus,SIGNAL(triggered(bool)),this,SLOT(realTimeShowFocus()));
    }

    if (_realTimeHideFocus==NULL)
    {
        _realTimeHideFocus = _realTimeMenu->addAction("Hide Natural Focus");
        connect(_realTimeHideFocus,SIGNAL(triggered(bool)),this,SLOT(realTimeHideFocus()));
    }

    if (_realTimeShowRange==NULL)
    {
        _realTimeShowRange = _realTimeMenu->addAction("Show Sonication Range");
        connect(_realTimeShowRange,SIGNAL(triggered(bool)),this,SLOT(realTimeShowRange()));
    }

    if (_realTimeHideRange==NULL)
    {
        _realTimeHideRange = _realTimeMenu->addAction("Hide Sonication Range");
        connect(_realTimeHideRange,SIGNAL(triggered(bool)),this,SLOT(realTimeHideRange()));
    }

    if (_realTimeResume==NULL)
    {
        _realTimeResume = _realTimeMenu->addAction("Reset to Original Image Size");
        connect(_realTimeResume,SIGNAL(triggered(bool)),this,SLOT(realTimeResume()));
    }

    // 此功能当前没需求
//    if (_realTimeChange == NULL)
//    {
//        _realTimeChange = _realTimeMenu->addAction("Change Planning Image");
//        connect(_realTimeChange,SIGNAL(triggered(bool)),this,SLOT(changeImage()));

//    }
}

void NewMainWindow::initTableMenu()
{
    if (_tableMenu == NULL)
    {
        _tableMenu = new QMenu(ui->tableView_current_plan);
        // menu白底黑字
        _tableMenu->setStyleSheet("background-color: rgb(55, 55, 55);\ncolor: rgb(0, 0, 0);");
    }

    if (_tableCopy == NULL)
    {
//        _tableCopy = _tableMenu->addAction("复制");
//        connect(_tableCopy,SIGNAL(triggered(bool)),this,SLOT(copyRows()));
    }

    if (_tableDelete==NULL)
    {
        _tableDelete = _tableMenu->addAction("Delete");
        connect(_tableDelete,SIGNAL(triggered(bool)),this,SLOT(deleteRows()));
    }

//    if (_tableStop==NULL)
//    {
//        _tableStop = _tableMenu->addAction("Stop");
//        connect(_tableStop,SIGNAL(triggered(bool)),this,SLOT(stopRows()));
//    }

//    if (_tableContinue==NULL)
//    {
//        _tableContinue = _tableMenu->addAction("继续");
//        connect(_tableContinue,SIGNAL(triggered(bool)),this,SLOT(continueRows()));
//    }
}

void NewMainWindow::initTargetMenu()
{
    if (_targetMenu==NULL)
    {
        _targetMenu = new QMenu(ui->graphicsView_target_scene);
        _targetMenu->setStyleSheet("background-color:rgb(55,55,55);");
    }

    if (_targetShowElement==NULL)
    {
        _targetShowElement = _targetMenu->addAction("Show Sonication Cells");
        connect(_targetShowElement,SIGNAL(triggered(bool)),this,SLOT(targetShowElement()));
    }

    if (_targetHideElement==NULL)
    {
        _targetHideElement = _targetMenu->addAction("Hide Sonication Cells");
        connect(_targetHideElement,SIGNAL(triggered(bool)),this,SLOT(targetHideElement()));
    }

    if (_targetZoomOut==NULL)
    {
        _targetZoomOut = _targetMenu->addAction("Zoom In");
        connect(_targetZoomOut,SIGNAL(triggered(bool)),this,SLOT(targetZoomOut()));
    }

    if (_targetZoomIn==NULL)
    {
        _targetZoomIn = _targetMenu->addAction("Zoom Out");
        connect(_targetZoomIn,SIGNAL(triggered(bool)),this,SLOT(targetZoomIn()));
    }

    if (_targetShowDeleted==NULL)
    {
        _targetShowDeleted = _targetMenu->addAction("Show Deleted Cells");
        connect(_targetShowDeleted,SIGNAL(triggered(bool)),this,SLOT(targetShowDeleted()));
    }

    if (_targetHideDeleted==NULL)
    {
        _targetHideDeleted = _targetMenu->addAction("Hide Deleted Cells");
        connect(_targetHideDeleted,SIGNAL(triggered(bool)),this,SLOT(targetHideDeleted()));
    }

    if (_targetSelectInvert == NULL)
    {
        _targetSelectInvert = _targetMenu->addAction("Invert Sonication Point Selection");
        connect(_targetSelectInvert,SIGNAL(triggered(bool)),this,SLOT(targetSelectInvert()));
    }

    if (_targetSmartSelect == NULL)
    {
        _targetSmartSelect = _targetMenu->addAction("Smart Select Cells");
        connect(_targetSmartSelect,SIGNAL(triggered(bool)),this,SLOT(targetSmartSelect()));
    }

    if (_targetResumeDelected==NULL)
    {
        _targetResumeDelected = _targetMenu->addAction("Restore Deleted Cells");
        connect(_targetResumeDelected,SIGNAL(triggered(bool)),this,SLOT(targetResumeDeleted()));
    }

    if (_targetResumeImages==NULL)
    {
        _targetResumeImages = _targetMenu->addAction("Reset Image Size");
        connect(_targetResumeImages,SIGNAL(triggered(bool)),this,SLOT(targetResumeImages()));
    }

//    if (_targetShowContours == NULL)
//    {
//        _targetShowContours = _targetMenu->addAction("显示fibroid轮廓");
//        connect(_targetShowContours,SIGNAL(triggered(bool)),this,SLOT(targetShowContours()));
//    }

//    if (_targetHideContours == NULL)
//    {
//        _targetHideContours = _targetMenu->addAction("隐藏fibroid轮廓");
//        connect(_targetHideContours,SIGNAL(triggered(bool)),this,SLOT(targetHideContours()));
//    }

    if (_targetShowDraw==NULL)
    {
        _targetShowDraw = _targetMenu->addAction("Show Traced Contour");
        connect(_targetShowDraw,SIGNAL(triggered(bool)),this,SLOT(targetShowDraw()));
    }

    if  (_targetHideDraw == NULL)
    {
        _targetHideDraw = _targetMenu->addAction("Hide Traced Contour");
        connect(_targetHideDraw,SIGNAL(triggered(bool)),this,SLOT(targetHideDraw()));
    }

//    if(_targetDrag == NULL)
//    {
//        _targetDrag = _targetMenu->addAction("手动Edit轮廓");
//        connect(_targetDrag,SIGNAL(triggered(bool)),this,SLOT(targetDrag()));
//    }

//    if(_targetDragConfirm == NULL)
//    {
//        _targetDragConfirm = _targetMenu->addAction("Confirm拖动轮廓");
//        connect(_targetDragConfirm,SIGNAL(triggered(bool)),this,SLOT(targetDragConfirm()));
//    }
}

void NewMainWindow::showRegistWindow()
{
    qCDebug(NEWMAINWINDOW())<< NEWMAINWINDOW().categoryName()
                         << "Enter show regist window !"
                         << TRACE_CMH();
    emit showUserManager(true);
    _location->ReceiveHeadControl(0,0);
    _location->ReceiveHeadControl(1,0);
    _location->ReceiveHeadControl(2,0);
    _pHeaderControl->close();
    _pChoseimage->close();
    DelayClose();
}

//  Close the application
void NewMainWindow::on_pushButton_quit_clicked()
{
    // 当参数右边标志显示为红色时进入此代码
    QMessageBox msgBox("Close Application",
                       "Close the application?",
                       QMessageBox::Critical,
                       QMessageBox::Yes | QMessageBox::Default,
                       QMessageBox::No | QMessageBox::Escape,
                       QMessageBox::NoButton);
    msgBox.setWindowFlags(Qt::WindowTitleHint|Qt::CustomizeWindowHint|Qt::WindowStaysOnTopHint);
    msgBox.setGeometry(QRect(1400,400,msgBox.width(),msgBox.height()));
    msgBox.setFont(m_msgBoxFont);
    msgBox.setButtonText(QMessageBox::Yes, "OK");
    msgBox.setButtonText(QMessageBox::No, "Cancel");

    int ret = msgBox.exec();
    if (ret == QMessageBox::No)
    {
        return;
    }
//    if(_seriesAcquired)
//    {
//        QMessageBox::information(0,"PositioningMiddle","请等待Positioning结束后再Close Application","Confirm",0);
//        return;
//    }

    // sonication过程中，Stopsonication
    if (_isIrradiating)
    {
        on_pushButton_stop_clicked();
    }

    if (_pHeaderControl!=NULL)
    {
        _pHeaderControl->Bed_Stop();
    }

    // 提前阻止机械运动
    if(_location!=NULL)
    {
        delete _location;
        _location = NULL;
    }

    QMessageBox msgDrainTank("Drain Water Tank",
                       "Drain the water tank?",
                       QMessageBox::Critical,
                       QMessageBox::Yes | QMessageBox::Default,
                       QMessageBox::No | QMessageBox::Escape,
                       QMessageBox::NoButton);
    msgDrainTank.setWindowFlags(Qt::WindowTitleHint|Qt::CustomizeWindowHint|Qt::WindowStaysOnTopHint);
    msgDrainTank.setGeometry(QRect(1400,400,msgDrainTank.width(),msgDrainTank.height()));
    msgDrainTank.setFont(m_msgBoxFont);
    msgDrainTank.setButtonText(QMessageBox::Yes, "OK");
    msgDrainTank.setButtonText(QMessageBox::No, "Cancel");

    ret = msgDrainTank.exec();
    qDebug()<<"Receive msg drain tank "<<ret;
    if (ret == QMessageBox::No)
    {
        qDebug()<<"Enter msg drain tank no "<<ret;

        if (_pChoseimage!=NULL)
        {
            _pChoseimage->accept();    // DeleteSelect成像面窗口
            delete _pChoseimage;
            _pChoseimage = NULL;
        }

        if (_pHeaderControl!=NULL)
        {
            _pHeaderControl->accept();
            delete _pHeaderControl;
            _pHeaderControl = NULL;
        }
//        if(_location!=NULL)
//        {
//            delete _location;
//            _location = NULL;
//        }

        emit MainClose();
        this->close();
        //return;
    }
    else
    {
        qDebug()<<"Send message to drain tank !";
        emit sendDrainTank();
        _drainTankForm->showFullScreen();
    }

}

void NewMainWindow::on_horizontalScrollBar_images_valueChanged(int value)
{
    // 同步相同位置核磁与US图像
    on_horizontalScrollBar_slices_valueChanged(value);
    ui->horizontalScrollBar_slices->setValue(value);

    if (value<91)
    {
        ui->spinBox_angle_one->setValue(value-91);
    }
    else
    {
        ui->spinBox_angle_two->setValue(value-91);
    }

    on_horizontalScrollBar_images_sliderReleased();
}

// 对剩余未registration的核磁fibroid轮廓registration
void NewMainWindow::registRemainContours()
{
}

void NewMainWindow::on_acquireSeries_completed()
{
    qCDebug(NEWMAINWINDOW()) << NEWMAINWINDOW().categoryName()
                          << "acquire completed scene map size is "<< _sceneMap.size()
                          << TRACE_CMH();

    _seriesAcquired = false;
    ui->pushButton_headL->setEnabled(true);
    ui->pushButton_headR->setEnabled(true);
    ui->pushButton_headU->setEnabled(true);
    ui->pushButton_headD->setEnabled(true);
    ui->pushButton_headF->setEnabled(true);
    ui->pushButton_headB->setEnabled(true);
    ui->toolButton_change_image_plane->setEnabled(true);
    ui->toolButton_change_image_plane->setStyleSheet(SELECTED);
    ui->toolButton_header_zero->setEnabled(true);
    ui->toolButton_header_zero->setStyleSheet(SELECTED);
    // Measurement Analysis模块Restore可用
    ui->tabWidget_operator->setTabEnabled(0,true);
    if(Image_WaterLock)
    {
        Image_WaterLock = false;
        controlunlock();
    }

    if (!ui->pushButton_measure_length->isEnabled())
    {
        // 设置长度测量可用
        ui->pushButton_measure_length->setEnabled(true);
        ui->pushButton_measure_length->setStyleSheet(SELECTED);
    }

    // 采集Complete，直方图分析可用
    ui->pushButton_histogram->setEnabled(true);
    ui->pushButton_histogram->setStyleSheet(SELECTED);

    // 设置可用实声束和Transducer图标
    if (!ui->toolButton_beam_path->isEnabled())
    {
        ui->toolButton_beam_path->setEnabled(true);
        ui->toolButton_beam_path->setStyleSheet(SELECTED);

        ui->toolButton_transducer->setEnabled(true);
        ui->toolButton_transducer->setStyleSheet(SELECTED);
    }
    if (_targetMenu!=NULL){
        _targetMenu->setEnabled(true);
    }
    if (_realTimeMenu!=NULL){
        _realTimeMenu->setEnabled(true);
    }
    if (_layerMenu!=NULL){
        _layerMenu->setEnabled(true);
    }

    // 对剩余未Register Contoursregistration
    registRemainContours();
    // 设置Plan制定按钮可用
    setWholePlanControlEnable();

    if (_para==NULL)
    {
        _para = new IrradiatePara;
    }

    initTreatPara();

    // 触发重建层图像
    emit setLayer();
}

void NewMainWindow::initTreatPara()
{
    QString text = ui->comboBox_deepth->currentText();
    on_comboBox_deepth_currentTextChanged(text);
}

void NewMainWindow::init()
{
    _append = new Append();
    _hifuimagealgo = new HifuImageAlgo;
    newHardwares();
    Delay_MSec(5000);
    initMsgCtrl();
    initSchedule();    // TreatmentPlan系统初始化
    initGrabber();     // 图像采集系统初始化
    Delay_MSec(30000);
    initCore();        // 功放驱动初始化
    initLocation();    // Motion System系统初始化
    initDate();
    on_displayTime_updated();
    Delay_MSec(5000);

    initWater();       // Circulation系统初始化
    initTimers();
    initPlanTable();
    initSmartPlanPanel();   // 初始化智能Trace控件
    initComboBox();         // 初始化
    initHIFUService();

    connect(_realtimeScene, SIGNAL(seriesAcquireCompleted()),
            this, SLOT(on_acquireSeries_completed()));

    connect(_realtimeScene,SIGNAL(synImage()),this,SLOT(refreshHistogram()));

//    connect(_water,SIGNAL(statusUpdated(uint,uint,uint)),
//            _pSelfCheck,SLOT(statusUpdated(uint,uint,uint)));

    locationZero();
    connect(_location, SIGNAL(error(ERRORTYPE)),
            this, SLOT(on_location_error_updated(ERRORTYPE)));
    // updateLocation();

    initAppend();
    Heartthread_thread->start();
    grabber_thread->start();
}

void NewMainWindow::refreshHistogram()
{
    if (_pWidget!=NULL)
    {
        _pWidget->update();
    }
}

void NewMainWindow::switchChanged(bool status)
{
    if (status)
    {
        ui->label_switch->setPixmap(QPixmap(QString(ICON_PIC_PATH).append("踏板开.png")));
    }
    else
    {
        ui->label_switch->setPixmap(QPixmap(QString(ICON_PIC_PATH).append("踏板关.png")));
    }
}

void NewMainWindow::setpos(int a,float b)
{
    if(!_seriesAcquired)
    {
        _location->setPos((AID)a,b);
    }
}

void NewMainWindow::initHIFUService()
{
    _hifuHttpServer=new HifuHttpServer();

    connect(_hifuHttpServer,SIGNAL(setLocationPos(int,float)),this,SLOT(setpos(int,float)));

    if (_hifuHttpServer->StartServer())
    {
        ui->label_network_icon->setPixmap(QPixmap::fromImage(QImage(":/icons/img/bottomicon/Network-Interface.png")));
        ui->label_network_text->setStyleSheet(WORK);
        ui->label_alarm_light->setPixmap(QPixmap(QString(ICON_PIC_PATH).append("green.png")));
    }
    else
    {
        ui->label_network_icon->setPixmap(QPixmap::fromImage(QImage(":/icons/img/bottomicon/Network-Interface-abnormal.png")));
        ui->label_network_text->setStyleSheet(NOT_WORK);
        networkready=false;
        ui->label_alarm_light->setPixmap(QPixmap(QString(ICON_PIC_PATH).append("red.png")));
    }
}

void NewMainWindow::initAppend()
{
    bool bAppend=false;
    if(_append!=NULL&&_append->exist())
    {
        bAppend=true;
    }
    else
    {
        emit SendSubSysStatus(iappned,false);
        return;
    }

    connect(_append,SIGNAL(switchChanged(bool)),this,SLOT(switchChanged(bool)));
    connect(_append,SIGNAL(error()),this,SLOT(appendConnectFailed()));

    if (_subSysStatus[0]&&_subSysStatus[1]&&_subSysStatus[2]&&_subSysStatus[3]&&_subSysStatus[4] &&powerampready)
    {
        ui->label_alarm_light->setPixmap(QPixmap(QString(ICON_PIC_PATH).append("green.png")));
        _append->setRedOff();
//        if (_pMediaPlayer!=NULL)
//        {
//            _pMediaPlayer->stop();
//        }
    }
    else
    {
        ui->label_alarm_light->setPixmap(QPixmap(QString(ICON_PIC_PATH).append("red.png")));
        _append->setRedOn();

        qCDebug(NEWMAINWINDOW())<<NEWMAINWINDOW().categoryName()
                             <<"Error init append failed !"
                             << TRACE_CMH();
//        if (_pMediaPlayer!=NULL)
//        {
//            _pMediaPlayer->play();
//        }
    }
    ui->label_alarm_text->setText(QString("Indicator"));

    ui->label_running_light->setPixmap(QPixmap(QString(ICON_PIC_PATH).append("green.png")));
    ui->label_running_text->setText(QString("Running"));

    ui->label_power_light->setPixmap(QPixmap(QString(ICON_PIC_PATH).append("green.png")));

    _append->setGreenOn();

    ui->label_power_text->setText(QString("Power"));

    if (_append->isSwitchOn())
    {
        ui->label_switch->setPixmap(QPixmap(QString(ICON_PIC_PATH).append("踏板开.png")));
        qDebug()<<"switch_en = true";
    }
    else
    {
        ui->label_switch->setPixmap(QPixmap(QString(ICON_PIC_PATH).append("踏板关.png")));
    }

    ui->label_switch_text->setText(QString("Foot Switch"));

    emit SendSubSysStatus(iappned,true);
}

void NewMainWindow::initComboBox()
{
//    ui->comboBox_irradiate_model->addItem(QWidget::tr("Inside Out"));
//    ui->comboBox_irradiate_model->addItem(QWidget::tr("Outside In"));
    setUserModel();

//    ui->comboBox_part->addItem(QWidget::tr("uterine fibroid"));
//    ui->comboBox_part->setCurrentText("uterine fibroid");
    ui->comboBox_part->addItem(QWidget::tr("Custom Mode"));
    ui->comboBox_part->setCurrentText("Custom Mode");

//    ui->comboBox_deepth->clear();
//    ui->comboBox_deepth->addItem(QWidget::tr("3"));
//    ui->comboBox_deepth->addItem(QWidget::tr("5"));
//    ui->comboBox_deepth->addItem(QWidget::tr("7"));
    ui->comboBox_deepth->clear();
    ui->comboBox_deepth->addItem(QWidget::tr("3"));
    ui->comboBox_deepth->addItem(QWidget::tr("4"));
    ui->comboBox_deepth->addItem(QWidget::tr("5"));
    ui->comboBox_deepth->addItem(QWidget::tr("6"));
    ui->comboBox_deepth->addItem(QWidget::tr("7"));
    ui->comboBox_deepth->addItem(QWidget::tr("8"));
    ui->comboBox_deepth->addItem(QWidget::tr("9"));
    ui->comboBox_deepth->addItem(QWidget::tr("10"));
    ui->comboBox_deepth->addItem(QWidget::tr("11"));
    ui->comboBox_deepth->addItem(QWidget::tr("12"));
    ui->comboBox_deepth->setCurrentText("6");
}

void NewMainWindow::initDate() {
    QDate date = QDate::currentDate();
    ui->label_current_day->setText(QString("%1").arg(date.day()));
    ui->label_current_month->setText(QString("%1").arg(date.month()));

    _startTime = "";
    startTime = 0;
    _endTime = "";
    endTime = 0;
    _totalSpot = 0;
    _seriesSpot = 0;
    treatedSpot = 0;
    _recSpot = 0;
    _totalTime = 0;
}

void NewMainWindow::initTimers()
{
    LogHeartTimer = new QTimer();
    LogHeartTimer->setInterval(5000);
    connect(LogHeartTimer,SIGNAL(timeout()),this,SLOT(LogHeartBeat()));

    _updatedDateTimer = new QTimer(this);
    _updatedDateTimer->setInterval(1000);
    _updatedDateTimer->setSingleShot(false);
    connect(_updatedDateTimer, SIGNAL(timeout()), this, SLOT(on_displayTime_updated()));
    _updatedDateTimer->start();

    _acquisionTimer = new QTimer(this);
    _acquisionTimer->setInterval(25);
    _acquisionTimer->setSingleShot(true);


    _treatmentTimer = new QTimer(this);
    _treatmentTimer->setInterval(1000);
    _treatmentTimer->setSingleShot(false);
    connect(_treatmentTimer, SIGNAL(timeout()), this, SLOT(on_treatmentTime_updated()));
//    _treatmentTimer->start();

//    _treatmentTimeRemain = new QTimer(this);
//    _treatmentTimeRemain->setInterval(5000);
//    _treatmentTimeRemain->setSingleShot(false);
//    connect(_treatmentTimeRemain, SIGNAL(timeout()), this, SLOT(on_treatment_time_remain_update()));
//    _treatmentTimeRemain->start();

    LocationCD = new QTimer(this);
    LocationCD->setInterval(500);
    LocationCD->setSingleShot(true);
    connect(LocationCD, SIGNAL(timeout()), this, SLOT(on_Location_CD()));

    servicecalled.setInterval(10000);
    servicecalled.setSingleShot(false);
    connect(&servicecalled,SIGNAL(timeout()),this,SLOT(networkcalled()));
}

bool NewMainWindow::networkcalled()
{
    m_networkservice++;
    if (_schedule!=NULL)
    {
        _schedule->onServiceCalled(INQUIRY, NETWORK);
    }
    if(m_networkservice>=18)
    {
        servicecalled.stop();
    }
    return true;
}

void NewMainWindow::on_treatment_time_remain_update()
{
//    m_totalTime.spendTime+=5;
    QString str = getRemainTotalTime();
    ui->label_time_remain->setText(str);

//    str = getTotalTime();
//    ui->label_time_treatment->setText(str);
}

void NewMainWindow::on_displayTime_updated()
{
    QString str = QTime::currentTime().toString("hh:mm:ss");
    ui->label_current_time->setText(str);
}

void NewMainWindow::on_refresh_real_angle_timeout()
{
    if (_location!=NULL)
    {
        _location->getAng();
    }
}

void NewMainWindow::openPump()    // Start Pump
{
//    if (_water!=NULL)
//    {
//        _water->openPump();
//    }
}

void NewMainWindow::closePump()    // Stop Pump
{
//    if (_water!=NULL)
//    {
//        _water->closePump();
//    }
}

void NewMainWindow::setMeter(short premeter)
{
//    if(_water!=NULL)
//    {
//        _water->setMeter(premeter);
//    }
}

void NewMainWindow::queryStatus()
{
    qCDebug(NEWMAINWINDOW())<<NEWMAINWINDOW().categoryName()
                         <<"query water status !"
                         << TRACE_CMH();

//    if (_water!=NULL)
//    {
//        qCDebug(NEWMAINWINDOW())<<NEWMAINWINDOW().categoryName()
//                             <<"water pointer is "<<_water
//                             << TRACE_CMH();

//        _water->queryStatus();
//    }
}

void NewMainWindow::getPump1()
{
//    if (_water!=NULL)
//    {
//        _water->getPump1();
//    }
}

void NewMainWindow::getPump2()
{
//    if (_water!=NULL)
//    {
//        _water->getPump2();
//    }
}

void NewMainWindow::getwater()
{
//    if (_water!=NULL)
//    {
//        _water->getwater();
//    }
}

void NewMainWindow::setduPump1Speed(int Speed)
{
//    qDebug()<<"setpump1speed"<<QString::number(Speed);
//    if(_water!=NULL)
//    {
//        _water->setduPump1(Speed);
//    }
}

void NewMainWindow::setduPump2Speed(int Speed)
{
//    qDebug()<<"setpump2speed";
//    if(_water!=NULL)
//    {
//        _water->setduPump2(Speed);
//    }
}

void NewMainWindow::closeduPump1()
{
//    qDebug()<<"closepump1";
//    if(_water!=NULL)
//    {
//        _water->closeduPump1();
//    }
}

void NewMainWindow::closeduPump2()
{
//    qDebug()<<"closepump2";
//    if(_water!=NULL)
//    {
//        _water->closeduPump2();
//    }
}

void NewMainWindow::queryMeter()
{
//    if(_water!=NULL)
//    {
//        _water->getMeter();
    //    }
}

void NewMainWindow::LogHeartBeat()
{
    updateLog("---");
}

void NewMainWindow::setPumpSpeed(int speed)
{
//    if (_water!=NULL)
//    {
//        _water->setSpeed(speed);
//    }
}

bool NewMainWindow::setPos(AID axis,float pos)
{
    if (_location!=NULL)
    {
        bool bReturn=false;
       if(axis==AXIS_Z)
           bReturn = _location->setPos(axis,-pos);
       else
           bReturn = _location->setPos(axis,pos);
       if (bReturn)
       {
           switch(axis)
           {
           case AXIS_L:
               qDebug()<<"head L======"<<pos;
               if(pos==(int)pos)
                    ui->horizontalSlider_left_right->setValue(pos);
               break;

           case AXIS_F:
               if(pos==(int)pos)
                    ui->horizontalSlider_forward_backward->setValue(pos);
               break;

           case AXIS_Z:
               if(pos==(int)pos)
                    ui->horizontalSlider_up_down->setValue(pos);
               break;

           default:
               break;
           }
       }

       return bReturn;
    }

    return false;
}

bool NewMainWindow::setPos(int angle)
{

    if (_refreshRealAngle==NULL)
    {
        _refreshRealAngle = new QTimer(this);
        _refreshRealAngle->setInterval(500);  // 大概83ms转一度
        _refreshRealAngle->setSingleShot(false);
        connect(_refreshRealAngle, SIGNAL(timeout()), this, SLOT(on_refresh_real_angle_timeout()));
    }

    _destAngle = angle;
    _refreshRealAngle->start();
    _pChoseimage->setDisabled();

    // 旋转探头时设置采集图像不可用
    ui->toolButton_acquire_image->setEnabled(false);
    ui->toolButton_acquire_image->setStyleSheet(NOT_WORK);

    return _location->setPos(AXIS_A,angle);
}

void NewMainWindow::sendpar(uchar par, uchar dat)
{
//    if(_water!=NULL)
//    {
//        _water->sendPar(par,dat);
//    }
}

void NewMainWindow::on_acquireSingleFrame_completed() {
    // 当前最多181幅Positioning图像
    if (_currentImages<ANGLE_COUNT)
    {
        ui->horizontalScrollBar_images->setMaximum(++_currentImages);
    }

    disconnect(_acquisionTimer, SIGNAL(timeout()),
               this, SLOT(on_acquireSingleFrame_completed()));

    if (_location!=NULL)
    {
        _location->setPos(AXIS_A, _realtimeScene->angle());
    }
    else
    {
        qCDebug(NEWMAINWINDOW())<<NEWMAINWINDOW().categoryName()
                                <<"_location null !"
                                << TRACE_CMH();
    }

}

void NewMainWindow::newHardwares()
{
    if (_grabber==NULL)
    {
        _grabber = new ImageGrabber(this);
        grabber_thread = new QThread;
        _grabber->moveToThread(grabber_thread);
        connect(this,SIGNAL(SaveVideo(QString)),_grabber,SLOT(startSaveVideo(QString)));
        connect(_grabber,SIGNAL(no_signal()),this,SLOT(ImageNoSignal()));

        connect(_grabber,SIGNAL(on_signal()),this,SLOT(ImageOnSignal()));
    }

    _location = new Location(this);
}

void NewMainWindow::initMsgCtrl() {
    _msgCtrl = new Message(this);
    connect(_grabber, SIGNAL(statusUpdated(DataFlag)),
            _msgCtrl, SIGNAL(statusUpdated(DataFlag)));
    connect(_location, SIGNAL(statusUpdated(DataFlag)),
            _msgCtrl, SIGNAL(statusUpdated(DataFlag)));
    connect(_msgCtrl, SIGNAL(statusUpdated(DataFlag)),
            this, SLOT(on_status_updated(DataFlag)));
}

void NewMainWindow::disableBeforeSetPos()
{
    // 设置前两按钮文字颜色一致
//    ui->toolButton_dqa->setStyleSheet(SELECTED);
    ui->toolButton_acquire_image->setStyleSheet(SELECTED);

    //设置Zoom in、Zoom out、Hide Acoustic Window和Show Acoustic Window按钮格式
    ui->toolButton_zoomout_image->setStyleSheet(SELECTED);
    ui->toolButton_zoomin_image->setStyleSheet(SELECTED);
    ui->toolButton_showpath->setStyleSheet(SELECTED);
    ui->toolButton_hidepath->setStyleSheet(SELECTED);

    //设置Zoom in、Zoom out、Hide Acoustic Window和Show Acoustic Window按钮不可用
    ui->toolButton_zoomout_image->setDisabled(true);
    ui->toolButton_zoomout_image->setStyleSheet(NOT_WORK);
    ui->toolButton_zoomin_image->setDisabled(true);
    ui->toolButton_zoomin_image->setStyleSheet(NOT_WORK);
//    ui->toolButton_showpath->setDisabled(true);
//    ui->toolButton_showpath->setStyleSheet(NOT_WORK);
//    ui->toolButton_hidepath->setDisabled(true);
//    ui->toolButton_hidepath->setStyleSheet(NOT_WORK);

    // 初始化设置Plan和Treatment按钮不可用
    ui->toolButton_therapy->setDisabled(true);
    ui->toolButton_therapy->setStyleSheet(NOT_WORK);

    //ui->toolButton_safe_margin->setDisabled(true);
    //ui->toolButton_safe_margin->setStyleSheet(NOT_WORK);

    // 设置声束和Transducer显示控件不可用
    ui->toolButton_beam_path->setDisabled(true);
    ui->toolButton_beam_path->setStyleSheet(NOT_WORK);
    ui->toolButton_transducer->setDisabled(true);
    ui->toolButton_transducer->setStyleSheet(NOT_WORK);

    // 设置Treatment按钮不可用
//    ui->toolButton_therapy->setDisabled(true);
//    ui->toolButton_therapy->setStyleSheet(NOT_WORK);

    // Plan前设置改变成像面不可用
    //ui->toolButton_change_image_plane->setDisabled(true);
    //ui->toolButton_change_image_plane->setStyleSheet(NOT_WORK);

    ui->pushButton_stop->setDisabled(true);
    ui->pushButton_stop->setStyleSheet(NOT_WORK);

    ui->pushButton_continue->setDisabled(true);
    ui->pushButton_continue->setStyleSheet(NOT_WORK);

    ui->pushButton_start_treat->setDisabled(true);
    ui->pushButton_start_treat->setStyleSheet(NOT_WORK);

    setModifyPlanControlDisable();

    // 设置浏览Treatment Report不可用
    ui->pushButton_treatment_report->setDisabled(true);
    ui->pushButton_treatment_report->setStyleSheet(NOT_WORK);

    ui->horizontalScrollBar_images->setDisabled(true);
    ui->horizontalScrollBar_images->setStyleSheet(NOT_WORK);
    ui->pushButton_front->setDisabled(true);
    ui->pushButton_front->setStyleSheet(NOT_WORK);
    ui->pushButton_back->setDisabled(true);
    ui->pushButton_back->setStyleSheet(NOT_WORK);

    disableLayerControl();
}

void NewMainWindow::disableLayerControl()
{
    ui->pushButton_treatment_layer_down->setDisabled(true);
    ui->pushButton_treatment_layer_down->setStyleSheet(NOT_WORK);
    ui->pushButton_treatment_layer_middle->setDisabled(true);
    ui->pushButton_treatment_layer_middle->setStyleSheet(NOT_WORK);
    ui->pushButton_treatment_layer_up->setDisabled(true);
    ui->pushButton_treatment_layer_up->setStyleSheet(NOT_WORK);
}
// 设置Treatment Plane相关控件可用
void NewMainWindow::enableLayerControl()
{
    ui->pushButton_treatment_layer_down->setEnabled(true);
    ui->pushButton_treatment_layer_middle->setEnabled(true);
    ui->pushButton_treatment_layer_up->setEnabled(true);
}

void NewMainWindow::refreshAmpPara(DataFlag pFlag, QByteArray &ba)
{
    emit refreshAmpParaInTable(pFlag, ba);
}

void NewMainWindow::initSchedule() {
//    _schedule = new ServiceBuffer(this);

//    connect(_schedule, SIGNAL(bytesToWrite(QByteArray&)),
//            _client, SLOT(send(QByteArray&)));
//    connect(_schedule, SIGNAL(bytesToWrite(QByteArray&)),
//            Heartthread,SLOT(stopheart()));
//    connect(_client, SIGNAL(packageReady(QByteArray&)),
//            _schedule, SLOT(onPackageReceived(QByteArray&)));
//    connect(_client, SIGNAL(statusUpdated(PackageFlag, DataFlag)),
//            _msgCtrl, SLOT(work(PackageFlag, DataFlag)));
//    connect(_schedule, SIGNAL(serviceCalled(PackageFlag, DataFlag)),
//            _msgCtrl, SLOT(work(PackageFlag, DataFlag)));

    // Refreshpower amp参数
//    connect(_schedule,SIGNAL(refreshAmpPara(DataFlag,QByteArray&)),
//            this,SLOT(refreshAmpPara(DataFlag,QByteArray&)));

    // 设置Ultrasound Driver图标可用  TODO要QueryUltrasound Driver是否真的可用
    ui->label_hifu_driver_icon->setPixmap(QPixmap::fromImage(QImage(":/icons/img/bottomicon/Ultrasound-Driver.png")));
    ui->label_hifu_driver_text->setStyleSheet(WORK);

    _plan = new Plan(this);
    _subSysStatus[POWER_AMP]= true;    // 功放输出正常
    _subSysStatus[NET_WORK]= true;     // 网络正常
//    servicecalled.start();

//    if (_schedule!=NULL)
//    {
//        _schedule->onServiceCalled(INQUIRY, NETWORK);
//    }

//    qCDebug(NEWMAINWINDOW())<<NEWMAINWINDOW().categoryName()
//                         << "power amp initial success !"
//                         << "net work initial success !"
//                         << TRACE_CMH();

    disableBeforeSetPos();
}

// 记录图像采集卡采集到的图像
void NewMainWindow::addScene(QImage image)
{
    if (!_seriesAcquired)
    {
        // 只显示需要采集的图像
        return;
    }

    int angle = _realtimeScene->angle();
    QMap<int,DrawScene*>::iterator iter = _sceneMap.find(angle);
    DrawScene* tempScene = NULL;
    if (iter==_sceneMap.end()) //已有相同Angle则不Add
    {
        tempScene = new DrawScene(angle);
        tempScene->setSceneRect(0,0,574,574);
        tempScene->setBackgroundPixmap(QPixmap::fromImage(image));
        connect(tempScene,SIGNAL(changeView2D(viewer2D_info)),this,SLOT(changeView2D(viewer2D_info)));
        connect(tempScene,SIGNAL(spotSelected(QString,QColor)),this,SLOT(spotSelected(QString,QColor)));
        _sceneMap.insert(angle,tempScene);
    }

    if (_targetScene!=NULL)
    {
        return;
    }
    else
    {
        iter = _sceneMap.find(-90);
        if (iter!=_sceneMap.end())
        {
            _targetScene = *iter;
            ui->graphicsView_target_scene->setScene(_targetScene);
            ui->label_select_image_angle->setText("-90 °");
        }
        else
        {
            return;
        }
    }
}

void NewMainWindow::clearLayerScenes()
{
    if (_layerScenes.empty())
    {
        return;
    }

     QQueue<DrawScene *>::iterator iter = _layerScenes.begin();
     for (; iter!=_layerScenes.end(); ++iter)
     {
         delete *iter;
     }

     _layerScenes.clear();
}

void NewMainWindow::clearTargetScenes()
{
    if (_sceneMap.empty())
    {
        return;
    }

    QMap<int,DrawScene*>::iterator iter = _sceneMap.begin();
    for (;iter != _sceneMap.end();++iter)
    {
        delete iter.value();
    }

    _sceneMap.clear();
}

void NewMainWindow::clearSliceScenes()
{
    if(_sliceSceneMap.empty())
    {
       return;
    }

    QMap<int,DrawScene*>::iterator iter = _sliceSceneMap.begin();
    for (;iter != _sliceSceneMap.end();++iter)
    {
        delete iter.value();
    }

    _sliceSceneMap.clear();
}

void NewMainWindow::refreshRealtimeImage(QImage image)    // 更新Live Image，方便Mobile调用
{
    image.save("../resources/realtime.jpg");
}

void NewMainWindow::initGrabber() {
    if(_realtimeScene==NULL){
        _realtimeScene = new RealtimeScene;
    }
    _realtimeScene->setSceneRect(0,0,574,574);  // RECT设置比图像小，防止滚轮滚动时图像滚动
    ui->graphicsView_realtime_image->setScene(_realtimeScene);

    if (imagesignal) {
#ifdef DEBUG
        ui->label_status->setText("Status: image grabber is connected");
#endif
        qCDebug(NEWMAINWINDOW())<<NEWMAINWINDOW().categoryName()
                             <<"image grabber initial success !"
                             << TRACE_CMH();
//        qDebug()<<"配置文件："<<RESOLUTION<<X_OFFSET<<Y_OFFSET;
        connect(_grabber, SIGNAL(imageRefreshed(QImage)), _realtimeScene, SLOT(refresh(QImage)));
        connect(_realtimeScene,SIGNAL(synPlanImage(QImage)),this,SLOT(addScene(QImage)));
        connect(_grabber, SIGNAL(imageRefreshed(QImage)), this, SLOT(refreshRealtimeImage(QImage)));
        _grabber->work();

        _subSysStatus[SUPER_B]= true; // 超声Status正常
        emit SendSubSysStatus(iimagegraber,true);
        on_status_updated(IMAGE_GRABBER_AVAILABLE);
    }
    else {
#ifdef DEBUG
        ui->label_status->setText("Status: image grabber is disconnected");
#endif
        qCDebug(NEWMAINWINDOW())<<NEWMAINWINDOW().categoryName()
                             << "Status: image grabber is disconnected"
                             << TRACE_CMH();
        emit SendSubSysStatus(iimagegraber,false);
//        emit SendSubSysStatus(iimagegraber,true);
    }
    qApp->processEvents(QEventLoop::ExcludeUserInputEvents);
}

void NewMainWindow::initWater()
{
    //_water->closeAutoMeter();
    _dupumpControl = new dupumpcontrol;
//    connect(_dupumpControl,SIGNAL(BedZeroStatus(bool)),this,SLOT(GetBedZeroStatus(bool)));
    if (_dupumpControl->init())
    {
        _dupumpControl->SetMainWindow(this);
        connect(_dupumpControl,SIGNAL(ConnectFailed(DataFlag)),this,SLOT(on_status_updated(DataFlag)));
        connect(this,SIGNAL(showdupumpcontrol()),_dupumpControl,SLOT(showFromMainWindow()));
        connect(_dupumpControl,SIGNAL(Temp1Update(int)),this,SLOT(Temp1Updated(int)));
        connect(this,SIGNAL(MeterUpdate(short)),_dupumpControl,SLOT(updateMeterDat(short)));
        connect(this,SIGNAL(pump1ctl()),_dupumpControl,SLOT(controlpump1()));
        connect(this,SIGNAL(speed1up()),_dupumpControl,SLOT(speed1up()));
        connect(this,SIGNAL(speed1down()),_dupumpControl,SLOT(speed1down()));
        connect(this,SIGNAL(pump2ctl()),_dupumpControl,SLOT(controlpump2()));
        connect(this,SIGNAL(speed2up()),_dupumpControl,SLOT(speed2up()));
        connect(this,SIGNAL(speed2down()),_dupumpControl,SLOT(speed2down()));
        connect(this,SIGNAL(getRodPos()),_dupumpControl,SLOT(Get_RodPos()));
        // 窗口向双泵水处理对象发送Drain Water Tank消息
        connect(this,SIGNAL(sendDrainTank()),_dupumpControl,SLOT(on_Drain_Tank_from_Window()));

        connect(_dupumpControl,SIGNAL(ReceiveHeadControl(int,int)),this,SLOT(LocationHeadControl(int,int)));
        connect(_dupumpControl,SIGNAL(ReceiveRodPos(int,int,int)),_pHeaderControl,SLOT(getBedPos(int,int,int)));
        connect(_pHeaderControl,SIGNAL(setRodPos(int,int,int)),_dupumpControl,SLOT(RodControl(int,int,int)));
        connect(_pHeaderControl,SIGNAL(getRodPos()),_dupumpControl,SLOT(Get_RodPos()));
//        connect(_pHeaderControl,SIGNAL(setBedZero()),_dupumpControl,SLOT(Set_BedZero()));
        connect(this,SIGNAL(setIrradiating(bool)),_dupumpControl,SLOT(TreatState(bool)));
#ifdef DEBUG
        ui->label_status->setText("Status: water is available");
#endif
        ui->label_water_icon->setPixmap(QPixmap::fromImage(QImage(":/icons/img/bottomicon/Circulation.png")));
        ui->label_water_text->setStyleSheet(WORK);
        _subSysStatus[WATER_CIRCLE]= true;    // Circulation系统正常
        emit SendSubSysStatus(iwarter,true);
        //sendpar(PAR_METER_AUTO,PAR_ENABLE);
        //_water->sendPar(PAR_METER_AUTO,PAR_ENABLE);
    }
    else
    {
#ifdef DEBUG
        ui->label_status->setText("Status: water is not available");
#endif
        ui->label_water_icon->setPixmap(QPixmap::fromImage(QImage(":/icons/img/bottomicon/Circulation-abnormal.png")));
        ui->label_water_text->setStyleSheet(NOT_WORK);
        _subSysStatus[WATER_CIRCLE]= false;
        emit SendSubSysStatus(iwarter,false);
        waterready=false;
        ui->label_alarm_light->setPixmap(QPixmap(QString(ICON_PIC_PATH).append("red.png")));
    }
    qApp->processEvents(QEventLoop::ExcludeUserInputEvents);
}

void NewMainWindow::initCore()
{
    qDebug()<<"init CORE!!!!!!!";
    _core = new IrradiateCore(_irradiatePlan,_append,_schedule,_plan,_msgCtrl);
    if(_core->init())
    {
        qDebug()<<"inetwork init succeeded!!!";
        _core->setcurrentCompleteMap(&_currentCompleteMap);
        emit SendSubSysStatus(inetwork,true);
        ui->label_hifu_driver_icon->setPixmap(QPixmap::fromImage(QImage(":/icons/img/bottomicon/Ultrasound-Driver.png")));
        ui->label_hifu_driver_text->setStyleSheet(WORK);
        _subSysStatus[POWER_AMP]= true;    // 功放输出正常
        _subSysStatus[NET_WORK]= true;     // 网络正常
    }
    else
    {
        qDebug()<<"inetwork init failed!!!";
        emit SendSubSysStatus(inetwork,false);
    }
}

void NewMainWindow::initLocation() {

    setLocationTips();
    connect(_location,SIGNAL(LocationErr()),this,SLOT(LocationAlarm()));
    connect(_location,SIGNAL(SetPosSuccess()),this,SLOT(LocationSetPosSuccess()));
    if (_location->initialize())
    {
        //_location->getAng();
        ui->label_status->setText("Location is available");
        connect(_location, SIGNAL(posDat(AID, APOS)),
                this, SLOT(on_location_status_updated(AID, APOS)));

        _subSysStatus[MECHINE_MOVE]= true;    // Motion System系统正常
    }
    else
    {
        ui->label_status->setText("Location is nonavailable");
        emit SendSubSysStatus(ilocation,false);
    }
    //qApp->processEvents(QEventLoop::ExcludeUserInputEvents);
}

void NewMainWindow::locationZero()
{
    if (_location==NULL)
    {
        qCDebug(NEWMAINWINDOW())<<NEWMAINWINDOW().categoryName()
                                <<"_location null !"
                                << TRACE_CMH();
        return;
    }
    if(!_subSysStatus[MECHINE_MOVE]){
        qCDebug(NEWMAINWINDOW())<<NEWMAINWINDOW().categoryName()
                                <<"_location init failed !"
                                << TRACE_CMH();
        return;
    }
    // 旋转轴零位校正
    if(!_location->zero(AXIS_A))
    {
        ui->label_status->setText("Axis A zero error !");
        _subSysStatus[MECHINE_MOVE] = false;
    }
    if (!_location->setPos(AXIS_L,0))  // 左右
    {
        ui->label_status->setText("Axis left right zero error !");
        _subSysStatus[MECHINE_MOVE] = false;
    }

    if (!_location->setPos(AXIS_F,0))  // 前后
    {
        ui->label_status->setText("Axis front back zero error !");
        _subSysStatus[MECHINE_MOVE] = false;
    }

    if(!_location->zero(AXIS_Z))
    {
        ui->label_status->setText("Axis Z zero error !");
        _subSysStatus[MECHINE_MOVE] = false;
    }

    if(BedZero)
        emit SendSubSysStatus(ilocation,_subSysStatus[MECHINE_MOVE]);
    ui->label_real_time_angle->setText("0 °");
//    else
//        emit SendSubSysStatus(ilocation,false);
}

void NewMainWindow::Temp1Updated(int temp)
{
    ui->label_temp1_value->setText(QString::number(temp));
    if(temp>=30){
        ui->label_temp1_value->setStyleSheet("color: rgb(237, 242, 13);");
        ui->label_temp->setStyleSheet("color: rgb(237, 242, 13);");
        ui->label_temp1_unit->setStyleSheet("color: rgb(237, 242, 13);");
    }
    else if(temp<=25){
        ui->label_temp1_value->setStyleSheet("color: rgb(255, 255, 255);");
        ui->label_temp->setStyleSheet("color: rgb(255, 255, 255);");
        ui->label_temp1_unit->setStyleSheet("color: rgb(255, 255, 255);");
    }
}

void NewMainWindow::statusUpdated(uint temp1,uint temp2,uint level)
{
    qDebug()<<"status update "<<temp1<<temp2<<level;
    if(temp1<=0x32&&temp1>=0)
    {
        ui->label_temp1_value->setText(QString::number(temp1));
    }
    else if(temp2<=0x32&&temp2>=0)
    {
        ui->label_temp1_value->setText(QString::number(temp2));
    }
//    ui->label_temp1_value->setText(QString::number(temp1));
//    ui->label_temp2_value->setText(QString::number(temp2));
    if(level==0x70)
    {
        // Water Level Low，图标灰掉
//        ui->label_water_icon->setPixmap(QPixmap::fromImage(QImage(":/icons/img/bottomicon/Circulation-abnormal.png")));
//        ui->label_water_text->setStyleSheet(NOT_WORK);
//        ui->label_levle_value->setText("Low");
//        waterready=false;
//        ui->label_alarm_light->setPixmap(QPixmap(QString(ICON_PIC_PATH).append("red.png")));
        ui->label_water_icon->setPixmap(QPixmap::fromImage(QImage(":/icons/img/bottomicon/Circulation.png")));
        ui->label_water_text->setStyleSheet(WORK);
        ui->label_levle_value->setText("High");
        waterready=true;
        if(powerampready&&networkready&&waterready&&locationready&&appendready)
            ui->label_alarm_light->setPixmap(QPixmap(QString(ICON_PIC_PATH).append("green.png")));

    }
    else if(level==0x71)
    {
        // Water Level Normal，图标变亮
        ui->label_water_icon->setPixmap(QPixmap::fromImage(QImage(":/icons/img/bottomicon/Circulation.png")));
        ui->label_water_text->setStyleSheet(WORK);
        ui->label_levle_value->setText("Middle");
        waterready=true;
        if(powerampready&&networkready&&waterready&&locationready&&appendready)
            ui->label_alarm_light->setPixmap(QPixmap(QString(ICON_PIC_PATH).append("green.png")));
    }
    else if(level==0x72)
    {
        ui->label_water_icon->setPixmap(QPixmap::fromImage(QImage(":/icons/img/bottomicon/Circulation.png")));
        ui->label_water_text->setStyleSheet(WORK);
        ui->label_levle_value->setText("High");
        waterready=true;
        if(powerampready&&networkready&&waterready&&locationready&&appendready)
            ui->label_alarm_light->setPixmap(QPixmap(QString(ICON_PIC_PATH).append("green.png")));
    }
    else
    {
//        ui->label_water_icon->setPixmap(QPixmap::fromImage(QImage(":/icons/img/bottomicon/Circulation-abnormal.png")));
//        ui->label_water_text->setStyleSheet(NOT_WORK);
//        waterready=false;
//        ui->label_alarm_light->setPixmap(QPixmap(QString(ICON_PIC_PATH).append("red.png")));
        ui->label_water_icon->setPixmap(QPixmap::fromImage(QImage(":/icons/img/bottomicon/Circulation.png")));
        ui->label_water_text->setStyleSheet(WORK);
        waterready=true;
        if(powerampready&&networkready&&waterready&&locationready&&appendready)
            ui->label_alarm_light->setPixmap(QPixmap(QString(ICON_PIC_PATH).append("green.png")));
    }

    emit selfCheckStatusUpdate(temp1,temp2,level);
}

void NewMainWindow::updateMeterDat(short recmeter)
{
    qDebug()<<"Meter update"<<(float)recmeter/100;
    emit MeterUpdate(recmeter);
}

 void NewMainWindow::updateLocation()
 {
     if (_location!=NULL)
     {
         _location->getPos(AXIS_L);  // 左右
         _location->getPos(AXIS_F);  // 前后
         _location->getPos(AXIS_Z);  // 上下
     }

 }

 // 设置Stop按钮可用
 void NewMainWindow::enableStopButton()
 {
     if (!ui->pushButton_stop->isEnabled())
     {
         ui->pushButton_stop->setEnabled(true);
         ui->pushButton_stop->setStyleSheet(REDCOLOR);
     }
 }

 // 设置Stop按钮不可用
 void NewMainWindow::disableStopButton()
 {
     if (ui->pushButton_stop->isEnabled())
     {
         ui->pushButton_stop->setEnabled(false);
         ui->pushButton_stop->setStyleSheet(NOT_WORK);
     }
 }

void NewMainWindow::on_status_updated(DataFlag dFlag)
{
    // TODO: update log in a file
    switch (dFlag) {
    case POWER_AMPLIFIERS_READY:
        ui->label_status->setText("Status: driver is available");
        ui->label_hifu_driver_icon->setPixmap(QPixmap::fromImage(QImage(":/icons/img/bottomicon/Ultrasound-Driver.png")));
        ui->label_hifu_driver_text->setStyleSheet(WORK);
        powerampready = true;
        break;
    case POWER_AMPLIFIERS_NOT_READY:
        ui->label_status->setText("Status: driver is nonavailable");
        ui->label_hifu_driver_icon->setPixmap(QPixmap::fromImage(QImage(":/icons/img/bottomicon/Ultrasound-Driver-abnormal.png")));
        ui->label_hifu_driver_text->setStyleSheet(NOT_WORK);
        ui->label_alarm_light->setPixmap(QPixmap(QString(ICON_PIC_PATH).append("red.png")));
        _append->setRedOn();
        qCDebug(NEWMAINWINDOW())<<NEWMAINWINDOW().categoryName()
                             <<"Error power amplifiers not ready !"
                             << TRACE_CMH();

        powerampready = false;
        break;
    case NETWORK_CONNECTED:
        ui->label_status->setText("Status: network is connected");
        ui->label_network_icon->setPixmap(QPixmap::fromImage(QImage(":/icons/img/bottomicon/Network-Interface.png")));
        ui->label_network_text->setStyleSheet(WORK);
        qDebug()<<"Network Communication***********正常";
        _subSysStatus[NET_WORK]=true;
        emit SendSubSysStatus(inetwork,true);
        servicecalled.stop();
        m_networkservice = 0;
        _append->setRedOff();
        networkready=true;
        if(powerampready&&networkready&&waterready&&locationready&&appendready)
        {
            ui->label_alarm_light->setPixmap(QPixmap(QString(ICON_PIC_PATH).append("green.png")));
            _append->setRedOff();
        }
//        Heartthread_thread->start();
        break;
    case NETWORK_DISCONNECTED:
        ui->label_status->setText("Status: network is disconnected");
        ui->label_network_icon->setPixmap(QPixmap::fromImage(QImage(":/icons/img/bottomicon/Network-Interface-abnormal.png")));
        ui->label_network_text->setStyleSheet(NOT_WORK);
        qDebug()<<"Network Communication***********不正常";
        networkready=false;
        ui->label_alarm_light->setPixmap(QPixmap(QString(ICON_PIC_PATH).append("red.png")));
        _append->setRedOn();
        qCDebug(NEWMAINWINDOW())<<NEWMAINWINDOW().categoryName()
                             <<"Error network disconnected !"
                             << TRACE_CMH();
        _subSysStatus[NET_WORK]=false;
        if(m_networkservice>=18)
        {
            emit SendSubSysStatus(inetwork,false);
        }
        else
            emit networkcheck();
        break;
    case IMAGE_GRABBER_AVAILABLE:
        ui->label_status->setText("Status: image grabber is connected");
        ui->label_grabber_icon->setPixmap(QPixmap::fromImage(QImage(":/icons/img/bottomicon/Image-Grabber.png")));
        ui->label_grabber_text->setStyleSheet(WORK);
        break;
    case IMAGE_GRABBER_NONAVAILABLE:
        ui->label_status->setText("Status: image grabber is disconnected");
        ui->label_grabber_icon->setPixmap(QPixmap::fromImage(QImage(":/icons/img/bottomicon/Image-Grabber-abnormal.png")));
        ui->label_grabber_text->setStyleSheet(NOT_WORK);
        break;
    case POSITION_AVAILABLE:
        ui->label_position_icon->setPixmap(QPixmap::fromImage(QImage(":/icons/img/bottomicon/Motion-System.png")));
        ui->label_position_text->setStyleSheet(WORK);
        break;
    case POSITION_NONAVAILABLE:
        ui->label_position_icon->setPixmap(QPixmap::fromImage(QImage(":/icons/img/bottomicon/Motion-System-abnormal.png")));
        ui->label_position_text->setStyleSheet(NOT_WORK);
        break;
    case WATER_CYCLE_AVAILABLE:
        ui->label_water_icon->setPixmap(QPixmap::fromImage(QImage(":/icons/img/bottomicon/Circulation.png")));
        ui->label_water_text->setStyleSheet(WORK);
        waterready=true;
        if(powerampready&&networkready&&waterready&&locationready&&appendready)
        {
            ui->label_alarm_light->setPixmap(QPixmap(QString(ICON_PIC_PATH).append("green.png")));
            _append->setRedOff();
        }
        break;

    case WATER_CYCLE_NONAVAILABLE:
        ui->label_water_icon->setPixmap(QPixmap::fromImage(QImage(":/icons/img/bottomicon/Circulation-abnormal.png")));
        ui->label_water_text->setStyleSheet(NOT_WORK);
        waterready=false;
        ui->label_alarm_light->setPixmap(QPixmap(QString(ICON_PIC_PATH).append("red.png")));
        _append->setRedOn();
        qCDebug(NEWMAINWINDOW())<<NEWMAINWINDOW().categoryName()
                             <<"Error water cycle not available !"
                             << TRACE_CMH();
        break;

    case PLAN_READY:
    {
        ui->label_status->setText("Status: plan is ready");
        // readyFire();
        break;
    }

    case PLAN_NOT_READY:
        ui->label_status->setText("Status: plan is not ready");
        break;

    case SONICATION_STARTED:
        ui->label_status->setText("Status: sonication is started");
        enableStopButton();
        break;

    case SONICATION_STOPPED:
        ui->label_status->setText("Status: sonication is stopped");
        qDebug()<<"main fram received sonication stopped message !";
        disableStopButton();
        break;

    case PHASE_CONTROLLER_DISABLED:
        ui->label_status->setText("Status: phase controller disabled");
        break;

    case PHASE_CONTROLLER_ENABLED:
        ui->label_status->setText("Status: phase controller enabled");
        break;

    case SPOT_COMPLETED:
        ui->label_status->setText("Status: spot is completed");
        qDebug()<<"main fram received sonication completed message !";
        disableStopButton();
        break;

    case PLAN_COMPLETED:
        ui->label_status->setText("Status: plan is completed");
        // RefreshRemaining time
        --m_totalTime.spotsRemain;
        refreshRemainTime();

        break;

    case POWER_AMPLIFIERS_AVAILABLE:
        ui->label_status->setText("Status: driver is available");
        ui->label_hifu_driver_icon->setPixmap(QPixmap::fromImage(QImage(":/icons/img/bottomicon/Ultrasound-Driver.png")));
        ui->label_hifu_driver_text->setStyleSheet(WORK);
        powerampready = true;
        break;
    case POWER_AMPLIFIERS_NONAVAILABLE:
        ui->label_status->setText("Status: driver is nonavailable");
        ui->label_hifu_driver_icon->setPixmap(QPixmap::fromImage(QImage(":/icons/img/bottomicon/Ultrasound-Driver-abnormal.png")));
        ui->label_hifu_driver_text->setStyleSheet(NOT_WORK);
        ui->label_alarm_light->setPixmap(QPixmap(QString(ICON_PIC_PATH).append("red.png")));
        _append->setRedOn();
        qCDebug(NEWMAINWINDOW())<<NEWMAINWINDOW().categoryName()
                             <<"Error power amplifier not available !"
                             << TRACE_CMH();
        powerampready = false;
        break;

    default: break;
    }
}

void NewMainWindow::GateWayConnectFailed()
{
    qDebug()<<"GateWayConnectFailed!!!!!!!!!";
    on_status_updated(NETWORK_DISCONNECTED);
    on_status_updated(POWER_AMPLIFIERS_NOT_READY);
}

void NewMainWindow::setBackground()
{
    if (_pCurrentModel==NULL)
    {
        qCritical(NEWMAINWINDOW()) << NEWMAINWINDOW().categoryName()
                                << "_pCurrentModel pointer null !"
                                << TRACE_CMH();
        return;
    }

    // Refresh表格时不检查表格内容
    disconnect(_pCurrentModel,SIGNAL(dataChanged(QModelIndex,QModelIndex)),
               this,SLOT(currentPlanChanged(QModelIndex,QModelIndex)));

    int size = _pCurrentModel->rowCount();
    for (int i = 0; i < size; ++i)
    {
        if (_currentCompleteMap.find(_pCurrentModel->item(i,0)->text())
            !=_currentCompleteMap.end()
            &&_pCurrentModel->item(i,0)->background()!=COMPLETE_COLOR)
        {
            for (int j = 0; j< 3; ++j)
            {
                _pCurrentModel->item(i,j)->setBackground(COMPLETE_COLOR);
            }

            for (int j = 3; j < 6; ++j)
            {
                _pCurrentModel->item(i,j)->setEditable(false);
                _pCurrentModel->item(i,j)->setBackground(COMPLETE_COLOR);
            }
        }
    }

    // Refresh表格时检查表格内容
    connect(_pCurrentModel,SIGNAL(dataChanged(QModelIndex,QModelIndex)),
               this,SLOT(currentPlanChanged(QModelIndex,QModelIndex)));

//    size = _pTotalModel->rowCount();
//    for (int i = 0; i < size; ++i)
//    {
//        if (_totalCompleteMap.find(_pTotalModel->item(i,0)->text())
//            !=_totalCompleteMap.end()
//            &&_pTotalModel->item(i,0)->background()!=COMPLETE_COLOR)
//        {
//            for (int j = 0; j< 5; ++j)
//            {
//                _pTotalModel->item(i,j)->setEditable(false);
//                _pTotalModel->item(i,j)->setBackground(COMPLETE_COLOR);
//            }
//        }
//    }
}

void NewMainWindow::enablestop()
{
    enableStopButton();
}

void NewMainWindow::disablestop()
{
    disableStopButton();
}

bool NewMainWindow::isNextRowContinue()
{
    int rows = _pCurrentModel->rowCount();
    for (int i = 0; i < rows; ++i)
    {
        if (_pCurrentModel->item(i,0)->background()!=COMPLETE_COLOR)
        {
            if (_pCurrentModel->item(i,5)->background()==QColor(0,255,0))
            {
                return true;
            }
            else
            {
                return false;
            }
        }
    }
}

void NewMainWindow::synImageAngle(int angle)
{
    if(angle < -90 || angle > 90)
    {
        qCDebug(NEWMAINWINDOW()) << NEWMAINWINDOW().categoryName()
                                 << "synchro image error,angle error !"<<angle
                                 << TRACE_CMH();
        return;
    }

    ui->horizontalScrollBar_images->setValue(angle+91);
    on_horizontalScrollBar_images_valueChanged(angle+91);
    if (_location!=NULL)
    {
        if (!_location->setPos(AXIS_A,angle))
        {
            qCDebug(NEWMAINWINDOW()) << NEWMAINWINDOW().categoryName()
                                  << "synchro image error,location set pos failed !"
                                  << TRACE_CMH();

        }
    }
}

bool NewMainWindow::deleteFile(QString file)
{
    QFileInfo fileInfo;
    if (fileInfo.dir().exists(file))
    {
        if (!fileInfo.dir().remove(file))  // Delete未执行的Plan
        {
            qCDebug(NEWMAINWINDOW())<<NEWMAINWINDOW().categoryName()
                                 <<"delete file failed !"<<file
                                 <<TRACE_CMH();
            return false;
        }
    }

    return true;
}

void NewMainWindow::setIrradiatingFlag(bool b)
{
    _isIrradiating = b;
    // 没有sonication，则可修改sonication参数
    if (!_isIrradiating)
    {
        setModifyPlanControlEnable();
    }
    setLocationControlEnable(!b);

    qDebug()<<"Main window Send irradiating false !";
    emit setIrradiating(_isIrradiating);
}

// Confirm开始下一个sonication point
void NewMainWindow::moveToNextSpot()
{
    on_treatment_time_remain_update();
    if (_irradiatePlan->getDisplaySize()==_currentCompleteMap.size())
    {
        // 当前显示的Plan点已sonication完,设置sonicationIndicator灯为灰色
        if (_append!=NULL)
        {
            _append->setYellowOff();
            ui->label_running_light->setPixmap(QPixmap(QString(ICON_PIC_PATH).append("green.png")));
        }
        // ExitsonicationStatus
        _isIrradiating = false;
        setLocationControlEnable(true);
        emit setIrradiating(_isIrradiating);

        // 设置Treatment相关按钮可用
        setTreatmentControlEnable();
        return;
    }

    // 如果下一行标志为绿色，则继续
    if (isNextRowContinue())
    {
        if (_core!=NULL)
        {
            qCDebug(NEWMAINWINDOW())<<NEWMAINWINDOW().categoryName()
                                 <<"from is next row continue !"
                                 << TRACE_CMH();

            _core->sendSpotParameter();
        }

        return;
    }

    // 当参数右边标志显示为红色时进入此代码
    QMessageBox msgBox("Continue",
                       "Continue the current treatment plan?",
                       QMessageBox::Question,
                       QMessageBox::Yes | QMessageBox::Default,
                       QMessageBox::No | QMessageBox::Escape,
                       QMessageBox::NoButton);
    msgBox.setWindowFlags(Qt::WindowTitleHint|Qt::CustomizeWindowHint|Qt::WindowStaysOnTopHint);
    msgBox.setGeometry(QRect(1400,400,msgBox.width(),msgBox.height()));
    msgBox.setFont(m_msgBoxFont);
    msgBox.setButtonText(QMessageBox::Yes, "OK");
    msgBox.setButtonText(QMessageBox::No, "Cancel");

    int ret = msgBox.exec();
    if (ret == QMessageBox::Yes)
    {
        if (_core!=NULL)
        {
            qCDebug(NEWMAINWINDOW())<<NEWMAINWINDOW().categoryName()
                                 <<"Enter next confirm !"
                                 << TRACE_CMH();

            _core->sendSpotParameter();
        }
    }
    else
    {
        // Stopsonication
        _isIrradiating = false;
        setLocationControlEnable(true);
        emit setIrradiating(_isIrradiating);

        // 设置Treatment相关按钮可用
        setTreatmentControlEnable();
    }
}

void NewMainWindow::on_toolButton_dqa_clicked()
{

    if (_location->initialize())
    {
        qCDebug(NEWMAINWINDOW()) << NEWMAINWINDOW().categoryName()
                              << "initialize success !"
                              << TRACE_CMH();

    }
    else
    {
        qCDebug(NEWMAINWINDOW()) << NEWMAINWINDOW().categoryName()
                              << "initialize failed !"
                              << TRACE_CMH();
    }
}

void NewMainWindow::setSpotColor(QString id,QColor color)
{
    if (_targetScene!=NULL)
    {
        _targetScene->selectedExectue(id,color);
    }



    for (int i = 0; i < _layerScenes.size(); ++i)
    {
//        _layerScenes[i]->setDrawable(true);
        if (_layerScenes[i]->selectedExectue(id,color))
        {
            switch(i)
            {
                case 0:
                    on_pushButton_treatment_layer_up_clicked();
                    break;
                case 1:
                    on_pushButton_treatment_layer_middle_clicked();
                    break;
                case 2:
                    on_pushButton_treatment_layer_down_clicked();
                    break;
            }
        }

    }
}

void NewMainWindow::on_horizontalSlider_left_right_valueChanged(int value)
{
    ui->label_left_right_value->setText(QString::number(value));
//    _location->setPos(AXIS_L, (float)ui->horizontalSlider_left_right->value());
    setLocationTips();
}

void NewMainWindow::on_horizontalSlider_forward_backward_valueChanged(int value)
{
    ui->label_forward_backward_value->setText(QString::number(value));
//    _location->setPos(AXIS_F, (float)ui->horizontalSlider_forward_backward->value());
    setLocationTips();
}

void NewMainWindow::on_horizontalSlider_up_down_valueChanged(int value)
{
//    if (_location!=NULL)
//    {
//        if (!_location->setPos(AXIS_Z,-value))
//        {
//            QString info("Set axis Z location error !");
//            info.append(QString::number(value));
//            ui->label_status->setText(info);
//        }
//        else
//            qDebug()<<"Z轴移动信号";
//    }
    ui->label_up_down_value->setText(QString::number(value));
    setLocationTips();
}

void NewMainWindow::on_horizontalSlider_left_right_sliderReleased()
{
//    _location->setPos(AXIS_L, (float)ui->horizontalSlider_left_right->value());
}

void NewMainWindow::on_horizontalSlider_forward_backward_sliderReleased()
{
//    _location->setPos(AXIS_F, (float)ui->horizontalSlider_forward_backward->value());
}

void NewMainWindow::on_location_status_updated(AID id, APOS pos)
{
    // 设置Live Image显示窗口Angle
    if (id==AXIS_A)
    {
        // 探头Angle四舍五入取整数
        int angle;
        if (pos<0.0)
        {
            angle = pos - 0.5;
        }
        else
        {
            angle = pos + 0.5;
        }
        posA=angle;
        // 设置当前超声Angle
        emit setCurrentUSAngle(angle);

        ui->label_real_time_angle->setText(QString::number(angle).append(" °"));

        _pChoseimage->setValue(angle);

        if (_refreshRealAngle!=NULL &&
            _refreshRealAngle->isActive() &&
            _destAngle==angle)
        {
            _refreshRealAngle->stop();
            _pChoseimage->setEnabled();

            // 旋转结束，设置图像采集按钮可用
            ui->toolButton_acquire_image->setEnabled(true);
            ui->toolButton_acquire_image->setStyleSheet(SELECTED);
        }

        ui->label_status->setText(QString("Status: %1 at axis %2").arg(angle).arg(id));
        if (_seriesAcquired)
        {
            _realtimeScene->setSeriesAcquired(true);
            connect(_acquisionTimer, SIGNAL(timeout()),
                    this, SLOT(on_acquireSingleFrame_completed()));
            _acquisionTimer->start();
        }

        return;
    }

    switch (id)
    {
    case AXIS_Z:
        qDebug()<<"AXISZ pos update========="<<QString::number(-pos);
        posZ=-pos;
        ui->label_up_down_value->setText(QString::number(-pos));
        ui->horizontalSlider_up_down->setValue(-pos);
        break;

    case AXIS_L:
        posL=pos;
        ui->label_left_right_value->setText(QString::number(pos));
        ui->horizontalSlider_left_right->setValue(pos);
        m_sHeaderAdjust.currentLR = pos;
        checkHeader();
        break;

    case AXIS_F:
        posF=pos;
        ui->label_forward_backward_value->setText(QString::number(pos));
        ui->horizontalSlider_forward_backward->setValue(pos);
        m_sHeaderAdjust.currentFB = pos;
        checkHeader();
        break;

    default:
        break;
    }
    emit updateHeadPos(posL,posF,posZ);
}

void NewMainWindow::checkHeader()
{
    if (m_sHeaderAdjust.isAdjust)
    {
        if (fabs(m_sHeaderAdjust.currentFB-m_sHeaderAdjust.destFB)<0.01
            && fabs(m_sHeaderAdjust.currentLR-m_sHeaderAdjust.destLR)<0.01)
        {
            on_toolButton_acquire_image_clicked();
            m_sHeaderAdjust.isAdjust = false;
            m_sHeaderAdjust.destFB = 0.0;
            m_sHeaderAdjust.destLR = 0.0;
        }
    }
}

void NewMainWindow::on_location_error_updated(ERRORTYPE err) {
    QString str = QString("Status: location error (%1)").arg(err);
    ui->label_status->setText(str);
}

void NewMainWindow::on_toolButton_save_image_2_clicked()
{
//    _grabber->cutscreen = true;
    if (_realtimeScene!=NULL)
    {
        // _realtimeScene->setFrameAcquired(true);
        QImage image = _realtimeScene->getPixmap().toImage();
        FileSaveDialog *fdig=new FileSaveDialog(PA_IMAGE);
        if(fdig->exec()!=QDialog::Accepted)
        {
            delete fdig;
            return;
        }
        QString fFullpath=PatientManager::getInstance()->getImageFullPath();
        qDebug()<<"fFullpath:"<<fFullpath;
        if(image.save(fFullpath))
        {
            QMessageBox msgBox("Notice",
                               "Image saved.",
                               QMessageBox::Information,
                               QMessageBox::Ok | QMessageBox::Escape,
                               QMessageBox::NoButton,
                               QMessageBox::NoButton);
            msgBox.setWindowFlags(Qt::WindowTitleHint|Qt::CustomizeWindowHint|Qt::WindowStaysOnTopHint);
            msgBox.setGeometry(QRect(1400,400,msgBox.width(),msgBox.height()));
            msgBox.setFont(m_msgBoxFont);
            msgBox.setButtonText(QMessageBox::Ok, "OK");
            msgBox.exec();
//            ui->toolButton_save_image_2->setText(QString("Save成功"));
//            QTimer::singleShot(500,[this](){
//                ui->toolButton_save_image_2->setText(QString("Save Image"));
//            });
        }
        else{
            QMessageBox msgBox("Notice",
                               "Image could not be saved.",
                               QMessageBox::Information,
                               QMessageBox::Ok | QMessageBox::Escape,
                               QMessageBox::NoButton,
                               QMessageBox::NoButton);
            msgBox.setWindowFlags(Qt::WindowTitleHint|Qt::CustomizeWindowHint|Qt::WindowStaysOnTopHint);
            msgBox.setGeometry(QRect(1400,400,msgBox.width(),msgBox.height()));
            msgBox.setFont(m_msgBoxFont);
            msgBox.setButtonText(QMessageBox::Ok, "OK");
            msgBox.exec();
//            ui->toolButton_save_image_2->setText(QString("Save失败"));
//            QTimer::singleShot(500,[this](){
//                ui->toolButton_save_image_2->setText(QString("Save Image"));
//            });
        }
        delete fdig;
//        QString file = "E:\\woman";
//        file.append(QTime::currentTime().toString("HH_mm_ss"));
//        file.append(".bmp");
//        image.save(file);
    }
}

void NewMainWindow::on_toolButton_acquire_image_clicked()
{
    _seriesAcquired = true;
    if(TID == "")
        TID = uuid(2).toString();
    is_plan_completed=true;
    // 采集图像时设置Measurement Analysis模块不可用
    ui->tabWidget_operator->setTabEnabled(0,false);
    //Positioning后无法直接Back to Information Management
    ui->pushButton_return->setEnabled(false);
    ui->pushButton_return->setStyleSheet(NOT_WORK);
    if(!Auto_WaterLock)
    {
        Image_WaterLock = true;
        controllock();
    }

    // 采集期间，直方图分析不可用
    ui->pushButton_histogram->setEnabled(false);
    ui->pushButton_histogram->setStyleSheet(NOT_WORK);

    //设置Zoom in、Zoom out、Show Acoustic Window和Hide Acoustic Window按钮可用
    ui->toolButton_zoomout_image->setEnabled(true);
    ui->toolButton_zoomout_image->setStyleSheet(SELECTED);
    ui->toolButton_zoomin_image->setEnabled(true);
    ui->toolButton_zoomin_image->setStyleSheet(SELECTED);
    ui->toolButton_showpath->setEnabled(true);
    ui->toolButton_showpath->setStyleSheet(SELECTED);
    ui->toolButton_hidepath->setEnabled(true);
    ui->toolButton_hidepath->setStyleSheet(SELECTED);

    ui->pushButton_headL->setEnabled(false);
    ui->pushButton_headR->setEnabled(false);
    ui->pushButton_headU->setEnabled(false);
    ui->pushButton_headD->setEnabled(false);
    ui->pushButton_headF->setEnabled(false);
    ui->pushButton_headB->setEnabled(false);

    ui->toolButton_header_zero->setEnabled(false);
    ui->toolButton_header_zero->setStyleSheet(NOT_WORK);
    _pHeaderControl->close();
    ui->toolButton_change_image_plane->setEnabled(false);
    ui->toolButton_change_image_plane->setStyleSheet(NOT_WORK);
    _pChoseimage->close();

    //_startTime = QTime::currentTime().toString("hh:mm:ss");
    // 设置初Plan始角为-90度
    //新建一个当前Time命名的文件夹，用以存储Ultrasound Image
    QDateTime time;
    QDir dirtemp;
    QString str_time = time.currentDateTime().toString("yy_MMdd_hhmmss");
    QString save_path = "D:/HIFU-V1.0/Image/";
    save_path.append(str_time);
    imagePath = save_path;
    QDir dir(save_path);
    if (!dir.exists())
    {
        dir.mkpath(save_path);
    }
    QString path = save_path;
    dir.mkpath(path);
    qDebug()<<"新建一个文件夹："<<path;
    if(_realtimeScene !=NULL)
    {
        _realtimeScene->setfoldpath(path);
    }
    // 如果Control Box黄灯在sonicationStatus,关闭黄灯
    if (_append!=NULL&&_append->isYellowOn())
    {
        _append->setYellowOff();
        ui->label_running_light->setPixmap(QPixmap(QString(ICON_PIC_PATH).append("green.png")));
    }
    initRealtimeMenu();    // 设置Live Image右键菜单可用

    // 设置Treatment Report按钮不可用
    if (ui->pushButton_treatment_report->isEnabled())
    {
        ui->pushButton_treatment_report->setDisabled(true);
        ui->pushButton_treatment_report->setStyleSheet(NOT_WORK);
    }
    resetScenes();
    if (_realtimeScene!=NULL)
    {
        _realtimeScene->resetFrameIndex();    // 从-90度开始扫描
    }
    if (!_location->setPos(AXIS_A, _realtimeScene->angle()))
    {
        qCDebug(NEWMAINWINDOW()) << NEWMAINWINDOW().categoryName()
                                 << "location set pos -90 error !"
                                 << TRACE_CMH();
    }

    // 设置target图像Edit按钮可用
    // setSingleFrameEditEnable();
    // 设置target右键菜单可用
    initTargetMenu();

    // 采集图像结束，设置水平滑动条正常，图像Select功能正常，图像张数显示正常
    if(!ui->horizontalScrollBar_images->isEnabled())
    {
        ui->horizontalScrollBar_images->setEnabled(true);
        ui->horizontalScrollBar_images->setStyleSheet(SELECTED);
        ui->pushButton_front->setEnabled(true);
        ui->pushButton_front->setStyleSheet(SELECTED_NO_ROUND);
        ui->pushButton_back->setEnabled(true);
        ui->pushButton_back->setStyleSheet(SELECTED_NO_ROUND);

        // 设置声束和Transducer图标可用
//        ui->toolButton_beam_path->setEnabled(true);
//        ui->toolButton_beam_path->setStyleSheet(SELECTED);

//        ui->toolButton_transducer->setEnabled(true);
//        ui->toolButton_transducer->setStyleSheet(SELECTED);
    }
    if(!(LogHeartTimer->isActive()))
    {
        LogHeartTimer->start();
    }
    if (_targetMenu!=NULL){
        _targetMenu->setEnabled(false);
    }
    if (_realTimeMenu!=NULL){
        _realTimeMenu->setEnabled(false);
    }
    if (_layerMenu!=NULL){
        _layerMenu->setEnabled(false);
    }
}

void NewMainWindow::on_horizontalScrollBar_images_sliderReleased()
{
    // Restorescene和测量按钮Status
    resumeMeasureLength();

    int angle = ui->horizontalScrollBar_images->value()-91;
    QMap<int,DrawScene*>::iterator iter = _sceneMap.find(angle);
    if (iter == _sceneMap.end())
    {
        return;
    }
    else
    {
        _targetScene = _sceneMap.find(angle).value();
//        connect(this,SIGNAL(spotStatusChanged(QString,QColor)),_targetScene,SLOT(selectedExectue(QString,QColor)));
    }

    if (_targetScene!=NULL)
    {
        QString str(QString::number(angle));
        str.append(" °");
        ui->label_select_image_angle->setText(str);
        _targetScene->setBeamVisible(stat._beamVisiable);
        _targetScene->setContoursVisiable(_contoursVisiable);
        _targetScene->setTransducerVisible(stat._transducerVisiable);

        // 如果Delete按钮可用，则scene可Edit
        _targetScene->setDrawable(stat._bDrawable);
        _targetScene->setTargetDrawable(stat._bTargetDrawable);
        _targetScene->setMeasureDrawable(stat._bMeasureable);

        if (!_targetScene->equalViewer2D(_viewer2DInfo))
        {
            _targetScene->setViewer2D(_viewer2DInfo);
        }

        ui->graphicsView_target_scene->setScene(_targetScene);
        QPixmap usimg=_targetScene->getPixmap();
        if(_newScene3d!=NULL&&_newScene3d->isInit)
        {
            _newScene3d->ChangeUsImg((double)angle);
        }

    }
}

void NewMainWindow::on_toolButton_transducer_clicked()
{
    // 将Transducer显示标志取反
    stat._transducerVisiable = !stat._transducerVisiable;
    _targetScene->setTransducerVisible(stat._transducerVisiable);
}

void NewMainWindow::spotSelectedInTarget(QString id,QColor color)
{
    if (_targetSceneMap.empty())
    {
        qCDebug(NEWMAINWINDOW()) << NEWMAINWINDOW().categoryName()
                                 << "target scene map empty !"
                                 << TRACE_CMH();
        return;
    }

    QMap<int,DrawScene*>::iterator iter = _targetSceneMap.begin();
    for (; iter != _targetSceneMap.end(); ++iter)
    {
        if (iter.value()->selectedExectue(id,color))
        {
            _targetScene = iter.value();
            ui->graphicsView_target_scene->setScene(_targetScene);
            ui->horizontalScrollBar_images->setValue(iter.key()+91);
            return;
        }
    }
}

void NewMainWindow::spotSelected(QString id,QColor color)
{
    if (_layerScenes.empty())
    {
        qCDebug(NEWMAINWINDOW()) << NEWMAINWINDOW().categoryName()
                                 << "layerScenes empty !"
                                 << TRACE_CMH();
        return;
    }

    for (int i = 0; i < _layerScenes.size(); ++i)
    {
        if (_layerScenes[i]->selectedExectue(id,color))
        {
            switch(i)
            {
            case 2:
                on_pushButton_treatment_layer_down_clicked();
                return;

            case 1:
                on_pushButton_treatment_layer_middle_clicked();
                return;

            case 0:
                on_pushButton_treatment_layer_up_clicked();
               return;

            default:
                return;
            }

        }
    }
}

void NewMainWindow::on_toolButton_beam_path_clicked()
{
    stat._beamVisiable = !stat._beamVisiable;
    if (_targetScene!=NULL)
    {
        _targetScene->setBeamVisible(stat._beamVisiable);
    }
}

void NewMainWindow::setScenesUndrawable()
{
    QMap<int,DrawScene*>::iterator mapIter = _sceneMap.begin();
    for (;mapIter != _sceneMap.end();++mapIter)
    {
        if (mapIter.value()->drawable())
        {
            mapIter.value()->setDrawable(false);
        }

        if (mapIter.value()->targetDrawable())
        {
            mapIter.value()->setTargetDrawable(false);
        }
    }
}

// 创建Patients对应的Plan文件夹，如果文件夹创建Time不超过一小时，
// 则认为是相同的Patients，否则Delete重新创建
void NewMainWindow::createPlanFold()
{
    deletePlanFold();    // Delete以前SavePlan的文件夹
    QDir fold;
    if (!fold.exists(PLAN_FOLD))
    {
        fold.mkdir(PLAN_FOLD);
    }
}

// Deleteplan文件夹
void NewMainWindow::deletePlanFold()
{
    QDir dir(PLAN_FOLD);
    if (!dir.exists())
    {
        return;
    }
    QStringList fileList;
    QList<QFileInfo> fileInfos(dir.entryInfoList(fileList));
    foreach(QFileInfo fileInfo, fileInfos)
    {
        if (fileInfo.fileName().size()>5)
        {
            fileInfo.dir().remove(fileInfo.fileName());
            qCDebug(NEWMAINWINDOW()) << NEWMAINWINDOW().categoryName()
                                  << "delete file "<<fileInfo.fileName()
                                  << TRACE_CMH();
        }
    }

    dir.rmpath(dir.absolutePath());
    qCDebug(NEWMAINWINDOW()) << NEWMAINWINDOW().categoryName()
                          << "delete fold "<<dir.absolutePath()
                          << TRACE_CMH();
}

void NewMainWindow::showDeletedElement()
{
    QMap<int,DrawScene*>::iterator iter = _targetSceneMap.begin();
    for (; iter != _targetSceneMap.end(); ++iter)
    {
        iter.value()->showSpot();
    }

    for (int i = 0; i < _layerScenes.size(); ++i)
    {
        _layerScenes[i]->showSpot();
    }
}

void NewMainWindow::hideDeletedElement()
{
    QMap<int,DrawScene*>::iterator iter = _targetSceneMap.begin();
    for (; iter != _targetSceneMap.end(); ++iter)
    {
        iter.value()->hideSpot();
    }

    for (int i = 0; i < _layerScenes.size(); ++i)
    {
        _layerScenes[i]->hideSpot();
    }
}

void NewMainWindow::combinePlan(DENSITY_TYPE density)
{
    QQueue<double> x;
    QQueue<double> y;
    QQueue<double> z;
    QQueue<Circle> circles;
    QQueue<QString> idBorn;

    DrawScene* tempScene = NULL;
    QMap<int,DrawScene*>::iterator iter = _targetSceneMap.begin();
    for (; iter != _targetSceneMap.end(); ++iter)
    {
        tempScene = iter.value();
        if (tempScene==NULL)
        {
            continue;
        }
        QQueue<double> xTemp = tempScene->getFocusX();
        QQueue<double> yTemp = tempScene->getFocusY();
        QQueue<double> zTemp = tempScene->getFocusZ();
        QQueue<Circle> circleTemp = tempScene->getCircles();
        QQueue<QString> idBornTemp = tempScene->getIdBorn();
        for (int i = 0; i < xTemp.size(); ++i)
        {
            x << xTemp[i];
            y << yTemp[i];
            z << zTemp[i];
            circles << circleTemp[i];
            idBorn << idBornTemp[i];
        }
    }

    // GenerateTreatment PlaneSonication Point Distribution图
    if (!_layerScenes.empty())
    {
        _layerScenes[0]->clearExamplePoint();
        _layerScenes[1]->clearExamplePoint();
        _layerScenes[2]->clearExamplePoint();
    }
    else
    {
        DrawScene* upScene = new DrawScene(0);
        upScene->setDensity(density);
        upScene->setSceneType(SCENE_LAYER);
        upScene->setDrawable(true);
//        upScene->setTargetDrawable(true);

        DrawScene* middleScene = new DrawScene(0);
        middleScene->setDensity(density);
        middleScene->setDrawable(true);
//        middleScene->setTargetDrawable(true);
        middleScene->setSceneType(SCENE_LAYER);

        DrawScene* downScene = new DrawScene(0);
        downScene->setDensity(density);
        downScene->setDrawable(true);
//        downScene->setTargetDrawable(true);
        downScene->setSceneType(SCENE_LAYER);

        // 以focus所在sonicationPlane为中心，上下裁剪，向上10mm为上一sonicationPlane，向下10mm为下一sonicationPlane
        QImage downImage(_pHifu3d->GetImage(HalfClipHigh+10*RESOLUTION),CLIP_WIDE,CLIP_HIGH,QImage::Format_Indexed8);
        QVector<QRgb> colorTbable;
        unsigned int rgb = 0;
        for (int i = 0; i <256;++i)
        {
            colorTbable.append(rgb);
            rgb+=0x00010101;
        }
        downImage.setColorTable(colorTbable);
        QImage middleImage(_pHifu3d->GetImage(HalfClipHigh),CLIP_WIDE,CLIP_HIGH,QImage::Format_Indexed8);
        QImage upImage(_pHifu3d->GetImage(HalfClipHigh-10*RESOLUTION),CLIP_WIDE,CLIP_HIGH,QImage::Format_Indexed8);

        middleImage.setColorTable(colorTbable);
        upImage.setColorTable(colorTbable);

//        QMatrix matrix;
//        matrix.rotate(145);
//        middleImage = middleImage.transformed(matrix);
//        upImage = upImage.transformed(matrix);
//        downImage = downImage.transformed(matrix);
        QImage upMirror = upImage.mirrored(true,true);
        QImage middleMirror = middleImage.mirrored(true,true);
        QImage downMirror = downImage.mirrored(true,true);

        downMirror.save("../resources/downimage.bmp","bmp");
        middleMirror.save("../resources/middleimage.bmp","bmp");
        upMirror.save("../resources/upimage.bmp","bmp");

//        downImage.save("E:\\downimage.bmp","bmp");
//        middleImage.save("E:\\middleimage.bmp","bmp");
//        upImage.save("E:\\upimage.bmp","bmp");

        downScene->setBackgroundPixmap(QPixmap::fromImage(QImage("../resources/upimage.bmp")));
        middleScene->setBackgroundPixmap(QPixmap::fromImage(QImage("../resources/middleimage.bmp")));
        upScene->setBackgroundPixmap(QPixmap::fromImage(QImage("../resources/downimage.bmp")));

        connect(downScene,SIGNAL(changeView2D(viewer2D_info)),this,SLOT(changeLayerViewer(viewer2D_info)));
        connect(downScene,SIGNAL(spotSelected(QString,QColor)),this,SLOT(spotSelectedInTarget(QString,QColor)));
        connect(middleScene,SIGNAL(changeView2D(viewer2D_info)),this,SLOT(changeLayerViewer(viewer2D_info)));
        connect(middleScene,SIGNAL(spotSelected(QString,QColor)),this,SLOT(spotSelectedInTarget(QString,QColor)));
        if (_planLayerScene!=NULL)
        {
            connect(middleScene,SIGNAL(getDrawEdges()),_planLayerScene,SLOT(getDrawEdges()));
            connect(_planLayerScene,SIGNAL(setDrawEdges(QPainterPath)),middleScene,SLOT(setDrawEdges(QPainterPath)));
        }
        connect(upScene,SIGNAL(changeView2D(viewer2D_info)),this,SLOT(changeLayerViewer(viewer2D_info)));
        connect(upScene,SIGNAL(spotSelected(QString,QColor)),this,SLOT(spotSelectedInTarget(QString,QColor)));

        _layerScenes<<downScene;
        _layerScenes<<middleScene;
        _layerScenes<<upScene;

    }

    bool flags[3] = {false};
    // _3DStatus.clear();
    for (size_t i = 0;i<z.size();++i)
    {
        // _3DStatus.insert(idBorn[i],Spot_3d_Status(x[i],y[i],z[i],UNIRADIATE_COLOR));

        if (z[i]>=0.145 && z[i]<0.155)
        {
            // LowerTreatment平面
            _layerScenes[2]->addExamplePoint(idBorn[i],QPointF(x[i],y[i]),HALF_CLIP_WIDE);
            flags[0] = true;

        }
        else if (z[i]<0.145 && z[i]>=0.135)
        {
            // MiddleTreatment平面
            _layerScenes[1]->addExamplePoint(idBorn[i],QPointF(x[i],y[i]),HALF_CLIP_WIDE);
            flags[1] = true;

        }
        else if(z[i]<0.135 && z[i]>=0.125)
        {
            // UpperTreatment平面
            _layerScenes[0]->addExamplePoint(idBorn[i],QPointF(x[i],y[i]),HALF_CLIP_WIDE);
            flags[2] = true;
        }
    }

    qDebug()<<"sys draw edges !";
    _layerScenes[1]->sysDrawEdges();
    if (flags[0])
    {
        on_pushButton_treatment_layer_down_clicked();
        return;
    }
    else if (flags[1])
    {
        on_pushButton_treatment_layer_middle_clicked();
        return;
    }
    else if (flags[2])
    {
        on_pushButton_treatment_layer_up_clicked();
        return;
    }
    else
    {
        on_pushButton_treatment_layer_down_clicked();
        return;
    }

}

// 显示TreatmentPlan，包括当前和整体
void NewMainWindow::showPlan()
{
    if (_irradiatePlan==NULL)
    {
        qCDebug(NEWMAINWINDOW())<<NEWMAINWINDOW().categoryName()
                             <<"irradiate plan pointer null !"
                             << TRACE_CMH();
        return;
    }

    if (_pCurrentModel==NULL)
    {
        qCDebug(NEWMAINWINDOW())<<NEWMAINWINDOW().categoryName()
                             <<"current model pointer null !"
                             << TRACE_CMH();
        return;
    }

//    if (_pTotalModel==NULL)
//    {
//        qCDebug(NEWMAINWINDOW())<<NEWMAINWINDOW().categoryName()
//                             <<"total model pointer null !"
//                             << TRACE_CMH();
//        return;
//    }

    QQueue<Irradiate_Spot> currentPlan;
    _irradiatePlan->getNextPlan(currentPlan);
    refreshCurrentTable(currentPlan);
    // refreshTotalTable();

    // 同步Live Image和Save的图像
    int angle;
    if (_irradiatePlan->getDisplayAngle(angle))
    {
        ui->horizontalScrollBar_images->setValue(angle+91);
        on_horizontalScrollBar_images_valueChanged(angle+91);
        if (_location!=NULL)
        {
            if (!_location->setPos(AXIS_A,angle))
            {
                qCDebug(NEWMAINWINDOW()) << NEWMAINWINDOW().categoryName()
                                      << "synchro image error,location set pos failed !"
                                      << TRACE_CMH();
            }
        }
    }
}

void NewMainWindow::realTimeZoomOut()
{
    if (_realtimeScene!=NULL)
    {
        _realtimeScene->zoomOut();
    }
}

void NewMainWindow::realTimeZoomIn()
{
    if (_realtimeScene!=NULL)
    {
        _realtimeScene->zoomIn();
    }
}

void NewMainWindow::realTimeShowPath()  // 显示Live Image上声束
{
    if (_realtimeScene!=NULL)
    {
        _realtimeScene->setBeamVisible(true);
    }
}

void NewMainWindow::realTimeHidePath()  // 隐藏Live Image上声束
{
    if (_realtimeScene!=NULL)
    {
        _realtimeScene->setBeamVisible(false);
    }
}

void NewMainWindow::realTimeShowFocus()
{
    if (_realtimeScene!=NULL)
    {
        _realtimeScene->showFocus();
    }
}

void NewMainWindow::realTimeShowRange()
{
    if (_realtimeScene!=NULL)
    {
        _realtimeScene->showRange();
    }
}

void NewMainWindow::realTimeHideRange()
{
    if (_realtimeScene!=NULL)
    {
        _realtimeScene->hideRange();
    }
}

void NewMainWindow::realTimeHideFocus()
{
    if (_realtimeScene!=NULL)
    {
        _realtimeScene->hideFocus();
    }
}

void NewMainWindow::realTimeResume()
{
    if (_realtimeScene!=NULL)
    {
        _realtimeScene->resumeImage();
    }
}

void NewMainWindow::changeImage()
{
    QMessageBox msgBox("Change Planning Image",
                       "Change the planning image?",
                       QMessageBox::Question,
                       QMessageBox::Yes | QMessageBox::Default,
                       QMessageBox::No | QMessageBox::Escape,
                       QMessageBox::NoButton);
    msgBox.setWindowFlags(Qt::WindowTitleHint|Qt::CustomizeWindowHint|Qt::WindowStaysOnTopHint);
    msgBox.setGeometry(QRect(1400,400,msgBox.width(),msgBox.height()));
    msgBox.setFont(m_msgBoxFont);
    msgBox.setButtonText(QMessageBox::Yes, "OK");
    msgBox.setButtonText(QMessageBox::No, "Cancel");

    int ret = msgBox.exec();
    if (ret == QMessageBox::No)
    {
        return;
    }

    QString angleStr = ui->label_real_time_angle->text();
    int angle = angleStr.mid(0,angleStr.size()-2).toInt();

    DrawScene* tempScene = NULL;
    tempScene = new DrawScene(angle);
    tempScene->setBackgroundPixmap(_realtimeScene->getPixmap());
    connect(tempScene,SIGNAL(changeView2D(viewer2D_info)),this,SLOT(changeView2D(viewer2D_info)));

    SPOT_DISTRIBUTE dist;
    dist.setAllSpots(true);
    if (angle!=0)  // 非零度角，不Addsonication中轴线上的sonication point
    {
        dist.setCenter(false);
    }
    tempScene->drawSpots(dist);

    QMap<int,DrawScene*>::iterator iter = _sceneMap.find(angle);
    if (iter != _sceneMap.end())
    {
        delete iter.value();
    }
    _sceneMap.insert(angle,tempScene);

    _targetScene = tempScene;
    ui->graphicsView_target_scene->setScene(_targetScene);
    QString str(QString::number(angle));
    str.append(" °");
    ui->label_select_image_angle->setText(str);
    ui->horizontalScrollBar_images->setValue(angle+91);
    on_horizontalScrollBar_images_sliderReleased();
}

void NewMainWindow::setProgressRate(int rate)
{
    ui->progressBar_cooling_time->setValue(rate);
}

void NewMainWindow::setRemainCoolingTimeValue(int value)
{
    ui->label_remain_cooling_time_value->setText(QString::number(value));
}

void NewMainWindow::setBlockTableRowColor(QString id, QColor color)
{
    if (_pBlockModel==NULL)
    {
        qCritical(NEWMAINWINDOW()) << NEWMAINWINDOW().categoryName()
                                   << "_pBlockModel pointer null !"
                                   << TRACE_CMH();
        return;
    }

    int size = _pBlockModel->rowCount();
    for (int i = 0; i < size; ++i)
    {
        if (_pBlockModel->item(i,0)->text()==id)
        {
            for (int j = 0; j< 3; ++j)
            {
                _pBlockModel->item(i,j)->setBackground(color);
            }

            break;
        }
    }
}

// 设置表格sonication相关行颜色
void NewMainWindow::setTableRowColor(QString id, QColor color)
{
    if (color==COMPLETE_COLOR)
    {
        _currentCompleteMap.insert(id,true);
        _totalCompleteMap.insert(id,true);
        ++_totalSpot;
        ++_seriesSpot;
        ++treatedSpot;
        synReportInfo();

        setBackground();
    }
    else
    {
        if (_pCurrentModel==NULL)
        {
            qCritical(NEWMAINWINDOW()) << NEWMAINWINDOW().categoryName()
                                       << "_pCurrentModel pointer null !"
                                       << TRACE_CMH();
            return;
        }

        // 设置表格颜色时，不更新表格内容
        disconnect(_pCurrentModel,SIGNAL(dataChanged(QModelIndex,QModelIndex)),
                   this,SLOT(currentPlanChanged(QModelIndex,QModelIndex)));

        int size = _pCurrentModel->rowCount();
        for (int i = 0; i < size; ++i)
        {
            if (_pCurrentModel->item(i,0)->text()==id)
            {
                for (int j = 0; j< 3; ++j)
                {
                    _pCurrentModel->item(i,j)->setBackground(color);
                }

                for (int j = 3; j < 6; ++j)
                {
                    _pCurrentModel->item(i,j)->setEditable(false);
                    _pCurrentModel->item(i,j)->setBackground(color);
                }

                break;
            }
        }

        connect(_pCurrentModel,SIGNAL(dataChanged(QModelIndex,QModelIndex)),
                           this,SLOT(currentPlanChanged(QModelIndex,QModelIndex)));
    }


}

// Save制定Plan的目标图像
void NewMainWindow::saveTargetImages()
{

    QString imagesPath=PatientManager::getInstance()->getImagePath();
    QString timeStamp = PatientManager::getInstance()->getCurDatetimeNmae();
    imagesPath.append(timeStamp);
    imagesPath.append("/");
    PatientManager::getInstance()->isDirExist(imagesPath);

    outputTreatmentLog(imagesPath);
    QMap<int,DrawScene*>::iterator iter = _targetSceneMap.begin();
    for (iter = _sceneMap.begin();iter != _sceneMap.end();++iter)
    {
        QString fileName(imagesPath);
        fileName.append(QString::number(iter.key()));
        fileName.append(".bmp");
        QImage image(iter.value()->getPixmap().toImage());
        image.save(fileName);
    }
}

void NewMainWindow::removeDeleteSpots()
{
    QMap<int,DrawScene*>::iterator iter = _targetSceneMap.begin();
    for (; iter != _targetSceneMap.end(); ++iter)
    {
        iter.value()->removeDeletedSpots();
    }

    for (int i = 0; i < _layerScenes.size(); ++i)
    {
        _layerScenes[i]->removeDeletedSpots();
    }
}

void NewMainWindow::on_toolButton_therapy_clicked()
{
    QMessageBox msgBox("Ready for Sonication",
                       "Ready to start sonication?",
                       QMessageBox::Question,
                       QMessageBox::Yes | QMessageBox::Default,
                       QMessageBox::No | QMessageBox::Escape,
                       QMessageBox::NoButton);
    msgBox.setWindowFlags(Qt::WindowTitleHint|Qt::CustomizeWindowHint|Qt::WindowStaysOnTopHint);
    msgBox.setGeometry(QRect(1400,400,msgBox.width(),msgBox.height()));
    msgBox.setFont(m_msgBoxFont);
    msgBox.setButtonText(QMessageBox::Yes, "OK");
    msgBox.setButtonText(QMessageBox::No, "Cancel");

    int ret = msgBox.exec();
    if (ret == QMessageBox::No)
    {
        return;
    }

    if(!_newScene3d->isInit)
    {
        _newScene3d->InitScene3D();
    }

    // SavePlanning Image数据
    // saveTargetImages();
    // sonication开始后Delete不要的右键菜单选项
    removeUnavailableMenu();
    //QString sTumorStlPath=QApplication::applicationDirPath()+"/Tumor3d.stl";
    //_newScene3d->LoadTumorStl(sTumorStlPath.toStdString().c_str());
    // 点击Treatment按钮后Treatment按钮设置为不可用
    ui->toolButton_therapy->setDisabled(true);
    ui->toolButton_therapy->setStyleSheet(NOT_WORK);

    // Start treatmentBack，设置targetScene中所有图像为不可Edit
    // setScenesUndrawable();

    // DeleteDelete掉的sonication point
    removeDeleteSpots();

    //设置层图像不可Edit
    for (int i = 0; i < _layerScenes.size(); ++i)
    {
        _layerScenes[i]->setDrawable(false);
        _layerScenes[i]->setTargetDrawable(false);
    }

    initIrradiatePlan();
    initTableMenu();

    showPlan();

    connect(_pCurrentModel,SIGNAL(dataChanged(QModelIndex,QModelIndex)),
            this,SLOT(currentPlanChanged(QModelIndex,QModelIndex)));

    // 设置对比图像scene

    // 显示3d图形
    {

#if 0
//        clock_t time = clock();
        uchar* data = new uchar[CLIP_WIDE*CLIP_HIGH*181];
        memset(data,0,CLIP_WIDE*CLIP_HIGH*181);
        createMask(data);

/*        clock_t time1 = clock();
        qDebug()<<"step 1 time consume is "<< time1-time*/;

        if (_pHifu3d!=NULL)
        {
            _pMesh = _pHifu3d->getMesh(data);
            // _pMesh->WriteStl("E:\\surfaceraw.stl");
            _pMesh->MoveCenter();

            Smooth algo(_pMesh);
            algo.Taubin(3,0.9,0.5);
            // _pMesh->WriteStl("E:\\surface.stl");

            if (_pMesh->m_vVerNormal.empty())
            {
                _pMesh->CalcVerNomal();
            }

        }

        delete []data;
#endif

        // QSurfaceFormat format;
//        format.setDepthBufferSize(24);
//        format.setStencilBufferSize(8);
//        format.setVersion(3, 2);
//        format.setProfile(QSurfaceFormat::CoreProfile);
//        ui->openGLWidget_3d->setFormat(format);
//        _pScene3d = new Scene3d(_3DStatus,ui->openGLWidget_3d);
//        _pScene3d->show();

    }

    // 点了Treatment之后开始和Stop可用
    if (!ui->pushButton_start_treat->isEnabled())
    {
        setTreatmentControlEnable();

        setModifyPlanControlEnable();
//        ui->pushButton_modify_para->setEnabled(true);
//        ui->pushButton_modify_para->setStyleSheet(SELECTED);

//        ui->pushButton_replan->setEnabled(true);
//        ui->pushButton_replan->setStyleSheet(SELECTED);

//        ui->pushButton_confirm_para->setEnabled(true);
//        ui->pushButton_confirm_para->setStyleSheet(SELECTED);
    }

    // 设置TreatmentPlan制定相关按钮不可用
    setWholePlanControlDisable();

    // GenerateTreatment过程控制模块
//    _core = new IrradiateCore(_irradiatePlan,_append,_schedule,_plan,_msgCtrl);
    if(!flag1)
    {
    connect(_core,SIGNAL(setProgressRate(int)),this,SLOT(setProgressRate(int)));
    connect(_core,SIGNAL(setRemainCoolingTime(int)),this,SLOT(setRemainCoolingTimeValue(int)));
    connect(_core,SIGNAL(moveToNextSpot()),this,SLOT(moveToNextSpot()));
    connect(_core,SIGNAL(setIrradiatingFlag(bool)),this,SLOT(setIrradiatingFlag(bool)));
    connect(_core,SIGNAL(setLighting(APPEND_LABEL,bool)),this,SLOT(setLighting(APPEND_LABEL,bool)));
    disconnect(_append,SIGNAL(switchChanged(bool)),this,SLOT(switchChanged(bool)));
    connect(_core,SIGNAL(setTableRowColor(QString,QColor)),this,SLOT(setTableRowColor(QString,QColor)));
    connect(_core, SIGNAL(setSpotColor(QString,QColor)),this,SLOT(setSpotColor(QString,QColor)));
    connect(_core, SIGNAL(setSpotColor(QString,QColor)),_newScene3d,SLOT(setSpotColor(QString,QColor)));
    connect(_core,SIGNAL(setTreatmentControlEnable()),this,SLOT(setTreatmentControlEnable()));
    connect(this,SIGNAL(setIsPause(bool)),_core,SLOT(setIsPause(bool)));
    connect(this,SIGNAL(setCurrentUSAngle(int)),_core,SLOT(setCurrentUSAngle(int)));
    connect(this,SIGNAL(setIrradiating(bool)),_core,SLOT(setIrradiating(bool)));
    connect(_core,SIGNAL(synImageAngle(int)),this,SLOT(synImageAngle(int)));
    connect(_core,SIGNAL(startsonication()),this,SLOT(enablestop()));
    connect(_core,SIGNAL(stop_sonication()),this,SLOT(disablestop()));
    flag1=true;
    }

    // 设置targetScene图像为不可Edit
    if (_targetScene!=NULL)
    {
        _targetScene->setDrawable(false);
        _targetScene->setTargetDrawable(false);
    }
}

void NewMainWindow::setLighting(APPEND_LABEL label,bool status)
{
    if (!status)
    {
        switch (label)
        {
        case LABEL_RED:
            ui->label_alarm_light->setPixmap(QPixmap(QString(ICON_PIC_PATH).append("green.png")));
            return;

        case LABEL_YELLOW:
            ui->label_running_light->setPixmap(QPixmap(QString(ICON_PIC_PATH).append("green.png")));
            return;

        case LABEL_GREEN:
            ui->label_power_light->setPixmap(QPixmap(QString(ICON_PIC_PATH).append("green.png")));
            return;

        case SWITCH:
            ui->label_switch->setPixmap(QPixmap(QString(ICON_PIC_PATH).append("踏板关.png")));
            return;

        default:
            break;
        }
    }
    else
    {
        switch (label)
        {
        case LABEL_RED:
            ui->label_alarm_light->setPixmap(QPixmap(QString(ICON_PIC_PATH).append("red.png")));
            return;

        case LABEL_YELLOW:
            ui->label_running_light->setPixmap(QPixmap(QString(ICON_PIC_PATH).append("yellow.png")));
            return;

        case LABEL_GREEN:
            ui->label_power_light->setPixmap(QPixmap(QString(ICON_PIC_PATH).append("green.png")));
            return;

        case SWITCH:
            ui->label_switch->setPixmap(QPixmap(QString(ICON_PIC_PATH).append("踏板开.png")));
            return;

        default:
            break;
        }
    }
}

// 设置修改TreatmentPlan控件可用
void NewMainWindow::setModifyPlanControlEnable()
{
    ui->pushButton_modify_para->setEnabled(true);
    ui->pushButton_modify_para->setStyleSheet(SELECTED);

    ui->pushButton_replan->setEnabled(true);
    ui->pushButton_replan->setStyleSheet(SELECTED);

    ui->tableView_current_plan->setContextMenuPolicy(Qt::CustomContextMenu);

//    ui->pushButton_confirm_para->setEnabled(true);
//    ui->pushButton_confirm_para->setStyleSheet(SELECTED);
}

// 设置修改TreatmentPlan控件不可用
void NewMainWindow::setModifyPlanControlDisable()
{
    ui->pushButton_modify_para->setEnabled(false);
    ui->pushButton_modify_para->setStyleSheet(NOT_WORK);

    ui->pushButton_replan->setEnabled(false);
    ui->pushButton_replan->setStyleSheet(NOT_WORK);

    ui->pushButton_confirm_para->setEnabled(false);
    ui->pushButton_confirm_para->setStyleSheet(NOT_WORK);

    ui->tableView_current_plan->setContextMenuPolicy(Qt::NoContextMenu);
}

// 设置Treatment开始，Confirm，继续相关控件不可用
void NewMainWindow::setTreatmentControlDisable()
{
    ui->pushButton_start_treat->setDisabled(true);
    ui->pushButton_start_treat->setStyleSheet(NOT_WORK);

    ui->pushButton_continue->setDisabled(true);
    ui->pushButton_continue->setStyleSheet(NOT_WORK);
}

void NewMainWindow::setLocationControlEnable(bool flag)
{
    if(flag)
    {
        if(_seriesAcquired)
        {
            return;
        }
        ui->pushButton_headL->setEnabled(true);
        ui->pushButton_headR->setEnabled(true);
        ui->pushButton_headF->setEnabled(true);
        ui->pushButton_headB->setEnabled(true);
        ui->pushButton_headD->setEnabled(true);
        ui->pushButton_headU->setEnabled(true);
        ui->toolButton_header_zero->setEnabled(true);
        ui->toolButton_header_zero->setStyleSheet(SELECTED);
        ui->toolButton_change_image_plane->setEnabled(true);
        ui->toolButton_change_image_plane->setStyleSheet(SELECTED);
    }
    else
    {
        ui->pushButton_headL->setEnabled(false);
        ui->pushButton_headR->setEnabled(false);
        ui->pushButton_headF->setEnabled(false);
        ui->pushButton_headB->setEnabled(false);
        ui->pushButton_headD->setEnabled(false);
        ui->pushButton_headU->setEnabled(false);
        ui->toolButton_header_zero->setEnabled(false);
        ui->toolButton_header_zero->setStyleSheet(NOT_WORK);
        ui->toolButton_change_image_plane->setEnabled(false);
        ui->toolButton_change_image_plane->setStyleSheet(NOT_WORK);
    }
}

// 设置Treatment开始，Confirm，继续相关控件可用
void NewMainWindow::setTreatmentControlEnable()
{
    ui->pushButton_start_treat->setEnabled(true);
    ui->pushButton_start_treat->setStyleSheet(SELECTED);

//    ui->pushButton_stop->setEnabled(true);
//    ui->pushButton_stop->setStyleSheet(REDCOLOR);

    ui->pushButton_continue->setEnabled(true);
    ui->pushButton_continue->setStyleSheet(SELECTED);
    ui->toolButton_acquire_image->setEnabled(true);
    ui->toolButton_acquire_image->setStyleSheet(SELECTED);
    if (!ui->pushButton_treatment_report->isEnabled())
    {
        ui->pushButton_treatment_report->setEnabled(true);
        ui->pushButton_treatment_report->setStyleSheet(SELECTED);
    }
}

// 设置Treatment Parameters控件不可用
void NewMainWindow::setTreatmentParaControlDisable()
{
    if (ui->horizontalSlider_power->isEnabled())
    {
        ui->horizontalSlider_power->setEnabled(false);
        ui->horizontalSlider_power->setStyleSheet(NOT_SELECTED);

        ui->label_power->setEnabled(false);
        ui->label_power_value->setEnabled(false);
        ui->label_power_unit->setEnabled(false);
    }

    if (ui->horizontalSlider_cooling_time->isEnabled())
    {
        ui->horizontalSlider_cooling_time->setEnabled(false);
        ui->horizontalSlider_cooling_time->setStyleSheet(NOT_SELECTED);

        ui->label_cooling_time->setEnabled(false);
        ui->label_cooling_time_value->setEnabled(false);
        ui->label_cooling_time_unit->setEnabled(false);
    }

    if (ui->horizontalSlider_on_time->isEnabled())
    {
        ui->horizontalSlider_on_time->setEnabled(false);
        ui->horizontalSlider_on_time->setStyleSheet(NOT_SELECTED);

        ui->label_on_time->setEnabled(false);
        ui->label_on_time_unit->setEnabled(false);
        ui->label_on_time_value->setEnabled(false);
    }

    if (ui->horizontalSlider_off_time->isEnabled())
    {
        ui->horizontalSlider_off_time->setEnabled(false);
        ui->horizontalSlider_off_time->setStyleSheet(NOT_SELECTED);

        ui->label_off_time->setEnabled(false);
        ui->label_off_time_unit->setEnabled(false);
        ui->label_off_time_value->setEnabled(false);
    }

    if (ui->horizontalSlider_pulse_number->isEnabled())
    {
        ui->horizontalSlider_pulse_number->setEnabled(false);
        ui->horizontalSlider_pulse_number->setStyleSheet(NOT_SELECTED);

        ui->label_pulse_number->setEnabled(false);
        ui->label_pulse_number_unit->setEnabled(false);
        ui->label_pulse_number_value->setEnabled(false);
    }
}

// 设置Treatment Parameters控件可用
void NewMainWindow::setTreatmentParaControlEnable()
{
    if (!ui->horizontalSlider_power->isEnabled())
    {
        ui->horizontalSlider_power->setEnabled(true);
        ui->horizontalSlider_power->setStyleSheet(HORIZON_TYPE);

        ui->label_power->setEnabled(true);
        ui->label_power_value->setEnabled(true);
        ui->label_power_unit->setEnabled(true);
    }

    if (!ui->horizontalSlider_cooling_time->isEnabled())
    {
        ui->horizontalSlider_cooling_time->setEnabled(true);
        ui->horizontalSlider_cooling_time->setStyleSheet(HORIZON_TYPE);

        ui->label_cooling_time->setEnabled(true);
        ui->label_cooling_time_value->setEnabled(true);
        ui->label_cooling_time_unit->setEnabled(true);
    }

    if (!ui->horizontalSlider_on_time->isEnabled())
    {
        ui->horizontalSlider_on_time->setEnabled(true);
        ui->horizontalSlider_on_time->setStyleSheet(HORIZON_TYPE);

        ui->label_on_time->setEnabled(true);
        ui->label_on_time_unit->setEnabled(true);
        ui->label_on_time_value->setEnabled(true);
    }

    if (!ui->horizontalSlider_off_time->isEnabled())
    {
        ui->horizontalSlider_off_time->setEnabled(true);
        ui->horizontalSlider_off_time->setStyleSheet(HORIZON_TYPE);

        ui->label_off_time->setEnabled(true);
        ui->label_off_time_unit->setEnabled(true);
        ui->label_off_time_value->setEnabled(true);
    }

    if (!ui->horizontalSlider_pulse_number->isEnabled())
    {
        ui->horizontalSlider_pulse_number->setEnabled(true);
        ui->horizontalSlider_pulse_number->setStyleSheet(HORIZON_TYPE);

        ui->label_pulse_number->setEnabled(true);
        ui->label_pulse_number_unit->setEnabled(true);
        ui->label_pulse_number_value->setEnabled(true);
    }
}

// 设置Plan制定控件不可用
void NewMainWindow::setWholePlanControlDisable()
{
    ui->checkBox_density_high->setEnabled(false);
    ui->checkBox_density_middle->setEnabled(false);
    ui->checkBox_density_low->setEnabled(false);

    ui->checkBox_layer_up->setEnabled(false);
    ui->checkBox_layer_middle->setEnabled(false);
    ui->checkBox_layer_down->setEnabled(false);
    ui->checkBox_circle_one->setEnabled(false);
    ui->checkBox_circle_two->setEnabled(false);
    ui->checkBox_circle_three->setEnabled(false);
    ui->checkBox_circle_four->setEnabled(false);
    ui->checkBox_circle_five->setEnabled(false);

    ui->spinBox_angle_one->setEnabled(false);
    ui->spinBox_angle_two->setEnabled(false);
    ui->checkBox_draw_one->setEnabled(false);
    ui->checkBox_draw_two->setEnabled(false);

    ui->checkBox_confirm_one->setEnabled(false);
    ui->checkBox_confirm_two->setEnabled(false);

    ui->pushButton_draw_spot->setEnabled(false);
    ui->pushButton_draw_spot->setStyleSheet(NOT_SELECTED);

    ui->pushButton_clear_contour->setEnabled(false);
    ui->pushButton_clear_contour->setStyleSheet(NOT_SELECTED);

     ui->comboBox_part->setEnabled(false);
    // ui->comboBox_part->setStyleSheet(NOT_SELECTED);
    ui->comboBox_deepth->setEnabled(false);
    // ui->comboBox_deepth->setStyleSheet(NOT_SELECTED);

    ui->comboBox_irradiate_model->setEnabled(false);

    // 设置Treatment Parameters控件不可用
    setTreatmentParaControlDisable();

    ui->commandLinkButton_generate_plan->setEnabled(false);
    ui->commandLinkButton_generate_plan->setStyleSheet(NOT_SELECTED);

    ui->pushButton_resume_para->setEnabled(false);
    ui->pushButton_resume_para->setStyleSheet(NOT_SELECTED);
}

// 设置Plan制定控件可用
void NewMainWindow::setWholePlanControlEnable()
{
    ui->checkBox_density_high->setEnabled(true);
    ui->checkBox_density_middle->setEnabled(true);
    ui->checkBox_density_low->setEnabled(true);

    ui->checkBox_density_low->setChecked(true);

    ui->checkBox_layer_up->setEnabled(true);
    ui->checkBox_layer_middle->setEnabled(true);
    ui->checkBox_layer_down->setEnabled(true);
    ui->checkBox_circle_one->setEnabled(true);
    ui->checkBox_circle_two->setEnabled(true);
    ui->checkBox_circle_three->setEnabled(true);
    ui->checkBox_circle_four->setEnabled(false);
    ui->checkBox_circle_five->setEnabled(false);

    ui->spinBox_angle_one->setEnabled(true);
    ui->spinBox_angle_two->setEnabled(true);
    ui->checkBox_draw_one->setEnabled(true);

    ui->checkBox_confirm_one->setEnabled(true);

    ui->pushButton_draw_spot->setEnabled(true);
    ui->pushButton_draw_spot->setStyleSheet(SELECTED);

    ui->pushButton_clear_contour->setEnabled(true);
    ui->pushButton_clear_contour->setStyleSheet(SELECTED);

    ui->comboBox_deepth->setEnabled(true);
    ui->comboBox_part->setEnabled(true);
    ui->comboBox_irradiate_model->setEnabled(true);

    setTreatmentParaControlEnable();

    ui->commandLinkButton_generate_plan->setEnabled(true);
    ui->commandLinkButton_generate_plan->setStyleSheet(SELECTED);

    ui->pushButton_resume_para->setEnabled(true);
    ui->pushButton_resume_para->setStyleSheet(SELECTED);
}

// 构造三维模型边界
void NewMainWindow::createMask(uchar* data)
{
    if (_targetSceneMap.empty())
    {
        qCDebug(NEWMAINWINDOW()) << NEWMAINWINDOW().categoryName()
                              << "target scenes empty !"
                              << TRACE_CMH();
        return;
    }

    if (data==NULL)
    {
        qCDebug(NEWMAINWINDOW()) << NEWMAINWINDOW().categoryName()
                              << "data pointer empty !"
                              << TRACE_CMH();
        return;
    }

    QMap<int,DrawScene*>::iterator iter = _targetSceneMap.begin();
    QGraphicsPathItem* targetContour = iter.value()->targetContour();
    if (targetContour==NULL)
    {
        qCDebug(NEWMAINWINDOW()) << NEWMAINWINDOW().categoryName()
                              << "targetContour null !"
                              << TRACE_CMH();
        return;
    }

    qreal upLeftX = X_OFFSET - 0.5*CLIP_WIDE;
    qreal upLeftY = FOCAL_PLANE_140MM-0.5*CLIP_HIGH;
    // 以第一个模型为准 半边旋转得到
    for (int i = 0 ; i < CLIP_HIGH;++i)
    {
        for (int j = 0; j < 0.5*CLIP_WIDE; ++j)
        {
            if (targetContour->contains(QPointF(j+upLeftX,i+upLeftY)))
            {
                data[j+i*CLIP_WIDE] = 255;
            }
        }
        // 关于y对称
        for (int j = 0.5*CLIP_WIDE; j < CLIP_WIDE; ++j)
        {
            if (targetContour->contains(QPointF(j+upLeftX,i+upLeftY)))
            {
                data[j+i*CLIP_WIDE] = data[i*CLIP_WIDE+CLIP_WIDE-j-1];
            }
        }
    }


    uchar* temp = new uchar[CLIP_WIDE*CLIP_HIGH];
    memcpy(temp,data,CLIP_WIDE*CLIP_HIGH);

    // 膨胀一圈
    for (int i = 1; i < CLIP_HIGH-1;++i)
    {
        for (int j = 1 ; j < CLIP_WIDE-1;++j)
        {
            // 水平腐蚀
            if (temp[j+i*CLIP_WIDE-1]==255 || temp[j+i*CLIP_WIDE+1] == 255)
            {
                data[j+i*CLIP_WIDE] = 255;
            }

            // 垂直腐蚀
            if (temp[j+(i-1)*CLIP_WIDE]==255 || temp[j+(i+1)*CLIP_WIDE] == 255)
            {
                data[j+i*CLIP_WIDE] = 255;
            }
        }
    }

    delete []temp;
    int skip = CLIP_WIDE*CLIP_HIGH*sizeof(uchar);
    for (int i = 1; i < 181; ++i)
    {
        memcpy(data+i*skip,data,skip);
    }

//    FILE* file = fopen("E:\\mask.bin","wb");
//    if (file == NULL)
//    {
//        return;
//    }
//    fwrite(data,1,CLIP_WIDE*CLIP_HIGH*181,file);

//    fclose(file);
}

bool NewMainWindow::isDigitStr(const QString& str)
{
    bool ok = false;
    str.toInt(&ok);
    return ok;
}

bool NewMainWindow::isInputAvai(const QModelIndex &topLeft,double& voltage,int& pulse)
{

    pulse = _pCurrentModel->item(topLeft.row(),4)->text().toInt();
    if (pulse>0 && pulse <=30)
    {

    }
    else
    {
        return false;
    }

    double value;
    bool ret = getVoltageFromDisplay(_pCurrentModel->item(topLeft.row(),3)->text().toStdString(),value);
    if (ret)
    {
//        int onTime = ui->horizontalSlider_on_time->value();
        voltage = value;
    }
    else
    {
        return false;
    }

    return true;
}

void NewMainWindow::currentPlanChanged(const QModelIndex &topLeft,
                                       const QModelIndex &bottomRight)
{
    if (topLeft.column()==3 || topLeft.column()==4)
    {

    }
    else
    {
        return;
    }

    if (_pCurrentModel==NULL)
    {
        qCDebug(NEWMAINWINDOW()) << NEWMAINWINDOW().categoryName()
                                 << "_pCurrentModel NULL !"
                                 << TRACE_CMH();
        return;
    }

    QString str = _pCurrentModel->item(topLeft.row(),topLeft.column())->text();
    Irradiate_Spot spot;
    // 判断Input参数是否正确
    if (!isDigitStr(str) || !isInputAvai(topLeft,spot.voltage,spot.pulseCount))
    {
        // Input失败，Refresh当前Treatment Parameters
        disconnect(_pCurrentModel,SIGNAL(dataChanged(QModelIndex,QModelIndex)),
                this,SLOT(currentPlanChanged(QModelIndex,QModelIndex)));

        QQueue<Irradiate_Spot> plan;
        _irradiatePlan->getDisplayPlan(plan);
        _pCurrentModel->item(topLeft.row(),topLeft.column())->setText(plan[topLeft.row()].getPlanParaFromColumn(topLeft.column()));
        connect(_pCurrentModel,SIGNAL(dataChanged(QModelIndex,QModelIndex)),
                this,SLOT(currentPlanChanged(QModelIndex,QModelIndex)));

        QMessageBox msgBox("Enter a Valid Number",
                           "Enter a valid number. Use the allowed power level and enter 1 to 30 pulses.",
                           QMessageBox::Question,
                           QMessageBox::Yes | QMessageBox::Default,
                           QMessageBox::NoButton,
                           QMessageBox::NoButton);
        msgBox.setWindowFlags(Qt::WindowTitleHint|Qt::CustomizeWindowHint|Qt::WindowStaysOnTopHint);
        msgBox.setGeometry(QRect(1400,400,msgBox.width(),msgBox.height()));
        msgBox.setFont(m_msgBoxFont);
        msgBox.setButtonText(QMessageBox::Yes, "OK");

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

    spot.id = _pCurrentModel->item(topLeft.row(),0)->text();
    _irradiatePlan->updatePlan(spot);

    // refreshTotalTable();
}

// 初始化Plan表格
void NewMainWindow::initPlanTable()
{
    if (_pCurrentModel == NULL)
    {
        _pCurrentModel = new QStandardItemModel();
        // _pCurrentModel = new DragModel();
    }

//    if (_table==NULL)
//    {
//        _table = new DragTable(ui->tableView_current_plan);
//        _table->setGeometry(0,0,320,418);
//    }

    ui->tableView_current_plan->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView_current_plan->setSelectionMode(QAbstractItemView::SingleSelection);
    //_table->setAcceptDrops(true);
    //_table->setDefaultDropAction(Qt::DropAction::MoveAction);
    //_table->setDragEnabled(true);
    //_table->setDropIndicatorShown(true);

    ui->tableView_current_plan->setModel(_pCurrentModel);
    connect(ui->tableView_current_plan,SIGNAL(exchangeRows(QMap<QString,QString>,int)),
            _pCurrentModel,SLOT(exchangeRows(QMap<QString,QString>,int)),Qt::UniqueConnection);

    _pCurrentModel->setColumnCount(6);
    _pCurrentModel->setHeaderData(0,Qt::Horizontal,QString("ID"));
    _pCurrentModel->setHeaderData(1,Qt::Horizontal,QString("Plane"));
    _pCurrentModel->setHeaderData(2,Qt::Horizontal,QString("Angle"));
    _pCurrentModel->setHeaderData(3,Qt::Horizontal,QString("Power"));
    _pCurrentModel->setHeaderData(4,Qt::Horizontal,QString("Pulses"));
    _pCurrentModel->setHeaderData(5,Qt::Horizontal,QString("Continue"));

    ui->tableView_current_plan->setColumnWidth(0,55);
    ui->tableView_current_plan->setColumnWidth(1,55);
    ui->tableView_current_plan->setColumnWidth(2,55);
    ui->tableView_current_plan->setColumnWidth(3,55);
    ui->tableView_current_plan->setColumnWidth(4,55);
    ui->tableView_current_plan->setColumnWidth(5,35);
    ui->tableView_current_plan->verticalHeader()->hide();

}

void NewMainWindow::initBlocksTable()
{
    if (_pBlockModel==NULL)
    {
        _pBlockModel = new QStandardItemModel();
    }

    ui->tableView_block->setModel(_pBlockModel);
    _pBlockModel->setColumnCount(3);
    _pBlockModel->setHeaderData(0,Qt::Horizontal,QString("ID"));
    _pBlockModel->setHeaderData(1,Qt::Horizontal,QString("Treatment Plate Left/Right Tilt Angle"));
    _pBlockModel->setHeaderData(2,Qt::Horizontal,QString("Treatment Plate Forward/Back Tilt Angle"));

    ui->tableView_block->setColumnWidth(0,200);
    ui->tableView_block->setColumnWidth(1,200);
    ui->tableView_block->setColumnWidth(2,200);
    ui->tableView_block->verticalHeader()->hide();
}

void NewMainWindow::on_pushButton_stop_clicked()
{
    if (_core==NULL/*_schedule==NULL*/)
    {
        return;
    }
    else
    {
        qDebug()<<"111111111=========";
//        _schedule->onServiceCalled(SONICATION,SONICATION_STOP);
        emit setIsPause(false);
        _core->StopSonication();
//        connect(_append,SIGNAL(switchChanged(bool)),this,SLOT(switchChanged(bool)));
    }
        qDebug()<<"222222222222=========";

    _isIrradiating = false;    // Exit某一点在sonicationMiddleStatus
    emit setIrradiating(_isIrradiating);
    setLocationControlEnable(true);
    // 设置Treatment相关按钮可用
    // setTreatmentControlEnable();

    // 设置修改Plan控件可用
    setModifyPlanControlEnable();

    // Refreshsonication pointStatus
    setBackground();
        qDebug()<<"333333333333=========";
    ui->label_remain_cooling_time_value->setText("0");
    ui->progressBar_cooling_time->setValue(100);
    if (_append!=NULL)
    {
        _append->setYellowOff();
        ui->label_running_light->setPixmap(QPixmap(QString(ICON_PIC_PATH).append("green.png")));
    }
    qDebug()<<"44444444444444=========";
}

// Restore当前scene长度测量Status
void NewMainWindow:: resumeMeasureLength()
{
    if (_targetScene==NULL)
    {
        qCDebug(NEWMAINWINDOW()) << "_targetScene null !"
                                 << TRACE_CMH();
        return;
    }

    if (OPER_MEASURE==_targetScene->getOperator())
    {
        _targetScene->setOperator(OPER_DELETE);
    }

//     Restore原始Status
    _targetScene->setDrawable(stat._bDrawable);
    _targetScene->setTargetDrawable(stat._bTargetDrawable);
    _targetScene->setMeasureDrawable(stat._bMeasureable);
    _targetScene->setBeamVisible(stat._beamVisiable);
    _targetScene->setTransducerVisible(stat._transducerVisiable);
}

void NewMainWindow::on_commandLinkButton_measure_length_clicked()
{
    if (_targetScene==NULL)
    {
        qCDebug(NEWMAINWINDOW()) << "_targetScene null !"
                                 << TRACE_CMH();

        return;
    }

    if (OPER_MEASURE==_targetScene->getOperator())
    {
        _targetScene->setOperator(OPER_DELETE);

        // Restore原始Status
        _targetScene->setDrawable(stat._bDrawable);
        _targetScene->setTargetDrawable(stat._bTargetDrawable);

    }
    else
    {
        _targetScene->setOperator(OPER_MEASURE);

        stat._bDrawable = _targetScene->drawable();
        stat._bTargetDrawable = _targetScene->targetDrawable();
        // 设置为scene可操作
        if (!_targetScene->drawable())
        {
            _targetScene->setDrawable(true);
        }

        if (!_targetScene->targetDrawable())
        {
            _targetScene->setTargetDrawable(true);
        }
    }

    // Restore原来的操作
    // _targetScene->setOperator(tempOper);
}

void NewMainWindow::on_progressBar_cooling_time_valueChanged(int value)
{
    ui->progressBar_cooling_time->setValue(value);
}

QString NewMainWindow::getTimeString(int time)
{
    int hour = time/ 3600;
    int minute = (time - 3600*hour)/60;
    int second = time -3600*hour-60*minute;
    QString temp(QString::number(hour));
    temp.append(":");
    temp.append(QString::number(minute));
    temp.append(":");
    temp.append(QString::number(second));
    return temp;
}

void NewMainWindow::on_label_real_time_angle_objectNameChanged(const QString &objectName)
{
}

void NewMainWindow::on_toolButton_change_image_plane_clicked()
{
    if(_pChoseimage!=NULL)
    {
       // 设置禁止窗口最大最小化
       _pChoseimage->setWindowFlags(Qt::WindowStaysOnTopHint | Qt::WindowCloseButtonHint);
       _pChoseimage->setFixedSize(_pChoseimage->width(),_pChoseimage->height());
       _pChoseimage->show();
       _pChoseimage->setEnabled();
       ui->toolButton_acquire_image->setEnabled(true);
       _pChoseimage->move(852,473);

    }
}

void NewMainWindow::on_horizontalSlider_power_valueChanged(int value)
{
    qDebug()<<"滑动条改变数值："<<value;
    int v = sqrt(value);
    if (m_iLastPower>value)
    {
        // 只处理平方数据
        if (v*v==value)
        {

        }
        else
        {
            ui->horizontalSlider_power->setValue((v)*(v));
            m_iLastPower = v*v;
            return;
        }
    }
    else
    {
        // 只处理平方数据
        if (v*v==value)
        {

        }
        else
        {
            ui->horizontalSlider_power->setValue((v+1)*(v+1));
            m_iLastPower=(v+1)*(v+1);
            return;
        }
    }
    qDebug()<<"滑动条输出数值："<<value;
    ui->label_power_value->setText(getDisplayPower(value));
    if (value != _parameter.power)
    {
        ui->label_power_value->setStyleSheet(CHANGED);
        ui->toolButton_therapy->setDisabled(true);
        ui->toolButton_therapy->setStyleSheet(NOT_WORK);
    }
    else
    {
        ui->label_power_value->setStyleSheet(UNCHANGED);
    }
}

void NewMainWindow::resetScenes()
{
    ui->graphicsView_target_scene->clearMask();
    ui->horizontalScrollBar_images->setValue(-90);

    // 设置Treatment按钮不可用
    ui->toolButton_therapy->setDisabled(true);
    ui->toolButton_therapy->setStyleSheet(NOT_WORK);

    _currentImages = 0;
    ui->horizontalScrollBar_images->setDisabled(true);
    ui->horizontalScrollBar_images->setStyleSheet(NOT_WORK);

    ui->pushButton_front->setDisabled(true);
    ui->pushButton_front->setStyleSheet(NOT_WORK);

    ui->pushButton_back->setDisabled(true);
    ui->pushButton_back->setStyleSheet(NOT_WORK);

    if (ui->pushButton_treatment_report->isEnabled())
    {
        ui->pushButton_treatment_report->setEnabled(false);
        ui->pushButton_treatment_report->setStyleSheet(NOT_WORK);
    }

    if (ui->pushButton_measure_length->isEnabled())
    {
        ui->pushButton_measure_length->setEnabled(false);
        ui->pushButton_measure_length->setStyleSheet(NOT_WORK);
    }

    if (ui->pushButton_histogram->isEnabled())
    {
        ui->pushButton_histogram->setEnabled(false);
        ui->pushButton_histogram->setStyleSheet(NOT_WORK);
    }

    ui->pushButton_cancel->setEnabled(false);
    ui->pushButton_cancel->setStyleSheet(NOT_WORK);

    // 设置声束和Transducer不可用
    ui->toolButton_beam_path->setDisabled(true);
    ui->toolButton_beam_path->setStyleSheet(NOT_WORK);
    ui->toolButton_transducer->setDisabled(true);
    ui->toolButton_transducer->setStyleSheet(NOT_WORK);

    setWholePlanControlDisable();
    setTreatmentControlDisable();
    // 设置改变sonication参数及Replan控件不可用
    setModifyPlanControlDisable();
    ui->label_select_image_angle->setText("Angle Unknown");

    // 重置Trace初始角
    ui->spinBox_angle_one->setValue(-90);
    ui->spinBox_angle_two->setValue(0);

    ui->checkBox_draw_one->setChecked(false);
    ui->checkBox_confirm_one->setChecked(false);
    ui->checkBox_draw_two->setChecked(false);
    ui->checkBox_confirm_two->setChecked(false);

    _targetSceneMap.clear();
    stat._transducerVisiable = false;
    stat._beamVisiable = false;

    // Reset to Original Image Size
    _viewer2DInfo.scal = 1.0;
    _viewer2DInfo.rect = QRectF(0,0,574,574);
    _layerViewer2DInfo.scal = 1.0;
    _layerViewer2DInfo.rect = QRectF(0,0,200,200);
    clearTargetScenes();
    clearSliceScenes();
    // 清除以前Save的核磁超声registration位置数据
    _contoursMoveMap.clear();

//    if (_pScene3d!=NULL)
//    {
//        _pScene3d->destorySpots();
//        delete _pScene3d;
//        _pScene3d = NULL;
//    }

    if(_newScene3d!=NULL&&_newScene3d->isInit)
    {
        _newScene3d->ClearSpot();
    }

    // 记录sonicationComplete的点数
    // _totalSpot += _totalCompleteMap.size();
    // Clear Allsonication表格中记录的id
    _totalCompleteMap.clear();
    qDebug()<<"bf _currentCompleteMap clear";
    _currentCompleteMap.clear();
    qDebug()<<"af _currentCompleteMap clear";
    _targetScene = NULL;
    _isIrradiating = false;
    emit setIrradiating(_isIrradiating);
    setLocationControlEnable(true);

    ui->graphicsView_treatment_layer->clearMask();
    disableLayerControl();
    clearLayerScenes();

    // 进度条进度设置为0
    ui->progressBar_cooling_time->setValue(0);
    ui->label_remain_cooling_time_value->setText("0");
//    if (_core!=NULL)
//    {
//        delete _core;
//        _core = NULL;
//    }

    if (_pCurrentModel!=NULL)
    {
        _pCurrentModel->clear();
    }

//    if (_pTotalModel!=NULL)
//    {
//       _pTotalModel->clear();
//    }
    initPlanTable();

    disconnect(_pCurrentModel,SIGNAL(dataChanged(QModelIndex,QModelIndex)),
            this,SLOT(currentPlanChanged(QModelIndex,QModelIndex)));
}

void NewMainWindow::synReportInfo()
{
    _recSpot++;
    qDebug()<<"_recSpot====="<<QString::number(_recSpot);
    qDebug()<<"_totalSpot====="<<QString::number(_totalSpot);
    _endTime = QTime::currentTime().toString("HH:mm:ss");
    endTime = QDateTime::currentMSecsSinceEpoch();

    QString deep = ui->comboBox_deepth->currentText();
    double value = getVoltage(ui->horizontalSlider_power->value());
    int onTime = ui->horizontalSlider_on_time->value();
    QString pulse = QString::number(ui->horizontalSlider_pulse_number->value());
    int pulseNum = ui->horizontalSlider_pulse_number->value();
    // QString energy =  QString::number(value*value*onTime*pulseNum*0.001);
    if(_recSpot<=_totalSpot)
    {
        _totalTime += onTime*pulseNum;
    }
    QString totalTime =  QString::number(_totalTime/1000);


    outputTreatmentLog("80%",_startTime,_endTime,QString::number(_totalSpot),totalTime,deep,"Zhonghui",pulse);
    emit setTerapyInfo("80%",_startTime,_endTime,QString::number(_totalSpot),totalTime,deep,"Zhonghui",pulse,getDisplayPower(value*value));
    ui->label_totalPoint->setText("Total irradiation points "+QString::number(_totalSpot));
    ui->label_totalTime->setText("Total irradiation time "+totalTime+"S");
//    emit saveReport();

    if(!is_first_treat)
    updateLog("@@@"+power+","+on_time+","+off_time+","+pulse_number+","
                +cooling_time+","+QString::number(_seriesSpot)+"@@@");
}

void NewMainWindow::on_pushButton_treatment_report_clicked()
{
    QMessageBox msgBox("Warning","Finish treatment and open the report print page?",
                       QMessageBox::Warning,
                       QMessageBox::Ok | QMessageBox::Escape,
                       QMessageBox::Cancel,
                       QMessageBox::NoButton);
    msgBox.setWindowFlags(Qt::WindowTitleHint|Qt::CustomizeWindowHint|Qt::WindowStaysOnTopHint);
    msgBox.setGeometry(QRect(1400,400,msgBox.width(),msgBox.height()));
    msgBox.setFont(m_msgBoxFont);
    msgBox.setButtonText(QMessageBox::Ok, "OK");
    msgBox.setButtonText(QMessageBox::Cancel, "Cancel");

    int ret = msgBox.exec();
    if (ret == QMessageBox::Cancel)
    {
        return;
    }

    qCDebug(NEWMAINWINDOW())<< NEWMAINWINDOW().categoryName()
                         << "Enter treament report clicked !"
                         << TRACE_CMH();

    // SaveTreatment效果图
    saveTreatResult();
    if (_isOnSaveVideo)
    {
        // 在Save Video，自动关闭Save的视频
        on_toolButton_save_video_2_clicked();
    }

    resetScenes();

    // 设置Treatment相关参数

    if (_treatmentTimeRemain!=NULL)
    {
        _treatmentTimeRemain->stop();
    }

    if (_treatmentTimer!=NULL)
    {
        _treatmentTimer->stop();
    }

    is_first_treat = true;
    // 同步报告信息
    synReportInfo();
    emit saveReport();
    LogHeartTimer->stop();
    if(_totalSpot%37)
        TreatedUnit = QString::number(_totalSpot/37 + 1);
    else
        TreatedUnit = QString::number(_totalSpot/37);
    updateLog("!!!"+QString::number(startTime)+","+QString::number(endTime)+","+QString::number(_totalSpot)+","
            +TreatedUnit+","+QString::number(_totalTime)+","
            +imagePath+"/Target_Image_Sequence_0_degree.jpg"+","
            +imagePath+"/Target_Image_Sequence_90_degree.jpg"+"!!!");
    ui->label_totalPoint->setText("Total irradiation points ");
    ui->label_totalTime->setText("Total irradiation time ");
    TID.clear();

    _printDialog->show();
    //emit showReport();

//        if (_imageCapture!=NULL)
//        {
//            _imageCapture->stopSaveVideo();
//        }

    m_totalTime = TOTAL_TIME();
}

void NewMainWindow::on_pushButton_front_clicked()
{
    if (_targetSceneMap.empty())
    {
        return;
    }

    int angle = ui->horizontalScrollBar_images->value() - 91;

    qCDebug(NEWMAINWINDOW())<< NEWMAINWINDOW().categoryName()
                         << "angle is  !"<<angle
                         << TRACE_CMH();

    int newAngle = getTargetSceneAbove(angle);
    ui->horizontalScrollBar_images->setValue(newAngle+91);
    on_horizontalScrollBar_images_sliderReleased();

}

int  NewMainWindow::getTargetSceneAbove(int angle)
{
    QMap<int, DrawScene*>::iterator iter = _targetSceneMap.begin();
    for (;iter != _targetSceneMap.end();++iter)
    {
        if (iter.key()>angle)
        {
            return iter.key();
        }
    }

    // 没找到，Back最大的一个
    iter = _targetSceneMap.end();
    --iter;
    return iter.key();
}

int  NewMainWindow::getTargetSceneBelow(int angle)
{
    QMap<int, DrawScene*>::iterator iter = _targetSceneMap.end();
    for (; iter != _targetSceneMap.begin(); )
    {
        --iter;
        if (iter.key()<angle)
        {
            return iter.key();
        }
    }

    // 没找到，Back最小的一个
    iter = _targetSceneMap.begin();
    return iter.key();
}

void NewMainWindow::on_pushButton_back_clicked()
{
    if (_targetSceneMap.empty())
    {
        return;
    }

    int angle = ui->horizontalScrollBar_images->value()-91;

    qCDebug(NEWMAINWINDOW())<< NEWMAINWINDOW().categoryName()
                         << "value is  !"<< angle
                         << TRACE_CMH();

    int newAngle = getTargetSceneBelow(angle);
    ui->horizontalScrollBar_images->setValue(newAngle+91);
    on_horizontalScrollBar_images_sliderReleased();
}

void NewMainWindow::on_pushButton_treatment_layer_down_clicked()
{
    ui->pushButton_treatment_layer_down->setStyleSheet(SELECTED);
    ui->pushButton_treatment_layer_middle->setStyleSheet(NOT_SELECTED);
    ui->pushButton_treatment_layer_up->setStyleSheet(NOT_SELECTED);

    _layerScene = _layerScenes[2];
    if (!_layerScene->equalViewer2D(_layerViewer2DInfo))
    {
        _layerScene->setViewer2D(_layerViewer2DInfo);
    }
    ui->graphicsView_treatment_layer->setScene(_layerScene);
}

void NewMainWindow::on_pushButton_treatment_layer_middle_clicked()
{
    ui->pushButton_treatment_layer_down->setStyleSheet(NOT_SELECTED);
    ui->pushButton_treatment_layer_middle->setStyleSheet(SELECTED);
    ui->pushButton_treatment_layer_up->setStyleSheet(NOT_SELECTED);
    _layerScene = _layerScenes[1];
    if (!_layerScene->equalViewer2D(_layerViewer2DInfo))
    {
        _layerScene->setViewer2D(_layerViewer2DInfo);
    }
    ui->graphicsView_treatment_layer->setScene(_layerScene);
}

void NewMainWindow::on_pushButton_treatment_layer_up_clicked()
{
    ui->pushButton_treatment_layer_down->setStyleSheet(NOT_SELECTED);
    ui->pushButton_treatment_layer_middle->setStyleSheet(NOT_SELECTED);
    ui->pushButton_treatment_layer_up->setStyleSheet(SELECTED);
    _layerScene = _layerScenes[0];
    if (!_layerScene->equalViewer2D(_layerViewer2DInfo))
    {
        _layerScene->setViewer2D(_layerViewer2DInfo);
    }
    ui->graphicsView_treatment_layer->setScene(_layerScene);
}

void NewMainWindow::on_pushButton_start_treat_clicked()
{
    if (_irradiatePlan==NULL)
    {
        qCDebug(NEWMAINWINDOW())<<NEWMAINWINDOW().categoryName()
                             <<"irradiate plan null !"
                             << TRACE_CMH();
        return;
    }

    qDebug()<<"disp size1====="<<_irradiatePlan->getDisplaySize();
    qCDebug(NEWMAINWINDOW())<<NEWMAINWINDOW().categoryName()
                         <<"Enter start treat clicked !"
                         << TRACE_CMH();
    ui->pushButton_treatment_report->setEnabled(false);
    ui->pushButton_treatment_report->setStyleSheet(NOT_WORK);
    ui->toolButton_acquire_image->setEnabled(false);
    ui->toolButton_acquire_image->setStyleSheet(NOT_WORK);

    if (_pHeaderControl!=NULL && !_pHeaderControl->isHidden())
    {
        _pHeaderControl->hide();
    }

    if (_pChoseimage!=NULL && !_pChoseimage->isHidden())
    {
        _pChoseimage->hide();
    }

//    Heartthread->changeflag();
    // 已经Start treatment了
    qDebug()<<"is irradiating flag is "<< _isIrradiating;
    if (_isIrradiating)
    {
        return;
    }

    // 进入某一点开始sonicationStatus
    _isIrradiating = true;
    emit setIrradiating(_isIrradiating);
qDebug()<<"disp size2====="<<_irradiatePlan->getDisplaySize();
    if (ui->horizontalSlider_power->isEnabled())
    {
        // 设置改变Treatment Parameters控件不可用
        setTreatmentParaControlDisable();
    }

    // sonication开始设置改变参数不可用
    setModifyPlanControlDisable();
    // sonication开始运动控制控件不可用
    setLocationControlEnable(false);
qDebug()<<"disp size3====="<<_irradiatePlan->getDisplaySize();
    // 设置Treatment相关按钮不可用
    setTreatmentControlDisable();
    if (_core!=NULL)
    {qDebug()<<"disp size4====="<<_irradiatePlan->getDisplaySize();
        if (!_core->sendSpotParameter())
        {
            _isIrradiating = false;
            emit setIrradiating(_isIrradiating);
            setLocationControlEnable(true);
            // 设置Treatment相关按钮可用
            setTreatmentControlEnable();
            return;
        }
    }

    // RefreshTreatmentRemaining time
    if(is_plan_completed)
    {
        refreshRemainTime();
        QString str = "Remaining time " + getTimeString(m_totalTime.remainTime);
        ui->label_time_remain->setText(str);
        is_plan_completed = false;
    }

    if(is_first_treat == true){
        _treatmentTimer->start();
        is_first_treat = false;
    }
}

void NewMainWindow::saveTreatResult()
{
    QDesktopWidget* pDesktoWidget = QApplication::desktop();
    QScreen* pqscreen = QGuiApplication::primaryScreen();
    QPixmap pixmap = pqscreen->grabWindow(pDesktoWidget->winId(),130,700,328,328);
    QString name;
    if (ui->pushButton_treatment_layer_down->styleSheet()==SELECTED)
    {
        name = "../resources/downimage.bmp";
    }
    else if(ui->pushButton_treatment_layer_middle->styleSheet()==SELECTED)
    {
        name = "../resources/middleimage.bmp";
    }
    else
    {
        name = "../resources/upimage.bmp";
    }
    pixmap.save(name);
}

void NewMainWindow::on_pushButton_continue_clicked()
{
    saveTreatResult();

    // 设置Cooling进度条为0
    ui->progressBar_cooling_time->setValue(0);
    ui->label_remain_cooling_time_value->setText("0");

    QQueue<Irradiate_Spot> plan;
    _irradiatePlan->getNextPlan(plan);

    if (plan.empty())
    {
        QMessageBox msgBox("Notice","There are no other treatment planes to switch to.",
                           QMessageBox::Warning,
                           QMessageBox::Ok | QMessageBox::Escape,
                           QMessageBox::NoButton,
                           QMessageBox::NoButton);
        msgBox.setWindowFlags(Qt::WindowTitleHint|Qt::CustomizeWindowHint|Qt::WindowStaysOnTopHint);
        msgBox.setGeometry(QRect(1400,400,msgBox.width(),msgBox.height()));
        msgBox.setFont(m_msgBoxFont);
        msgBox.setButtonText(QMessageBox::Ok, "OK");

        msgBox.exec();
        return;
    }

    refreshCurrentTable(plan);
    _currentCompleteMap.clear();

    ui->pushButton_modify_para->setEnabled(true);
    ui->pushButton_modify_para->setStyleSheet(SELECTED);

    // 同步Live Image和Save的图像
    int angle;
    if (_irradiatePlan->getDisplayAngle(angle))
    {
        ui->horizontalScrollBar_images->setValue(angle+91);
        on_horizontalScrollBar_images_valueChanged(angle+91);
        if (_location!=NULL)
        {
            if (!_location->setPos(AXIS_A,angle))
            {
                qCDebug(NEWMAINWINDOW()) << NEWMAINWINDOW().categoryName()
                                      << "synchro image error,location set pos failed !"
                                      << TRACE_CMH();
            }
        }
    }
}

void NewMainWindow::queryAmpTempature()
{
    qCDebug(NEWMAINWINDOW())<<NEWMAINWINDOW().categoryName()
                            <<"Enter query amplify voltage "
                            << TRACE_CMH();

    if (_schedule==NULL)
    {
        qCDebug(NEWMAINWINDOW()) << NEWMAINWINDOW().categoryName()
                                 << "query amplitify tempature schedule null !"
                                 << TRACE_CMH();
        return;
    }

    _schedule->onServiceCalled(INQUIRY,POWER_AMPLIFIERS_TEMPATURE);
}

void NewMainWindow::queryAmpVoltage()
{
    qCDebug(NEWMAINWINDOW())<<NEWMAINWINDOW().categoryName()
                            <<"Enter query amplify voltage "
                            << TRACE_CMH();

    if (_schedule==NULL)
    {
        qCDebug(NEWMAINWINDOW()) << NEWMAINWINDOW().categoryName()
                                 << "query amplitify tempature schedule null !"
                                 << TRACE_CMH();
        return;
    }

    _schedule->onServiceCalled(INQUIRY,POWER_AMPLIFIERS_VOLTAGE);
}
void NewMainWindow::refreshRemainTime()
{
    int totalSpot = m_totalTime.spotsRemain - treatedSpot;
    qDebug()<<"totalSpot====="<<totalSpot;
    m_totalTime.remainTime = ((m_totalTime.onTime + m_totalTime.offTime)*m_totalTime.pulseNumber*0.001
                 +(m_totalTime.coolingTime+m_totalTime.delayTime))*totalSpot;
    remainTime = m_totalTime.remainTime;
    qDebug()<<"remainTime====="<<m_totalTime.remainTime;
    qDebug()<<"onTime====="<<m_totalTime.onTime;
    qDebug()<<"offTime====="<<m_totalTime.offTime;
    qDebug()<<"pulseNumber====="<<m_totalTime.pulseNumber;

}

// 获取此次TreatmentRemaining time，单位为秒
QString NewMainWindow::getTotalTime()
{
    if (m_totalTime.remainTime < 0)
    {
        return QString("Treatment time");
    }

    int total = m_totalTime.remainTime + m_totalTime.spendTime;
    QString label = "Treatment time " + getTimeString(total);
    return label;
}

// 获取此次TreatmentRemaining time，单位为秒
QString NewMainWindow::getRemainTotalTime()
{
    remainTime -= (m_totalTime.onTime + m_totalTime.offTime)*m_totalTime.pulseNumber*0.001
            +(m_totalTime.coolingTime+m_totalTime.delayTime);
    m_totalTime.remainTime = remainTime;
    if (m_totalTime.remainTime < 0)
    {
        return QString("Remaining time");
    }

    QString label = "Remaining time " + getTimeString(m_totalTime.remainTime);
    return label;
}

void NewMainWindow::refreshBlocksTable(QQueue<Irradiate_Block>& blocks)
{
    if (blocks.empty())
    {
        _pBlockModel->setRowCount(0);
        return;
    }

    // 设置Current Plan表中背景为象牙白
    _pBlockModel->setRowCount(blocks.size());    // 清除多余的表格
    for (int i = 0;i <blocks.size();++i)
    {
        for (int j = 0; j < 3; ++j)
        {
            _pBlockModel->setItem(i,j,new QStandardItem(blocks[i].para2String(j)));
            _pBlockModel->item(i,j)->setBackground(QColor(250,255,240));
            _pBlockModel->item(i,j)->setEditable(false);
        }
    }
}

void NewMainWindow::refreshCurrentTable(QQueue<Irradiate_Spot>& plan)
{
    // Refresh表格时不检查表格内容
    disconnect(_pCurrentModel,SIGNAL(dataChanged(QModelIndex,QModelIndex)),
               this,SLOT(currentPlanChanged(QModelIndex,QModelIndex)));

    if (plan.empty())
    {
        _pCurrentModel->setRowCount(0);
        return;
    }

    // 设置Current Plan表中背景为象牙白
    _pCurrentModel->setRowCount(plan.size());    // 清除多余的表格
    for (int i = 0;i <plan.size();++i)
    {
        for (int j = 0; j < 3; ++j)
        {
            _pCurrentModel->setItem(i,j,new QStandardItem(plan[i].getPlanParaFromColumn(j)));
            _pCurrentModel->item(i,j)->setBackground(QColor(250,255,240));
            _pCurrentModel->item(i,j)->setEditable(false);
        }

        for (int j = 3; j < 5; ++j)
        {
            _pCurrentModel->setItem(i,j,new QStandardItem(plan[i].getPlanParaFromColumn(j)));
            _pCurrentModel->item(i,j)->setBackground(QColor(250,255,240));

        }

        _pCurrentModel->setItem(i,5,new QStandardItem(QString("")));
        _pCurrentModel->item(i,5)->setBackground(QColor(0,255,0));
    }

    _pCurrentModel->setColumnCount(6);
    _pCurrentModel->setHeaderData(0,Qt::Horizontal,QString("ID"));
    _pCurrentModel->setHeaderData(1,Qt::Horizontal,QString("Plane"));
    _pCurrentModel->setHeaderData(2,Qt::Horizontal,QString("Angle"));
    _pCurrentModel->setHeaderData(3,Qt::Horizontal,QString("Power"));
    _pCurrentModel->setHeaderData(4,Qt::Horizontal,QString("Pulses"));
    _pCurrentModel->setHeaderData(5,Qt::Horizontal,QString("Continue"));

    ui->tableView_current_plan->setColumnWidth(0,55);
    ui->tableView_current_plan->setColumnWidth(1,55);
    ui->tableView_current_plan->setColumnWidth(2,55);
    ui->tableView_current_plan->setColumnWidth(3,55);
    ui->tableView_current_plan->setColumnWidth(4,55);
    ui->tableView_current_plan->setColumnWidth(5,35);
    ui->tableView_current_plan->verticalHeader()->hide();

    connect(_pCurrentModel,SIGNAL(dataChanged(QModelIndex,QModelIndex)),
            this,SLOT(currentPlanChanged(QModelIndex,QModelIndex)));
}

// RefreshOverall Plan显示表格
//void NewMainWindow::refreshTotalTable()
//{
//    QQueue<Irradiate_Spot> totalPlan = _irradiatePlan->getTotalPlan();
//    _pTotalModel->setRowCount(totalPlan.size());
//    for (int i = 0; i < totalPlan.size();++i)
//    {
//        for (int j = 0; j<5; ++j)
//        {
//            _pTotalModel->setItem(i,j,new QStandardItem(totalPlan[i].getPlanParaFromColumn(j)));
//            _pTotalModel->item(i,j)->setBackground(QColor(250,255,240));
//        }

//    }
//}

// 相对于初始点位置Sonication Point Distribution为
// 相对Angle      0     20  30 40 60    80 90 100 120 140 150 160
// 点个数        7     2   2  2  6     2  2  2   6    2   2  2
// 半径       0,1,2,3  3   2  3 1,2,3  3  2  3  1,2,3 3   2  3
void NewMainWindow::on_commandLinkButton_generate_plan_clicked()
{
    stat._bDrawable = true;
    stat._bMeasureable = false;
    stat._bTargetDrawable =false;
    // 设置targetScene初始Status
    if (_targetScene!=NULL)
    {
        _targetScene->setDrawable(stat._bDrawable);
        _targetScene->setMeasureDrawable(stat._bMeasureable);
        _targetScene->setTargetDrawable(stat._bTargetDrawable);

    }

    if (!_targetSceneMap.empty())
    {
        QMap<int,DrawScene*>::iterator iter = _targetSceneMap.begin();
        for (; iter!=_targetSceneMap.end(); ++iter)
        {
            iter.value()->removeSpots();
        }
    }

    SPOT_DISTRIBUTE dist;    // 此平面Treatment点分布情况
    int angle = -90;
    bool isLayerUp = ui->checkBox_layer_up->isChecked();
    bool isLayerMiddle = ui->checkBox_layer_middle->isChecked();
    bool isLayerDown = ui->checkBox_layer_down->isChecked();
    bool isCircleOne = ui->checkBox_circle_one->isChecked();
    bool isCircleTwo = ui->checkBox_circle_two->isChecked();
    bool isCircleThree = ui->checkBox_circle_three->isChecked();

    bool isCircleFour = ui->checkBox_circle_four->isChecked();
    bool isCircleFive = ui->checkBox_circle_five->isChecked();

    if (ui->checkBox_density_high->isChecked())
    {
        dist.setDensity(DENSITY_HIGH);
    }

    if (ui->checkBox_density_middle->isChecked())
    {
        dist.setDensity(DENSITY_MIDDLE);
    }

    if (ui->checkBox_density_low->isChecked())
    {
        dist.setDensity(DENSITY_LOW);
    }

    if (isLayerUp)
    {
        dist.setLayerUp(true);
    }

    if (isLayerMiddle)
    {
        dist.setLayerMiddle(true);
    }

    if (isLayerDown)
    {
        dist.setLayerDown(true);
    }

    if (isCircleOne)
    {
        dist.setCircleOne(true);
    }

    if (isCircleTwo)
    {
        dist.setCircleTwo(true);
    }

    if (isCircleThree)
    {
        dist.setCircleThree(true);
    }

    if (isCircleFour)
    {
        dist.setCircleFour(true);
    }

    if (isCircleFive)
    {
        dist.setCircleFive(true);
    }

    SmartPlan smart(angle,dist);
    DrawScene* tempScene;
    _targetSceneMap.clear();
    QMap<int,DrawScene*>::iterator iter;
    size_t size = smart.getSize();
    for (int i = 0; i < size; ++i)
    {
        int angle = smart.getAngle(i);
        iter = _sceneMap.find(angle);
        if (iter==_sceneMap.end())
        {
            continue;
        }
        tempScene = iter.value();
        if (tempScene!=NULL)
        {
            // tempScene->drawSpots(smart.getDistribute(i));
            tempScene->setDensity(smart.getDensity());
            tempScene->drawSpotsNew(m_iRotate,smart.getDistribute(i));
            if (tempScene->isUsefulScene())
            {
                _targetSceneMap.insert(angle,tempScene);
            }
        }
    }



    if (_pHifu3d==NULL)
    {
        _pHifu3d = new Hifu3d;
        _pHifu3d->LoadImages(_sceneMap,X_OFFSET,FOCAL_PLANE_140MM+m_f3dDelta*RESOLUTION,CLIP_WIDE,CLIP_HIGH,ANGLE_COUNT);
    }
    else
    {
        _pHifu3d->LoadImages(_sceneMap,X_OFFSET,FOCAL_PLANE_140MM+m_f3dDelta*RESOLUTION,CLIP_WIDE,CLIP_HIGH,ANGLE_COUNT);
    }

    combinePlan(smart.getDensity());

    // 值颜色设置为已设置
    ui->label_power_value->setStyleSheet(UNCHANGED);
    ui->label_on_time_value->setStyleSheet(UNCHANGED);
    ui->label_off_time_value->setStyleSheet(UNCHANGED);
    ui->label_pulse_number_value->setStyleSheet(UNCHANGED);
    ui->label_cooling_time_value->setStyleSheet(UNCHANGED);

    power = ui->label_power_value->text();
    on_time = QString::number(ui->horizontalSlider_on_time->value());
    off_time = QString::number(ui->horizontalSlider_off_time->value());
    pulse_number = QString::number(ui->horizontalSlider_pulse_number->value());
    cooling_time = QString::number(ui->horizontalSlider_cooling_time->value());
    updateLog("%%%"+power+","+on_time+","+off_time+","+pulse_number+","+cooling_time+"%%%");
    _seriesSpot = 0;
    treatedSpot = 0;

    // layer scene对应的右键菜单
    initLayerMenu();
    if (!ui->pushButton_treatment_layer_down->isEnabled())
    {
        enableLayerControl();
    }

    // 设置Treatment按钮可用
    if (!ui->toolButton_therapy->isEnabled())
    {
        ui->toolButton_therapy->setEnabled(true);
        ui->toolButton_therapy->setStyleSheet(SELECTED);
    }
}

void NewMainWindow::setParameter()
{
    QString text = ui->comboBox_deepth->currentText();
    on_comboBox_deepth_currentTextChanged(text);

    // 值颜色设置为已设置
    ui->label_power_value->setStyleSheet(UNCHANGED);
    ui->label_on_time_value->setStyleSheet(UNCHANGED);
    ui->label_off_time_value->setStyleSheet(UNCHANGED);
    ui->label_pulse_number_value->setStyleSheet(UNCHANGED);
    ui->label_cooling_time_value->setStyleSheet(UNCHANGED);
}

void NewMainWindow::on_tableView_current_plan_customContextMenuRequested(const QPoint &pos)
{
    if (_tableMenu!=NULL)
    {
        disconnect(_pCurrentModel,SIGNAL(dataChanged(QModelIndex,QModelIndex)),
                this,SLOT(currentPlanChanged(QModelIndex,QModelIndex)));

        _tableMenu->exec(QCursor::pos());

        connect(_pCurrentModel,SIGNAL(dataChanged(QModelIndex,QModelIndex)),
                this,SLOT(currentPlanChanged(QModelIndex,QModelIndex)));

    }
}

// 复制Current Plan表格中选中的行
void NewMainWindow::copyRows()
{
    QItemSelectionModel* selection = ui->tableView_current_plan->selectionModel();
    QModelIndexList list = selection->selectedIndexes();
    QMap<int,QString> map;
    QMap<int,QString>::iterator iter = map.end();
    // 取出选中对应的行号
    foreach (QModelIndex index, list)
    {
        if (_pCurrentModel!=NULL)
        {
            iter = map.find(index.row());
            if (iter == map.end())
            {
                map.insert(index.row(),_pCurrentModel->item(index.row(),0)->text());
            }
        }
    }

    QVector<QString> vec;
    for (iter = map.begin();iter != map.end(); ++iter)
    {
        vec.push_back(iter.value());
    }
    // 复制对应的点
    emit copySpots(vec);

    QQueue<Irradiate_Spot> plan;
    _irradiatePlan->getDisplayPlan(plan);

    refreshCurrentTable(plan);
    setBackground();

    m_totalTime.spotsRemain += map.size();
    refreshRemainTime();
}

void NewMainWindow::stopRows()
{
    QItemSelectionModel* selection = ui->tableView_current_plan->selectionModel();
    QModelIndexList list = selection->selectedIndexes();
    int index = list.first().row();
    _pCurrentModel->item(index,5)->setBackground(QColor(255,0,0));
}

void NewMainWindow::continueRows()
{
    QItemSelectionModel* selection = ui->tableView_current_plan->selectionModel();
    QModelIndexList list = selection->selectedIndexes();
    int index = list.first().row();
    _pCurrentModel->item(index,5)->setBackground(QColor(0,255,0));
}

// DeleteCurrent Plan表格中选中的行
void NewMainWindow::deleteRows()
{
    QItemSelectionModel* selection = ui->tableView_current_plan->selectionModel();
    QModelIndexList list = selection->selectedIndexes();
    QMap<int,QString> map;
    QMap<int,QString>::iterator iter = map.end();
    // 取出选中对应的行号
    foreach (QModelIndex index, list)
    {
        if (_pCurrentModel!=NULL)
        {
            iter = map.find(index.row());
            if (iter == map.end())
            {
                map.insert(index.row(),_pCurrentModel->item(index.row(),0)->text());
            }
        }
    }

    QVector<QString> vec;
    for (iter = map.begin();iter != map.end(); ++iter)
    {
        // 已经sonication过的行，不能Delete
        if (_currentCompleteMap.find(iter.value())==_currentCompleteMap.end())
        {
            vec.push_back(iter.value());
        }

    }

    // Delete对应的sonication point
    if(!_isIrradiating)
    {
        emit deleteSpots(vec);

        m_totalTime.spotsRemain -= map.size();
        refreshRemainTime();

        QQueue<Irradiate_Spot> plan;
        _irradiatePlan->getDisplayPlan(plan);
        refreshCurrentTable(plan);
        setBackground();
    }
}

void NewMainWindow::on_graphicsView_realtime_image_customContextMenuRequested(const QPoint &pos)
{
    if (_realTimeMenu!= NULL)
    {
        _realTimeMenu->exec(QCursor::pos());
    }
}

void NewMainWindow::on_graphicsView_target_scene_customContextMenuRequested(const QPoint &pos)
{
    if (_targetMenu!=NULL)
    {
        _targetMenu->exec(QCursor::pos());
    }
}

void NewMainWindow::targetShowElement()    // Show Sonication Cells
{
    if (_targetScene!=NULL)
    {
        _targetScene->showElement();
    }
}

void NewMainWindow::targetResumeImages()
{
   if (_targetScene!=NULL)
   {
       _targetScene->setScal(1.0);
       _targetScene->setSceneRect(0,0,574,574);
       _viewer2DInfo.scal = 1.0;
       _viewer2DInfo.rect = QRectF(0,0,574,574);
   }
}

void NewMainWindow::targetShowContours()   // target显示fibroid轮廓
{
    if (_targetScene!=NULL)
    {
        _contoursVisiable = true;
        _targetScene->setContoursVisiable(true);
    }
}

void NewMainWindow::targetShowDraw()       // targetShow Traced Contour
{
    QMap<int,DrawScene*>::iterator iter = _targetSceneMap.begin();
    for (; iter != _targetSceneMap.end(); ++iter)
    {
        iter.value()->showDraw();
    }
}

void NewMainWindow::targetHideDraw()       // targetHide Traced Contour
{
    QMap<int,DrawScene*>::iterator iter = _targetSceneMap.begin();
    for (; iter != _targetSceneMap.end(); ++iter)
    {
        iter.value()->hideDraw();
    }
}

void NewMainWindow::targetDrag()
{
    if(_targetScene!=NULL)
    {
        _targetScene->setOperator(DRAG);
    }
}

void NewMainWindow::targetDragConfirm()
{
    if(_targetScene!=NULL)
    {
        if(_targetScene->getOperator()==DRAG)
        {
            _targetScene->setOperator(OPER_DRAW_TARGET);
        }
    }
}

void NewMainWindow::targetHideContours()   // target隐藏fibroid轮廓
{
    if (_targetScene!=NULL)
    {
        _contoursVisiable = false;
        _targetScene->setContoursVisiable(false);
    }
}

void NewMainWindow::targetZoomOut()        // target窗口Zoom In
{
    if (_targetScene!=NULL)
    {
        _targetScene->zoomOut();
    }
}

void NewMainWindow::targetZoomIn()         // target窗口Zoom Out
{
    if (_targetScene!=NULL)
    {
        _targetScene->zoomIn();
    }
}

void NewMainWindow::layerShowDeleted()     // layer显示Delete的sonication cell
{
    showDeletedElement();
}

void NewMainWindow::layerSelectInvert()    // 反选
{
    for (int i = 0; i < _layerScenes.size(); ++i)
    {
        _layerScenes[i]->selectInvert();
    }
}

void NewMainWindow::layerResumeDeleted()   // Restore Deleted Cells
{
    QMap<int,DrawScene*>::iterator iter = _targetSceneMap.begin();
    for (; iter != _targetSceneMap.end(); ++iter)
    {
        iter.value()->resumeDelected();
    }

    for (int i = 0; i < _layerScenes.size(); ++i)
    {
        _layerScenes[i]->resumeDelected();
    }
}

void NewMainWindow::layerHideDeleted()     // layer隐藏Delete的sonication cell
{
    hideDeletedElement();
}

void NewMainWindow::targetShowDeleted()    // targetShow Deleted Cells
{
    showDeletedElement();
}

void NewMainWindow::targetSelectInvert()
{
    if (_targetSceneMap.empty())
    {
        return;
    }

    QMap<int,DrawScene*>::iterator iter = _targetSceneMap.begin();
    for (; iter != _targetSceneMap.end(); ++iter)
    {
        iter.value()->selectInvert();
    }
}

void NewMainWindow::targetSmartSelect()
{
    if (_targetSceneMap.empty())
    {
        return;
    }

    QMap<int,DrawScene*>::iterator iter = _targetSceneMap.begin();
    for (; iter != _targetSceneMap.end(); ++iter)
    {
        iter.value()->smartSelect();
    }
}

void NewMainWindow::targetResumeDeleted()
{
    if (_targetSceneMap.empty())
    {
        return;
    }

    QMap<int,DrawScene*>::iterator iter = _targetSceneMap.begin();
    for (; iter != _targetSceneMap.end(); ++iter)
    {
        iter.value()->resumeDelected();
    }
}

void NewMainWindow::targetHideDeleted()    // targetHide Deleted Cells
{
    hideDeletedElement();
}

void NewMainWindow::targetHideElement()    // 隐藏未标红的sonication cell
{
    if (_targetScene!=NULL)
    {
        _targetScene->hideElement();
    }
}

void NewMainWindow::on_graphicsView_treatment_layer_customContextMenuRequested(const QPoint &pos)
{
    if (_layerMenu!=NULL)
    {
        _layerMenu->exec(QCursor::pos());
    }
}

//void NewMainWindow::on_commandLinkButton_save_target_clicked()
//{
//    if (_targetScene!=NULL && _irradiatePlan!=NULL)
//    {
//        _irradiatePlan->refreshPlan(_targetScene->getAngle(),
//                                    _targetScene->getFocusX(),
//                                    _targetScene->getFocusY(),
//                                    _targetScene->getFocusZ(),
//                                    _targetScene->getIdBorn(),
//                                    _totalCompleteMap);

//        _currentCompleteMap.clear();

//        QQueue<Irradiate_Spot> plan;
//        _irradiatePlan->getNextPlan(plan);

//        refreshCurrentTable(plan);
//        refreshTotalTable();
//        setBackground();
//    }
//}

void NewMainWindow::on_horizontalSlider_cooling_time_valueChanged(int value)
{
    ui->label_cooling_time_value->setText(QString::number(value));
    // _coolingTime = value;
    if (value!=_parameter.coolingTime)
    {
        ui->label_cooling_time_value->setStyleSheet(CHANGED);
        ui->toolButton_therapy->setDisabled(true);
        ui->toolButton_therapy->setStyleSheet(NOT_WORK);
    }
    else
    {
        ui->label_cooling_time_value->setStyleSheet(UNCHANGED);
    }
}

void NewMainWindow::on_label_target_image_customContextMenuRequested(const QPoint &pos)
{

}

void NewMainWindow::on_toolButton_save_video_2_clicked()
{
    if (_grabber!=NULL)
    {
        if (!_isOnSaveVideo)
        {
            FileSaveDialog *fdig=new FileSaveDialog(PA_VIDEO);
            if(fdig->exec()!=QDialog::Accepted)
            {
                delete fdig;
                return;
            }
            ui->toolButton_save_video_2->setStyleSheet(OPERATING);
            QDir dir(PatientManager::getInstance()->getVideoPath());
            if (!dir.exists())
            {
                dir.mkpath(PatientManager::getInstance()->getVideoPath());
            }
//            QDateTime current_date_time = QDateTime::currentDateTime();
//            QString current_date = current_date_time.toString("yyyy-MM-dd_hh-mm-ss");
//            QString file("E:\\");
//            file.append(current_date);
//            file.append(".avi");
            m_realPath = PatientManager::getInstance()->getVideoFullPath();
            m_tmpPath = QStringLiteral("%1.avi").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd_hh-mm-ss-zzz"));
            emit SaveVideo(QString("%1").arg(m_realPath));
//            emit SaveVideo(QString("%1").arg(m_tmpPath));
//            _grabber->startSaveVideo(QString("%1").arg(m_tmpPath));
            _isOnSaveVideo = true;
            delete fdig;
        }
        else
        {
            _grabber->stopSaveVideo();
            ui->toolButton_save_video_2->setStyleSheet(SELECTED);
            _isOnSaveVideo = false;

//            QFile::copy(m_tmpPath, m_realPath);
//            QFile::remove(m_tmpPath);
            QMessageBox msgBox("Notice",
                               "Video saved.",
                               QMessageBox::Information,
                               QMessageBox::Ok | QMessageBox::Escape,
                               QMessageBox::NoButton,
                               QMessageBox::NoButton);
            msgBox.setWindowFlags(Qt::WindowTitleHint|Qt::CustomizeWindowHint|Qt::WindowStaysOnTopHint);
            msgBox.setGeometry(QRect(1400,400,msgBox.width(),msgBox.height()));
            msgBox.setFont(m_msgBoxFont);
            msgBox.setButtonText(QMessageBox::Ok, "OK");
            msgBox.exec();
        }

    }
}

void NewMainWindow::on_horizontalSlider_on_time_valueChanged(int value)
{
    ui->label_on_time_value->setText(QString::number(value));

    if (value != _parameter.onTime)
    {
        ui->label_on_time_value->setStyleSheet(CHANGED);
        ui->toolButton_therapy->setDisabled(true);
        ui->toolButton_therapy->setStyleSheet(NOT_WORK);
    }
    else
    {
        ui->label_on_time_value->setStyleSheet(UNCHANGED);
    }
}

void NewMainWindow::on_horizontalSlider_off_time_valueChanged(int value)
{
    ui->label_off_time_value->setText(QString::number(value));

    if (value != _parameter.offTime)
    {
        ui->label_off_time_value->setStyleSheet(CHANGED);
        ui->toolButton_therapy->setDisabled(true);
        ui->toolButton_therapy->setStyleSheet(NOT_WORK);
    }
    else
    {
        ui->label_off_time_value->setStyleSheet(UNCHANGED);
    }
}

void NewMainWindow::on_horizontalSlider_pulse_number_valueChanged(int value)
{
    ui->label_pulse_number_value->setText(QString::number(value));

    if (value != _parameter.pulse)
    {
        // _parameter.pulse = value;
        ui->label_pulse_number_value->setStyleSheet(CHANGED);
        ui->toolButton_therapy->setDisabled(true);
        ui->toolButton_therapy->setStyleSheet(NOT_WORK);
    }
    else
    {
        ui->label_pulse_number_value->setStyleSheet(UNCHANGED);
    }
}

void NewMainWindow::on_commandLinkButton_water_circle_clicked()
{
//    _water->sendPar(PAR_METER_AUTO,PAR_ENABLE);
//    sendpar(PAR_METER_AUTO,PAR_ENABLE);
    emit showdupumpcontrol();
}

void NewMainWindow::closeEvent(QCloseEvent *event)
{
    _quitform->showFullScreen();
    Delay_MSec(3000);
    _quitform->close();
}

void NewMainWindow::QuitBeforShow()
{
    qDebug()<<"**************about to quit**************************";
    //主窗口还没有显示的时候推出程序所执行的
    if(_append!=NULL)
    {
        _append->setLightsOff();
    }
//    if (_water!=NULL)
//    {
//        if(m_bPump1Open)
//            _water->closeduPump1();
//        if(m_bPump2Open)
//            _water->closeduPump2();
//        sendpar(PAR_METER_AUTO,PAR_DISABLE);
//    }
}

void NewMainWindow::showEvent(QShowEvent *event)
{
    //this->activateWindow();
    this->setAttribute(Qt::WA_Mapped);
    QWidget::showEvent(event);
}

void NewMainWindow::on_comboBox_part_currentTextChanged(const QString &arg)
{
//    if (arg=="uterine fibroid")
    if (arg=="Myometrium")
    {
        ui->comboBox_deepth->clear();
        ui->comboBox_deepth->addItem(QWidget::tr("3"));
        ui->comboBox_deepth->addItem(QWidget::tr("5"));
        ui->comboBox_deepth->addItem(QWidget::tr("7"));
    }
    else
    {
        ui->comboBox_deepth->clear();
        ui->comboBox_deepth->addItem(QWidget::tr("3"));
        ui->comboBox_deepth->addItem(QWidget::tr("4"));
        ui->comboBox_deepth->addItem(QWidget::tr("5"));
        ui->comboBox_deepth->addItem(QWidget::tr("6"));
        ui->comboBox_deepth->addItem(QWidget::tr("7"));
        ui->comboBox_deepth->addItem(QWidget::tr("8"));
        ui->comboBox_deepth->addItem(QWidget::tr("9"));
        ui->comboBox_deepth->addItem(QWidget::tr("10"));
        ui->comboBox_deepth->addItem(QWidget::tr("11"));
        ui->comboBox_deepth->addItem(QWidget::tr("12"));
        ui->comboBox_deepth->setCurrentText("6");

        // 对应电压值
        QSettings *voltage_setting = new QSettings(SETTINGS_PATH,QSettings::IniFormat);
        double voltage1 = voltage_setting->value("Voltage/Power22").toString().toDouble();
        HalfClipHigh = voltage_setting->value("UltrasoundImage/HALF_CLIP_HIGH").toString().toInt();
        qDebug()<<"HALF_CLIP_HIGH======"<<QString::number(HalfClipHigh);
        ui->horizontalSlider_power->setMaximum(196);
        ui->horizontalSlider_power->setMinimum(1);
        ui->horizontalSlider_power->setValue(1);
        _parameter.power = 22;
        m_iLastPower = 22;
        _parameter.voltage = voltage1;


        ui->horizontalSlider_on_time->setMinimum(100);
        ui->horizontalSlider_on_time->setMaximum(1000);
        ui->horizontalSlider_on_time->setValue(100);
        if (!ui->horizontalSlider_on_time->isEnabled())
        {
            ui->horizontalSlider_on_time->setEnabled(true);
            ui->horizontalSlider_on_time->setStyleSheet(HORIZON_TYPE);
        }
        _parameter.onTime = 100;
        ui->label_on_time_value->setText(QString::number(ui->horizontalSlider_on_time->value()));

        ui->horizontalSlider_off_time->setMinimum(200);
        ui->horizontalSlider_off_time->setMaximum(2000);
        ui->horizontalSlider_off_time->setValue(200);
        if (!ui->horizontalSlider_off_time->isEnabled())
        {
            ui->horizontalSlider_off_time->setEnabled(true);
            ui->horizontalSlider_off_time->setStyleSheet(HORIZON_TYPE);
        }
        _parameter.offTime = 200;
        ui->label_off_time_value->setText(QString::number(ui->horizontalSlider_off_time->value()));

        ui->horizontalSlider_pulse_number->setMinimum(1);
        ui->horizontalSlider_pulse_number->setMaximum(20);
        ui->horizontalSlider_pulse_number->setValue(1);
        if (!ui->horizontalSlider_pulse_number->isEnabled())
        {
            ui->horizontalSlider_pulse_number->setEnabled(true);
            ui->horizontalSlider_pulse_number->setStyleSheet(HORIZON_TYPE);
        }
        _parameter.pulse = 1;
        ui->label_pulse_number_value->setText(QString::number(ui->horizontalSlider_pulse_number->value()));

        ui->horizontalSlider_cooling_time->setMinimum(0);
        //ui->horizontalSlider_cooling_time->setMaximum(30);
        ui->horizontalSlider_cooling_time->setValue(0);
        if (!ui->horizontalSlider_cooling_time->isEnabled())
        {
            ui->horizontalSlider_cooling_time->setEnabled(true);
            ui->horizontalSlider_cooling_time->setStyleSheet(HORIZON_TYPE);
        }
        _parameter.coolingTime = 0;
        ui->label_cooling_time_value->setText(QString::number(ui->horizontalSlider_cooling_time->value()));

        ui->label_power_value->setStyleSheet(UNCHANGED);
        ui->label_on_time_value->setStyleSheet(UNCHANGED);
        ui->label_off_time_value->setStyleSheet(UNCHANGED);
        ui->label_pulse_number_value->setStyleSheet(UNCHANGED);
        ui->label_cooling_time_value->setStyleSheet(UNCHANGED);
    }
}

void NewMainWindow::on_comboBox_deepth_currentTextChanged(const QString &arg)
{

    if (_para==NULL)
    {
        qCDebug(NEWMAINWINDOW()) << "_para NULL"
                                 << TRACE_CMH();
        return;
    }

//    if(arg=="3")
//    {
//        ui->horizontalSlider_power->setMaximum(196);
//        ui->horizontalSlider_power->setMinimum(1);
//        ui->horizontalSlider_power->setValue(64);
//        _parameter.power = 64;
//        m_iLastPower = 64;
//        _parameter.voltage = voltage_8;

//        ui->horizontalSlider_on_time->setMinimum(0);
//        ui->horizontalSlider_on_time->setMaximum(1000);
//        ui->horizontalSlider_on_time->setValue(400);
//        if (!ui->horizontalSlider_on_time->isEnabled())
//        {
//            ui->horizontalSlider_on_time->setEnabled(true);
//            ui->horizontalSlider_on_time->setStyleSheet(HORIZON_TYPE);
//        }
//        _parameter.onTime = 400;

//        ui->horizontalSlider_off_time->setMinimum(0);
//        ui->horizontalSlider_off_time->setMaximum(2000);
//        ui->horizontalSlider_off_time->setValue(400);
//        if (!ui->horizontalSlider_off_time->isEnabled())
//        {
//            ui->horizontalSlider_off_time->setEnabled(true);
//            ui->horizontalSlider_off_time->setStyleSheet(HORIZON_TYPE);
//        }
//        _parameter.offTime = 400;

//        ui->horizontalSlider_pulse_number->setMinimum(1);
//        ui->horizontalSlider_pulse_number->setMaximum(30);
//        ui->horizontalSlider_pulse_number->setValue(6);
//        if (!ui->horizontalSlider_pulse_number->isEnabled())
//        {
//            ui->horizontalSlider_pulse_number->setEnabled(true);
//            ui->horizontalSlider_pulse_number->setStyleSheet(HORIZON_TYPE);
//        }
//        _parameter.pulse = 6;

//        ui->horizontalSlider_cooling_time->setMinimum(0);
//        //ui->horizontalSlider_cooling_time->setMaximum(30);
//        ui->horizontalSlider_cooling_time->setValue(0);
//        if (!ui->horizontalSlider_cooling_time->isEnabled())
//        {
//            ui->horizontalSlider_cooling_time->setEnabled(true);
//            ui->horizontalSlider_cooling_time->setStyleSheet(HORIZON_TYPE);
//        }
//        _parameter.coolingTime = 0;

//        ui->label_power_value->setStyleSheet(UNCHANGED);
//        ui->label_on_time_value->setStyleSheet(UNCHANGED);
//        ui->label_off_time_value->setStyleSheet(UNCHANGED);
//        ui->label_pulse_number_value->setStyleSheet(UNCHANGED);
//        ui->label_cooling_time_value->setStyleSheet(UNCHANGED);
//    }

//    if (ui->comboBox_part->currentText()=="uterine fibroid")
    if (ui->comboBox_part->currentText()=="Myometrium")
    {
        int deep = ui->comboBox_deepth->currentText().toInt();
        Element ele;
        if (_para->getElement(deep,ele))
        {
            setControlRange(ele);
        }

        ui->label_power_value->setStyleSheet(UNCHANGED);
        ui->label_on_time_value->setStyleSheet(UNCHANGED);
        ui->label_off_time_value->setStyleSheet(UNCHANGED);
        ui->label_pulse_number_value->setStyleSheet(UNCHANGED);
        ui->label_cooling_time_value->setStyleSheet(UNCHANGED);
    }
    else
    {
        return;
    }

}

void NewMainWindow::setControlRange(Element ele)
{
    ui->horizontalSlider_power->setMaximum((ele.voltage)*(ele.voltage));
    ui->horizontalSlider_power->setMinimum((6)*(6));
    ui->horizontalSlider_power->setValue(ele.voltage*ele.voltage);
    _parameter.power = ele.voltage*ele.voltage;
    m_iLastPower = _parameter.power;
    _parameter.voltage = ele.voltage;

//    ui->horizontalSlider_on_time->setMaximum(ele.on_time);
//    ui->horizontalSlider_on_time->setMinimum(ele.on_time);
    ui->horizontalSlider_on_time->setValue(ele.on_time);
    ui->horizontalSlider_on_time->setEnabled(false);
    ui->horizontalSlider_on_time->setStyleSheet(NOT_SELECTED);
    _parameter.onTime = ele.on_time;

    ui->horizontalSlider_off_time->setValue(ele.off_time);
    ui->horizontalSlider_off_time->setEnabled(false);
    ui->horizontalSlider_off_time->setStyleSheet(NOT_SELECTED);
    _parameter.offTime = ele.off_time;

    ui->horizontalSlider_pulse_number->setValue(ele.pulse_number);
    ui->horizontalSlider_pulse_number->setEnabled(false);
    ui->horizontalSlider_pulse_number->setStyleSheet(NOT_SELECTED);
    _parameter.pulse = ele.pulse_number;

    ui->horizontalSlider_cooling_time->setValue(ele.cooling_time);
    ui->horizontalSlider_cooling_time->setEnabled(false);
    ui->horizontalSlider_cooling_time->setStyleSheet(NOT_SELECTED);

    _parameter.coolingTime = ele.cooling_time;
}

//重新Refresh子系统Status并发送给自检界面
void NewMainWindow::RefreshSubsystem()
{
    if(!_subSysStatus[NET_WORK]){
        qDebug()<<"RefreshSubsystem[NET_WORK]";
        if (/*_schedule!=NULL*/_core!=NULL){
//            _schedule->onServiceCalled(INQUIRY, NETWORK);
            if(_core->init())
                emit SendSubSysStatus(inetwork,true);
            else
                emit SendSubSysStatus(inetwork,false);
            QEventLoop eventloop;
            QTimer::singleShot(500, &eventloop, SLOT(quit()));
            eventloop.exec();
        }
    }
    else
    {
        emit SendSubSysStatus(inetwork,true);
    }
    qApp->processEvents(QEventLoop::ExcludeUserInputEvents);

    if(!_subSysStatus[MECHINE_MOVE])
    {
        qDebug()<<"RefreshSubsystem[MECHINE_MOVE]";
        initLocation();
        locationZero();
    }
    else
    {
        if(BedZero)
            emit SendSubSysStatus(ilocation,true);
//        else
//            emit SendSubSysStatus(ilocation,false);
    }
    qApp->processEvents(QEventLoop::ExcludeUserInputEvents);

    if(!_subSysStatus[WATER_CIRCLE]){
        qDebug()<<"RefreshSubsystem[WATER_CIRCLE]";
        if(_dupumpControl!=NULL)
        {
            if (_dupumpControl->init())
            {
                emit SendSubSysStatus(iwarter,true);
            }
            else
            {
                emit SendSubSysStatus(iwarter,false);
            }
        }
    }
    else
    {
        emit SendSubSysStatus(iwarter,true);
    }
    qApp->processEvents(QEventLoop::ExcludeUserInputEvents);

    if(!_subSysStatus[SUPER_B]){
        qDebug()<<"RefreshSubsystem[SUPER_B]";
        initGrabber();
    }
    else
    {
        emit SendSubSysStatus(iimagegraber,true);
    }
    qApp->processEvents(QEventLoop::ExcludeUserInputEvents);

    if(!_append->exist())
    {
        qDebug()<<"RefreshSubsystem[_append]";
        initAppend();
    }
    else
    {
        if (_subSysStatus[0]&&_subSysStatus[1]&&_subSysStatus[2]&&_subSysStatus[3]&&_subSysStatus[4]&&powerampready)
        {
            ui->label_alarm_light->setPixmap(QPixmap(QString(ICON_PIC_PATH).append("green.png")));
            _append->setRedOff();
//            if (_pMediaPlayer!=NULL)
//            {
//                _pMediaPlayer->stop();
//            }
        }
        else
        {
            ui->label_alarm_light->setPixmap(QPixmap(QString(ICON_PIC_PATH).append("red.png")));
            _append->setRedOn();
            qCDebug(NEWMAINWINDOW())<<NEWMAINWINDOW().categoryName()
                                 <<"Error refresh subsystem failed !"
                                 << TRACE_CMH();

//            if (_pMediaPlayer!=NULL)
//            {
//                _pMediaPlayer->play();
//            }
        }
        emit SendSubSysStatus(iappned,true);
    }
    qApp->processEvents(QEventLoop::ExcludeUserInputEvents);
}

// 处理windows消息
bool NewMainWindow::nativeEvent(const QByteArray &eventType, void *message, long *result)
{
    if (_location==NULL)
    {
        return false;
    }

    if (eventType!="windows_generic_MSG")
    {
        qDebug()<<"event type is "<< eventType;
        return false;
    }

    MSG* msg = static_cast<MSG*>(message);
    switch (msg->message)
    {
    case WM_USER+777:
        qDebug()<<"message is "<< msg->message;
        qDebug()<<"wparam is "<< msg->wParam;
        qDebug()<<"lParam is "<< msg->lParam;

        _location->setPos(msg->wParam,msg->lParam);

        return true;
        break;

    default:
       return QWidget::nativeEvent(eventType,message,result);

    }

}

void NewMainWindow::DelayClose()
{
    delayTimer.setSingleShot(true);
    delayTimer.setInterval(500);
    delayTimer.start();
    connect(&delayTimer,SIGNAL(timeout()),this,SLOT(hide()));
}

void NewMainWindow::on_horizontalScrollBar_slices_valueChanged(int value)
{
    if (_pNavigation==NULL)
    {
        qCDebug(NEWMAINWINDOW()) << "_pNavigation NULL"
                                 << TRACE_CMH();
        return;
    }

    value = value -1;
    // qDebug()<<"slices value change value is "<<value;
    if (value < 0 || value > 180)
    {
        return;
    }

    if(_sliceSceneMap.isEmpty())
        return;

    _slicesScene = _sliceSceneMap[value];
    ui->graphicsView_slices->setScene(_slicesScene);

    QString str(QString::number(value-90));
    str.append(" °");
    ui->label_mr_angle->setText(str);
}

void NewMainWindow::registContour(DrawScene* scene,int angle,const QImage& image)
{
    qCDebug(NEWMAINWINDOW()) << NEWMAINWINDOW().categoryName()
                          << "find corrspond scene for angle is "<< angle-90
                          << TRACE_CMH();

    // 传递过来的图像为1 pixel/mm
    QImage scaled = image.scaled(image.width()*RESOLUTION,image.height()*RESOLUTION);


//    qCDebug(NEWMAINWINDOW()) << NEWMAINWINDOW().categoryName()
//                          << "w=============="<<image.width()
//                           <<"h=============="<<image.height()
//                          << TRACE_CMH();

//    QString str2 = QString("E:\\revscale\\contour%1.bmp").arg(angle);
//    QString str = QString("E:\\revimage\\contour%1.bmp").arg(angle);
//    image.save(str.toStdString().c_str());
//    scaled.save(str2.toStdString().c_str());




    m_contours.insert(angle,scaled);
    QMap<int,QPointF>::iterator iter = _contoursMoveMap.find(angle/4);
    if (iter==_contoursMoveMap.end())
    {
        // Regist algo(scene,&scaled,m_iSearchRadius);
//        float para[4] = {X_OFFSET,FOCAL_PLANE_140MM+m_f3dDelta*RESOLUTION,0.0,0.8};
        float para[4] = {X_OFFSET,delta*RESOLUTION+140 * RESOLUTION+ZERO_DEVIATION,0.0,0.8};

//        if (!algo.match(angle,para[0],para[1],para[2],para[3]))
//        {

//            para[3] = 0.0f;
//            para[0] = 0.0f;
//            para[1] = 0.0f;
//            // mapScore.insert(0.0f,qMakePair(angle,QPointF(0.0,0.0)));
//        }


        double score = 0.1*angle;
        mapScore.insert(score,qMakePair(angle,QPointF(para[0],para[1])));
        _contoursMoveMap.insert(angle/4,QPointF(para[0],para[1]));
    }

    if (_contoursMoveMap.size()==46)
    {
        QMap<float,QPair<int,QPointF> >::iterator iter = mapScore.begin();
        QVector<int> tempIndex;

        for (iter;iter != mapScore.end();++iter)
        {
            if (iter.key()>0.20)
            {
                m_movPos.insert(iter.value().first,iter.value().second);
            }
        }

        if (m_movPos.size()<8)
        {
            m_movPos.clear();
            _contoursMoveMap.clear();
            QMessageBox msgBox("Registration Failed",
                               "Too few reliable results. Confirm the fibroid contour.",
                               QMessageBox::Critical,
                               QMessageBox::Yes | QMessageBox::Default,
                               QMessageBox::No | QMessageBox::Escape,
                               QMessageBox::NoButton);
            msgBox.setWindowFlags(Qt::WindowTitleHint|Qt::CustomizeWindowHint|Qt::WindowStaysOnTopHint);
            msgBox.setGeometry(QRect(1400,400,msgBox.width(),msgBox.height()));
            msgBox.setFont(m_msgBoxFont);
            msgBox.setButtonText(QMessageBox::Yes, "OK");
            msgBox.setButtonText(QMessageBox::No, "Cancel");

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

        for (QMap<int,QPointF>::iterator loop = m_movPos.begin();
             loop != m_movPos.end();++loop)
        {
            tempIndex.push_back(loop.key());
        }

        for (int j = 0; j <=180;++j)
        {
            if (j<tempIndex[0])
            {
                m_movPos.insert(j,m_movPos[tempIndex[0]]);
                continue;
            }

            if (j>tempIndex[tempIndex.size()-1])
            {
                m_movPos.insert(j,m_movPos[tempIndex[tempIndex.size()-1]]);
                continue;
            }

            for (int loop = 0; loop < tempIndex.size()-1;++loop)
            {
                if (j<tempIndex[loop+1]&&j>tempIndex[loop])
                {

                    float x = m_movPos[tempIndex[loop]].x()+(m_movPos[tempIndex[loop+1]].x()-m_movPos[tempIndex[loop]].x())*(j-tempIndex[loop])/(tempIndex[loop+1]-tempIndex[loop]);
                    float y = m_movPos[tempIndex[loop]].y()+(m_movPos[tempIndex[loop+1]].y()-m_movPos[tempIndex[loop]].y())*(j-tempIndex[loop])/(tempIndex[loop+1]-tempIndex[loop]);
                    m_movPos.insert(j,QPointF(x,y));
                }
            }
        }

        moveContour();
        m_contours.clear();



        // emit sendGetContour();
    }
}

void NewMainWindow::receiveOuter(QImage outer,int angle)
{
    if (angle==0)
    {
        // 清除以前的registration数据
        m_movPos.clear();
        _contoursMoveMap.clear();
    }

    QMap<int,DrawScene*>::iterator iter = _sceneMap.find(angle-90);
    if (iter != _sceneMap.end())
    {
        DrawScene* scene = iter.value();
        registContour(scene,angle,outer);

//        QString str = QString("D:\\contour%1.bmp").arg(angle);
//        image.save(str.toStdString().c_str());
    }
}

void NewMainWindow::moveContour()
{

    QMap<int,QImage>::iterator iContours = m_contours.begin();
    for (iContours = m_contours.begin();iContours!=m_contours.end();++iContours)
    {



        QMap<int,DrawScene*>::iterator iter = _sceneMap.find(iContours.key()-90);
        QMap<int,QPointF>::iterator i = m_movPos.find(iContours.key());
        if (iter!=_sceneMap.end()&&i!=m_movPos.end())
        {

            Regist algo(iter.value(),&iContours.value());
            iContours.value().save("E:\\testimage2.png");
            algo.move(i.value());

        }
    }
}

void NewMainWindow::receiveContour(QImage image,int angle)
{
    QImage contour = image.scaled(image.width()*RESOLUTION,image.height()*RESOLUTION);

    QMap<int,DrawScene*>::iterator iter = _sceneMap.find(angle-90);
    QMap<int,QPointF>::iterator i = m_movPos.find(angle);
    if (iter!=_sceneMap.end()&&i!=m_movPos.end())
    {
        Regist algo(iter.value(),&contour);
        algo.move(i.value());
    }
	else
    {
    }
}

void NewMainWindow::receiveQImage(QImage image,int angle)
{
    DrawScene* tempScene;
    QMap<int,DrawScene*>::iterator iter = _sliceSceneMap.find(angle);
    if (iter == _sliceSceneMap.end())
    {
        tempScene = new DrawScene(angle);
        tempScene->setBackgroundPixmap(QPixmap::fromImage(image));

        _sliceSceneMap.insert(angle,tempScene);
    }
    else
    {
        tempScene = iter.value();
        tempScene->setBackgroundPixmap(QPixmap::fromImage(image));
    }

    if(_sliceSceneMap.size()==1)
    {
        _slicesScene = _sliceSceneMap[0];
        ui->graphicsView_slices->setScene(_slicesScene);

        QString str(QString::number(-90));
        str.append(" °");
        ui->label_mr_angle->setText(str);

        ui->horizontalScrollBar_slices->setMaximum(181);
    }
}

void NewMainWindow::on_pushButton_resume_para_clicked()
{
    ui->horizontalSlider_power->setValue(1);
    ui->horizontalSlider_on_time->setValue(100);
    ui->horizontalSlider_off_time->setValue(200);
    ui->horizontalSlider_pulse_number->setValue(1);
    ui->horizontalSlider_cooling_time->setValue(0);

//    ui->horizontalSlider_power->setValue(_parameter.power);
//    ui->horizontalSlider_on_time->setValue(_parameter.onTime);
//    ui->horizontalSlider_off_time->setValue(_parameter.offTime);
//    ui->horizontalSlider_pulse_number->setValue(_parameter.pulse);
//    ui->horizontalSlider_cooling_time->setValue(_parameter.coolingTime);
}

void NewMainWindow::on_spinBox_angle_one_valueChanged(int arg)
{
    ui->horizontalScrollBar_images->setValue(arg+91);
    on_horizontalScrollBar_images_sliderReleased();
}

void NewMainWindow::on_spinBox_angle_two_valueChanged(int arg)
{
    ui->horizontalScrollBar_images->setValue(arg+91);
    on_horizontalScrollBar_images_sliderReleased();
}

void NewMainWindow::on_checkBox_draw_one_toggled(bool checked)
{

    if (_targetScene==NULL)
    {
        return;
    }

    if(_targetScene->getOperator()==OPER_MEASURE)
    {
        _targetScene->setOperator(OPER_DRAW_TARGET);
    }
    if (checked)
    {
        stat._bDrawable = true;
        stat._bTargetDrawable = true;
        stat._bMeasureable =false;

        if(!ui->pushButton_measure_length->isEnabled())
        {
            ui->pushButton_measure_length->setEnabled(true);
            ui->pushButton_cancel->setDisabled(false);
            ui->pushButton_measure_length->setStyleSheet(SELECTED);
            ui->pushButton_cancel->setStyleSheet(NOT_WORK);
        }

        _targetScene->setDrawable(stat._bDrawable);
        _targetScene->setMeasureDrawable(stat._bMeasureable);
        _targetScene->setTargetDrawable(stat._bTargetDrawable);


        int angle = ui->spinBox_angle_one->value();
        ui->horizontalScrollBar_images->setValue(angle+91);
        m_bDrawOutline = true;
        on_horizontalScrollBar_images_sliderReleased();
    }
}

void NewMainWindow::on_checkBox_confirm_one_toggled(bool checked)
{
    if (_targetScene==NULL)
    {
        return;
    }


    if(_targetScene->getOperator()==OPER_MEASURE)
    {
        _targetScene->setOperator(OPER_DRAW_TARGET);
    }

    if (checked)
    {
        m_bDrawOutline = false;
        ui->checkBox_confirm_two->setEnabled(true);
        ui->checkBox_draw_two->setEnabled(true);
        stat._bDrawable = false;
        stat._bTargetDrawable = false;
        stat._bMeasureable = false;

        _targetScene->setDrawable(stat._bDrawable);
        _targetScene->setMeasureDrawable(stat._bMeasureable);
        _targetScene->setTargetDrawable(stat._bTargetDrawable);

        if(!ui->pushButton_measure_length->isEnabled())
        {
            ui->pushButton_measure_length->setDisabled(false);
            ui->pushButton_cancel->setEnabled(false);
            ui->pushButton_measure_length->setStyleSheet(SELECTED);
            ui->pushButton_cancel->setStyleSheet(NOT_WORK);

        }


    }
    if(!_outlineMap.isEmpty())
    {
        _outlineMap.clear();
    }
    int angle = ui->spinBox_angle_one->value();
    _outlineMap.insert(angle,_targetScene);
}


void NewMainWindow::on_checkBox_draw_two_toggled(bool checked)
{
    if (_targetScene==NULL)
    {
        return;
    }


    if(_targetScene->getOperator()==OPER_MEASURE)
    {
        _targetScene->setOperator(OPER_DRAW_TARGET);
    }

    if (checked)
    {
        m_bDrawOutline = true;
        stat._bDrawable = true;
        stat._bTargetDrawable = true;
        stat._bMeasureable = false;
        int angle = ui->spinBox_angle_one->value();
        ui->spinBox_angle_two->setValue(angle + 90);
        qDebug()<<"angle two value is "<< angle+90;
        ui->horizontalScrollBar_images->setValue(angle+91+90);

        _targetScene->setDrawable(stat._bDrawable);
        _targetScene->setMeasureDrawable(stat._bMeasureable);
        _targetScene->setTargetDrawable(stat._bTargetDrawable);

        if(!ui->pushButton_measure_length->isEnabled())
        {
            ui->pushButton_measure_length->setEnabled(true);
            ui->pushButton_cancel->setEnabled(false);
            ui->pushButton_measure_length->setStyleSheet(SELECTED);
            ui->pushButton_cancel->setStyleSheet(NOT_WORK);

        }

        on_horizontalScrollBar_images_sliderReleased();
    }
}

void NewMainWindow::on_checkBox_confirm_two_toggled(bool checked)
{
    if (_targetScene==NULL)
    {
        return;
    }


    if(_targetScene->getOperator()==OPER_MEASURE)
    {
        _targetScene->setOperator(OPER_DRAW_TARGET);
    }

    if (checked)
    {
        stat._bDrawable = false;
        stat._bTargetDrawable = false;
        stat._bMeasureable = false;
        m_bDrawOutline = false;

        _targetScene->setDrawable(stat._bDrawable);
        _targetScene->setMeasureDrawable(stat._bMeasureable);
        _targetScene->setTargetDrawable(stat._bTargetDrawable);

        if(!ui->pushButton_measure_length->isEnabled())
        {
            ui->pushButton_measure_length->setDisabled(false);
            ui->pushButton_cancel->setEnabled(false);
            ui->pushButton_measure_length->setStyleSheet(SELECTED);
            ui->pushButton_cancel->setStyleSheet(NOT_WORK);

        }
    }

    int angle = ui->spinBox_angle_two->value();
    _outlineMap.insert(angle,_targetScene);
}

void NewMainWindow::on_pushButton_draw_spot_clicked()
{
    if (_outlineMap.size()!=2)
    {
        qCDebug(NEWMAINWINDOW()) << NEWMAINWINDOW().categoryName()
                                 << "_outlineMap map size error "<<_targetSceneMap.size()
                                 << TRACE_CMH();
        return;
    }
    ui->toolButton_therapy->setDisabled(true);
    ui->toolButton_therapy->setStyleSheet(NOT_WORK);

    QMap<int,DrawScene*>::iterator iter = _outlineMap.begin();
    DrawScene* scene0 = iter.value();
    ++iter;
    DrawScene* scene1 = iter.value();
    QPainterPath path;
    scene0->getIntersectPath(scene1,path);

    for (iter = _sceneMap.begin();iter != _sceneMap.end();++iter)
    {
          iter.value()->setTargetPath(path);
    }

    m_bDrawOutline = false;
    stat._bTargetDrawable = false;
    stat._bMeasureable = true;
    _targetScene->setDrawable(stat._bDrawable);
    _targetScene->setTargetDrawable(stat._bTargetDrawable);

    _outlineMap.clear();


    if(_targetScene->getOperator()==OPER_MEASURE)
    {
        _targetScene->setOperator(OPER_DRAW_TARGET);
    }

}

void NewMainWindow::on_pushButton_clear_contour_clicked()
{
    for (QMap<int,DrawScene*>::iterator iter = _sceneMap.begin();iter != _sceneMap.end();++iter)
    {
        iter.value()->clearTargetPath();
    }

    if(_targetScene->getOperator()==OPER_MEASURE)
    {
        _targetScene->setOperator(OPER_DRAW_TARGET);
    }

    ui->checkBox_draw_one->setChecked(false);
    ui->checkBox_draw_two->setChecked(false);
    ui->checkBox_confirm_one->setChecked(false);
    ui->checkBox_confirm_two->setChecked(false);

    ui->checkBox_confirm_two->setEnabled(false);
    ui->checkBox_draw_two->setEnabled(false);
    m_bDrawOutline = false;
    stat._bTargetDrawable = false;
    stat._bMeasureable = false;
    if(ui->commandLinkButton_generate_plan->isEnabled())
    {
        stat._bDrawable = true;
    }
    else
    {
        stat._bDrawable = false;
    }
}

void NewMainWindow::on_comboBox_irradiate_model_currentIndexChanged(const QString &arg1)
{
    if (ui->comboBox_irradiate_model->currentText()=="Inside Out")
    {
        m_eModel = Model_inner_to_outer;
    }
    else if (ui->comboBox_irradiate_model->currentText()=="Outside In")
    {
        m_eModel = Model_outer_to_inner;
    }
    else
    {
        qCDebug(NEWMAINWINDOW())<<NEWMAINWINDOW().categoryName()
                                <<"irradiate model change text unknown "
                                <<TRACE_CMH();

        m_eModel = Model_inner_to_outer;
    }
}

void NewMainWindow::on_toolButton_header_zero_clicked()
{
    if(_pHeaderControl!=NULL)
    {
       // 设置禁止窗口最大最小化
       _pHeaderControl->setWindowFlags(Qt::WindowStaysOnTopHint | Qt::WindowCloseButtonHint);
       _pHeaderControl->setFixedSize(_pHeaderControl->width(),_pHeaderControl->height());
       _pHeaderControl->show();
    }
    emit getRodPos();
    emit updateHeadPos(posL,posF,posZ);
}

void NewMainWindow::on_pushButton_modify_para_clicked()
{
    setTreatmentParaControlEnable();

    if (!ui->pushButton_confirm_para->isEnabled())
    {
        ui->pushButton_confirm_para->setEnabled(true);
        ui->pushButton_confirm_para->setStyleSheet(SELECTED);
    }
    ui->pushButton_start_treat->setEnabled(false);
    ui->pushButton_start_treat->setStyleSheet(NOT_WORK);
    is_plan_completed = true;
}

void NewMainWindow::on_pushButton_confirm_para_clicked()
{
    setTreatmentParaControlDisable();

    // 设置自身不可用
    ui->pushButton_confirm_para->setEnabled(false);
    ui->pushButton_confirm_para->setStyleSheet(NOT_WORK);
    ui->pushButton_start_treat->setEnabled(true);
    ui->pushButton_start_treat->setStyleSheet(SELECTED);
    _seriesSpot = 0;

    int pulseCount = ui->horizontalSlider_pulse_number->value();
    int offTime = ui->horizontalSlider_off_time->value();
    int onTime = ui->horizontalSlider_on_time->value();
    int coolingTime = ui->horizontalSlider_cooling_time->value();
    double voltage = getVoltage(ui->horizontalSlider_power->value());

    power = ui->label_power_value->text();
    on_time = QString::number(ui->horizontalSlider_on_time->value());
    off_time = QString::number(ui->horizontalSlider_off_time->value());
    pulse_number = QString::number(ui->horizontalSlider_pulse_number->value());
    cooling_time = QString::number(ui->horizontalSlider_cooling_time->value());
    updateLog("%%%"+power+","+on_time+","+off_time+","+pulse_number+","+cooling_time+"%%%");

    m_totalTime.onTime = onTime;
    m_totalTime.offTime = offTime;
    m_totalTime.pulseNumber = pulseCount;
    m_totalTime.coolingTime = coolingTime;
    refreshRemainTime();

    if (_irradiatePlan!=NULL)
    {
        _irradiatePlan->refreshIrradiatePara(voltage,onTime,offTime,
                                             coolingTime,pulseCount,
                                             _currentCompleteMap,
                                             _totalCompleteMap);
        QQueue<Irradiate_Spot> plan;
        _irradiatePlan->getDisplayPlan(plan);
        refreshCurrentTable(plan);
        // refreshTotalTable();
        setBackground();

        ui->label_power_value->setStyleSheet(UNCHANGED);
        ui->label_on_time_value->setStyleSheet(UNCHANGED);
        ui->label_off_time_value->setStyleSheet(UNCHANGED);
        ui->label_pulse_number_value->setStyleSheet(UNCHANGED);
        ui->label_cooling_time_value->setStyleSheet(UNCHANGED);
    }
}

void NewMainWindow::on_pushButton_histogram_clicked()
{

    if (_realtimeScene==NULL || _targetScene==NULL)
    {
        qCDebug(NEWMAINWINDOW()) << NEWMAINWINDOW().categoryName()
                                 << "scene pointer null"
                                 << TRACE_CMH();
        return;
    }

    if (_pWidget==NULL)
    {
        _pWidget = new PaintedWidget(_realtimeScene,ui->widget_histogram);
    }

    _pWidget->setDrawScene(_targetScene);
    _pWidget->show();

}

void NewMainWindow::on_pushButton_measure_length_clicked()
{

    if (_targetScene==NULL)
    {
        return;
    }

    if(_targetScene->getOperator()!=OPER_MEASURE)
    {
        _targetScene->setOperator(OPER_MEASURE);
    }
    stat._bDrawable = true;
    stat._bMeasureable = true;
    stat._bTargetDrawable = false;
    _targetScene->setDrawable(stat._bDrawable);
    _targetScene->setMeasureDrawable(stat._bMeasureable);
    _targetScene->setTargetDrawable(stat._bTargetDrawable);

    ui->pushButton_measure_length->setDisabled(true);
    ui->pushButton_measure_length->setStyleSheet(NOT_WORK);

    ui->pushButton_cancel->setEnabled(true);
    ui->pushButton_cancel->setStyleSheet(SELECTED);

}

void NewMainWindow::on_pushButton_cancel_clicked()
{
    if(_targetScene==NULL)
    {
        return;
    }

    if(_targetScene->getOperator()==OPER_MEASURE)
    {
        _targetScene->setOperator(OPER_DRAW_TARGET);
    }

    if(ui->commandLinkButton_generate_plan->isEnabled())
    {
        stat._bDrawable = true;
    }
    else
    {
        stat._bDrawable = false;
    }

    stat._bMeasureable = false;
    _targetScene->setDrawable(stat._bDrawable);
    _targetScene->setMeasureDrawable(stat._bMeasureable);
    if(!(ui->pushButton_measure_length->isEnabled()))
    {
        ui->pushButton_measure_length->setDisabled(false);
        ui->pushButton_measure_length->setStyleSheet(SELECTED);

        ui->pushButton_cancel->setEnabled(false);
        ui->pushButton_cancel->setStyleSheet(NOT_WORK);
    }

}

void NewMainWindow::on_checkBox_density_high_toggled(bool checked)
{
    if (checked)
    {
        ui->checkBox_density_low->setChecked(false);
        ui->checkBox_density_middle->setChecked(false);

        ui->checkBox_circle_one->setChecked(true);
        ui->checkBox_circle_two->setChecked(true);
        ui->checkBox_circle_three->setChecked(true);
        ui->checkBox_circle_four->setChecked(true);
        ui->checkBox_circle_five->setChecked(true);

        ui->checkBox_circle_one->setEnabled(true);
        ui->checkBox_circle_two->setEnabled(true);
        ui->checkBox_circle_three->setEnabled(true);
        ui->checkBox_circle_four->setEnabled(true);
        ui->checkBox_circle_five->setEnabled(true);
    }
}

void NewMainWindow::on_checkBox_density_middle_toggled(bool checked)
{
    if (checked)
    {
        ui->checkBox_density_high->setChecked(false);
        ui->checkBox_density_low->setChecked(false);

        ui->checkBox_circle_one->setChecked(true);
        ui->checkBox_circle_two->setChecked(true);
        ui->checkBox_circle_three->setChecked(true);
        ui->checkBox_circle_four->setChecked(true);
        ui->checkBox_circle_five->setChecked(false);

        ui->checkBox_circle_one->setEnabled(true);
        ui->checkBox_circle_two->setEnabled(true);
        ui->checkBox_circle_three->setEnabled(true);
        ui->checkBox_circle_four->setEnabled(true);
        ui->checkBox_circle_five->setEnabled(false);
    }
}

void NewMainWindow::on_checkBox_density_low_toggled(bool checked)
{
    if (checked)
    {
        ui->checkBox_density_high->setChecked(false);
        ui->checkBox_density_middle->setChecked(false);

        ui->checkBox_circle_one->setChecked(true);
        ui->checkBox_circle_two->setChecked(true);
        ui->checkBox_circle_three->setChecked(true);
        ui->checkBox_circle_four->setChecked(false);
        ui->checkBox_circle_five->setChecked(false);

        ui->checkBox_circle_one->setEnabled(true);
        ui->checkBox_circle_two->setEnabled(true);
        ui->checkBox_circle_three->setEnabled(true);
        ui->checkBox_circle_four->setEnabled(false);
        ui->checkBox_circle_five->setEnabled(false);
    }
}

// Replan without acquiring images
void NewMainWindow::on_pushButton_replan_clicked()
{
    // 重新开始Plan，sonicationStop
    _isIrradiating = false;
    emit setIrradiating(_isIrradiating);
    setLocationControlEnable(true);

    setTreatmentControlDisable();
    setModifyPlanControlDisable();
    setWholePlanControlEnable();

    // Delete以前Plan的sonication point
    if (!_targetSceneMap.empty())
    {
        QMap<int,DrawScene*>::iterator iter = _targetSceneMap.begin();
        for (; iter!=_targetSceneMap.end(); ++iter)
        {
            iter.value()->removeSpots();
        }
    }

    // 增加Delete的右键菜单选项
    initTargetMenu();

    if(_newScene3d!=NULL&&_newScene3d->isInit)
    {
        _newScene3d->ClearSpot();
    }

    _totalCompleteMap.clear();
    _currentCompleteMap.clear();
//    _isIrradiating = false;
//    emit setIrradiating(_isIrradiating);

    ui->graphicsView_treatment_layer->clearMask();
    disableLayerControl();
    clearLayerScenes();

    // 进度条进度设置为0
    ui->progressBar_cooling_time->setValue(0);
    ui->label_remain_cooling_time_value->setText("0");
//    if (_core!=NULL)
//    {
//        delete _core;
//        _core = NULL;
//    }

    if (_pCurrentModel!=NULL)
    {
        _pCurrentModel->clear();
    }

//    if (_pTotalModel!=NULL)
//    {
//       _pTotalModel->clear();
//    }
    disconnect(ui->tableView_current_plan,SIGNAL(exchangeRows(QMap<QString,QString>,int)),
            _pCurrentModel,SLOT(exchangeRows(QMap<QString,QString>,int)));
    initPlanTable();

    disconnect(_pCurrentModel,SIGNAL(dataChanged(QModelIndex,QModelIndex)),
            this,SLOT(currentPlanChanged(QModelIndex,QModelIndex)));

    m_totalTime.spotsRemain = 0;
    is_plan_completed = true;
    refreshRemainTime();
}

void NewMainWindow::on_pushButton_generate_plan_clicked()
{
    inintBlockPlan();
    int diameter = 50;
    int deep = 50;
    _pBlockPlan->generateBlocks(diameter,deep);
    refreshBlocksTable(_pBlockPlan->getBlocks());
    _currentCompleteBlock.clear();

    _planScene3d->InitScene3D();
    double pos[3] = {0.0,0.0,0.0};
    _planScene3d->LoadTumorStl("../resources/ball.stl",pos,diameter);

    QMap<QString,QVector<double> > SpotList;
    QQueue<Irradiate_Block>& blocks = _pBlockPlan->getBlocks();
    for (int i = 0; i < blocks.size();++i)
    {

        QVector<double> pos0;
        pos0.push_back(tan(blocks[i].leftRight*PI/180.0)*277.0);
        pos0.push_back(tan(blocks[i].frontBack*PI/180.0)*277.0);
        pos0.push_back(0);
        SpotList.insert(blocks[i].id,pos0);
    }

    _planScene3d->Init_IrradiationPoint(pos,SpotList,m_iPatientOrientation);
}

void NewMainWindow::on_pushButton_block_confirm_clicked()
{
    QMessageBox msgBox("Start treatment",
                       "Start treatment for the next region?",
                       QMessageBox::Warning,
                       QMessageBox::Yes | QMessageBox::Default,
                       QMessageBox::No | QMessageBox::Escape,
                       QMessageBox::NoButton);
    msgBox.setWindowFlags(Qt::WindowTitleHint|Qt::CustomizeWindowHint|Qt::WindowStaysOnTopHint);
    msgBox.setGeometry(QRect(1500,800,msgBox.width(),msgBox.height()));
    msgBox.setFont(m_msgBoxFont);
    msgBox.setButtonText(QMessageBox::Yes, "OK");
    msgBox.setButtonText(QMessageBox::No, "Cancel");

    int ret = msgBox.exec();
    if (ret == QMessageBox::No)
    {
        return;
    }

    if (_pBlockPlan==NULL)
    {
        return;
    }

    for (QMap<QString,bool>::iterator iter = _currentCompleteBlock.begin();
         iter != _currentCompleteBlock.end();++iter)
    {
        setBlockTableRowColor(iter.key(),COMPLETE_COLOR);
        emit setBlockColor(iter.key(),COMPLETE_COLOR);
    }

    Irradiate_Block block;
    if (!_pBlockPlan->getNextBlock(_currentCompleteBlock,block))
    {
        return;
    }
    else
    {
        if (_location!=NULL)
        {
            // 坐标系的关系，交换
            if (!_location->setPos(AXIS_L,block.leftRight))  // 左右
            {
                qCDebug(NEWMAINWINDOW()) << NEWMAINWINDOW().categoryName()
                                         << "set Axis left and right error ! "<< block.leftRight
                                         << TRACE_CMH();
            }

            if (!_location->setPos(AXIS_F,-1.0*block.frontBack))  // 前后
            {
                qCDebug(NEWMAINWINDOW()) << NEWMAINWINDOW().categoryName()
                                         << "set Axis front and back error ! "<< block.frontBack
                                         << TRACE_CMH();
            }
            else
            {

                outputTreatmentLog("id ",block.id,"left to right angle is",QString::number(block.leftRight),
                                   "front to back angle is ",QString::number(block.frontBack));

                setBlockTableRowColor(block.id,IRADIATING_COLOR);
                _currentCompleteBlock.insert(block.id,true);
                emit setBlockColor(block.id,IRADIATING_COLOR);
                emit setCurrentBlockId(block.id);

                m_iRotate;
                m_sHeaderAdjust.isAdjust = true;
                // 坐标系交换
                m_sHeaderAdjust.destLR = block.leftRight;
                m_sHeaderAdjust.destFB = -1.0*block.frontBack;

                // 设置Series采集未开始
                _seriesAcquired = false;
                // on_toolButton_acquire_image_clicked();

                --m_totalTime.blocks;
                m_totalTime.spotsRemain=m_totalTime.spots;

                if (!ui->tabWidget_operator->isTabEnabled(3))
                {
                    ui->tabWidget_operator->setTabEnabled(3,true);
                }

                refreshRemainTime();
                on_treatment_time_remain_update();

                // 没有TreatmentPlan了，让Treatment按钮灰掉
                if (_currentCompleteBlock.size()==_planLayerScene->getBlocksSize())
                {
                    ui->pushButton_block_confirm->setEnabled(false);
                    ui->pushButton_block_confirm->setStyleSheet(NOT_WORK);
                }
            }
        }
    }
}

void NewMainWindow::on_pushButton_draw_clicked()
{
    if (_planLayerScene!=NULL)
    {
        _planLayerScene->setDrawable(true);
        // _planLayerScene->setTargetDrawable(true);
        _planLayerScene->setOper(OPER_DRAW);
        _planLayerScene->setRangeVisible(false);
        _planLayerScene->drawBorder(QPointF(m_sHeaderAdjust.destLR,
                                    m_sHeaderAdjust.destFB));
        _planLayerScene->clearMarginAndBlocks();
    }

    ui->pushButton_draw_confirm->setEnabled(true);
    ui->pushButton_draw_confirm->setStyleSheet(SELECTED);

    if (ui->pushButton_set_order->isEnabled())
    {
        ui->pushButton_set_order->setEnabled(false);
        ui->pushButton_set_order->setStyleSheet(NOT_WORK);

        ui->pushButton_confirm_order->setEnabled(false);
        ui->pushButton_confirm_order->setStyleSheet(NOT_WORK);
    }

    if (ui->pushButton_block_confirm->isEnabled())
    {
        ui->pushButton_block_confirm->setEnabled(false);
        ui->pushButton_block_confirm->setStyleSheet(NOT_WORK);
    }
}

void NewMainWindow::on_pushButton_drag_clicked()
{
    if (_planLayerScene!=NULL)
    {
        _planLayerScene->setDrawable(false);
        _planLayerScene->setTargetDrawable(false);

        _planLayerScene->drawRange(QPointF(HALF_CLIP_WIDE,HALF_CLIP_WIDE));
        _planLayerScene->drawBorder(QPointF(m_sHeaderAdjust.destLR,
                                    m_sHeaderAdjust.destFB));
        _planLayerScene->setOper(OPER_DRAG);
        _planLayerScene->setMarginVisible(true);
    }

    ui->pushButton_drag_confirm->setEnabled(true);
    ui->pushButton_drag_confirm->setStyleSheet(SELECTED);
}

void NewMainWindow::on_pushButton_draw_confirm_clicked()
{
    double radius = 277.0;
    if (_planLayerScene!=NULL)
    {
        inintBlockPlan();

        _planLayerScene->createDrawBlocks();

        if(_planScene3d!=NULL&&_planScene3d->isInit)
        {
            _planScene3d->ClearSpot();
            _planScene3d->ClearTumor();
        }

        if (_pBlockModel!=NULL)
        {
            _pBlockModel->clear();
        }

        initBlocksTable();
        _currentCompleteBlock.clear();

        refreshBlocksTable(_planLayerScene->getBlocks());

        if (_pBlockPlan!=NULL)
        {
            _pBlockPlan->setBlocks(_planLayerScene->getBlocks());
        }

        _planScene3d->InitScene3D();
        double pos[3] = {0.0,0.0,0.0};
        _planScene3d->LoadTumorStl("../resources/ball.stl",pos,8);

        QMap<QString,QVector<double> > SpotList;
        QQueue<Irradiate_Block>& blocks = _planLayerScene->getBlocks();
        for (int i = 0; i < blocks.size();++i)
        {

            QVector<double> pos0;
            pos0.push_back(tan(blocks[i].frontBack*PI/180.0)*radius);
            pos0.push_back(-1.0*tan(blocks[i].leftRight*PI/180.0)*radius);
            pos0.push_back(0);
            SpotList.insert(blocks[i].id,pos0);
        }

        _planScene3d->Init_IrradiationPoint(pos,SpotList,m_iPatientOrientation);

        m_totalTime.blocks = blocks.size();
        m_totalTime.spots = 37;
        m_totalTime.onTime = 400;
        m_totalTime.offTime = 100;
        m_totalTime.pulseNumber = 6;
        m_totalTime.coolingTime = 25;
        m_totalTime.delayTime = 13;
        refreshRemainTime();
        on_treatment_time_remain_update();
    }

    ui->pushButton_set_order->setEnabled(true);
    ui->pushButton_set_order->setStyleSheet(SELECTED);

    if (!ui->pushButton_block_confirm->isEnabled())
    {
        ui->pushButton_block_confirm->setEnabled(true);
        ui->pushButton_block_confirm->setStyleSheet(SELECTED);
    }

    if (!ui->pushButton_block_confirm->isEnabled())
    {
        ui->pushButton_block_confirm->setEnabled(true);
        ui->pushButton_block_confirm->setStyleSheet(SELECTED);
    }
}

void NewMainWindow::on_pushButton_drag_confirm_clicked()
{

    QPoint pos;
    double angleLR;
    double angleFB;
    if (_planLayerScene!=NULL)
    {
        pos = _planLayerScene->getRangePos();
        angleLR = qAtan((double)(pos.x()-HALF_CLIP_WIDE)/(277.0*RESOLUTION))*180.0/PI;
        angleFB = -1*qAtan((double)(pos.y()-HALF_CLIP_WIDE)/(277.0*RESOLUTION))*180.0/PI;
        if (angleLR>15.0 || angleLR < -15.0 || angleFB>15.0 || angleFB<-15.0)
        {
            qCDebug(NEWMAINWINDOW()) << NEWMAINWINDOW().categoryName()
                                     << "range angle error ! "<< angleLR << angleFB
                                     << TRACE_CMH();
            return;
        }

        if (_location!=NULL)
        {
            // 坐标系的关系，交换
            if (!_location->setPos(AXIS_L,angleLR))  // 左右
            {
                qCDebug(NEWMAINWINDOW()) << NEWMAINWINDOW().categoryName()
                                         << "set Axis left and right error ! "<< angleLR
                                         << TRACE_CMH();
            }

            if (!_location->setPos(AXIS_F,angleFB))  // 前后
            {
                qCDebug(NEWMAINWINDOW()) << NEWMAINWINDOW().categoryName()
                                         << "set Axis front and back error ! "<< angleFB
                                         << TRACE_CMH();
            }
            else
            {
                m_sHeaderAdjust.isAdjust = true;
                // 坐标系交换
                m_sHeaderAdjust.destLR = angleLR;
                m_sHeaderAdjust.destFB = angleFB;

                // 设置Series采集未开始
                _seriesAcquired = false;
                emit setCurrentBlockId(" ");    //设置当前block块id为空

                if (!ui->tabWidget_operator->isTabEnabled(3))
                {
                    ui->tabWidget_operator->setTabEnabled(3,true);
                }
            }
        }
    }
}



void NewMainWindow::generateLayer()
{
    if (_pHifu3d==NULL)
    {
        _pHifu3d = new Hifu3d;
    }
    _pHifu3d->LoadImages(_sceneMap,X_OFFSET,FOCAL_PLANE_140MM+m_f3dDelta*RESOLUTION,CLIP_WIDE,CLIP_HIGH,ANGLE_COUNT);

    QVector<QRgb> colorTbable;
    unsigned int rgb = 0;
    for (int i = 0; i <256;++i)
    {
        colorTbable.append(rgb);
        rgb+=0x00010101;
    }
    QImage middleImage(_pHifu3d->GetImage(HalfClipHigh),CLIP_WIDE,CLIP_HIGH,QImage::Format_Indexed8);
    middleImage.setColorTable(colorTbable);
    QImage middleMirror = middleImage.mirrored(true,true);
    middleMirror.save("../resources/slice.bmp");

    _planLayerScene->setBackgroundPixmap(QPixmap::fromImage(QImage("../resources/slice.bmp")));

    ui->graphicsView_layer_plan->setScene(_planLayerScene);

    disconnect(this,SIGNAL(setLayer()),this,SLOT(generateLayer()));

    ui->pushButton_draw->setEnabled(true);
    ui->pushButton_draw->setStyleSheet(SELECTED);

    ui->pushButton_drag->setEnabled(true);
    ui->pushButton_drag->setStyleSheet(SELECTED);
}

void NewMainWindow::on_pushButton_acquire_clicked()
{
    _seriesAcquired = true;
    // 采集图像时设置Measurement Analysis模块不可用
    ui->tabWidget_operator->setTabEnabled(0,false);
    ui->tabWidget_operator->setTabEnabled(3,false);

    // 采集期间，直方图分析不可用
    ui->pushButton_histogram->setEnabled(false);
    ui->pushButton_histogram->setStyleSheet(NOT_WORK);

    if (ui->pushButton_draw->isEnabled())
    {
        ui->pushButton_draw->setEnabled(false);
        ui->pushButton_draw->setStyleSheet(NOT_WORK);
    }

    if (ui->pushButton_block_confirm->isEnabled())
    {
        ui->pushButton_block_confirm->setEnabled(false);
        ui->pushButton_block_confirm->setStyleSheet(NOT_WORK);
    }

    clearBigMyoma();

    if (ui->pushButton_drag->isEnabled())
    {
        ui->pushButton_drag->setEnabled(false);
        ui->pushButton_drag->setStyleSheet(NOT_WORK);
    }

    if (ui->pushButton_draw_confirm->isEnabled())
    {
        ui->pushButton_draw_confirm->setEnabled(false);
        ui->pushButton_draw_confirm->setStyleSheet(NOT_WORK);
    }

    if (ui->pushButton_drag_confirm->isEnabled())
    {
        ui->pushButton_drag_confirm->setEnabled(false);
        ui->pushButton_drag_confirm->setStyleSheet(NOT_WORK);
    }

    initRealtimeMenu();    // 设置Live Image右键菜单可用

    qDebug()<<"bf resetScenes";
    resetScenes();
    qDebug()<<"af resetScenes";

    if (_realtimeScene!=NULL)
    {
        _realtimeScene->resetFrameIndex();    // 从-90度开始扫描
    }

    if (!_location->setPos(AXIS_A, _realtimeScene->angle()))
    {
        qCDebug(NEWMAINWINDOW()) << NEWMAINWINDOW().categoryName()
                                 << "location set pos -90 error !"
                                 << TRACE_CMH();
    }

    // 设置target图像Edit按钮可用
    // setSingleFrameEditEnable();
    // 设置target右键菜单可用
    initTargetMenu();

    if (_planLayerScene!=NULL)
    {
        _planLayerScene->clearAll();
        _planLayerScene->setDrawable(false);
        _planLayerScene->setTargetDrawable(false);
    }

    // 采集图像结束，设置水平滑动条正常，图像Select功能正常，图像张数显示正常
    if(!ui->horizontalScrollBar_images->isEnabled())
    {
        ui->horizontalScrollBar_images->setEnabled(true);
        ui->horizontalScrollBar_images->setStyleSheet(SELECTED);
        ui->pushButton_front->setEnabled(true);
        ui->pushButton_front->setStyleSheet(SELECTED_NO_ROUND);
        ui->pushButton_back->setEnabled(true);
        ui->pushButton_back->setStyleSheet(SELECTED_NO_ROUND);
    }

    m_bIsPlanLayerScene = true;
    connect(this,SIGNAL(setLayer()),this,SLOT(generateLayer()));
}

void NewMainWindow::on_pushButton_set_order_clicked()
{
    if (ui->pushButton_draw->isEnabled())
    {
        ui->pushButton_draw_confirm->setEnabled(false);
        ui->pushButton_draw_confirm->setStyleSheet(NOT_WORK);
    }

    if (ui->pushButton_block_confirm->isEnabled())
    {
        ui->pushButton_block_confirm->setEnabled(false);
        ui->pushButton_block_confirm->setStyleSheet(NOT_WORK);
    }

    if (_planLayerScene!=NULL)
    {
        _planLayerScene->setDrawable(false);
        _planLayerScene->clearOrder(_currentCompleteBlock);
        size_t index = _currentCompleteBlock.size()+1;
        _planLayerScene->setOper(OPER_SORT,index);
    }
}

void NewMainWindow::setConfirmSetOrderEnable()
{
    if (!ui->pushButton_confirm_order->isEnabled())
    {
        ui->pushButton_confirm_order->setEnabled(true);
        ui->pushButton_confirm_order->setStyleSheet(SELECTED);
    }
}

void NewMainWindow::on_pushButton_confirm_order_clicked()
{
    if (ui->pushButton_confirm_order->isEnabled())
    {
        ui->pushButton_confirm_order->setEnabled(false);
        ui->pushButton_confirm_order->setStyleSheet(NOT_WORK);
    }

    if (!ui->pushButton_block_confirm->isEnabled())
    {
        ui->pushButton_block_confirm->setEnabled(true);
        ui->pushButton_block_confirm->setStyleSheet(SELECTED);
    }

    if (_planLayerScene!=NULL)
    {
        _planLayerScene->refreshBlocks();

        refreshBlocksTable(_planLayerScene->getBlocks());

        if (_pBlockPlan!=NULL)
        {
            _pBlockPlan->setBlocks(_planLayerScene->getBlocks());
        }

        for (QMap<QString,bool>::iterator iter = _currentCompleteBlock.begin();
             iter != _currentCompleteBlock.end();++iter)
        {
            setBlockTableRowColor(iter.key(),COMPLETE_COLOR);
            emit setBlockColor(iter.key(),COMPLETE_COLOR);
        }

        if (!ui->pushButton_block_confirm->isEnabled())
        {
            ui->pushButton_block_confirm->setEnabled(true);
            ui->pushButton_block_confirm->setStyleSheet(SELECTED);
        }
    }

}

void NewMainWindow::on_horizontalScrollBar_contrast_valueChanged(int value)
{
    if (_realtimeScene!=NULL)
    {
        _realtimeScene->setContrast(value);
    }
    //emit setContrast(f);
}

//void NewMainWindow::on_pushButton_controlpump_clicked()
//{
//    if(!m_bPump1Open)
//    {
//        ui->pushButton_controlpump->setText("Stop Pump");
//        emit pumpctl();
//    }
//    else
//    {
//        ui->pushButton_controlpump->setText("Start Pump");
//        emit pumpctl();
//    }
//}

void NewMainWindow::updatespeed_in(int value)
{
    ui->label_speed_value_in->setText(QString::number(value));
}

void NewMainWindow::updatespeed_out(int value)
{
    ui->label_speed_value_out->setText(QString::number(value));
}

void NewMainWindow::updatelevel(int value)
{
    if(value<=33)
        ui->label_levle_value->setText("Low");
    else if(value>33&&value<66)
        ui->label_levle_value->setText("Middle");
    else if(value>66)
        ui->label_levle_value->setText("High");
}

void NewMainWindow::updatemeter(float value)
{
    ui->label_meter_value->setText(QString::number(value));
}

void NewMainWindow::on_toolButton_zoomout_image_clicked()
{
    realTimeZoomOut();
}

void NewMainWindow::on_toolButton_zoomin_image_clicked()
{
    realTimeZoomIn();
}

void NewMainWindow::on_toolButton_showpath_clicked()
{
    realTimeShowPath();

}

void NewMainWindow::on_toolButton_hidepath_clicked()
{
    realTimeHidePath();
}

void NewMainWindow::on_pushButton_pump1control_clicked()
{
    if(!m_bPump1Open)
    {
        if(ui->label_levle_value->text()=="Low")
        {
            QMessageBox msgBox("Notice",
                               "Water tank level is too low. Water bladder filling is paused.",
                               QMessageBox::Information,
                               QMessageBox::Ok | QMessageBox::Escape,
                               QMessageBox::NoButton,
                               QMessageBox::NoButton);
            msgBox.setWindowFlags(Qt::WindowTitleHint|Qt::CustomizeWindowHint|Qt::WindowStaysOnTopHint);
            msgBox.setGeometry(QRect(1400,400,msgBox.width(),msgBox.height()));
            msgBox.setFont(m_msgBoxFont);
            msgBox.setButtonText(QMessageBox::Ok, "OK");
            msgBox.exec();
            return;
        }
        ui->pushButton_pump1control->setText("Stop Pump");
        ui->pushButton_pump1control->setEnabled(false);
        ui->pushButton_pump1control->setStyleSheet("background-color: rgb(55, 55, 55)");
        ui->pushButton_pump1speedup->setEnabled(false);
        ui->pushButton_pump1speedup->setStyleSheet("background-color: rgb(55, 55, 55)");
        ui->pushButton_pump1speeddown->setEnabled(false);
        ui->pushButton_pump1speeddown->setStyleSheet("background-color: rgb(55, 55, 55)");
        emit pump1ctl();

        qDebug(NEWMAINWINDOW())<<NEWMAINWINDOW().categoryName()
                               <<"Open pump 1 water in !"
                               <<TRACE_CMH();
    }
    else
    {
        ui->pushButton_pump1control->setText("Start Pump");
        ui->pushButton_pump1control->setEnabled(false);
        ui->pushButton_pump1control->setStyleSheet("background-color: rgb(55, 55, 55)");
        ui->pushButton_pump1speedup->setEnabled(false);
        ui->pushButton_pump1speedup->setStyleSheet("background-color: rgb(55, 55, 55)");
        ui->pushButton_pump1speeddown->setEnabled(false);
        ui->pushButton_pump1speeddown->setStyleSheet("background-color: rgb(55, 55, 55)");
        emit pump1ctl();

        qDebug(NEWMAINWINDOW())<<NEWMAINWINDOW().categoryName()
                               <<"Close pump 1 water in !"
                               <<TRACE_CMH();
    }
}

void NewMainWindow::on_pushButton_pump2control_clicked()
{
    if(!m_bPump2Open)
    {
        ui->pushButton_pump2control->setText("Stop Pump");
        ui->pushButton_pump2control->setEnabled(false);
        ui->pushButton_pump2control->setStyleSheet("background-color: rgb(55, 55, 55)");
        ui->pushButton_pump2speedup->setEnabled(false);
        ui->pushButton_pump2speedup->setStyleSheet("background-color: rgb(55, 55, 55)");
        ui->pushButton_pump2speeddown->setEnabled(false);
        ui->pushButton_pump2speeddown->setStyleSheet("background-color: rgb(55, 55, 55)");
        emit pump2ctl();

        qDebug(NEWMAINWINDOW())<<NEWMAINWINDOW().categoryName()
                               <<"Open pump 2 water out !"
                               <<TRACE_CMH();
    }
    else
    {
        ui->pushButton_pump2control->setText("Start Pump");
        ui->pushButton_pump2control->setEnabled(false);
        ui->pushButton_pump2control->setStyleSheet("background-color: rgb(55, 55, 55)");
        ui->pushButton_pump2speedup->setEnabled(false);
        ui->pushButton_pump2speedup->setStyleSheet("background-color: rgb(55, 55, 55)");
        ui->pushButton_pump2speeddown->setEnabled(false);
        ui->pushButton_pump2speeddown->setStyleSheet("background-color: rgb(55, 55, 55)");
        emit pump2ctl();

        qDebug(NEWMAINWINDOW())<<NEWMAINWINDOW().categoryName()
                               <<"Close pump 2 water out !"
                               <<TRACE_CMH();
    }
}

void NewMainWindow::on_pushButton_pump1speeddown_clicked()
{
    emit speed1down();
}

void NewMainWindow::on_pushButton_pump1speedup_clicked()
{
    emit speed1up();
}

void NewMainWindow::on_pushButton_pump2speeddown_clicked()
{
    emit speed2down();
}

void NewMainWindow::on_pushButton_pump2speedup_clicked()
{
    emit speed2up();
}

void NewMainWindow::pump1open()
{
    m_bPump1Open=true;
    ui->pushButton_pump1control->setText("Stop Pump");
    ui->pushButton_pump1control->setEnabled(true);
    ui->pushButton_pump1control->setStyleSheet("background-color: rgb(255, 255, 255)");
    ui->pushButton_pump1speedup->setEnabled(true);
    ui->pushButton_pump1speedup->setStyleSheet("background-color: rgb(255, 255, 255)");
    ui->pushButton_pump1speeddown->setEnabled(true);
    ui->pushButton_pump1speeddown->setStyleSheet("background-color: rgb(255, 255, 255)");
}

void NewMainWindow::pump1close()
{
    m_bPump1Open=false;
    ui->pushButton_pump1control->setText("Start Pump");
    ui->pushButton_pump1control->setEnabled(true);
    ui->pushButton_pump1control->setStyleSheet("background-color: rgb(255, 255, 255)");
    ui->pushButton_pump1speedup->setEnabled(false);
    ui->pushButton_pump1speedup->setStyleSheet("background-color: rgb(55, 55, 55)");
    ui->pushButton_pump1speeddown->setEnabled(false);
    ui->pushButton_pump1speeddown->setStyleSheet("background-color: rgb(55, 55, 55)");
}

void NewMainWindow::pump1lock()
{
    ui->pushButton_pump1control->setEnabled(false);
    ui->pushButton_pump1control->setStyleSheet("background-color: rgb(55, 55, 55)");
    ui->pushButton_pump1speedup->setEnabled(false);
    ui->pushButton_pump1speedup->setStyleSheet("background-color: rgb(55, 55, 55)");
    ui->pushButton_pump1speeddown->setEnabled(false);
    ui->pushButton_pump1speeddown->setStyleSheet("background-color: rgb(55, 55, 55)");
}

void NewMainWindow::pump2open()
{
    m_bPump2Open=true;
    ui->pushButton_pump2control->setText("Stop Pump");
    ui->pushButton_pump2control->setEnabled(true);
    ui->pushButton_pump2control->setStyleSheet("background-color: rgb(255, 255, 255)");
    ui->pushButton_pump2speedup->setEnabled(true);
    ui->pushButton_pump2speedup->setStyleSheet("background-color: rgb(255, 255, 255)");
    ui->pushButton_pump2speeddown->setEnabled(true);
    ui->pushButton_pump2speeddown->setStyleSheet("background-color: rgb(255, 255, 255)");
}

void NewMainWindow::pump2close()
{
    m_bPump2Open=false;
    ui->pushButton_pump2control->setText("Start Pump");
    ui->pushButton_pump2control->setEnabled(true);
    ui->pushButton_pump2control->setStyleSheet("background-color: rgb(255, 255, 255)");
    ui->pushButton_pump2speedup->setEnabled(false);
    ui->pushButton_pump2speedup->setStyleSheet("background-color: rgb(55, 55, 55)");
    ui->pushButton_pump2speeddown->setEnabled(false);
    ui->pushButton_pump2speeddown->setStyleSheet("background-color: rgb(55, 55, 55)");
}

void NewMainWindow::pump2lock()
{
    ui->pushButton_pump2control->setEnabled(false);
    ui->pushButton_pump2control->setStyleSheet("background-color: rgb(55, 55, 55)");
    ui->pushButton_pump2speedup->setEnabled(false);
    ui->pushButton_pump2speedup->setStyleSheet("background-color: rgb(55, 55, 55)");
    ui->pushButton_pump2speeddown->setEnabled(false);
    ui->pushButton_pump2speeddown->setStyleSheet("background-color: rgb(55, 55, 55)");
}

void NewMainWindow::controllock()
{
    Auto_WaterLock = true;
    ui->pushButton_pump1control->setEnabled(false);
    ui->pushButton_pump1control->setStyleSheet("background-color: rgb(55, 55, 55)");
    ui->pushButton_pump1speedup->setEnabled(false);
    ui->pushButton_pump1speedup->setStyleSheet("background-color: rgb(55, 55, 55)");
    ui->pushButton_pump1speeddown->setEnabled(false);
    ui->pushButton_pump1speeddown->setStyleSheet("background-color: rgb(55, 55, 55)");
    ui->pushButton_pump2control->setEnabled(false);
    ui->pushButton_pump2control->setStyleSheet("background-color: rgb(55, 55, 55)");
    ui->pushButton_pump2speedup->setEnabled(false);
    ui->pushButton_pump2speedup->setStyleSheet("background-color: rgb(55, 55, 55)");
    ui->pushButton_pump2speeddown->setEnabled(false);
    ui->pushButton_pump2speeddown->setStyleSheet("background-color: rgb(55, 55, 55)");
}

void NewMainWindow::controlunlock()
{
    Auto_WaterLock = false;
    ui->pushButton_pump1control->setEnabled(true);
    ui->pushButton_pump1control->setStyleSheet("background-color: rgb(255, 255, 255)");
    if(m_bPump1Open)
    {
        ui->pushButton_pump1speedup->setEnabled(true);
        ui->pushButton_pump1speedup->setStyleSheet("background-color: rgb(255, 255, 255)");
        ui->pushButton_pump1speeddown->setEnabled(true);
        ui->pushButton_pump1speeddown->setStyleSheet("background-color: rgb(255, 255, 255)");
    }
    ui->pushButton_pump2control->setEnabled(true);
    ui->pushButton_pump2control->setStyleSheet("background-color: rgb(255, 255, 255)");
    if(m_bPump2Open)
    {
        ui->pushButton_pump2speedup->setEnabled(true);
        ui->pushButton_pump2speedup->setStyleSheet("background-color: rgb(255, 255, 255)");
        ui->pushButton_pump2speeddown->setEnabled(true);
        ui->pushButton_pump2speeddown->setStyleSheet("background-color: rgb(255, 255, 255)");
    }
}

void NewMainWindow::stopsonication()
{
    emit setIrradiating(false);
    emit setIsPause(false);
    disableStopButton();
}

void NewMainWindow::netconnected()
{
    ui->label_status->setText("Status: network is connected");
    ui->label_network_icon->setPixmap(QPixmap::fromImage(QImage(":/icons/img/bottomicon/Network-Interface.png")));
    ui->label_network_text->setStyleSheet(WORK);
    networkready=true;
    if(powerampready&&networkready&&waterready&&locationready&&appendready)
        ui->label_alarm_light->setPixmap(QPixmap(QString(ICON_PIC_PATH).append("green.png")));
}

void NewMainWindow::getSnapshot()
{
    HANDLE hProcess = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
    if(!hProcess){

        qDebug() << "CreateToolhelp32Snapshot failed!";
        return;
    }
    process_vec.clear();
    PROCESSENTRY32 info;
    info.dwSize = sizeof(PROCESSENTRY32);
    if(!Process32First(hProcess, &info)){

        qDebug() << "Process32First null";
        return;
    }

    bool haveNext = true;
    while(haveNext){

        haveNext = Process32Next(hProcess, &info);
        if(haveNext){

            HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, TRUE, info.th32ProcessID);

            insertInfo(QString::number(info.th32ProcessID), QString::number(info.cntUsage),
                       QString::number(info.th32ParentProcessID), QString::number(info.pcPriClassBase),
                       QString::number((DWORD)hProcess), QString::fromWCharArray(info.szExeFile));

            //qDebug() << QString::fromWCharArray(info.szExeFile);
            process_vec.push_back(QString::fromWCharArray(info.szExeFile));
            qDebug() << info.th32ProcessID << "  " << info.pcPriClassBase << "  " << QString::fromWCharArray(info.szExeFile) << "  " << hProcess;
        }
    }
}

void NewMainWindow::insertInfo(const QString &pID, const QString &thrNum,
                               const QString &parePID, const QString &processPr,
                               const QString &handle, const QString &processName)
{
    ProcessInfo info;
    info.processID = pID;
    info.threadNum = thrNum;
    info.parentProcessID = parePID;
    info.processPri = processPr;
    info.processHandle = handle;
    info.processName = processName;

    m_vec.append(info);
}

void NewMainWindow::Delay_MSec(unsigned int msec)
{
    QTime _Timer = QTime::currentTime().addMSecs(msec);

    while(QTime::currentTime() < _Timer)
        QCoreApplication::processEvents(QEventLoop::AllEvents,100);
}

void NewMainWindow::updateLogFile(QString name, QByteArray &log)
{
    QFile file(name);
    if (file.open(QIODevice::WriteOnly | QIODevice::Append)) {
        QTextStream stream(&file);
        stream << log << endl;
        file.close();
    }
}

void NewMainWindow::updateLog(const QString &msg)
{
    QDateTime time = QDateTime::currentDateTime();
    QString path = "D:/TreationLOGS/" + PID +"/" +time.toString("yyyy-MM-dd");
    QDir dir(path);
    if (!dir.exists())
    {
        dir.mkpath(path);
    }
    QString logFileName = path + "/" + TID + ".log";
    QByteArray log = time.toString("yyyy-MM-dd hh:mm:ss.zzz").toLocal8Bit();
    log += QString(" >> %1\r\n").arg(msg);

    updateLogFile(logFileName, log);
}

void NewMainWindow::LocationAlarm()
{
    locationerror++;
    if(locationerror>10 && locationready){
        ui->label_position_icon->setPixmap(QPixmap::fromImage(QImage(":/icons/img/bottomicon/Motion-System-abnormal.png")));
        ui->label_position_text->setStyleSheet(NOT_WORK);
        ui->label_alarm_light->setPixmap(QPixmap(QString(ICON_PIC_PATH).append("red.png")));
        _append->setRedOn();
        qCDebug(NEWMAINWINDOW())<<NEWMAINWINDOW().categoryName()
                             <<"Error location alarm !"
                             << TRACE_CMH();

        locationready=false;
        locationerror = 0;
    }
}

void NewMainWindow::LocationSetPosSuccess()
{
    locationerror = 0;
}

void NewMainWindow::LocationHeadControl(int AID, int direction)
{
    if(_isIrradiating)
    {
        return;
    }
    switch(AID)
    {
        case 0:
            if(direction==0)
            {
                _location->ReceiveHeadControl(0,0);
                _location->getPos(AXIS_Z);
            }
            else if(direction==-1)
            {
                _location->setPos(AXIS_Z, 400);
            }
            else if(direction==1)
            {
                _location->setPos(AXIS_Z, 0);
            }
            break;

        case 1:
            if(direction==0)
            {
                _location->ReceiveHeadControl(1,0);
            }
            else if(direction==-1)
            {
                _location->setPos(AXIS_L, -16);
            }
            else if(direction==1)
            {
                _location->setPos(AXIS_L, 16);
            }
            break;

        case 2:
            if(direction==0)
            {
                _location->ReceiveHeadControl(2,0);
            }
            else if(direction==-1)
            {
                _location->setPos(AXIS_F, -16);
            }
            else if(direction==1)
            {
                _location->setPos(AXIS_F, 16);
            }
            break;
    }
}

void NewMainWindow::appendConnectFailed()
{
    appendready=false;
    if(!appendalarm)
    {
        ui->label_alarm_light->setPixmap(QPixmap(QString(ICON_PIC_PATH).append("red.png")));
        appendalarm=true;
        QMessageBox msgBox("Alert Light Box Communication Error","Communication with the alert light box was lost. Check the cable connection and restart the software.",
                           QMessageBox::Warning,
                           QMessageBox::Ok | QMessageBox::Escape,
                           QMessageBox::NoButton,
                           QMessageBox::NoButton);
        msgBox.setWindowFlags(Qt::WindowTitleHint|Qt::CustomizeWindowHint|Qt::WindowStaysOnTopHint);
        msgBox.setGeometry(QRect(1400,400,msgBox.width(),msgBox.height()));
        msgBox.setFont(m_msgBoxFont);
        msgBox.setButtonText(QMessageBox::Ok, "OK");
        msgBox.exec();
    }
}

//void NewMainWindow::deepthmeasure()
//{

//    int deepth = _hifuimagealgo->deepthmeasure(_realtimeScene->getPixmap().toImage());
////    _hifuimagealgo.
//    qDebug()<<"是否到达！"<<deepth;
//    if(deepth==-1)
//    {
//        ui->label_deepthmeasure->setText("Depth Unknown");
//    }
//    ui->label_deepthmeasure->setText("深度:"+QString::number(deepth));


//    if(_realtimeScene!=NULL)
//    {
//        QImage image = _realtimeScene->getPixmap().toImage();
//        QImage testimage = image.copy(200,100,300,430);
//        QQueue<QPoint> point_num;
//        QColor imagergb;
//        for(int i = 0;i<testimage.height();i++)
//        {
//            for(int j = 0;j<testimage.width();j++)
//            {
//                imagergb = testimage.pixel(j,i);
//                if(imagergb.red()!=imagergb.blue()||imagergb.red()!=imagergb.green())
//                {
//                    point_num<<QPoint(j,i);
//                }
//            }
//        }
//        qDebug()<<"大小："<<point_num.size();
//        int num_point = 0;
//        for(int k = 0;k<point_num.size();k++)
//        {
//            if(point_num[point_num.size()-k-1].y()<point_num[point_num.size()-1].y())
//            {
//                num_point = k;
// //                qDebug()<<"哪个点："<<num_point;
//                break;
//            }
//        }
//        qDebug()<<"哪个点："<<num_point;
//        QPoint center_point;
//        if(point_num.isEmpty())
//        {
//            QMessageBox::warning(0,"Notice","识别失败");
//        }
//        else
//        {
//            if(num_point%2)
//            {
//                center_point = (point_num[point_num.size()-1-num_point/2]);
//            }
//            else
//            {
//                center_point = (point_num[point_num.size()-1-(num_point+1)/2]);
//            }
//        }
//        Mat test_image(testimage.height(),testimage.width(),CV_8UC4,testimage.bits(),testimage.bytesPerLine());
//        Mat gray_image;
//        cvtColor(test_image,gray_image,COLOR_RGB2GRAY);
//        Mat erode_mat;
// //        cv::Mat otsu_mat1;

//        Mat otsu_mat;
//        Mat erode_kernel = getStructuringElement(MORPH_ERODE,Size(5,5));
//        threshold(gray_image,otsu_mat,0,255,THRESH_OTSU);
// //        cv::threshold(otsu_mat1,otsu_mat,0,255,THRESH_OTSU);
//        erode(otsu_mat,erode_mat,erode_kernel);
//        QImage erode_image((const unsigned char *)(erode_mat.data),erode_mat.cols,erode_mat.rows,erode_mat.step,QImage::Format_Indexed8);
//        for(int i=0;i<erode_image.height();i++)
//        {
//            QColor erode_pix = erode_image.pixel(center_point.rx(),i);
//    //        double rgb = (erode_pixel.red()+erode_pixel.blue()+erode_pixel.green())/3;
//            double rgb = erode_pix.value();
//            double r = erode_pix.red();
//            double g = erode_pix.green();
//            double b = erode_pix.blue();
//    //        qDebug()<<"rgb:"<<r<<" "<<g<<" "<<b<<" "<<rgb<<""<<center_point.rx()<<" "<<center_point.ry();
//    //        qDebug()<<"dkda!"<<erode_pix[i];
//            if(rgb>0)
//            {
//                double deep = (center_point.y()-i)/6;
//                ui->label_deepthmeasure->setText("深度:"+QString::number(deep));
//                qDebug()<<"深度:"+QString::number(deep);
//                break;
//            }
//        }

//    }
//    else
//    {
//        ui->label_deepthmeasure->setText("深度无法测量！");
//        return;
//    }

//}

void NewMainWindow::startdeeptimer()
{
//    QTimer *deeptimer = new QTimer(this);
//    connect(deeptimer,SIGNAL(timeout()),this,SLOT(deepthmeasure()));
//    deeptimer->start(1000);
    ui->pushButton_return->setEnabled(true);
    ui->pushButton_return->setStyleSheet(SELECTED);
}

void NewMainWindow::setLocationTips()
{
    //设置Motion Systemtips
//    ui->horizontalSlider_left_right->setStyleSheet("QToolTip{background: white; color:black;}");
    ui->horizontalSlider_left_right->setToolTip(QString::number(ui->horizontalSlider_left_right->value())+"°");
//    ui->horizontalSlider_forward_backward->setStyleSheet("QToolTip{background: white; color:black;}");
    ui->horizontalSlider_forward_backward->setToolTip(QString::number(ui->horizontalSlider_forward_backward->value())+"°");
//    ui->horizontalSlider_up_down->setStyleSheet("QToolTip{background: white; color:black;}");
    ui->horizontalSlider_up_down->setToolTip(QString::number(ui->horizontalSlider_up_down->value()));
}

void NewMainWindow::setdupump(dupumpcontrol *_dupump)
{
    _dupumpControl = _dupump;
}

void NewMainWindow::on_pushButton_headL_pressed()
{
//    _location->ReceiveHeadControl(1,-1);
    if(!_seriesAcquired && LocationEN)
    {
        LocationEN = false;
        ReleaseFlag = false;
        _location->setPos(AXIS_L, -16);
    }
}


void NewMainWindow::on_pushButton_headL_released()
{
    if(!_seriesAcquired && !LocationEN && !ReleaseFlag)
    {
        ReleaseFlag = true;
        LocationCD->start();
        _location->ReceiveHeadControl(1,0);
    }
}


void NewMainWindow::on_pushButton_headR_pressed()
{
//    _location->ReceiveHeadControl(1,1);
    if(!_seriesAcquired && LocationEN)
    {
        LocationEN = false;
        ReleaseFlag = false;
        _location->setPos(AXIS_L, 16);
    }
}


void NewMainWindow::on_pushButton_headR_released()
{
    if(!_seriesAcquired && !LocationEN && !ReleaseFlag)
    {
        ReleaseFlag = true;
        LocationCD->start();
        _location->ReceiveHeadControl(1,0);
    }
}


void NewMainWindow::on_pushButton_headF_pressed()
{
//    _location->ReceiveHeadControl(2,-1);
    if(!_seriesAcquired && LocationEN)
    {
        LocationEN = false;
        ReleaseFlag = false;
        _location->setPos(AXIS_F, -16);
    }
}


void NewMainWindow::on_pushButton_headF_released()
{
    if(!_seriesAcquired && !LocationEN && !ReleaseFlag)
    {
        ReleaseFlag = true;
        LocationCD->start();
        _location->ReceiveHeadControl(2,0);
    }
}


void NewMainWindow::on_pushButton_headB_pressed()
{
//    _location->ReceiveHeadControl(2,1);
    if(!_seriesAcquired && LocationEN)
    {
        LocationEN = false;
        ReleaseFlag = false;
        _location->setPos(AXIS_F, 16);
    }
}


void NewMainWindow::on_pushButton_headB_released()
{
    if(!_seriesAcquired && !LocationEN && !ReleaseFlag)
    {
        ReleaseFlag = true;
        LocationCD->start();
        _location->ReceiveHeadControl(2,0);
    }
}


void NewMainWindow::on_pushButton_headD_pressed()
{
//    _location->ReceiveHeadControl(0,-1);
    if(!_seriesAcquired && LocationEN)
    {
        LocationEN = false;
        ReleaseFlag = false;
        _location->setPos(AXIS_Z, 400);
    }
}


void NewMainWindow::on_pushButton_headD_released()
{
    if(!_seriesAcquired && !LocationEN && !ReleaseFlag)
    {
        ReleaseFlag = true;
        LocationCD->start();
        _location->ReceiveHeadControl(0,0);
        _location->getPos(AXIS_Z);
    }
}


void NewMainWindow::on_pushButton_headU_pressed()
{
//    _location->ReceiveHeadControl(0,1);
    if(!_seriesAcquired && LocationEN)
    {
        LocationEN = false;
        ReleaseFlag = false;
        _location->setPos(AXIS_Z, 0);
    }
}


void NewMainWindow::on_pushButton_headU_released()
{
    if(!_seriesAcquired && !LocationEN && !ReleaseFlag)
    {
        ReleaseFlag = true;
        LocationCD->start();
        _location->ReceiveHeadControl(0,0);
        _location->getPos(AXIS_Z);
    }
}

void NewMainWindow::GetBedZeroStatus(bool flag)
{
    qDebug()<<"BBBBBBEEEEEEDDDDDDDD";
    if(flag)
        BedZero=true;
    else
        BedZero=false;
}


void NewMainWindow::ImageOnSignal()
{
    if(!imagesignal)
    {
        imagesignal = true;
        on_status_updated(IMAGE_GRABBER_AVAILABLE);
        emit SendSubSysStatus(iimagegraber,true);
    }
}

void NewMainWindow::ImageNoSignal()
{
    if(imagesignal)
    {
        imagesignal = false;
        on_status_updated(IMAGE_GRABBER_NONAVAILABLE);
        emit SendSubSysStatus(iimagegraber,false);
    }
}

void NewMainWindow::on_checkBox_layer_up_clicked()
{
    ui->toolButton_therapy->setDisabled(true);
    ui->toolButton_therapy->setStyleSheet(NOT_WORK);
}


void NewMainWindow::on_checkBox_layer_middle_clicked()
{
    ui->toolButton_therapy->setDisabled(true);
    ui->toolButton_therapy->setStyleSheet(NOT_WORK);
}


void NewMainWindow::on_checkBox_layer_down_clicked()
{
    ui->toolButton_therapy->setDisabled(true);
    ui->toolButton_therapy->setStyleSheet(NOT_WORK);
}


void NewMainWindow::on_checkBox_circle_one_clicked()
{
    ui->toolButton_therapy->setDisabled(true);
    ui->toolButton_therapy->setStyleSheet(NOT_WORK);
}


void NewMainWindow::on_checkBox_circle_two_clicked()
{
    ui->toolButton_therapy->setDisabled(true);
    ui->toolButton_therapy->setStyleSheet(NOT_WORK);
}


void NewMainWindow::on_checkBox_circle_three_clicked()
{
    ui->toolButton_therapy->setDisabled(true);
    ui->toolButton_therapy->setStyleSheet(NOT_WORK);
}


void NewMainWindow::on_checkBox_circle_four_clicked()
{
    ui->toolButton_therapy->setDisabled(true);
    ui->toolButton_therapy->setStyleSheet(NOT_WORK);
}


void NewMainWindow::on_checkBox_circle_five_clicked()
{
    ui->toolButton_therapy->setDisabled(true);
    ui->toolButton_therapy->setStyleSheet(NOT_WORK);
}


void NewMainWindow::on_checkBox_density_high_clicked()
{
    ui->toolButton_therapy->setDisabled(true);
    ui->toolButton_therapy->setStyleSheet(NOT_WORK);
}


void NewMainWindow::on_checkBox_density_middle_clicked()
{
    ui->toolButton_therapy->setDisabled(true);
    ui->toolButton_therapy->setStyleSheet(NOT_WORK);
}


void NewMainWindow::on_checkBox_density_low_clicked()
{
    ui->toolButton_therapy->setDisabled(true);
    ui->toolButton_therapy->setStyleSheet(NOT_WORK);
}

void NewMainWindow::on_treatmentTime_updated(){
    m_totalTime.spendTime++;
    QString str = "Treatment time " + getTimeString(m_totalTime.spendTime);
    ui->label_time_treatment->setText(str);
}


void NewMainWindow::on_pushButton_clicked()
{
    _core->TestMode();
}


void NewMainWindow::on_pushButton_return_clicked()
{
    showRegistWindow();
}

void NewMainWindow::on_Location_CD()
{
    LocationEN = true;
}

quint64 NewMainWindow::MakeId(quint32 proj_id, quint32 mach_id, quint32 obj_id, quint32 squ_id)
{
    quint64 stamp = static_cast<quint64>(QDateTime::currentMSecsSinceEpoch() - QDateTime::fromString(Baseline, "yyyy-M-d").toMSecsSinceEpoch()) / 1000;
    return (stamp << 38) | (static_cast<quint64>(proj_id) << 29) | (static_cast<quint64>(mach_id) << 12) | (static_cast<quint64>(obj_id) << 8) | static_cast<quint64>(squ_id);
}

QHash<QString, QVariant> NewMainWindow::SplitId(quint64 mId)
{
    QHash<QString, QVariant> snow;

    QHash<QString, quint64> masks = {
        {"sequence", 8},
        {"object", 4},
        {"machine", 17},
        {"project", 9},
        {"stamp", 25},
    };

    for (auto mask : masks.keys()) {
        snow[mask] = static_cast<qint64>(mId & ((1ULL << masks[mask]) - 1));
        mId >>= masks[mask];
    }

    snow["stamp"] = static_cast<qint64>(snow["stamp"].toUInt()) * 1000 + QDateTime::fromString(Baseline, "yyyy-M-d").toMSecsSinceEpoch();
    snow["date"] = QDateTime::fromMSecsSinceEpoch(snow["stamp"].toLongLong()).toString();

    return snow;
}

quint32 NewMainWindow::mIDIndex()
{
    if (idStamp == static_cast<quint64>(QDateTime::currentMSecsSinceEpoch() / 1000)) {
        idIndex++;
    } else {
        idIndex = 0;
        idStamp = static_cast<quint64>(QDateTime::currentMSecsSinceEpoch() / 1000);
    }

    return idIndex;
}

void NewMainWindow::mIDInit(quint32 proj_id, quint32 mach_id)
{
    idStamp = static_cast<quint64>(QDateTime::currentMSecsSinceEpoch() / 1000);
    idIndex = 0;

    project = proj_id;
    machine = mach_id;
}

QVariant NewMainWindow::uuid(QVariant param)
{
    if (param.type() == QVariant::ULongLong) {
        return SplitId(param.toULongLong());
    }
    return MakeId(project, machine, param.toUInt(), mIDIndex());
}

