#include <QDebug>
#include <QFile>
#include <QTextStream>

#include "message.h"


Message::Message(QObject *parent) : QObject(parent)
{
    initFlagString();
}

void Message::initFlagString() {
    m_mapPackage[INQUIRY] = "INQUIRY";
    m_mapPackage[PLAN] = "PLAN";
    m_mapPackage[SONICATION] = "SONICATION";
    m_mapPackage[FEEDBACK] = "FEEDBACK";

    m_mapData[NETWORK] = "NETWORK";
    m_mapData[NETWORK_CONNECTED] = "NETWORK_CONNECTED";
    m_mapData[NETWORK_DISCONNECTED] = "NETWORK_DISCONNECTED";
    m_mapData[POWER_AMPILIFIERS] = "POWER_AMPILIFIERS";
    m_mapData[POWER_AMPLIFIERS_AVAILABLE] = "POWER_AMPLIFIERS_AVAILABLE";
    m_mapData[POWER_AMPLIFIERS_NONAVAILABLE] = "POWER_AMPLIFIERS_NONAVAILABLE";
    m_mapData[PHASE_CONTROLLER] = "PHASE_CONTROLLER";
    m_mapData[PHASE_CONTROLLER_AVAILABLE] = "PHASE_CONTROLLER_AVAILABLE";
    m_mapData[PHASE_CONTROLLER_NONAVAILABLE] = "PHASE_CONTROLLER_NONAVAILABLE";
    m_mapData[HIFU_DRIVER] = "HIFU_DRIVER";
    m_mapData[HIFU_DRIVER_AVAILABLE] = "HIFU_DRIVER_AVAILABLE";
    m_mapData[HIFU_DRIVER_NONAVAILABLE] = "HIFU_DRIVER_NONAVAILABLE";
    m_mapData[IMAGE_GRABBER] = "IMAGE_GRABBER";
    m_mapData[IMAGE_GRABBER_AVAILABLE] = "IMAGE_GRABBER_AVAILABLE";
    m_mapData[IMAGE_GRABBER_NONAVAILABLE] = "IMAGE_GRABBER_NONAVAILABLE";
    m_mapData[WATER_CYCLE] = "WATER_CYCLE";
    m_mapData[WATER_CYCLE_AVAILABLE] = "WATER_CYCLE_AVAILABLE";
    m_mapData[WATER_CYCLE_NONAVAILABLE] = "WATER_CYCLE_NONAVAILABLE";
    m_mapData[WATER_CYCLE_LEVEL_FULL] = "WATER_CYCLE_LEVEL_FULL";
    m_mapData[WATER_CYCLE_LEVEL_HALF] = "WATER_CYCLE_LEVEL_HALF";
    m_mapData[WATER_CYCLE_LEVEL_NO] = "WATER_CYCLE_LEVEL_NO";
    m_mapData[PLAN_READY] = "PLAN_READY";
    m_mapData[PLAN_NOT_READY] = "PLAN_NOT_READY";
    m_mapData[SONICATION_GET_READY] = "SONICATION_GET_READY";
    m_mapData[SONICATION_START] = "SONICATION_START";
    m_mapData[SONICATION_STOP] = "SONICATION_STOP";
    m_mapData[POWER_AMPLIFIERS_READY] = "POWER_AMPLIFIERS_READY";
    m_mapData[POWER_AMPLIFIERS_NOT_READY] = "POWER_AMPLIFIERS_NOT_READY";
    m_mapData[SONICATION_STARTED] = "SONICATION_STARTED";
    m_mapData[SONICATION_STOPPED] = "SONICATION_STOPPED";
    m_mapData[SPOT_COMPLETED] = "SPOT_COMPLETED";
    m_mapData[PLAN_COMPLETED] = "PLAN_COMPLETED";
}

void Message::writeLog(QString log) {
    QFile file(LOG_PATH);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "Cannot find the log file";
        return;
    }

    QTextStream out(&file);
    out << log << "\n";
    file.close();
}

void Message::work(PackageFlag pFlag, DataFlag dFlag) {
    QString str = mapFromPackage(pFlag) + ' ' + mapFromData(dFlag);
    writeLog(str);
    switch (pFlag) {
    case INQUIRY: handleInquiry(dFlag);
    case SONICATION: handleSonication(dFlag);
    case FEEDBACK: handleFeedback(dFlag);
    case TEST: break;
    default: break;
    }
}

void Message::handleInquiry(DataFlag dFlag) {
    switch (dFlag) {
    case NETWORK_CONNECTED:
    case NETWORK_DISCONNECTED:
    case POWER_AMPLIFIERS_AVAILABLE:
    case POWER_AMPLIFIERS_NONAVAILABLE:
    case PHASE_CONTROLLER_AVAILABLE:
    case PHASE_CONTROLLER_NONAVAILABLE:
    case HIFU_DRIVER_AVAILABLE:
    case HIFU_DRIVER_NONAVAILABLE:
    case IMAGE_GRABBER_AVAILABLE:
    case IMAGE_GRABBER_NONAVAILABLE:
    case WATER_CYCLE_NONAVAILABLE:
    case WATER_CYCLE_LEVEL_FULL:
    case WATER_CYCLE_LEVEL_HALF:
    case WATER_CYCLE_LEVEL_NO: emit statusUpdated(dFlag);
    case POWER_VOLTAGE_ERROR: emit statusUpdated(dFlag);
    default: break;
    }
}

void Message::handleSonication(DataFlag dFlag) {
    switch (dFlag) {
    case PLAN_READY:
    case PLAN_NOT_READY:
    case POWER_AMPLIFIERS_READY:
    case POWER_AMPLIFIERS_NOT_READY:
    case SONICATION_STARTED:
    case SONICATION_STOPPED:
    case PHASE_CONTROLLER_DISABLED:
    case PHASE_CONTROLLER_ENABLED:
        emit statusUpdated(dFlag);
    default: break;
    }
}

void Message::handleFeedback(DataFlag dFlag) {
    switch (dFlag) {
    case SPOT_COMPLETED:
    case PLAN_COMPLETED: emit statusUpdated(dFlag);
    default: break;
    }
}
