#include <QSettings>
#include <QDataStream>
#ifdef DEBUG
#include <QDebug>
#endif

#include "server.h"

Q_LOGGING_CATEGORY(HIFU_SERVER, "HIFU Server")

Server::Server(QObject *parent) : QObject(parent)
{
    qDebug()<<"Enter to construce server !";

    // Initialization the listening server and its socket, the sending socket
    m_server = new QTcpServer(this);
    m_txSocket = new QTcpSocket(this);

    m_waitTimer.setSingleShot(true);
    connect(&m_waitTimer, SIGNAL(timeout()), this, SLOT(onWaitTimerTimeout()));

    //connect(this,SIGNAL(receheart()),this,SLOT(sendheartrece()));
    sendfailcount = 0;
    loadSettings();
    listen();
}

void Server::loadSettings() {
    QSettings *settings = new QSettings(SETTINGS_PATH, QSettings::IniFormat);
    m_rxIp = localIP().toString().isNull()? "":localIP().toString();
    m_rxPort = settings->value("Server/rx_port").toString().toUShort(0,10);
    m_txIp = settings->value("Server/tx_ip").toString();
    m_txPort = settings->value("Server/tx_port").toString().toUShort(0,10);

    qDebug() << "rx ip is :" << m_rxIp;
    qDebug() << "rx port is :" << m_rxPort;
    qDebug() << "tx ip is :" << m_txIp;
    qDebug() << "tx port is :" << m_txPort;
    delete settings;
}

void Server::updateSettings() {
    QSettings *settings = new QSettings(SETTINGS_PATH, QSettings::IniFormat);
    settings->setValue("Server/rx_ip", m_rxIp);
    settings->setValue("Server/rx_port", m_rxPort);
    settings->setValue("Server/tx_ip", m_txIp);
    settings->setValue("Server/tx_port", m_txPort);

    qDebug() << "rx ip is :" << m_rxIp;
    qDebug() << "rx port is :" << m_rxPort;
    qDebug() << "tx ip is :" << m_txIp;
    qDebug() << "tx port is :" << m_txPort;
    delete settings;
}

void Server::onErrorReceived(QAbstractSocket::SocketError) {
    qCDebug(HIFU_SERVER()) << "RX" << m_rxSocket->errorString();
}

void Server::onErrorSent(QAbstractSocket::SocketError) {
    qCDebug(HIFU_SERVER()) << "TX" << m_txSocket->errorString();
}

//  IP of the local server
QHostAddress Server::localIP() {
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

void Server::listen() {
    if (m_rxIp.isEmpty()) {
        // TODO: Handle this situation
        qCDebug(HIFU_SERVER()) << "IP does not exist";
        emit statusUpdated(INQUIRY, NETWORK_DISCONNECTED);
        return;
    }

    if (m_server->isListening()) {
        // TODO: Handle this situation
        qCDebug(HIFU_SERVER()) << "IP is already listening now";
        emit statusUpdated(INQUIRY, NETWORK_CONNECTED);
    } else {
        if (m_server->listen(QHostAddress(m_rxIp), m_rxPort)) {
            qCDebug(HIFU_SERVER()) << "Listening IP: " << m_rxIp
                                   << "Port: " << m_rxPort;
            connect(m_server, SIGNAL(newConnection()),
                    this, SLOT(onNewConnection()));
            emit statusUpdated(INQUIRY, NETWORK_CONNECTED);
        } else {
            qCDebug(HIFU_SERVER()) << "Error in listening";
            m_server->close();
            emit statusUpdated(INQUIRY, NETWORK_CONNECTED);
        }
    }
}

void Server::onNewConnection() {
    // flush();
        m_bufferReceive.clear();
        m_rxSocket = m_server->nextPendingConnection();
        connect(m_rxSocket, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
        connect(m_rxSocket, SIGNAL(error(QAbstractSocket::SocketError)),
                this, SLOT(onErrorReceived(QAbstractSocket::SocketError)));
        qCDebug(HIFU_SERVER()) << "New connection connected";
}

void Server::onReadyRead() {
    QByteArray ba = m_rxSocket->readAll();
    m_bufferReceive += ba;
    qDebug() << "bytes from client:"<<ba.toHex();
    QByteArray terminator = ba.right(sizeof(qint64));
    qDebug() << "Size of bytes in server:" << ba.size();
    if (isheartbeat(terminator))
    {
        qDebug()<<"heartbeat receive";
        disconnect(m_rxSocket, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
        disconnect(m_rxSocket, SIGNAL(error(QAbstractSocket::SocketError)),
                   this, SLOT(onErrorReceived(QAbstractSocket::SocketError)));
        m_bufferReceive.clear();
        sendheartrece();
        return;
    }
    if (isTerminator(terminator)) {
        disconnect(m_rxSocket, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
        disconnect(m_rxSocket, SIGNAL(error(QAbstractSocket::SocketError)),
                   this, SLOT(onErrorReceived(QAbstractSocket::SocketError)));
        QByteArray bufferBack = m_bufferReceive;
        emit packageReady(bufferBack);
        m_bufferReceive.clear();
//        emit pa_checkchannels();
//        qDebug()<<"check poweramp chennels";
    }

    else qCDebug(HIFU_SERVER()) << "Still receiving";
}

bool Server::isTerminator(QByteArray &ba) {
    QDataStream in(&ba, QIODevice::ReadOnly);
    in.setVersion(QDataStream::Qt_5_4);
    qint64 terminator;
    in >> terminator;

    qDebug() << "Terminator in server:" << terminator;

    return terminator == qint64(TERMINATOR) ? true:false;
}

bool Server::isheartbeat(QByteArray &ba)
{
    QDataStream in(&ba, QIODevice::ReadOnly);
    in.setVersion(QDataStream::Qt_5_4);
    qint64 terminator;
    in >> terminator;
    qDebug() << "Terminator in client:" << terminator;
    return terminator == qint64(HEARTBEAT) ? true:false;
}

void Server::connectServer() {
    if (m_txSocket->state() == QAbstractSocket::UnconnectedState) {
        m_txSocket->connectToHost(QHostAddress(m_txIp), m_txPort);
        connect(m_txSocket, SIGNAL(error(QAbstractSocket::SocketError)),
                this, SLOT(onErrorSent(QAbstractSocket::SocketError)));
    }else
        qCDebug(HIFU_SERVER()) << "Still writing data, please wait for a whole to connect";
}

void Server::send(QByteArray &ba) {
    try {
        if ( m_txSocket->state() != QAbstractSocket::UnconnectedState) {
            qDebug() << "Still writing data, please wait for a whole to connect";
            m_bufferSend = ba;
            m_waitTimer.start(waitMsec);
            //sendfailcount++;
            return;
        }

        qCDebug(HIFU_SERVER()) << HIFU_SERVER().categoryName()
                               << "send ba size is "<<ba.size()
                               << TRACE_CMH();

        m_txSocket->connectToHost(QHostAddress(m_txIp), m_txPort);
        connect(m_txSocket, SIGNAL(error(QAbstractSocket::SocketError)),
                this, SLOT(onErrorSent(QAbstractSocket::SocketError)));
        m_txSocket->write(ba);
        qDebug()<<"ba = "<<ba.toHex();
        m_txSocket->disconnectFromHost();
        qCDebug(HIFU_SERVER()) << HIFU_SERVER().categoryName()
                               << "send ba success " << ba.size()
                               << TRACE_CMH();

        // flush();
        //m_bufferSend.clear();
        emit sendCompleted();

    } catch (const std::exception& e) {
        qCDebug(HIFU_SERVER()) << e.what();
    }
}

void Server::flush()
{
    qCDebug(HIFU_SERVER()) << HIFU_SERVER().categoryName()
                           << "clear buffer "
                           << TRACE_CMH();

    // if (!m_buffer.isEmpty()) m_buffer.clear();

    qCDebug(HIFU_SERVER()) << HIFU_SERVER().categoryName()
                           //<< "after clear buffer buffer size "<<m_buffer.size()
                           << TRACE_CMH();
}

void Server::onWaitTimerTimeout()
{
    qCDebug(HIFU_SERVER()) << HIFU_SERVER().categoryName()
                           << "send wait time out, send again !"
                           << TRACE_CMH();

    // send(m_buffer);
    send(m_bufferSend);
}


void Server::work(PackageFlag pFlag, DataFlag dFlag) {
    if(SpaceEnough)
    {
        if (pFlag == INQUIRY && dFlag == NETWORK)
            emit statusUpdated(INQUIRY, NETWORK_CONNECTED);
        if(PowerVoltError)
        {
            emit statusUpdated(INQUIRY,POWER_VOLTAGE_ERROR);
        }
    }
    else
        emit statusUpdated(INQUIRY,NETWORK_DISCONNECTED);
}

void Server::sendheartrece()
{
    QByteArray m_bufferheart;
    m_bufferheart.resize(32);
    for(int i = 0;i<31;i++)
        m_bufferheart[i] = 0x00;
    m_bufferheart[31] = 0xFE;
    send(m_bufferheart);
    qDebug()<<"senddata = "<<m_bufferheart.toHex();
}

void Server::ServerSpaceError()
{
    SpaceEnough=false;
}

void Server::PVoltError()
{
    PowerVoltError=true;
     qDebug()<<"PowerVoltError=true!!!!!!!!!!!!!!!";
}
