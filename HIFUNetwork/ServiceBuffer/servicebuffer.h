#ifndef SERVICEBUFFER_H
#define SERVICEBUFFER_H

#include <QObject>
#include <QByteArray>
#include <QLoggingCategory>
#include <QMap>

#include "variable.h"
#include "servicebuffer_global.h"

Q_DECLARE_LOGGING_CATEGORY(HIFU_SERVICE)

class SERVICEBUFFERSHARED_EXPORT ServiceBuffer : public QObject
{
    Q_OBJECT

public:
    // Server
    ServiceBuffer(QObject *parent=0);

    // Used for plan
    void onServiceCalled(QByteArray&);

    inline const QString mapFromPackage(PackageFlag);
    inline const QString mapFromData(DataFlag);

public slots:
    void onPackageReceived(QByteArray& ba);
    // Client-end & Server-end
    // Used for hardware inquiry and sonication
    void onServiceCalled(PackageFlag, DataFlag);

signals:
    // Client-end and Server-end
    void bytesToWrite(QByteArray&);
    // Server-end: hardware inquiry and sonication
    // Client-end: feedback
    void serviceCalled(PackageFlag, DataFlag);
    // Used for plan
    void planCalled(QByteArray&);

    // 刷新功放板状态
    void refreshAmpPara(DataFlag,QByteArray&);

private:
    // TODO: check that the send-receive pair is consistent
    unsigned int m_serviceCounter;
    QMap<PackageFlag, QString> m_mapPackage;
    QMap<DataFlag, QString> m_mapData;

    void initFlagString();
    void serialize(PackageFlag, DataFlag, QByteArray&);
    void deserialize(QByteArray&);
    inline void increaseServiceCounter() { m_serviceCounter++; }
    inline void decreaseServiceCounter() { m_serviceCounter--; }
};

inline const QString ServiceBuffer::mapFromPackage(PackageFlag pFlag) { return m_mapPackage[pFlag]; }
inline const QString ServiceBuffer::mapFromData(DataFlag dFlag) { return m_mapData[dFlag]; }

#endif // SERVICEBUFFER_H
