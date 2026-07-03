#ifndef DUPUMPCONTROL_H
#define DUPUMPCONTROL_H

#include <QDialog>
#include <QTimer>
#include <QList>
#include <QLabel>
#include <QWebSocket>
#include "newmainwindow.h"
#include <QDebug>
#include "qcw_thermometer.h"
#include "commondefine.h"
#include "variable.h"
#include "q_water.h"
#include "qthread.h"
#if _MSC_VER >1600
#pragma execution_character_set("utf-8")
#endif

#define FLOAT_DELTA         0.0001

namespace Ui {
class dupumpcontrol;
}

class dupumpcontrol : public QDialog
{
    Q_OBJECT

public:
    explicit dupumpcontrol(QWidget *parent = 0);
    ~dupumpcontrol();

    void SetMainWindow(NewMainWindow* main);

    Q_Water *q_water;
    QWebSocket* SerialClient;

protected:
void paintEvent(QPaintEvent *);

private:
    Ui::dupumpcontrol *ui;

    bool m_bPump1Open;
    bool m_bPump2Open;
    bool meterAutoOpen;
    bool autoquerywater;
    bool queryPressure;
    bool btnpumpin;
    bool btnpumpout;
    bool btnwaterout;
    bool btncooler;
    bool btnvalvebottle;
    bool lowlevel=false;
    bool OverLevel=false;
    bool HighLevel=false;
    bool Cooler2Flag=false;

    bool meterSet = false;

    QTimer* levelQueryTimer;
    QTimer* BottleOutTimer;
    NewMainWindow* m_pMainWin;
    int m_PreMeter,m_RecMeter,m_Meter[METER_QUANTITY];
    int m_Spd2_Adj,m_Spd2_Set;
    int pump1speed=0,pump2speed=0;
    int warningPressureCount = 0;

    QcwThermometer *tempGauge1;
    QcwThermometer *tempGauge2;

    QThread *Water_Thread;

    void keyPressEvent(QKeyEvent *event);
    void Delay_MSec(unsigned int msec);


signals:
    void OpenPumpIN();
    void OpenPumpOUT();
    void ClosePumpIN();
    void ClosePumpOUT();
    void OpenValveIN();
    void CloseValveIN();
    void OpenValveOUT();
    void CloseValveOUT();
    void OpenCooler1();
    void CloseCooler1();
    void OpenCooler();
    void CloseCooler();
    void ControlCooler2(bool);
    void OpenValveBottle();
    void CloseValveBottle();
    void OpenWaterOut();
    void CloseWaterOut();
    void OpenXHPump();
    void OpenZKPump();
    void CloseXHPump();
    void CloseZKPump();
    void queryStatus();
    void setPump1Speed(int);
    void setPump2Speed(int);
    void openPump1();
    void openPump2();
    void closePump1();
    void closePump2();
    void setWaterIn(bool);
    void setWaterOut(bool);
    void setWaterCycle(bool);
    void queryAmpTempature();
    void queryAmpVoltage();
    void setMeter(int);
    void queryMeter(bool);
    void queryLevel(bool);
    void queryTemp(bool);
    void queryVacuum(bool);
    void getPump1();
    void getPump2();
    void getwater();
    void sendpar(uchar,uchar);
    void signalAdjPump2();
    void pump1open();
    void pump1close();
    void pump1lock();
    void pump2open();
    void pump2close();
    void pump2lock();
    void updatespeed1(int);
    void updatespeed2(int);
    void updatemeter(float);
    void closeautomode();
    void controllock();
    void controlunlock();
    void AutoModeON();
    void AutoModeOFF();
    void Temp1Update(int);
    void UpdateLevel(int);
    void ReceiveHeadControl(int AID,int direction);
    void ReceiveRodPos(int posx,int posy,int posz);
    void SetRodPos(int id,int direction,int pos);
    void GetRodPos();
    void SetBedZero();
    void BedZeroStatus(bool);
//    void GetWater(bool);
    void ScreenWaterIn();
    void ScreenWaterOut();
    void ScreenManual();
    void ScreenAuto();
    void ScreenNULL();
    void SetBedLock(bool);
    void WaterReceiveData(QString);
    void messageReceived(const QString &message);
    void ConnectFailed(DataFlag);

    // 开始Drain Water Tank操作
    void DrainTankStart();

    // 向窗口对象发送Water tank drain complete消息
    void DrainTankFinishedToWindow();

private slots:

    void on_toolButton_pump1_control_clicked();

    void on_toolButton_pump2_control_clicked();

    void on_toolButton_querypump1_clicked();

    //void on_toolButton_querypump2_clicked();

    void on_spinBox_pump1_speed_valueChanged(int arg1);

    void on_spinBox_pump2_speed_valueChanged(int arg1);

    void on_dial_pump1_speed_valueChanged(int value);

    void on_dial_pump2_speed_valueChanged(int value);

    void on_pushButton_quit_clicked();

    void on_toolButton_setmeter_clicked();

    //void on_toolButton_querymeter_clicked();

    //void on_spinBox_pump2_speed_valueChanged(const QString &arg1);

    //void on_spinBox_pump1_speed_valueChanged(const QString &arg1);

//    void statusUpdated(uint temp1,uint temp2,uint level);

    void on_toolButton_automode_clicked();

    void slotAdjPump2();

    void close_automode();

    //void on_toolButton_querymeter_clicked(bool checked);

    void on_toolButton_pumpin_clicked();

    void on_toolButton_pumpout_clicked();

    void on_toolButton_waterout_clicked();

    void on_toolButton_cooler_clicked();

    void on_toolButton_bottle_clicked();

    void ReceiveHead(int AID,int direction);
    void ReceiveRod(int,int,int);
    void HeadOut(bool);
    void WaterIn(bool);
    void WaterOut(bool);
    void WaterFailed();
    void WaterCycleState(bool);

    void on_toolButton_queryMeter_clicked();

    void on_toolButton_querywater_clicked();

    void on_dial_pump1_speed_sliderReleased();

    void on_dial_pump2_speed_sliderReleased();

public slots:
    void TempUpdate(int);
    void Temp2Update(int);
    void LevelUpdate(int);
    void SpeedOutUpdate(int);
    bool init();
//    void retryConnection();
    void showFromMainWindow();
//    void updateWaterStatus(int,int,int);
    void updatePump1Status(int);
    void updatePump2Status(int);
    void updateMeterDat(int);

    void controlpump1();
    void controlpump2();
    void speed1up();
    void speed1down();
    void speed2up();
    void speed2down();
    void RodControl(int id,int direction,int pos);
    void Get_RodPos();
    void Set_BedZero();
    void Bed_ZeroStatus(bool);
    void Send_Message(QString);
    void TreatState(bool);

    // 收到主窗口Drain Water Tank的消息
    void on_Drain_Tank_from_Window();

    // 收到Q_WaterWater tank drain complete消息
    void DrainTankFinished();

};

#endif // DUPUMPCONTROL_H
