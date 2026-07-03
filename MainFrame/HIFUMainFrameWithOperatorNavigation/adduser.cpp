#include "adduser.h"
#include "ui_adduser.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlDatabase>
#include <QSqlRecord>
#include <QDebug>
#include <QMessageBox>
#include <QStandardItemModel>
#include <QTableView>
#include <QVector>
#include <QCryptographicHash>
#include <QDate>
#include <QDir>
#if _MSC_VER >1600
#pragma execution_character_set("utf-8")
#endif

AddUser::AddUser(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddUser)
{
    ui->setupUi(this);

//    automatic_connect();
    //设置tableview表格格式
    ui->tableView->verticalHeader()->setDefaultAlignment(Qt::AlignCenter);
    ui->tableView->horizontalHeader()
            ->setStyleSheet("QHeaderView::section {color: black;font: 75 12pt \"微软雅黑\";border: 1px solid #6c6c6c;}");
    ui->tableView->verticalHeader()
            ->setStyleSheet("QHeaderView::section {color: black;font: 75 10pt \"微软雅黑\";border: 1px solid #6c6c6c;}");
    this->ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);  //单击选择一行
    this->ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection); //设置只能选择一行，不能多行选中
    this->ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);   //设置每行内容不可更改
    ui->tableView->setModel(model);
    SetTableViewHeader();


    ui->tableView->setVisible(false);
    ui->lineEdit_findname->setVisible(false);
    ui->pushButton_findname->setVisible(false);
    ui->pushButton_PaCommit1->setVisible(false);
    ui->lineEdit_PaName1->setVisible(false);

    ui->lineEdit_PaDepth->setValidator(new QIntValidator(0,50,this));
    ui->lineEdit_PaSizeX->setValidator(new QIntValidator(0,105,this));
    ui->lineEdit_PaSizeY->setValidator(new QIntValidator(0,105,this));
    ui->lineEdit_PaSizeZ->setValidator(new QIntValidator(0,105,this));



    ui->tabWidget->setFocusPolicy(Qt::NoFocus);
    ui->tab_Doctor->setFocusPolicy(Qt::NoFocus);
    ui->tab_Patient->setFocusPolicy(Qt::NoFocus);

    ui->comboBox_PaGender->setEnabled(false);

    setTabOrder(ui->lineEdit_PaName,ui->comboBox_PaGender);
    setTabOrder(ui->comboBox_PaGender,ui->lineEdit_PaAge);
    setTabOrder(ui->lineEdit_PaAge,ui->lineEdit_PaMobile);
    setTabOrder(ui->lineEdit_PaMobile,ui->lineEdit_PaDepth);
    setTabOrder(ui->lineEdit_PaDepth,ui->lineEdit_PaSizeX);
    setTabOrder(ui->lineEdit_PaSizeX,ui->lineEdit_PaSizeY);
    setTabOrder(ui->lineEdit_PaSizeY,ui->lineEdit_PaSizeZ);
    setTabOrder(ui->lineEdit_PaSizeZ,ui->lineEdit_PaAddress);
    setTabOrder(ui->lineEdit_PaAddress,ui->pushButton_PaCommit);
    setTabOrder(ui->pushButton_PaCommit,ui->pushButton_Cancel);

    setTabOrder(ui->lineEdit_DrLastName,ui->lineEdit_DrFirstName);
    setTabOrder(ui->lineEdit_DrFirstName,ui->lineEdit_DrFullName);
    setTabOrder(ui->lineEdit_DrFullName,ui->comboBox_DrGender);
    setTabOrder(ui->comboBox_DrGender,ui->lineEdit_DrAge);
    setTabOrder(ui->lineEdit_DrAge,ui->lineEdit_DrPost);
    setTabOrder(ui->lineEdit_DrPost,ui->checkBox_DrIsAdmin);
    setTabOrder(ui->checkBox_DrIsAdmin,ui->lineEdit_DrMobile);
    setTabOrder(ui->lineEdit_DrMobile,ui->lineEdit_DrEmail);
    setTabOrder(ui->lineEdit_DrEmail,ui->lineEdit_DrLoginUserName);
    setTabOrder(ui->lineEdit_DrLoginUserName,ui->lineEdit_DrLonginPwd);
    setTabOrder(ui->lineEdit_DrLonginPwd,ui->lineEdit_DrLoginPwd2);
    setTabOrder(ui->lineEdit_DrLoginPwd2,ui->pushButton_DrCommit);
    setTabOrder(ui->pushButton_DrCommit,ui->pushButton_Cancel);
    ui->lineEdit_PaAddress->setHidden(true);
    ui->label_27->setHidden(true);
}

AddUser::~AddUser()
{
    delete ui;
}

//设置tableview表头
void AddUser::SetTableViewHeader()
{
    model->setHorizontalHeaderItem(0,new QStandardItem(QObject::tr("Name")));
    model->setHorizontalHeaderItem(1,new QStandardItem(QObject::tr("ID")));
//    model->setHeaderData(1,Qt::Horizontal,QObject::tr("ID"));
//    QStringList labels = QObject::trUtf8("姓名,ID").simplified().split(",");

//    model->setHorizontalHeaderLabels(labels);

    ui->tableView->setColumnWidth(0,60);
    ui->tableView->setColumnWidth(1,80);
    ui->tableView->setModel(model);
//    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);


}


//void AddUser::automatic_connect()
//{
//    QSqlDatabase mysql = QSqlDatabase::addDatabase("QMYSQL");
//    mysql.setDatabaseName("patientinfo");
//    mysql.setHostName("192.168.10.109");
//    mysql.setUserName("root");
//    mysql.setPassword("123456");
//    mysql.setPort(3306);
//    if(!mysql.open())
//    {
//        ui->label_connectstatus->setText("Database connection failed.");
//        QMessageBox::critical(0, QObject::tr("Notice"),
//                                          "添加失败！\n错误信息："+mysql.lastError().text());
//        return;
//    }
//    else
//    {
//        ui->label_connectstatus->setText("Database connected.");
//        return;
//    }
//}

void AddUser::on_pushButton_Cancel_clicked()
{
    this->reject();
}

void AddUser::SetDefaultDataMode(QueryDataType data)
{
    if(data==TypePatient)
    {
        ui->tabWidget->setCurrentIndex(0);
        ui->tabWidget->setTabEnabled(1,false);
    }
    else
    {
        ui->tabWidget->setCurrentIndex(1);
        ui->tabWidget->setTabEnabled(0,false);
    }
}
//添加患者
void AddUser::on_pushButton_PaCommit_clicked()
{
    QDate date1 = QDate::fromString(ui->LineEdit_Birth->text(),"yyyy-M-d");
    QDate date2 = QDate::fromString(ui->LineEdit_Birth->text(),"yyyy/M/d");
    QDate date3 = QDate::fromString(ui->LineEdit_Birth->text(),"yyyy-MM-dd");
    if(date1.isValid())
    {
        birth = date1.toString("yyyy-MM-dd");
    }
    else if(date2.isValid())
    {
        birth = date2.toString("yyyy-MM-dd");
    }
    else if(date3.isValid())
    {
        birth = date3.toString("yyyy-MM-dd");
    }
    else
    {
        birth = "";
    }
    PID = uuid(1).toString();
    SID = ui->LineEdit_SID->text();
    Pname = ui->lineEdit_PaName->text();
    QString gender = "女";
    QString PaAge = ui->lineEdit_PaAge->text();
    QString mobile = ui->lineEdit_PaMobile->text();
    QString depth = ui->lineEdit_PaDepth->text();
    QString x = ui->lineEdit_PaSizeX->text();
    QString y = ui->lineEdit_PaSizeY->text();
    QString z = ui->lineEdit_PaSizeZ->text();
    QString address = ui->lineEdit_PaAddress->text();
    QString doctor = ui->LineEdit_Doctor->text();

    QSqlDatabase m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName("D:/HIFU-V1.0/backups/database.db");
    m_db.setUserName("root");
    m_db.setPassword("123456");
    m_db.open();
    if(ui->lineEdit_PaName->text().isEmpty()||ui->lineEdit_PaSizeX->text().isEmpty()
            ||ui->lineEdit_PaSizeY->text().isEmpty()||ui->lineEdit_PaSizeZ->text().isEmpty())
    {
        QMessageBox::critical(0, QObject::tr("Notice"),"Name and fibroid size are required.");
        m_db.close();
        return;
    }
    if(ui->lineEdit_PaName->text().contains(" ")||ui->lineEdit_PaSizeX->text().contains(" ")
            ||ui->lineEdit_PaSizeY->text().contains(" ")||ui->lineEdit_PaSizeZ->text().contains(" ")){
        QMessageBox::critical(0, QObject::tr("Notice"),"Name and fibroid size cannot contain spaces.");
        m_db.close();
        return;
    }
    QString strSQl=QString("INSERT INTO PatientInfo (pid,showid,name,gender,birth,age,mobile,size_x,size_y,size_z,depth,address,doctor) VALUES ('%1','%2','%3','%4','%5','%6','%7','%8','%9','%10','%11','%12','%13')")
            .arg(PID)
            .arg(ui->LineEdit_SID->text())
            .arg(ui->lineEdit_PaName->text())
            .arg(ui->comboBox_PaGender->currentText())
            .arg(birth)
            .arg(ui->lineEdit_PaAge->text())
            .arg(ui->lineEdit_PaMobile->text())
            .arg(ui->lineEdit_PaSizeX->text())
            .arg(ui->lineEdit_PaSizeY->text())
            .arg(ui->lineEdit_PaSizeZ->text())
            .arg(ui->lineEdit_PaDepth->text())
            .arg(ui->lineEdit_PaAddress->text())
            .arg(ui->LineEdit_Doctor->text());
    qDebug()<<strSQl;
    QSqlQuery updateQuery;
    if(updateQuery.exec(strSQl))
    {
        QString strSQl2=QString("SELECT * FROM PatientInfo order by id desc limit 0,1");
        bool flag=updateQuery.exec(strSQl2);
        if(flag)
        {
            while(updateQuery.next())
            {
                int id = updateQuery.value(0).toInt();
                emit setdata(id,PID,ui->LineEdit_SID->text(),ui->lineEdit_PaName->text(),ui->comboBox_PaGender->currentText()
                             ,birth,ui->lineEdit_PaAge->text().toInt()
                             ,ui->lineEdit_PaMobile->text(),ui->lineEdit_PaDepth->text().toInt()
                             ,ui->lineEdit_PaSizeX->text().toInt(),ui->lineEdit_PaSizeY->text().toInt()
                             ,ui->lineEdit_PaSizeZ->text().toInt(),ui->lineEdit_PaAddress->text()
                             ,ui->LineEdit_Doctor->text());
            }
        }


        QMessageBox::information(NULL,"Notice","Added successfully.",QMessageBox::Ok);
        updateInfo(PID+","+SID+","+Pname+","+gender+","+birth+","+PaAge+","+mobile+","+depth+","+x+","+y+","+z+","+address+","+doctor);
        m_db.close();
        this->accept();
    }
    else
    {
        QMessageBox::critical(0, QObject::tr("Notice"),
                                          "添加失败！\n错误信息："+updateQuery.lastError().text());
        m_db.close();
        this->reject();
    }

}
//添加医生
void AddUser::on_pushButton_DrCommit_clicked()
{
    QSqlDatabase m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName("D:/HIFU-V1.0/backups/database.db");
    m_db.setUserName("root");
    m_db.setPassword("123456");
    m_db.open();


    if(ui->lineEdit_DrLoginUserName->text().isEmpty()||ui->lineEdit_DrFirstName->text().isEmpty()
            ||ui->lineEdit_DrFullName->text().isEmpty()||ui->lineEdit_DrLastName->text().isEmpty())
    {
        QMessageBox::critical(0, QObject::tr("Notice"),"Name and username are required.");
        m_db.close();
        return;
    }
    if(ui->lineEdit_DrLoginUserName->text().contains(" ")||ui->lineEdit_DrFirstName->text().contains(" ")
            ||ui->lineEdit_DrLastName->text().contains(" ")){
        QMessageBox::critical(0, QObject::tr("Notice"),"Username, last name, and first name cannot contain spaces.");
        m_db.close();
        return;
    }
    if(ui->lineEdit_DrLonginPwd->text().isEmpty())
    {
        QMessageBox::critical(0, QObject::tr("Notice"),"Password is required.");
        m_db.close();
        return;
    }
    if(ui->lineEdit_DrLonginPwd->text()!=ui->lineEdit_DrLoginPwd2->text())
    {
        QMessageBox::critical(0, QObject::tr("Notice"),"Passwords do not match.");
        m_db.close();
        return;
    }

    QSqlQuery query;

    query.prepare(QString("SELECT COUNT(*) FROM LoginInfo WHERE userName = '%1'").arg(ui->lineEdit_DrLoginUserName->text()));
    if(query.exec()){
        if(query.next()){
            int count = query.value(0).toInt();
            if(count >0){
                //存在
                QMessageBox::critical(0, QObject::tr("Notice"),"This account already exists.");
                m_db.close();
                return;
            }
        }
    }

    QString strSQl=QString("INSERT INTO DrInfo (firstName,lastName,fullName,gender,age,post,isAdmin,mobile,email) VALUES ('%1','%2','%3','%4','%5','%6','%7','%8','%9')")
            .arg(ui->lineEdit_DrFirstName->text())
            .arg(ui->lineEdit_DrLastName->text())
            .arg(ui->lineEdit_DrFullName->text())
            .arg(ui->comboBox_DrGender->currentText())
            .arg(ui->lineEdit_DrAge->text())
            .arg(ui->lineEdit_DrPost->text())
            .arg(ui->checkBox_DrIsAdmin->isChecked())
            .arg(ui->lineEdit_DrMobile->text())
            .arg(ui->lineEdit_DrEmail->text());
    qDebug()<<strSQl;

    //hash加密
    QCryptographicHash hash256(QCryptographicHash::Sha256);
//    QString Username = ui->lineEdit_DrLoginUserName->text();
    QString pw = ui->lineEdit_DrLonginPwd->text();
//    hash256.addData(Username.toLatin1().data());
//    QString hash_uservalue = hash256.result().toHex().toUpper();
    hash256.addData(pw.toLatin1().data());
    QString hash_pwvalue = hash256.result().toHex().toUpper();
    qDebug()<<"账号hash:"<<ui->lineEdit_DrLoginUserName->text()<<" 密码hash："<<hash_pwvalue
           <<"密码明码："<<pw;

    QSqlQuery updateQuery;
    if(updateQuery.exec(strSQl))
    {
        int insertId=updateQuery.lastInsertId().toInt();
        QString strAddLoginSQl=QString("INSERT INTO LoginInfo (name,userName,pw,DrID) VALUES ('%1','%2','%3',%4)")
                .arg(ui->lineEdit_DrFullName->text())
                .arg(ui->lineEdit_DrLoginUserName->text())
                .arg(hash_pwvalue)
                .arg(insertId);
        if(updateQuery.exec(strAddLoginSQl))
        {
            QMessageBox::information(NULL,"Notice","Added successfully.",QMessageBox::Ok);
        }
        else
        {
            QMessageBox::critical(0, QObject::tr("Notice"),
                                  "添加登录信息记录失败！\n错误信息："+updateQuery.lastError().text());
        }

        qDebug()<<insertId;
        m_db.close();
        this->accept();
    }
    else
    {
        QMessageBox::critical(0, QObject::tr("Notice"),
                                          "添加基本信息记录失败！\n错误信息："+updateQuery.lastError().text());
        m_db.close();
        this->reject();
    }
}

void AddUser::on_pushButton_findname_clicked()
{
    //连接数据库
    QSqlDatabase mysql = QSqlDatabase::addDatabase("QMYSQL");
    mysql.setDatabaseName("patientinfo");
    mysql.setHostName("192.168.10.109");
    mysql.setUserName("root");
    mysql.setPassword("123456");
    mysql.setPort(3306);
    if(!mysql.open())
    {
        ui->label_connectstatus->setText("Database connection failed.");
        QMessageBox::critical(0, QObject::tr("Notice"),
                                          "添加失败！\n错误信息："+mysql.lastError().text());
        return;
    }
    else
    {
        ui->label_connectstatus->setText("Database connected.");

    }



    QString patientname = ui->lineEdit_findname->text();

    if(ui->lineEdit_findname->text().indexOf(" ",0)==0)
    {
        QMessageBox::warning(NULL,"Notice","Name cannot start with a space.");
        return;
    }
    if(ui->lineEdit_findname->text().isEmpty())
    {
        QMessageBox::warning(NULL,"Notice","Name is required.");
        return;
    }
    QSqlQuery findquery_patient;
//    findquery_patient.exec("select 编号,姓名,年龄,电话,家庭住址 from 病人信息 where 姓名='"+patientname+"'");
    findquery_patient.exec("select id,name,age,telephone,address from patient where name='"+patientname+"'");
    QSqlRecord rec = findquery_patient.record();
    QSqlError rec_err = findquery_patient.lastError();
    if(rec_err.isValid())
    {
        QMessageBox::information(NULL,"Notice","Invalid name.",QMessageBox::Yes);
        mysql.close();
        return;
    }
    qDebug()<<"record:"<<rec;
//    int patient_num = 0;
    //释放内存
    id.clear();
    name.clear();
    age.clear();
    telephone.clear();
    birthday.clear();
    address.clear();

    rec_err = findquery_patient.lastError();
//        int namecol = rec.indexOf(patientname);
    qDebug()<<"错误："<<rec_err.isValid();

    while(findquery_patient.next())
    {
        rec = findquery_patient.record();

        id.push_back(findquery_patient.value(0).toString());
        name.push_back(findquery_patient.value(1).toString());
        age.push_back(findquery_patient.value(2).toString());
        telephone.push_back(findquery_patient.value(3).toString());
//        birthday.push_back(findquery_patient.value(4).toString());
        address.push_back(findquery_patient.value(4).toString());

//        QMessageBox::information(NULL,"Notice","查找成功：",QMessageBox::Yes);
//        qDebug()<<"name:"<<name<<" age:"<<age<<" telephone:"<<telephone<<" birathday:"<<birthday<<" address:"<<address;
    }

    qDebug()<<"数量："<<name.count();




    //写入表格
    model->clear();
    SetTableViewHeader();
//    model->removeRow(name.count());
    for(int i=0;i<name.count();i++)
    {
        //QString item = StudyUID[i];
        qDebug()<<"NAME选项为："<<name[i];
//        qDebug()<<"ID选项为："<<birthday[i];
        QStandardItem *itemname = new QStandardItem(name[i]);
        model->setItem(i,0,itemname);
        QStandardItem *itemid = new QStandardItem(id[i]);
        model->setItem(i,1,itemid);

//                    ui->comboBox_study->addItem(StudyUID[i]);
    }

    //关闭数据库
    mysql.close();
    QSqlDatabase::removeDatabase(mysql.connectionName());
}

void AddUser::on_tableView_clicked(const QModelIndex &index)
{
    //连接数据库
    QSqlDatabase mysql = QSqlDatabase::addDatabase("QMYSQL");
    mysql.setDatabaseName("patientinfo");
    mysql.setHostName("192.168.10.109");
    mysql.setUserName("root");
    mysql.setPassword("123456");
    mysql.setPort(3306);
    if(!mysql.open())
    {
        ui->label_connectstatus->setText("Database connection failed.");
        QMessageBox::critical(0, QObject::tr("Notice"),
                                          "添加失败！\n错误信息："+mysql.lastError().text());
        return;
    }
    else
    {
        ui->label_connectstatus->setText("Database connected.");

    }


    int tableview_row = index.row();

    QSqlQuery findquery_inspect;
    findquery_inspect.exec("select deep,sizeX,sizeY,sizeZ from inspect where id='"+id[tableview_row]+"'");
    QSqlRecord inspect_rec = findquery_inspect.record();
    qDebug()<<"inspectrecord:"<<inspect_rec;
//    qDebug()<<"tele:"<<telephone[tableview_row].isNull();
//    int patient_num = 0;
    //释放内存
    deep.clear();
    sizeX.clear();
    sizeY.clear();
    sizeZ.clear();

    ui->lineEdit_PaAddress->clear();
    ui->lineEdit_PaAge->clear();
    ui->lineEdit_PaDepth->clear();
    ui->lineEdit_PaMobile->clear();
    ui->lineEdit_PaSizeX->clear();
    ui->lineEdit_PaSizeY->clear();
    ui->lineEdit_PaSizeZ->clear();
    while(findquery_inspect.next())
    {
        inspect_rec = findquery_inspect.record();

//        int namecol = rec.indexOf(patientname);
//        qDebug()<<"数量："<<namecol;
//        inspect_err = findquery_inspect.lastError();
//        qDebug()<<"rec_err:"<<
        deep.push_back(findquery_inspect.value(0).toString());
        sizeX.push_back(findquery_inspect.value(1).toString());
        sizeY.push_back(findquery_inspect.value(2).toString());
        sizeZ.push_back(findquery_inspect.value(3).toString());
//        QMessageBox::information(NULL,"Notice","查找成功：",QMessageBox::Yes);
        qDebug()<<"deep:"<<deep<<" sizeX:"<<sizeX<<" sizeY:"<<sizeY<<" sizeZ:"<<sizeZ;
    }

    if(name[tableview_row]!="null")
        ui->lineEdit_PaName->setText(name[tableview_row]);

    if(age[tableview_row]!="null")
        ui->lineEdit_PaAge->setText(age[tableview_row]);

//    qDebug()<<"tele:"<<telephone[tableview_row].isNull();
    if(telephone[tableview_row]!="null")
        ui->lineEdit_PaMobile->setText(telephone[tableview_row]);

    if(address[tableview_row]!="null")
        ui->lineEdit_PaAddress->setText(address[tableview_row]);

    if(deep[0]!="null")
        ui->lineEdit_PaDepth->setText(deep[0]);


    if(sizeX[0]!="null")
        ui->lineEdit_PaSizeX->setText(sizeX[0]);

    if(sizeY[0]!="null")
        ui->lineEdit_PaSizeY->setText(sizeY[0]);

    if(sizeZ[0]!="null")
       ui->lineEdit_PaSizeZ->setText(sizeY[0]);


    if(name[tableview_row]=="null" || sizeX[0]=="null" || sizeY[0]=="null" || sizeZ[0]=="null")
    {
        QMessageBox::information(NULL,"Notice","No name or fibroid size data was found. Enter it manually.",QMessageBox::Yes);
    }

    //关闭数据库
    mysql.close();
//    QSqlDatabase::removeDatabase(mysql.connectionName());
}

//void AddUser::close_mysql()
//{
//    QSqlDatabase mysql = QSqlDatabase::addDatabase("QMYSQL");
//    mysql.close();
//}

quint64 AddUser::MakeId(quint32 proj_id, quint32 mach_id, quint32 obj_id, quint32 squ_id)
{
    quint64 stamp = static_cast<quint64>(QDateTime::currentMSecsSinceEpoch() - QDateTime::fromString(Baseline, "yyyy-M-d").toMSecsSinceEpoch()) / 1000;
    return (stamp << 38) | (static_cast<quint64>(proj_id) << 29) | (static_cast<quint64>(mach_id) << 12) | (static_cast<quint64>(obj_id) << 8) | static_cast<quint64>(squ_id);
}

QHash<QString, QVariant> AddUser::SplitId(quint64 mId)
{
    QHash<QString, QVariant> snow;

    QHash<QString, quint64> masks = {
        {"sequence", 8},
        {"object", 4},
        {"machine", 17},
        {"project", 9},
        {"stamp", 25},
    };

    for (auto mask : masks.keys()) {
        snow[mask] = static_cast<qint64>(mId & ((1ULL << masks[mask]) - 1));
        mId >>= masks[mask];
    }

    snow["stamp"] = static_cast<qint64>(snow["stamp"].toUInt()) * 1000 + QDateTime::fromString(Baseline, "yyyy-M-d").toMSecsSinceEpoch();
    snow["date"] = QDateTime::fromMSecsSinceEpoch(snow["stamp"].toLongLong()).toString();

    return snow;
}

quint32 AddUser::mIDIndex()
{
    if (idStamp == static_cast<quint64>(QDateTime::currentMSecsSinceEpoch() / 1000)) {
        idIndex++;
    } else {
        idIndex = 0;
        idStamp = static_cast<quint64>(QDateTime::currentMSecsSinceEpoch() / 1000);
    }

    return idIndex;
}

void AddUser::mIDInit(quint32 proj_id, quint32 mach_id)
{
    idStamp = static_cast<quint64>(QDateTime::currentMSecsSinceEpoch() / 1000);
    idIndex = 0;

    project = proj_id;
    machine = mach_id;
}

QVariant AddUser::uuid(QVariant param)
{
    if (param.type() == QVariant::ULongLong) {
        return SplitId(param.toULongLong());
    }
    return MakeId(project, machine, param.toUInt(), mIDIndex());
}

void AddUser::updateInfoFile(QString name, QByteArray &log)
{
    QFile file(name);
    if (file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        QTextStream stream(&file);
        stream.setCodec("UTF-8");
        stream << log << endl;
        file.close();
    }
}

void AddUser::updateInfo(const QString &msg)
{
    QString path = "D:/TreationLOGS/" + PID;
    QDir dir(path);
    if (!dir.exists())
    {
        dir.mkpath(path);
    }
    QString logFileName = path + "/" + PID
                                + ".log";
    QByteArray log;
    log += QString("%1\r\n").arg(msg);

    updateInfoFile(logFileName, log);
}
