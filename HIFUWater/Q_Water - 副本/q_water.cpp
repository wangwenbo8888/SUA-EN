#include "q_water.h"
#include <QByteArray>
#include <QDebug>
#include <QSettings>
#if _MSC_VER >1600
#pragma execution_character_set("utf-8")
#endif

Q_Water::Q_Water(QObject* parent) : QObject(parent)
{
//    init();
    m_Speed1 = 0;
    m_Speed2 = 0;
    relay = 0;
    Port_Status=true;
    b_Disable_Slot=false;
    m_SpdOut_Adj=0;
    RODX_POS=0,RODY_POS=0,RODZ_POS=0;
}

Q_Water::~Q_Water()
{
    if(exist())
    {
        if(m_serial->isOpen())
        {
            m_serial->close();
        }
        delete m_serial;
        qDebug()<<"delete m_serial";
    }
}

bool Q_Water::init()
{
    QSettings *settings = new QSettings(SETTINGS_PATH, QSettings::IniFormat);
    AutoSpeedIN = settings->value("Water/SpeedIN").toInt();
    AutoSpeedOUT = settings->value("Water/SpeedOUT").toInt();
    delete settings;
    qDebug()<<"water===="<<AutoSpeedIN<<AutoSpeedOUT;
    QList<QSerialPortInfo> infoList = QSerialPortInfo::availablePorts();
    if(infoList.count()==0){
        qDebug()<<"SerialPort not exist";
        return false;
    }
    m_serial = new QSerialPort();
    m_serial->setPortName("COM1");
    if (openSerial())
    {
        qDebug()<<"Open SerialPort: "<<m_serial->portName();
        setSerial();
        if (checkSerial()&&enableSerial()) {
            qDebug()<<m_serial->portName()<<"已连接水路控制器";
            connect(m_serial,SIGNAL(readyRead()),this,SLOT(ReadData()),Qt::QueuedConnection);
            if(checkBedZero())
            {
                qDebug()<<m_serial->portName()<<"床体校零已完成";
                emit BedZeroStatus(true);
            }
            else
            {
                qDebug()<<m_serial->portName()<<"床体校零未完成";
                emit BedZeroStatus(false);
            }
            return true;
        }
        else
            return false;
    }
    else
    {
    foreach (const QSerialPortInfo &serialPortInfo, infoList){
        m_serial->setPort(serialPortInfo);
        if (!openSerial()) continue;
        qDebug()<<"Open SerialPort: "<<m_serial->portName();
        setSerial();
        if (checkSerial()&&enableSerial()) {
            qDebug()<<m_serial->portName()<<"已连接水路控制器";
            connect(m_serial,SIGNAL(readyRead()),this,SLOT(ReadData()),Qt::QueuedConnection);
            return true;
        }
        else{
            qDebug()<<"Failed to connect Serialport: "<<m_serial->portName();
            m_serial->close();
            return false;
            }
        }
    }
    return false;
}

bool Q_Water::openSerial()
{
    bool opened = false;
    if (exist()) opened = m_serial->isOpen() ? true : m_serial->open(QIODevice::ReadWrite);
    if (opened)
        qDebug()<< "Opened the serial successfully.";
    else {
        qDebug()<< "Failed to open the serial.";
    }

    return opened;
}

void Q_Water::setSerial()
{
    m_serial->setBaudRate(QSerialPort::Baud9600);
    m_serial->setParity(QSerialPort::NoParity);
    m_serial->setDataBits(QSerialPort::Data8);
    m_serial->setStopBits(QSerialPort::OneStop);
    m_serial->setFlowControl(QSerialPort::NoFlowControl);
    m_serial->setReadBufferSize(0);
    m_serial->clearError();
    m_serial->clear();
}

bool Q_Water::enableSerial()
{
    bool enable = false;
    chSend[0]=COM_ENABLE;              //Get ID
    sendSerial(2);
    if(receiveSerial(2)!=2) return enable;
    if (chRead[0] != COM_DONE) {return enable;}
    chSend[0]=Bed_Enable;
    chSend[1]=0x01;
    sendSerial(3);
    enable = true;
    return enable;
}

bool Q_Water::checkSerial()
{
    //check device
    bool checked = false;
    chSend[0]=COM_GET_ID;              //Get ID
    sendSerial(2);
    if(receiveSerial(33)!=33) return checked;
    if (chRead[0] != COM_GET_ID) {return checked;}
    checked = true;
    return checked;
}

bool Q_Water::checkBedZero()
{
    bool checked = false;
    chSend[0]=COM_BED_ZERO;
    sendSerial(2);
    if(receiveSerial(3)!=3)
    {
        sendSerial(2);
        if(receiveSerial(3)!=3)
            return checked;
    }
    if ((chRead[0] != COM_BED_ZERO)||(chRead[1]!=0x02)) {return checked;}
    checked = true;
    return checked;
}

void Q_Water::SetBedZero()
{
    b_Disable_Slot=true;
    chSend[0]=COM_BED_SET_ZERO;      //set pump
    sendSerial(2);
    int num = receiveSerial(2);
    if(num!=2 || chRead[0] != COM_DONE)
    {
        sendSerial(2);
        int num2 = receiveSerial(2);
        if(num2!=2 || chRead[0] != COM_DONE)
        {
            qDebug()<<"Set Bed Zero Error";
        }
    }
    b_Disable_Slot=false;
}

void Q_Water::sendSerial(int num){      //发送信息,显示
    int i;
    QByteArray baSend;
    m_serial->clear();
    baSend.resize(num);
    chSend[num-1] = caluCRC(&chSend[0],num-1);
    for(i=0;i<num;i++){
        baSend[i]=chSend[i];
    }
    qDebug()<<"Send message："<<baSend.toHex();
    m_serial->write(baSend);
    if(!(m_serial->waitForBytesWritten(50)))
        qDebug()<<"Send False!!!!!!!!!!!!!!!!!!!!";
}

int Q_Water::receiveSerial(int num){     //在50ms内接收指定数量的信息,显示
    int n=0;
    if (num==0) num=255;
    if(!(m_serial->waitForReadyRead(200))){return -1;}

    n=m_serial->bytesAvailable();
    baRead=m_serial->readAll();
    while (n<num) {
        if(!(m_serial->waitForReadyRead(200))) break;
        n+=m_serial->bytesAvailable();
        baRead+=m_serial->readAll();
    }
    qDebug()<<"reveive message:"<<baRead.toHex();
    for(int i=0;i<n;i++){
       chRead[i]=baRead[i];
    }
    if (num==255) num=n;
    if (caluCRC(&chRead[0],num) == 0) return num;
    else{
        qDebug()<<"CRC Error";
        return -2;
    }
}

//CRC8 Table
unsigned char Tab_CRC8[256] = {
    0x00,0x5E,0xBC,0xE2,0x61,0x3F,0xDD,0x83, 0xC2,0x9C,0x7E,0x20,0xA3,0xFD,0x1F,0x41,
    0x9D,0xC3,0x21,0x7F,0xFC,0xA2,0x40,0x1E, 0x5F,0x01,0xE3,0xBD,0x3E,0x60,0x82,0xDC,
    0x23,0x7D,0x9F,0xC1,0x42,0x1C,0xFE,0xA0, 0xE1,0xBF,0x5D,0x03,0x80,0xDE,0x3C,0x62,
    0xBE,0xE0,0x02,0x5C,0xDF,0x81,0x63,0x3D, 0x7C,0x22,0xC0,0x9E,0x1D,0x43,0xA1,0xFF,
    0x46,0x18,0xFA,0xA4,0x27,0x79,0x9B,0xC5, 0x84,0xDA,0x38,0x66,0xE5,0xBB,0x59,0x07,
    0xDB,0x85,0x67,0x39,0xBA,0xE4,0x06,0x58, 0x19,0x47,0xA5,0xFB,0x78,0x26,0xC4,0x9A,
    0x65,0x3B,0xD9,0x87,0x04,0x5A,0xB8,0xE6, 0xA7,0xF9,0x1B,0x45,0xC6,0x98,0x7A,0x24,
    0xF8,0xA6,0x44,0x1A,0x99,0xC7,0x25,0x7B, 0x3A,0x64,0x86,0xD8,0x5B,0x05,0xE7,0xB9,
    0x8C,0xD2,0x30,0x6E,0xED,0xB3,0x51,0x0F, 0x4E,0x10,0xF2,0xAC,0x2F,0x71,0x93,0xCD,
    0x11,0x4F,0xAD,0xF3,0x70,0x2E,0xCC,0x92, 0xD3,0x8D,0x6F,0x31,0xB2,0xEC,0x0E,0x50,
    0xAF,0xF1,0x13,0x4D,0xCE,0x90,0x72,0x2C, 0x6D,0x33,0xD1,0x8F,0x0C,0x52,0xB0,0xEE,
    0x32,0x6C,0x8E,0xD0,0x53,0x0D,0xEF,0xB1, 0xF0,0xAE,0x4C,0x12,0x91,0xCF,0x2D,0x73,
    0xCA,0x94,0x76,0x28,0xAB,0xF5,0x17,0x49, 0x08,0x56,0xB4,0xEA,0x69,0x37,0xD5,0x8B,
    0x57,0x09,0xEB,0xB5,0x36,0x68,0x8A,0xD4, 0x95,0xCB,0x29,0x77,0xF4,0xAA,0x48,0x16,
    0xE9,0xB7,0x55,0x0B,0x88,0xD6,0x34,0x6A, 0x2B,0x75,0x97,0xC9,0x4A,0x14,0xF6,0xA8,
    0x74,0x2A,0xC8,0x96,0x15,0x4B,0xA9,0xF7, 0xB6,0xE8,0x0A,0x54,0xD7,0x89,0x6B,0x35
};
//CRC16
const uint8_t crctablehi[] = {
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
    0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
    0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01,
    0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81,
    0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01,
    0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
    0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
    0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01,
    0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
    0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01,
    0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
    0x40
};
/* Table of CRC values for low–order byte */
const uint8_t crctablelo[] = {
    0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06, 0x07, 0xC7, 0x05, 0xC5, 0xC4,
    0x04, 0xCC, 0x0C, 0x0D, 0xCD, 0x0F, 0xCF, 0xCE, 0x0E, 0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09,
    0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9, 0x1B, 0xDB, 0xDA, 0x1A, 0x1E, 0xDE, 0xDF, 0x1F, 0xDD,
    0x1D, 0x1C, 0xDC, 0x14, 0xD4, 0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3,
    0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3, 0xF2, 0x32, 0x36, 0xF6, 0xF7,
    0x37, 0xF5, 0x35, 0x34, 0xF4, 0x3C, 0xFC, 0xFD, 0x3D, 0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A,
    0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38, 0x28, 0xE8, 0xE9, 0x29, 0xEB, 0x2B, 0x2A, 0xEA, 0xEE,
    0x2E, 0x2F, 0xEF, 0x2D, 0xED, 0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26,
    0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60, 0x61, 0xA1, 0x63, 0xA3, 0xA2,
    0x62, 0x66, 0xA6, 0xA7, 0x67, 0xA5, 0x65, 0x64, 0xA4, 0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F,
    0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 0x69, 0xA9, 0xA8, 0x68, 0x78, 0xB8, 0xB9, 0x79, 0xBB,
    0x7B, 0x7A, 0xBA, 0xBE, 0x7E, 0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5,
    0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71, 0x70, 0xB0, 0x50, 0x90, 0x91,
    0x51, 0x93, 0x53, 0x52, 0x92, 0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C,
    0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E, 0x5A, 0x9A, 0x9B, 0x5B, 0x99, 0x59, 0x58, 0x98, 0x88,
    0x48, 0x49, 0x89, 0x4B, 0x8B, 0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C,
    0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42, 0x43, 0x83, 0x41, 0x81, 0x80,
    0x40
};

unsigned char Q_Water::caluCRC(unsigned char* ptr,unsigned char len){            //CRC8计算,Ret:CRC值
    unsigned char D,R=0;
    while(len--){
        D = *ptr++;
        R = Tab_CRC8[R^D];
    }
    return R;
}

uint16_t Q_Water::CRC16(uint8_t *Data, uint16_t len)
{
    uint8_t crchi = 0xff;
    uint8_t crclo = 0xff;
    uint16_t index;
    while (len--)
    {
        index = crclo ^ *Data++;
        crclo = crchi ^ crctablehi[index];
        crchi = crctablelo[index];
    }
    return (crchi << 8 | crclo);
}

void Q_Water::setPump1()
{
    b_Disable_Slot=true;
    chSend[0]=COM_SET_PUMP_1;      //set pump
    chSend[1]=m_Speed1>>8 & 0xff;
    chSend[2]=m_Speed1 & 0xff;
    sendSerial(4);
    int num = receiveSerial(2);
    if(num!=2 || chRead[0] != COM_DONE)
    {
        sendSerial(4);
        int num2 = receiveSerial(2);
        if(num2!=2 || chRead[0] != COM_DONE)
        {
            qDebug()<<"Set Pump 1 data Error";
        }
    }
    b_Disable_Slot=false;
}

void Q_Water::setPump2()
{
    b_Disable_Slot=true;
    chSend[0]=COM_SET_PUMP_2;      //set pump
    chSend[1]=m_Speed2>>8 & 0xff;
    chSend[2]=m_Speed2 & 0xff;
    sendSerial(4);
    int num = receiveSerial(2);
    if(num!=2 || chRead[0] != COM_DONE)
    {
        sendSerial(4);
        int num2 = receiveSerial(2);
        if(num2!=2 || chRead[0] != COM_DONE)
        {
            sendSerial(4);
            int num3 = receiveSerial(2);
            if(num3!=2 || chRead[0] != COM_DONE)
            {
                qDebug()<<"Set Pump 2 data Error";
            }
        }
    }
    b_Disable_Slot=false;
}

void Q_Water::setduPump1(int speed1)
{
    m_Speed1 = (speed1*10) & 0x7FFF;
    qDebug()<<"speed1 = "<<m_Speed1;
    setPump1();
    emit getPump1(speed1);
}

void Q_Water::setduPump2(int speed2)
{
    m_Speed2 = (speed2*10) & 0x7FFF;
    qDebug()<<"speed2 = "<<m_Speed2;
    setPump2();
    emit getPump2(speed2);
}

void Q_Water::closeduPump1()
{
    m_Speed1 |= 0x8000;
    qDebug()<<"close pump1";
    setPump1();
}

void Q_Water::closeduPump2()
{
    m_Speed2 |= 0x8000;
    qDebug()<<"close pump2";
    setPump2();
}

void Q_Water::OpenValveIN()
{
    b_Disable_Slot=true;
    relay |=RELAY_ValveIN;
    chSend[0]=COM_SET_RELAY;      //set relay
    chSend[1]=relay>>8 & 0xff;
    chSend[2]=relay & 0xff;
    sendSerial(4);
    int num = receiveSerial(2);
    if(num!=2 || chRead[0] != COM_DONE)
    {
        sendSerial(4);
        int num2 = receiveSerial(2);
        if(num2!=2 || chRead[0] != COM_DONE)
        {
            qDebug()<<"Set ValveIN data Error";
        }
    }
    b_Disable_Slot=false;
}

void Q_Water::CloseValveIN()
{
    b_Disable_Slot=true;
    relay &=(~RELAY_ValveIN);
    chSend[0]=COM_SET_RELAY;      //set relay
    chSend[1]=relay>>8 & 0xff;
    chSend[2]=relay & 0xff;
    sendSerial(4);
    int num = receiveSerial(2);
    if(num!=2 || chRead[0] != COM_DONE)
    {
        sendSerial(4);
        int num2 = receiveSerial(2);
        if(num2!=2 || chRead[0] != COM_DONE)
        {
            qDebug()<<"Set ValveIN data Error";
        }
    }
    b_Disable_Slot=false;
}

void Q_Water::OpenValveOUT()
{
    b_Disable_Slot=true;
    relay |=RELAY_ValveOUT;
    chSend[0]=COM_SET_RELAY;      //set relay
    chSend[1]=relay>>8 & 0xff;
    chSend[2]=relay & 0xff;
    sendSerial(4);
    int num = receiveSerial(2);
    if(num!=2 || chRead[0] != COM_DONE)
    {
        sendSerial(4);
        int num2 = receiveSerial(2);
        if(num2!=2 || chRead[0] != COM_DONE)
        {
            qDebug()<<"Set ValveOUT data Error";
        }
    }
    b_Disable_Slot=false;
}

void Q_Water::CloseValveOUT()
{
    b_Disable_Slot=true;
    relay &=(~RELAY_ValveOUT);
    chSend[0]=COM_SET_RELAY;      //set relay
    chSend[1]=relay>>8 & 0xff;
    chSend[2]=relay & 0xff;
    sendSerial(4);
    int num = receiveSerial(2);
    if(num!=2 || chRead[0] != COM_DONE)
    {
        sendSerial(4);
        int num2 = receiveSerial(2);
        if(num2!=2 || chRead[0] != COM_DONE)
        {
            qDebug()<<"Set ValveOUT data Error";
        }
    }
    b_Disable_Slot=false;
}

void Q_Water::OpenWaterOut()
{
    b_Disable_Slot=true;
    relay |=RELAY_WaterOut;
    chSend[0]=COM_SET_RELAY;      //set relay
    chSend[1]=relay>>8 & 0xff;
    chSend[2]=relay & 0xff;
    sendSerial(4);
    int num = receiveSerial(2);
    if(num!=2 || chRead[0] != COM_DONE)
    {
        sendSerial(4);
        int num2 = receiveSerial(2);
        if(num2!=2 || chRead[0] != COM_DONE)
        {
            qDebug()<<"Set WaterOut data Error";
        }
    }
    b_Disable_Slot=false;
}

void Q_Water::CloseWaterOut()
{
    b_Disable_Slot=true;
    relay &=(~RELAY_WaterOut);
    chSend[0]=COM_SET_RELAY;      //set relay
    chSend[1]=relay>>8 & 0xff;
    chSend[2]=relay & 0xff;
    sendSerial(4);
    int num = receiveSerial(2);
    if(num!=2 || chRead[0] != COM_DONE)
    {
        sendSerial(4);
        int num2 = receiveSerial(2);
        if(num2!=2 || chRead[0] != COM_DONE)
        {
            qDebug()<<"Set WaterOut data Error";
        }
    }
    b_Disable_Slot=false;
}

void Q_Water::OpenValveBottle()
{
    b_Disable_Slot=true;
    relay |=RELAY_ValveBTT;
    chSend[0]=COM_SET_RELAY;      //set relay
    chSend[1]=relay>>8 & 0xff;
    chSend[2]=relay & 0xff;
    sendSerial(4);
    int num = receiveSerial(2);
    if(num!=2 || chRead[0] != COM_DONE)
    {
        sendSerial(4);
        int num2 = receiveSerial(2);
        if(num2!=2 || chRead[0] != COM_DONE)
        {
            qDebug()<<"Set ValveBottle data Error";
        }
    }
    b_Disable_Slot=false;
}

void Q_Water::CloseValveBottle()
{
    b_Disable_Slot=true;
    relay &=(~RELAY_ValveBTT);
    chSend[0]=COM_SET_RELAY;      //set relay
    chSend[1]=relay>>8 & 0xff;
    chSend[2]=relay & 0xff;
    sendSerial(4);
    int num = receiveSerial(2);
    if(num!=2 || chRead[0] != COM_DONE)
    {
        sendSerial(4);
        int num2 = receiveSerial(2);
        if(num2!=2 || chRead[0] != COM_DONE)
        {
            qDebug()<<"Set ValveBottle data Error";
        }
    }
    b_Disable_Slot=false;
}

void Q_Water::OpenZKPump()
{
    b_Disable_Slot=true;
    relay |=RELAY_ZKPump;
    chSend[0]=COM_SET_RELAY;      //set relay
    chSend[1]=relay>>8 & 0xff;
    chSend[2]=relay & 0xff;
    sendSerial(4);
    int num = receiveSerial(2);
    if(num!=2 || chRead[0] != COM_DONE)
    {
        sendSerial(4);
        int num2 = receiveSerial(2);
        if(num2!=2 || chRead[0] != COM_DONE)
        {
            qDebug()<<"Set ZKPump data Error";
        }
    }
    b_Disable_Slot=false;
}

void Q_Water::CloseZKPump()
{
    b_Disable_Slot=true;
    relay &=(~RELAY_ZKPump);
    chSend[0]=COM_SET_RELAY;      //set relay
    chSend[1]=relay>>8 & 0xff;
    chSend[2]=relay & 0xff;
    sendSerial(4);
    int num = receiveSerial(2);
    if(num!=2 || chRead[0] != COM_DONE)
    {
        sendSerial(4);
        int num2 = receiveSerial(2);
        if(num2!=2 || chRead[0] != COM_DONE)
        {
            qDebug()<<"Set ZKPump data Error";
        }
    }
    b_Disable_Slot=false;
}

void Q_Water::OpenPumpOUT()
{
    b_Disable_Slot=true;
    relay |=RELAY_PumpOUT;
    chSend[0]=COM_SET_RELAY;      //set relay
    chSend[1]=relay>>8 & 0xff;
    chSend[2]=relay & 0xff;
    sendSerial(4);
    int num = receiveSerial(2);
    if(num!=2 || chRead[0] != COM_DONE)
    {
        sendSerial(4);
        int num2 = receiveSerial(2);
        if(num2!=2 || chRead[0] != COM_DONE)
        {
            qDebug()<<"Set PumpOUT data Error";
        }
    }
    b_Disable_Slot=false;
}

void Q_Water::ClosePumpOUT()
{
    b_Disable_Slot=true;
    relay &=(~RELAY_PumpOUT);
    chSend[0]=COM_SET_RELAY;      //set relay
    chSend[1]=relay>>8 & 0xff;
    chSend[2]=relay & 0xff;
    sendSerial(4);
    int num = receiveSerial(2);
    if(num!=2 || chRead[0] != COM_DONE)
    {
        sendSerial(4);
        int num2 = receiveSerial(2);
        if(num2!=2 || chRead[0] != COM_DONE)
        {
            qDebug()<<"Set PumpOUT data Error";
        }
    }
    b_Disable_Slot=false;
}

void Q_Water::OpenCooler1()
{
    b_Disable_Slot=true;
    relay |=RELAY_Cooler1;
    chSend[0]=COM_SET_RELAY;      //set relay
    chSend[1]=relay>>8 & 0xff;
    chSend[2]=relay & 0xff;
    sendSerial(4);
    int num = receiveSerial(2);
    if(num!=2 || chRead[0] != COM_DONE)
    {
        sendSerial(4);
        int num2 = receiveSerial(2);
        if(num2!=2 || chRead[0] != COM_DONE)
        {
            qDebug()<<"Set Cooler1 data Error";
        }
    }
    b_Disable_Slot=false;
}

void Q_Water::CloseCooler1()
{
    b_Disable_Slot=true;
    relay &=(~RELAY_Cooler1);
    chSend[0]=COM_SET_RELAY;      //set relay
    chSend[1]=relay>>8 & 0xff;
    chSend[2]=relay & 0xff;
    sendSerial(4);
    int num = receiveSerial(2);
    if(num!=2 || chRead[0] != COM_DONE)
    {
        sendSerial(4);
        int num2 = receiveSerial(2);
        if(num2!=2 || chRead[0] != COM_DONE)
        {
            qDebug()<<"Set Cooler1 data Error";
        }
    }
    b_Disable_Slot=false;
}

void Q_Water::OpenCooler2()
{
    b_Disable_Slot=true;
    relay |=RELAY_Cooler2;
    chSend[0]=COM_SET_RELAY;      //set relay
    chSend[1]=relay>>8 & 0xff;
    chSend[2]=relay & 0xff;
    sendSerial(4);
    int num = receiveSerial(2);
    if(num!=2 || chRead[0] != COM_DONE)
    {
        sendSerial(4);
        int num2 = receiveSerial(2);
        if(num2!=2 || chRead[0] != COM_DONE)
        {
            qDebug()<<"Set Cooler2 data Error";
        }
    }
    b_Disable_Slot=false;
}

void Q_Water::CloseCooler2()
{
    b_Disable_Slot=true;
    relay &=(~RELAY_Cooler2);
    chSend[0]=COM_SET_RELAY;      //set relay
    chSend[1]=relay>>8 & 0xff;
    chSend[2]=relay & 0xff;
    sendSerial(4);
    int num = receiveSerial(2);
    if(num!=2 || chRead[0] != COM_DONE)
    {
        sendSerial(4);
        int num2 = receiveSerial(2);
        if(num2!=2 || chRead[0] != COM_DONE)
        {
            qDebug()<<"Set Cooler2 data Error";
        }
    }
    b_Disable_Slot=false;
}

void Q_Water::OpenCooler3()
{
    b_Disable_Slot=true;
    relay |=RELAY_Cooler3;
    chSend[0]=COM_SET_RELAY;      //set relay
    chSend[1]=relay>>8 & 0xff;
    chSend[2]=relay & 0xff;
    sendSerial(4);
    int num = receiveSerial(2);
    if(num!=2 || chRead[0] != COM_DONE)
    {
        sendSerial(4);
        int num2 = receiveSerial(2);
        if(num2!=2 || chRead[0] != COM_DONE)
        {
            qDebug()<<"Set Cooler3 data Error";
        }
    }
    b_Disable_Slot=false;
}

void Q_Water::CloseCooler3()
{
    b_Disable_Slot=true;
    relay &=(~RELAY_Cooler3);
    chSend[0]=COM_SET_RELAY;      //set relay
    chSend[1]=relay>>8 & 0xff;
    chSend[2]=relay & 0xff;
    sendSerial(4);
    int num = receiveSerial(2);
    if(num!=2 || chRead[0] != COM_DONE)
    {
        sendSerial(4);
        int num2 = receiveSerial(2);
        if(num2!=2 || chRead[0] != COM_DONE)
        {
            qDebug()<<"Set Cooler3 data Error";
        }
    }
    b_Disable_Slot=false;
}

void Q_Water::Cooler(int number)
{
}

void Q_Water::getWater(bool enable)
{
    qDebug()<<"getwater1111111111111";
    b_Disable_Slot=true;
    chSend[0]=COM_GET_METER;      //get meter
    if(enable)
        chSend[1]=0x01;
    else
        chSend[1]=0x00;
    sendSerial(3);
    qDebug()<<"getwater22222222222222222";
    int num = receiveSerial(2);
    if(num!=2 || chRead[0] != COM_DONE)
    {
        sendSerial(3);
        int num2 = receiveSerial(2);
        if(num2!=2 || chRead[0] != COM_DONE)
        {
            qDebug()<<"Send Get_Water data Error";
        }
    }
    b_Disable_Slot=false;
}

void Q_Water::openAutoMode()
{
    qDebug()<<"open auto mode 111111111";
    waterAutoOpen = true;
    setduPump1(AutoSpeedIN);
    setduPump2(AutoSpeedOUT);
    OpenZKPump();
    connect(this,SIGNAL(signalAdjPumpOut()),this,SLOT(slotAdjPumpOut()));
    m_SpdOut_Set = AutoSpeedOUT;
    m_SpdOut_Adj=0;
}

void Q_Water::closeAutoMode()
{
    waterAutoOpen = false;
    closeduPump1();
    closeduPump2();
    CloseZKPump();
    disconnect(this,SIGNAL(signalAdjPumpOut()),this,SLOT(slotAdjPumpOut()));
}

void Q_Water::setMeter(int meter)
{
    m_PreMeter = meter;
}

void Q_Water::meterAutoMode()
{
    if(waterAutoOpen)
    {
        int RecMeter = m_RecMeter;
        if(RecMeter>32767)
            RecMeter=0;
        int dlt = RecMeter-m_PreMeter;
        bool b_chg = false;
        switch(m_SpdOut_Adj)
        {
        case 20:
            if (dlt<=5){
                m_SpdOut_Adj=5;
                b_chg=true;
            }
            break;
        case -20:
            if (dlt>=-5){
                m_SpdOut_Adj=-5;
                b_chg=true;
            }
            break;
        case 5:
            if (dlt>10){
                m_SpdOut_Adj=20;
                b_chg=true;
            }else if (dlt<=0){
                m_SpdOut_Adj=0;
                b_chg=true;
            }
            break;
        case -5:
            if (dlt<-10){
                m_SpdOut_Adj=-20;
                b_chg=true;
            }else if (dlt>=0){
                m_SpdOut_Adj=0;
                b_chg=true;
            }
            break;
        case 0:
            if (dlt>5){
                m_SpdOut_Adj=5;
                b_chg=true;
            }else if (dlt<-5){
                m_SpdOut_Adj=-5;
                b_chg=true;
            }
            break;
        default:
            m_SpdOut_Adj=0;
            break;
        }
        if(b_chg)
            emit signalAdjPumpOut();
    }
}

void Q_Water::slotAdjPumpOut()
{
    setduPump2(m_SpdOut_Set+m_SpdOut_Adj);
    emit SpeedOutUpdate(m_SpdOut_Set+m_SpdOut_Adj);
}

int Q_Water::receiveSerial_Status(int num)
{
    int n=0;
    QByteArray baRead;
    if (num==0) num=255;
    if(!(m_serial->waitForReadyRead(100))){qDebug()<<"TimeOut Error";return -1;}

    n=m_serial->bytesAvailable();
    baRead=m_serial->readAll();
    while (n<num) {
        if(!(m_serial->waitForReadyRead(100))) break;
        n+=m_serial->bytesAvailable();
        baRead+=m_serial->readAll();
    }
//    qDebug()<<"传感器接收消息："<<baRead.toHex();
//    qDebug()<<"baRead:"<<baRead.toHex();
    for(int i=0;i<n;i++){
       chRead[i]=baRead[i];
    }
    if (num==255) num=n;
    if (CRC16(&chRead[0],num) == 0) return num;
    else{
        return -2;
    }
}

void Q_Water::Delay_MSec(unsigned int msec)
{
    QTime _Timer = QTime::currentTime().addMSecs(msec);
    while(QTime::currentTime() < _Timer)
        QCoreApplication::processEvents(QEventLoop::AllEvents,100);
}

void Q_Water::UpdateAuto(bool isauto)
{
    if(isauto)
    {
        b_Disable_Slot=true;
        chSend[0]=COM_AUTOMODE;
        chSend[1]=1;
        sendSerial(3);
        int num = receiveSerial(2);
        b_Disable_Slot=false;
        if(num!=2 || chRead[0] != COM_DONE) {qDebug()<<"Set auto data Error";}
    }
    else
    {
        b_Disable_Slot=true;
        chSend[0]=COM_AUTOMODE;
        chSend[1]=0;
        sendSerial(3);
        int num = receiveSerial(2);
        b_Disable_Slot=false;
        if(num!=2 || chRead[0] != COM_DONE) {qDebug()<<"Set auto data Error";}
    }
}

void Q_Water::ReadData()
{
    if(b_Disable_Slot)return;
    int n=0;
    QByteArray baRead;
//    if(!(m_serial->waitForReadyRead(100))){qDebug()<<"TimeOut Error";}

    n=m_serial->bytesAvailable();
//    if(n==0)
//    {
//        qDebug()<<"message=====0";
//        return;
//    }
    baRead=m_serial->readAll();
//    while (1)
//    {
//        disconnect(m_serial,SIGNAL(readyRead()),this,SLOT(ReadData()));
//        if(!(m_serial->waitForReadyRead(100))) break;
//        n+=m_serial->bytesAvailable();
//        baRead+=m_serial->readAll();
//        QByteArray ba=m_serial->readAll();
//        qDebug()<<"ba data====="<<ba.toHex();
//    }
//    connect(m_serial,SIGNAL(readyRead()),this,SLOT(ReadData()),Qt::QueuedConnection);
    qDebug()<<"receive message num====="<<n;
    qDebug()<<"receive message data====="<<baRead.toHex();
    m_serial->clear();
    if(baRead.at(0)==(char)COM_ROD_KEY)
    {
        switch(baRead[1])
        {
            case ROD_COM1:
                 break;
            case ROD_COM2:
                 break;
            case ROD_COM3:
                 break;
            case ROD_COM4:
                 break;
            case ROD_COM5:
                 break;
            case ROD_COM6:
                 break;
        }
    }
    else if(baRead.at(0)==(char)COM_ELE_KEY)
    {
        qDebug()<<"baRead=======COM_ELE_KEY";
        switch(baRead[1])
        {
            case ELE_COM1:
                if(baRead.at(2)==(char)0x00)
                {
                    qDebug()<<"ELE_COM1=======0";
                    emit Pos_control(2,0);
                    break;
                }
                else if(baRead.at(2)==(char)0x01)
                {
                    qDebug()<<"ELE_COM1=======1";
                    emit Pos_control(2,-1);
                    break;
                }
            case ELE_COM2:
                if(baRead.at(2)==(char)0x00)
                {
                    qDebug()<<"ELE_COM2=======0";
                    emit Pos_control(2,0);
                    break;
                }
                else if(baRead.at(2)==(char)0x01)
                {
                    qDebug()<<"ELE_COM2=======1";
                    emit Pos_control(2,1);
                    break;
                }
            case ELE_COM3:
                if(baRead.at(2)==(char)0x00)
                {
                    qDebug()<<"ELE_COM3=======0";
                    emit Pos_control(1,0);
                    break;
                }
                else if(baRead.at(2)==(char)0x01)
                {
                    qDebug()<<"ELE_COM3=======1";
                    emit Pos_control(1,-1);
                    break;
                }
            case ELE_COM4:
                if(baRead.at(2)==(char)0x00)
                {
                    qDebug()<<"ELE_COM4=======0";
                    emit Pos_control(1,0);
                    break;
                }
                else if(baRead.at(2)==(char)0x01)
                {
                    qDebug()<<"ELE_COM4=======1";
                    emit Pos_control(1,1);
                    break;
                }
            case ELE_COM5:
                if(baRead.at(2)==(char)0x00)
                {
                    qDebug()<<"ELE_COM5=======0";
                    emit Pos_control(0,0);
                    break;
                }
                else if(baRead.at(2)==(char)0x01)
                {
                    qDebug()<<"ELE_COM5=======1";
                    emit Pos_control(0,-1);
                    break;
                }
            case ELE_COM6:
                if(baRead.at(2)==(char)0x00)
                {
                    qDebug()<<"ELE_COM6=======0";
                    emit Pos_control(0,0);
                    break;
                }
                else if(baRead.at(2)==(char)0x01)
                {
                    qDebug()<<"ELE_COM6=======1";
                    emit Pos_control(0,1);
                    break;
                }

        }
    }
    else if(baRead.at(0)==(char)COM_BED_KEY)
    {
        switch(baRead[1])
        {
            case BED_HEADOUT:
                if(baRead.at(2)==(char)0x00)
                {
                    emit Button_HeadOut(false);
                    break;
                }
                else if(baRead.at(2)==0x01)
                {
                    emit Button_HeadOut(true);
                    break;
                }
            case BED_WATEROUT:
                if(baRead.at(2)==(char)0x00)
                {
                    emit Button_WaterOut();
                    break;
                }
            case BED_WATERIN:
                if(baRead.at(2)==(char)0x00)
                {
                    emit Button_WaterIn();
                    break;
                }
            default:
                break;
        }
    }
    else if(baRead.at(0)==(char)COM_GET_METER/*&&n==12*/)
    {
        if(n<12)
        {
            disconnect(m_serial,SIGNAL(readyRead()),this,SLOT(ReadData()));
            while (1)
            {
                if(!(m_serial->waitForReadyRead(50))) break;
                n+=m_serial->bytesAvailable();
                QByteArray ba = m_serial->readAll();
                qDebug()<<"ba data====="<<ba.toHex();
                baRead+=ba;
                qDebug()<<"baRead data====="<<baRead.toHex();
                if(n>=12) break;
            }
            connect(m_serial,SIGNAL(readyRead()),this,SLOT(ReadData()),Qt::QueuedConnection);
        }
        if(n==12)
        {
            m_RecMeter=(baRead[1]&0xff)*256+(baRead[2]&0xff);
            m_Level=(baRead[3]&0xff)*256+(baRead[4]&0xff);
            m_Vacuum=(baRead[5]&0xff)*256+(baRead[6]&0xff);
            m_Temp=(baRead[7]&0xff)*256+(baRead[8]&0xff);
            m_Temp2=(baRead[9]&0xff)*256+(baRead[10]&0xff);
            emit MeterUpdate(m_RecMeter);
            emit VacuumUpdate(m_Vacuum);
            emit LevelUpdate(m_Level);
            emit TempUpdate(m_Temp);
            emit Temp2Update(m_Temp2);
            meterAutoMode();
        }
    }
    else if(baRead.at(0)==(char)COM_RODX_POS)
    {
        RODX_POS=(baRead[1]&0xff)*256+(baRead[2]&0xff)-13;
        emit GetBedPos(RODX_POS,RODY_POS,RODZ_POS);
    }
    else if(baRead.at(0)==(char)COM_RODY_POS)
    {
        RODY_POS=(baRead[1]&0xff)*256+(baRead[2]&0xff)-13;
        emit GetBedPos(RODX_POS,RODY_POS,RODZ_POS);
    }
    else if(baRead.at(0)==(char)COM_RODZ_POS)
    {
        RODZ_POS=(baRead[1]&0xff)*256+(baRead[2]&0xff)-15;
        emit GetBedPos(RODX_POS,RODY_POS,RODZ_POS);
    }
}

void Q_Water::GetRodPos()
{
    b_Disable_Slot=true;
    chSend[0]=COM_QUERY_ROD;      //set relay
    sendSerial(2);
    int num = receiveSerial(8);
    if(num!=8 || chRead[0] != COM_QUERY_ROD)
    {
        sendSerial(2);
        int num2 = receiveSerial(8);
        if(num2!=8 || chRead[0] != COM_QUERY_ROD)
        {
            qDebug()<<"QUERY ROD POS Error";
            return;
        }
    }
    RODX_POS=(baRead[1]&0xff)*256+(baRead[2]&0xff)-13;
    RODY_POS=(baRead[3]&0xff)*256+(baRead[4]&0xff)-13;
    RODZ_POS=(baRead[5]&0xff)*256+(baRead[6]&0xff)-15;
    emit GetBedPos(RODX_POS,RODY_POS,RODZ_POS);
    b_Disable_Slot=false;
}

void Q_Water::SetRodPos(int id,int direction, int pos)
{
    int num;
    switch(id)
    {
        case 1:
            b_Disable_Slot=true;
            chSend[0]=COM_RODX_CONTROL;
            chSend[1]=direction;
            chSend[2]=pos>>8 & 0xff;
            chSend[3]=pos & 0xff;
            sendSerial(5);
            num = receiveSerial(2);
            if(num!=2 || chRead[0] != COM_DONE)
            {
                sendSerial(5);
                int num2 = receiveSerial(2);
                if(num2!=2 || chRead[0] != COM_DONE)
                {
                    qDebug()<<"Set RODX data Error";
                }
            }
            b_Disable_Slot=false;
            break;
        case 2:
            b_Disable_Slot=true;
            chSend[0]=COM_RODY_CONTROL;
            chSend[1]=direction;
            chSend[2]=pos>>8 & 0xff;
            chSend[3]=pos & 0xff;
            sendSerial(5);
            num = receiveSerial(2);
            if(num!=2 || chRead[0] != COM_DONE)
            {
                sendSerial(5);
                int num2 = receiveSerial(2);
                if(num2!=2 || chRead[0] != COM_DONE)
                {
                    qDebug()<<"Set RODY data Error";
                }
            }
            b_Disable_Slot=false;
            break;
        case 3:
            b_Disable_Slot=true;
            chSend[0]=COM_RODZ_CONTROL;
            chSend[1]=direction;
            chSend[2]=pos>>8 & 0xff;
            chSend[3]=pos & 0xff;
            sendSerial(5);
            num = receiveSerial(2);
            if(num!=2 || chRead[0] != COM_DONE)
            {
                sendSerial(5);
                int num2 = receiveSerial(2);
                if(num2!=2 || chRead[0] != COM_DONE)
                {
                    qDebug()<<"Set RODZ data Error";
                }
            }
            b_Disable_Slot=false;
            break;
    }
}

void Q_Water::BedLock(bool flag)
{
    if(flag)
    {
        b_Disable_Slot=true;
        chSend[0]=COM_BED_STATUS;
        chSend[1]=COM_BED_LOCK;
        sendSerial(3);
        int num = receiveSerial(2);
        if(num!=2 || chRead[0] != COM_DONE)
        {
            sendSerial(3);
            int num2 = receiveSerial(2);
            if(num2!=2 || chRead[0] != COM_DONE)
            {
                qDebug()<<"Set Bed Lock Error";
            }
        }
        b_Disable_Slot=false;
    }
    else
    {
        b_Disable_Slot=true;
        chSend[0]=COM_BED_STATUS;
        chSend[1]=COM_BED_UNLOCK;
        sendSerial(3);
        int num = receiveSerial(2);
        if(num!=2 || chRead[0] != COM_DONE)
        {
            sendSerial(3);
            int num2 = receiveSerial(2);
            if(num2!=2 || chRead[0] != COM_DONE)
            {
                qDebug()<<"Set Bed Unlock Error";
            }
        }
        b_Disable_Slot=false;
    }
}

void Q_Water::SendScreenWaterIn()
{
    b_Disable_Slot=true;
    chSend[0]=COM_SCREEN;
    chSend[1]=SCREEN_WATERIN;
    sendSerial(3);
    int num = receiveSerial(2);
    if(num!=2 || chRead[0] != COM_DONE)
    {
        sendSerial(3);
        int num2 = receiveSerial(2);
        if(num2!=2 || chRead[0] != COM_DONE)
        {
            qDebug()<<"Set Screen Words Error";
        }
    }
    b_Disable_Slot=false;
}

void Q_Water::SendScreenWaterOut()
{
    b_Disable_Slot=true;
    chSend[0]=COM_SCREEN;
    chSend[1]=SCREEN_WATEROUT;
    sendSerial(3);
    int num = receiveSerial(2);
    if(num!=2 || chRead[0] != COM_DONE)
    {
        sendSerial(3);
        int num2 = receiveSerial(2);
        if(num2!=2 || chRead[0] != COM_DONE)
        {
            qDebug()<<"Set Screen Words Error";
        }
    }
    b_Disable_Slot=false;
}

void Q_Water::SendScreenNULL()
{
    b_Disable_Slot=true;
    chSend[0]=COM_SCREEN;
    chSend[1]=SCREEN_NULL;
    sendSerial(3);
    int num = receiveSerial(2);
    if(num!=2 || chRead[0] != COM_DONE)
    {
        sendSerial(3);
        int num2 = receiveSerial(2);
        if(num2!=2 || chRead[0] != COM_DONE)
        {
            qDebug()<<"Set Screen Words Error";
        }
    }
    b_Disable_Slot=false;
}

QSerialPort* Q_Water::getserial()
{
    return m_serial;
}
