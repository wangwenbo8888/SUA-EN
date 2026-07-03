#include "append.h"
#include <QDebug>
#include <QSettings>
#include <QList>
#include "stdint.h"
#include <QDateTime>
#include <QEventLoop>
#include <QTimer>
#include <QDebug>

Append::Append(QObject *parent)
: QObject(parent)
{
    m_bExist=false;
    m_bSwitch = false;
    m_bRed = false;
    m_bYellow = false;
    m_bGreen = false;

    _workerThread=new QThread();
    _serialWorker=new SerialWorker();
    _serialWorker->moveToThread(_workerThread);

    connect(this,SIGNAL(serialWorkerInit(QString)),_serialWorker,SLOT(init(QString)));
    connect(_workerThread, &QThread::finished, _serialWorker, &QObject::deleteLater);
    connect(_serialWorker,SIGNAL(serialReadReady(QByteArray)),this,SLOT(readInfo(QByteArray)));
    connect(this,SIGNAL(serialWorkerSend(QByteArray)),_serialWorker,SLOT(senddata(QByteArray)));
    connect(this,SIGNAL(serialWorkerClose()),_serialWorker,SLOT(close()));
    connect(_serialWorker,SIGNAL(serialInitComplet(bool,bool)),this,SLOT(readExist(bool,bool)));
    connect(_serialWorker,SIGNAL(serialError()),this,SLOT(serialerror()));
    _workerThread->start();
    init();

}


Append::~Append()
{
    emit serialWorkerClose();

    _workerThread->quit();
    _workerThread->wait();

    if(_workerThread!=NULL){
        delete _workerThread;
        _workerThread=NULL;
    }
}

bool Append::init()
{
    m_bInitComplete=false;
    readSettings();
    emit serialWorkerInit(m_portName);
    while (!m_bInitComplete){
        QEventLoop eventloop;
        QTimer::singleShot(100, &eventloop, SLOT(quit()));
        eventloop.exec();
    }
    return m_bExist;
}

void Append::readSettings()
{
    QSettings* settings = new QSettings(SETTINGS_PATH,QSettings::IniFormat);
    m_portName = settings->value("Append/port").toString();
    delete settings;
    //m_portName="COM6";
}

void Append::readExist(bool exist,bool bswitch)
{
    m_bExist=exist;
    m_bSwitch=bswitch;
    m_bInitComplete=true;
    emit switchChanged(m_bSwitch);
}

bool Append::m_setLight(Light_Color color,Light_Status status)
{
    QByteArray baSend;
    baSend.resize(1);

    switch(color)
    {
    case RED:
        if (LIGHT_ON==status)
        {
            baSend[0] = 0xdd;
            m_bRed = true;
        }
        else if (LIGHT_OFF==status)
        {
            baSend[0] = 0xdc;
            m_bRed = false;
        }
        else
        {
            return false;
        }
        break;

    case GREEN:
        if (LIGHT_ON==status)
        {
            baSend[0] = 0xd9;
            m_bGreen = true;
        }
        else if (LIGHT_OFF==status)
        {
            baSend[0] = 0xd8;
            m_bGreen = false;
        }
        else
        {
            return false;
        }
        break;

    case YELLOW:
        if (LIGHT_ON==status)
        {
            baSend[0] = 0xdb;
            m_bYellow = true;
        }
        else if (LIGHT_OFF==status)
        {
            baSend[0] = 0xda;
            m_bYellow = false;
        }
        else
        {
            return false;
        }
        break;

    default:
        return false;
    }
    emit serialWorkerSend(baSend);
}

void Append::readInfo(QByteArray baRead)
{
    if (baRead.toHex()=="d400" )
    {
        m_bSwitch = false;
        qDebug() << (tr("脚踏开关-关 switch off ! %1 m_bSwitch:%2")
                                    .arg(QTime::currentTime().toString()).arg(m_bSwitch));
        emit switchChanged(m_bSwitch);
        return;
    }
    else if (baRead.toHex()=="d510")
    {
        m_bSwitch = true;
        qDebug() << (tr("脚踏开关-开 switch on ! %1 m_bSwitch:%2")
                                    .arg(QTime::currentTime().toString()).arg(m_bSwitch));
        emit switchChanged(m_bSwitch);
        return;
    }
    else
    {
        qDebug()<<"read other message "<< baRead.toHex();
        return;
    }
}

void Append::setLightsOff()
{
    if (m_bRed)
    {
        setRedOff();
        m_bRed = false;
    }

    if (m_bGreen)
    {
        setGreenOff();
        m_bGreen = false;
    }

    if (m_bYellow)
    {
        setYellowOff();
        m_bYellow = false;
    }
}

void Append::close()
{
    emit serialWorkerClose();
}

bool Append::SetLight(Light_Color color, Light_Status status)
{
//    m_setLight(color,status);
//    return true;
    QByteArray baSend;
    baSend.resize(1);

    switch(color)
    {
    case RED:
        if (LIGHT_ON==status)
        {
            baSend[0] = 0xdd;
            m_bRed = true;
        }
        else if (LIGHT_OFF==status)
        {
            baSend[0] = 0xdc;
            m_bRed = false;
        }
        else
        {
            return false;
        }
        break;

    case GREEN:
        if (LIGHT_ON==status)
        {
            baSend[0] = 0xd9;
            m_bGreen = true;
        }
        else if (LIGHT_OFF==status)
        {
            baSend[0] = 0xd8;
            m_bGreen = false;
        }
        else
        {
            return false;
        }
        break;

    case YELLOW:
        if (LIGHT_ON==status)
        {
            baSend[0] = 0xdb;
            m_bYellow = true;
        }
        else if (LIGHT_OFF==status)
        {
            baSend[0] = 0xda;
            m_bYellow = false;
        }
        else
        {
            return false;
        }
        break;

    default:
        return false;
    }
    emit serialWorkerSend(baSend);
    return true;

}

void Append::serialerror()
{
    emit error();
}
