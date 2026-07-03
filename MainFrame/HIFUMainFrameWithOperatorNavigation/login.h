#ifndef LOGIN_H
#define LOGIN_H

#include <QObject>
#include <QString>
#include <QVariant>
#include <QtSql/QSqlDatabase>
#include "tablemodel.h"
#include "newmainwindow.h"

class Login : public QObject
{
    Q_OBJECT

public:
    Login(QObject *parent = 0);
    QSqlDatabase m_db;
    TableModel* m_model_Pt;
    TableModel* m_model_Dr;
    TableModel* m_model_TP;
    NewMainWindow w;
signals:
    void showResult(QVariant success,QVariant isAdmin);
    void showPatientBasic(QVariant id,QVariant fname,QVariant lname,QVariant age,QVariant mobile,QVariant x,QVariant y,QVariant z);
    void showPatientTS(QVariant date,QVariant startT,QVariant finishT,QVariant currFocus,QVariant currDutyOn,QVariant currDutyOff,QVariant currCooling,QVariant isComplete);
    void clearPatientTS();
    void showDoctorBasic(QVariant id,QVariant fname,QVariant lname,QVariant gender,QVariant age,QVariant post,QVariant mobile,QVariant email);
    void showDoctorLogin(QVariant fullName,QVariant userName,QVariant pw);
    void closeNewPt();
    void closeNewDr();
public slots:
    void checkInfo(QString nameToCheck, QString pwToCheck);
    void updatePtSearchList(int index, QString content);
    void updateDrSearchList(int index, QString content);
    void deletePtRow(int rowIndex);
    void deleteDrRow(int rowIndex);
    void addPtRow(QString fname,QString lname,QString age,QString mobile,QString x,QString y,QString z);
    void addDrRow(QString fname,QString lname,QString gender,QString age,QString post,QString isAdmin,QString mobile,QString email);
    void changePtRow(QString id,QString fname,QString lname,QString age,QString mobile,QString x,QString y,QString z);
    void changeDrBasicRow(QString id,QString fname,QString lname,QString gender,QString age,QString post,QString isAdmin,QString mobile,QString email);
    void changeDrLoginRow(QString fullName, QString userName, QString pw);
    void getPatientBasic(int rowIndex);
    void getPatientTreatment(int rowIndex);
    void getDoctorBasic(int rowIndex);
    void getDoctorLogin(int rowIndex);    
};

#endif // LOGIN_H
