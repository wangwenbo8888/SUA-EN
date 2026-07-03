#include "login.h"
#include <QDebug>
#include <QtSql/QSqlQuery>
#include <QMessageBox>
#include <QSqlRecord>
//#include <QQmlContext>
//#include <QQuickItem>
//#include <QQuickView>
#include <QSqlError>

Login::Login(QObject *parent):QObject(parent)
{
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName("database.db");
    m_db.setUserName("root");
    m_db.setPassword("123456");
    m_db.open();
    if (m_db.isOpen())
        qDebug()<<"open";

    m_model_Pt = new TableModel;
    m_model_Pt->setTable("PatientInfo");
    m_model_Pt->generateRoleNames();
    m_model_Pt->select();

    m_model_Dr = new TableModel;
    m_model_Dr->setTable("DrInfo");
    m_model_Dr->generateRoleNames();
    m_model_Dr->select();

    m_model_TP = new TableModel;
    m_model_TP->setTable("PatientTP");
    m_model_TP->generateRoleNames();
    m_model_TP->select();
}

void Login::checkInfo(QString nameToCheck, QString pwToCheck)
{
    bool success = false;
    bool isAdmin = false;

    if (m_db.open())
    {
        QSqlQuery query;
        query.exec("SELECT name,userName,pw FROM LoginInfo");
        while(query.next())
        {
            QString userName = query.value(1).toString();
            QString pw = query.value(2).toString();

            if (nameToCheck == userName && pwToCheck == pw)
            {
                success = true;
                QString name = query.value(0).toString();
                query.clear();
                query.exec("SELECT isAdmin FROM DrInfo WHERE fullName = '" + name +"'");
                while(query.next())
                {
                    isAdmin = query.value(0).toBool();
                }
            }
        }
    }

    emit showResult(success,isAdmin);
}

void Login::updatePtSearchList(int index, QString content)
{
    if (content == "")
    {
        m_model_Pt->setTable("PatientInfo");
        m_model_Pt->generateRoleNames();
        m_model_Pt->select();
    }
    else
    {
        QString searchCondition;
        switch(index){
            case 1:
                searchCondition = "id";
                break;
            case 2:
                searchCondition = "firstName";
                break;
            case 3:
                searchCondition = "lastName";
                break;
            case 4:
                searchCondition = "age";
                break;
            case 5:
                searchCondition = "mobile";
                break;
        }

        if (m_db.open())
            m_model_Pt->setFilter(searchCondition + " LIKE '" + content + "%'");
    }
}

void Login::deletePtRow(int rowIndex)
{
    int ok = QMessageBox::warning(NULL,"Warning","Are you sure to delete the selected record?",QMessageBox::Yes|QMessageBox::No,QMessageBox::No);
    if (ok == QMessageBox::Yes)
    {
        m_model_Pt->removeRow(rowIndex);
        m_model_Pt->submitAll();
        m_model_Pt->select();
    }
}

void Login::updateDrSearchList(int index, QString content)
{
    if (content == "")
    {
        m_model_Dr->setTable("DrInfo");
        m_model_Dr->generateRoleNames();
        m_model_Dr->select();
    }
    else
    {
        QString searchCondition;
        switch(index){
            case 1:
                searchCondition = "id";
                break;
            case 2:
                searchCondition = "firstName";
                break;
            case 3:
                searchCondition = "lastName";
                break;
            case 4:
                searchCondition = "gender";
                break;
            case 5:
                searchCondition = "age";
                break;
            case 6:
                searchCondition = "post";
                break;
//            case 7:
//                searchCondition = "mobile";
//                break;
//            case 8:
//                searchCondition = "email";
//                break;
        }

        if (m_db.open())
            m_model_Dr->setFilter(searchCondition + " LIKE '" + content + "%'");
    }
}

void Login::deleteDrRow(int rowIndex)
{
    int ok = QMessageBox::warning(NULL,"Warning","Are you sure to delete the selected record?",QMessageBox::Yes|QMessageBox::No,QMessageBox::No);
    if (ok == QMessageBox::Yes)
    {
        m_model_Dr->removeRow(rowIndex);
        m_model_Dr->submitAll();
        m_model_Dr->select();
    }
}

void Login::addPtRow( QString fname, QString lname, QString age, QString mobile, QString x, QString y, QString z)
{
    if (m_db.open())
    {
        int row = m_model_Pt->rowCount();
        QSqlRecord lastRecord = m_model_Pt->record(row - 1);
        int lastID = lastRecord.value(0).toInt();

        m_model_Pt->insertRow(row);
        m_model_Pt->setData(m_model_Pt->index(row,0),lastID + 1);
        m_model_Pt->setData(m_model_Pt->index(row,1),fname);
        m_model_Pt->setData(m_model_Pt->index(row,2),lname);
        m_model_Pt->setData(m_model_Pt->index(row,3),age);
        m_model_Pt->setData(m_model_Pt->index(row,4),mobile);

        QSqlQuery query;
        query.prepare("INSERT INTO PatientInfo(id,firstName,lastName,age,mobile,size_x,size_y,size_z)"
                      "VALUES(:id,:firstName,:lastName,:age,:mobile,:size_x,:size_y,:size_z)");
        query.bindValue(":id",lastID + 1);
        query.bindValue(":firstName",fname);
        query.bindValue(":lastName",lname);
        query.bindValue(":age",age.toInt());
        query.bindValue(":mobile",mobile);
        query.bindValue(":size_x",x.toDouble());
        query.bindValue(":size_y",y.toDouble());
        query.bindValue(":size_z",z.toDouble());
        bool ok = query.exec();
        qDebug() << ok;
        if(ok)
        {
            QMessageBox::information(NULL,"Information","New patient has been addeed successfully.",QMessageBox::Ok);
            emit closeNewPt();
        }
        else
        {
            QMessageBox::information(NULL,"Information","Operation failed. Try again.",QMessageBox::Ok);
            qDebug() << query.lastError();
        }
    }
}

void Login::addDrRow(QString fname, QString lname, QString gender, QString age, QString post, QString isAdmin, QString mobile, QString email)
{
    if (m_db.open())
    {
        int row = m_model_Dr->rowCount();
        QSqlRecord lastRecord = m_model_Dr->record(row - 1);
        int lastID = lastRecord.value(0).toInt();

        m_model_Dr->insertRow(row);
        m_model_Dr->setData(m_model_Dr->index(row,0),lastID + 1);
        m_model_Dr->setData(m_model_Dr->index(row,1),fname);
        m_model_Dr->setData(m_model_Dr->index(row,2),lname);
        m_model_Dr->setData(m_model_Dr->index(row,4),gender);
        m_model_Dr->setData(m_model_Dr->index(row,5),age);
        m_model_Dr->setData(m_model_Dr->index(row,6),post);
        m_model_Dr->setData(m_model_Dr->index(row,7),isAdmin);
        m_model_Dr->setData(m_model_Dr->index(row,8),mobile);
        m_model_Dr->setData(m_model_Dr->index(row,9),email);

        QSqlQuery query;
        query.prepare("INSERT INTO DrInfo(id,firstName,lastName,fullName,gender,age,post,isAdmin,mobile,email)"
                      "VALUES(:id,:firstName,:lastName,:fullName,:gender,:age,:post,:isAdmin,:mobile,:email)");
        query.bindValue(":id",lastID + 1);
        query.bindValue(":firstName",fname);
        query.bindValue(":lastName",lname);
        query.bindValue(":fullName",fname + " " + lname);
        query.bindValue(":gender",gender);
        query.bindValue(":age",age.toInt());
        query.bindValue(":post",post);
        query.bindValue(":isAdmin",isAdmin);
        query.bindValue(":mobile",mobile);
        query.bindValue(":email",email);
        bool ok = query.exec();
        qDebug() << ok;
        if(ok)
        {
            QMessageBox::information(NULL,"Information","New Doctor has been addeed successfully.",QMessageBox::Ok);
            emit closeNewDr();
        }
        else
        {
            QMessageBox::information(NULL,"Information","Operation failed. Try again.",QMessageBox::Ok);
            qDebug() << query.lastError();
        }
    }
}

void Login::changePtRow(QString id,QString fname, QString lname, QString age, QString mobile, QString x, QString y, QString z)
{
    QSqlQuery query;
    bool ok = query.exec("UPDATE PatientInfo SET firstName = '" + fname + "',lastName = '" + lname +
                         "',age = " + age + ",mobile = '" + mobile + "',size_x = " + x +
                         ",size_y = " + y + ",size_z = " + z + " WHERE id = " + id);
    if(ok)
    {
        QMessageBox::information(NULL,"Information","Changed information committed.",QMessageBox::Ok);
        m_model_Pt->select();
    }
    else
    {
        QMessageBox::information(NULL,"Information","Operation failed. Try again.",QMessageBox::Ok);
        qDebug() << query.lastError();
    }
}

void Login::changeDrBasicRow(QString id, QString fname, QString lname, QString gender, QString age, QString post, QString isAdmin, QString mobile, QString email)
{
    QSqlQuery query;
    bool ok;
    if(age.isEmpty())
        ok = query.exec("UPDATE DrInfo SET firstName = '" + fname + "',lastName = '" + lname +
                             "',fullName = '" + fname + " " + lname +
                             "',gender = '" + gender + "',age = '" + age + "',post = '" + post +
                             "',isAdmin = '" + isAdmin + "',mobile = '" + mobile +
                             "',email = '" + email + "' WHERE id = " + id);
    else
        ok = query.exec("UPDATE DrInfo SET firstName = '" + fname + "',lastName = '" + lname +
                         "',fullName = '" + fname + " " + lname +
                         "',gender = '" + gender + "',age = " + age + ",post = '" + post +
                         "',isAdmin = '" + isAdmin + "',mobile = '" + mobile +
                         "',email = '" + email + "' WHERE id = " + id);
    if(ok)
    {
        QMessageBox::information(NULL,"Information","Changed information committed.",QMessageBox::Ok);
        emit showDoctorBasic(id,fname,lname,gender,age,post,mobile,email);
        m_model_Dr->select();
    }
    else
    {
        QMessageBox::information(NULL,"Information","Operation failed. Try again.",QMessageBox::Ok);
        qDebug() << query.lastError();
    }
}

void Login::changeDrLoginRow(QString fullName,QString userName, QString pw)
{
    QSqlQuery query;
    bool ok = query.exec("UPDATE LoginInfo SET userName = '" + userName + "',pw = '" + pw + "' WHERE name = '" + fullName + "'");
    if(ok)
    {
        QMessageBox::information(NULL,"Information","Changed information committed.",QMessageBox::Ok);
        emit showDoctorLogin(fullName,userName,pw);
    }
    else
    {
        QMessageBox::information(NULL,"Information","Operation failed. Try again.",QMessageBox::Ok);
        qDebug() << query.lastError();
    }
}

void Login::getPatientBasic(int rowIndex)
{
    QSqlRecord patientBasicRecord;
    QVariant id,fname,lname,age,mobile,x,y,z;

    if(rowIndex >= 0)
    {
        patientBasicRecord = m_model_Pt->record(rowIndex);
        id = patientBasicRecord.value(0);
        fname = patientBasicRecord.value(1);
        lname = patientBasicRecord.value(2);
        age = patientBasicRecord.value(3);
        mobile = patientBasicRecord.value(4);
        x = patientBasicRecord.value(5);
        y = patientBasicRecord.value(6);
        z = patientBasicRecord.value(7);

        QString ID = id.toString();
        m_model_TP->setFilter("patientID = " + ID);

        emit showPatientBasic(id,fname,lname,age,mobile,x,y,z);
    }
}

void Login::getPatientTreatment(int rowIndex)
{
    QSqlRecord patientBasicRecord;
    patientBasicRecord = m_model_Pt->record(rowIndex);
    QString id = patientBasicRecord.value(0).toString();
    QVariant date,startT,finishT,currFocus,currDutyOn,currDutyOff,currCooling,isComplete;

    if(rowIndex >= 0)
    {
        QSqlQuery query;
        query.exec("SELECT date,startTime,finishTime,currentFocusNo,currentDutyOn,currentDutyOff,"
                   "currentCooling,isComplete FROM TreatSession WHERE patientID = " + id);
        while(query.next())
        {
            date = query.value(0);
            startT = query.value(1);
            finishT = query.value(2);
            currFocus = query.value(3);
            currDutyOn = query.value(4);
            currDutyOff = query.value(5);
            currCooling = query.value(6);
            isComplete = query.value(7);

            emit showPatientTS(date,startT,finishT,currFocus,currDutyOn,currDutyOff,currCooling,isComplete);
            return;
        }
        emit clearPatientTS();
    }
}

void Login::getDoctorBasic(int rowIndex)
{
    QSqlRecord doctorBasicRecord;
    QVariant id,fname,lname,gender,age,post,mobile,email;

    if(rowIndex >= 0)
    {
        doctorBasicRecord = m_model_Dr->record(rowIndex);
        id = doctorBasicRecord.value(0);
        fname = doctorBasicRecord.value(1);
        lname = doctorBasicRecord.value(2);
        gender = doctorBasicRecord.value(4);
        age = doctorBasicRecord.value(5);
        post = doctorBasicRecord.value(6);
        mobile = doctorBasicRecord.value(8);
        email = doctorBasicRecord.value(9);

        emit showDoctorBasic(id,fname,lname,gender,age,post,mobile,email);
    }
}

void Login::getDoctorLogin(int rowIndex)
{
    QSqlRecord doctorLoginRecord;
    QVariant fullName,userName,pw;

    if(rowIndex >= 0)
    {
        doctorLoginRecord = m_model_Dr->record(rowIndex);
        fullName = doctorLoginRecord.value(3);
        QSqlQuery query;
        query.exec("SELECT userName,pw FROM LoginInfo WHERE name = '" + fullName.toString() + "'");
        while(query.next())
        {
            userName = query.value(0);
            pw = query.value(1);
        }
        emit showDoctorLogin(fullName,userName,pw);
    }
}
