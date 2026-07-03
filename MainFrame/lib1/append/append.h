#ifndef APPEND_H
#define APPEND_H

#include <QObject>
#include "append_global.h"
#include <QDebug>
#include <QThread>
#include "serialworker.h"
#include <QLoggingCategory>

//#include "Inc/bdaqctrl.h"
#include "constant.h"
#include "variable.h"

enum Light_Color
{
  RED = 0,
  GREEN,
  YELLOW
};

enum Light_Status
{
  LIGHT_ON = 0,
  LIGHT_OFF,
};

enum Switch_Status
{
  SWITCH_ON = 0,
  SWITCH_OFF,
  SWITCH_ERROR
};

class APPENDSHARED_EXPORT Append : public QObject
{
    Q_OBJECT

public:
    Append(QObject *parent=0);
    ~Append();

    bool init();

    inline bool exist() {return m_bExist;}
    inline bool sopen() {return true;}

    inline bool setRedOn(){return m_setLight(RED,LIGHT_ON); }
    inline bool setRedOff() {return m_setLight(RED,LIGHT_OFF); }
    inline bool setGreenOn() {return m_setLight(GREEN,LIGHT_ON); }
    inline bool setGreenOff() {return m_setLight(GREEN,LIGHT_OFF); }
    inline bool setYellowOn() {return m_setLight(YELLOW,LIGHT_ON); }
    inline bool setYellowOff() {return m_setLight(YELLOW,LIGHT_OFF); }
    inline bool isSwitchOn() { return m_bSwitch ; }
    inline bool isRedOn() {return m_bRed; }
    inline bool isYellowOn() { return m_bYellow; }
    inline bool isGreenOn() { return m_bGreen; }

    void setLightsOff();

    bool SetLight(Light_Color color, Light_Status status);

signals:
    void error();
    void switchChanged(bool flag);

    //发送给线程的信号
    void serialWorkerInit(QString portNmae);
    void serialWorkerSend(QByteArray data);
    void serialWorkerClose();

private:
    QString m_portName;

    void readSettings();

    bool m_setLight(Light_Color color,Light_Status status);


    //  脚踏开关状态
    bool m_bSwitch;
    bool m_bRed;
    bool m_bYellow;
    bool m_bGreen;

    bool m_bInitComplete;
    bool m_bExist;//是否成功打开串口

    SerialWorker *_serialWorker;
    QThread *_workerThread;

public slots:
    void close();
private slots:
    void readInfo(QByteArray baRead);
    void readExist(bool exist,bool bswitch);
    void serialerror();

    //接受线程的槽
};

#endif // APPEND_H
