#include "selftestdlg.h"
#include "ui_selftestdlg.h"
#include "sampletotemperature.h"
#include <QMovie>
#include <QStandardItemModel>
#if _MSC_VER >1600
#pragma execution_character_set("utf-8")
#endif

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

    timer=new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(timeoutslot()));

    setUserModel();
    ui->pushButton_refresh->setEnabled(false);
    ui->pushButton_refresh->setStyleSheet(NOT_WORK);
    ui->pushButton_quit->setEnabled(false);
    ui->pushButton_quit->setStyleSheet(NOT_WORK);
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
//    if (tempGauge1!=NULL)
//    {
//        delete tempGauge1;
//        tempGauge1 = NULL;
//    }
//    if (tempGauge2!=NULL)
//    {
//        delete tempGauge2;
//        tempGauge2 = NULL;
//    }
//    if (levelQueryTimer!=NULL)
//    {
//        delete levelQueryTimer;
//        levelQueryTimer = NULL;
//    }

//    if (m_pAmpModel!=NULL)
//    {
//        delete m_pAmpModel;
//        m_pAmpModel = NULL;
//    }

//    if (m_table!=NULL)
//    {
//        delete m_table;
//        m_table = NULL;
//    }

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

void SelfTestDlg::SetMainWindow(NewMainWindow* main)
{
    m_pMainWin=main;
    if(m_pMainWin==NULL)
        return;
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
    if(this->isHidden())
    {
        this->show();
    }
    else
    {
        bOK=true;
        foreach (QLabel *lbl, lblList) {
            lbl->setPixmap(pixUnknown);
        }
        //movieLoad->start();
        this->activateWindow();
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
    //如果是RefreshStatus则执行
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
//    if(sys==ipower && !status){
//        if(ischecknetwork){
//            ui->textEdit_shelf_check_status->append("power amp电压异常\nNotice：检查PCpower amp是否正常工作");
//            lblList[inetwork]->setPixmap(pixNO);
//            selfcheckfailed=true;
//        }else{
//            return;
//        }
//    }else{
        SubSysStatus[sys]=status;

//        if(sys==inetwork){
//            ischecknetwork = true;
//        }
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
//    }
}

void SelfTestDlg::timeoutslot()
{
    // Query，RefreshCirculationStatus
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
    if (value==100&&_isShowFromLogin/*&&!selfcheckfailed*/)
    {
        if(bOK)
        {
            emit showLogin();
            _isShowFromLogin = false;
            DelayClose();
        }
        ui->pushButton_refresh->setEnabled(true);
        ui->pushButton_refresh->setStyleSheet(SELECTED);
        ui->pushButton_quit->setStyleSheet(SELECTED);
        ui->pushButton_quit->setEnabled(true);
        ui->textEdit_shelf_check_status->append("Self-test complete.");
    }

	//emit showLogin();
	//_isShowFromLogin = false;
	//DelayClose();
}

void SelfTestDlg::on_pushButton_quit_clicked()
{
    QCoreApplication::quit();
    this->close();
}

void SelfTestDlg::DelayClose()
{
    delayTimer.setSingleShot(true);
    delayTimer.setInterval(500);
    delayTimer.start();
    connect(&delayTimer,SIGNAL(timeout()),this,SLOT(close()));
}

//Refresh自检Status
void SelfTestDlg::on_pushButton_refresh_clicked()
{
    _isShowFromLogin = true;
    iSelfTestProgress=0;
    if(!this->isHidden()){
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

void SelfTestDlg::showEvent(QShowEvent *event)
{
    this->activateWindow();
}

void SelfTestDlg::on_textEdit_shelf_check_status_textChanged()
{
    ui->textEdit_shelf_check_status->moveCursor(QTextCursor::End);
}

void SelfTestDlg::keyPressEvent(QKeyEvent *event)
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
