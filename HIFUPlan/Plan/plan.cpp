#include <QDataStream>
#ifdef DEBUG
#include <QDebug>
#endif

#include "variable.h"
#include "plan.h"

Q_LOGGING_CATEGORY(HIFUPLAN, "HIFU Plan")

Plan::Plan(QObject *parent) : QObject(parent),
    m_available(false)
{
}

void Plan::spotCoord(double coord[]){
    qCDebug(HIFUPLAN()) << m_spotXCoords.size() << " spot(s) in x.";
    qCDebug(HIFUPLAN()) << m_spotYCoords.size() << " spot(s) in y.";
    qCDebug(HIFUPLAN()) << m_spotZCoords.size() << " spot(s) in z.";
    if (m_spotXCoords.size() &&
        m_spotYCoords.size() &&
        m_spotZCoords.size()) {
        coord[0] = m_spotXCoords.dequeue();
        coord[1] = m_spotYCoords.dequeue();
        coord[2] = m_spotZCoords.dequeue();
    } else {
        // TODO: Handle this situation
        setAvailable(false);
        qCDebug(HIFUPLAN()) << "No available spot!";
    }
}

int Plan::spotPulseCount(){
    // Bug: pop an element when the queue is empty
    // Status: fixed use the if-condition to ensure the queue is not empty
    if (!m_spotPulseCount.isEmpty()) {
        int count = m_spotPulseCount.dequeue();
        qDebug()<<"dequeue spot pulse count is "<<count;

        count--;
        // Update count after completing a pulse
        if (count) m_spotPulseCount.push_front(count);
        // Bug: a pulse more than set
        // Status: fixed
        qDebug()<<"push front count is "<<count;
        return count;
    }

    return 0;
}

// This function can be called if loadSpotCoords() and loadSpotParams() have been invoked to produce a new plan,
// otherwise return an empty ba.
void Plan::serialize(QByteArray &ba)
{
    if (!available()) return;

    QDataStream out(&ba, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_4);

    qCDebug(HIFUPLAN()) << "Serializing ...";
    out << (qint64)0
        << (qint64)0
        << m_spotFocusCount
        << m_spotXCoords
        << m_spotYCoords
        << m_spotZCoords
        << m_spotVoltage
        << m_spotPulseOn
        << m_spotPulseOff
        << m_spotPulseCount
        << m_spotCoolingTime
        << (qint64)TERMINATOR;

    qDebug() << "Size of bytes:" << ba.size();
    if (out.device()->seek(0))
    {
        out << (qint64)PLAN << (qint64)ba.size();
        qDebug()<<"add PLAN after ba size !"<< ba.size();
    }
    else {
        // TODO: Handle this situation
        qCDebug(HIFUPLAN()) << "Error in serializing!";
        ba.clear();
        return;
    }
    qCDebug(HIFUPLAN()) << "Serialization completed!";
}

// The function of deserialize() can be used in server end. The received bytes(ba) are converted into the plan parameters.
// If ba is empty, return directly.
void Plan::deserialize(QByteArray &ba) {
    if (ba.isEmpty()) {
        emit statusUpdated(SONICATION, PLAN_NOT_READY);
        qCDebug(HIFUPLAN()) << "The byte array is empty.";
        return;
    }

    QDataStream in(&ba, QIODevice::ReadOnly);
    in.setVersion(QDataStream::Qt_5_4);
    qCDebug(HIFUPLAN()) << "Deserializing...";
    qint64 flag, baSize, terminator;
    QQueue<int> focusCount, pulseCount;
    QQueue<double> x, y, z;
    double volt; int pulseOn, pulseOff, coolingTime;
    in >> flag
       >> baSize
       >> focusCount
       >> x
       >> y
       >> z
       >> volt
       >> pulseOn
       >> pulseOff
       >> pulseCount
       >> coolingTime
       >> terminator;
    qDebug() <<"flag in plan deserialize is "<< flag;
    qDebug() << "Terminator in plan:"        << terminator;
    qDebug() <<"set dequeue spot pulse is "  << pulseCount;
    qDebug() <<"volt is "         << volt;
    qDebug() <<"pulse on is  "    << pulseOn;
    qDebug() <<"pulse off is "    << pulseOff;
    qDebug() <<"cooling time is " << coolingTime;
    if (terminator == TERMINATOR) {
        loadSpotCoords(focusCount, x, y, z);
        loadSpotParams(volt, pulseOn, pulseOff, coolingTime, pulseCount);
        setAvailable(true);
        emit statusUpdated(SONICATION, PLAN_READY);
    } else {
        qCDebug(HIFUPLAN()) << "TERMINATOR:" << terminator;
        // TODO: Handle this situation
        setAvailable(false);
        emit statusUpdated(SONICATION, PLAN_NOT_READY);
        qCDebug(HIFUPLAN()) << "Error in deserializing!";
        return;
    }
    qCDebug(HIFUPLAN()) << "Deserialization completed!";
}

// If loadSpotCoords() and loadSpotParams() are called in client end, setAvailable(true) should be also called to
// indicate the plan is available.
void Plan::loadSpotCoords(QQueue<int>& count, QQueue<double>& x, QQueue<double>& y, QQueue<double>& z) {
    setSpotFocusCount(count);
    setSpotXCoords(x);
    setSpotYCoords(y);
    setSpotZCoords(z);
}

void Plan::loadSpotParams(double volt, int onTime, int offTime, int coolingTime, QQueue<int>& count) {
    setVoltage(volt);
    setPulseOn(onTime);
    setPulseOff(offTime);
    setCoolingTime(coolingTime);
    setSpotPulseCount(count);
}

// Called in both serialize and deserialize
void Plan::writeLog() {
    // TODO: Write the spot coordinates and parameters into the logging file
}
