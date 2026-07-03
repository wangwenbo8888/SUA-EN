#ifndef SelfTestDlg_H
#define SelfTestDlg_H

#include <QDialog>
#include <QTimer>
#include <QList>
#include <QLabel>
#include "newmainwindow.h"
#include <QDebug>
#include "qcw_thermometer.h"

namespace Ui {
class SelfTestDlg;
}

enum selfSubSys
{
    iappned = 0,    // Control Box
    ilocation,   // Motion System
    iwarter,   // Circulation
    inetwork,      // 网络
    iimagegraber   //Ultrasound Image
};

class SelfTestDlg : public QDialog
{
    Q_OBJECT

public:
    explicit SelfTestDlg(NewMainWindow* pMain,QWidget *parent = 0);
    ~SelfTestDlg();

    void showCurrent(uint temp1,uint temp2,uint level);

signals:

    void showUserManger();

    void showLogin();

    void queryStatus();

    void setPumpSpeed(int);

    void openPump();

    void closePump();

public slots:

    void updateStatus(bool bappend,bool blocation,bool bwarter,bool bnetwork,bool bimagegraber);

    void timeoutslot();

    void showCheck();

    void showFromMainWindow();

private slots:

    void statusUpdated(uint temp1, uint temp2, uint level);

//    void on_pushButton_continue_clicked();

    void on_toolButton_pump_control_clicked();

    void on_toolButton_query_clicked();

    void on_horizontalScrollBar_pump_speed_valueChanged(int value);

    void on_spinBox_pump_speed_valueChanged(int arg1);

    void on_horizontalScrollBar_pump_speed_actionTriggered(int action);

    QString outputCheckInfo(const QString& info);

    void on_shelf_check_timer_timeout();
    void showResultMsg(QVariant a ,QVariant b);

    void on_progressBar_valueChanged(int value);

    void on_pushButton_quit_clicked();

private:

    void initShelfCheckTimer();
    Ui::SelfTestDlg *ui;
    bool *selfTestStatus;
    QTimer *timer;
    int sindex;
    QList<QLabel *> lblList;
    QPixmap pixOK;
    QPixmap pixNO;
    QMovie *movieLoad;
    bool bOK;
    bool _isShowFromLogin;

    NewMainWindow* m_pMainWin;

    QTimer* _shelfCheckTimer;
    bool m_bPumpOpen;
    uint m_iTemperature1;
    uint m_iTemperature2;
    uint m_iLevel;
    uint m_iCheckCircle;

    QTimer* levelQueryTimer;
    QcwThermometer *tempGauge1;
    QcwThermometer *tempGauge2;
    void DelayClose();
    QTimer delayTimer;
};

#endif // SelfTestDlg_H
