#ifndef USERMANAGER_H
#define USERMANAGER_H

#include <QDialog>
#include <QtSql/QSqlDatabase>
#include <QSqlTableModel>
#include "usermanager_global.h"
#include <QTimer>
#include "qevent.h"

#if _MSC_VER >1600
#pragma execution_character_set("utf-8")
#endif

namespace Ui {
class UserManager;
}

class UserManager : public QDialog
{
    Q_OBJECT

public:
    explicit UserManager(QWidget *parent = 0);
    ~UserManager();

    QSqlDatabase m_db;

signals:
    void ShowMainWindow();
    void SendPatientInfo(QString id,QString fname,
                         QString age,QString mobile,
                         QString x,QString y,QString z,int PatientOrientation);

protected:
    bool eventFilter(QObject *obj, QEvent *event);
    void showEvent(QShowEvent *event);

private slots:
    void on_commandLinkButton_Patient_clicked();
    void on_commandLinkButton_Doctor_clicked();
    void on_radioButton_SID_clicked();
    void on_radioButton_SFirstName_clicked();
    void on_radioButton_SAge_clicked();
    void on_radioButton_SMobile_clicked();
    void on_toolButton_Search_clicked();
    void on_tableView_clicked(const QModelIndex &index);
    void on_commandLinkButton_add_clicked();
    void on_toolButton_UndoSearch_clicked();

    void on_pushButton_PaEdit_clicked();

    void on_pushButton_DrEdit_clicked();

    void on_pushButton_DrCommit_clicked();

    void on_pushButton_PaCommit_clicked();

    void on_commandLinkButton_delete_clicked();

    void on_commandLinkButton_quit_clicked();

    void on_pushButton_DrLogInfoEdit_clicked();

    void on_pushButton_PaTreatment_clicked();

    void UpdateModel();

private:
    class CustomSqlTableModel : public QSqlTableModel
    {
    public:
        using QSqlTableModel::QSqlTableModel;

        QString selectStatement() const override
        {
            QString query = QSqlTableModel::selectStatement();
            query += " LIMIT 1000"; // Add LIMIT 子句
            return query;
        }
    };

    Ui::UserManager *ui;
    void SetTableViewHeader(int dataType);
    QString sSearchField;
    QSqlTableModel *model;
    CustomSqlTableModel *model1;

    PatientDataRow m_PaDataRow;
    DoctorDataRow m_DrDataRow;
    DrLoginDataRow m_DrLoginDataRow;

    QueryDataType m_QDataType;

    void DelayClose();
    QTimer delayTimer;

    int m_iPaOrientation;//Patients朝向，左侧为0，右侧为1

    void keyPressEvent(QKeyEvent *event);

    QString PID;
    void updateInfoFile(QString name, QByteArray& log);
    void updateInfo(const QString &msg);

public slots:
    void ShowUserManager(bool isPatient);
    void Newtreatment(int ID,QString PID,QString SID,QString Name,QString Gender,QString Birth,
                      int Age,QString Mobile,int Depth,int x,int y,int z,QString Address,QString Doctor);

protected:
    void SetPatientCtlData(PatientDataRow data);
    void SetPatientCtlEditable(bool editable);
    void SetDoctorCtlData(DoctorDataRow data);
    void SetDoctorCtlEditable(bool editable);
};

#endif // USERMANAGER_H
