#include "logininfoedit.h"
#include "ui_logininfoedit.h"
#include <QDebug>
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QtSql/QSqlDatabase>
#include <QCryptographicHash>
#if _MSC_VER >1600
#pragma execution_character_set("utf-8")
#endif

LoginInfoEdit::LoginInfoEdit(QWidget *parent,int id) :
    QDialog(parent),
    ui(new Ui::LoginInfoEdit)
{
    ui->setupUi(this);
    m_DrLoginData.DrID=-1;
    GetLoginInfo(id);
}

LoginInfoEdit::~LoginInfoEdit()
{
    delete ui;
}

void LoginInfoEdit::on_buttonBox_accepted()
{
    if(m_DrLoginData.DrID==-1)
    {
        QMessageBox::critical(0, QObject::tr("Notice"),
                                          "无法修改，数据为空！");
        return;
    }
    bool bUserEmpty=ui->lineEdit_UserName->text().isEmpty();
    bool bPwdEmpty=ui->lineEdit_NewPwd->text().isEmpty();
    if(bUserEmpty)
    {
        QMessageBox::critical(0, QObject::tr("Notice"),
                                          "登录用户名不能为空！");
        return;
    }

    //hash加密
    QCryptographicHash hash256(QCryptographicHash::Sha256);
    QString pw = ui->lineEdit_OldPwd->text();
    hash256.addData(pw.toLatin1().data());
    QString hash_pwvalue = hash256.result().toHex().toUpper();

    qDebug()<<"oldhash============"<<hash_pwvalue;

    if(hash_pwvalue!=m_DrLoginData.pw)
    {
        QMessageBox::critical(0, QObject::tr("Notice"),
                                          "原密码输入错误！");
        return;
    }
    QString strSQl;
    if(bPwdEmpty)
    {//仅修改用户名
        strSQl=QString("UPDATE LoginInfo SET userName = '%1' WHERE DrID = %2")
                .arg(ui->lineEdit_UserName->text())
                .arg(m_DrLoginData.DrID);
    }
    else
    {
        if(ui->lineEdit_NewPwd->text()!=ui->lineEdit_NewPwd2->text())
        {
//            QMessageBox::critical(0, QObject::tr("Notice"),"两次输入新密码不匹配！");
            ui->lineEdit_NewPwd2->setText("");
            return;
        }

        //hash加密
        QCryptographicHash hash2567(QCryptographicHash::Sha256);
        QString newPw = ui->lineEdit_NewPwd->text();
        hash2567.addData(newPw.toLatin1().data());
        QString hash_new_pwvalue = hash2567.result().toHex().toUpper();

        strSQl=QString("UPDATE LoginInfo SET userName = '%1',pw='%2' WHERE DrID = %3")
                .arg(ui->lineEdit_UserName->text())
                .arg(hash_new_pwvalue)
                .arg(m_DrLoginData.DrID);
        qDebug()<<"hash============"<<hash_new_pwvalue;

    }
    qDebug()<<strSQl;
    QSqlQuery sQuery;
    if(sQuery.exec(strSQl))
    {
        QMessageBox::information(0, QObject::tr("Notice"),"Updated successfully.");
        this->accept();
    }
    else
    {
        QMessageBox::critical(0, QObject::tr("Notice"),
                                          "修改失败！\n错误信息："+sQuery.lastError().text());
        this->reject();
    }
}

void LoginInfoEdit::GetLoginInfo(int id)
{
    QSqlDatabase db = QSqlDatabase::database();
    qDebug()<<db.isOpen();
    QString strSQl=QString("SELECT name,userName,pw,DrID FROM LoginInfo WHERE DrID = %1").arg(id);
    qDebug()<<strSQl;
    QSqlQuery sQuery;
    if(!sQuery.exec(strSQl))
    {
        QMessageBox::critical(0, QObject::tr("Notice"),
                                          "查询数据库出错！\n错误信息："+sQuery.lastError().text());
        return;
    }
    qDebug()<<sQuery.size();
    while (sQuery.next()) {
            m_DrLoginData.name = sQuery.value(0).toString();
            m_DrLoginData.userName = sQuery.value(1).toString();
            m_DrLoginData.pw = sQuery.value(2).toString();
            m_DrLoginData.DrID = sQuery.value(3).toInt();
        }
    ui->lineEdit_UserName->setText(m_DrLoginData.userName);
}

void LoginInfoEdit::on_buttonBox_rejected()
{
    this->reject();
}
