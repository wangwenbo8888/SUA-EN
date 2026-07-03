#include "windows.h"
#include <QDebug>
#include <QTime>
#include <QPainter>

#include <synchapi.h>
#include <QMessageBox>

#include "dupumpcontrol.h"
#include "ui_dupumpcontrol.h"
#if _MSC_VER >1600
#pragma execution_character_set("utf-8")
#endif

dupumpcontrol::dupumpcontrol(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dupumpcontrol)
{
    SerialClient = new QWebSocket();
    qDebug()<<"SerialClient=========="<<SerialClient;
    SerialClient->open(QUrl("ws://127.0.0.1:9706"));
    connect(SerialClient, &QWebSocket::textMessageReceived,[this](const QString &message){
        qDebug() << "WebSocketClient::messageReceived : \n" << message << endl;
        emit WaterReceiveData(message);
    });

    connect(SerialClient, static_cast<void (QWebSocket::*)(QAbstractSocket::SocketError)>(&QWebSocket::error), [this](){
//        qDebug() << "WebSocketClient::retryConnection : \n" << endl;
        QTimer::singleShot(5000, [=](){
            qDebug() << "Retrying WebSocket connection..." << endl;
            SerialClient->open(QUrl("ws://127.0.0.1:9706"));
        });
    });

    Water_Thread = new QThread;
    q_water = new Q_Water;
    m_bPump1Open = false;
    m_bPump2Open = false;
    meterAutoOpen = false;
    autoquerywater = false;
    queryPressure = true;
    btnpumpin=false;
    btnpumpout=false;
    btnwaterout=false;
    btncooler=false;
    btnvalvebottle=false;
    m_PreMeter = 200;
    m_Spd2_Adj = 0;
    for (int i=0;i<METER_QUANTITY;i++) m_Meter[i]=0;
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_QuitOnClose,false);
    tempGauge1=new QcwThermometer(this);
    tempGauge1->setBackground(QColor::fromRgb(197,197,197));
    tempGauge1->setLiquidColor(QColor::fromRgb(30,144,255));
    tempGauge1->setBorderColor(Qt::gray);
    //tempGauge1->setGeometry(160,570,145,360);
    tempGauge1->setMinValue(-10);
    tempGauge1->setMaxValue(50);
    tempGauge1->setScaleMajor(6);
    tempGauge1->setScaleMinor(10);
    ui->verticalLayout_tempGaugeLeft->addWidget(tempGauge1);

    tempGauge2=new QcwThermometer(this);
    tempGauge2->setBackground(QColor::fromRgb(197,197,197));
    tempGauge2->setLiquidColor(QColor::fromRgb(30,144,255));
    tempGauge2->setBorderColor(Qt::gray);
    //tempGauge2->setGeometry(380,570,145,360);
    tempGauge2->setMinValue(-10);
    tempGauge2->setMaxValue(50);
    tempGauge2->setScaleMajor(6);
    tempGauge2->setScaleMinor(10);
    ui->verticalLayout__tempGaugeRight->addWidget(tempGauge2);

    ui->groupBox_4->setVisible(false);

    BottleOutTimer = new QTimer(this);
    BottleOutTimer->setInterval(30000);
    BottleOutTimer->setSingleShot(true);
    connect(BottleOutTimer,SIGNAL(timeout()),this,SLOT(on_toolButton_bottle_clicked()));

    ui->dial_pump1_speed->setEnabled(false);
    ui->spinBox_pump1_speed->setEnabled(false);
    ui->dial_pump2_speed->setEnabled(false);
    ui->spinBox_pump2_speed->setEnabled(false);
    qDebug()<<"_water1======"<<q_water;
//    ui->pushButton_refresh->setVisible(false);
    ui->toolButton_querypump1->setVisible(false);
    ui->toolButton_pumpin->setVisible(false);
    ui->toolButton_pumpout->setVisible(false);
    ui->toolButton_waterout->setVisible(false);
    ui->toolButton_cooler->setVisible(false);
    ui->toolButton_bottle->setVisible(false);
}

dupumpcontrol::~dupumpcontrol()
{
    Water_Thread->quit();
    Water_Thread->wait();
    delete ui;
}

//void dupumpcontrol::retryConnection()
//{
//    qDebug() << "Retrying WebSocket connection..." << endl;
//    SerialClient->open(QUrl("ws://127.0.0.1:9706"));
//}

bool dupumpcontrol::init()
{
    connect(q_water,SIGNAL(BedZeroStatus(bool)),this,SLOT(Bed_ZeroStatus(bool)));
    connect(q_water,SIGNAL(sendser(QString)),this,SLOT(Send_Message(QString)));
    connect(this,SIGNAL(WaterReceiveData(QString)),q_water,SLOT(ReceiveData(QString)));
    if(q_water->init())
    {  
        q_water->moveToThread(Water_Thread);
        connect(this,SIGNAL(OpenPumpIN()),q_water,SLOT(OpenPumpIN()));
        connect(this,SIGNAL(OpenPumpOUT()),q_water,SLOT(OpenPumpOUT()));
        connect(this,SIGNAL(ClosePumpIN()),q_water,SLOT(ClosePumpIN()));
        connect(this,SIGNAL(ClosePumpOUT()),q_water,SLOT(ClosePumpOUT()));
        connect(this,SIGNAL(OpenValveIN()),q_water,SLOT(OpenValveIN()));
        connect(this,SIGNAL(CloseValveIN()),q_water,SLOT(CloseValveIN()));
        connect(this,SIGNAL(OpenValveOUT()),q_water,SLOT(OpenValveOUT()));
        connect(this,SIGNAL(CloseValveOUT()),q_water,SLOT(CloseValveOUT()));
        connect(this,SIGNAL(OpenCooler1()),q_water,SLOT(OpenCooler1()));
        connect(this,SIGNAL(CloseCooler1()),q_water,SLOT(CloseCooler1()));
        connect(this,SIGNAL(OpenCooler()),q_water,SLOT(OpenCooler2()));
        connect(this,SIGNAL(CloseCooler()),q_water,SLOT(CloseCooler2()));
        connect(this,SIGNAL(ControlCooler2(bool)),q_water,SLOT(WaterCooler2(bool)));
        connect(this,SIGNAL(OpenValveBottle()),q_water,SLOT(OpenValveBottle()));
        connect(this,SIGNAL(CloseValveBottle()),q_water,SLOT(CloseValveBottle()));
        connect(this,SIGNAL(OpenWaterOut()),q_water,SLOT(OpenWaterOut()));
        connect(this,SIGNAL(CloseWaterOut()),q_water,SLOT(CloseWaterOut()));
        connect(this,SIGNAL(OpenXHPump()),q_water,SLOT(OpenXHPump()));
        connect(this,SIGNAL(OpenZKPump()),q_water,SLOT(OpenZKPump()));
        connect(this,SIGNAL(CloseXHPump()),q_water,SLOT(CloseXHPump()));
        connect(this,SIGNAL(CloseZKPump()),q_water,SLOT(CloseZKPump()));
        connect(this,SIGNAL(AutoModeON()),q_water,SLOT(openAutoMode()));
        connect(this,SIGNAL(AutoModeOFF()),q_water,SLOT(closeAutoMode()));
        connect(this,SIGNAL(setPump1Speed(int)),q_water,SLOT(setduPump1(int)));
        connect(this,SIGNAL(setPump2Speed(int)),q_water,SLOT(setduPump2(int)));
        connect(this,SIGNAL(closePump1()),q_water,SLOT(closeduPump1()));
        connect(this,SIGNAL(closePump2()),q_water,SLOT(closeduPump2()));
        connect(this,SIGNAL(setWaterIn(bool)),q_water,SLOT(WaterIn(bool)));
        connect(this,SIGNAL(setWaterOut(bool)),q_water,SLOT(WaterOut(bool)));
        connect(this,SIGNAL(setWaterCycle(bool)),q_water,SLOT(WaterCycle(bool)));
        connect(this,SIGNAL(setMeter(int)),q_water,SLOT(setMeter(int)));
        connect(this,SIGNAL(queryMeter(bool)),q_water,SLOT(queryMeter(bool)));
        connect(this,SIGNAL(queryTemp(bool)),q_water,SLOT(queryTemp(bool)));
        connect(this,SIGNAL(queryVacuum(bool)),q_water,SLOT(queryVacuum(bool)));

        // 向Q_Water发送开始排tank水消息
        connect(this,SIGNAL(DrainTankStart()),q_water,SLOT(DrainTank()));

        // 收到Q_Water发送的Water tank drain complete的消息

        connect(q_water,SIGNAL(DrainTankFinished()),this,SLOT(DrainTankFinished()));

        connect(q_water,SIGNAL(TempUpdate(int)),this,SLOT(TempUpdate(int)));
        connect(q_water,SIGNAL(Temp2Update(int)),this,SLOT(Temp2Update(int)));
        connect(q_water,SIGNAL(MeterUpdate(int)),this,SLOT(updateMeterDat(int)));
//        connect(q_water,SIGNAL(VacuumUpdate(int)),this,SLOT(VacuumUpdate(int)));
        connect(q_water,SIGNAL(LevelUpdate(int)),this,SLOT(LevelUpdate(int)));
        connect(q_water,SIGNAL(SpeedOutUpdate(int)),this,SLOT(SpeedOutUpdate(int)));
        connect(q_water,SIGNAL(Pos_control(int,int)),this,SLOT(ReceiveHead(int,int)));
        connect(q_water,SIGNAL(GetBedPos(int,int,int)),this,SLOT(ReceiveRod(int,int,int)));
        connect(this,SIGNAL(SetRodPos(int,int,int)),q_water,SLOT(SetRodPos(int,int,int)));
        connect(this,SIGNAL(GetRodPos()),q_water,SLOT(GetRodPos()));
        connect(this,SIGNAL(SetBedZero()),q_water,SLOT(SetBedZero()));
//        connect(this,SIGNAL(GetWater(bool)),q_water,SLOT(getWater(bool)));
        connect(q_water,SIGNAL(Button_HeadOut(bool)),this,SLOT(HeadOut(bool)));
        connect(q_water,SIGNAL(Button_WaterIn(bool)),this,SLOT(WaterIn(bool)));
        connect(q_water,SIGNAL(Button_WaterOut(bool)),this,SLOT(WaterOut(bool)));
        connect(this,SIGNAL(ScreenNULL()),q_water,SLOT(SendScreenNULL()));
        connect(this,SIGNAL(ScreenWaterIn()),q_water,SLOT(SendScreenWaterIn()));
        connect(this,SIGNAL(ScreenWaterOut()),q_water,SLOT(SendScreenWaterOut()));
        connect(this,SIGNAL(ScreenManual()),q_water,SLOT(SendScreenManual()));
        connect(this,SIGNAL(ScreenAuto()),q_water,SLOT(SendScreenAuto()));
        connect(q_water,SIGNAL(ConnectFailed()),this,SLOT(WaterFailed()));
        connect(q_water,SIGNAL(WaterCycleState(bool)),this,SLOT(WaterCycleState(bool)));

        connect(this,SIGNAL(SetBedLock(bool)),q_water,SLOT(BedLock(bool)));
        ui->toolButton_automode->setEnabled(false);
        Water_Thread->start();
        emit SetBedLock(false);
        on_toolButton_querypump1_clicked();

        return true;
    }
    else
    {
        return false;
    }
}

void dupumpcontrol::SetMainWindow(NewMainWindow* main)
{
    m_pMainWin=main;
    if(m_pMainWin==NULL)
        return;
    connect(this,SIGNAL(pump1open()),m_pMainWin,SLOT(pump1open()));
    connect(this,SIGNAL(pump1close()),m_pMainWin,SLOT(pump1close()));
    connect(this,SIGNAL(pump1lock()),m_pMainWin,SLOT(pump1lock()));
    connect(this,SIGNAL(pump2open()),m_pMainWin,SLOT(pump2open()));
    connect(this,SIGNAL(pump2close()),m_pMainWin,SLOT(pump2close()));
    connect(this,SIGNAL(pump2lock()),m_pMainWin,SLOT(pump2lock()));
    connect(this,SIGNAL(updatespeed1(int)),m_pMainWin,SLOT(updatespeed_in(int)));
    connect(this,SIGNAL(updatespeed2(int)),m_pMainWin,SLOT(updatespeed_out(int)));
    connect(this,SIGNAL(updatemeter(float)),m_pMainWin,SLOT(updatemeter(float)));
    connect(this,SIGNAL(closeautomode()),this,SLOT(close_automode()));
    connect(this,SIGNAL(controllock()),m_pMainWin,SLOT(controllock()));
    connect(this,SIGNAL(controlunlock()),m_pMainWin,SLOT(controlunlock()));
    connect(this,SIGNAL(UpdateLevel(int)),m_pMainWin,SLOT(updatelevel(int)));

    // 向窗口发送Water tank drain complete的消息
    connect(this,SIGNAL(DrainTankFinishedToWindow()),m_pMainWin,SLOT(on_Drain_Water_Finished()));
//    connect(this,SIGNAL(setPump1Speed(int)),_water,SLOT(setduPump1(int)));
//    connect(this,SIGNAL(setPump2Speed(int)),_water,SLOT(setduPump2(int)));
//    connect(this,SIGNAL(closePump1()),_water,SLOT(closeduPump1()));
//    connect(this,SIGNAL(closePump2()),_water,SLOT(closeduPump2()));
//    connect(this,SIGNAL(getwater()),_water,SLOT(getwater()));
//    connect(this,SIGNAL(setMeter(int)),_water,SLOT(setMeter(int)));

}

void dupumpcontrol::on_toolButton_pump1_control_clicked()
{
    if (!m_bPump1Open)
    {
        if(lowlevel)
        {
            QFont m_msgBoxFont;
            m_msgBoxFont.setFamily("Microsoft YaHei");
            m_msgBoxFont.setPixelSize(16);
//            QMessageBox::information(0,"Notice","Water tank level is too low. Water bladder filling is paused.","Confirm",0);
            QMessageBox msgBox("Notice",
                               "Water tank level is too low. Water bladder filling is paused.",
                               QMessageBox::Information,
                               QMessageBox::Ok | QMessageBox::Escape,
                               QMessageBox::NoButton,
                               QMessageBox::NoButton);
            msgBox.setWindowFlags(Qt::WindowTitleHint|Qt::CustomizeWindowHint|Qt::WindowStaysOnTopHint);
            msgBox.setGeometry(QRect(1400,400,msgBox.width(),msgBox.height()));
            msgBox.setFont(m_msgBoxFont);
            msgBox.setButtonText(QMessageBox::Ok, "OK");
            msgBox.exec();
            return;
        }
        m_bPump1Open = true;
        Cooler2Flag = false;
        emit pump1lock();
        // 泵关闭
//        emit OpenZKPump();//真空泵
//        Delay_MSec(50);
//        emit OpenCooler();
        emit setWaterIn(true);
        Delay_MSec(50);
        ui->textEdit_shelf_check_status->setText("Fill peristaltic pump started.");
        ui->textEdit_shelf_check_status->append("Fill pump speed set to 70.");
        ui->dial_pump1_speed->setValue(70);
        emit updatespeed1(70);
//        on_dial_pump1_speed_sliderReleased();
//        emit setPump1Speed(70);
        ui->toolButton_pump1_control->setText("Stop Pump");
        ui->dial_pump1_speed->setEnabled(true);
//        ui->spinBox_pump1_speed->setEnabled(true);
        if(!meterAutoOpen){
            ui->toolButton_pump1_control->setEnabled(false);
            ui->toolButton_pump2_control->setEnabled(false);
            ui->toolButton_automode->setEnabled(false);
            ui->toolButton_setmeter->setEnabled(false);
            Delay_MSec(1000);
            ui->toolButton_pump1_control->setEnabled(true);
            ui->toolButton_pump2_control->setEnabled(true);
            ui->toolButton_setmeter->setEnabled(true);
            if(meterSet){
                ui->toolButton_automode->setEnabled(true);
            }
            emit pump1open();
        }
    }
    else
    {
        m_bPump1Open = false;
        emit pump1lock();
        // 泵开着
//        emit CloseCooler();
//        Delay_MSec(50);
//        emit CloseCooler2();
        ui->textEdit_shelf_check_status->setText("Fill peristaltic pump stopped.");
//        Delay_MSec(50);
//        emit CloseZKPump();//真空泵
//        emit closePump1();
        emit setWaterIn(false);
        ui->toolButton_pump1_control->setText("Start Pump");
        ui->dial_pump1_speed->setEnabled(false);
        ui->spinBox_pump1_speed->setEnabled(false);
        if(!meterAutoOpen){
            ui->toolButton_pump1_control->setEnabled(false);
            ui->toolButton_pump2_control->setEnabled(false);
            ui->toolButton_automode->setEnabled(false);
            ui->toolButton_setmeter->setEnabled(false);
            Delay_MSec(1000);
            ui->toolButton_pump1_control->setEnabled(true);
            ui->toolButton_pump2_control->setEnabled(true);
            ui->toolButton_setmeter->setEnabled(true);
            if(meterSet){
                ui->toolButton_automode->setEnabled(true);
            }
            emit pump1close();
        }
    }
}

void dupumpcontrol::on_toolButton_querypump1_clicked()
{
    autoquerywater=!autoquerywater;
    if(autoquerywater)
    {
        qDebug()<<"Query温度Level信息";
        q_water->getWater(true);
//        emit GetWater(true);
        ui->toolButton_querypump1->setText("Stop Query");
    }
    else
    {
        qDebug()<<"Stop Query温度Level信息";
        q_water->getWater(false);
//        emit GetWater(false);
        ui->toolButton_querypump1->setText("Query");
    }
}

void dupumpcontrol::on_spinBox_pump1_speed_valueChanged(int value)
{
    ui->dial_pump1_speed->setValue(value);
//    QString str("Set fill pump speed ");
//    str.append(QString::number(value));
//    ui->textEdit_shelf_check_status->setText(str);
}

void dupumpcontrol::on_dial_pump1_speed_valueChanged(int value)
{
    ui->spinBox_pump1_speed->setValue(value);
    pump1speed=value;
}

void dupumpcontrol::on_toolButton_pump2_control_clicked()
{
    if (!m_bPump2Open)
    {
        m_bPump2Open = true;
        emit pump2lock();
        // 泵关闭
        emit setWaterOut(true);
        ui->textEdit_shelf_check_status->setText("Drain peristaltic pump started.");
        ui->textEdit_shelf_check_status->append("Drain pump speed set to 70.");
        ui->dial_pump2_speed->setValue(70);
        emit updatespeed2(70);
//        emit setPump2Speed(70);
//        on_dial_pump2_speed_sliderReleased();
        m_Spd2_Set = 65;
        ui->toolButton_pump2_control->setText("Stop Pump");
        ui->dial_pump2_speed->setEnabled(true);
//        ui->spinBox_pump2_speed->setEnabled(true);
        if(!meterAutoOpen){
            ui->toolButton_pump1_control->setEnabled(false);
            ui->toolButton_pump2_control->setEnabled(false);
            ui->toolButton_automode->setEnabled(false);
            ui->toolButton_setmeter->setEnabled(false);
            Delay_MSec(1000);
            ui->toolButton_pump1_control->setEnabled(true);
            ui->toolButton_pump2_control->setEnabled(true);
            ui->toolButton_setmeter->setEnabled(true);
            if(meterSet){
                ui->toolButton_automode->setEnabled(true);
            }
            emit pump2open();
        }
    }
    else
    {
        m_bPump2Open = false;
        emit pump2lock();
        // 泵开着
        ui->textEdit_shelf_check_status->setText("Drain peristaltic pump stopped.");
//        emit closePump2();
        emit setWaterOut(false);
        qDebug()<<"close pump2";
        ui->toolButton_pump2_control->setText("Start Pump");
        ui->dial_pump2_speed->setEnabled(false);
        ui->spinBox_pump2_speed->setEnabled(false);
        if(!meterAutoOpen){
            ui->toolButton_pump1_control->setEnabled(false);
            ui->toolButton_pump2_control->setEnabled(false);
            ui->toolButton_automode->setEnabled(false);
            ui->toolButton_setmeter->setEnabled(false);
            Delay_MSec(1000);
            ui->toolButton_pump1_control->setEnabled(true);
            ui->toolButton_pump2_control->setEnabled(true);
            ui->toolButton_setmeter->setEnabled(true);
            if(meterSet){
                ui->toolButton_automode->setEnabled(true);
            }
            emit pump2close();
        }
    }
}

void dupumpcontrol::on_spinBox_pump2_speed_valueChanged(int value)
{
    ui->dial_pump2_speed->setValue(value);
//    QString str("Set drain pump speed ");
//    str.append(QString::number(value));
//    ui->textEdit_shelf_check_status->setText(str);
}

void dupumpcontrol::on_dial_pump2_speed_valueChanged(int value)
{
    ui->spinBox_pump2_speed->setValue(value);
    pump2speed=value;
}

void dupumpcontrol::on_pushButton_quit_clicked()
{
    this->close();
}

void dupumpcontrol::showFromMainWindow()
{
    if(this->isHidden()){
        this->show();
    }
    else{
        this->activateWindow();
    }
}

void dupumpcontrol::on_toolButton_setmeter_clicked()
{
    meterSet = true;
    short dat = ui->lineEdit_Meter->text().toFloat()*100 + FLOAT_DELTA;
    if (dat>1000)
    {
        dat=1000;
        ui->lineEdit_Meter->setText("10");
    }
    else if (dat<0)
    {
        dat=0;
        ui->lineEdit_Meter->setText("0");
    }
    m_PreMeter = dat;
    ui->textEdit_shelf_check_status->setText(QString("Pressure monitor setpoint = %1").arg((float)dat/100,0,'f',2));
    emit setMeter(m_PreMeter);
    repaint();
    if(!OverLevel)
        ui->toolButton_automode->setEnabled(true);
}

void dupumpcontrol::SpeedOutUpdate(int speed)
{
    ui->dial_pump2_speed->setValue(speed);
    emit updatespeed2(speed);
}

void dupumpcontrol::TempUpdate(int temp)
{
    qDebug()<<"TempUpdate"<<temp;
    tempGauge1->setValue(temp/10);
    emit Temp1Update(temp/10);
    if(temp<100&&(m_bPump1Open)&&(Cooler2Flag))
    {
        Cooler2Flag = false;
        emit ControlCooler2(false);
    }
    else if(temp>200&&(m_bPump1Open)&&(!Cooler2Flag))
    {
        Cooler2Flag = true;
        emit ControlCooler2(true);
    }
}

void dupumpcontrol::Temp2Update(int temp)
{
    tempGauge2->setValue(temp/10);
}

void dupumpcontrol::LevelUpdate(int level)
{
    qDebug()<<"LevelUpdate"<<level;
    emit UpdateLevel(level);
    if(level<=25)
    {
        if(!lowlevel)
        {
           ui->label_WaterLow->setStyleSheet("background-color: rgb(237, 242, 13);");
           ui->label_WaterNormal->setStyleSheet("background-color: rgb(197, 197, 197);");
           ui->label_WaterHigh->setStyleSheet("background-color: rgb(197, 197, 197);");
           lowlevel=true;
           HighLevel=false;
           OverLevel=false;
           bool boxflag = false;
           if(meterAutoOpen){
               on_toolButton_automode_clicked();
               QMessageBox msgBox("Notice","Water tank level is too low. Water circulation has been turned off. Fill the tank above 50% to keep water processing working properly.",
                                  QMessageBox::Warning,
                                  QMessageBox::Ok | QMessageBox::Escape,
                                  QMessageBox::NoButton,
                                  QMessageBox::NoButton);
               msgBox.setWindowFlags(Qt::WindowTitleHint|Qt::CustomizeWindowHint);
               msgBox.setGeometry(QRect(1400,400,msgBox.width(),msgBox.height()));
               QFont font;
               font.setFamily("Microsoft YaHei");
               font.setPixelSize(16);
               msgBox.setFont(font);
               msgBox.setButtonText(QMessageBox::Ok, "OK");
               int ret = msgBox.exec();
               if ((ret == QMessageBox::Ok)||(ret == QMessageBox::Close))
               {
                   return;
               }
               else
               {
                   return;
               }
               qDebug()<<"tank当前Water Level Low，已自动关闭Circulation";
           }
           if(m_bPump2Open){
               on_toolButton_pump2_control_clicked();
               boxflag = true;
           }
           if(m_bPump1Open){
               on_toolButton_pump1_control_clicked();
               boxflag = true;
           }
           if(boxflag){
               boxflag = false;
               QMessageBox msgBox("Notice","Water tank level is too low. Water bladder filling and draining have been turned off. Fill the tank above 50% to keep water processing working properly.",
                                  QMessageBox::Warning,
                                  QMessageBox::Ok | QMessageBox::Escape,
                                  QMessageBox::NoButton,
                                  QMessageBox::NoButton);
               msgBox.setWindowFlags(Qt::WindowTitleHint|Qt::CustomizeWindowHint);
               msgBox.setGeometry(QRect(1400,400,msgBox.width(),msgBox.height()));
               QFont font;
               font.setFamily("Microsoft YaHei");
               font.setPixelSize(16);
               msgBox.setFont(font);
               msgBox.setButtonText(QMessageBox::Ok, "OK");
               int ret = msgBox.exec();
               if ((ret == QMessageBox::Ok)||(ret == QMessageBox::Close))
               {
                   return;
               }
               else
               {
                   return;
               }
           }
        }
    }
    else if(level>25&&level<=50)
    {
       ui->label_WaterLow->setStyleSheet("background-color: rgb(197, 197, 197);");
       ui->label_WaterNormal->setStyleSheet("background-color: rgb(0, 238, 0);");
       ui->label_WaterHigh->setStyleSheet("background-color: rgb(197, 197, 197);");
       lowlevel=false;
       HighLevel=false;
       OverLevel=false;
    }
    else if(level>50)
    {
       ui->label_WaterLow->setStyleSheet("background-color: rgb(197, 197, 197);");
       ui->label_WaterNormal->setStyleSheet("background-color: rgb(197, 197, 197);");
       ui->label_WaterHigh->setStyleSheet("background-color: rgb(0, 238, 0);");
       lowlevel=false;
    }
    if(level>=70&&btnpumpin)
    {
        on_toolButton_pumpin_clicked();
        HighLevel=true;
        QMessageBox msgBox("Notice","The current water tank level supports treatment.",
                           QMessageBox::Warning,
                           QMessageBox::Ok | QMessageBox::Escape,
                           QMessageBox::NoButton,
                           QMessageBox::NoButton);
        msgBox.setWindowFlags(Qt::WindowTitleHint|Qt::WindowStaysOnTopHint);
        QFont font;
        font.setFamily("Microsoft YaHei");
        font.setPixelSize(16);
        msgBox.setFont(font);
        msgBox.setButtonText(QMessageBox::Ok, "OK");
        int ret = msgBox.exec();
        if ((ret == QMessageBox::Ok)||(ret == QMessageBox::Close))
        {
            return;
        }
        else
        {
            return;
        }
//        QMessageBox::information(0,"Notice","The current water tank level supports treatment.","Confirm",0);
    }
    if(level>=85&&(!OverLevel))
    {
        OverLevel = true;
        if(meterAutoOpen)
            on_toolButton_automode_clicked();
        if(m_bPump2Open)
            on_toolButton_pump2_control_clicked();
        if(m_bPump1Open)
            on_toolButton_pump1_control_clicked();
        emit controllock();
        ui->toolButton_pump2_control->setEnabled(false);
        ui->toolButton_pump1_control->setEnabled(false);
        ui->dial_pump2_speed->setEnabled(false);
        ui->dial_pump1_speed->setEnabled(false);
        ui->toolButton_automode->setEnabled(false);
        QMessageBox msgBox("Notice","Water tank level is too high. Drain the tank below 70% to keep water processing working properly.",
                           QMessageBox::Warning,
                           QMessageBox::Ok | QMessageBox::Escape,
                           QMessageBox::NoButton,
                           QMessageBox::NoButton);
        msgBox.setWindowFlags(Qt::WindowTitleHint|Qt::WindowStaysOnTopHint);
        QFont font;
        font.setFamily("Microsoft YaHei");
        font.setPixelSize(16);
        msgBox.setFont(font);
        msgBox.setButtonText(QMessageBox::Ok, "OK");
        int ret = msgBox.exec();
        if ((ret == QMessageBox::Ok)||(ret == QMessageBox::Close))
        {
            return;
        }
        else
        {
            return;
        }
    }
    if(level<=70)
    {

        if(!meterAutoOpen && OverLevel){
            ui->toolButton_pump1_control->setEnabled(true);
            ui->toolButton_pump2_control->setEnabled(true);
    //        ui->dial_pump2_speed->setEnabled(true);
    //        ui->dial_pump1_speed->setEnabled(true);
            if(meterSet){
                ui->toolButton_automode->setEnabled(true);
            }
            emit controlunlock();
            OverLevel = false;
        }
    }
    if(level==0)
    {
        if(btnpumpout&&(!BottleOutTimer->isActive()))
        {
            emit CloseValveOUT();
            emit OpenValveBottle();
            emit OpenCooler1();
            btnvalvebottle=true;
            ui->toolButton_bottle->setText("Bottle Valve Open");
            BottleOutTimer->start();
        }
    }
}

void dupumpcontrol::updatePump2Status(int m_Speed2)
{
    QString strS;
    if(m_Speed2&0x8000)
    {
        ui->toolButton_pump2_control->setText("Stop Pump");
        strS = "On";
    }
    else
    {
        ui->toolButton_pump2_control->setText("Start Pump");
        strS = "Off";
    }
    ui->textEdit_shelf_check_status->setText(QString("Peristaltic Pump 2: Speed = %1, Status: %2").arg((float)(m_Speed2 & 0xFFF)/10,0,'f',1).arg(strS));
}

void dupumpcontrol::updatePump1Status(int m_Speed1)
{
    QString strS;
    if(m_Speed1&0x8000)
    {
        ui->toolButton_pump1_control->setText("Stop Pump");
        strS = "On";
    }
    else
    {
        ui->toolButton_pump1_control->setText("Start Pump");
        strS = "Off";
    }
    ui->textEdit_shelf_check_status->setText(QString("Peristaltic Pump 1: Speed = %1, Status: %2").arg((float)(m_Speed1 & 0xFFF)/10,0,'f',1).arg(strS));
}

void dupumpcontrol::updateMeterDat(int m_RecMeter)
{
    qDebug()<<"updateMeterDat"<<m_RecMeter;
    if(m_RecMeter>32767)
        m_RecMeter=0;
//        m_RecMeter=m_RecMeter-65535;
    float recMeter=(float)m_RecMeter*1.00/100;
    for (int i=METER_QUANTITY-1;i>0;i--) m_Meter[i]=m_Meter[i-1];
    if(m_RecMeter<0)
        m_Meter[0] = 0;
    else
    {
        m_Meter[0] = m_RecMeter;
    }
    if(queryPressure)
    {
        emit updatemeter(recMeter);
        ui->label_RecMeter->setText(QString("%1").arg((float)m_RecMeter*1.00/100,0,'f',2));
        repaint();
    }
    if(meterAutoOpen&&m_bPump1Open&&m_bPump2Open)
    {
        if((m_RecMeter<100)||(m_RecMeter>2000))
        {
            warningPressureCount++;
            if(warningPressureCount>=6)
            {
                emit closeautomode();
                warningPressureCount = 0;
                return;
            }
        }
        else{
            warningPressureCount = 0;
        }

        int dlt = m_RecMeter - m_PreMeter;
        bool b_chg = false;
        switch(m_Spd2_Adj)
        {
            case 20:
                if (dlt<=5){
                    m_Spd2_Adj=5;
                    b_chg=true;
                }
                break;
            case -20:
                if (dlt>=-5){
                    m_Spd2_Adj=-5;
                    b_chg=true;
                }
                break;
            case 5:
                if (dlt>10){
                    m_Spd2_Adj=20;
                    b_chg=true;
                }else if (dlt<=0){
                    m_Spd2_Adj=0;
                    b_chg=true;
                }
                break;
            case -5:
                if (dlt<-10){
                    m_Spd2_Adj=-20;
                    b_chg=true;
                }else if (dlt>=0){
                    m_Spd2_Adj=0;
                    b_chg=true;
                }
                break;
            case 0:
                if (dlt>5){
                    m_Spd2_Adj=5;
                    b_chg=true;
                }else if (dlt<-5){
                    m_Spd2_Adj=-5;
                    b_chg=true;
                }
                break;
            default:
                m_Spd2_Adj=0;
                break;
        }
//        if(b_chg)
        emit signalAdjPump2();

    }
}

void dupumpcontrol::paintEvent(QPaintEvent *)
{
    int X0,Y0,Xw,Yh;
    float Xzm,Yzm;
    int Xb,Yb,Xe,Ye;
    X0=910,Y0=1020;                  //0点坐标(右偏下)
    Xw=500,Yh=400;Xzm=500/METER_QUANTITY;Yzm=Yh/10;  //大小500x310,比例

    QPainter painter(this);
    //画监控线
    QPen pen;
    pen.setColor(Qt::blue);
    pen.setStyle(Qt::DashLine);
    painter.setPen(pen);
    Xb=X0;Xe=X0-Xw;Yb=Y0-m_PreMeter*Yzm/100;Ye=Yb;
    painter.drawLine(QPointF(Xb,Yb), QPointF(Xe,Ye));
    //画PressureLive曲线
    pen.setColor(Qt::black);
    pen.setStyle(Qt::SolidLine);
    painter.setPen(pen);
    Xe=X0;
    if(m_Meter[0]<0)
        Ye=Y0;
    else
        Ye=Y0-m_Meter[0]*Yzm/100;           //第一个点
    for (int i=0;i<METER_QUANTITY;i++) {
        Xb=Xe;Yb=Ye;                        //上一个点
        Xe=X0-i*Xzm;
        if(m_Meter[i]<0)
            Ye=Y0;
        else
            Ye=Y0-m_Meter[i]*Yzm/100; //新点
        painter.drawLine(QPointF(Xb,Yb), QPointF(Xe,Ye));
    }
}

void dupumpcontrol::on_toolButton_automode_clicked()
{
    if(!meterAutoOpen)
    {
        if(lowlevel)
        {
            QMessageBox msgBox("Notice","Water tank level is too low. Water circulation is paused.",
                               QMessageBox::Warning,
                               QMessageBox::Ok | QMessageBox::Escape,
                               QMessageBox::NoButton,
                               QMessageBox::NoButton);
            msgBox.setWindowFlags(Qt::WindowTitleHint|Qt::WindowStaysOnTopHint);
            QFont font;
            font.setFamily("Microsoft YaHei");
            font.setPixelSize(16);
            msgBox.setFont(font);
            msgBox.setButtonText(QMessageBox::Ok, "OK");
            int ret = msgBox.exec();
            if ((ret == QMessageBox::Ok)||(ret == QMessageBox::Close))
            {
                return;
            }
            else
            {
                return;
            }
//            QMessageBox::information(0,"Notice","Water tank level is too low. Water circulation is paused.","Confirm",0);
            return;
        }
//        if(!autoquerywater)
//            on_toolButton_querypump1_clicked();
        ui->textEdit_shelf_check_status->setText("Auto mode enabled. The software adjusts Pump 2 speed based on pressure readings.");
//        emit ScreenAuto();
        meterAutoOpen = true;
//        if(!m_bPump1Open){
//            on_toolButton_pump1_control_clicked();
//        }
//        else{
//            ui->dial_pump1_speed->setValue(70);
//            on_dial_pump1_speed_sliderReleased();
//        }
//        if(!m_bPump2Open){
//            on_toolButton_pump2_control_clicked();
//        }
//        else{
//            ui->dial_pump2_speed->setValue(65);
//            on_dial_pump2_speed_sliderReleased();
//        }
        emit setWaterCycle(true);
        m_bPump1Open = true;
        m_bPump2Open = true;
        Cooler2Flag = false;
        ui->dial_pump1_speed->setValue(70);
        ui->toolButton_pump1_control->setText("Stop Pump");
        ui->dial_pump1_speed->setEnabled(true);
        emit pump1open();
        emit updatespeed1(70);
        ui->dial_pump2_speed->setValue(65);
        m_Spd2_Set = 65;
        ui->toolButton_pump2_control->setText("Stop Pump");
        ui->dial_pump2_speed->setEnabled(true);
        emit pump2open();
        emit updatespeed2(65);

        ui->toolButton_pump1_control->setEnabled(false);
        ui->dial_pump1_speed->setEnabled(false);
        ui->spinBox_pump1_speed->setEnabled(false);

        ui->toolButton_pump2_control->setEnabled(false);
        ui->dial_pump2_speed->setEnabled(false);
        ui->spinBox_pump2_speed->setEnabled(false);

        emit controllock();
//        emit AutoModeON();
        connect(this,SIGNAL(signalAdjPump2()),this,SLOT(slotAdjPump2()));
        ui->toolButton_automode->setText("Manual Control");
        ui->toolButton_automode->setEnabled(false);
        ui->toolButton_setmeter->setEnabled(false);
        Delay_MSec(1000);
        ui->toolButton_automode->setEnabled(true);
        ui->toolButton_setmeter->setEnabled(true);
    }
    else
    {
        ui->textEdit_shelf_check_status->setText("Manual mode enabled. Control pump speed manually.");
//        emit ScreenManual();
        meterAutoOpen = false;
        m_bPump1Open = false;
        m_bPump2Open = false;
        warningPressureCount = 0;

//        if(m_bPump1Open){
//            on_toolButton_pump1_control_clicked();
//        }
//        if(m_bPump2Open){
//            on_toolButton_pump2_control_clicked();
//        }
//        emit AutoModeOFF();
//        emit ScreenNULL();
        disconnect(this,SIGNAL(signalAdjPump2()),this,SLOT(slotAdjPump2()));
//        ui->toolButton_automode->setText("Auto Control");
        ui->toolButton_automode->setEnabled(false);
        ui->toolButton_setmeter->setEnabled(false);
//        Delay_MSec(1000);
//        ui->toolButton_automode->setEnabled(true);
//        ui->toolButton_setmeter->setEnabled(true);
        emit setWaterCycle(false);
//        ui->toolButton_pump1_control->setText("Start Pump");
//        ui->dial_pump1_speed->setEnabled(false);
//        ui->spinBox_pump1_speed->setEnabled(false);
//        emit pump1close();
//        ui->toolButton_pump2_control->setText("Start Pump");
//        ui->dial_pump2_speed->setEnabled(false);
//        ui->spinBox_pump2_speed->setEnabled(false);
//        emit pump2close();

//        ui->toolButton_pump1_control->setEnabled(true);

//        ui->toolButton_pump2_control->setEnabled(true);
//        emit controlunlock();
    }
}

void dupumpcontrol::slotAdjPump2()
{
    ui->dial_pump2_speed->setValue(m_Spd2_Set+m_Spd2_Adj);
    on_dial_pump2_speed_sliderReleased();
}

void dupumpcontrol::controlpump1()
{
    on_toolButton_pump1_control_clicked();
}

void dupumpcontrol::controlpump2()
{
    on_toolButton_pump2_control_clicked();
}

void dupumpcontrol::speed1up()
{
    int value;
    value = ui->spinBox_pump1_speed->value() + 10;
    if(value>100)
        value = 100;
    ui->spinBox_pump1_speed->setValue(value);
    emit setPump1Speed(value);
    emit updatespeed1(value);
    QString str("Set fill pump speed ");
    str.append(QString::number(value));
    ui->textEdit_shelf_check_status->setText(str);
}

void dupumpcontrol::speed1down()
{
    int value;
    value = ui->spinBox_pump1_speed->value() - 10;
    if(value<0)
        value = 0;
    ui->spinBox_pump1_speed->setValue(value);
    emit setPump1Speed(value);
    emit updatespeed1(value);
    QString str("Set fill pump speed ");
    str.append(QString::number(value));
    ui->textEdit_shelf_check_status->setText(str);
}

void dupumpcontrol::speed2up()
{
    int value;
    value = ui->spinBox_pump2_speed->value() + 10;
    if(value>100)
        value = 100;
    ui->spinBox_pump2_speed->setValue(value);
    emit setPump2Speed(value);
    emit updatespeed2(value);
    QString str("Set drain pump speed ");
    str.append(QString::number(value));
    ui->textEdit_shelf_check_status->setText(str);
}

void dupumpcontrol::speed2down()
{
    int value;
    value = ui->spinBox_pump2_speed->value() - 10;
    if(value<0)
        value = 0;
    ui->spinBox_pump2_speed->setValue(value);
    emit setPump2Speed(value);
    emit updatespeed2(value);
    QString str("Set drain pump speed ");
    str.append(QString::number(value));
    ui->textEdit_shelf_check_status->setText(str);
}

void dupumpcontrol::close_automode()
{
    ui->textEdit_shelf_check_status->setText("Manual mode enabled. Control pump speed manually.");
    meterAutoOpen = false;
    m_bPump1Open = false;
    m_bPump2Open = false;
    warningPressureCount = 0;

    disconnect(this,SIGNAL(signalAdjPump2()),this,SLOT(slotAdjPump2()));
    ui->toolButton_automode->setEnabled(false);
    ui->toolButton_setmeter->setEnabled(false);
//    ui->toolButton_automode->setText("Auto Control");
//    on_toolButton_automode_clicked();
//    m_bPump1Open=true;
//    m_bPump2Open=true;
//    on_toolButton_pump1_control_clicked();
//    on_toolButton_pump2_control_clicked();
//    Delay_MSec(1000);
    emit setWaterCycle(false);
//    ui->toolButton_pump1_control->setText("Start Pump");
//    ui->dial_pump1_speed->setEnabled(false);
//    ui->spinBox_pump1_speed->setEnabled(false);
//    emit pump1close();
//    ui->toolButton_pump2_control->setText("Start Pump");
//    ui->dial_pump2_speed->setEnabled(false);
//    ui->spinBox_pump2_speed->setEnabled(false);
//    emit pump2close();
//    ui->toolButton_pump1_control->setEnabled(true);
//    ui->toolButton_pump2_control->setEnabled(true);
//    emit controlunlock();
//    ui->toolButton_automode->setEnabled(true);
    QMessageBox msgBox("Water Bladder Pressure Out of Range","Water circulation has been turned off. Make sure water bladder pressure is above 1 kPa before restarting.",
                       QMessageBox::Warning,
                       QMessageBox::Ok | QMessageBox::Escape,
                       QMessageBox::NoButton,
                       QMessageBox::NoButton);
    msgBox.setWindowFlags(Qt::WindowTitleHint|Qt::CustomizeWindowHint);
    msgBox.setGeometry(QRect(1400,400,msgBox.width(),msgBox.height()));
    QFont font;
    font.setFamily("Microsoft YaHei");
    font.setPixelSize(16);
    msgBox.setFont(font);
//    msgBox.setButtonText(QMessageBox::Ok, QStringLiteral("OK"));
    msgBox.setButtonText(QMessageBox::Ok, "OK");
    int ret = msgBox.exec();
    if ((ret == QMessageBox::Ok)||(ret == QMessageBox::Close))
    {
        return;
    }
    else
    {
        return;
    }
}

void dupumpcontrol::on_toolButton_pumpin_clicked()
{
    if(btnpumpout||btnwaterout||HighLevel)
    {
        return;
    }
    btnpumpin = !btnpumpin;
    if(btnpumpin&&!btnpumpout)
    {
        ui->toolButton_pumpin->setText("Filling");
        if(!autoquerywater)
        {
            on_toolButton_querypump1_clicked();
        }
//        emit OpenPumpIN();
        emit OpenValveIN();
        emit ScreenWaterIn();
    }
    else if(!btnpumpin&&!btnpumpout)
    {
        ui->toolButton_pumpin->setText("Fill Water Tank");
//        emit ClosePumpIN();
        emit CloseValveIN();
        emit ScreenNULL();
    }
}


void dupumpcontrol::on_toolButton_pumpout_clicked()
{
    if(btnpumpin||btnwaterout)
    {
        return;
    }
    btnpumpout = !btnpumpout;
    if(btnpumpout&&!btnpumpin)
    {
        ui->toolButton_pumpout->setText("Draining");
        if(!autoquerywater)
        {
            on_toolButton_querypump1_clicked();
        }
        emit OpenPumpOUT();
        emit OpenValveOUT();
        emit ScreenWaterOut();
    }
    else if(!btnpumpout&&!btnpumpin)
    {
        ui->toolButton_pumpout->setText("Drain Water Tank");
        emit ClosePumpOUT();
        emit CloseValveOUT();
        emit ScreenNULL();
    }
}


void dupumpcontrol::on_toolButton_waterout_clicked()
{
    if(btnpumpin||btnpumpout)
    {
        return;
    }
    btnwaterout = !btnwaterout;
    if(btnwaterout)
    {
        ui->toolButton_waterout->setText("Draining");
        emit OpenPumpOUT();
        emit OpenWaterOut();
    }
    else
    {
        ui->toolButton_waterout->setText("Drain Water Bladder");
        emit ClosePumpOUT();
        emit CloseWaterOut();
    }
}


void dupumpcontrol::on_toolButton_cooler_clicked()
{
    btncooler = !btncooler;
    if(btncooler)
    {
        ui->toolButton_cooler->setText("Cooling");
        emit OpenCooler();
        emit ControlCooler2(true);
    }
    else
    {
        ui->toolButton_cooler->setText("Cooling");
        emit CloseCooler();
        emit ControlCooler2(false);
    }
}


void dupumpcontrol::on_toolButton_bottle_clicked()
{
    btnvalvebottle = !btnvalvebottle;
    if(btnvalvebottle)
    {
        ui->toolButton_bottle->setText("Bottle Valve Open");
        emit OpenValveBottle();
        emit OpenPumpOUT();
        emit OpenCooler1();
    }
    else
    {
        ui->toolButton_bottle->setText("Vacuum Bottle Valve");
        emit CloseValveBottle();
        emit ClosePumpOUT();
        emit CloseCooler1();
        if(btnpumpout)
        {
            btnpumpout=false;
            ui->toolButton_pumpout->setText("Drain Water Tank");
            emit ScreenNULL();
            QMessageBox msgBox("Notice","Water tank drain complete",
                               QMessageBox::Warning,
                               QMessageBox::Ok | QMessageBox::Escape,
                               QMessageBox::NoButton,
                               QMessageBox::NoButton);
            msgBox.setWindowFlags(Qt::WindowTitleHint|Qt::WindowStaysOnTopHint);
            QFont font;
            font.setFamily("Microsoft YaHei");
            font.setPixelSize(16);
            msgBox.setFont(font);
            msgBox.setButtonText(QMessageBox::Ok, "OK");
            int ret = msgBox.exec();
            if ((ret == QMessageBox::Ok)||(ret == QMessageBox::Close))
            {
                return;
            }
            else
            {
                return;
            }
//            QMessageBox::information(0,"Notice","Water tank drain complete","Confirm",0);
        }
    }
}

void dupumpcontrol::ReceiveHead(int AID, int direction)
{
    emit ReceiveHeadControl(AID,direction);
}

void dupumpcontrol::ReceiveRod(int posx, int posy, int posz)
{
    emit ReceiveRodPos(posx,posy,posz);
}

void dupumpcontrol::RodControl(int id,int direction,int pos)
{
    emit SetRodPos(id,direction,pos);
}

void dupumpcontrol::Get_RodPos()
{
    emit GetRodPos();
}

void dupumpcontrol::Set_BedZero()
{
    emit SetBedZero();
}

void dupumpcontrol::Bed_ZeroStatus(bool flag)
{
    if(flag)
    {
        emit BedZeroStatus(true);
    }
    else
        emit BedZeroStatus(false);
}

void dupumpcontrol::Send_Message(QString str)
{
    SerialClient->sendTextMessage(str);
}

void dupumpcontrol::TreatState(bool state)
{
    qDebug()<<"dupump control set bed lock !"<<state;
    emit SetBedLock(state);
}

// 收到Water tank drain complete消息处理槽函数
void dupumpcontrol::DrainTankFinished()
{
    emit DrainTankFinishedToWindow();
}

void dupumpcontrol::on_Drain_Tank_from_Window()
{
    qDebug()<<"Dupump receive drain tank from window !";
    emit DrainTankStart();
}

void dupumpcontrol::HeadOut(bool flag)
{
    if(btnpumpin||btnpumpout)
    {
        return;
    }
    if(flag)
    {
        btnwaterout=false;
        on_toolButton_waterout_clicked();
    }
    else
    {
        btnwaterout=true;
        on_toolButton_waterout_clicked();
    }
}

void dupumpcontrol::WaterIn(bool flag)
{
    if(btnpumpout||btnwaterout)
    {
        return;
    }
    if((flag==true&&btnpumpin==false)||(flag==false&&btnpumpin==true))
    {
        on_toolButton_pumpin_clicked();
    }
}

void dupumpcontrol::WaterOut(bool flag)
{
    if(btnpumpin||btnwaterout)
    {
        return;
    }
    if((flag==true&&btnpumpout==false)||(flag==false&&btnpumpout==true))
    {
        on_toolButton_pumpout_clicked();
    }
}

void dupumpcontrol::WaterFailed()
{
    emit ConnectFailed(WATER_CYCLE_NONAVAILABLE);
}

void dupumpcontrol::WaterCycleState(bool open)
{
    if(!open){
        Delay_MSec(1000);
        ui->toolButton_automode->setText("Auto Control");
        ui->toolButton_automode->setEnabled(true);
        ui->toolButton_setmeter->setEnabled(true);
        ui->toolButton_pump1_control->setText("Start Pump");
        ui->dial_pump1_speed->setEnabled(false);
        ui->spinBox_pump1_speed->setEnabled(false);
        emit pump1close();
        ui->toolButton_pump2_control->setText("Start Pump");
        ui->dial_pump2_speed->setEnabled(false);
        ui->spinBox_pump2_speed->setEnabled(false);
        emit pump2close();
        ui->toolButton_pump1_control->setEnabled(true);
        ui->toolButton_pump2_control->setEnabled(true);
        emit controlunlock();
    }
}

void dupumpcontrol::keyPressEvent(QKeyEvent *event)
{
    switch(event->key())
    {
    case Qt::Key_Escape:
        break;
    case Qt::Key_Return:
        break;
    default:
        QDialog::keyPressEvent(event);
    }
}

void dupumpcontrol::Delay_MSec(unsigned int msec)
{
    QTime _Timer = QTime::currentTime().addMSecs(msec);
    while(QTime::currentTime() < _Timer)
        QCoreApplication::processEvents(QEventLoop::AllEvents,100);
}

void dupumpcontrol::on_toolButton_queryMeter_clicked()
{
    queryPressure=!queryPressure;
    if(queryPressure)
    {
        ui->toolButton_queryMeter->setText("Stop Query");
    }
    else
    {
        ui->toolButton_queryMeter->setText("Query Pressure");
    }
}


void dupumpcontrol::on_toolButton_querywater_clicked()
{
}


void dupumpcontrol::on_dial_pump1_speed_sliderReleased()
{
    QString str("Set fill pump speed ");
    str.append(QString::number(pump1speed));
    emit setPump1Speed(pump1speed);
    ui->textEdit_shelf_check_status->setText(str);
    emit updatespeed1(pump1speed);
}


void dupumpcontrol::on_dial_pump2_speed_sliderReleased()
{
    QString str("Set drain pump speed ");
    str.append(QString::number(pump2speed));
    emit setPump2Speed(pump2speed);
    ui->textEdit_shelf_check_status->setText(str);
    emit updatespeed2(pump2speed);
}
