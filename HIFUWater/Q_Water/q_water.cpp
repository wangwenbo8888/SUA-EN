#include "q_water.h"
#include <QByteArray>
#include <QDebug>
#include <QSettings>
#include "constant.h"

#if _MSC_VER >1600
#pragma execution_character_set("utf-8")
#endif

Q_LOGGING_CATEGORY(QWATER,"QWATER")

Q_Water::Q_Water(QObject* parent) : QObject(parent)
{
//    init();
    m_Speed1 = 0;
    m_Speed2 = 0;
    isBedLock = false;
    BedLockFlag = false;
    relay = 0;
    Port_Status=true;
    b_Disable_Slot=false;
    m_SpdOut_Adj=0;
    RODX_POS=0,RODY_POS=0,RODZ_POS=0;
    HeartSendTimer = new QTimer(this);
    HeartSendTimer->setInterval(500);
    HeartSendTimer->setSingleShot(false);
    connect(HeartSendTimer,SIGNAL(timeout()),this,SLOT(SendHeartbeat()));
    WaterInTimer = new QTimer(this);
    WaterInTimer->setInterval(500);
    WaterInTimer->setSingleShot(true);
    connect(WaterInTimer,SIGNAL(timeout()),this,SLOT(ReSendWaterIn()));
    WaterOutTimer = new QTimer(this);
    WaterOutTimer->setInterval(500);
    WaterOutTimer->setSingleShot(true);
    connect(WaterOutTimer,SIGNAL(timeout()),this,SLOT(ReSendWaterOut()));
    WaterCycleTimer = new QTimer(this);
    WaterCycleTimer->setInterval(500);
    WaterCycleTimer->setSingleShot(true);
    connect(WaterCycleTimer,SIGNAL(timeout()),this,SLOT(ReSendWaterCycle()));
    CoolerTimer = new QTimer(this);
    CoolerTimer->setInterval(500);
    CoolerTimer->setSingleShot(true);
    connect(CoolerTimer,SIGNAL(timeout()),this,SLOT(ReSendWaterCooler2()));
    BedLockTimer = new QTimer(this);
    BedLockTimer->setInterval(500);
    BedLockTimer->setSingleShot(true);
    connect(BedLockTimer,SIGNAL(timeout()),this,SLOT(ReSendBedLock()));
}

Q_Water::~Q_Water()
{
    if(exist())
    {

    }
}

bool Q_Water::init()
{
    QSettings *settings = new QSettings(SETTINGS_PATH, QSettings::IniFormat);
    AutoSpeedIN = settings->value("Water/SpeedIN").toInt();
    AutoSpeedOUT = settings->value("Water/SpeedOUT").toInt();
    Level_Adj = settings->value("Water/Level_adj").toInt();
    delete settings;
    qDebug()<<"water===="<<AutoSpeedIN<<AutoSpeedOUT;
    Delay_MSec(6000);
    qDebug()<<"after websocket";
    checkSerial();
    checkBedZero();
    Delay_MSec(3000);
    if(!checked_serial)
    {
        checkSerial();
    }
    Delay_MSec(1000);
    SendLevelRef();
    HeartSendTimer->start();
    if (checked_serial) {
        if(checked_bedzero)
        {
            qDebug()<<"床体校零已完成";
            emit BedZeroStatus(true);
            return true;
        }
        else
        {
            qDebug()<<"床体校零未完成";
            emit BedZeroStatus(false);
        }
        return true;
    }
    else{
        return false;
    }
//    }
//    else
//    {
//    foreach (const QSerialPortInfo &serialPortInfo, infoList){
//        m_serial->setPort(serialPortInfo);
//        if (!openSerial()) continue;
//        qDebug()<<"Open SerialPort: "<<m_serial->portName();
//        setSerial();
//        if (checkSerial()) {
//            qDebug()<<m_serial->portName()<<"已连接水路控制器";
//            connect(m_serial,SIGNAL(readyRead()),this,SLOT(ReadData()),Qt::QueuedConnection);
//            return true;
//        }
//        else{
//            qDebug()<<"Failed to connect Serialport: "<<m_serial->portName();
//            m_serial->close();
//            return false;
//            }
//        }
//    }
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
    m_serial->setBaudRate(QSerialPort::Baud115200);
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
    chSend[0]=COM_Water;              //Get ID
    chSend[1]=COM_EN;
    chSend[2]=0x00;
    chSend[3]=0x00;
    sendSerial(5);
    return true;
}

bool Q_Water::checkBedZero()
{
    chSend[0]=COM_Bed;
    chSend[1]=COM_BedZero;
    chSend[2]=0x01;
    chSend[3]=0x00;
    chSend[4]=0x01;
    sendSerial(6);
    return true;
}

void Q_Water::SetBedZero()
{
    b_Disable_Slot=true;
    chSend[0]=COM_Bed;
    chSend[1]=COM_BED_SET_ZERO;
    chSend[2]=0x01;
    chSend[3]=0x00;
    chSend[4]=0x01;
    sendSerial(6);
}

void Q_Water::sendSerial(int num){      //发送信息,显示

    int i;
    QByteArray baSend;
    baSend.resize(num);
    chSend[num-1] = caluCRC(&chSend[0],num-1);
    for(i=0;i<num;i++){
        baSend[i]=chSend[i];
    }
    QString str = QString::fromUtf8(baSend.toHex());
    qDebug()<<"str========="<<str;
    emit sendser(str);


}

void Q_Water::SendHeartbeat()
{
    if(heartcount>=9)
    {
        qCDebug(QWATER())<<QWATER().categoryName()
                             <<"Error heart beat count !"<< heartcount
                             << TRACE_CMH();

        HeartSendTimer->stop();
        disconnect(HeartSendTimer,SIGNAL(timeout()),this,SLOT(SendHeartbeat()));
        heartcount=0;
        emit ConnectFailed();
        return;
    }
    heartcount++;
    chSend[0]=COM_Water;
    chSend[1]=HEARTBEAT;
    chSend[2]=0x00;
    chSend[3]=0x00;
    sendSerial(5);
}

void Q_Water::ReSendWaterIn()
{
    if(WaterInFlag!=WaterInFlagREC){
        if(WaterInFlagREC == 10){
            WaterIn(false);
        }
        else if(WaterInFlagREC == 11){
            WaterIn(true);
        }
    }
}

void Q_Water::ReSendWaterOut()
{
    if(WaterOutFlag!=WaterOutFlagREC){
        if(WaterOutFlagREC == 20){
            WaterOut(false);
        }
        else if(WaterOutFlagREC == 21){
            WaterOut(true);
        }
    }
}

void Q_Water::ReSendWaterCycle()
{
    if(WaterCycleFlag!=WaterCycleFlagREC){
        if(WaterCycleFlagREC == 30){
            WaterCycle(false);
        }
        else if(WaterCycleFlagREC == 31){
            WaterCycle(true);
        }
    }
}

void Q_Water::ReSendWaterCooler2()
{
    if(Cooler2Flag!=Cooler2FlagREC){
        if(Cooler2FlagREC == 40){
            CloseCooler3();
        }
        else if(Cooler2FlagREC == 41){
            OpenCooler3();
        }
    }
}

void Q_Water::ReSendBedLock()
{
    if(!BedLockFlag){
        if(isBedLock){
            BedLock(true);
        }
        else{
            BedLock(false);
        }
    }
}

void Q_Water::WaterInControl(bool open)
{
    chSend[0]=COM_Water;
    chSend[1]=COM_SET_WATER;      //set pump
    chSend[2]=00;
    if(open){
        chSend[3]=0x11;
        WaterInFlagREC = 11;
    }
    else{
        chSend[3]=0x10;
        WaterInFlagREC = 10;
    }
    sendSerial(5);
    WaterInTimer->start();
}

void Q_Water::WaterOutControl(bool open)
{
    chSend[0]=COM_Water;
    chSend[1]=COM_SET_WATER;      //set pump
    chSend[2]=00;
    if(open){
        chSend[3]=0x21;
        WaterOutFlagREC = 21;
    }
    else{
        chSend[3]=0x20;
        WaterOutFlagREC = 20;
    }
    sendSerial(5);
    WaterOutTimer->start();
}

void Q_Water::WaterCycleControl(bool open)
{
    chSend[0]=COM_Water;
    chSend[1]=COM_SET_WATER;      //set pump
    chSend[2]=00;
    if(open){
        chSend[3]=0x31;
        WaterCycleFlagREC = 31;
    }
    else{
        chSend[3]=0x30;
        WaterCycleFlagREC = 30;
    }
    sendSerial(5);
    WaterCycleTimer->start();
}

void Q_Water::sendSerial_write(){      //发送信息,显示
//    int i;
//    QByteArray baSend;
//    m_serial->clear();
//    baSend.resize(num);
//    chSend[num-1] = caluCRC(&chSend[0],num-1);
//    for(i=0;i<num;i++){
//        baSend[i]=chSend[i];
//    }
        while(ReadArray.size()>100)
        {
            ReadArray.removeFirst();
        }
        int num = 0;
            if(WriteArray.size())
            {
                QByteArray baSend = WriteArray.first();
                num = baSend.size();
                WriteArray.removeFirst();
                qDebug()<<"Send message start:"<<baSend.toHex();
                m_serial->write(baSend);
                qDebug()<<"Send message finish:"<<baSend.toHex();
//                if(m_serial->waitForBytesWritten(50))
//                {
//                    qDebug()<<"Send False!!!!!!!!!!!!!!!!!!!!";
//                }
            }
            receiveSerial(num);
            //        qDebug()<<"Send False!!!!!!!!!!!!!!!!!!!!";
}

void Q_Water::webtest()
{

}

int Q_Water::receiveSerial(int num){     //在200ms内接收指定数量的信息,显示
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
    if (caluCRC(&chRead[0],num) == 0)
    {
        ReadArray.append(baRead);
        return num;
    }
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
    setpump1Flag=false;
    chSend[0]=COM_Water;
    chSend[1]=COM_SET_PUMP_1;      //set pump
    chSend[2]=m_Speed1>>8 & 0xff;
    chSend[3]=m_Speed1 & 0xff;
    sendSerial(5);
}

void Q_Water::setPump2()
{
    b_Disable_Slot=true;
    setpump2Flag=false;
    chSend[0]=COM_Water;
    chSend[1]=COM_SET_PUMP_2;      //set pump
    chSend[2]=m_Speed2>>8 & 0xff;
    chSend[3]=m_Speed2 & 0xff;
    sendSerial(5);
}

void Q_Water::setduPump1(int speed1)
{
    m_Speed1 = (speed1*10) & 0x7FFF;
    qDebug()<<"speed1 = "<<m_Speed1;
    setPump1();
}

void Q_Water::setduPump2(int speed2)
{
    m_Speed2 = (speed2*10) & 0x7FFF;
    qDebug()<<"speed2 = "<<m_Speed2;
    setPump2();
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
    chSend[0]=COM_Water;
    chSend[1]=COM_SET_RELAY;      //set relay
    chSend[2]=relay>>8 & 0xff;
    chSend[3]=relay & 0xff;
    sendSerial(5);
}

void Q_Water::CloseValveIN()
{
    b_Disable_Slot=true;
    relay &=(~RELAY_ValveIN);
    chSend[0]=COM_Water;
    chSend[1]=COM_SET_RELAY;      //set relay
    chSend[2]=relay>>8 & 0xff;
    chSend[3]=relay & 0xff;
    sendSerial(5);
}

void Q_Water::OpenValveOUT()
{
    b_Disable_Slot=true;
    relay |=RELAY_ValveOUT;
    chSend[0]=COM_Water;
    chSend[1]=COM_SET_RELAY;      //set relay
    chSend[2]=relay>>8 & 0xff;
    chSend[3]=relay & 0xff;
    sendSerial(5);
}

void Q_Water::CloseValveOUT()
{
    b_Disable_Slot=true;
    relay &=(~RELAY_ValveOUT);
    chSend[0]=COM_Water;
    chSend[1]=COM_SET_RELAY;      //set relay
    chSend[2]=relay>>8 & 0xff;
    chSend[3]=relay & 0xff;
    sendSerial(5);
}

void Q_Water::OpenWaterOut()
{
    b_Disable_Slot=true;
    relay |=RELAY_WaterOut;
    chSend[0]=COM_Water;
    chSend[1]=COM_SET_RELAY;      //set relay
    chSend[2]=relay>>8 & 0xff;
    chSend[3]=relay & 0xff;
    sendSerial(5);
}

void Q_Water::CloseWaterOut()
{
    b_Disable_Slot=true;
    relay &=(~RELAY_WaterOut);
    chSend[0]=COM_Water;
    chSend[1]=COM_SET_RELAY;      //set relay
    chSend[2]=relay>>8 & 0xff;
    chSend[3]=relay & 0xff;
    sendSerial(5);
}

void Q_Water::OpenValveBottle()
{
    b_Disable_Slot=true;
    relay |=RELAY_ValveBTT;
    chSend[0]=COM_Water;
    chSend[1]=COM_SET_RELAY;      //set relay
    chSend[2]=relay>>8 & 0xff;
    chSend[3]=relay & 0xff;
    sendSerial(5);
}

void Q_Water::CloseValveBottle()
{
    b_Disable_Slot=true;
    relay &=(~RELAY_ValveBTT);
    chSend[0]=COM_Water;
    chSend[1]=COM_SET_RELAY;      //set relay
    chSend[2]=relay>>8 & 0xff;
    chSend[3]=relay & 0xff;
    sendSerial(5);
}

void Q_Water::OpenZKPump()
{
    b_Disable_Slot=true;
    relay |=RELAY_ZKPump;
    chSend[0]=COM_Water;
    chSend[1]=COM_SET_RELAY;      //set relay
    chSend[2]=relay>>8 & 0xff;
    chSend[3]=relay & 0xff;
    sendSerial(5);
}

void Q_Water::CloseZKPump()
{
    b_Disable_Slot=true;
    relay &=(~RELAY_ZKPump);
    chSend[0]=COM_Water;
    chSend[1]=COM_SET_RELAY;      //set relay
    chSend[2]=relay>>8 & 0xff;
    chSend[3]=relay & 0xff;
    sendSerial(5);
}

void Q_Water::OpenPumpOUT()
{
    b_Disable_Slot=true;
    relay |=RELAY_PumpOUT;
    chSend[0]=COM_Water;
    chSend[1]=COM_SET_RELAY;      //set relay
    chSend[2]=relay>>8 & 0xff;
    chSend[3]=relay & 0xff;
    sendSerial(5);
}

// 发送排干水箱消息
void Q_Water::DrainTank()
{
    qDebug()<<"Water receive drain tank from dupump !";
    BedLock(true);  // 发送窗体锁定消息

    b_Disable_Slot=true;
    relay |=RELAY_DrainTank;
    chSend[0]=COM_Water;
    chSend[1]=COM_Drain;      //set relay
    chSend[2]=relay>>8 & 0xff;
    chSend[3]=relay & 0xff;
    sendSerial(5);
}

// 向下位机返回收到水箱排干消息指令
void Q_Water::DrainTankReturn()
{
    qDebug()<<"Send drain tank return to com !";
    b_Disable_Slot=true;
    relay |=RELAY_DrainTankReturn;
    chSend[0]=COM_Water;
    chSend[1]=COM_Drain;      //set relay
    chSend[2]=relay>>8 & 0xff;
    chSend[3]=relay & 0xff;
    sendSerial(5);
}

void Q_Water::ClosePumpOUT()
{
    b_Disable_Slot=true;
    relay &=(~RELAY_PumpOUT);
    chSend[0]=COM_Water;
    chSend[1]=COM_SET_RELAY;      //set relay
    chSend[2]=relay>>8 & 0xff;
    chSend[3]=relay & 0xff;
    sendSerial(5);
}

void Q_Water::OpenCooler1()
{
    b_Disable_Slot=true;
    relay |=RELAY_Cooler1;
    chSend[0]=COM_Water;
    chSend[1]=COM_SET_RELAY;      //set relay
    chSend[2]=relay>>8 & 0xff;
    chSend[3]=relay & 0xff;
    sendSerial(5);
}

void Q_Water::CloseCooler1()
{
    b_Disable_Slot=true;
    relay &=(~RELAY_Cooler1);
    chSend[0]=COM_Water;
    chSend[1]=COM_SET_RELAY;      //set relay
    chSend[2]=relay>>8 & 0xff;
    chSend[3]=relay & 0xff;
    sendSerial(5);
}

void Q_Water::OpenCooler2()
{
    b_Disable_Slot=true;
    relay |=RELAY_Cooler2;
    chSend[0]=COM_Water;
    chSend[1]=COM_SET_RELAY;      //set relay
    chSend[2]=relay>>8 & 0xff;
    chSend[3]=relay & 0xff;
    sendSerial(5);
}

void Q_Water::CloseCooler2()
{
    b_Disable_Slot=true;
    relay &=(~RELAY_Cooler2);
    chSend[0]=COM_Water;
    chSend[1]=COM_SET_RELAY;      //set relay
    chSend[2]=relay>>8 & 0xff;
    chSend[3]=relay & 0xff;
    sendSerial(5);
}

void Q_Water::OpenCooler3()
{
//    b_Disable_Slot=true;
//    relay |=RELAY_Cooler3;
//    chSend[0]=COM_Water;
//    chSend[1]=COM_SET_RELAY;      //set relay
//    chSend[2]=relay>>8 & 0xff;
//    chSend[3]=relay & 0xff;
//    sendSerial(5);
    chSend[0]=COM_Water;
    chSend[1]=COM_SET_WATER;      //set pump
    chSend[2]=00;
    chSend[3]=0x41;
    sendSerial(5);
    Cooler2FlagREC = 41;
    CoolerTimer->start();
}

void Q_Water::CloseCooler3()
{
//    b_Disable_Slot=true;
//    relay &=(~RELAY_Cooler3);
//    chSend[0]=COM_Water;
//    chSend[1]=COM_SET_RELAY;      //set relay
//    chSend[2]=relay>>8 & 0xff;
//    chSend[3]=relay & 0xff;
//    sendSerial(5);
    chSend[0]=COM_Water;
    chSend[1]=COM_SET_WATER;      //set pump
    chSend[2]=00;
    chSend[3]=0x40;
    sendSerial(5);
    Cooler2FlagREC = 40;
    CoolerTimer->start();
}

void Q_Water::Cooler(int number)
{
}

void Q_Water::getWater(bool enable)
{
    qDebug()<<"getwater1111111111111";
    if(enable)
    {
        qDebug()<<"before waterTimerOn";
        emit sendser("waterTimerOn");
        qDebug()<<"after waterTimerOff";
    }
    else
    {
        qDebug()<<"before waterTimerOff";
        emit sendser("waterTimerOff");
        qDebug()<<"after waterTimerOff";
    }
}

void Q_Water::getPressure()
{
    b_Disable_Slot=true;
    chSend[0]=COM_Water;
    chSend[1]=COM_GET_METER;
    chSend[2]=0x00;
    chSend[3]=0xFD;
    sendSerial(5);
}

void Q_Water::getTemp1()
{
    b_Disable_Slot=true;
    chSend[0]=COM_Water;
    chSend[1]=COM_GET_TEMP;
    chSend[2]=0x00;
    chSend[3]=0xFD;
    sendSerial(5);
}

void Q_Water::getTemp2()
{
    b_Disable_Slot=true;
    chSend[0]=COM_Water;
    chSend[1]=COM_GET_TEMP_2;
    chSend[2]=0x00;
    chSend[3]=0xFD;
    sendSerial(5);
}

void Q_Water::getLevel()
{
    b_Disable_Slot=true;
    chSend[0]=COM_Water;
    chSend[1]=COM_GET_LEVEL;
    chSend[2]=0x00;
    chSend[3]=0xFD;
    sendSerial(5);
}

void Q_Water::getVacuum()
{
    b_Disable_Slot=true;
    chSend[0]=COM_Water;
    chSend[1]=COM_GET_Vacuum;
    chSend[2]=0x00;
    chSend[3]=0xFD;
    sendSerial(5);
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
    WaterOut(false);
    disconnect(this,SIGNAL(signalAdjPumpOut()),this,SLOT(slotAdjPumpOut()));
}

void Q_Water::setMeter(int meter)
{
    m_PreMeter = meter;
    qCDebug(QWATER())<<QWATER().categoryName()
                         <<"QWater set meter !"<<m_PreMeter
                         << TRACE_CMH();
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

void Q_Water::COM_BACK(char id, char code)
{
    if(id==(char)0xF2)
    {
        chSend[0]=id;
        chSend[1]=code;
        chSend[2]=0x00;
        chSend[3]=0x00;
        chSend[4]=0xFD;
        sendSerial(6);
    }
    else
    {
        chSend[0]=id;
        chSend[1]=code;
        chSend[2]=0x00;
        chSend[3]=0xFD;
        sendSerial(5);
    }
}

void Q_Water::SerialCallback(QString data, void (*callback)())
{
    Delay_MSec(1000);
    int len = ReadArray.size();
    qDebug()<<"checkSerial ReadArray======"<<ReadArray;
    for(int i=0;i<len;i++)
    {
        qDebug()<<"checkSerial ReadArray Array======"<</*ReadArray[i].toHex()*/ReadArray[i];

        QByteArray res = ReadArray[i];

        qDebug()<<"checkSerial ReadArray data======" << res.toHex();

//        qDebug()<< res[0] << " toHex() " << (res.at(0) == 0xF1);

//        QString str = QString::fromUtf8(ReadArray[i].toHex());
//        qDebug()<<"receive data is "<<str;
//        if(str==data)
//        {
//            qDebug()<<"checkSerial success======";
//            checked_serial = true;
//        }
    }
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
    qDebug()<<"receive data";
//    if(b_Disable_Slot)return;
    int n=0;
    QByteArray baRead;

    n=m_serial->bytesAvailable();
    baRead=m_serial->readAll();
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
    else if(baRead.at(1)==(char)COM_ELE_KEY)
    {
        qDebug()<<"baRead=======COM_ELE_KEY";
        switch(baRead[2])
        {
            case ELE_COM_F:
                if(baRead.at(3)==(char)0x00)
                {
                    COM_BACK(COM_Water,COM_ELE_KEY);
                    qDebug()<<"ELE_COMF=======0";
                    emit Pos_control(2,0);
                    break;
                }
                else if(baRead.at(3)==(char)0x01)
                {
                    COM_BACK(COM_Water,COM_ELE_KEY);
                    qDebug()<<"ELE_COMF=======1";
                    emit Pos_control(2,-1);
                    break;
                }
            case ELE_COM_B:
                if(baRead.at(3)==(char)0x00)
                {
                    COM_BACK(COM_Water,COM_ELE_KEY);
                    qDebug()<<"ELE_COM_B=======0";
                    emit Pos_control(2,0);
                    break;
                }
                else if(baRead.at(3)==(char)0x01)
                {
                    COM_BACK(COM_Water,COM_ELE_KEY);
                    qDebug()<<"ELE_COM_B=======1";
                    emit Pos_control(2,1);
                    break;
                }
            case ELE_COM_L:
                if(baRead.at(3)==(char)0x00)
                {
                    COM_BACK(COM_Water,COM_ELE_KEY);
                    qDebug()<<"ELE_COM_L=======0";
                    emit Pos_control(1,0);
                    break;
                }
                else if(baRead.at(3)==(char)0x01)
                {
                    COM_BACK(COM_Water,COM_ELE_KEY);
                    qDebug()<<"ELE_COM_L=======1";
                    emit Pos_control(1,-1);
                    break;
                }
            case ELE_COM_R:
                if(baRead.at(3)==(char)0x00)
                {
                    COM_BACK(COM_Water,COM_ELE_KEY);
                    qDebug()<<"ELE_COM_R=======0";
                    emit Pos_control(1,0);
                    break;
                }
                else if(baRead.at(3)==(char)0x01)
                {
                    COM_BACK(COM_Water,COM_ELE_KEY);
                    qDebug()<<"ELE_COM_R=======1";
                    emit Pos_control(1,1);
                    break;
                }
            case ELE_COM_D:
                if(baRead.at(3)==(char)0x00)
                {
                    COM_BACK(COM_Water,COM_ELE_KEY);
                    qDebug()<<"ELE_COM_D=======0";
                    emit Pos_control(0,0);
                    break;
                }
                else if(baRead.at(3)==(char)0x01)
                {
                    COM_BACK(COM_Water,COM_ELE_KEY);
                    qDebug()<<"ELE_COM_D=======1";
                    emit Pos_control(0,-1);
                    break;
                }
            case ELE_COM_U:
                if(baRead.at(3)==(char)0x00)
                {
                    COM_BACK(COM_Water,COM_ELE_KEY);
                    qDebug()<<"ELE_COM_U=======0";
                    emit Pos_control(0,0);
                    break;
                }
                else if(baRead.at(3)==(char)0x01)
                {
                    COM_BACK(COM_Water,COM_ELE_KEY);
                    qDebug()<<"ELE_COM_U=======1";
                    emit Pos_control(0,1);
                    break;
                }

        }
    }
    else if(baRead.at(0)==(char)COM_SCREEN&&baRead.at(1)==(char)COM_BED_KEY)
    {
        switch(baRead[2])
        {
            case BED_HEADOUT:
                if(baRead.at(3)==(char)0x00)
                {
                    COM_BACK(COM_SCREEN,COM_BED_KEY);
                    emit Button_HeadOut(false);
                    break;
                }
                else if(baRead.at(3)==(char)0x01)
                {
                    COM_BACK(COM_SCREEN,COM_BED_KEY);
                    emit Button_HeadOut(true);
                    break;
                }
            case BED_WATEROUT:
                if(baRead.at(3)==(char)0x01)
                {
                    COM_BACK(COM_SCREEN,COM_BED_KEY);
                    emit Button_WaterOut(true);
                    break;
                }
                else if(baRead.at(3)==(char)0x00)
                {
                    COM_BACK(COM_SCREEN,COM_BED_KEY);
                    emit Button_WaterOut(false);
                    break;
                }
            case BED_WATERIN:
                if(baRead.at(3)==(char)0x01)
                {
                    COM_BACK(COM_SCREEN,COM_BED_KEY);
                    emit Button_WaterIn(true);
                    break;
                }
                else if(baRead.at(3)==(char)0x00)
                {
                    COM_BACK(COM_SCREEN,COM_BED_KEY);
                    emit Button_WaterIn(false);
                    break;
                }
            default:
                break;
        }
    }
    else if((n==9)&&baRead.at(0)==(char)COM_Bed)
    {
        if(baRead.at(1)==(char)COM_QUERY_ROD)
        {
            COM_BACK(COM_Bed,COM_QUERY_ROD);
            RODX_POS=(baRead[2]&0xff)*256+(baRead[3]&0xff)-13;
            RODY_POS=(baRead[4]&0xff)*256+(baRead[5]&0xff)-13;
            RODZ_POS=(baRead[6]&0xff)*256+(baRead[7]&0xff)-15;
//            emit GetBedPos(RODX_POS,RODY_POS,RODZ_POS);
        }
    }
}

void Q_Water::GetRodPos()
{
    b_Disable_Slot=true;
    chSend[0]=COM_Bed;
    chSend[1]=COM_QUERY_ROD;
    chSend[2]=0X01;
    chSend[3]=0X00;
    chSend[4]=0X01;
    sendSerial(6);
}

void Q_Water::SetRodPos(int id,int direction, int pos)
{
    int num;
    switch(id)
    {
        case 1:
            b_Disable_Slot=true;
            chSend[0]=COM_Bed;
            chSend[1]=COM_RODX_CONTROL;
            chSend[2]=direction;
            chSend[3]=pos>>8 & 0xff;
            chSend[4]=pos & 0xff;
            sendSerial(6);
            break;
        case 2:
            b_Disable_Slot=true;
            chSend[0]=COM_Bed;
            chSend[1]=COM_RODY_CONTROL;
            chSend[2]=direction;
            chSend[3]=pos>>8 & 0xff;
            chSend[4]=pos & 0xff;
            sendSerial(6);
            break;
        case 3:
            b_Disable_Slot=true;
            chSend[0]=COM_Bed;
            chSend[1]=COM_RODZ_CONTROL;
            chSend[2]=direction;
            chSend[3]=pos>>8 & 0xff;
            chSend[4]=pos & 0xff;
            sendSerial(6);
            break;
    }
}

void Q_Water::BedLock(bool flag)
{
    if(flag)
    {
        qDebug()<<"Water send bed lock true !";
        b_Disable_Slot=true;
        chSend[0]=COM_Water;
        chSend[1]=COM_BED_LOCK;
        chSend[2]=0X00;
        chSend[3]=0X01;
        sendSerial(5);
        isBedLock = true;
        BedLockFlag = false;
        BedLockTimer->start();
    }
    else
    {
        qDebug()<<"Water send bed lock false !";

        b_Disable_Slot=true;
        chSend[0]=COM_Water;
        chSend[1]=COM_BED_LOCK;
        chSend[2]=0X00;
        chSend[3]=0X00;
        sendSerial(5);
        isBedLock = false;
        BedLockFlag = false;
        BedLockTimer->start();
    }
}

void Q_Water::SendScreenWaterIn()
{
    b_Disable_Slot=true;
    chSend[0]=COM_SCREEN;
    chSend[1]=SCREEN_WATER;
    chSend[2]=0x00;
    chSend[3]=0x01;
    sendSerial(5);
}

void Q_Water::SendScreenManual()
{
    b_Disable_Slot=true;
    chSend[0]=COM_SCREEN;
    chSend[1]=SCREEN_WATER;
    chSend[2]=0x00;
    chSend[3]=0x03;
    sendSerial(5);
}

void Q_Water::SendScreenAuto()
{
    b_Disable_Slot=true;
    chSend[0]=COM_SCREEN;
    chSend[1]=SCREEN_WATER;
    chSend[2]=0x00;
    chSend[3]=0x04;
    sendSerial(5);
}

void Q_Water::SendScreenWaterOut()
{
    b_Disable_Slot=true;
    chSend[0]=COM_SCREEN;
    chSend[1]=SCREEN_WATER;
    chSend[2]=0x00;
    chSend[3]=0x02;
    sendSerial(5);
}

void Q_Water::SendScreenNULL()
{
    b_Disable_Slot=true;
    chSend[0]=COM_SCREEN;
    chSend[1]=SCREEN_WATER;
    chSend[2]=0x00;
    chSend[3]=0x00;
    sendSerial(5);
}

void Q_Water::ReceiveData(QString message)
{
    QVector<QString> intArray;
    // 使用 Qt 的 splitRef 函数拆分字符串，然后将每个子字符串转换为整数并添加到 QVector
    for (const QStringRef &item : message.splitRef(',')) {
        intArray.append(QString::number(item.toInt(), 16));
    }
    qDebug()<<"intArray====="<<intArray;
    if(intArray.at(0) == "f1"&&intArray.size()>=5)
    {
        heartcount = 0;
        int water_data_h = (intArray.at(2).toInt(nullptr,16)&0xff)*256;
        int water_data_l = intArray.at(3).toInt(nullptr,16)&0xff;
        int water_data = water_data_h + water_data_l;
//        qDebug()<<"water_data======="<<water_data;
//        qDebug()<<"water_data_h======"<<water_data_h<<",water_data_l======"<<water_data_l;
        if(intArray.at(1) == "ca")
        {
            if(intArray.at(3)== "10")
            {
                WaterInFlag = 10;
                Cooler2Flag = 40;
            }
            else if(intArray.at(3)== "11")
            {
                WaterInFlag = 11;
            }
            else if(intArray.at(3)== "20")
            {
                WaterOutFlag = 20;
            }
            else if(intArray.at(3)== "21")
            {
                WaterOutFlag = 21;
            }
            else if(intArray.at(3)== "30")
            {
                WaterCycleFlag = 30;
                WaterInFlag = 10;
                WaterOutFlag = 20;
                Cooler2Flag = 40;
                emit WaterCycleState(false);
            }
            else if(intArray.at(3)== "31")
            {
                WaterCycleFlag = 31;
                WaterInFlag = 11;
                WaterOutFlag = 21;
                emit WaterCycleState(true);
            }
            else if(intArray.at(3)== "40")
            {
                Cooler2Flag = 40;
            }
            else if(intArray.at(3)== "41")
            {
                Cooler2Flag = 41;
            }
        }
        else if(intArray.at(1) == "c1")
        {
            emit MeterUpdate(water_data);
        }
        else if(intArray.at(1) == "c6")
        {
            setpump1Flag = true;
        }
        else if(intArray.at(1) == "c7")
        {
            setpump2Flag = true;
        }
        else if(intArray.at(1) == "e4")
        {
            heartcount = 0;
        }
        else if(intArray.at(1) == "e2")
        {
            BedLockFlag = true;
        }
        else if(intArray.at(1) == "c2")
        {
            if(water_data<Level_Adj)
                water_data=Level_Adj;
            int level = (water_data-Level_Adj)/3;
            emit LevelUpdate(level);
        }
        else if(intArray.at(1) == "c3")
        {
            emit VacuumUpdate(water_data);
        }
        else if(intArray.at(1) == "c4")
        {
            emit TempUpdate(water_data);
        }
        else if(intArray.at(1) == "c5")
        {
            emit Temp2Update(water_data);
        }
        else if(intArray.at(1) == "e1")
        {
            checked_serial = true;
        }
        else if (intArray.at(1) == "d0")
        {
            qDebug()<<"Receive drain tank finished from com !"<<intArray.at(3);
            if (intArray.at(3)=="3")
            {
                // 接收到排水完成消息，告诉上位机关闭软件，向下位机发送返回收到指令
                qDebug()<<"Receive drain tank finished from com !";
                DrainTankReturn();
                BedLock(false);  // 向下位机发送床解锁消息
                emit DrainTankFinished();
            }
        }
        else if(intArray.at(1) == "d2")
        {
            if(intArray.at(2)== "1")
            {
                if(intArray.at(3)== "0")
                {
                    COM_BACK(COM_Water,COM_ELE_KEY);
                    qDebug()<<"ELE_COMF=======0";
                    emit Pos_control(2,0);
                }
                else if(intArray.at(3)== "1")
                {
                    COM_BACK(COM_Water,COM_ELE_KEY);
                    qDebug()<<"ELE_COMF=======1";
                    emit Pos_control(2,-1);
                }
            }
            else if(intArray.at(2)== "2")
            {
                if(intArray.at(3)== "0")
                {
                    COM_BACK(COM_Water,COM_ELE_KEY);
                    qDebug()<<"ELE_COM_B=======0";
                    emit Pos_control(2,0);
                }
                else if(intArray.at(3)== "1")
                {
                    COM_BACK(COM_Water,COM_ELE_KEY);
                    qDebug()<<"ELE_COM_B=======1";
                    emit Pos_control(2,1);
                }
            }
            else if(intArray.at(2)== "3")
            {
                if(intArray.at(3)== "0")
                {
                    COM_BACK(COM_Water,COM_ELE_KEY);
                    qDebug()<<"ELE_COM_L=======0";
                    emit Pos_control(1,0);
                }
                else if(intArray.at(3)== "1")
                {
                    COM_BACK(COM_Water,COM_ELE_KEY);
                    qDebug()<<"ELE_COM_L=======1";
                    emit Pos_control(1,-1);
                }
            }
            else if(intArray.at(2)== "4")
            {
                if(intArray.at(3)== "0")
                {
                    COM_BACK(COM_Water,COM_ELE_KEY);
                    qDebug()<<"ELE_COM_R=======0";
                    emit Pos_control(1,0);
                }
                else if(intArray.at(3)== "1")
                {
                    COM_BACK(COM_Water,COM_ELE_KEY);
                    qDebug()<<"ELE_COM_R=======1";
                    emit Pos_control(1,1);
                }
            }
            else if(intArray.at(2)== "5")
            {
                if(intArray.at(3)== "0")
                {
                    COM_BACK(COM_Water,COM_ELE_KEY);
                    qDebug()<<"ELE_COM_D=======0";
                    emit Pos_control(0,0);
                }
                else if(intArray.at(3)== "1")
                {
                    COM_BACK(COM_Water,COM_ELE_KEY);
                    qDebug()<<"ELE_COM_D=======1";
                    emit Pos_control(0,-1);
                }
            }
            else if(intArray.at(2)== "6")
            {
                if(intArray.at(3)== "0")
                {
                    COM_BACK(COM_Water,COM_ELE_KEY);
                    qDebug()<<"ELE_COM_U=======0";
                    emit Pos_control(0,0);
                }
                else if(intArray.at(3)== "1")
                {
                    COM_BACK(COM_Water,COM_ELE_KEY);
                    qDebug()<<"ELE_COM_U=======1";
                    emit Pos_control(0,1);
                }
            }
        }
        else if(intArray.at(1) == "c5")
        {
            emit Temp2Update(water_data);
        }
        else
        {
            qDebug()<<"receive error message data";
        }
    }
    else if(intArray.at(0) == "f2"&&intArray.size()>=5)
    {
        if(intArray.at(1) == "b3"&&intArray.at(4) == "2")
        {
            checked_bedzero = true;
        }
        if(intArray.at(1) == "b2")
        {
            if(intArray.size()==9)
            {
                COM_BACK(COM_Bed,COM_QUERY_ROD);
                int rodx_pos = (intArray.at(2).toInt(nullptr,16)&0xff)*256+(intArray.at(3).toInt(nullptr,16)&0xff)-13;
                int rody_pos = (intArray.at(4).toInt(nullptr,16)&0xff)*256+(intArray.at(5).toInt(nullptr,16)&0xff)-13;
                int rodz_pos = (intArray.at(6).toInt(nullptr,16)&0xff)*256+(intArray.at(7).toInt(nullptr,16)&0xff)-15;
                emit GetBedPos(rodx_pos,rody_pos,rodz_pos);
            }
        }
    }
    else if(intArray.at(0) == "f3"&&intArray.size()>=5)
    {
        if(intArray.at(1) == "d4")
        {
            if(intArray.at(2) == "1")
            {
                if(intArray.at(3) == "0")
                {
                    COM_BACK(COM_SCREEN,COM_BED_KEY);
                    emit Button_HeadOut(false);
                }
                else if(intArray.at(3) == "1")
                {
                    COM_BACK(COM_SCREEN,COM_BED_KEY);
                    emit Button_HeadOut(true);
                }
            }
            else if(intArray.at(2) == "2")
            {
                if(intArray.at(3) == "0")
                {
                    COM_BACK(COM_SCREEN,COM_BED_KEY);
                    emit Button_WaterOut(false);
                }
                else if(intArray.at(3) == "1")
                {
                    COM_BACK(COM_SCREEN,COM_BED_KEY);
                    emit Button_WaterOut(true);
                }
            }
            else if(intArray.at(2) == "3")
            {
                if(intArray.at(3) == "0")
                {
                    COM_BACK(COM_SCREEN,COM_BED_KEY);
                    emit Button_WaterIn(false);
                }
                else if(intArray.at(3) == "1")
                {
                    COM_BACK(COM_SCREEN,COM_BED_KEY);
                    emit Button_WaterIn(true);
                }
            }
        }
    }
}

void Q_Water::SendLevelRef()
{
    chSend[0]=COM_Water;
    chSend[1]=COM_Level_Ref;
    chSend[2]=Level_Adj>>8 & 0xff;
    chSend[3]=Level_Adj & 0xff;
    sendSerial(6);
}

void Q_Water::WaterIn(bool open)
{
    WaterInControl(open);
}

void Q_Water::WaterOut(bool open)
{
    WaterOutControl(open);
}

void Q_Water::WaterCycle(bool open)
{
    WaterCycleControl(open);
}

void Q_Water::WaterCooler2(bool open)
{
    if(open){
        OpenCooler3();
    }
    else{
        CloseCooler3();
    }
}

QSerialPort* Q_Water::getserial()
{
    return m_serial;
}
