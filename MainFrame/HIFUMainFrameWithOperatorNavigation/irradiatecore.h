

#ifndef IRRADIATECORE_H_
#define IRRADIATECORE_H_

#include <QObject>
#include <QString>
#include "qloggingcategory.h"

#include "commondefine.h"
#include "variable.h"
#include "qcolor.h"
#include "phasecontroller.h"
#include "poweramp.h"

#if _MSC_VER >1600
#pragma execution_character_set("utf-8")
#endif

class IrradiatePlan;
class Append;
class ServiceBuffer;
class Plan;
class Message;
class QMessageBox;
class QTimer;
class QCloseEvent;

enum Irradiate_Status
{
    STATUS_UNKNOWN = 0,             // 状态未知
    STATUS_SENDED_PARAMETER,    // 发送了辐照参数
    STATUS_READY,               // 工控机准备完成
    STATUS_STARTED,             // 开始辐照
    STATUS_PAUSE,               // 辐照被暂停
    STATUS_STOPPED,             // 辐照被停止
    STATUS_SPOT_COMPLETED,
    STATUS_PLAN_COMPLETED            // 辐照顺利完成
};


Q_DECLARE_LOGGING_CATEGORY(IRRADIATECORE)

// 辐照过程控制
class IrradiateCore : public QObject
{
    Q_OBJECT

public:

    IrradiateCore(IrradiatePlan* plan,Append* append,
                  ServiceBuffer* schedule,Plan* planInfo,
                  Message* message);

    ~IrradiateCore();

    // 向工控机发送治疗点相关参数
    bool sendSpotParameter();
    void setcurrentCompleteMap(QMap<QString,bool> *_map);

private slots:

    void setIsPause(bool b);

    // 设置是否正在辐照标志
    void setIrradiating(bool b);
    // 脚踏开关槽函数
    void switchChanged(bool flag);
    // 工控机状态变化通知消息
    void statusChanged(DataFlag flag);

    // 弹出框yes按钮被按下
    void yesButtonDown();

    // 弹出框no按钮被按下

    void noButtonDown();
    // 冷却定时器超时
    void on_cooling_timeout();
    // 进度条更新定时器超时
    void on_progress_timeout();

    // 设置超声探头当前位置
    void setCurrentUSAngle(int angle);

    void SpotCompleted();

public slots:
    void StopSonication();
    bool init();
    void SetDisPlayPlan(IrradiatePlan* plan);
    void TestMode();

signals:

    // 同步计划图像与实时图像和辐照的位置角度一致
    void synImageAngle(int angle);

    // 设置是否在辐照标志
    void setIrradiatingFlag(bool);
    // 设置治疗相关控件可用
    void setTreatmentControlEnable();
    // 设置与灯和开关对应界面图标状态
    void setLighting(APPEND_LABEL,bool);

    // 确认开始下一个辐照点
    void moveToNextSpot();

    // 设置进度条进度
    void setProgressRate(int rate);

    // 设置剩余冷却时间数值
    void setRemainCoolingTime(int value);

    // 设置辐照参数表格对应颜色
    void setTableRowColor(QString,QColor);
    // 设置辐照点颜色
    void setSpotColor(QString,QColor);
    void startsonication();
    void stop_sonication();

private :

    bool isUSReady()
    {
        return _CurrentUSAngle==_AimUSAngle;
    }

    // 确认是否开始辐照
    void readyFire();

//    // 接收到辐照停止的消息
//    void onSonicationStopped();

    // 接收到辐照计划完成的消息
    void onPlanComplete();

    // 处理脚踏开关迅速抬起的情况
    void switchQuickOff();

    // 初始化冷却及进度条相关
    void initCooling();

    void initPA();
    void initPC();
    QString _spotId;           // 当前辐照点id
    QString _spotBornId;       // 当前辐照点born id
    bool _isPause;             // 是否是暂停
    bool _isIrradiating;       // 是否在辐照
    bool b_PA,b_PC;
    bool PlanIsCompleted;
    bool _isButtonYesDown;
    bool _isButtonNoDown;
    bool pmessageflag;
    int freqnum = 0;

    ServiceBuffer* _schedule;
    Message* _msgCtrl;
    Plan* _plan;
    Append* _append;           // 控制盒及脚踏开关
    IrradiatePlan* _irradiatePlan;    // 当前辐照计划
    Irradiate_Status _status;         // 当前对应辐照点状态

    PowerAmp *m_PA;
    PhaseController *m_PC;

    QMessageBox* _pMessage;           // 脚踏开关未闭合弹出对话框

    QTimer* _coolingTimer;    // 冷却时间定时器，单辐照点发送计划，驱动模块忽略最后一个点冷却时间
    QTimer* _progressTimer;   // 冷却进度条刷新定时器
    int _coolingTime;         // 冷却时间
    int _coolingTimeCount;    // 冷却时间
    // 脚踏开关导致的停止认为是暂停，其它认为是停止

    // 期望超声角度
    int _AimUSAngle;

    // 实际超声角度
    int _CurrentUSAngle;

    double voltage;

    QMap<QString,bool> *_currentCompleteMap;    // 记录当前计划完成辐照点信息

    double mainFreq = 340e6;
    int grade[8] = {250, 248, 246, 244, 242, 256, 254, 252};
    double freq[8] = {
        1.360e6,
        1.370968e6,
        1.382114e6,
        1.393443e6,
        1.404959e6,
        1.328125e6,
        1.338583e6,
        1.349206e6
    };
};


#endif // IRRADIATECORE_H_

