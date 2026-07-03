#include <QSettings>
//#include <QtSerialPort/QSerialPortInfo>

#ifdef DEBUG
#include <QDebug>
#endif

#include <Windows.h>
#include <synchapi.h>
#include <math.h>
#include "poweramp.h"

Q_LOGGING_CATEGORY(PA, "PowerAmp")
int panum;

PowerAmp::PowerAmp(QObject *parent)
    : QObject(parent)
{
    init();
}

PowerAmp::~PowerAmp() {
    closeSerial();
}

void PowerAmp::loadSettings(){
    QSettings* settings = new QSettings(SETTINGS_PATH, QSettings::IniFormat);
    m_Port = settings->value("PowerAmp/port").toString();
    m_Baud = settings->value("PowerAmp/baud").toInt();
    b_Debug = settings->value("PowerAmp/debug").toBool();
    delete settings;
}

void PowerAmp::updateSettings(){
    QSettings* settings = new QSettings(SETTINGS_PATH, QSettings::IniFormat);
    settings->setValue("PowerAmp/port", m_Port);
    settings->setValue("PowerAmp/baud", m_Baud);
    delete settings;
}

void PowerAmp::init(){
    panum=10;
    qCDebug(PA()) << "\n";
    qCDebug(PA()) << "Initialization ...";
    loadSettings();
    m_serial = new QSerialPort(m_Port);
    if (openSerial()){
        setSerial(m_Baud);
//        if (connectSerial()){
//            if (checkAllChannels()){
                qCDebug(PA()) << "Initialized successfully. default serial port/baud is " << m_Port << "/" << m_Baud << ".\n";
                return;
                // In the consistent way to emit signal.
//                emit statusUpdated(INQUIRY, POWER_AMPLIFIERS_AVAILABLE);
//                return;
//            }
//        } else {
//            qCDebug(PA()) << "The serial port/baud was changed.";
//            closeSerial();
//        }
    }
    delete m_serial, m_serial = NULL;

    scanSerial();
}

void PowerAmp::scanSerial(){
    qCDebug(PA()) << "Scan serials ...";
    QList<QSerialPortInfo> infoList = QSerialPortInfo::availablePorts();
    m_serial = new QSerialPort;
    foreach (const QSerialPortInfo &info, infoList){
        m_serial->setPort(info);
        m_Port = m_serial->portName();
        if (!openSerial()) continue;
        for (int i=0;i<2;i++){
            if (i==0) m_Baud=38400; else m_Baud=9600;
            setSerial(m_Baud);
            if (connectSerial()) {
//                if (checkAllChannels()){
                    updateSettings();
                    qCDebug(PA()) << "Initialized successfully. new serial port/baud is " << m_Port << "/" << m_Baud << ", updating!\n";
//                    emit statusUpdated(INQUIRY, POWER_AMPLIFIERS_AVAILABLE);
                    return;
//                } else{
//                    break;
//                }
            }
        }
        m_serial->close();
    }
    delete m_serial, m_serial = NULL;
    qCDebug(PA()) << "Failed to be initialized.\n";
    qCWarning(PA()) << "Failed to be initialized.";
    // In the consistent way to emit signal
    emit statusUpdated(INQUIRY, POWER_AMPLIFIERS_NONAVAILABLE);
}

bool PowerAmp::openSerial(){
    if (m_serial->isOpen()){
        qCDebug(PA()) << "The serial" << m_Port << " already opened.";
        return true;
    }
    if (m_serial->open(QIODevice::ReadWrite)){
        qCDebug(PA()) << "Opened the serial" << m_Port << " successfully.";
        return true;
    } else{
        qCDebug(PA()) << "Failed to open the serial" << m_Port << ".";
        return false;
    }
}

// Close the serial and destruct it.
void PowerAmp::closeSerial(){
    if (m_serial->isOpen()){
        m_serial->close();
        delete m_serial, m_serial = NULL;
        qCDebug(PA()) << "Closed the serial" << m_Port << " successfully.";
    }
}

void PowerAmp::setSerial(int baud){
    if (baud == 38400) m_serial->setBaudRate(QSerialPort::Baud38400);
    else m_serial->setBaudRate(QSerialPort::Baud9600);
    qCDebug(PA()) << "Set baudRate =" << m_serial->baudRate() << ".";

    m_serial->setParity(QSerialPort::NoParity);
    m_serial->setDataBits(QSerialPort::Data8);
    m_serial->setStopBits(QSerialPort::OneStop);
    m_serial->setFlowControl(QSerialPort::NoFlowControl);
    m_serial->setReadBufferSize(0);

    m_serial->clearError();
    m_serial->clear();
}

bool PowerAmp::connectSerial(){
    qCDebug(PA()) << "Connect ...";
    for (int i=0; i<TEST_CHANNEL_COUNT; i++){
        int channel = randomNumber() % AMP_COUNT + 1;
        if (dataLink(channel,RESET) != -1){
            qCDebug(PA()) << "Successful!";
            return true;
        }
    }
    qCDebug(PA()) << "Failure.";
    return false;
}

bool PowerAmp::checkAllChannels(){
    QList<int> errorChannels;
    dataLink(0,RESET);
    Sleep(WAITING_PERIOD);
    // add by wangwenbo 20260401
    //Sleep(2000);

    // wangwenbo 20260401 test fetch valtage error begin
    for (int i=1; i<=AMP_COUNT; i++){
        if (dataLink(i,FETCH_VOLTAGE) == -1) errorChannels.append(i);
    }
    // wangwenbo 20260401 test fetch valtage error end

//    for (int i=1; i<=AMP_COUNT; i++)
//    {
//        //int re = dataLink(i,FETCH_VOLTAGE);
//        double value;
//        bool re = fetch(value,i,FETCH_VOLTAGE);
//        qCWarning(PA())  << "The : #" << i
//                         << "voltage "<< value;
//        if ( re == false || fabs(value)<10.0 )
//            errorChannels.append(i);
//    }

    if (errorChannels.size() != 0){
        qCWarning(PA()) << errorChannels.size() << "channels have error."
                        << "They are: #" << errorChannels;
    }

    if (errorChannels.size() < FAIL_THRESHOLD){
        qCDebug(PA()) << "Checked the channels successfully.";
        emit statusUpdated(SONICATION, POWER_AMPLIFIERS_READY);
        return true;
    } else{
        qCWarning(PA()) << "Failed to start the channels.";
        emit statusUpdated(SONICATION, POWER_AMPLIFIERS_NOT_READY);
        return false;
    }
}

bool PowerAmp::sendSerial(){      //发送信息,显示
    baSend.resize(BYTE_RETURNED_COUNT);

    m_serial->clear();
    m_serial->write(baSend);
    m_serial->waitForBytesWritten(WAITING_PERIOD);
    if (b_Debug) qCDebug(PA()) << "Send data=" << baSend.toHex().toUpper() << ".";
    return true;
}

bool PowerAmp::receiveSerial(){     //在TimeOut时间内接收指定数量的信息
    if(m_serial->waitForReadyRead(WAITING_PERIOD)){
        baRead = m_serial->readAll();

        //若接收数据数量不足,则继续等待
        while(baRead.size()<BYTE_RETURNED_COUNT){
            if(m_serial->waitForReadyRead(WAITING_PERIOD)){
                baRead += m_serial->readAll();
            } else{
                break;
            }
        }
    } else{
        qCWarning(PA()) << "Read timeOut error. Send data="<< baSend.toHex().toUpper()<< ".";
        return false;
    }

    if (b_Debug) qCDebug(PA()) << "Receive data=" << baRead.toHex().toUpper() << ".";
    return true;
}

void PowerAmp::genSendDat(int channel, Action action, int argument){
    //参数合法性判断
    if (channel==-1) channel=0;
    else if (channel>AMP_COUNT) channel=AMP_COUNT;
    else if (channel<0) channel=1;
    if (argument>VOLTAGE_MAXIMUM*10) argument=VOLTAGE_MAXIMUM*10;
    else if (argument<0) argument=0;

    baSend[0] = ((channel >> 7) & 0x01) | 0x80; baSend[1] = channel & 0x7F;

    switch (action){
    case RESET:
        baSend[2] = 0x00; baSend[3] = 0x00;
        break;
    case START:
        baSend[2] = ((argument >> 7) & 0x01) | 0x40; baSend[3] = argument & 0x7F;
        break;
    case FETCH_VOLTAGE:
        baSend[2] = 0x20; baSend[3] = 0x00;
        break;
    case FETCH_TEMPERATURE:
        baSend[2] = 0x10; baSend[3] = 0x00;
        break;
    default:
        break;
    }

    baSend[4] = (baSend[0] + baSend[1] + baSend[2] + baSend[3]) & 0x7F;
}

int PowerAmp::dataLink(int channel,Action action,int argument){
    genSendDat(channel,action,argument);        //生成发送参数
    if (!sendSerial()) return -1;

    if (channel==0 || channel==-1) return 0;    //全体命令,直接返回
    if (!receiveSerial()) return -1;

    //接收数据检查: 数量,格式,CRC,通道号
    if ((baRead.size() != BYTE_RETURNED_COUNT) ||                           //接收数据数量不符
       (((baRead[0]|baRead[1]|baRead[2]|baRead[3]|baRead[4])&0x80) != 0) || //接收数据格式不符(bit7=0)
       (((baRead[0]+baRead[1]+baRead[2]+baRead[3]-baRead[4])&0x7F) != 0) || //CRC错误
       (((baRead[0]&0xFF)!=(baSend[0]&0x7F)) || (baRead[1]!=baSend[1]))){   //通道号不符
        qCWarning(PA()) << "Read data error. Send data="<< baSend.toHex().toUpper()<< ",receive data="<< baRead.toHex().toUpper()<< ".";
        return -1;
    }

    if (action==START || action==RESET) return 0;
    return ((baRead[2] & 0x7f)<<7 | (baRead[3] & 0x7f));
}

//温度-电压采样数据表，温度范围：-30°C~+149°C，电阻型号：NTC-10K-3950
static int tempTab[]={
     54, 57, 61, 64, 68, 72, 76, 80, 85, 89, 94,100,105,111,116,122,129,135,142,149,
    156,163,170,178,186,194,203,212,221,230, 239,249,259,269,279,290,300,311,322,333,
    344,355,366,378,389,400,412,423,434,446, 457,468,479,490,501,512,525,537,549,561,
    572,583,593,604,614,624,633,643,652,661, 670,679,687,696,704,713,721,729,737,745,
    752,760,767,775,782,789,795,802,808,814, 821,826,832,838,843,848,853,858,863,867,
    871,876,880,884,888,891,895,899,902,906, 909,913,916,919,923,926,929,932,934,937,
    939,942,944,946,948,950,952,954,956,958, 959,961,963,964,966,967,969,970,972,973,
    974,976,977,978,979,980,981,982,984,985, 986,987,987,988,989,990,991,992,993,993,
    994,995,996,996,997,998,998,999,1000,1000, 1001,1001,1002,1002,1003,1003,1004,1004,1005,1005
};
int PowerAmp::transTemp(int value){
    int i;
    for (i=0;i<=100;i++){
        if (value<tempTab[i+31]) return i;
    }
    return i;
}

bool PowerAmp::fetch(double &value, int channel, Action action){
    if (!exist()) return false;
    value = -1;
    int returned = dataLink(channel,action);

    if (channel==-1 || channel==0) {
        qCDebug(PA()) << "Ready fetch all channels.";
        return true;
    }

    if (returned != -1) {
        if (action == FETCH_VOLTAGE) value = (double)returned / 10;
        else value = (double)transTemp(returned);
        qCDebug(PA()) << "The value of #" << channel << " is " << value << ".";
        return true;
    }else {
        qCWarning(PA()) << "Cannot fetch the value of #" << channel << ".";
        return false;
    }
}

bool PowerAmp::start(double voltage, int channel){
    if (!exist()) return false;
//    if(panum>=10)
//    {
//        if (!checkAllChannels())
//        {
//            panum++;
//            return false;
//        }
//        else
//            panum=1;
//    }
//    else
//        panum++;
    int argument = voltage * 10;
    int returned = dataLink(channel,START,argument);
    if (channel==-1 || channel==0) {
        qCDebug(PA()) << "Start all channels with voltage= " << voltage << "v.";
        return true;
    }
    if (returned != -1) {
        qCDebug(PA()) << "Started #" << channel << " with voltage= " << voltage << "v,successfully.";
        return true;
    }else {
        qCWarning(PA()) << "Failed to start #" << channel << ".";
        return false;
    }
}

bool PowerAmp::reset(int channel) {
    if (!exist()) return false;
    int returned = dataLink(channel,RESET);
    if (channel==-1 || channel==0) {
        qCDebug(PA()) << "Reset all channels.";
        return true;
    }
    if (returned != -1) {
        qCDebug(PA()) << "Reset #" << channel << "successfully.";
        return true;
    }else {
        qCWarning(PA()) << "Failed to reset #" << channel << ".";
        return false;
    }
}

void PowerAmp::checkchannels()
{
    if(checkAllChannels())
    {
        qDebug()<<"poweramp channels ready";
    }
    else
    {
        qDebug()<<"poweramp channels notready";
    }
}
