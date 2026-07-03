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

// 记录辐照时间相关信息
struct progress_stru
{
    int pauseOn;       // 脉冲持续时间，单位为毫秒
    int pauseOff;      // 脉冲停止时间，单位为毫秒
    int coolingTime;   // 冷却时间，单位为秒
    int potsCount;     // 点数
    int pauseCount;    // 脉冲数
    int currentIndex;  // 当前运行到多少秒
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

// 辐照区域生成方式
enum AREA_GENERATE_TYPE
{
    TYPE_DRAW = 0,   // 手动勾画
    TYPE_AUTO,       // 自动生成

    TYPE_UNKNOWN
};

struct header_adjust_stru
{
    bool isAdjust;
    double destLR;      // 治疗盘目标位置左右
    double destFB;      // 治疗盘目标位置前后
    double currentLR;   // 治疗盘当前位置左右
    double currentFB;   // 治疗盘当前位置前后

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
    bool _bDrawable;           // 记录测量之前scene状态
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

    void realTimeShowPath();  // 显示实时图像上声束
    void realTimeHidePath();  // 隐藏实时图像上声束

    void realTimeShowFocus();
    void realTimeHideFocus();

    void realTimeShowRange();
    void realTimeHideRange();

    void realTimeResume();
    void changeImage();    // 改变计划图像

    void copyRows();       // 复制辐照点参数
    void deleteRows();     // 删除福找点参数
    void stopRows();       // 在此行停止
    void continueRows();   // 继续此行

    void targetShowElement();    // target窗口显示辐照单元
    void targetHideElement();    // target窗口隐藏辐照单元
    void targetResumeImages();   // target窗口图像恢复到原始图像大小
    void targetZoomOut();        // target窗口图像放大
    void targetZoomIn();         // target窗口图像缩小
    void targetShowDeleted();    // target显示删除单元
    void targetHideDeleted();    // target隐藏删除单元
	
    void targetSelectInvert();   // target反选
	void targetSmartSelect();    // target智能选择单元
    void targetResumeDeleted();  // target恢复删除的单元
	
    void targetShowContours();   // target显示肌瘤轮廓
    void targetHideContours();   // target隐藏肌瘤轮廓

    void targetShowDraw();       // target显示勾画轮廓
    void targetHideDraw();       // target隐藏勾画轮廓

    void targetDrag();           //target手动拖拽轮廓
    void targetDragConfirm();    //target确认拖拽轮廓

    void layerShowElement();     // layer窗口显示辐照单元
    void layerHideElement();     // layer窗口隐藏辐照单元
    void layerZoomOut();         // layer窗口放大图像
    void layerZoomIn();          // layer窗口缩小图像
    void layerShowDeleted();     // 显示删除的辐照单元
    void layerHideDeleted();     // 隐藏删除的辐照单元

    void layerSelectInvert();    // 反选
    void layerResumeDeleted();   // 恢复删除单元

    void layerResumeImages();    // layer窗口恢复原始图像大小

    void removeUnavailableMenu();    // 辐照开始后删除多余的右键菜单选项

    void openPump();     // 开泵
    void closePump();    // 关泵
    void queryStatus();  // 查询状态

    void getPump1();
    void getPump2();
    void getwater();
    void closeduPump1();
    void closeduPump2();
    void setduPump1Speed(int Speed);
    void setduPump2Speed(int Speed);
    void setMeter(short premeter);
    void queryMeter();

    void LogHeartBeat();   //治疗log心跳

    // 保存治疗效果图像
    void saveTreatResult();

    void statusUpdated(uint temp1,uint temp2,uint level);

    void Temp1Updated(int temp);

    void updateMeterDat(short);

    void setPumpSpeed(int speed);  // 设置泵速度

    bool setPos(int angle);    // 设置成像面角度

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

    // 收到水箱排水完成消息
    void on_Drain_Water_Finished();

    void showTableViewToolTip(const QModelIndex &index);

    void currentPlanChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight);

    void refreshAmpPara(DataFlag pFlag,QByteArray& ba);

    void refreshRealtimeImage(QImage image);    // 更新实时图像，方便移动端调用

    // 同步实时图像与计划图像与当前扫描图像角度一致
    void synImageAngle(int angle);

    // 刷新直方图
    void refreshHistogram();

    // 设置配准时搜索半径
    void setSearchRadius(int);

    // 生成层图像
    void generateLayer();

    void setConfirmSetOrderEnable();

    // 设置是否在辐照标志
    void setIrradiatingFlag(bool b);

    void switchChanged(bool status);
    // 设置治疗开始，确认，继续相关控件可用
    void setTreatmentControlEnable();

    // 设置与灯和开关对应界面图标状态
    void setLighting(APPEND_LABEL label,bool status);

    // 决定是否开启下一个辐照点
    void moveToNextSpot();
    // 设置冷却进度条进度
    void setProgressRate(int rate);

    // 设置剩余冷却时间
    void setRemainCoolingTimeValue(int value);

    // 设置表格辐照相关行颜色
    void setTableRowColor(QString id,QColor color);

    // 设置block表格辐照相关行颜色
    void setBlockTableRowColor(QString id, QColor color);

    // 设置target,layer窗口对应辐照点状态对应颜色
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

    // 发送水箱开始排水消息
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
    // 设置超声探头是否转到位
    void setCurrentUSAngle(int);

    // 设置辐照模块颜色
    void setBlockColor(QString,QColor);

    void setCurrentBlockId(QString);

    void setIrradiating(bool);

    // 设置报告中的患者信息
    void setPatientInfo(QVariant id,QVariant fname,
                        QVariant age,QVariant mobile,
                        QVariant x,QVariant y,QVariant z);

    void setTerapyInfo(QVariant rate,QVariant startTime,
                       QVariant endTime,QVariant number,
                       QVariant totalTime,QVariant deep,
                       QVariant doctor,QVariant pulse,QVariant vol);

    // 复制当前计划辐照点
    void copySpots(QVector<QString>&);
    // 删除当前计划辐照点
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
    bool _seriesAcquired;    // 是否在采集图像状态
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

    //定位图像地址
    QString imagePath;

    statusProperty stat;   //状态

    int m_networkservice;

    bool m_bIsPlanLayerScene;
    LayerScene* _planLayerScene;
    DrawScene* _slicesScene;
    // 当前实现一个角度对应一个scene，相同角度scene依次排列
    QMap<int,DrawScene*> _sceneMap;
    QMap<int,DrawScene*> _targetSceneMap;
    QMap<int,DrawScene*> _outlineMap;//保存两个轮廓的map
    // 切割图像
    QMap<int,DrawScene*> _sliceSceneMap;

    QQueue<DrawScene *> _layerScenes;    // 治疗层面辐照点分布
    QMap<QString,Spot_3d_Status> _3DStatus;    // 3d显示点状态

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
    QTimer* _refreshRealAngle;    // 刷新实时角度定时器
    QTimer* _treatmentTimeRemain;     // 此患者剩余治疗时间

    QTimer* _treatmentTimer;//治疗时间

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

    // 删除删除掉的辐照点
    void removeDeleteSpots();

    bool _subSysStatus[5];    // 记录各子系统状态
    int _currentImages;

    int planrecount=0;

    TREAT_PARA _parameter;
    int m_iLastPower;          // 记录功率数据

    QString _startTime;                        // 治疗开始时间
    QString _endTime;                          // 治疗结束时间
    qint64 startTime;
    qint64 endTime;
    int _totalTime;
    float remainTime = 0;
    int _recSpot;
    int _totalSpot;
    int _seriesSpot;
    int treatedSpot;
    int HalfClipHigh = 0;  // 裁剪图像体中心
    QString _currentSpotId;                    // 记录当前辐照点id
    QString _currentSpotIdBorn;                // 记录当前辐照点诞生id
    QMap<QString,bool> _currentCompleteMap;    // 当前完成辐照点id集合
    QMap<QString,bool> _totalCompleteMap;      // 所有完成辐照点id集合

    QMap<QString,bool> _currentCompleteBlock;  // 当前完成的block
    header_adjust_stru m_sHeaderAdjust;

    void registContour(DrawScene* scene,
                       int angle,const QImage& image);  // 配准超声肌瘤和核磁肌瘤轮廓
    void registRemainContours();
    QMap<int,QPointF> _contoursMoveMap;        // 轮廓映射偏移表
    // QMap<int,QImage> _contoursMap; // 保存未做映射的核磁肌瘤轮廓,角度为0-180
    QMap<float,QPair<int,QPointF> > mapScore;
    QMap<int,QPointF> m_movPos;
    QMap<int,QImage> m_contours;

    bool _isIrradiating;      // 记录某一点正在辐照，防止用户连续点击了多次开始
    bool _isOnSaveVideo;
    bool m_bDrawOutline;   //是否在勾画状态


    HIFUNavigation* _pNavigation;
    Mesh* _pMesh;
//    Scene3d* _pScene3d;
    Hifu3d* _pHifu3d;
    Choseimage* _pChoseimage;
    HeaderControl* _pHeaderControl;  //治疗头控制界面
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

    QMenu* _tableMenu;        // 当前治疗计划右键菜单
    QAction* _tableCopy;      // 治疗计划右键菜单对应的拷贝操作
    QAction* _tableDelete;    // 治疗计划右键菜单对应的删除操作
    QAction* _tableStop;      // 治疗计划右键菜单对应的停止操作
    QAction* _tableContinue;  // 治疗计划右键菜单对应的继续操作

    QMenu* _realTimeMenu;            // 实时图像窗口右键菜单
    QAction* _realTimeZoomOut;       // 实时图像窗口右键放大
    QAction* _realTimeZoomIn;        // 实时图像窗口右键缩小

    QAction* _realTimeShowPath;      // 实时图像窗口显示声窗
    QAction* _realTimeHidePath;      // 实时图像窗口隐藏声窗

    QAction* _realTimeShowFocus;     // 实时图像窗口显示自然焦点
    QAction* _realTimeHideFocus;     // 实时图像窗口隐藏自然焦点

    QAction* _realTimeShowRange;     // 实时图像窗口显示辐照范围
    QAction* _realTimeHideRange;     // 实时图像窗口隐藏辐照范围

    QAction* _realTimeResume;        // 实时图像窗口右键恢复原始图像大小
    QAction* _realTimeChange;        // 将实时图像添加进治疗计划图像序列

    QMenu* _targetMenu;              // target窗口右键菜单
    QAction* _targetShowElement;     // 显示辐照单元
    QAction* _targetHideElement;     // 隐藏辐照单元
    QAction* _targetResumeImages;    // 还原原始图像
    QAction* _targetZoomOut;         // 图像放大
    QAction* _targetZoomIn;          // 图像缩小
    QAction* _targetShowDeleted;     // 显示删除图像
    QAction* _targetHideDeleted;     // 隐藏删除图像
    QAction* _targetSelectInvert;    // 反选
    QAction* _targetSmartSelect;     // 智能选择辐照单元
    QAction* _targetResumeDelected;   // 恢复删除的辐照单元
    QAction* _targetShowContours;    // 显示肌瘤轮廓
    QAction* _targetHideContours;    // 隐藏肌瘤轮廓

    QAction* _targetShowDraw;         // 显示勾画轮廓
    QAction* _targetHideDraw;          // 隐藏勾画轮廓

    QAction* _targetDrag;               //手动拖拽配准轮廓
    QAction* _targetDragConfirm;        //确认拖拽

    QMenu* _layerMenu;
    QAction* _layerShowElement;
    QAction* _layerHideElement;
    QAction* _layerZoomOut;
    QAction* _layerZoomIn;
    QAction* _layerResumeImages;
    QAction* _layerShowDeleted;     // 显示删除图像
    QAction* _layerHideDeleted;     // 隐藏删除图像

    QAction* _layerSelectInvert;    // 反选
    QAction* _layerResumeDelected;  // 恢复删除辐照点

    QMenu* _researchMenu;        // 科研对应右键菜单
    QAction* _researchShow;      // 显示输入参数对话框

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

    void setLocationTips();   //设置机械移动tips

    void initTableMenu();       // 初始化治疗表格右键菜单
    void initRealtimeMenu();    // 初始化实时图像右键菜单
    void initTargetMenu();      // 初始化target窗口右键菜单
    void initLayerMenu();       // 初始化治疗层窗口右键菜单
    void initReserarchMenu();   // 初始化科研右键菜单
    void initIrradiatePlan();   // 初始化辐照计划对象

    void inintBlockPlan();    // 初始化block计划

    void initTreatPara();       // 初始化治疗参数
    void setParameter();        // 设置治疗参数
    void setControlRange(Element ele);     // 设置治疗参数控件值范围

    void initComboBox();
    void initHIFUService();    // 初始化HIFUService

    void newHardwares();
    void initDate();
    void initTimers();

    void synReportInfo();        // 同步报告信息

    void resumeMeasureLength();  // 恢复当前scene长度测量状态

    int  getTargetSceneAbove(int angle);
    int  getTargetSceneBelow(int angle);
    void initSmartPlanPanel();    // 初始化智能勾画控件

    void showDeletedElement();
    void hideDeletedElement();

    void initPlanTable();

    void initBlocksTable();

    void resetScenes();
    bool isNextRowContinue();    // 判断下一行继续标志是否为绿

    // 定位前先设置一些按钮不可用
    void disableBeforeSetPos();
    // 设置治疗层面相关控件不可用
    void disableLayerControl();
    // 设置治疗层面相关控件可用
    void enableLayerControl();

    // 设置停止按钮可用
    void enableStopButton();
    // 设置停止按钮不可用
    void disableStopButton();

    // 设置计划制定控件不可用
    void setWholePlanControlDisable();
    // 设置辐照整体计划制定控件可用
    void setWholePlanControlEnable();

    // 设置治疗开始，确认，继续相关控件不可用
    void setTreatmentControlDisable();

    // 设置运动控制相关控件不可用
    void setLocationControlEnable(bool);

    // 设置修改治疗计划控件可用
    void setModifyPlanControlEnable();

    // 设置修改治疗计划控件不可用
    void setModifyPlanControlDisable();

    // 设置治疗参数控件不可用
    void setTreatmentParaControlDisable();

    // 设置治疗参数控件可用
    void setTreatmentParaControlEnable();

    // 保存制定计划的目标图像
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

    // 清理大肌瘤治疗计划环境
    void clearBigMyoma();

    void createMask(uchar* data);    // 根据勾画的区域创建三维模型边界

    // 设置各控件tooltip
    void setToolTip();


    //测量治疗深度


    // 设置当前辐照过的点背景为绿色
    void setBackground();
    // 刷新当前计划显示表格
    void refreshCurrentTable(QQueue<Irradiate_Spot>& plan);

    void refreshBlocksTable(QQueue<Irradiate_Block>& blocks);

    // 获取此次治疗剩余时间，单位为秒
    QString getRemainTotalTime();
    // 获取整体治疗时间
    QString getTotalTime();

    void refreshRemainTime();

    // 刷新整体计划显示表格
    // void refreshTotalTable();

    void combinePlan(DENSITY_TYPE density);

    // 检查指标盘是否转到位
    void checkHeader();

    //更新患者信息
    void updateInfo(const QString &msg);

    QString getTimeString(int time);    // 由以秒为单位得到小时，分，秒表示的时间

    void DelayClose();
    QTimer delayTimer;
    QTimer servicecalled;

    // 直方图
    PaintedWidget* _pWidget;

    int m_iPatientOrientation;  //患者朝向，左侧为0，右侧为1
    int m_iRotate;    // 大肌瘤治疗时每一个block旋转角度

    float m_f3dDelta;   // 重建横断面图像时中心层面相对于自然焦点深度的偏移量

    int m_iSearchRadius;
    int bPingSuccess = 0;

    SelfTestDlg *_selfTestDlg;
    dupumpcontrol *_dupumpControl;

    QMediaPlayer* _pMediaPlayer;
    QMediaPlaylist* _playlist;

    Irradiate_model m_eModel;  // 辐照区域生成方式


    // 用于计算剩余治疗时间的数据结构
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

    //更新治疗log
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
