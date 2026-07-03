#include "irradiatecore.h"

#include <QMessageBox>
#include <qabstractbutton.h>
#include <QTimer>

#include "irradiatespot.h"
#include "irradiateplan.h"
#include "plan.h"
#include "message.h"
#include "append.h"
#include "servicebuffer.h"
#include "PhaseCalculatorN.c"

#include "constant.h"
#if _MSC_VER >1600
#pragma execution_character_set("utf-8")
#endif

Q_LOGGING_CATEGORY(IRRADIATECORE,"IRRADIATECORE")

IrradiateCore::IrradiateCore(IrradiatePlan* plan,Append* append,
                             ServiceBuffer* schedule,Plan* planInfo,
                             Message* message)
{
    _pMessage = NULL;

    _status = STATUS_UNKNOWN;
    _irradiatePlan = plan;
    _append = append;
    _schedule = schedule;
    _plan = planInfo;
    _msgCtrl = message;
    _isPause = false;
    _isIrradiating = false;
    _AimUSAngle = -100;
    // 实际超声角度
    _CurrentUSAngle = -100;

    initCooling();

    connect(_msgCtrl, SIGNAL(statusUpdated(DataFlag)),this,SLOT(statusChanged(DataFlag)));
    connect(_append,SIGNAL(switchChanged(bool)),this,SLOT(switchChanged(bool)));
}

IrradiateCore::~IrradiateCore()
{
    if (_pMessage!=NULL)
    {
        delete _pMessage;
        _pMessage = NULL;
    }

    // 冷却时间定时器，单辐照点发送计划，驱动模块忽略最后一个点冷却时间
    if (_coolingTimer!=NULL)
    {
        delete _coolingTimer;
        _coolingTimer = NULL;
    }

    if (_progressTimer!=NULL)
    {
        delete _progressTimer;   // 冷却进度条刷新定时器
        _progressTimer = NULL;
    }
}

void IrradiateCore::initPA()
{
    m_PA = new PowerAmp;
    if(m_PA->exist())
    {
        b_PA = true;
        return;
    }
    else
    {
        b_PA = false;
        return;
    }
}

void IrradiateCore::initPC()
{
    m_PC = new PhaseController;
    if(m_PC->exist())
    {
        b_PC = true;
        return;
    }
    else
    {
        b_PC = false;
        return;
    }
}

// 冷却定时器超时
void IrradiateCore::on_cooling_timeout()
{

    // 停止定时器
    if (_coolingTime > 0)
    {
        _coolingTimer->stop();
        _progressTimer->stop();
    }

    // 同步进度条
    emit setProgressRate(100);

    // 设置剩余冷却时间为0
    emit setRemainCoolingTime(0);

    // 设置辐照参数表格颜色
    emit setTableRowColor(_spotId,COMPLETE_COLOR);
    // 设置图像辐照点颜色
    emit setSpotColor(_spotBornId,COMPLETE_COLOR);

    // 确认开始下一个辐照点
    emit moveToNextSpot();
}

// 进度条更新定时器超时
void IrradiateCore::on_progress_timeout()
{
    ++_coolingTimeCount;
    emit setProgressRate(_coolingTimeCount*100/_coolingTime);

    emit setRemainCoolingTime(_coolingTime-_coolingTimeCount);
}

// 初始化冷却相关
void IrradiateCore::initCooling()
{
    if (!_irradiatePlan->getCoolingTime(_coolingTime))
    {
        qCDebug(IRRADIATECORE()) << IRRADIATECORE().categoryName()
                                 << "get cooling time failed !"
                                 << TRACE_CMH();

        _coolingTime = 0;
    }


//    if (_coolingTime < 1)
//    {
//        _coolingTimer = NULL;
//        _progressTimer = NULL;
//    }
//    else
//    {
        _coolingTimeCount = 0;
        _coolingTimer = new QTimer(this);
        _coolingTimer->setInterval(_coolingTime*MILLISECOND_UNIT);
        _coolingTimer->setSingleShot(true);
        connect(_coolingTimer,SIGNAL(timeout()),this,SLOT(on_cooling_timeout()));

        _progressTimer = new QTimer(this);
        _progressTimer->setInterval(MILLISECOND_UNIT);
        _progressTimer->setSingleShot(false);
        connect(_progressTimer,SIGNAL(timeout()),this,SLOT(on_progress_timeout()));
//    }
}

// 设置超声探头当前位置
void IrradiateCore::setCurrentUSAngle(int angle)
{
    _CurrentUSAngle = angle;
    if (_schedule!=NULL && _append!=NULL
        && isUSReady()
        && _status == STATUS_READY
        && _append->isSwitchOn())
    {
        _schedule->onServiceCalled(SONICATION, SONICATION_START);
        emit setSpotColor(_spotBornId,IRADIATING_COLOR);

        emit setTableRowColor(_spotId,IRADIATING_COLOR);

        _currentCompleteMap.insert(_spotId,true);
        _append->setYellowOn();
        emit setLighting(LABEL_YELLOW,true);
    }
}

bool IrradiateCore::sendSpotParameter()
{

    if (_irradiatePlan==NULL)
    {
        qCDebug(IRRADIATECORE()) << IRRADIATECORE().categoryName()
                                 << "load spot irradiate plan null !"
                                 << TRACE_CMH();
        return false;
    }

    Irradiate_Spot spot;
    if (_irradiatePlan->getIrradiateSpot(_currentCompleteMap,spot))
    {
        double coord[3];
        QQueue<double> x, y, z;
        QQueue<int> focusCount, pulseCount;
        int onTime, offTime, coolingTime,period;
        double voltage;
        uint8 phase[144];
        x << spot.position.x;
        y << spot.position.y;
        z << spot.position.z;
        coord[0] = spot.position.x;
        coord[1] = spot.position.y;
        coord[2] = spot.position.z;
        PhaseCalculatorN(freq[0],coord,grade[0],phase);

        for(int i=0;i<144;i++)
        {
            m_PC->updatePhase(i,phase[i]);
        }

        voltage = spot.voltage;
        onTime = spot.onTime;
        offTime = spot.offTime;
        period = onTime+offTime;
        focusCount << spot.focusCount;
        pulseCount << spot.pulseCount;

        // 记录治疗过程中参数
        outputTreatmentLog(QString::number(spot.position.x),
                           QString::number(spot.position.y),
                           QString::number(spot.position.z),
                           QString::number(voltage),
                           QString::number(onTime),
                           QString::number(offTime),
                           QString::number(spot.focusCount),
                           QString::number(spot.pulseCount),
                           QString::number(spot.coolingTime));

        coolingTime = spot.coolingTime;
        if (_coolingTimer!=NULL)
        {
            // 重新设置当前点冷却时间
            _coolingTime = coolingTime;
            _coolingTimer->setInterval(coolingTime*MILLISECOND_UNIT);
        }

        _plan->loadSpotCoords(focusCount, x, y, z);
        _plan->loadSpotParams(voltage, onTime, offTime, coolingTime, pulseCount);
        _plan->setAvailable(true);

        _spotId = spot.id;
        _spotBornId = spot.idBorn;

        QByteArray ba;
        _plan->serialize(ba);
        qDebug()<<"before send irrdata";
        _schedule->onServiceCalled(ba);
        qDebug()<<"after send irrdata";
        _schedule->onServiceCalled(SONICATION, SONICATION_GET_READY);

        emit synImageAngle(spot.usAngle);
        // 记录辐照点超声角度
        _AimUSAngle = spot.usAngle;

        _status = STATUS_SENDED_PARAMETER;
        // 发送辐照参数后设置进度条进度为0
        emit setProgressRate(0);


        return true;
    }
    else
    {
        QMessageBox message("No Sonication Points Available",
                            "请确认当前计划是否辐照完成",
                            QMessageBox::Warning,
                            QMessageBox::Yes | QMessageBox::Default,
                            QMessageBox::No | QMessageBox::Escape,
                            QMessageBox::NoButton);

        message.setGeometry(QRect(1400,400,message.width(),message.height()));
        int ret = message.exec();

        return false;
    }
}

// 更新当前辐照过程状态
void IrradiateCore::statusChanged(DataFlag flag)
{
    switch(flag)
    {
    // 下发治疗参数成功，工控机已经准备好
    case PLAN_READY:
        qDebug()<<"plan ready !";
        _status = STATUS_READY;
        readyFire();
        break;

    // 治疗已经开始
    case  SONICATION_STARTED:
        qCDebug(IRRADIATECORE()) << IRRADIATECORE().categoryName()
                                 << "sonication started "
                                 << TRACE_CMH();

        _status = STATUS_STARTED;
        switchQuickOff();

        break;

    // 治疗计划被终止
    case SONICATION_STOPPED:
        qCDebug(IRRADIATECORE()) << IRRADIATECORE().categoryName()
                                 << "sonication stoped "
                                 << TRACE_CMH();

        _status = STATUS_STOPPED;
        if (_isPause)
        {
           onPlanComplete();
        }
        else
        {
            // 设置治疗控件可用
            emit setTreatmentControlEnable();
        }

        break;

    case SPOT_COMPLETED :
        qCDebug(IRRADIATECORE()) << IRRADIATECORE().categoryName()
                                 << "sonication completed "
                                 << TRACE_CMH();

        _status = STATUS_SPOT_COMPLETED;
        break;

    // 一个点治疗计划完成
    case PLAN_COMPLETED:
        _status = STATUS_PLAN_COMPLETED;

        qCDebug(IRRADIATECORE()) << IRRADIATECORE().categoryName()
                                 << "plan completed !"
                                 << TRACE_CMH();
        // 任然继续辐照
        if (_isIrradiating)
        {
           onPlanComplete();
        }
        else
        {
            // 设置治疗控件可用
            emit setTreatmentControlEnable();
        }
        break;

    default:
        break;
    }
}

// 接收到辐照计划完成的消息
void IrradiateCore::onPlanComplete()
{
    if (_coolingTime<1)
    {
        on_cooling_timeout();
    }
    else
    {
        _coolingTimer->start();
        _progressTimer->start();
        _coolingTimeCount = 0;
    }
}

void IrradiateCore::readyFire()
{
    if (_append!=NULL)
    {
        if (_append->isSwitchOn())
        {
            if (_schedule!=NULL)
            {
                if (isUSReady())
                {
                    _schedule->onServiceCalled(SONICATION, SONICATION_START);
                    emit setSpotColor(_spotBornId,IRADIATING_COLOR);
                    emit setTableRowColor(_spotId,IRADIATING_COLOR);

                    _currentCompleteMap.insert(_spotId,true);
                    _append->setYellowOn();
                    emit setLighting(LABEL_YELLOW,true);
                }

            }
        }
        else
        {
            // 脚踏开关未闭合
            if (_pMessage==NULL)
            {
                _pMessage = new QMessageBox("Foot Switch Status",
                                            "是否忘记踩下脚踏开关?",
                                            QMessageBox::Question,
                                            QMessageBox::Yes | QMessageBox::Default,
                                            QMessageBox::No | QMessageBox::Escape,
                                            QMessageBox::NoButton);


                _pMessage->setWindowFlags(Qt::WindowTitleHint|Qt::CustomizeWindowHint);
                _pMessage->setGeometry(QRect(1400,400,_pMessage->width(),_pMessage->height()));

                _pMessage->button(QMessageBox::StandardButton::Yes)->setToolTip("Confirm Sonication");
                _pMessage->button(QMessageBox::StandardButton::No)->setToolTip("Cancel Sonication");

                connect(_pMessage->button(QMessageBox::StandardButton::Yes),SIGNAL(clicked()),this,SLOT(yesButtonDown()));
                connect(_pMessage->button(QMessageBox::StandardButton::No),SIGNAL(clicked()),this,SLOT(noButtonDown()));
//                connect(_pMessage,SIGNAL(rejected()),this,SLOT(noButtonDown()));
            }

            // 用信号槽函数自动执行
            int ret = _pMessage->exec();
        }
    }
}

void IrradiateCore::yesButtonDown()
{
    if (_append==NULL)
    {
        qCDebug(IRRADIATECORE()) << IRRADIATECORE().categoryName()
                                 << "_append pointer null !"
                                 << TRACE_CMH();
        return;
    }
    qDebug()<<"闭合了-yesButtonDown";
    if (_append->isSwitchOn())
    {
        qDebug()<<"闭合了-append->isSwitchOn()";
        if (_schedule!=NULL)
        {
            // 超声探头是否转到位
            if (isUSReady()&&(_status==STATUS_READY))
            {
                _schedule->onServiceCalled(SONICATION, SONICATION_START);
                // 设置当前辐照点颜色
                emit setSpotColor(_spotBornId,IRADIATING_COLOR);
                emit setTableRowColor(_spotId,IRADIATING_COLOR);

                if (_currentCompleteMap.find(_spotId)==_currentCompleteMap.end())
                {
                    _currentCompleteMap.insert(_spotId,true);
                }

                _append->setYellowOn();
                emit setLighting(LABEL_YELLOW,true);
            }
            else if(!isUSReady()&&_status==STATUS_READY)
            {
                emit synImageAngle(_AimUSAngle);
            }
        }
    }
    else
    {
        // 脚踏开关未闭合
        if (_pMessage==NULL)
        {
            _pMessage = new QMessageBox("Foot Switch Status",
                                        "是否忘记踩下脚踏开关?",
                                        QMessageBox::Question,
                                        QMessageBox::Yes | QMessageBox::Default,
                                        QMessageBox::No | QMessageBox::Escape,
                                        QMessageBox::NoButton);

            _pMessage->setWindowFlags(Qt::WindowTitleHint|Qt::CustomizeWindowHint);
            _pMessage->setGeometry(QRect(1400,400,_pMessage->width(),_pMessage->height()));
            _pMessage->button(QMessageBox::StandardButton::Yes)->setToolTip("Confirm Sonication");
            _pMessage->button(QMessageBox::StandardButton::No)->setToolTip("Cancel Sonication");

            connect(_pMessage->button(QMessageBox::StandardButton::Yes),SIGNAL(clicked()),this,SLOT(yesButtonDown()));
            connect(_pMessage->button(QMessageBox::StandardButton::No),SIGNAL(clicked()),this,SLOT(noButtonDown()));
//             connect(_pMessage,SIGNAL(rejected()),this,SLOT(noButtonDown()));

        }

        // 用信号槽函数自动执行
        int ret = _pMessage->exec();
    }
}

void IrradiateCore::noButtonDown()
{
    // 治疗停止
    if (_schedule!=NULL)
    {
        _schedule->onServiceCalled(SONICATION,SONICATION_STOP);
        emit setIrradiatingFlag(false);
        _isPause = false;

        if (_spotId!="")
        {
            _spotId = "";
            _spotBornId = "";
        }
    }

    // 设置治疗相关按钮可用
    // emit setTreatmentControlEnable();
    if (_append!=NULL)
    {
        _append->setYellowOff();
        emit setLighting(LABEL_YELLOW,false);
    }

    return;
}

void IrradiateCore::switchQuickOff()
{
    if (_append!=NULL)
    {
        if (!_append->isSwitchOn())
        {
            qCDebug(IRRADIATECORE()) << IRRADIATECORE().categoryName()
                                     << "switch off and quick off !"
                                     << TRACE_CMH();

            if (_schedule!=NULL)
            {
                _schedule->onServiceCalled(SONICATION, SONICATION_STOP);
                _isPause = true;
                if (_append!=NULL)
                {
                    _append->setYellowOff();
                    emit setLighting(LABEL_YELLOW,false);
                }
            }
        }
    }

}

void IrradiateCore::setIrradiating(bool b)
{
    _isIrradiating = b;
}

void IrradiateCore::setIsPause(bool b)
{
    _isPause = b;
    // 设置辐照参数表格颜色为已辐照完成
    emit setTableRowColor(_spotId,COMPLETE_COLOR);
    // 设置图像辐照点颜色为已完成
    emit setSpotColor(_spotBornId,COMPLETE_COLOR);
    // 辐照停止则停止冷却和进度定时器
    if (_coolingTimer!=NULL && _coolingTimer->isActive())
    {
        _coolingTimer->stop();
    }

    if (_progressTimer!=NULL && _progressTimer->isActive())
    {
        _progressTimer->stop();
    }
}

// 响应脚踏开关状态变化槽函数
void IrradiateCore::switchChanged(bool flag)
{
    if (!flag)
    {
        // 脚踏开关断开

            // 开始辐照则辐照停止
        if (_schedule!=NULL)
        {
            qDebug()<<"status is "<<_status;

            if (_status==STATUS_STARTED)
            {
                _schedule->onServiceCalled(SONICATION, SONICATION_STOP);
                _isPause = true;
                if (_append!=NULL)
                {
                    _append->setYellowOff();
                    emit setLighting(LABEL_YELLOW,false);
                }
            }
            else
            {
                qDebug() << "status is "<<_status
                         << "switch off but sonication not stop !"
                         << TRACE_CMH();
            }
        }

        emit setLighting(SWITCH,false);
    }
    else
    {
        qDebug()<<"闭合了";
        // 脚踏开关闭合
        emit setLighting(SWITCH,true);
        if (_schedule!=NULL)
        {
            if (_status==STATUS_READY)
            {
                qDebug()<<"闭合了-status==STATUS_READY";
                // 发送完治疗参数,工控机准备完成
                yesButtonDown();

                if (_pMessage!=NULL)
                {
                    _pMessage->close();
                }
            }
        }
    }
}

