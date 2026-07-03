#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QtNetwork>
#include <QSettings>
#include <QByteArray>
#include <QTimer>
#include <QLoggingCategory>

#include "constant.h"
#include "variable.h"
#include "client_global.h"

Q_DECLARE_LOGGING_CATEGORY(HIFU_CLIENT)

const int waitMSec = 1000;

class CLIENTSHARED_EXPORT Client : public QObject
{
    Q_OBJECT

public:
    Client(QObject *parent=0);
    void listen();

public slots:
    void send(QByteArray& ba);
    void sendheartbeat();
    void closeheartbeat();

signals:
    void statusUpdated(PackageFlag, DataFlag);
    void packageReady(QByteArray&);
    void heartrece();
    void sendheart();
    void connectsuccess();

private slots:
    void onErrorSent(QAbstractSocket::SocketError);
    void onErrorReceived(QAbstractSocket::SocketError);
    void onNewConnection();
    void onReadyRead();
    void onWaitTimerTimeout();
    void connectfailed();
    void sendflagchange();

private:
    QTcpServer *m_client;
    QTcpSocket *m_rxSocket, *m_txSocket;
    QString m_rxIp, m_txIp;
    quint16 m_rxPort, m_txPort;

    // Scenario: the period between two sending is so short that it can be negelcted.
    // Use a timer to delay the next sending to avoid crash.
    QTimer m_waitTimer;
    QTimer m_heartbeat;
    QTimer m_recevieheart;
    QByteArray m_buffer;
    QByteArray m_bufferReceive;

    quint8 m_sendCounter;

    int heartcount;
    void loadSettings();
    void updateSettings();
    QHostAddress localIP();
    void connectServer();
    bool isTerminator(QByteArray& ba);
    bool isheartbeat(QByteArray& ba);
    bool netwarning;
    bool sendflag;
    void flush();
//    void timeStamp(QString& stamp);
};

#endif // CLIENT_H
