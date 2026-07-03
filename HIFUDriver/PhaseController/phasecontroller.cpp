#include "phasecontroller.h"
#include <QDebug>
#include <QSettings>
#include <QtCore>

Q_LOGGING_CATEGORY(PC, "Phase Controller")

PhaseController::PhaseController(QObject *parent) :
    QObject(parent)
{
    init();
    PhaseReadCount=0;
}

PhaseController::~PhaseController(){
    close();
}

void PhaseController::loadSettings()
{
    QSettings* settings = new QSettings(SETTINGS_PATH, QSettings::IniFormat);
    m_Port = settings->value("PhaseController/port").toString();
    delete settings;
}

void PhaseController::init()
{
    b_Disable_Slot=false;
    sendEndFlag=false;
    sendStartFlag=false;
    loadSettings();
    m_serial=new QSerialPort(m_Port);
    qDebug()<<"phaseC connecting...,COM==="<<m_Port;
    if(openSerial())
    {
        qDebug()<<"phaseC connect succeed";
        setSerial();
        connect(m_serial,SIGNAL(readyRead()),this,SLOT(ReadData()));
        return;
    }
    else
    {
        qDebug()<<"phaseC connect failed";
        if (m_serial->isOpen()){
            m_serial->close();
            delete m_serial, m_serial = NULL;
        }
    }
    delete m_serial, m_serial = NULL;
}

bool PhaseController::openSerial()
{
    if (m_serial->isOpen()){
        return true;
    }
    if (m_serial->open(QIODevice::ReadWrite)){
//        connect(m_serial,SIGNAL(readyRead()),this,SLOT(receiveSerial()));
        return true;
    } else{
        return false;
    }
}

void PhaseController::setSerial()
{
    m_serial->setBaudRate(QSerialPort::Baud115200);
    m_serial->setParity(QSerialPort::NoParity);
    m_serial->setDataBits(QSerialPort::Data8);
    m_serial->setStopBits(QSerialPort::OneStop);
    m_serial->setFlowControl(QSerialPort::NoFlowControl);
    m_serial->setReadBufferSize(0);
    m_serial->clearError();
    m_serial->clear();
}

bool PhaseController::checkSerial()
{
    bool checked = false;
    b_Disable_Slot=true;
    chSend[0]=0xaa;              //Get ID
    sendSerial();
    if(receiveSerial()) return checked;
    if (chRead[0] != 0xaa) {return checked;}
    b_Disable_Slot=false;
    checked = true;
    return checked;
}

void PhaseController::sendSerial(){      //发送信息,显示
    int i;
    b_Disable_Slot=true;
    chSend[0]=0xaa;
    chSend[1]=0x03;
    chSend[5]=0x88;
    m_serial->clear();
    baSend.resize(6);
    for(i=0;i<6;i++){
        baSend[i]=chSend[i];
    }
    m_serial->write(baSend);
    if(!(m_serial->waitForBytesWritten(50)))
        qDebug()<<"Send False!!!!!!!!!!!!!!!!!!!!";
//    if(receiveSerial())
//    {
//        qDebug()<<"receive succeed";
//    }
    m_senddata += baSend;
    b_Disable_Slot=false;
}

bool PhaseController::receiveSerial(){     //在50ms内接收指定数量的信息,显示
    int n=0;
    if(!(m_serial->waitForReadyRead(50))){
        return -1;}

    n=m_serial->bytesAvailable();
    baRead=m_serial->readAll();
//    while (n<6) {
//        if(!(m_serial->waitForReadyRead(100))) break;
//        n+=m_serial->bytesAvailable();
//        baRead+=m_serial->readAll();
//    }
//    for(int i=0;i<n;i++){
//       chRead[i]=baRead[i];
//    }
    qDebug()<<"receive========"<<baRead.toHex();
    return true;
}

void PhaseController::updatePhase(uint8 channel, uint8 phase) {
    chSend[3]=phase;
    chSend[2]=channel;
    chSend[4]=BYTE_LOAD;
    sendSerial();
    chSend[4]=BYTE_LOCK;
    sendSerial();
}

void PhaseController::updatePhaseE(uint8 channel, uint8 phase,bool phase_disable) {
//    writeByte(PORT_CHANNEL, channel);
//    writeByte(PORT_PHASE, phase);
    if(channel==0)
    {
        sendEndFlag=false;
        sendStartFlag=true;
        m_senddata.clear();
    }

    if(phase_disable)
    {
        chSend[3]=phase;
        chSend[2]=channel;
        chSend[4]=BYTE_LOAD|BYTE_PHASE_DISABLE;
        sendSerial();
        chSend[4]=BYTE_LOCK|BYTE_PHASE_DISABLE;
        sendSerial();
    }
    else
    {
        chSend[3]=phase;
        chSend[2]=channel;
        chSend[4]=BYTE_LOAD|BYTE_PHASE_ENABLE;
        sendSerial();
        chSend[4]=BYTE_LOCK|BYTE_PHASE_ENABLE;
        sendSerial();
    }

    if(channel==143)
    {
        Delay_MSec(200);
        sendEndFlag=true;
    }
}

void PhaseController::enable()
{
    chSend[2]=0x00;
    chSend[3]=0x00;
    chSend[4]=BYTE_ENABLE;
    sendSerial();
}

void PhaseController::disable()
{
    chSend[2]=0x00;
    chSend[3]=0x00;
    chSend[4]=BYTE_DISABLE;
    sendSerial();
}

void PhaseController::close()
{
    if (m_serial->isOpen()){
        m_serial->close();
        delete m_serial, m_serial = NULL;
    }
}

void PhaseController::updateTime(uint16 time, uint ratio)
{
    uint offtime=time*ratio/100;
    b_Disable_Slot=true;
    chSend[0]=0xab;
    chSend[1]=time>>8 & 0xff;
    chSend[2]=time & 0xff;
    chSend[3]=offtime>>8 & 0xff;
    chSend[4]=offtime & 0xff;
    chSend[5]=0x88;
    m_serial->clear();
    baSend.resize(6);
    for(int i=0;i<6;i++){
        baSend[i]=chSend[i];
    }
    m_serial->write(baSend);
    if(!(m_serial->waitForBytesWritten(50)))
        qDebug()<<"Send False!!!!!!!!!!!!!!!!!!!!";
//    if(receiveSerial())
//    {
//        qDebug()<<"receive succeed";
//    }
    b_Disable_Slot=false;
}

bool PhaseController::updatePlan(uint time, uint ontime, uint count)
{
    qDebug()<<"count========="<<count;
    b_Disable_Slot=true;
    chSend[0]=0xab;
    chSend[1]=time>>8 & 0xff;
    chSend[2]=time & 0xff;
    chSend[3]=ontime>>8 & 0xff;
    chSend[4]=ontime & 0xff;
    chSend[5]=0x80;
    chSend[6]=count & 0xff;
    chSend[7]=0x88;
    if(count>256)
        chSend[5]=0x00;
    m_serial->clear();
    baSend.resize(8);
    for(int i=0;i<8;i++){
        baSend[i]=chSend[i];
    }
    m_serial->write(baSend);
    if(!(m_serial->waitForBytesWritten(50)))
    {
        qDebug()<<"Send False!!!!!!!!!!!!!!!!!!!!";
        return false;
    }
//    if(receiveSerial())
//    {
//        qDebug()<<"receive succeed";
//    }
    m_senddata += baSend;
    b_Disable_Slot=false;
    return true;
}

void PhaseController::clearPlan()
{
    b_Disable_Slot=true;
    chSend[0]=0xab;
    chSend[1]=0x01;
    chSend[2]=0x64;
    chSend[3]=0x00;
    chSend[4]=0x32;
    chSend[5]=0x40;
    chSend[6]=0x04;
    chSend[7]=0x88;
    m_serial->clear();
    baSend.resize(8);
    for(int i=0;i<8;i++){
        baSend[i]=chSend[i];
    }
    m_serial->write(baSend);
    if(!(m_serial->waitForBytesWritten(50)))
        qDebug()<<"Send False!!!!!!!!!!!!!!!!!!!!";
//    if(receiveSerial())
//    {
//        qDebug()<<"receive succeed";
//    }
    m_senddata += baSend;
    b_Disable_Slot=false;
}

void PhaseController::ReadData()
{
    if(b_Disable_Slot) return;
    int n=0;
    QByteArray baRead;
    n=m_serial->bytesAvailable();
    PhaseReadCount+=n;
    qDebug()<<"n====="<<n;
    qDebug()<<"PhaseReadCount====="<<PhaseReadCount;
    baRead=m_serial->readAll();
    qDebug()<<"baRead======"<<baRead.toHex();
    m_readdata+=baRead;
    if(!sendStartFlag)
    {
        PhaseReadCount=0;
        m_readdata.clear();
    }
    else if(PhaseReadCount==1736&&sendEndFlag)
    {
        qDebug()<<"传感器发送消息："<<m_senddata.toHex();
        qDebug()<<"传感器接收消息："<<m_readdata.toHex();
        if(m_senddata==m_readdata)
        {
            qDebug()<<"m_senddata======m_readdata";
        }
        m_readdata.clear();
        PhaseReadCount=0;
        sendEndFlag=false;
        sendStartFlag=false;
    }
    else if(PhaseReadCount!=1736&&sendEndFlag)
    {
        qDebug()<<"phase send failed";
        PhaseReadCount=0;
        sendEndFlag=false;
        sendStartFlag=false;
        emit PhaseSendError();
    }
    if((baRead.at(0)==(char)0xac)||(n==7&&baRead.at(6)==(char)0xac))
    {
        qDebug()<<"receive PointCompleted data";
        disable();
        clearPlan();
        emit PointCompleted();
    }
}

void PhaseController::Delay_MSec(unsigned int msec)
{
    QTime _Timer = QTime::currentTime().addMSecs(msec);
    while(QTime::currentTime() < _Timer)
        QCoreApplication::processEvents(QEventLoop::AllEvents,100);
}
