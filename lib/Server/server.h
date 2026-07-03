#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QtNetwork>
#include <QByteArray>
#include <QTimer>
#include <QLoggingCategory>

#include "constant.h"
#include "variable.h"
#include "server_global.h"

Q_DECLARE_LOGGING_CATEGORY(HIFU_SERVER)

const int waitMsec = 1000;

class SERVERSHARED_EXPORT Server : public QObject
{
    Q_OBJECT

public:
    Server(QObject *parent=0);
    void listen();

public slots:
    void send(QByteArray& ba);
    void work(PackageFlag, DataFlag);
    void ServerSpaceError();
    void PVoltError();

private slots:
    void onNewConnection();
    void onReadyRead();
    void onErrorSent(QAbstractSocket::SocketError);
    void onErrorReceived(QAbstractSocket::SocketError);
    void onWaitTimerTimeout();
    void sendheartrece();

signals:
    void statusUpdated(PackageFlag, DataFlag);
    void sendCompleted();
    void packageReady(QByteArray& ba);
    //void receheart();
    void pa_checkchannels();

private:
    QTcpServer *m_server;
    QTcpSocket *m_txSocket, *m_rxSocket;
    QString m_rxIp, m_txIp;
    quint16 m_rxPort, m_txPort;

    // Scenario: the period between two sending is so short that it can be negelcted.
    // Use a timer to delay the next sending to avoid crash.
    QTimer m_waitTimer;
    QByteArray m_bufferSend;
    QByteArray m_bufferReceive;


    int sendfailcount;
    void loadSettings();
    void updateSettings();
    QHostAddress localIP();
    bool isTerminator(QByteArray& ba);
    bool isheartbeat(QByteArray& ba);
    void connectServer();
    void flush();
    bool isconnected;
    bool checkchannels;
    bool SpaceEnough=true;
    bool PowerVoltError=false;
};

#endif // SERVER_H
