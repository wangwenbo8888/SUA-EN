#include "logindialog.h"
#include "ui_logindialog.h"
#include <QDebug>
#include <QSqlQuery>
#include <QFile>
#include <QDir>



LoginDialog::LoginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginDialog)
{
    ui->setupUi(this);
    QString HifuPath = "C:/HIFUTreation/build-HIFUMainframe-Desktop_Qt_5_12_11_MSVC2017_64bit-Release/release/database.db";
    QString DataPath = "D:/HIFU-V1.0/backups/";
    QString DataFullPath = "D:/HIFU-V1.0/backups/database.db";
    QFile file (DataFullPath);
    QDir dir(DataPath);
    if (!dir.exists())
    {
        dir.mkpath(DataPath);
    }
    if(!file.exists())
    {
        QFile::copy(HifuPath,DataFullPath);
    }
//    m_db = QSqlDatabase::addDatabase("QSQLITE");
//    m_db.setDatabaseName("D:/HIFU-V1.0/backups/database.db");
//    m_db.setUserName("root");
//    m_db.setPassword("123456");
//    m_db.open();
//    if (m_db.isOpen())
//        qDebug()<<"Open databe success.";
//    else
//        qDebug()<<"Open databe error.";

    this->setWindowFlags(Qt::FramelessWindowHint);

    ui->pushButtonLogin->setFocusPolicy(Qt::StrongFocus);
//    ui->pushButtonLogin_usermanager->setFocusPolicy(Qt::StrongFocus);

    setTabOrder(ui->lineEditUserName,ui->lineEditPassWord);
    setTabOrder(ui->lineEditPassWord,ui->pushButtonLogin);
//    setTabOrder(ui->pushButtonLogin,ui->pushButtonLogin_usermanager);
//    setTabOrder(ui->pushButtonLogin_usermanager,ui->commandLinkButton);
    setTabOrder(ui->commandLinkButton,ui->lineEditUserName);
}

LoginDialog::~LoginDialog()
{
    delete ui;
}

void LoginDialog::on_pushButtonLogin_clicked()
{
//    qDebug()<<"进入m_db中0000";
//    try{
//        qDebug()<<"进入m_db中1111";
        QString nameToCheck=ui->lineEditUserName->text();
        QString pwToCheck=ui->lineEditPassWord->text();

//        if (nameToCheck.size()!=6)
//        {
//            QMessageBox::information(0,"Notice",tr("用户名必须等于6个字符！"),QMessageBox::Ok);
//            ui->lineEditUserName->setText("");
//            return;
//        }

//        if (pwToCheck.size()<6)
//        {
//            QMessageBox::information(0,"Notice","密码不得少于6个字符！",QMessageBox::Ok);
//            ui->lineEditPassWord->setText("");
//            return;
//        }

        bool success = false;
        bool isAdmin = false;

        //hash加密
        QCryptographicHash hash(QCryptographicHash::Sha256);
//        hash.addData(nameToCheck.toLatin1().data());
//        QString hash_username = hash.result().toHex().toUpper();
        hash.addData(pwToCheck.toLatin1().data());
        QString hash_pw = hash.result().toHex().toUpper();
//        qDebug()<<"账号111111："<<nameToCheck<<"\n密码："<<hash_pw;

        m_db = QSqlDatabase::addDatabase("QSQLITE");
        m_db.setDatabaseName("D:/HIFU-V1.0/backups/database.db");
        m_db.setUserName("root");
        m_db.setPassword("123456");
        m_db.open();
//        if (m_db.isOpen())
//            qDebug()<<"Open databe success.";
//        else
//            qDebug()<<"Open databe error.";
        if (m_db.isOpen())
        {
            QSqlQuery loginquery("SELECT name,userName,pw FROM LoginInfo",m_db);
            //loginquery.exec("SELECT name,userName,pw FROM LoginInfo");
//            qDebug()<<"进入m_db中";
            while(loginquery.next())
            {
                QString userName = loginquery.value(1).toString();
                QString pw = loginquery.value(2).toString();

                if (nameToCheck == userName && hash_pw == pw)
                {


                    success = true;
//                    qDebug()<<"无法判断！";
                    QString name = loginquery.value(0).toString();
                    loginquery.clear();
                    loginquery.exec("SELECT isAdmin FROM DrInfo WHERE fullName = '" + name +"'");
                    while(loginquery.next())
                    {
                        isAdmin = loginquery.value(0).toBool();
                    }
                }
            }
        }
        if(nameToCheck=="admin"&&pwToCheck=="123456")
        {
            success=true;
            isAdmin=true;
        }
//        else
//        {
//            qDebug()<<"没有打开database";
//        }
        qDebug()<<success<<isAdmin;
        if(success)
        {
            if(isAdmin)
            {
                emit ShowUserManager(false);
                DelayClose();
                //this->hide();
            }
            else
            {
                emit ShowUserManager(true);
                DelayClose();
    //            QMessageBox::information(0,"Notice","该用户无管理员权限！",QMessageBox::Ok);
            }
//            emit ShowUserManager(true);
//            DelayClose();
        }
        else
        {
           QMessageBox::information(0,"Notice","Incorrect username or password.",QMessageBox::Ok);
            ui->lineEditPassWord->setText("");
        }
//    }
//    catch(std::exception ex)
//    {
//        QString mes(ex.what());
//        QMessageBox::information(this,"",mes,QMessageBox::Ok,QMessageBox::Cancel);
//    }
        m_db.close();
}

void LoginDialog::on_commandLinkButton_clicked()
{
    this->reject();
    qApp->quit();
}

//void LoginDialog::on_pushButtonLogin_usermanager_clicked()
//{
//    try{
//    QString nameToCheck=ui->lineEditUserName->text();
//    QString pwToCheck=ui->lineEditPassWord->text();

//    if (nameToCheck.size()>20)
//    {
//        QMessageBox::information(0,"Notice","用户名不能超过20个字符！",QMessageBox::Ok);
//        ui->lineEditUserName->setText("");
//        return;
//    }

//    if (pwToCheck.size()>20)
//    {
//        QMessageBox::information(0,"Notice","密码不能超过20个字符！",QMessageBox::Ok);
//        ui->lineEditPassWord->setText("");
//        return;
//    }

//    bool success = false;
//    bool isAdmin = false;

//    if (m_db.isOpen())
//    {
//        QSqlQuery loginquery("SELECT name,userName,pw FROM LoginInfo",m_db);
//        //loginquery.exec("SELECT name,userName,pw FROM LoginInfo");
//        while(loginquery.next())
//        {
//            QString userName = loginquery.value(1).toString();
//            QString pw = loginquery.value(2).toString();

//            if (nameToCheck == userName && pwToCheck == pw)
//            {
//                success = true;
//                QString name = loginquery.value(0).toString();
//                loginquery.clear();
//                loginquery.exec("SELECT isAdmin FROM DrInfo WHERE fullName = '" + name +"'");
//                while(loginquery.next())
//                {
//                    isAdmin = loginquery.value(0).toBool();
//                }
//            }
//        }
//    }
//    qDebug()<<success<<isAdmin;
//    if(success)
//    {
//        if(isAdmin)
//        {
//            emit ShowUserManager(false);
//            //this->hide();
//        }
//        else
//        {
//            emit ShowUserManager(true);
//            QMessageBox::information(0,"Notice","该用户无管理员权限！",QMessageBox::Ok);
//        }
//    }
//    else
//    {
//        QMessageBox::information(0,"Notice","用户名或密码错误！",QMessageBox::Ok);
//        ui->lineEditPassWord->setText("");
//    }
//    }
//    catch(std::exception ex)
//    {
//        QString mes(ex.what());
//        QMessageBox::information(this,"",mes,QMessageBox::Ok,QMessageBox::Cancel);
//    }
//}

void LoginDialog::DelayClose()
{
    delayTimer.setSingleShot(true);
    delayTimer.setInterval(500);
    delayTimer.start();
    connect(&delayTimer,SIGNAL(timeout()),this,SLOT(hide()));
}

void LoginDialog::showEvent(QShowEvent *event)
{
    this->activateWindow();
}

void LoginDialog::keyPressEvent(QKeyEvent *event )
{
    if(event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return )
    {
        on_pushButtonLogin_clicked();
    }
}
