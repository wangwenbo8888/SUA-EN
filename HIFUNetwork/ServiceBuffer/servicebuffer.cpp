#include <QDataStream>
#include <exception>
#ifdef DEBUG
#include <QDebug>
#endif

#include "constant.h"

#include "servicebuffer.h"

Q_LOGGING_CATEGORY(HIFU_SERVICE, "HIFU Service")

ServiceBuffer::ServiceBuffer(QObject *parent) : QObject(parent)
{
    m_serviceCounter = 0;
    initFlagString();
}

void ServiceBuffer::initFlagString(){
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

    m_mapData[POWER_AMPLIFIERS_TEMPATURE] = "POWER_AMPLIFIERS_TEMPATURE";
    m_mapData[POWER_AMPLIFIERS_VOLTAGE] = "POWER_AMPLIFIERS_VOLTAGE";

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
    m_mapData[POWER_VOLTAGE_ERROR] = "POWER_VOLTAGE_ERROR";
}

void ServiceBuffer::onPackageReceived(QByteArray &ba) {
    deserialize(ba);
}

void ServiceBuffer::onServiceCalled(PackageFlag pFlag, DataFlag dFlag)
{

    qCDebug(HIFU_SERVICE()) << HIFU_SERVICE().categoryName()
                            << "pFlag is "<< pFlag
                            << "dFlag is "<< dFlag
                            << TRACE_CMH();

    if (pFlag == PLAN) return;

    // TODO: pFlag and dFlag should be checked in pair
    QByteArray ba;
    serialize(pFlag, dFlag, ba);
//    increaseServiceCounter();
    qCDebug(HIFU_SERVICE()) << HIFU_SERVICE().categoryName()
                            << "pFlag is "<< pFlag
                            << "dFlag is "<< dFlag
                            << "ba size is "<<ba.size()
                            << TRACE_CMH();

    emit bytesToWrite(ba);
}

void ServiceBuffer::onServiceCalled(QByteArray &ba) {
//    increaseServiceCounter();
    emit bytesToWrite(ba);
}

void ServiceBuffer::serialize(PackageFlag pFlag, DataFlag dFlag, QByteArray& ba)
{

    qCDebug(HIFU_SERVICE()) << HIFU_SERVICE().categoryName()
                            << "serialize pFlag "<< m_mapPackage[(PackageFlag)pFlag]
                            << "\n"
                            << "serialize pFlag "<< m_mapData[(DataFlag)dFlag]
                            << TRACE_CMH();

    if (pFlag == PLAN) return;

    qCDebug(HIFU_SERVICE()) << "Serializing...";
    QDataStream out(&ba, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_4);

    qDebug() << "Package flag:" << pFlag;

    out << (qint64)0
        << (qint64)0
        << (qint64)dFlag
        << (qint64)TERMINATOR;

    qint64 baSize = ba.size();
    if (out.device()->seek(0)) out << (qint64)pFlag << baSize;
    else qCDebug(HIFU_SERVICE()) << "Error in serialization!";
}

void ServiceBuffer::deserialize(QByteArray &ba) {
    if (ba.isEmpty()) {
        qCDebug(HIFU_SERVICE()) << "The byte array is empty";
        return;
    }

    QByteArray baBackup = ba;
    QDataStream in(&ba, QIODevice::ReadOnly);
    in.setVersion(QDataStream::Qt_5_4);
    qCDebug(HIFU_SERVICE()) << "Deserializing...";
    qint64 pFlag;
    in >> pFlag;
    qDebug() << "Package flag:" << pFlag;
    if (pFlag == (qint64)HEARTBEAT) return;
    if (pFlag == (qint64)PLAN) emit planCalled(baBackup);
    else {
        qint64 size, dFlag, terminator;
        try {
            in >> size >> dFlag >> terminator;
            qDebug() << "Size of bytes:" << size
                     << "Data flag:" << dFlag
                     << "Terminator in schedule:" << terminator;
            if (pFlag==(qint64)INQUIRY && dFlag == POWER_AMPLIFIERS_TEMPATURE)
            {
                qCDebug(HIFU_SERVICE()) << HIFU_SERVICE().categoryName()
                                        << "deserial pFlag "<< m_mapPackage[(PackageFlag)pFlag]
                                        << "\n"
                                        << "deserial pFlag "<< m_mapData[(DataFlag)dFlag]
                                        << TRACE_CMH();

                 emit refreshAmpPara((DataFlag)dFlag,ba);
                 return;

            }
            else if (pFlag==(qint64)INQUIRY && dFlag == POWER_AMPLIFIERS_VOLTAGE)
            {
                qCDebug(HIFU_SERVICE()) << HIFU_SERVICE().categoryName()
                                        << "deserial pFlag "<< m_mapPackage[(PackageFlag)pFlag]
                                        << "\n"
                                        << "deserial pFlag "<< m_mapData[(DataFlag)dFlag]
                                        << TRACE_CMH();

                emit refreshAmpPara((DataFlag)dFlag,ba);
                return;
            }

            if (terminator == qint64(TERMINATOR))
            {
                qCDebug(HIFU_SERVICE()) << HIFU_SERVICE().categoryName()
                                        << "deserial pFlag "<< m_mapPackage[(PackageFlag)pFlag]
                                        << "\n"
                                        << "deserial pFlag "<< m_mapData[(DataFlag)dFlag]
                                        << TRACE_CMH();

                emit serviceCalled(PackageFlag(pFlag), DataFlag(dFlag));
            }

            else qCDebug(HIFU_SERVICE()) << "Terminator error in deserialization";
        }catch (const std::exception& e) {
            qCDebug(HIFU_SERVICE()) << e.what();
        }
    }
}
