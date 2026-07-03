#ifdef DEBUG
#include <QDebug>
#endif

#include <QDataStream>

//#include "phase.h"
#include "driver.h"
#include "phasecalc.h"

Q_LOGGING_CATEGORY(DRIVER, "HIFU Driver")

Driver::Driver(Plan *plan, QObject *parent)
 : QObject(parent),
   m_plan(plan),
   m_frqIndex(0)
{
    m_pa = new PowerAmp;
    // Notice: m_pc should be noticed because this construction may be not useful.
    m_pc = new PhaseController;

    init();
    m_status = STATUS_UNKNOWN;

    // 从配置文件中读取用户模式
    QSettings *settings = new QSettings(SETTINGS_PATH, QSettings::IniFormat);
    m_frqIndex = settings->value("DRIVER/frq_index").toString().toInt();
    qCDebug(DRIVER()) << DRIVER().categoryName()
                         << "set frq index is "<< m_frqIndex
                         << TRACE_CMH();

    delete settings;
}

// This constructor use the references of pa, pc and plan.
// Three private members are the same as the objects outside this class.
Driver::Driver(PowerAmp *pa, PhaseController *pc, Plan *plan, QObject *parent) :
    QObject(parent), m_pa(pa), m_pc(pc), m_plan(plan),
    m_threadToggle(false), m_pulseWave(true), m_frqIndex(0)
{
    init();
#ifdef DEBUG
    qDebug() << "m_pa =" << m_pa << endl
             << "m_pc =" << m_pc << endl
             << "m_plan =" << m_plan;
#endif

    // 从配置文件中读取用户模式
    QSettings *settings = new QSettings(SETTINGS_PATH, QSettings::IniFormat);
    m_frqIndex = settings->value("DRIVER/frq_index").toString().toInt();
    qCDebug(DRIVER()) << DRIVER().categoryName()
                         << "set frq index is "<< m_frqIndex
                         << TRACE_CMH();

    delete settings;
}

Driver::~Driver() { close(); }

// 查询功放板电压
void Driver::queryAmpVoltage()
{
    if (m_pa->exist())
    {
        QByteArray ba;
        QDataStream out(&ba, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_5_4);
        out << (qint64)0
            << (qint64)0
            << (qint64)POWER_AMPLIFIERS_VOLTAGE;
        for (int i = 1; i <= AMP_COUNT; ++i)
        {
            double value = 0.0;
            if (m_pa->fetch(value,i,PowerAmp::FETCH_VOLTAGE))
            {
            }
            else
            {
                qDebug()<<"fetch amp voltage failed ! channel "<< i;
            }
            out << value;
        }

        out << (qint64)TERMINATOR;

        if (out.device()->seek(0))
        {
            out << (qint64)INQUIRY << (qint64)ba.size();
            qDebug()<<"add PLAN after ba size !"<< ba.size();
            emit bytesToWrite(ba);
        }
        else
        {
            // TODO: Handle this situation
            qDebug() << "Error in serializing amp votage !";
            ba.clear();
            return;
        }
    }
    else
    {
        qDebug()<<"fetch power amp connect failed ! ";
    }
}

// 查询功放温度
void Driver::queryAmpTemperature()
{
    if (m_pa->exist())
    {
        QByteArray ba;
        QDataStream out(&ba, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_5_4);
        out << (qint64)0
            << (qint64)0
            << (qint64)POWER_AMPLIFIERS_TEMPATURE;
        for (int i = 1; i <= AMP_COUNT; ++i)
        {
            double value = 0.0;
            if (m_pa->fetch(value,i,PowerAmp::FETCH_TEMPERATURE))
            {
            }
            else
            {
                qDebug()<<"fetch amp tempature failed ! channel "<< i;
            }
            out << value;
        }

        out << (qint64)TERMINATOR;

        if (out.device()->seek(0))
        {
            out << (qint64)INQUIRY << (qint64)ba.size();
            qDebug()<<"add PLAN after ba size !"<< ba.size();
            emit bytesToWrite(ba);
        }
        else
        {
            // TODO: Handle this situation
            qDebug() << "Error in serializing amp tempature !";
            ba.clear();
            return;
        }

    }
    else
    {
        qDebug()<<"fetch power amp connect failed ! ";
    }
}

void Driver::init(){
    if (exist()) { emit statusUpdated(INQUIRY, HIFU_DRIVER_AVAILABLE);
#ifdef DEBUG
        qDebug() << "Driver exists.";
#endif
    } else { emit statusUpdated(INQUIRY, HIFU_DRIVER_NONAVAILABLE);
#ifdef DEBUG
        qDebug() << "Driver does not exists.";
#endif
    }
}

void Driver::moveToNextSpot()
{
    if (m_plan->completed())
    {
        qCDebug(DRIVER()) << "All the spots have been sonicated.";
        m_plan->setAvailable(false);
        emit statusUpdated(FEEDBACK, PLAN_COMPLETED);
        destroyTimer();
#ifndef NO_HARDWARE
        qDebug()<<"step 3 begin pa reset !";
        if (!m_pa->reset())
        {
            qDebug()<<"step 3 pa reset failed one !";
        }
        qDebug()<<"step 3 pa reset completed !";
#endif
    } else {
#ifdef DEBUG
        qDebug() << "Move to next spot";
#endif
        int count = m_plan->spotFocusCount();
        double coord[3];
        m_plan->spotCoord(coord);
        uint8_T phase[AMP_COUNT];
        int size[1];
        qCDebug(DRIVER()) << "The coordinate of current spot: ("
                          << coord[0] << "," << coord[1] << "," << coord[2] << ")";
        // PhaseCalculator(count, coord, phase, size);
        PhaseCalculatorN(m_frqIndex,coord,phase);
        // Phase array started at 0
        for (uint8_T i=0; i<AMP_COUNT; ++i) {
#ifndef NO_HARDWARE
            m_pc->updatePhase(i, phase[i]);
#endif

#ifdef DEBUG
            qDebug() << "Channel:" << i << ", phase:" << phase[i];
#endif
        }
    }
}

void Driver::onTimerOnPWTimeout() {
#ifdef DEBUG
    qDebug() << "PW on timeout is "<< QDateTime::currentDateTime().toString("HH:mm:ss.zzz");
#endif

#ifndef NO_HARDWARE
    off();
    qDebug() << "PW off is "<< QDateTime::currentDateTime().toString("HH:mm:ss.zzz");
#endif
    m_timerOff.start();
    qDebug() << "PW off time off start "<< QDateTime::currentDateTime().toString("HH:mm:ss.zzz");
}

void Driver::onTimerOnCWTimeout() {

#ifdef DEBU
    qDebug() << "CW on timeout";
#endif

    qDebug()<<"spot pulse count is "<<m_plan->spotPulseCount();
    if (m_plan->spotPulseCount()) m_timerOn.start();
    else {
        m_status = STATUS_STOPPED;
        emit statusUpdated(FEEDBACK, SPOT_COMPLETED);

#ifndef NO_HARDWARE
        off();
#endif
        // int coolingTime = m_plan->coolingTime() * MILLISECOND_UNIT;
        // m_timerCooling.start(coolingTime);
        qCDebug(DRIVER()) << "Sonication is stopped.";
        moveToNextSpot();
    }
}

void Driver::onTimerOffTimeout(){
#ifdef DEBUG
        qDebug() << "Off timeout is "<<QDateTime::currentDateTime().toString("HH:mm:ss.zzz");
#endif

    if (m_plan->spotPulseCount()) {
#ifndef NO_HARDWARE
        on();
#endif
        m_timerOn.start();
    } else
    {
        qDebug()<<"step 1 spot completed !";
        emit statusUpdated(FEEDBACK, SPOT_COMPLETED);
        m_status = STATUS_STOPPED;

#ifndef NO_HARDWARE
        qDebug()<<"step 2 pc disable !";
        off();
#endif
        // int coolingTime = m_plan->coolingTime() * MILLISECOND_UNIT;
        // m_timerCooling.start(coolingTime);
        qDebug()<<"before move to next spot !";
        moveToNextSpot();
    }
}

void Driver::onTimerCoolingTimeout() {
#ifndef NO_HARDWARE
    on();
#endif

    m_timerOn.start();

#ifdef DEBUG
    qDebug() << "Cooling timeout";
#endif
}

void Driver::initTimer(){
    if (m_plan->pulseOff() > 0) setPulseWave(true);
    else setPulseWave(false);

    m_timerOff.setSingleShot(true);
    m_timerOff.setInterval(m_plan->pulseOff());
    connect(&m_timerOff, SIGNAL(timeout()), this, SLOT(onTimerOffTimeout()));

    m_timerOn.setSingleShot(true);
    m_timerOn.setInterval(m_plan->pulseOn());
    if (pulseWave()) connect(&m_timerOn, SIGNAL(timeout()), this, SLOT(onTimerOnPWTimeout()));
    else connect(&m_timerOn, SIGNAL(timeout()), this, SLOT(onTimerOnCWTimeout()));

    // m_timerCooling.setSingleShot(true);
    // connect(&m_timerCooling, SIGNAL(timeout()), this, SLOT(onTimerCoolingTimeout()));
}

void Driver::destroyTimer(){
    m_timerOn.stop();
    if (pulseWave()) {
        disconnect(&m_timerOn, SIGNAL(timeout()), this, SLOT(onTimerOnPWTimeout()));
    } else disconnect(&m_timerOn, SIGNAL(timeout()), this, SLOT(onTimerOnCWTimeout()));
    m_timerOff.stop();
    disconnect(&m_timerOff, SIGNAL(timeout()), this, SLOT(onTimerOffTimeout()));
    // m_timerCooling.stop();
    // disconnect(&m_timerCooling, SIGNAL(timeout()), this, SLOT(onTimerCoolingTimeout()));
}

void Driver::startReady()
{
    qDebug()<<"step start ready !";
    if (m_plan->available()) {
        emit statusUpdated(SONICATION, PLAN_READY);
        m_status = STATUS_GET_READY;
#ifndef NO_HARDWARE
        if (m_pa->start(m_plan->voltage())){
            emit statusUpdated(SONICATION, POWER_AMPLIFIERS_READY);
#ifdef DEBUG
            qDebug() << "step Start PA successfully";
#endif
        } else {
            emit statusUpdated(SONICATION, POWER_AMPLIFIERS_NOT_READY);
#ifdef DEBUG
            qDebug() << "step Fail to start PA";
#endif
        }
#endif
    } else {
        qCDebug(DRIVER()) << "step 4 spot available.";
        statusUpdated(PLAN, PLAN_NOT_READY);
    }
}

void Driver::startSonication(){
    if (threadToggle()){
        // TODO: use the thread toggle

    } else {
        // Use the timer toggle
        initTimer();
        moveToNextSpot();
        m_status = STATUS_STARTED;

#ifndef NO_HARDWARE
        on();
#endif
        m_timerOn.start();
        qCDebug(DRIVER()) << "Sonication is started.";
        emit statusUpdated(SONICATION, SONICATION_STARTED);
    }
}

void Driver::stopSonication(){
    // Stop sonication
    qCDebug(DRIVER()) << DRIVER().categoryName()
                      << "DRIVER status is "<<m_status
                      << TRACE_CMH();

    if (m_status>=STATUS_STOPPED)
    {
        // 辐照已经停止或者在停止的过程中
        m_status = STATUS_UNKNOWN;
        return;
    }

    if (threadToggle()) {

    }
    else
    {
        qDebug()<<"Enter stop sonication !";
        m_status = STATUS_STOPPED;
        destroyTimer();
    #ifndef NO_HARDWARE
        qDebug()<<"step stop sonication disable pc !";
        m_pc->disable();
    #endif
        qDebug()<<"step reset pa stop sonication !";
        m_pa->reset();
        emit statusUpdated(SONICATION, SONICATION_STOPPED);
        qCDebug(DRIVER()) << "Sonication is stopped.";
    }
    // Save the current status

}

void Driver::disablePhaseController()
{
    // disable phase controller
    if (threadToggle()) {

    } else {
        m_pc->disable();
        emit statusUpdated(SONICATION, PHASE_CONTROLLER_DISABLED);
        qCDebug(DRIVER()) << "phase control is disabled !";
    }
    // Save the current status
}

void Driver::enablePhaseController()
{
    // enable phase controller
    if (threadToggle())
    {
    } else {
        m_pc->enable();
        emit statusUpdated(SONICATION, PHASE_CONTROLLER_ENABLED);
        qCDebug(DRIVER()) << "phase control is enable !";
    }
    // Save the current status
}

void Driver::work(PackageFlag pFlag, DataFlag dFlag) {
    switch(pFlag) {
    case INQUIRY:
        if (dFlag == HIFU_DRIVER)
        {
            init();
        }
        else if (dFlag == POWER_AMPLIFIERS_VOLTAGE)
        {
            qDebug()<<"Enter query amp voltage !";
            // 查询功率放大器电压
            queryAmpVoltage();
        }
        else if (dFlag == POWER_AMPLIFIERS_TEMPATURE)
        {
            qDebug()<<"Enter query amp tempature !";
            // 查询功率放大器温度
            queryAmpTemperature();
        }
        break;
    case SONICATION:
        if (dFlag == SONICATION_GET_READY) startReady();
        else if (dFlag == SONICATION_START) startSonication();
        else if (dFlag == SONICATION_STOP) stopSonication();
        else if (dFlag == PHASE_CONTROLLER_DISABLE) disablePhaseController();
        else if (dFlag == PHASE_CONTROLLER_ENABLE) enablePhaseController();
        break;
    default:
        // TODO: handle this situation
        break;
    }
}
