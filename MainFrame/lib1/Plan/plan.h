#ifndef PLAN_H
#define PLAN_H

#include <QObject>
#include <QQueue>
#include <QByteArray>
#include <QLoggingCategory>

#include "constant.h"
#include "variable.h"
#include "plan_global.h"

Q_DECLARE_LOGGING_CATEGORY(HIFUPLAN)

class PLANSHARED_EXPORT Plan : public QObject
{
    Q_OBJECT

public:
    Plan(QObject *parent=0);

    // Used at server end, get current values of parameters during sonication
    void spotCoord(double coord[]);
    inline int spotFocusCount();
    int spotPulseCount();
    inline double voltage() const;
    inline int pulseOn() const;
    inline int pulseOff() const;
    inline int coolingTime() const;

    // Used at both server/client ends
    void loadSpotCoords(QQueue<int>& count, QQueue<double>& x, QQueue<double>& y, QQueue<double>& z);
    void loadSpotParams(double volt, int onTime, int offTime, int coolingTime, QQueue<int>& count);

    // Determine whether current plan is available
    inline bool available() const;
    inline void setAvailable(bool available);
    // Reset the current plan
    inline void reset();
    // Used at server end, determine whether the sonication is completed
    inline bool completed();

public slots:
    // Used at client end, convert parameter values into ba which is sent to the server end
    void serialize(QByteArray& ba);
    // Used at server end, convert ba into parameter values for plan generation
    void deserialize(QByteArray& ba);

signals:
    void statusUpdated(PackageFlag, DataFlag);

private:
    // Spot coordinates (x, y, z) in the same plane
    QQueue<double> m_spotXCoords, m_spotYCoords, m_spotZCoords;
    // Focus count of the spot
    QQueue<int> m_spotFocusCount, m_spotPulseCount;
    double m_spotVoltage;
    int m_spotPulseOn, m_spotPulseOff, m_spotCoolingTime;

    bool m_available;

    // Spot coordinates
    inline void setSpotFocusCount(QQueue<int>& count) { m_spotFocusCount = count; }
    inline void setSpotXCoords(QQueue<double>& coords) { m_spotXCoords = coords; }
    inline void setSpotYCoords(QQueue<double>& coords) { m_spotYCoords = coords; }
    inline void setSpotZCoords(QQueue<double>& coords) { m_spotZCoords = coords; }

    // Sonication parameters
    inline void setSpotPulseCount(QQueue<int>& count) { m_spotPulseCount = count; }
    inline void setVoltage(double volt) { m_spotVoltage = volt; }
    inline void setPulseOn(int time) { m_spotPulseOn = time; }
    inline void setPulseOff(int time) { m_spotPulseOff = time; }
    inline void setCoolingTime(int time) { m_spotCoolingTime = time; }

    void writeLog();
};

inline int Plan::spotFocusCount(){ return m_spotFocusCount.size() ? m_spotFocusCount.dequeue():0; }
inline double Plan::voltage() const { return m_spotVoltage; }
inline int Plan::pulseOn() const { return m_spotPulseOn; }
inline int Plan::pulseOff() const { return m_spotPulseOff; }
inline int Plan::coolingTime() const { return m_spotCoolingTime; }

inline bool Plan::available() const { return m_available; }
inline void Plan::setAvailable(bool available) { m_available = available; }
inline void Plan::reset(){ if (available()) setAvailable(false); }
inline bool Plan::completed() { return m_spotPulseCount.size() ? false:true; }

#endif // PLAN_H
