#include "serialworker.h"
#include "constant.h"

#include <QtSerialPort/QtSerialPort>

SerialWorker::SerialWorker(QObject *parent) : QObject(parent)
{
    m_bExist=false;
    m_bSwitchStatusInit=false;
    m_SerialReadBuffer.clear();
    serial=NULL;
    //serial=new QSerialPort();
}

SerialWorker::~SerialWorker()
{
    if(serial!=NULL){
        delete serial;
        serial=NULL;
    }
}

void SerialWorker::init(QString portNmae2)
{
    //qDebug()<<serial;
    if(serial==NULL)
    {
        serial=new QSerialPort();
    }
    else if(serial->isOpen())
    {
        serial->close();
    }
    qDebug()<<serial<<portNmae2;
    serial->setPortName(portNmae2);
    if(serial->open(QIODevice::ReadWrite))
    {
        if(CheckDevice())
        {
            SetEnable();
            updateInitStauts();
            connect(serial,SIGNAL(readyRead()),this,SLOT(readdata()));
            qDebug()<<"串口打开成功,设备校验成功 "<<m_bExist<<serial;
            m_bExist=true;
        }
        else
        {
            qDebug()<<"串口打开成功,设备校验失败";
        }
    }
    else
    {
        qDebug()<<"串口打开失败"<<portNmae2;
        scanSerial();

    }
    emit serialInitComplet(m_bExist,m_bSwitchStatusInit);
}

void SerialWorker::readdata()
{
    m_SerialReadBuffer.append(serial->readAll());
    qDebug()<<"Append ReadData====="<<m_SerialReadBuffer.toHex();
    if(m_SerialReadBuffer.length()<2)
        return;
    if(DataValidation(m_SerialReadBuffer)){
        handleNewData();
    }
}

void SerialWorker::senddata(QByteArray data)
{
    if(!m_bExist)
        return;
    QByteArray DataComplet=DataAddCRC(data);
    serial->write(DataComplet);
    if(serial->waitForBytesWritten(300))
    {
        qDebug()<<"Write data in Thread success"<<QString(DataComplet.toHex());
    }
    else
        emit serialError();
}

void SerialWorker::close()
{
    if(serial->isOpen())
    {
        serial->close();
    }
}

bool SerialWorker::openSerial()
{
    bool opened = false;
    if (exist()) opened = serial->isOpen() ? true:serial->open(QIODevice::ReadWrite);
    if (opened)
    {
        //qCDebug(PA()) << "Opened the serial successfully.";
    }
    else
    {
        qDebug() << "Failed to open the serial.";
    }

    return opened;
}

void SerialWorker::updateSettings(){
    QSettings* settings = new QSettings(SETTINGS_PATH, QSettings::IniFormat);
    settings->setValue("Append/port", serial->portName());
    delete settings;
}

void SerialWorker::scanSerial()
{
    QList<QSerialPortInfo> infoList = QSerialPortInfo::availablePorts();
    if (serial==NULL)
    {
        serial = new QSerialPort;
    }

    foreach (const QSerialPortInfo &info, infoList)
    {
        serial->setPort(info);
        qDebug()<<"port is "<<info.portName();
        if (!openSerial())
        {
            continue;
        }
        else
        {
            if(CheckDevice())
            {
                SetEnable();
                updateInitStauts();
                connect(serial,SIGNAL(readyRead()),this,SLOT(readdata()));
                qDebug()<<"串口打开成功,设备校验成功 "<<m_bExist<<serial;
                m_bExist=true;
                updateSettings();
                return;
            }
            else
            {
                serial->close();
                qDebug()<<"串口打开成功,设备校验失败";
            }
        }
    }

    delete serial;
    serial = NULL;
}

bool SerialWorker::CheckDevice()
{
    int bCheck=2;
    QByteArray baSend, baRead;
    baSend.resize(2);
    baSend[0] = 0xF7;
    baSend[1] = 0xF7;

    if (serial!=NULL&&serial->isOpen())
    {
        while (bCheck!=0)
        {
            baRead.clear();
            serial->write(baSend);
            if(serial->waitForBytesWritten(100))
            {
                //check send success
                if (serial->waitForReadyRead(50))
                {
                    baRead = serial->readAll();
                    while (serial->waitForReadyRead(50))
                        baRead += serial->readAll();
                    QString response(baRead.toHex());
                    qDebug()<< "CheckDeviceResult:"<<response<<bCheck<<baRead.toHex();
                }

                if(baRead.toHex()=="f7a685")
                {
                    bCheck=0;
                    return true;
                }
                else
                {
                    qDebug()<<"bcheck decrease is "<<bCheck;
                    bCheck--;
                }
            }
            else
            {
                qDebug()<< "CheckDeviceResult:no response."<<bCheck;
                bCheck--;
            }
        }
    }
    return false;
}

void SerialWorker::SetEnable()
{
    QByteArray baSend, baRead;
    baSend.resize(2);
    baSend[0] = 0xF9;
    baSend[1] = 0xE8;
    if (serial!=NULL&&serial->isOpen())
    {
        serial->write(baSend);
        if(serial->waitForBytesWritten(100))
        {
            if (serial->waitForReadyRead(100))
            {
                baRead = serial->readAll();
                qDebug()<<"enable receive:  "<<baRead.toHex();
                if(baRead.toHex()=="f54b")
                {
                    qDebug()<<"Enable Succeeded";
                }
                else
                    qDebug()<<"Enable Failed";
            }
        }
    }
}

void SerialWorker::updateInitStauts()
{
    qDebug()<< "updateInitStauts";
    QByteArray baSend, baRead;
    baSend.resize(2);
    baSend[0] = 0xFA;
    baSend[1] = 0x0A;

    if (serial!=NULL&&serial->isOpen())
    {
        serial->write(baSend);
        if(serial->waitForBytesWritten(100))
        {
            if (serial->waitForReadyRead(100))
            {
                baRead = serial->readAll();
                while (serial->waitForReadyRead(50))
                    baRead += serial->readAll();
                QString response(baRead.toHex());
                qDebug()<< "updateStatus:"<<response;
                int len=baRead.length();
                if(len!=5)
                    return;
                if(baRead[2]&0x04){
                    qDebug()<< "脚踏开关初始：按下";
                    m_bSwitchStatusInit=true;
                }
                else{
                    qDebug()<< "脚踏开关初始：抬起";
                    m_bSwitchStatusInit=false;
                }
            }
        }
    }

}

bool SerialWorker::DataValidation(QByteArray data_in)
{
    QByteArray bin;
    bin.clear();
    bin=data_in;
    bin.remove(bin.length()-1,1);
    //qDebug()<<bin.toHex()<<m_baRead.toHex();
    unsigned char bCRC_Cal;
    bCRC_Cal=caluCRC((unsigned char*)bin.data(),bin.length());
    unsigned char bCRC_read=data_in.at(data_in.length()-1);
    //qDebug()<<bout<<bout2;
    if(bCRC_Cal==bCRC_read){
        return true;
    }
    else
        return false;
}

QByteArray SerialWorker::DataAddCRC(QByteArray data_in)
{
    unsigned char bCRC_Cal;
    bCRC_Cal=caluCRC((unsigned char*)data_in.data(),data_in.length());
    data_in.append(bCRC_Cal);
    return data_in;
}

unsigned char SerialWorker::caluCRC(unsigned char *p,unsigned char len)
{  //CRC8计算,Ret:CRC值
    unsigned char i,D,R=0;
    while(len--){
        D=*p++;
        for (i=0;i<8;i++){
            if ((R^D)&0x01){R^=0x18;R>>=1;R|=0x80;}
            else R>>=1;
            D>>=1;
        }
    }
    return R;
}

void SerialWorker::handleNewData()
{
    if(m_SerialReadBuffer.length()<=1){
        m_SerialReadBuffer.clear();
        return;
    }
    if(m_SerialReadBuffer.length()>=5){
        if(m_SerialReadBuffer.at(0) == (char)0xf5)
            m_SerialReadBuffer.remove(0,2);
        else if(m_SerialReadBuffer.at(3) == (char)0xf5)
            m_SerialReadBuffer.remove(m_SerialReadBuffer.length()-2,2);
    }
    if (m_SerialReadBuffer.at(0) == (char)0xd4 )
    {
        qDebug() << tr("脚踏开关-关 in Thread");
    }
    else if (m_SerialReadBuffer.at(0) == (char)0xd5)
    {
        qDebug() << tr("脚踏开关-开 in Thread");
    }
    m_SerialReadBuffer.remove(m_SerialReadBuffer.length()-1,1);
    emit this->serialReadReady(m_SerialReadBuffer);
    m_SerialReadBuffer.clear();
}
