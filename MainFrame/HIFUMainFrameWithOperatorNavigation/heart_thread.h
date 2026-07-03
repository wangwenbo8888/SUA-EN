#ifndef HEART_THREAD_H
#define HEART_THREAD_H

#include <QObject>
#include <QMessageBox>
#include <QThread>
#include <QDebug>
#include <QThread>
#include <QTimer>
#include <QTcpServer>  //监听套接字
#include <QTcpSocket> //通信套接字
#include <windows.h>
#include "client.h"

class heartthread : public QObject
{
    Q_OBJECT

public:
    heartthread();
    ~heartthread();

    inline void changeflag();

public slots:
    void _start();
    void deleteLater();
    void heartbeat();
    void isheartrece();
//    void stopheart();
//    void startheart();

private:
    Client* h_client;
    QTimer* m_heartbeat;
    QTimer* m_receiveheart;
    QTimer* m_waitTimer;

    int heartcount;
    int bPingSuccess=0;
    bool netwarning;

private slots:
    void connectfailed();


signals:
    void sendheart();
    void statusUpdated(DataFlag);
    void GateWayFailed();
    void connectfail();
};

inline void heartthread::changeflag() {netwarning = false;}

#endif // HEART_THREAD_H

