#ifndef SelfTestDlg_H
#define SelfTestDlg_H

#include <QDialog>
#include <QTimer>
#include <QList>
#include <QLabel>
#include "newmainwindow.h"
#include <QDebug>
#include "qcw_thermometer.h"
#include "commondefine.h"
#include "variable.h"
#if _MSC_VER >1600
#pragma execution_character_set("utf-8")
#endif

class QStandardItemModel;
class TemperTable;

namespace Ui {
class SelfTestDlg;
}

Q_DECLARE_LOGGING_CATEGORY(SELFCHECK)

class SelfTestDlg : public QDialog
{
    Q_OBJECT

public:
    explicit SelfTestDlg(NewMainWindow* pMain,QWidget *parent = 0);
    ~SelfTestDlg();

    void showCurrent(uint temp1,uint temp2,uint level);
    void SetMainWindow(NewMainWindow* main);

protected:
    void showEvent(QShowEvent *event);

signals:

    void showLogin();

    void queryStatus();

    void setPumpSpeed(int);

    void openPump();

    void closePump();

    void queryAmpTempature();

    void queryAmpVoltage();

public slots:

    // Refresh表格中的功放温度和电压参数
//    void refreshAmpParaInTable(DataFlag,QByteArray& );

    void updateStatus(bool bappend,bool blocation,bool bwarter,bool bnetwork,bool bimagegraber);

    void updateStatus(selfSubSys sys,bool status);

    void timeoutslot();

    void showCheck();

    void showFromMainWindow();

    void networkreflash();void on_textEdit_shelf_check_status_textChanged();


private slots:

//    void statusUpdated(uint temp1, uint temp2, uint level);

//    void on_toolButton_pump_control_clicked();

//    void on_toolButton_query_clicked();

//    void on_spinBox_pump_speed_valueChanged(int arg1);

    QString outputCheckInfo(const QString& info);

//    void on_shelf_check_timer_timeout();
    void showResultMsg(QVariant a ,QVariant b);

    void on_progressBar_valueChanged(int value);

    void on_pushButton_quit_clicked();

    void on_pushButton_refresh_clicked();

//    void on_toolButton_refresh_tempature_clicked();

//    void on_toolButton_refresh_voltage_clicked();

//    void on_dial_pump_speed_valueChanged(int value);

private:

    void setUserModel();
    void setUserModel(int model);

    // 初始化功放参数表格
    void initAmpParaTable();

    void initShelfCheckTimer();
    Ui::SelfTestDlg *ui;
    bool *selfTestStatus;
    QVector<bool> SubSysStatus;
    QTimer *timer;
    int sindex;
    int iSelfTestProgress;
    QList<QLabel *> lblList;
    QPixmap pixOK;
    QPixmap pixNO;
    QPixmap pixUnknown;
    //QMovie *movieLoad;
    bool bOK;
    bool _isShowFromLogin;
    bool selfcheckfailed=false;
    bool ischecknetwork=false;

    int m_networkservice;

//    TemperTable* m_table;

    // 用于Savepower amp温度和电压
//    QStandardItemModel* m_pAmpModel;

    NewMainWindow* m_pMainWin;

    QTimer* _shelfCheckTimer;
    bool m_bPumpOpen;
    uint m_iTemperature1;
    uint m_iTemperature2;
    uint m_iLevel;
    uint m_iCheckCircle;

//    QTimer* levelQueryTimer;
//    QcwThermometer *tempGauge1;
//    QcwThermometer *tempGauge2;
    void DelayClose();
    QTimer delayTimer;

    bool isdoublepump;

    void keyPressEvent(QKeyEvent *event);
};

#endif // SelfTestDlg_H
