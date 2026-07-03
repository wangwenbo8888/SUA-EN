#ifndef NEWMAINWINDOW_H
#define NEWMAINWINDOW_H

//vkt header file
//#include <vtkAutoInit.h>
//VTK_MODULE_INIT(vtkRenderingOpenGL2)
//VTK_MODULE_INIT(vtkInteractionStyle)
//VTK_MODULE_INIT(vtkRenderingFreeType)

#include <QMainWindow>
#include <QWidget>

#include <QCloseEvent>

#include "plan.h"
#include "imagegrabber.h"
#include "drawscene.h"
#include "client.h"
#include "servicebuffer.h"
#include "location.h"
//#include "hifuwater.h"
#include "q_water.h"
#include "constant.h"
#include "message.h"
#include "variable.h"
#include "choseimage.h"
#include "headercontrol.h"
#include "commondefine.h"
#include "dupumpcontrol.h"
#include <cstdio>
#include <QPair>
#include "irradiateplan.h"
// #include "printdialog.h"
#include "printdialogclinic.h"
#include "hifuimagealgo.h"

#include "dragmodel.h"
#include "hifuhttpserver.h"

// 处理windows消息
#include <windowsx.h>
#include <newscene3d.h>
#include <planscene3d.h>
#include <qmediaplayer.h>
#include <QMediaPlaylist>

#include "irradiateblock.h"

#include "heart_thread.h"
#include <QVector>
#include "quitform.h"
#include "draintankform.h"

#include "qgraphicsscene.h"


//#include "scene3d.h"



class QQuickView;
class IrradiateTables;
class QTableWidget;
class IrradiatePlan;
//class Scene3d;
class Hifu3d;
class Mesh;
class Append;
class QStandardItemModel;
class RealtimeScene;
class QMenu;
class QAction;
class QMessageBox;
class QQuickItem;
class IrradiateCore;
class IrradiatePara;
class HIFUNavigation;
class DragTable;
class SelfTestDlg;
class PaintedWidget;
class BlockPlan;
class LayerScene;
class dupumpcontrol;
//class HifuImageAlgo;
class Wireless;

// 记录sonicationTime相关信息
struct progress_stru
{
    int pauseOn;       // 脉冲Duration，单位为毫秒
    int pauseOff;      // 脉冲StopTime，单位为毫秒
    int coolingTime;   // Cooling，单位为秒
    int potsCount;     // 点数
    int pauseCount;    // Pulses
    int currentIndex;  // 当前Running到多少秒
    int totalTime;     // 单位秒
};

struct ProcessInfo{

    QString processID;              //进程ID
    QString threadNum;              //该进程开启的线程数
    QString parentProcessID;        //父进程ID
    QString processPri;             //线程优先权
    QString processHandle;          //进程句柄
    QString processName;            //名称
};

// sonication区域Generate方式
enum AREA_GENERATE_TYPE
{
    TYPE_DRAW = 0,   // Manual Tracing
    TYPE_AUTO,       // Auto Generate

    TYPE_UNKNOWN
};

struct header_adjust_stru
{
    bool isAdjust;
    double destLR;      // Treatment盘目标位置左右
    double destFB;      // Treatment盘目标位置前后
    double currentLR;   // Treatment盘当前位置左右
    double currentFB;   // Treatment盘当前位置前后

    header_adjust_stru()
    {
        isAdjust=false;
        destLR = 0.0;
        destFB = 0.0;
        currentLR = 0.0;
        currentFB = 0.0;
    }

};

struct statusProperty
{
    bool _bDrawable;           // 记录测量之前sceneStatus
    bool _bTargetDrawable;
    bool _bMeasureable;
    bool _beamVisiable;
    bool _transducerVisiable;
};

Q_DECLARE_LOGGING_CATEGORY(NEWMAINWINDOW)

using namespace std;
namespace Ui {
class NewMainWindow;
}

class NewMainWindow : public QMainWindow
{
    Q_OBJECT

public:

    explicit NewMainWindow(SelfTestDlg * selftestdlg,QWidget *parent = 0);
    ~NewMainWindow();

    void setMainWindow(QQuickView* view);

    Q_INVOKABLE void sendPatientInfo(QVariant id,QVariant name,
                           QVariant age,QVariant mobile,
                           QVariant x, QVariant y, QVariant z);

    void getSnapshot();//获取线程函数
    vector<QString> process_vec;//用于存放后台线程名

    void setdupump(dupumpcontrol *_dupump);
    double posZ,posA,posF,posL;

protected:

    void closeEvent(QCloseEvent *event);
    void showEvent(QShowEvent *event);

    bool nativeEvent(const QByteArray &eventType, void *message, long *result);

    void insertInfo(const QString &pID, const QString &thrNum,
                    const QString &parePID, const QString &processPr,
                    const QString &handle, const QString &processName);

private slots:

    void sendPatientInfo(QString id,QString fname,
                         QString age,QString mobile,
                         QString x,QString y,QString z,int PatientOrientation);

    void receiveQImage(QImage image,int angle);

    void receiveContour(QImage contour,int angle);

    void receiveOuter(QImage outer,int angle);

    void moveContour();

    //深度测量
//    void deepthmeasure();
    void startdeeptimer();

    void realTimeZoomOut();
    void realTimeZoomIn();

    void realTimeShowPath();  // 显示Live Image上声束
    void realTimeHidePath();  // 隐藏Live Image上声束

    void realTimeShowFocus();
    void realTimeHideFocus();

    void realTimeShowRange();
    void realTimeHideRange();

    void realTimeResume();
    void changeImage();    // 改变Planning Image

    void copyRows();       // 复制sonication point参数
    void deleteRows();     // Delete福找点参数
    void stopRows();       // 在此行Stop
    void continueRows();   // 继续此行

    void targetShowElement();    // target窗口Show Sonication Cells
    void targetHideElement();    // target窗口Hide Sonication Cells
    void targetResumeImages();   // target窗口图像Restore到原始图像大小
    void targetZoomOut();        // target窗口Zoom In
    void targetZoomIn();         // target窗口Zoom Out
    void targetShowDeleted();    // targetShow Deleted Cells
    void targetHideDeleted();    // targetHide Deleted Cells
	
    void targetSelectInvert();   // target反选
	void targetSmartSelect();    // targetSmart Select Cells
    void targetResumeDeleted();  // targetRestoreDelete的单元
	
    void targetShowContours();   // target显示fibroid轮廓
    void targetHideContours();   // target隐藏fibroid轮廓

    void targetShowDraw();       // targetShow Traced Contour
    void targetHideDraw();       // targetHide Traced Contour

    void targetDrag();           //target手动拖拽轮廓
    void targetDragConfirm();    //targetConfirm拖拽轮廓

    void layerShowElement();     // layer窗口Show Sonication Cells
    void layerHideElement();     // layer窗口Hide Sonication Cells
    void layerZoomOut();         // layer窗口Zoom in图像
    void layerZoomIn();          // layer窗口Zoom out图像
    void layerShowDeleted();     // 显示Delete的sonication cell
    void layerHideDeleted();     // 隐藏Delete的sonication cell

    void layerSelectInvert();    // 反选
    void layerResumeDeleted();   // Restore Deleted Cells

    void layerResumeImages();    // layer窗口Reset to Original Image Size

    void removeUnavailableMenu();    // sonication开始后Delete多余的右键菜单选项

    void openPump();     // Start Pump
    void closePump();    // Stop Pump
    void queryStatus();  // QueryStatus

    void getPump1();
    void getPump2();
    void getwater();
    void closeduPump1();
    void closeduPump2();
    void setduPump1Speed(int Speed);
    void setduPump2Speed(int Speed);
    void setMeter(short premeter);
    void queryMeter();

    void LogHeartBeat();   //Treatmentlog心跳

    // SaveTreatment效果图像
    void saveTreatResult();

    void statusUpdated(uint temp1,uint temp2,uint level);

    void Temp1Updated(int temp);

    void updateMeterDat(short);

    void setPumpSpeed(int speed);  // 设置Pump Speed度

    bool setPos(int angle);    // 设置成像面Angle

    bool setPos(AID axis,float pos);

    bool networkcalled();
    void on_pushButton_quit_clicked();

    void on_horizontalScrollBar_images_valueChanged(int value);

    void on_toolButton_dqa_clicked();

    void on_horizontalSlider_left_right_valueChanged(int value);

    void on_horizontalSlider_forward_backward_valueChanged(int value);

    void on_horizontalSlider_up_down_valueChanged(int value);

    void on_horizontalSlider_left_right_sliderReleased();

    void on_horizontalSlider_forward_backward_sliderReleased();

    void on_location_status_updated(AID, APOS);
    void on_location_error_updated(ERRORTYPE);

    void on_toolButton_save_image_2_clicked();

    void on_toolButton_acquire_image_clicked();

    void on_horizontalScrollBar_images_sliderReleased();

    void on_displayTime_updated();

    void on_treatment_time_remain_update();

    void on_refresh_real_angle_timeout();

    void on_acquireSingleFrame_completed();

    void on_acquireSeries_completed();

    void on_toolButton_transducer_clicked();

    void on_toolButton_beam_path_clicked();

    void on_toolButton_therapy_clicked();

    void on_pushButton_stop_clicked();

    void on_commandLinkButton_measure_length_clicked();

    void on_progressBar_cooling_time_valueChanged(int value);

    void on_label_real_time_angle_objectNameChanged(const QString &objectName);

    void on_toolButton_change_image_plane_clicked();

    void on_horizontalSlider_power_valueChanged(int value);

    void on_pushButton_treatment_report_clicked();

    // void on_toolButton_rute_acquire_image_clicked();

    void on_pushButton_front_clicked();

    void on_pushButton_back_clicked();

    void on_pushButton_treatment_layer_down_clicked();

    void on_pushButton_treatment_layer_middle_clicked();

    void on_pushButton_treatment_layer_up_clicked();

    void on_pushButton_start_treat_clicked();

    void on_pushButton_continue_clicked();

    void on_commandLinkButton_generate_plan_clicked();

    void on_tableView_current_plan_customContextMenuRequested(const QPoint &pos);

    void on_graphicsView_realtime_image_customContextMenuRequested(const QPoint &pos);

    void on_graphicsView_target_scene_customContextMenuRequested(const QPoint &pos);

    void on_graphicsView_treatment_layer_customContextMenuRequested(const QPoint &pos);

    // void on_commandLinkButton_save_target_clicked();

    void on_horizontalSlider_cooling_time_valueChanged(int value);

    void on_label_target_image_customContextMenuRequested(const QPoint &pos);

    void on_toolButton_save_video_2_clicked();

    void on_horizontalSlider_on_time_valueChanged(int value);

    void on_horizontalSlider_off_time_valueChanged(int value);

    void on_horizontalSlider_pulse_number_valueChanged(int value);

    void on_commandLinkButton_water_circle_clicked();

    void on_comboBox_part_currentTextChanged(const QString &arg1);

    void on_comboBox_deepth_currentTextChanged(const QString &arg1);

    void on_horizontalScrollBar_slices_valueChanged(int value);

    void on_pushButton_resume_para_clicked();

    void on_spinBox_angle_one_valueChanged(int arg1);

    void on_spinBox_angle_two_valueChanged(int arg1);

    void on_checkBox_draw_one_toggled(bool checked);

    void on_checkBox_confirm_one_toggled(bool checked);

    void on_checkBox_draw_two_toggled(bool checked);

    void on_checkBox_confirm_two_toggled(bool checked);

    void on_pushButton_draw_spot_clicked();

    void on_pushButton_clear_contour_clicked();

    void on_comboBox_irradiate_model_currentIndexChanged(const QString &arg1);

    void on_toolButton_header_zero_clicked();

    void on_pushButton_modify_para_clicked();

    void on_pushButton_confirm_para_clicked();

    void on_pushButton_histogram_clicked();

    void on_pushButton_measure_length_clicked();

    void on_checkBox_density_high_toggled(bool checked);

    void on_checkBox_density_middle_toggled(bool checked);

    void on_checkBox_density_low_toggled(bool checked);

    void on_pushButton_replan_clicked();

    void on_pushButton_generate_plan_clicked();

    void on_pushButton_block_confirm_clicked();

    void on_pushButton_draw_clicked();

    void on_pushButton_drag_clicked();

    void on_pushButton_draw_confirm_clicked();

    void on_pushButton_drag_confirm_clicked();

    void on_pushButton_cancel_clicked();

    void on_pushButton_acquire_clicked();

    void on_pushButton_set_order_clicked();

    void on_pushButton_confirm_order_clicked();

    void on_horizontalScrollBar_contrast_valueChanged(int value);

    void sendpar(uchar,uchar);

    //void on_pushButton_controlpump_clicked();

    void pump1open();
    void pump1close();
    void pump1lock();
    void pump2open();
    void pump2close();
    void pump2lock();
    void controllock();
    void controlunlock();

    void updatespeed_in(int);

    void updatespeed_out(int);

    void updatemeter(float);

    void updatelevel(int);

    void on_toolButton_zoomout_image_clicked();

    void on_toolButton_zoomin_image_clicked();

    void on_toolButton_showpath_clicked();

    void on_toolButton_hidepath_clicked();

    void on_pushButton_pump1control_clicked();

    void on_pushButton_pump2control_clicked();

    void on_pushButton_pump1speeddown_clicked();

    void on_pushButton_pump1speedup_clicked();

    void on_pushButton_pump2speeddown_clicked();

    void on_pushButton_pump2speedup_clicked();

    void stopsonication();
    void netconnected();
    void LocationAlarm();
    void LocationSetPosSuccess();

    void LocationHeadControl(int,int);

    void on_pushButton_headL_pressed();

    void on_pushButton_headL_released();

    void on_pushButton_headR_pressed();

    void on_pushButton_headR_released();

    void on_pushButton_headF_pressed();

    void on_pushButton_headF_released();

    void on_pushButton_headB_pressed();

    void on_pushButton_headB_released();

    void on_pushButton_headD_pressed();

    void on_pushButton_headD_released();

    void on_pushButton_headU_pressed();

    void on_pushButton_headU_released();

    void on_checkBox_layer_up_clicked();

    void on_checkBox_layer_middle_clicked();

    void on_checkBox_layer_down_clicked();

    void on_checkBox_circle_one_clicked();

    void on_checkBox_circle_two_clicked();

    void on_checkBox_circle_three_clicked();

    void on_checkBox_circle_four_clicked();

    void on_checkBox_circle_five_clicked();

    void on_checkBox_density_high_clicked();

    void on_checkBox_density_middle_clicked();

    void on_checkBox_density_low_clicked();

    void on_treatmentTime_updated();

    void on_pushButton_clicked();

    void on_pushButton_return_clicked();

    void on_Location_CD();

public slots:

    // 收到Water tank drain complete消息
    void on_Drain_Water_Finished();

    void showTableViewToolTip(const QModelIndex &index);

    void currentPlanChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight);

    void refreshAmpPara(DataFlag pFlag,QByteArray& ba);

    void refreshRealtimeImage(QImage image);    // 更新Live Image，方便Mobile调用

    // 同步Live Image与Planning Image与当前扫描图像Angle一致
    void synImageAngle(int angle);

    // Refresh直方图
    void refreshHistogram();

    // 设置registration时搜索半径
    void setSearchRadius(int);

    // Generate层图像
    void generateLayer();

    void setConfirmSetOrderEnable();

    // 设置是否在sonication标志
    void setIrradiatingFlag(bool b);

    void switchChanged(bool status);
    // 设置Treatment开始，Confirm，继续相关控件可用
    void setTreatmentControlEnable();

    // 设置与灯和开关对应界面图标Status
    void setLighting(APPEND_LABEL label,bool status);

    // 决定是否开启下一个sonication point
    void moveToNextSpot();
    // 设置Cooling进度条进度
    void setProgressRate(int rate);

    // 设置剩余Cooling
    void setRemainCoolingTimeValue(int value);

    // 设置表格sonication相关行颜色
    void setTableRowColor(QString id,QColor color);

    // 设置block表格sonication相关行颜色
    void setBlockTableRowColor(QString id, QColor color);

    // 设置target,layer窗口对应sonication pointStatus对应颜色
    void setSpotColor(QString id,QColor color);

    void spotSelected(QString id,QColor color);
    void spotSelectedInTarget(QString id,QColor color);

    void showRegistWindow();

    void addScene(QImage image);

    void queryAmpTempature();

    void queryAmpVoltage();

    void changeView2D(viewer2D_info info);
    void changeLayerViewer(viewer2D_info info);

    void RefreshSubsystem();

    void QuitBeforShow();

    void setpos(int a,float b);

    void appendConnectFailed();

    void on_status_updated(DataFlag);

    void GateWayConnectFailed();

    void enablestop();
    void disablestop();
    void GetBedZeroStatus(bool);

    void ImageOnSignal();

    void ImageNoSignal();
signals:

    // 发送tank开始排水消息
    void sendDrainTank();

    void sendGetContour();

    void sendPatientOrientation(int ori);

    void saveReport();

    void setLayer();

    void refreshAmpParaInTable(DataFlag,QByteArray& );

    void showSelfCheckDlg();

    void showdupumpcontrol();

    void setWirelessRun();

    void setIsPause(bool);
    // 设置Ultrasound Probe是否转到位
    void setCurrentUSAngle(int);

    // 设置sonication模块颜色
    void setBlockColor(QString,QColor);

    void setCurrentBlockId(QString);

    void setIrradiating(bool);

    // 设置报告中的Patient information
    void setPatientInfo(QVariant id,QVariant fname,
                        QVariant age,QVariant mobile,
                        QVariant x,QVariant y,QVariant z);

    void setTerapyInfo(QVariant rate,QVariant startTime,
                       QVariant endTime,QVariant number,
                       QVariant totalTime,QVariant deep,
                       QVariant doctor,QVariant pulse,QVariant vol);

    // 复制Current Plansonication point
    void copySpots(QVector<QString>&);
    // DeleteCurrent Plansonication point
    void deleteSpots(QVector<QString>&);

    void showUserManager(bool isPatient);
    void showReport();
    void MainClose();
    void networkfailed();
    void SendSelfTest(bool bappend,bool blocation,bool bwarter,bool bnetwork,bool bimagegraber);
    void SendSubSysStatus(selfSubSys sys,bool status);

    void selfCheckStatusUpdate(uint temp1,uint temp2,uint level);

    void networkcheck();

    void MeterUpdate(short meter);
    void pump1ctl();
    void speed1up();
    void speed1down();
    void pump2ctl();
    void speed2up();
    void speed2down();

    void heartconnect();
    void showquitform();
    void updateHeadPos(float,float,float);
    void getRodPos();

    void SaveVideo(QString);

private:

    Ui::NewMainWindow *ui;
    bool _seriesAcquired;    // 是否在采集图像Status
    bool imagesignal;

    QThread *grabber_thread;

    ImageGrabber *_grabber;
    RealtimeScene *_realtimeScene;
    DrawScene *_targetScene;
    DrawScene* _layerScene;
    HifuImageAlgo* _hifuimagealgo;


//   double RESOLUTION;
//   int X_OFFSET;
//   int Y_OFFSET;

    QString m_realPath;
    QString m_tmpPath;

    //Positioning图像地址
    QString imagePath;

    statusProperty stat;   //Status

    int m_networkservice;

    bool m_bIsPlanLayerScene;
    LayerScene* _planLayerScene;
    DrawScene* _slicesScene;
    // 当前实现一个Angle对应一个scene，相同Anglescene依次排列
    QMap<int,DrawScene*> _sceneMap;
    QMap<int,DrawScene*> _targetSceneMap;
    QMap<int,DrawScene*> _outlineMap;//Save两个轮廓的map
    // 切割图像
    QMap<int,DrawScene*> _sliceSceneMap;

    QQueue<DrawScene *> _layerScenes;    // Treatment PlaneSonication Point Distribution
    QMap<QString,Spot_3d_Status> _3DStatus;    // 3d显示点Status

    uchar* _pixData;
    ServiceBuffer *_schedule;
    Message *_msgCtrl;
    Plan *_plan;
    Location *_location;
    Append* _append;
    Wireless *_wireless;
    IrradiateCore* _core;
    IrradiatePara* _para;
    QTimer *_updatedDateTimer, *_acquisionTimer, *_prepareTimer;
    QTimer* _refreshRealAngle;    // RefreshLiveAngle定时器
    QTimer* _treatmentTimeRemain;     // 此Patients剩余Treatment time

    QTimer* _treatmentTimer;//Treatment time

    QTimer* LocationCD;//运动控制防抖
    bool LocationEN = true;
    bool ReleaseFlag = false;
    BOOL is_first_treat =true;//标志位
    BOOL is_plan_completed =false;//标志位

    int _destAngle;
    NewScene3D *_newScene3d;
    PlanScene3D* _planScene3d;
    HifuHttpServer *_hifuHttpServer;
    viewer2D_info _viewer2DInfo;
    viewer2D_info _layerViewer2DInfo;

    bool _contoursVisiable;
    bool appendalarm=false;

    // DeleteDelete掉的sonication point
    void removeDeleteSpots();

    bool _subSysStatus[5];    // 记录各子系统Status
    int _currentImages;

    int planrecount=0;

    TREAT_PARA _parameter;
    int m_iLastPower;          // 记录Power数据

    QString _startTime;                        // TreatmentStart time
    QString _endTime;                          // TreatmentEnd time
    qint64 startTime;
    qint64 endTime;
    int _totalTime;
    float remainTime = 0;
    int _recSpot;
    int _totalSpot;
    int _seriesSpot;
    int treatedSpot;
    int HalfClipHigh = 0;  // 裁剪图像体中心
    QString _currentSpotId;                    // 记录当前sonication pointid
    QString _currentSpotIdBorn;                // 记录当前sonication point诞生id
    QMap<QString,bool> _currentCompleteMap;    // 当前Completesonication pointid集合
    QMap<QString,bool> _totalCompleteMap;      // 所有Completesonication pointid集合

    QMap<QString,bool> _currentCompleteBlock;  // 当前Complete的block
    header_adjust_stru m_sHeaderAdjust;

    void registContour(DrawScene* scene,
                       int angle,const QImage& image);  // registration超声fibroid和核磁fibroid轮廓
    void registRemainContours();
    QMap<int,QPointF> _contoursMoveMap;        // 轮廓映射偏移表
    // QMap<int,QImage> _contoursMap; // Save未做映射的核磁fibroid轮廓,Angle为0-180
    QMap<float,QPair<int,QPointF> > mapScore;
    QMap<int,QPointF> m_movPos;
    QMap<int,QImage> m_contours;

    bool _isIrradiating;      // 记录某一点正在sonication，防止用户连续点击了多次开始
    bool _isOnSaveVideo;
    bool m_bDrawOutline;   //是否在TraceStatus


    HIFUNavigation* _pNavigation;
    Mesh* _pMesh;
//    Scene3d* _pScene3d;
    Hifu3d* _pHifu3d;
    Choseimage* _pChoseimage;
    HeaderControl* _pHeaderControl;  //Probe控制界面
    IrradiateTables* _tableList;
    IrradiatePlan*   _irradiatePlan;

    BlockPlan* _pBlockPlan;
    QStandardItemModel* _pCurrentModel;
    QStandardItemModel* _pBlockModel;
    // DragModel* _pCurrentModel;
    // QStandardItemModel* _pTotalModel;

    // DragTable* _table;

    PrintDialogClinic* _printDialog;

    quitform* _quitform;
    DrainTankForm* _drainTankForm;

    QMenu* _tableMenu;        // 当前TreatmentPlan右键菜单
    QAction* _tableCopy;      // TreatmentPlan右键菜单对应的拷贝操作
    QAction* _tableDelete;    // TreatmentPlan右键菜单对应的Delete操作
    QAction* _tableStop;      // TreatmentPlan右键菜单对应的Stop操作
    QAction* _tableContinue;  // TreatmentPlan右键菜单对应的继续操作

    QMenu* _realTimeMenu;            // Live Image窗口右键菜单
    QAction* _realTimeZoomOut;       // Live Image窗口右键Zoom in
    QAction* _realTimeZoomIn;        // Live Image窗口右键Zoom out

    QAction* _realTimeShowPath;      // Live Image窗口Show Acoustic Window
    QAction* _realTimeHidePath;      // Live Image窗口Hide Acoustic Window

    QAction* _realTimeShowFocus;     // Live Image窗口Show Natural Focus
    QAction* _realTimeHideFocus;     // Live Image窗口Hide Natural Focus

    QAction* _realTimeShowRange;     // Live Image窗口Show Sonication Range
    QAction* _realTimeHideRange;     // Live Image窗口Hide Sonication Range

    QAction* _realTimeResume;        // Live Image窗口右键Reset to Original Image Size
    QAction* _realTimeChange;        // 将Live ImageAdd进TreatmentPlanning ImageSeries

    QMenu* _targetMenu;              // target窗口右键菜单
    QAction* _targetShowElement;     // Show Sonication Cells
    QAction* _targetHideElement;     // Hide Sonication Cells
    QAction* _targetResumeImages;    // 还原原始图像
    QAction* _targetZoomOut;         // Zoom In
    QAction* _targetZoomIn;          // Zoom Out
    QAction* _targetShowDeleted;     // 显示Delete图像
    QAction* _targetHideDeleted;     // 隐藏Delete图像
    QAction* _targetSelectInvert;    // 反选
    QAction* _targetSmartSelect;     // 智能Selectsonication cell
    QAction* _targetResumeDelected;   // RestoreDelete的sonication cell
    QAction* _targetShowContours;    // 显示fibroid轮廓
    QAction* _targetHideContours;    // 隐藏fibroid轮廓

    QAction* _targetShowDraw;         // Show Traced Contour
    QAction* _targetHideDraw;          // Hide Traced Contour

    QAction* _targetDrag;               //手动拖拽Register Contours
    QAction* _targetDragConfirm;        //Confirm拖拽

    QMenu* _layerMenu;
    QAction* _layerShowElement;
    QAction* _layerHideElement;
    QAction* _layerZoomOut;
    QAction* _layerZoomIn;
    QAction* _layerResumeImages;
    QAction* _layerShowDeleted;     // 显示Delete图像
    QAction* _layerHideDeleted;     // 隐藏Delete图像

    QAction* _layerSelectInvert;    // 反选
    QAction* _layerResumeDelected;  // RestoreDeletesonication point

    QMenu* _researchMenu;        // 科研对应右键菜单
    QAction* _researchShow;      // 显示Input参数对话框

    QMessageBox* _pMessage;       // 弹出对话框

    void init();
    void initGrabber();
    void initWater();
    void initCore();
    void initSchedule();
    void initMsgCtrl();
    void initLocation();
    void initAppend();
    void setUserModel();

    void setLocationTips();   //设置Motion Systemtips

    void initTableMenu();       // 初始化Treatment表格右键菜单
    void initRealtimeMenu();    // 初始化Live Image右键菜单
    void initTargetMenu();      // 初始化target窗口右键菜单
    void initLayerMenu();       // 初始化Treatment层窗口右键菜单
    void initReserarchMenu();   // 初始化科研右键菜单
    void initIrradiatePlan();   // 初始化Sonication Plan对象

    void inintBlockPlan();    // 初始化blockPlan

    void initTreatPara();       // 初始化Treatment Parameters
    void setParameter();        // 设置Treatment Parameters
    void setControlRange(Element ele);     // 设置Treatment Parameters控件值范围

    void initComboBox();
    void initHIFUService();    // 初始化HIFUService

    void newHardwares();
    void initDate();
    void initTimers();

    void synReportInfo();        // 同步报告信息

    void resumeMeasureLength();  // Restore当前scene长度测量Status

    int  getTargetSceneAbove(int angle);
    int  getTargetSceneBelow(int angle);
    void initSmartPlanPanel();    // 初始化智能Trace控件

    void showDeletedElement();
    void hideDeletedElement();

    void initPlanTable();

    void initBlocksTable();

    void resetScenes();
    bool isNextRowContinue();    // 判断下一行继续标志是否为绿

    // Positioning前先设置一些按钮不可用
    void disableBeforeSetPos();
    // 设置Treatment Plane相关控件不可用
    void disableLayerControl();
    // 设置Treatment Plane相关控件可用
    void enableLayerControl();

    // 设置Stop按钮可用
    void enableStopButton();
    // 设置Stop按钮不可用
    void disableStopButton();

    // 设置Plan制定控件不可用
    void setWholePlanControlDisable();
    // 设置sonicationOverall Plan制定控件可用
    void setWholePlanControlEnable();

    // 设置Treatment开始，Confirm，继续相关控件不可用
    void setTreatmentControlDisable();

    // 设置运动控制相关控件不可用
    void setLocationControlEnable(bool);

    // 设置修改TreatmentPlan控件可用
    void setModifyPlanControlEnable();

    // 设置修改TreatmentPlan控件不可用
    void setModifyPlanControlDisable();

    // 设置Treatment Parameters控件不可用
    void setTreatmentParaControlDisable();

    // 设置Treatment Parameters控件可用
    void setTreatmentParaControlEnable();

    // Save制定Plan的目标图像
    void saveTargetImages();

    void createPlanFold();
    void deletePlanFold();
    void locationZero();
    void updateLocation();

    void clearTargetScenes();
    void clearSliceScenes();
    void clearLayerScenes();

    void closeHIFUService();

    void showPlan();

    bool deleteFile(QString file);

    // 判断字符串是否为数字串
    bool isDigitStr(const QString& str);

    bool isInputAvai(const QModelIndex &topLeft,
                     double& voltage,int& pulse);

    void setScenesUndrawable();

    // 清理Large Fibroid TreatmentPlan环境
    void clearBigMyoma();

    void createMask(uchar* data);    // 根据Trace的区域创建三维模型边界

    // 设置各控件tooltip
    void setToolTip();


    //测量Treatment depth


    // 设置当前sonication过的点背景为绿色
    void setBackground();
    // RefreshCurrent Plan显示表格
    void refreshCurrentTable(QQueue<Irradiate_Spot>& plan);

    void refreshBlocksTable(QQueue<Irradiate_Block>& blocks);

    // 获取此次TreatmentRemaining time，单位为秒
    QString getRemainTotalTime();
    // 获取整体Treatment time
    QString getTotalTime();

    void refreshRemainTime();

    // RefreshOverall Plan显示表格
    // void refreshTotalTable();

    void combinePlan(DENSITY_TYPE density);

    // 检查指标盘是否转到位
    void checkHeader();

    //更新Patient information
    void updateInfo(const QString &msg);

    QString getTimeString(int time);    // 由以秒为单位得到小时，分，秒表示的Time

    void DelayClose();
    QTimer delayTimer;
    QTimer servicecalled;

    // 直方图
    PaintedWidget* _pWidget;

    int m_iPatientOrientation;  //Patients朝向，左侧为0，右侧为1
    int m_iRotate;    // Large Fibroid Treatment时每一个block旋转Angle

    float m_f3dDelta;   // 重建横断面图像时中心Plane相对于focus深度的偏移量

    int m_iSearchRadius;
    int bPingSuccess = 0;

    SelfTestDlg *_selfTestDlg;
    dupumpcontrol *_dupumpControl;

    QMediaPlayer* _pMediaPlayer;
    QMediaPlaylist* _playlist;

    Irradiate_model m_eModel;  // sonication区域Generate方式


    // 用于计算剩余Treatment time的数据结构
    TOTAL_TIME m_totalTime;

    bool isdoublepump;
    bool m_bPump1Open;
    bool m_bPump2Open;

    bool connectflag;
    bool powerampready;
    bool networkready;
    bool waterready;
    bool locationready;
    int locationerror = 0;
    bool appendready;
    bool flag1;
    bool BedZero;
    bool Auto_WaterLock,Image_WaterLock;

    void Delay_MSec(unsigned int msec);

    heartthread *Heartthread;
    QThread *Heartthread_thread;

    QFont m_msgBoxFont;

    QVector<ProcessInfo> m_vec;//用于存放后台线程所有信息

    //更新Treatmentlog
    void updateLogFile(QString name, QByteArray& log);
    void updateLog(const QString &msg);

    QTimer *LogHeartTimer;
    QString PID,TID,PName,power,on_time,off_time,pulse_number,cooling_time,TreatedUnit;
    int totalNumber,totalTime;

    const QString Baseline = "2023-1-1";
    quint64 idIndex;
    quint64 idStamp;
    quint32 project;
    quint32 machine;
    quint32 object;
    QString Pname,SID;

    quint64 MakeId(quint32 proj_id, quint32 mach_id, quint32 obj_id, quint32 squ_id);
    QHash<QString, QVariant> SplitId(quint64 mId);
    quint32 mIDIndex();
    void mIDInit(quint32 proj_id, quint32 mach_id);
    QVariant uuid(QVariant param);
};

#endif // NEWMAINWINDOW_H
