#ifndef ADDUSER_H
#define ADDUSER_H

#include <QDialog>
#include "usermanager_global.h"
#include "usermanager.h"
#include <QSqlDatabase>
#include <QStandardItemModel>
#include <QVector>
#include <QTableView>
#include <QCryptographicHash>
#if _MSC_VER >1600
#pragma execution_character_set("utf-8")
#endif



namespace Ui {
class AddUser;
}

class AddUser : public QDialog
{
    Q_OBJECT

public:
    explicit AddUser(QWidget *parent = 0);
    ~AddUser();
    void SetDefaultDataMode(QueryDataType data);

private slots:
    void on_pushButton_Cancel_clicked();

    void on_pushButton_PaCommit_clicked();

    void on_pushButton_DrCommit_clicked();

//    void automatic_connect();

    void SetTableViewHeader();

    void on_pushButton_findname_clicked();

    void on_tableView_clicked(const QModelIndex &index);

//    void close_mysql();

signals:
    void setdata(int ID,QString PID,QString SID,QString Name,QString Gender,QString Birth,
                 int Age,QString Mobile,int Depth,int x,int y,int z,QString Address,QString Doctor);
private:
    Ui::AddUser *ui;

//    QCryptographicHash hash256(QCryptographicHash::Sha256);
//    QSqlDatabase mysql = QSqlDatabase::addDatabase("QMYSQL");
    QVector<QString> id;
    QVector<QString> pid;
    QVector<QString> sid;
    QVector<QString> name;
    QVector<QString> age;
    QVector<QString> telephone;
    QVector<QString> birthday;
    QVector<QString> address;
    QVector<QString> deep;
    QVector<QString> sizeX;
    QVector<QString> sizeY;
    QVector<QString> sizeZ;
    QVector<QString> doctor;

    QStandardItemModel *model = new QStandardItemModel();
    QTableView * tableview = new QTableView();

    const QString Baseline = "2023-1-1";
    quint64 idIndex;
    quint64 idStamp;
    quint32 project;
    quint32 machine;
    quint32 object;
    QString Pname,PID,SID;
    QString birth;

    quint64 MakeId(quint32 proj_id, quint32 mach_id, quint32 obj_id, quint32 squ_id);
    QHash<QString, QVariant> SplitId(quint64 mId);
    quint32 mIDIndex();
    void mIDInit(quint32 proj_id, quint32 mach_id);
    QVariant uuid(QVariant param);

    void updateInfoFile(QString name, QByteArray& log);
    void updateInfo(const QString &msg);
};

#endif // ADDUSER_H
