

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
    STATUS_UNKNOWN = 0,             // Status未知
    STATUS_SENDED_PARAMETER,    // 发送了sonication参数
    STATUS_READY,               // PC准备Complete
    STATUS_STARTED,             // 开始sonication
    STATUS_PAUSE,               // sonication被暂停
    STATUS_STOPPED,             // sonication被Stop
    STATUS_SPOT_COMPLETED,
    STATUS_PLAN_COMPLETED            // sonication顺利Complete
};


Q_DECLARE_LOGGING_CATEGORY(IRRADIATECORE)

// sonication过程控制
class IrradiateCore : public QObject
{
    Q_OBJECT

public:

    IrradiateCore(IrradiatePlan* plan,Append* append,
                  ServiceBuffer* schedule,Plan* planInfo,
                  Message* message);

    ~IrradiateCore();

    // 向PC发送Treatment点相关参数
    bool sendSpotParameter();

private slots:

    void setIsPause(bool b);

    // 设置是否正在sonication标志
    void setIrradiating(bool b);
    // Foot Switch槽函数
    void switchChanged(bool flag);
    // PCStatus变化通知消息
    void statusChanged(DataFlag flag);

    // 弹出框yes按钮被按下
    void yesButtonDown();

    // 弹出框no按钮被按下

    void noButtonDown();
    // Cooling定时器超时
    void on_cooling_timeout();
    // 进度条更新定时器超时
    void on_progress_timeout();

    // 设置Ultrasound Probe当前位置
    void setCurrentUSAngle(int angle);

signals:

    // 同步Planning Image与Live Image和sonication的位置Angle一致
    void synImageAngle(int angle);

    // 设置是否在sonication标志
    void setIrradiatingFlag(bool);
    // 设置Treatment相关控件可用
    void setTreatmentControlEnable();
    // 设置与灯和开关对应界面图标Status
    void setLighting(APPEND_LABEL,bool);

    // Confirm开始下一个sonication point
    void moveToNextSpot();

    // 设置进度条进度
    void setProgressRate(int rate);

    // 设置剩余Cooling数值
    void setRemainCoolingTime(int value);

    // 设置sonication参数表格对应颜色
    void setTableRowColor(QString,QColor);
    // 设置sonication point颜色
    void setSpotColor(QString,QColor);

private :

    bool isUSReady()
    {
        return _CurrentUSAngle==_AimUSAngle;
    }

    // Confirm是否开始sonication
    void readyFire();

//    // 接收到sonicationStop的消息
//    void onSonicationStopped();

    // 接收到Sonication PlanComplete的消息
    void onPlanComplete();

    // 处理Foot Switch迅速抬起的情况
    void switchQuickOff();

    // 初始化Cooling及进度条相关
    void initCooling();

    void initPA();
    void initPC();
    QString _spotId;           // 当前sonication pointid
    QString _spotBornId;       // 当前sonication pointborn id
    bool _isPause;             // 是否是暂停
    bool _isIrradiating;       // 是否在sonication
    bool b_PA,b_PC;

    ServiceBuffer* _schedule;
    Message* _msgCtrl;
    Plan* _plan;
    Append* _append;           // Control Box及Foot Switch
    IrradiatePlan* _irradiatePlan;    // 当前Sonication Plan
    Irradiate_Status _status;         // 当前对应sonication pointStatus

    PowerAmp *m_PA;
    PhaseController *m_PC;

    QMessageBox* _pMessage;           // Foot Switch未闭合弹出对话框

    QTimer* _coolingTimer;    // Cooling定时器，单sonication point发送Plan，驱动模块忽略最后一个点Cooling
    QTimer* _progressTimer;   // Cooling进度条Refresh定时器
    int _coolingTime;         // Cooling
    int _coolingTimeCount;    // Cooling
    // Foot Switch导致的Stop认为是暂停，其它认为是Stop

    // 期望超声Angle
    int _AimUSAngle;

    // 实际超声Angle
    int _CurrentUSAngle;

    QMap<QString,bool> _currentCompleteMap;    // 记录Current PlanCompletesonication point信息

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

