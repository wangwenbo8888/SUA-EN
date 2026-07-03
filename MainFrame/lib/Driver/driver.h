#ifndef DRIVER_H
#define DRIVER_H

#include <QQueue>
#include <QTimer>
#include <QLoggingCategory>
#include <QObject>

#include "poweramp.h"
#include "phasecontroller.h"
#include "plan.h"
#include "variable.h"
#include "constant.h"
#include "driver_global.h"

Q_DECLARE_LOGGING_CATEGORY(DRIVER)

enum Driver_Status
{
   STATUS_UNKNOWN = 0,
   STATUS_GET_READY,
   STATUS_STARTED,
   STATUS_STOPPED,
   STATUS_COMPLETED
};

class DRIVERSHARED_EXPORT Driver : public QObject
{
    Q_OBJECT

public:
    Driver(Plan* plan, QObject *parent=0);
    Driver(PowerAmp* pa, PhaseController* pc, Plan* plan, QObject *parent=0);
    ~Driver();

    // If pulseOff is 0 or pulseCount >= 1 in the plan, m_pulseWave is set true, else set false.
    inline void setPulseWave(const bool pulseWave);
    inline bool pulseWave() const;

    inline void setThreadToggle(const bool toggle);
    inline bool threadToggle() const;

public slots:
    void work(PackageFlag, DataFlag);

signals:
    void statusUpdated(PackageFlag, DataFlag);

    void bytesToWrite(QByteArray&);

private slots:
    // Pulse wave
    void onTimerOnPWTimeout();
    // Continuous wave
    void onTimerOnCWTimeout();
    void onTimerOffTimeout();
    void onTimerCoolingTimeout();

private:
    PowerAmp *m_pa; PhaseController *m_pc; Plan *m_plan;
    QTimer m_timerOn, m_timerOff;// m_timerCooling;
    bool m_threadToggle, m_pulseWave;

    Driver_Status m_status;
    int m_frqIndex;

    // Notice: the members are deleted in the class destructor, so the deletion may be execuated twice.
    inline void close() { delete m_pa, m_pa = NULL, delete m_pc, m_pc = NULL; }
    void moveToNextSpot();
    inline void on();
    inline void off();

    void init();
    // 查询功放电压
    void queryAmpVoltage();
    // 查询功放温度
    void queryAmpTemperature();

    inline bool exist();

    void startReady();
    void startSonication();
    void stopSonication();

    void enablePhaseController();
    void disablePhaseController();

    void initTimer();
    void destroyTimer();
};

inline bool Driver::exist() { return m_pa->exist() && m_pc->exist(); }
inline void Driver::setPulseWave(const bool pulseWave) { m_pulseWave = pulseWave; }
inline bool Driver::pulseWave() const { return m_pulseWave; }
inline void Driver::setThreadToggle(const bool toggle) { m_threadToggle = toggle; }
inline bool Driver::threadToggle() const { return m_threadToggle; }

inline void Driver::on() { if (exist()) m_pc->enable(); }
inline void Driver::off() { if (exist()) m_pc->disable(); }

#endif // DRIVER_H
