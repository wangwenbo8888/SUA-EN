#ifndef NETWORK_H
#define NETWORK_H

#include <QObject>
#include <QtNetwork>
#include <QLoggingCategory>

#include "network_global.h"

class NETWORKSHARED_EXPORT Network : public QObject
{
    Q_OBJECT

public:
    Network(QObject *parent=0);
    void send(QByteArray& ba);

private:
    QTcpServer* m_txServer;
    QTcpSocket* m_txSocket, *m_rxSocket;

    void loadSettings();
    void updateSettings();
};

QHostAddress localIP() {
    foreach (const QNetworkInterface& ni, QNetworkInterface::allInterfaces())
        if (ni.isValid() &&
            ni.flags().testFlag(QNetworkInterface::IsRunning) &&
            ni.flags().testFlag(QNetworkInterface::IsUp) &&
            !ni.flags().testFlag(QNetworkInterface::IsLoopBack))
            foreach (const QNetworkAddressEntry& entry, ni.addressEntries())
                if (entry.ip().toIPv4Address())
                    return QHostAddress(entry.ip().toString());
    // Return 0.0.0.0
    return QHostAddress();
}

#endif // NETWORK_H
