#ifndef PHASECONTROLLER_H
#define PHASECONTROLLER_H

#include <QObject>
#include <QLoggingCategory>
#include <QtSerialPort/QSerialPort>

#include "constant.h"
#include "variable.h"
#include "phasecontroller_global.h"

typedef unsigned char  uint8;
typedef unsigned short uint16;
typedef unsigned int  uint32;

Q_DECLARE_LOGGING_CATEGORY(PC)

class PHASECONTROLLERSHARED_EXPORT PhaseController : public QObject
{

    Q_OBJECT

public:
    PhaseController(QObject *parent=0);
    ~PhaseController();

    void init();
    inline bool exist();

    // Write and load the data of channel and phase
    void updatePhase(uint8 channel, uint8 phase);
    void updatePhaseE(uint8 channel, uint8 phase, bool phase_enable);
    // Enable and disable the output of the set phase signal
    void enable();
    void disable();
    void updateTime(uint16 time, uint ratio);
    bool updatePlan(uint time,uint ontime,uint count);
    void clearPlan();

signals:
    void statusUpdated(PackageFlag, DataFlag);
    void PointCompleted();
    void PhaseSendError();

private:

    void loadSettings();
    QSerialPort* m_serial;
    bool receiveSerial();
    bool openSerial();
    void setSerial();
    bool checkSerial();
    void sendSerial();
    void Delay_MSec(unsigned int msec);
    unsigned char chSend[4],chRead[32];
    QByteArray baSend, baRead, m_readdata, m_senddata;
    QString m_Port;
    bool b_Disable_Slot;
    int PhaseReadCount;
    bool sendEndFlag,sendStartFlag;

private slots:
    void ReadData();

public slots:
    void close();
};
inline bool PhaseController::exist() { return m_serial == NULL ? false:true; }

#endif // PHASECONTROLLER_H
