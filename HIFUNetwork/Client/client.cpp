#include <exception>
#ifdef DEBUG
#include <QDebug>
#endif
#include <QMessageBox>
#include "client.h"

Q_LOGGING_CATEGORY(HIFU_CLIENT, "HIFU Client")

Client::Client(QObject *parent) : QObject(parent),
    m_sendCounter(0)
{
    heartcount = 0;

    m_client = new QTcpServer(this);
    m_txSocket = new QTcpSocket(this);

    m_waitTimer.setSingleShot(true);
    connect(&m_waitTimer, SIGNAL(timeout()), this, SLOT(onWaitTimerTimeout()));


    m_heartbeat.setInterval(500);
    m_heartbeat.setSingleShot(true);
    connect(&m_heartbeat, SIGNAL(timeout()), this, SLOT(sendflagchange()));

    m_recevieheart.setInterval(5000);
    //connect(&m_recevieheart,SIGNAL(timeout()),this,SLOT(connectfailed()));

    loadSettings();
    listen();
    netwarning = false;
    sendflag = true;

    qDebug() << "State of m_txSocket:" << m_txSocket->state();
}

void Client::loadSettings() {
    QSettings *settings = new QSettings(SETTINGS_PATH, QSettings::IniFormat);
    //m_rxIp = localIP().isNull()?  "":localIP().toString();
    m_rxIp = settings->value("Server/tx_ip").toString().toUShort(0,10);
    m_rxPort = settings->value("Client/rx_port").toString().toUShort(0,10);
    m_txIp = settings->value("Client/tx_ip").toString();
    m_txPort = settings->value("Client/tx_port").toString().toUShort(0,10);

    qDebug()<<"set rx ip is "<<m_rxIp;
    qDebug()<<"set rx port is "<<m_rxPort;
    qDebug()<<"set tx ip is "<<m_txIp;
    qDebug()<<"set tx port is "<<m_txPort;
    delete settings;
}

void Client::updateSettings() {
    QSettings *settings = new QSettings(SETTINGS_PATH, QSettings::IniFormat);
    settings->setValue("Client/rx_ip", m_rxIp);
    settings->setValue("Client/rx_port", m_rxPort);
    settings->setValue("Client/tx_ip", m_txIp);
    settings->setValue("Client/tx_port", m_txPort);

    qDebug()<<"update rx ip is "<<m_rxIp;
    qDebug()<<"update rx port is "<<m_rxPort;
    qDebug()<<"update tx ip is "<<m_txIp;
    qDebug()<<"update tx port is "<<m_txPort;

    delete settings;
}

QHostAddress Client::localIP() {
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

void Client::connectServer() {
    if (m_txSocket->state() == QAbstractSocket::UnconnectedState) {
        m_txSocket->connectToHost(QHostAddress(m_txIp), m_txPort);
        connect(m_txSocket, SIGNAL(error(QAbstractSocket::SocketError)),
                this, SLOT(onErrorSent(QAbstractSocket::SocketError)));
        //m_heartbeat.start();
        //m_recevieheart.start();
    }else
        qCDebug(HIFU_CLIENT()) << "Still writing data, please wait for a while to connect";
}

void Client::onErrorSent(QAbstractSocket::SocketError) {
    qCDebug(HIFU_CLIENT()) << "TX" << m_txSocket->errorString();
}

void Client::onErrorReceived(QAbstractSocket::SocketError) {
    qCDebug(HIFU_CLIENT()) << "RX" << m_rxSocket->errorString();
}

void Client::send(QByteArray &ba) {
    qDebug()<<"sendflag = false";
    sendflag = false;
    qDebug() << "State of m_txSocket before sending:" << m_txSocket->state();

    qCDebug(HIFU_CLIENT()) << HIFU_CLIENT().categoryName()
                           << "ba size is "<< ba.size()
                           << TRACE_CMH();

    if ( m_txSocket->state() != QAbstractSocket::UnconnectedState) {
        qDebug() << "Still writing data, please wait for a while to connect";
        m_buffer = ba;
        // Add a safe counter
//        m_sendCounter++;
//        if (m_sendCounter <= SEND_SAFE_COUNTER) m_waitTimer.start(waitMSec);
//        else {
//            m_sendCounter = 0;
//            emit statusUpdated(INQUIRY, NETWORK_DISCONNECTED);
//        }

        m_waitTimer.start(waitMSec);
        return;
    }

    try {
        qDebug()<<"tx ip is "<<m_txIp;
        qDebug()<<"tx port is "<<m_txPort;
        m_txSocket->connectToHost(QHostAddress(m_txIp), m_txPort);
        if (m_txSocket->waitForConnected(1000)) {
            connect(m_txSocket, SIGNAL(error(QAbstractSocket::SocketError)),
                    this, SLOT(onErrorSent(QAbstractSocket::SocketError)));
            qDebug() << "Connected";
        }
        else {
            flush();
            qDebug() << "Disconnected";
            emit statusUpdated(INQUIRY, NETWORK_DISCONNECTED);
            m_heartbeat.start();
            return;
        }
        qint64 BytesWritten = m_txSocket->write(ba);
        qDebug() << "BytesWritten:" << BytesWritten;
        qDebug() << "Size of bytes in client:" << ba.size();
        qDebug()<<"ba = "<<ba.toHex();
        m_txSocket->disconnectFromHost();
        qDebug() << "State of m_txSocket after sending:" << m_txSocket->state();
        m_heartbeat.start();
        // Empty the buffer after writing the data
        //flush();
    } catch (std::exception const& e) {
        qCDebug(HIFU_CLIENT()) << e.what();
    }
}

void Client::listen() {
    if (m_rxIp.isEmpty()) {
        // TODO: Handle this situation
        qCDebug(HIFU_CLIENT()) << "IP does not exist";
        return;
    }

    if (m_client->isListening()) {
        // TODO: Handle this situation
        qCDebug(HIFU_CLIENT()) << "IP is already listening now";
    } else {

        qDebug()<<"rx ip is "<< m_rxIp;
        qDebug()<<"rx port is "<< m_rxPort;
        if (m_client->listen(QHostAddress(m_rxIp), m_rxPort)) {
            qCDebug(HIFU_CLIENT()) << "Listening IP: " << m_rxIp
                                   << "Port: " << m_rxPort;
            connect(m_client, SIGNAL(newConnection()), this, SLOT(onNewConnection()));
        } else {
            qCDebug(HIFU_CLIENT()) << "Error in listening";
            m_client->close();
        }
    }
}

void Client::onNewConnection() {
    // flush();
    m_bufferReceive.clear();
    m_rxSocket = m_client->nextPendingConnection();
    connect(m_rxSocket, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
    connect(m_rxSocket, SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT(onErrorReceived(QAbstractSocket::SocketError)));

    qCDebug(HIFU_CLIENT()) << "New connection connected";
    emit connectsuccess();
}

void Client::onReadyRead() {
    QByteArray ba = m_rxSocket->readAll();
    emit heartrece();
    // m_buffer += ba;
    m_bufferReceive += ba;
    qDebug()<<"receivedata = "<<ba.toHex();
    QByteArray terminator = ba.right(sizeof(qint64));
    // TODO: whether complete
    if (isheartbeat(terminator))
    {
        qDebug()<<"heartbeat receive";
        m_bufferReceive.clear();
        heartcount = 0;
        return;
    }
    if (isTerminator(terminator)) {
        disconnect(m_rxSocket, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
        disconnect(m_rxSocket, SIGNAL(error(QAbstractSocket::SocketError)),
                   this, SLOT(onErrorReceived(QAbstractSocket::SocketError)));
        // QByteArray bufferBack = m_buffer;
        QByteArray bufferBack = m_bufferReceive;
        emit packageReady(bufferBack);
        m_bufferReceive.clear();
        heartcount = 0;

    }  else qCDebug(HIFU_CLIENT()) << "Still receiving";
}

bool Client::isTerminator(QByteArray &ba) {
    QDataStream in(&ba, QIODevice::ReadOnly);
    in.setVersion(QDataStream::Qt_5_4);
    qint64 terminator;
    in >> terminator;
    qDebug() << "Terminator in client:" << terminator;
    return terminator == qint64(TERMINATOR) ? true:false;
}

bool Client::isheartbeat(QByteArray &ba)
{
    QDataStream in(&ba, QIODevice::ReadOnly);
    in.setVersion(QDataStream::Qt_5_4);
    qint64 terminator;
    in >> terminator;
    qDebug() << "Terminator in client:" << terminator;
    return terminator == qint64(HEARTRECE) ? true:false;
}

void Client::flush() {
    if (!m_buffer.isEmpty()) m_buffer.clear();
}

void Client::onWaitTimerTimeout() {
    send(m_buffer);
}

void Client::connectfailed()
{
//    heartcount++;
//    qDebug() << "heartcount = "<< heartcount;
//    if(heartcount>5)
//    {
//        qDebug() << "网络连接断开";
//        heartcount = 0;
//        //emit connectfail();
//        if(!netwarning)
//        {
//            netwarning = true;
            emit statusUpdated(INQUIRY, NETWORK_DISCONNECTED);
            QMessageBox msgBox("网络连接断开","请确保工控机连接正常",
                               QMessageBox::Warning,
                               QMessageBox::Ok | QMessageBox::Escape,
                               QMessageBox::NoButton,
                               QMessageBox::NoButton);
            msgBox.setWindowFlags(Qt::WindowTitleHint|Qt::CustomizeWindowHint);
            msgBox.setGeometry(QRect(1400,400,msgBox.width(),msgBox.height()));


            int ret = msgBox.exec();
            if ((ret == QMessageBox::Ok)||(ret == QMessageBox::Close))
            {
                return;
            }
            else
            {
                return;
            }
//        }
//    }
}

void Client::sendheartbeat()
{
    QByteArray m_bufferheart;
    m_bufferheart.resize(32);
    for(int i = 0;i<31;i++)
        m_bufferheart[i] = 0x00;
    m_bufferheart[31] = 0xFF;
    while(sendflag)
    {
    send(m_bufferheart);
    qDebug()<<"senddata = "<<m_bufferheart.toHex();
    }
}

void Client::closeheartbeat()
{
    m_heartbeat.stop();
    m_recevieheart.stop();
}

void Client::sendflagchange()
{
    sendflag = true;
    qDebug()<<"sendflag = true";
}
