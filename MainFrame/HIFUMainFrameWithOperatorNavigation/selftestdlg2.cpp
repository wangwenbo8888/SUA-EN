#include "selftestdlg.h"
#include "ui_selftestdlg.h"
#include <QMovie>


SelfTestDlg::SelfTestDlg(NewMainWindow* pMain,QWidget *parent) :
    m_pMainWin(pMain),
    QDialog(parent),
    ui(new Ui::SelfTestDlg)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);

    selfTestStatus =new bool[5];
    bOK=true;
    // ui->pushButton_continue->setEnabled(false);

    _isShowFromLogin = false;

    for(int i=0;i<5;i++)
    {
        selfTestStatus[i]=false;
    }

    pixOK.load(":/umimg/UMImage/success_48px.png");
    pixNO.load(":/umimg/UMImage/error_48px.png");

    timer=new QTimer(this);

    lblList.append(ui->label_append);
    lblList.append(ui->label_location);
    lblList.append(ui->label_warter);
    lblList.append(ui->label_network);
    lblList.append(ui->label_imageb);

    movieLoad = new QMovie(":/umimg/UMImage/loading_48px.gif");
    foreach (QLabel *lbl, lblList) {
        lbl->setMovie(movieLoad);
    }
    movieLoad->start();

    sindex=0;
    connect(timer,SIGNAL(timeout()),this,SLOT(timeoutslot()));

    //Circulation部分
    initShelfCheckTimer();
    m_iCheckCircle = 0;

    connect(this,SIGNAL(setPumpSpeed(int)),m_pMainWin,SLOT(setPumpSpeed(int)));
    connect(this,SIGNAL(openPump()),m_pMainWin,SLOT(openPump()));
    connect(this,SIGNAL(closePump()),m_pMainWin,SLOT(closePump()));
    connect(this,SIGNAL(queryStatus()),m_pMainWin,SLOT(queryStatus()));

    m_bPumpOpen = false;    //设置泵关闭
    ui->textEdit_shelf_check_status->setText("close pump !");
    closePump();

    tempGauge1=new QcwThermometer(this);
    tempGauge1->setGeometry(160,550,150,400);
    tempGauge1->setMinValue(-10);
    tempGauge1->setMaxValue(50);
    tempGauge1->setScaleMajor(6);
    tempGauge1->setScaleMinor(10);

    tempGauge2=new QcwThermometer(this);
    tempGauge2->setGeometry(380,550,150,400);
    tempGauge2->setMinValue(-10);
    tempGauge2->setMaxValue(50);
    tempGauge2->setScaleMajor(6);
    tempGauge2->setScaleMinor(10);

    levelQueryTimer =new QTimer(this);
    levelQueryTimer->setInterval(30000);
    connect(levelQueryTimer,SIGNAL(timeout()),this,SLOT(on_toolButton_query_clicked()));
    levelQueryTimer->stop();
    on_toolButton_query_clicked();
}

SelfTestDlg::~SelfTestDlg()
{
    delete ui;
}

void SelfTestDlg::showCheck()
{
    _isShowFromLogin = true;
    this->show();
    timer->start(1000);
}

void SelfTestDlg::showFromMainWindow()
{
    _isShowFromLogin = false;
    this->show();
}

void SelfTestDlg::updateStatus(bool bappend, bool blocation, bool bwarter, bool bnetwork, bool bimagegraber)
{
    selfTestStatus[iappned]=bappend;
    selfTestStatus[ilocation]=blocation;
    selfTestStatus[iwarter]=bwarter;
    selfTestStatus[inetwork]=bnetwork;
    selfTestStatus[iimagegraber]=bimagegraber;
}

void SelfTestDlg::timeoutslot()
{
    if(sindex>4)
    {
        timer->stop();
        return;
    }

    if(selfTestStatus[sindex])
    {
        //lblList[sindex]-
        lblList[sindex]->setPixmap(pixOK);

    }
    else
    {
        bOK=false;
        lblList[sindex]->setPixmap(pixNO);
    }
    ui->progressBar->setValue((sindex+1)*20);
    sindex++;

    timer->start(500);
}

//void SelfTestDlg::on_pushButton_continue_clicked()
//{
//    emit showUserManger();
//    this->close();
//}

void SelfTestDlg::on_toolButton_pump_control_clicked()
{
    if (!m_bPumpOpen)
    {
        qDebug()<<"open pump ************1";
        // 泵关闭
        ui->textEdit_shelf_check_status->setText("open pump !");
        qDebug()<<"open pump ************2";
        openPump();
        ui->textEdit_shelf_check_status->append("Set pump speed 40 !");
        qDebug()<<"open pump ************3";
        ui->horizontalScrollBar_pump_speed->setValue(40);
        qDebug()<<"open pump ************4";
        emit setPumpSpeed(40);
        qDebug()<<"open pump ************5";
        ui->toolButton_pump_control->setText("Stop Pump");
        qDebug()<<"open pump ************6";
        //ui->toolButton_pump_control->setStyleSheet(REDCOLOR);
        m_bPumpOpen = true;
        qDebug()<<"open pump ************7";
        levelQueryTimer->start();
        on_toolButton_query_clicked();
    }
    else
    {
        // 泵开着
        ui->textEdit_shelf_check_status->setText("close pump !");
        closePump();
        ui->toolButton_pump_control->setText("Start Pump");
        //ui->toolButton_pump_control->setStyleSheet(SELECTED);
        m_bPumpOpen = false;
        levelQueryTimer->stop();
    }
}

void SelfTestDlg::on_toolButton_query_clicked()
{
    qDebug()<<"Query温度Level信息";
    emit queryStatus();
}

void SelfTestDlg::on_horizontalScrollBar_pump_speed_valueChanged(int value)
{
    ui->spinBox_pump_speed->setValue(value);
    QString str("Set pump speed ");
    str.append(QString::number(value));
    emit setPumpSpeed(value);
    ui->textEdit_shelf_check_status->setText(str);
}

void SelfTestDlg::on_spinBox_pump_speed_valueChanged(int value)
{
    ui->horizontalScrollBar_pump_speed->setValue(value);
    QString str("Set pump speed ");
    str.append(QString::number(value));
    // this->setWindowFlags(Qt::WindowStaysOnTopHint);
    ui->textEdit_shelf_check_status->setText(str);
}

void SelfTestDlg::on_horizontalScrollBar_pump_speed_actionTriggered(int action)
{
    ui->spinBox_pump_speed->setValue(ui->horizontalScrollBar_pump_speed->value());
}

void SelfTestDlg::initShelfCheckTimer()
{
   _shelfCheckTimer = new QTimer(this);
   _shelfCheckTimer->setInterval(10000);
   _shelfCheckTimer->setSingleShot(false);
   connect(_shelfCheckTimer, SIGNAL(timeout()), this, SLOT(on_shelf_check_timer_timeout()));
}

void SelfTestDlg::on_shelf_check_timer_timeout()
{
    ++m_iCheckCircle;
    QString info("Self-Test Cycle");
    info.append(" ");
    info.append(QString::number(m_iCheckCircle));
    ui->textEdit_shelf_check_status->append(outputCheckInfo(info));
    queryStatus();
    // statusUpdated();

    info = "Temperature 1";
    info.append(" ");
    info.append(QString::number(m_iTemperature1));
    ui->textEdit_shelf_check_status->append(outputCheckInfo(info));

    info = "Temperature 2";
    info.append(" ");
    info.append(QString::number(m_iTemperature2));
    ui->textEdit_shelf_check_status->append(outputCheckInfo(info));

    info = "Level";
    info.append(" ");
    info.append(QString::number(m_iLevel));
    ui->textEdit_shelf_check_status->append(outputCheckInfo(info));
}

void SelfTestDlg::statusUpdated(uint temp1, uint temp2, uint level)
{
    qDebug()<<"Query信息更新";
    m_iTemperature1 = temp1;
    tempGauge1->setValue(temp1);
    m_iTemperature2 = temp2;
    tempGauge2->setValue(temp2);
    m_iLevel = level;

    ui->label_thermometer_left_value->setText(QString::number(m_iTemperature1));
    ui->label_thermometer_right_value->setText(QString::number(m_iTemperature2));
    QString levelStatus;
    if(m_iLevel==0x70){
        levelStatus="Water Level Low";
    }
    else if(m_iLevel==0x71){
        levelStatus="Water Level Normal";
    }
    else if(m_iLevel==0x72){
        levelStatus="Water Level High";
    }
    ui->label_level_value->setText(levelStatus);

    if(m_iLevel==0x70&&m_bPumpOpen){
        if(ui->spinBox_pump_speed->value()>30){
            ui->spinBox_pump_speed->setValue(30);
            QString str("Water level is low.\nSet pump speed ");
            str.append(QString::number(30));
            emit setPumpSpeed(30);
        }
    }
}

void SelfTestDlg::showCurrent(uint temp1,uint temp2,uint level)
{
    m_iTemperature1 = temp1;
    tempGauge1->setValue(temp1);
    m_iTemperature2 = temp2;
    tempGauge2->setValue(temp2);
    m_iLevel = level;

    ui->label_thermometer_left_value->setText(QString::number(m_iTemperature1));
    ui->label_thermometer_right_value->setText(QString::number(m_iTemperature2));
    ui->label_level_value->setText(QString::number(m_iLevel));
}

QString SelfTestDlg::outputCheckInfo(const QString& info)
{
    //QDate date = QDate::currentDate();
    QString str = QTime::currentTime().toString("HH:mm:ss");
    return QString(info+" "+str);
//    int day = date.day();
//    int month = date.month();
}

void SelfTestDlg::showResultMsg(QVariant a,QVariant b)
{
    //emit closeLoginWindow();
    this->showFullScreen();

    // initShelfCheckTimer();
    // _shelfCheckTimer->start();
    QString info("Self-test started.");
    ui->textEdit_shelf_check_status->setText(outputCheckInfo(info));
}

void SelfTestDlg::on_progressBar_valueChanged(int value)
{
    if (value==100&&_isShowFromLogin)
    {
        if(bOK)
        {
            // emit showUserManger();
            emit showLogin();
            _isShowFromLogin = false;
            DelayClose();
        }
    }
}

void SelfTestDlg::on_pushButton_quit_clicked()
{
    if(m_pMainWin!=NULL){
        if(m_pMainWin->isHidden()){
            m_pMainWin->QuitBeforShow();
        }
    }
    this->close();
}

void SelfTestDlg::DelayClose()
{
    delayTimer.setSingleShot(true);
    delayTimer.setInterval(500);
    delayTimer.start();
    connect(&delayTimer,SIGNAL(timeout()),this,SLOT(close()));
}
