#include "selftestdlg.h"
#include "ui_selftestdlg.h"
#include "sampletotemperature.h"
#include <QMovie>
#include <QStandardItemModel>

Q_LOGGING_CATEGORY(SELFCHECK,"SELFCHECK")

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

    iSelfTestProgress=0;
    SubSysStatus.clear();
    SubSysStatus.resize(5);
    SubSysStatus.fill(false);

    pixOK.load(":/umimg/UMImage/success_48px.png");
    pixNO.load(":/umimg/UMImage/error_48px.png");
    pixUnknown.load(":/umimg/UMImage/unknown_48px.png");

    timer=new QTimer(this);

    lblList.append(ui->label_append);
    lblList.append(ui->label_location);
    lblList.append(ui->label_warter);
    lblList.append(ui->label_network);
    lblList.append(ui->label_imageb);

    //movieLoad = new QMovie(":/umimg/UMImage/loading_48px.gif");
    foreach (QLabel *lbl, lblList) {
        lbl->setPixmap(pixUnknown);
    }
    //movieLoad->start();

    sindex=0;
    m_networkservice = 0;
    connect(timer,SIGNAL(timeout()),this,SLOT(timeoutslot()));

    //水循环部分
//    initShelfCheckTimer();
//    m_iCheckCircle = 0;

//    m_bPumpOpen = false;    //设置泵关闭
//    ui->textEdit_shelf_check_status->setText("close pump !");
//    closePump();

//    tempGauge1=new QcwThermometer(this);
//    tempGauge1->setBackground(QColor::fromRgb(197,197,197));
//    tempGauge1->setBorderColor(Qt::gray);
//    //tempGauge1->setGeometry(160,570,145,360);
//    tempGauge1->setMinValue(-10);
//    tempGauge1->setMaxValue(50);
//    tempGauge1->setScaleMajor(6);
//    tempGauge1->setScaleMinor(10);
//    ui->verticalLayout_tempGaugeLeft->addWidget(tempGauge1);

//    tempGauge2=new QcwThermometer(this);
//    tempGauge2->setBackground(QColor::fromRgb(197,197,197));
//    tempGauge2->setBorderColor(Qt::gray);
//    //tempGauge2->setGeometry(380,570,145,360);
//    tempGauge2->setMinValue(-10);
//    tempGauge2->setMaxValue(50);
//    tempGauge2->setScaleMajor(6);
//    tempGauge2->setScaleMinor(10);
//    ui->verticalLayout__tempGaugeRight->addWidget(tempGauge2);

//    levelQueryTimer =new QTimer(this);
//    levelQueryTimer->setInterval(30000);
//    connect(levelQueryTimer,SIGNAL(timeout()),this,SLOT(on_toolButton_query_clicked()));

//    levelQueryTimer->stop();

//    m_pAmpModel = NULL;
//    initAmpParaTable();

//    m_table = new TemperTable;

    setUserModel();
}

SelfTestDlg::~SelfTestDlg()
{
    if (selfTestStatus!=NULL)
    {
        delete selfTestStatus;
        selfTestStatus = NULL;
    }
    if (timer!=NULL)
    {
        delete timer;
        timer = NULL;
    }
    if (tempGauge1!=NULL)
    {
        delete tempGauge1;
        tempGauge1 = NULL;
    }
    if (tempGauge2!=NULL)
    {
        delete tempGauge2;
        tempGauge2 = NULL;
    }
    if (levelQueryTimer!=NULL)
    {
        delete levelQueryTimer;
        levelQueryTimer = NULL;
    }

    if (m_pAmpModel!=NULL)
    {
        delete m_pAmpModel;
        m_pAmpModel = NULL;
    }

    if (m_table!=NULL)
    {
        delete m_table;
        m_table = NULL;
    }

    delete ui;
}

void SelfTestDlg::setUserModel()
{
    // 从配置文件中读取用户模式
    QSettings *settings = new QSettings(SETTINGS_PATH, QSettings::IniFormat);
    int model = settings->value("MainFrame/UserModel").toString().toUInt();
    qCDebug(SELFCHECK()) << SELFCHECK().categoryName()
                         << "user model is "<< model
                         << TRACE_CMH();
    switch (model)
    {
        case 0:
//            setUserModel(0);
            break;

        case 1:
            break;
    }

    delete settings;


}

//void SelfTestDlg::setUserModel(int model)
//{
//    if (model==0)
//    {
//        ui->toolButton_refresh_tempature->setEnabled(false);
//        ui->toolButton_refresh_tempature->setStyleSheet(NOT_WORK);
//        ui->toolButton_refresh_voltage->setEnabled(false);
//        ui->toolButton_refresh_voltage->setStyleSheet(NOT_WORK);
//        ui->tableView->setEnabled(false);
//        ui->tableView->setStyleSheet(NOT_WORK);
//    }
//}

void SelfTestDlg::SetMainWindow(NewMainWindow* main)
{
    m_pMainWin=main;
    if(m_pMainWin==NULL)
        return;
//    connect(this,SIGNAL(setPumpSpeed(int)),m_pMainWin,SLOT(setPumpSpeed(int)));
//    connect(this,SIGNAL(openPump()),m_pMainWin,SLOT(openPump()));
//    connect(this,SIGNAL(closePump()),m_pMainWin,SLOT(closePump()));
//    connect(this,SIGNAL(queryStatus()),m_pMainWin,SLOT(queryStatus()));

//    connect(this,SIGNAL(queryAmpTempature()),m_pMainWin,SLOT(queryAmpTempature()));
//    connect(this,SIGNAL(queryAmpVoltage()),m_pMainWin,SLOT(queryAmpVoltage()));
//    connect(m_pMainWin,SIGNAL(refreshAmpParaInTable(DataFlag,QByteArray&)),
//            this,SLOT(refreshAmpParaInTable(DataFlag,QByteArray&)));
}

void SelfTestDlg::showCheck()
{
//    _isShowFromLogin = true;
//    this->show();
//    timer->start(1000);
    ui->textEdit_shelf_check_status->append("Running self-test...");
    _isShowFromLogin = true;
    bOK=true;
    iSelfTestProgress=0;
    if(!this->isHidden()){
        bOK=true;
        foreach (QLabel *lbl, lblList) {
            lbl->setPixmap(pixUnknown);
        }
        //movieLoad->start();
        this->activateWindow();
    }
    else
    {
        this->show();
    }

}

void SelfTestDlg::showFromMainWindow()
{
    ui->pushButton_refresh->setEnabled(false);
    _isShowFromLogin = false;
    if(this->isHidden()){
        this->show();
    }
    else{
        this->activateWindow();
    }
}

void SelfTestDlg::updateStatus(bool bappend, bool blocation, bool bwarter, bool bnetwork, bool bimagegraber)
{
    qDebug()<<"SelfTestDlg::updateStatus "<<bappend<<blocation<<bwarter<<bnetwork<<bimagegraber;
    selfTestStatus[iappned]=bappend;
    selfTestStatus[ilocation]=blocation;
    selfTestStatus[iwarter]=bwarter;
    selfTestStatus[inetwork]=bnetwork;
    selfTestStatus[iimagegraber]=bimagegraber;
    //如果是刷新状态则执行
    if(!this->isHidden()){
        bOK=true;
        foreach (QLabel *lbl, lblList) {
            //lbl->setMovie(movieLoad);
        }
        //movieLoad->start();

        sindex=0;
        timer->start(1000);
    }
}

void SelfTestDlg::updateStatus(selfSubSys sys,bool status)
{
    SubSysStatus[sys]=status;
    if(status)
    {
        lblList[sys]->setPixmap(pixOK);
    }
    else
    {
        bOK=false;
        lblList[sys]->setPixmap(pixNO);
        switch (sys) {
        case iappned:
            ui->textEdit_shelf_check_status->append("Control box initialization failed.\nCheck the control box USB serial cable and port number.");
            break;
        case ilocation:
            ui->textEdit_shelf_check_status->append("Motion system initialization failed.\nCheck the motion system USB cable.");
            break;
        case iwarter:
            ui->textEdit_shelf_check_status->append("Water circulation initialization failed.\nCheck the water circulation USB serial cable and port number.");
            break;
        case inetwork:
            ui->textEdit_shelf_check_status->append("Network initialization failed.\nCheck the network cable to the control PC and make sure the control PC software is running.");
            break;
        case iimagegraber:
            ui->textEdit_shelf_check_status->append("Ultrasound image initialization failed.\nCheck the image capture card and driver.");
            break;
        default:
            break;
        }
    }
    iSelfTestProgress++;
    ui->progressBar->setValue(iSelfTestProgress*20);
}

void SelfTestDlg::timeoutslot()
{
    // 查询，刷新水循环状态
    if(sindex>4)
    {
        timer->stop();
        if(bOK){
            ui->pushButton_refresh->setEnabled(false);
            ui->pushButton_refresh->setStyleSheet(NOT_WORK);
        }
        else{
            ui->pushButton_refresh->setEnabled(true);
            ui->pushButton_refresh->setStyleSheet(SELECTED);
        }
        emit queryStatus();
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
        switch (sindex) {
        case 0:
            ui->textEdit_shelf_check_status->append("Control box initialization failed.\nCheck the control box USB serial cable and port number.");
            break;
        case 1:
            ui->textEdit_shelf_check_status->append("Motion system initialization failed.\nCheck the motion system USB cable.");
            break;
        case 2:
            ui->textEdit_shelf_check_status->append("Water circulation initialization failed.\nCheck the water circulation USB serial cable and port number.");
            break;
        case 3:
            ui->textEdit_shelf_check_status->append("Network initialization failed.\nCheck the network cable to the control PC and make sure the control PC software is running.");
            break;
        case 4:
            ui->textEdit_shelf_check_status->append("Ultrasound image initialization failed.\nCheck the image capture card and driver.");
            break;
        default:
            break;
        }
    }
    ui->progressBar->setValue((sindex+1)*20);
    sindex++;

    timer->start(500);
}

void SelfTestDlg::networkreflash()
{
    ui->textEdit_shelf_check_status->append("Network initialization failed. Reconnecting...");
    m_networkservice++;
    QString str = QString("Reconnect attempt %1...").arg(m_networkservice);
    ui->textEdit_shelf_check_status->append(str);
}

//void SelfTestDlg::on_toolButton_pump_control_clicked()
//{
//    if (!m_bPumpOpen)
//    {
//        // 泵关闭
//        ui->textEdit_shelf_check_status->setText("open pump !");
//        openPump();
//        ui->textEdit_shelf_check_status->append("Set pump speed 40 !");
//        ui->dial_pump_speed->setValue(40);
//        emit setPumpSpeed(40);
//        ui->toolButton_pump_control->setText("Stop Pump");
//        //ui->toolButton_pump_control->setStyleSheet(REDCOLOR);
//        m_bPumpOpen = true;
//        levelQueryTimer->start();
//        on_toolButton_query_clicked();
//        ui->dial_pump_speed->setEnabled(true);
//    }
//    else
//    {
//        // 泵开着
//        ui->textEdit_shelf_check_status->setText("close pump !");
//        closePump();
//        ui->toolButton_pump_control->setText("Start Pump");
//        //ui->toolButton_pump_control->setStyleSheet(SELECTED);
//        m_bPumpOpen = false;
//        levelQueryTimer->stop();
//        ui->dial_pump_speed->setEnabled(false);
//    }
//}

//void SelfTestDlg::on_toolButton_query_clicked()
//{
//    qDebug()<<"查询温度液位信息";
//    emit queryStatus();
//}

//void SelfTestDlg::on_spinBox_pump_speed_valueChanged(int value)
//{
//    //ui->horizontalScrollBar_pump_speed->setValue(value);
//    ui->dial_pump_speed->setValue(value);
//    QString str("Set pump speed ");
//    str.append(QString::number(value));
//    // this->setWindowFlags(Qt::WindowStaysOnTopHint);
//    ui->textEdit_shelf_check_status->setText(str);
//}

//void SelfTestDlg::initShelfCheckTimer()
//{
//   _shelfCheckTimer = new QTimer(this);
//   _shelfCheckTimer->setInterval(10000);
//   _shelfCheckTimer->setSingleShot(false);
//   connect(_shelfCheckTimer, SIGNAL(timeout()), this, SLOT(on_shelf_check_timer_timeout()));
//}

//void SelfTestDlg::on_shelf_check_timer_timeout()
//{
//    ++m_iCheckCircle;
//    QString info("Self-Test Cycle");
//    info.append(" ");
//    info.append(QString::number(m_iCheckCircle));
//    ui->textEdit_shelf_check_status->append(outputCheckInfo(info));
//    queryStatus();

//    info = "温度1";
//    info.append(" ");
//    info.append(QString::number(m_iTemperature1));
//    ui->textEdit_shelf_check_status->append(outputCheckInfo(info));

//    info = "温度2";
//    info.append(" ");
//    info.append(QString::number(m_iTemperature2));
//    ui->textEdit_shelf_check_status->append(outputCheckInfo(info));

//    info = "液位";
//    info.append(" ");
//    info.append(QString::number(m_iLevel));
//    ui->textEdit_shelf_check_status->append(outputCheckInfo(info));
//}

//void SelfTestDlg::statusUpdated(uint temp1, uint temp2, uint level)
//{
//    qDebug()<<"查询信息更新";
//    m_iTemperature1 = temp1;
//    tempGauge1->setValue(temp1);
//    m_iTemperature2 = temp2;
//    tempGauge2->setValue(temp2);
//    m_iLevel = level;

//    //ui->label_thermometer_left_value->setText(QString::number(m_iTemperature1));
//    //ui->label_thermometer_right_value->setText(QString::number(m_iTemperature2));
//    QString levelStatus;
//    if(m_iLevel==0x70){
//        levelStatus="Water Level Low";
//        ui->label_WaterLow->setStyleSheet("background-color: rgb(204, 65, 37);");
//        ui->label_WaterNormal->setStyleSheet("background-color: rgb(197, 197, 197);");
//        ui->label_WaterHigh->setStyleSheet("background-color: rgb(197, 197, 197);");
//    }
//    else if(m_iLevel==0x71){
//        levelStatus="Water Level Normal";
//        ui->label_WaterLow->setStyleSheet("background-color: rgb(197, 197, 197);");
//        ui->label_WaterNormal->setStyleSheet("background-color: rgb(147, 196, 125);");
//        ui->label_WaterHigh->setStyleSheet("background-color: rgb(197, 197, 197);");
//    }
//    else if(m_iLevel==0x72){
//        levelStatus="Water Level High";
//        ui->label_WaterLow->setStyleSheet("background-color: rgb(197, 197, 197);");
//        ui->label_WaterNormal->setStyleSheet("background-color: rgb(197, 197, 197);");
//        ui->label_WaterHigh->setStyleSheet("background-color: rgb(204, 65, 37);");
//    }

//    if(m_iLevel==0x70&&m_bPumpOpen){
//        if(ui->spinBox_pump_speed->value()>0){
//            ui->spinBox_pump_speed->setValue(0);
//            QString str("Water level is low.\nSet pump speed ");
//            str.append(QString::number(0));
//            emit setPumpSpeed(0);
//        }
//    }
//}

//void SelfTestDlg::showCurrent(uint temp1,uint temp2,uint level)
//{
//    m_iTemperature1 = temp1;
//    tempGauge1->setValue(temp1);
//    m_iTemperature2 = temp2;
//    tempGauge2->setValue(temp2);
//    m_iLevel = level;

//    //ui->label_thermometer_left_value->setText(QString::number(m_iTemperature1));
//    //ui->label_thermometer_right_value->setText(QString::number(m_iTemperature2));
//}

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
            emit showLogin();
            _isShowFromLogin = false;
            DelayClose();
        }
        ui->textEdit_shelf_check_status->append("Self-test complete.");
    }
}

void SelfTestDlg::on_pushButton_quit_clicked()
{
    this->close();
    QApplication* app;
    app->quit();
}

void SelfTestDlg::DelayClose()
{
    delayTimer.setSingleShot(true);
    delayTimer.setInterval(500);
    delayTimer.start();
    connect(&delayTimer,SIGNAL(timeout()),this,SLOT(close()));
}

//刷新自检状态
void SelfTestDlg::on_pushButton_refresh_clicked()
{
    _isShowFromLogin = true;
    bOK=true;
    iSelfTestProgress=0;
    if(!this->isHidden()){
        bOK=true;
        foreach (QLabel *lbl, lblList) {
            lbl->setPixmap(pixUnknown);
        }
        //movieLoad->start();
        this->activateWindow();
    }
    else
    {
        this->show();
    }
    if(m_pMainWin!=NULL){
        m_pMainWin->RefreshSubsystem();
    }
}

//void SelfTestDlg::initAmpParaTable()
//{
//    if (m_pAmpModel == NULL)
//    {
//        m_pAmpModel = new QStandardItemModel();
//    }

//    m_pAmpModel->setColumnCount(12);
//    m_pAmpModel->setHeaderData(0,Qt::Horizontal,QString("通道"));
//    m_pAmpModel->setHeaderData(1,Qt::Horizontal,QString("温度"));
//    m_pAmpModel->setHeaderData(2,Qt::Horizontal,QString("电压"));
//    m_pAmpModel->setHeaderData(3,Qt::Horizontal,QString("通道"));
//    m_pAmpModel->setHeaderData(4,Qt::Horizontal,QString("温度"));
//    m_pAmpModel->setHeaderData(5,Qt::Horizontal,QString("电压"));
//    m_pAmpModel->setHeaderData(6,Qt::Horizontal,QString("通道"));
//    m_pAmpModel->setHeaderData(7,Qt::Horizontal,QString("温度"));
//    m_pAmpModel->setHeaderData(8,Qt::Horizontal,QString("电压"));
//    m_pAmpModel->setHeaderData(9,Qt::Horizontal,QString("通道"));
//    m_pAmpModel->setHeaderData(10,Qt::Horizontal,QString("温度"));
//    m_pAmpModel->setHeaderData(11,Qt::Horizontal,QString("电压"));

//    ui->tableView->setModel(m_pAmpModel);

//    int wide = 58;
//    ui->tableView->setColumnWidth(0,wide);
//    ui->tableView->setColumnWidth(1,wide);
//    ui->tableView->setColumnWidth(2,wide);
//    ui->tableView->setColumnWidth(3,wide);
//    ui->tableView->setColumnWidth(4,wide);
//    ui->tableView->setColumnWidth(5,wide);
//    ui->tableView->setColumnWidth(6,wide);
//    ui->tableView->setColumnWidth(7,wide);
//    ui->tableView->setColumnWidth(8,wide);
//    ui->tableView->setColumnWidth(9,wide);
//    ui->tableView->setColumnWidth(10,wide);
//    ui->tableView->setColumnWidth(11,wide);

//    ui->tableView->verticalHeader()->hide();

//    m_pAmpModel->setRowCount(36);

//    for (int i = 0; i < 36; ++i)
//    {
//        for (int j = 0; j < 4; ++j)
//        {
//            m_pAmpModel->setItem(i,3*j,new QStandardItem(QString::number(4*i+j+1)));
//            m_pAmpModel->setItem(i,3*j+1,new QStandardItem("null"));
//            m_pAmpModel->setItem(i,3*j+2,new QStandardItem("null"));
//        }
//    }

//    emit queryAmpTempature();
//    emit queryAmpVoltage();

//}

//void SelfTestDlg::on_toolButton_refresh_tempature_clicked()
//{
//    //m_pAmpModel->setRowCount(36);
//    emit queryAmpTempature();
//}

//void SelfTestDlg::on_toolButton_refresh_voltage_clicked()
//{
//    //m_pAmpModel->setRowCount(36);
//    emit queryAmpVoltage();
//}

//void SelfTestDlg::refreshAmpParaInTable(DataFlag pFlag, QByteArray& ba)
//{
//    QDataStream in(&ba, QIODevice::ReadOnly);
//    in.setVersion(QDataStream::Qt_5_4);
//    qint64 flag;
//    in >> flag;
//    qint64 size, dFlag;
//    in >> size >> dFlag;
//    double value;
//    int temperature = 0;
//    switch (pFlag)
//    {
//    case POWER_AMPLIFIERS_TEMPATURE:
//         for (int i = 0; i < 36; ++i)
//         {
//             for (int j = 0; j < 4; ++j)
//             {
//                 in >> value;
//                 temperature = m_table->getTemperature((int)value*10);
//                 m_pAmpModel->setItem(i,3*j+1,new QStandardItem(QString::number(temperature)));
//                 // m_pAmpModel->item(i,3*j+1)->setData(QString::number(value));
//             }
//         }

//        break;

//    case POWER_AMPLIFIERS_VOLTAGE:

//        for (int i = 0; i < 36; ++i)
//        {
//            for (int j = 0; j < 4; ++j)
//            {
//                in >> value;
//                m_pAmpModel->setItem(i,3*j+2,new QStandardItem(QString::number(value)));
//                // m_pAmpModel->item(i,3*j+2)->setData(QString::number(value));
//            }
//        }
//        break;

//    default:
//        break;
//    }
//}

void SelfTestDlg::showEvent(QShowEvent *event)
{
    this->activateWindow();
}

//void SelfTestDlg::on_dial_pump_speed_valueChanged(int value)
//{
//    ui->spinBox_pump_speed->setValue(value);
//    QString str("Set pump speed ");
//    str.append(QString::number(value));
//    emit setPumpSpeed(value);
//    ui->textEdit_shelf_check_status->setText(str);
//}

void SelfTestDlg::on_textEdit_shelf_check_status_textChanged()
{
    ui->textEdit_shelf_check_status->moveCursor(QTextCursor::End);
}
