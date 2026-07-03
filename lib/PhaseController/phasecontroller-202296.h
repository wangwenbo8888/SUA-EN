#ifndef PHASECONTROLLER_H
#define PHASECONTROLLER_H

#include <QObject>
#include <QLoggingCategory>

#include "Inc/bdaqctrl.h"
#include "constant.h"
#include "variable.h"
#include "phasecontroller_global.h"

using namespace Automation::BDaq;

Q_DECLARE_LOGGING_CATEGORY(PC)

class PHASECONTROLLERSHARED_EXPORT PhaseController : public QObject
{

    Q_OBJECT

public:
    PhaseController(QString deviceName=DIO_NAME, QObject *parent=0);
    ~PhaseController();

    void init();
    inline bool exist();

    // Write and load the data of channel and phase
    void updatePhase(uint8 channel, uint8 phase);
    void updatePhaseE(uint8 channel, uint8 phase, bool phase_disable);
    // Enable and disable the output of the set phase signal
    inline void enable() { writeByte(PORT_WORK, BYTE_ENABLE); }
    inline void disable() { writeByte(PORT_WORK, BYTE_DISABLE); }

signals:
    void statusUpdated(PackageFlag, DataFlag);

private:
    InstantDoCtrl *m_instantDoCtrl;

    void selectDevice(QString deviceName=DIO_NAME);
    void checkError(ErrorCode errorCode);
    void writeByte(int port, uint8 byte);
    void loadPhase();

private slots:
    void close();
};

//  Move the definition of function outside the class declaration
inline bool PhaseController::exist() { return m_instantDoCtrl == NULL ? false:true; }

#endif // PHASECONTROLLER_H
