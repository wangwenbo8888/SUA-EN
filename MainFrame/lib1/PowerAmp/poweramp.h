#ifndef POWERAMP_H
#define POWERAMP_H

#include <QObject>
#include <QtSerialPort/QtSerialPort>
#include <QTime>
#include <QLoggingCategory>

#include "constant.h"
#include "variable.h"
#include "poweramp_global.h"

Q_DECLARE_LOGGING_CATEGORY(PA)

class POWERAMPSHARED_EXPORT PowerAmp : public QObject {

    Q_OBJECT

public:
    PowerAmp(QObject* parent=0);
    ~PowerAmp();

    enum Action {
        START,
        RESET,
        FETCH_VOLTAGE,
        FETCH_TEMPERATURE
    };

    void init();
    inline bool exist();
    inline bool serial();

    //  Default: channel=-1 indicates that starting all the channels.
    //  i.e., start(voltage)
    bool start(double voltage, int channel=-1);
    //  Default: channel=-1 indicates that reseting all the channels.
    //  i.e., reset();
    bool reset(int channel=-1);
    //  Default: action=FETCH_VOLTAGE indicates that fetching the
    //  voltage value of the channel.
    //  i.e., fetch(value, channel), fetch(value, channel, FETCH_TEMPERATURE)
    //  Actions are limited to FETCH_VOLTAGE and FETCH_TEMPERATURE.
    bool fetch(double &value, int channel, Action action=FETCH_VOLTAGE);

public slots:
    void checkchannels();
    void closeSerial();

signals:
    void statusUpdated(PackageFlag, DataFlag);

private:
    QSerialPort *m_serial;
    QByteArray baSend, baRead;
    QString m_Port;
    int m_Baud;
    bool b_Debug;
//    int panum;

    void loadSettings();
    void updateSettings();

    bool openSerial();
    void setSerial(int baud);
    bool connectSerial();
    void scanSerial();
    //发送数据到串口
    bool sendSerial();
    //从串口接收数据
    bool receiveSerial();

    //生成发送数据,通道有效性判断,电压有效性判断
    void genSendDat(int channel, Action action, int argument);
    //  数据链路: 发送5字节,接收5字节(全体命令时无接收),电压单位: 0.1v,温度: 采样值
    //      channel: 0(或-1,兼容需要),对应所有通道
    //      action: 4种命令
    //      argument: 在START命令时设置电压参数;其他默认为0(无用)
    //      返回: 错误=-1;FETCH命令=电压值/温度采样值,其他=0
    int dataLink(int channel, Action action, int argument=0);
    //检查所有通道,通讯失败率超过指定值,检查失败
    bool checkAllChannels();

    //  温度数据转换: 温度采样值 -> 实际温度(0~100°);
    int transTemp(int value);

    //  TODO: The 3 functions below should be treated as the assistant functions of the class.
    inline int randomNumber() {
        QTime time = QTime::currentTime();
        qsrand(time.msec() + time.second() * MILLISECOND_UNIT);
        return qrand();
    }
};

//  Move the definition of the inline function out of the class declaration
//inline bool PowerAmp::exist() { return m_serial == NULL ? false:true; }
inline bool PowerAmp::exist() { return connectSerial(); }
//inline bool PowerAmp::serial() { return connectSerial(); }

#endif // POWERAMP_H
