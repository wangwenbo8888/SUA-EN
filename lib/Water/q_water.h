#ifndef Q_WATER_H
#define Q_WATER_H

#include <QObject>
#include <QtSerialPort/QSerialPort>
#include <QTimer>
#include <QSerialPortInfo>
#include "q_water_global.h"
#include <QTime>
#include <QCoreApplication>
#include <QLoggingCategory>

#define FLOAT_DELTA         0.0001      //用于float -> int转换,去误差

#define PUMP_METER      0X0B
#define PUMP_TEMP       0X15
#define PUMP_Vacuum     0X0D
#define PUMP_Level      0X0C
#define Bed_Enable      0x81

//通讯协议
#define COM_Water     0xF1
// 排干水箱通信协议
#define COM_Drain     0xD0

#define COM_EN        0xE1
#define COM_Bed       0xF2
#define COM_BedZero   0xB3
#define COM_SCREEN    0xF3
#define SCREEN_WATER  0xD1
#define HEARTBEAT     0xE4

#define COM_ROD_KEY     0xC3
    #define ROD_COM1        0x01
    #define ROD_COM2        0x02
    #define ROD_COM3        0x03
    #define ROD_COM4        0x04
    #define ROD_COM5        0x05
    #define ROD_COM6        0x06
#define COM_ELE_KEY     0xD2
    #define ELE_COM_F        0x01
    #define ELE_COM_B        0x02
    #define ELE_COM_L        0x03
    #define ELE_COM_R        0x04
    #define ELE_COM_D        0x05
    #define ELE_COM_U        0x06
#define COM_BED_KEY     0xD4
    #define BED_HEADOUT     0x01
    #define BED_WATEROUT    0x02
    #define BED_WATERIN     0x03
#define COM_RODX_CONTROL    0xA2
#define COM_RODY_CONTROL    0xA3
#define COM_RODZ_CONTROL    0xA4
#define COM_RODX_POS        0xB5       //260'
#define COM_RODY_POS        0xB6
#define COM_RODZ_POS        0xB7
#define COM_QUERY_ROD       0xB2
#define COM_BED_ZERO        0xED
#define COM_BED_SET_ZERO    0xB1

#define COM_BED_LOCK        0xE2

#define COM_GET_NAME        0xFE
#define COM_RESTART         0xFD
#define COM_SEARCH          0xFC
#define COM_GET_WATER       0xFB
#define COM_AUTOMODE		0x62

#define COM_ENABLE          0xF9
#define COM_GET_VER         0xF8
#define COM_GET_ID          0xFA
    #define ID_DUPUMP           0xA7
    #define ID_LAMP				0xA6
    #define ID_PUMP				0xA5
#define COM_DEBUG           0xF6		//兼容旧版,仅用于报警灯盒时进入调试状态
#define COM_DONE            0xFD
#define COM_ECHO            0xF4
#define COM_ERROR           0xF3

#define COM_SET_PAR         0xE0
#define COM_SET_METER       0xE1
#define COM_SET_PUMP_1      0xC6
#define COM_SET_PUMP_2      0xC7
#define COM_SET_WATER       0xCA

//#define COM_GET_PAR			0xE8
#define COM_GET_METER		0xC1
#define COM_GET_LEVEL		0xC2
#define COM_GET_Vacuum      0xC3
//#define COM_GET_PUMP_1		0xEA
//#define COM_GET_PUMP_2		0xEB
#define COM_GET_TEMP        0xC4
#define COM_GET_TEMP_2      0xC5

//自动发送命令
#define COM_AUTO_METER		0xD0
#define COM_AUTO_PUMP_2		0xD2
#define COM_AUTO_WATER		0xD3
#define COM_AUTO_FSW_OFF	0xD4
#define COM_AUTO_FSW_ON		0xD5

#define COM_GET_FSW_RL		0xD6
#define COM_Level_Ref		0xD9
#define COM_SET_RELAY		0xC8
    #define RELAY_PumpOUT   0x0001
    #define RELAY_ZKPump    0x0002
    #define RELAY_ValveIN   0x0004
    #define RELAY_ValveOUT  0x0008
    #define RELAY_WaterOut  0x0010
    #define RELAY_ValveBTT  0x0020
    #define RELAY_Cooler1   0x0040
    #define RELAY_Cooler2   0x0080
    #define RELAY_Cooler3   0x0100
    // 排干水箱相关
    #define RELAY_DrainTank 0x0101
    // 返回收到水箱排干消息指令
    #define RELAY_DrainTankReturn 0x00FD

    #define RELAY_9         0x0200
    #define RELAY_10        0x0400
    #define RELAY_11        0x0800
#define COM_RELAY0_OFF		0xD8		//D8/D9,DA/DB,DC/DD =relay0~2 off/on,兼容旧版,推荐使用D7: COM_SET_RELAY
//调试命令
#define COM_DBG_TEST		0xC0		//(调试状态下)试验
#define COM_DBG_485       	0xC8		//(调试状态下)转发消息
#define COM_DBG_485E      	0xC9		//(调试状态下)转发消息
#define COM_DBG_REC_BUF     0xCF		//(调试状态下)把接收缓冲区的数据发回主机

//参数命令
#define PAR_ALL				0x00    	//对应下面1~8项的BIT，读有效
#define PAR_MODE_AUTO		0x01
#define PAR_METER_AUTO		0x02
#define PAR_WATER_AUTO		0x03
#define PAR_FSW_AUTO		0x04
#define PAR_SAMPLE_OFF		0x07		//(调试状态下)
#define PAR_DEBUG			0x08
    #define PAR_DISABLE			0
    #define PAR_ENABLE			1

#define PAR_STAUS			0x09
#define PAR_ERROR			0x0A
#define PAR_TIME_METER		0x10
#define PAR_TIME_PUMP_2		0x20
#define PAR_TIME_TEMP		0x30
#define PAR_TIME_WATER		0x40

//通讯Timeout时间(unit: ms)
#define WAITING_PERIOD  50
//压力表数据的显示数量
#define METER_QUANTITY  500

#define SETTINGS_PATH   "../../lib/config/config.ini"

typedef unsigned char        uint8_t;
typedef unsigned short       uint16_t;

Q_DECLARE_LOGGING_CATEGORY(QWATER)

class Q_WATERSHARED_EXPORT Q_Water : public QObject
{
    Q_OBJECT
public:
    Q_Water(QObject* parent = 0);
    ~Q_Water();
    inline bool exist() {return m_serial != NULL ? true : false;}
    QByteArray baSend, baRead;
    void setPump1();
    void setPump2();
    bool init();
    QSerialPort* getserial();

private:
    QSerialPort* m_serial;
    QSerialPort* m_serialPort_Status;
    QString m_portName;
    int heartcount = 0;
    QTimer *HeartSendTimer;
    QTimer *WaterInTimer, *WaterOutTimer, *WaterCycleTimer, *CoolerTimer, *BedLockTimer;

    uint16_t CRC16(uint8_t *Data, uint16_t len);

    int receiveSerial_Status(int num);

    bool Port_Status;

    bool openSerial();
    void setSerial();
    bool checkSerial();
    bool enableSerial();
    int receiveSerial(int num);
    void Delay_MSec(unsigned int msec);
    void COM_BACK(char id,char code);
    void SerialCallback(QString, void (*callback)());

    unsigned char chSend[4],chRead[32];
    int m_SpdOut_Adj,m_SpdOut_Set,SpdOut;
    int m_Speed1,m_Speed2,relay;
    bool isBedLock,BedLockFlag;
    int m_PreMeter,m_RecMeter,m_Vacuum,m_Temp,m_Temp2,m_Level;
    int RODX_POS,RODY_POS,RODZ_POS;
    float Meter,Vacuum,Level;
    bool b_Disable_Slot,waterAutoOpen;
    int AutoSpeedIN,AutoSpeedOUT;
    int Level_Adj;
    QList<QByteArray> ReadArray;
    QList<QByteArray> WriteArray;
    bool checked_serial = false;
    bool checked_bedzero = false;
    bool setpump1Flag = false;
    bool setpump2Flag = false;
    int WaterInFlag = 0;
    int WaterOutFlag = 0;
    int WaterCycleFlag = 0;
    int Cooler2Flag = 0;
    int WaterInFlagREC = 0;
    int WaterOutFlagREC = 0;
    int WaterCycleFlagREC = 0;
    int Cooler2FlagREC = 0;


    unsigned char caluCRC(unsigned char* ptr, unsigned char len);

signals:
    void TempUpdate(int);
    void Temp2Update(int);
    void MeterUpdate(int);
    void VacuumUpdate(int);
    void LevelUpdate(int);
    void signalAdjPumpOut();
    void getPump1(int);
    void getPump2(int);
    void Pos_control(int,int);
    void SpeedOutUpdate(int);
    void GetBedPos(int,int,int);
    void BedZeroStatus(bool);
    void Button_HeadOut(bool);
    void Button_WaterOut(bool);
    void Button_WaterIn(bool);
    void sendser(QString);
    void ConnectFailed();
    void WaterCycleState(bool);

    // 告诉双泵水处理对象水箱已经排空，可以关闭软件
    void DrainTankFinished();

private slots:
    void meterAutoMode();
    void slotAdjPumpOut();
    void ReadData();
    void sendSerial_write();
    void webtest();
    void sendSerial(int num);
    void SendHeartbeat();
    void ReSendWaterIn();
    void ReSendWaterOut();
    void ReSendWaterCycle();
    void ReSendWaterCooler2();
    void ReSendBedLock();
    void WaterInControl(bool);
    void WaterOutControl(bool);
    void WaterCycleControl(bool);
    void OpenCooler3();
    void CloseCooler3();

public slots:
    void openAutoMode();
    void closeAutoMode();
    void setduPump1(int speed1);
    void setduPump2(int speed2);
    void closeduPump1();
    void closeduPump2();
    void getWater(bool enable);
    void getPressure();
    void getTemp1();
    void getTemp2();
    void getLevel();
    void getVacuum();
    void OpenValveIN();
    void CloseValveIN();
    void OpenValveOUT();
    void CloseValveOUT();
    void OpenValveBottle();
    void CloseValveBottle();
    void OpenZKPump();
    void CloseZKPump();
    void OpenPumpOUT();

    // 发送排干水箱消息
    void DrainTank();

    // 向下位机发送返回收到水箱排干消息指令
    void DrainTankReturn();

    void ClosePumpOUT();
    void Cooler(int number);
    void OpenCooler1();
    void CloseCooler1();
    void OpenCooler2();
    void CloseCooler2();
//    void OpenLight();
//    void CloseLight();
    void OpenWaterOut();
    void CloseWaterOut();
    void UpdateAuto(bool);
    void setMeter(int);
    void GetRodPos();
    void SetRodPos(int,int,int);
    void BedLock(bool);
    bool checkBedZero();
    void SetBedZero();
    void SendScreenWaterOut();
    void SendScreenWaterIn();
    void SendScreenManual();
    void SendScreenAuto();
    void SendScreenNULL();
    void ReceiveData(QString);
    void SendLevelRef();
    void WaterIn(bool);
    void WaterOut(bool);
    void WaterCycle(bool);
    void WaterCooler2(bool);
};

#endif // Q_WATER_H
