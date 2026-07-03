#include "heart_thread.h"

heartthread::heartthread()
    :QObject()
{
    heartcount = 0;
    netwarning = false;

}

heartthread::~heartthread()
{

}

void heartthread::_start()
{
//    h_client = new Client;
    qDebug()<<"thread ID: " << QThread::currentThreadId();
    m_heartbeat = new QTimer(this);
    m_heartbeat->setInterval(2500);
    connect(m_heartbeat,SIGNAL(timeout()),this,SLOT(heartbeat()));
//    m_receiveheart = new QTimer(this);
//    m_receiveheart->setInterval(5000);
//    connect(m_receiveheart,SIGNAL(timeout()),this,SLOT(connectfailed()));
    //connect(h_client,SIGNAL(heartrece()),this,SLOT(isheartrece()));
    m_heartbeat->start();
//    m_receiveheart->start();
}

void heartthread::deleteLater()
{

}

void heartthread::heartbeat()
{
    QProcess pingProcess;
    QString strArg = "ping 192.168.16.254 -n 1 -i 2";
    pingProcess.start(strArg, QIODevice::ReadOnly);
    pingProcess.waitForFinished(-1);
    QString p_stdout = QString::fromLocal8Bit(pingProcess.readAllStandardOutput());
    qDebug() << "p_stdout===" << qUtf8Printable(p_stdout);
    if(p_stdout.contains("TTL="))
    {
        bPingSuccess = 0;
        qDebug() << "ping success";
    }
    else
    {
        bPingSuccess++;
        qDebug() << "ping false";
        if(bPingSuccess>4)
        {
//            emit statusUpdated(NETWORK_DISCONNECTED);
//            emit statusUpdated(POWER_AMPLIFIERS_NOT_READY);
            emit GateWayFailed();
            m_heartbeat->stop();
        }
    }
}

void heartthread::isheartrece()
{
    heartcount = 0;
    qDebug()<<"heartcount清空";
}

void heartthread::connectfailed()
{
    heartcount++;
    if(heartcount>5)
    {
        qDebug() << "网络连接断开";
        heartcount = 0;
        if(!netwarning)
        {
            netwarning = true;
            emit connectfail();
            qDebug()<<"messagebox connectfailed";
//            //emit statusUpdated(INQUIRY, NETWORK_DISCONNECTED);
//            QMessageBox msgBox("Network Connection Lost","请重启客户端并确保工控机连接正常",
//                               QMessageBox::Warning,
//                               QMessageBox::Ok | QMessageBox::Escape,
//                               QMessageBox::NoButton,
//                               QMessageBox::NoButton);
//            msgBox.setWindowFlags(Qt::WindowTitleHint|Qt::CustomizeWindowHint);
//            msgBox.setGeometry(QRect(1400,400,msgBox.width(),msgBox.height()));


//            int ret = msgBox.exec();
//            if ((ret == QMessageBox::Ok)||(ret == QMessageBox::Close))
//            {
//                return;
//            }
//            else
//            {
//                return;
//            }
        }
    }
}

//void heartthread::stopheart()
//{
//    m_heartbeat->stop();
//    m_receiveheart->stop();
//}

//void heartthread::startheart()
//{
//    m_heartbeat->start();
//    m_receiveheart->start();
//}
