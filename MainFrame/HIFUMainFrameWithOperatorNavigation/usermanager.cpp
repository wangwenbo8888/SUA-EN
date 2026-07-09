#include "usermanager.h"
#include "ui_usermanager.h"
#include <QDebug>
#include <QSqlTableModel>
#include <QGroupBox>
#include <QMessageBox>
#include "adduser.h"
#include "usermanager_global.h"
#include <QSqlQuery>
#include <QSqlError>
#include "logininfoedit.h"
#include "commondefine.h"
#include "patientmanager.h"
#include <QDate>
#include <QFileSystemWatcher>
#if _MSC_VER >1600
#pragma execution_character_set("utf-8")
#endif

UserManager::UserManager(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UserManager)
{
    ui->setupUi(this);

    ui->label_Body_Orientation_Left->installEventFilter(this);
    ui->label_Body_Orientation_Right->installEventFilter(this);

    this->setWindowFlags(Qt::FramelessWindowHint);

    sSearchField="name";

    ui->pushButton_PaCommit->setEnabled(false);
    ui->pushButton_DrCommit->setEnabled(false);

    ui->tabWidget->tabBar()->hide();

    ui->LineEdit_PID->setVisible(false);
    ui->pIDLabel->setVisible(false);
    ui->label_7->setVisible(false);
    ui->lineEdit_PaAge->setVisible(false);
    ui->radioButton_SAge->setVisible(false);
    ui->radioButton_SMobile->setVisible(false);

    ui->tableView->horizontalHeader()->setFixedHeight(40);
    ui->tableView->verticalHeader()->setFixedWidth(30);
    ui->tableView->verticalHeader()->setDefaultAlignment(Qt::AlignCenter);
    ui->tableView->horizontalHeader()
            ->setStyleSheet("QHeaderView::section {color: black;font: 75 12pt \"微软雅黑\";border: 1px solid #6c6c6c;}");
    ui->tableView->verticalHeader()
            ->setStyleSheet("QHeaderView::section {color: black;font: 75 10pt \"微软雅黑\";border: 1px solid #6c6c6c;}");
    this->ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);  //单击Select一行
    this->ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection); //设置只能Select一行，不能多行选中
    this->ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);   //设置每行内容不可更改

    // 设置限制Input为整数及范围
    ui->lineEdit_DrAge->setValidator(new QIntValidator(18,150,this));
    ui->lineEdit_PaAge->setValidator(new QIntValidator(18,150,this));

    QRegExp rx("[M F O]");
    QRegExp regx("[a-zA-Z0-9\-\\\_]{25}");
    ui->lineEdit_PaGender->setEnabled(false);
//    ui->lineEdit_PaGender->setValidator(new QRegExpValidator(regx, this));
//    ui->lineEdit_PaGender->setToolTip("请Input字符和数字的组合。");

    ui->lineEdit_DrGender->setValidator(new QRegExpValidator(rx, this));
    ui->lineEdit_DrGender->setToolTip("Enter M for male, F for female, or O for other.");

    ui->lineEdit_PaDepth->setValidator(new QIntValidator(0,50,this));
    ui->lineEdit_PaSizeX->setValidator(new QIntValidator(0,105,this));
    ui->lineEdit_PaSizeY->setValidator(new QIntValidator(0,105,this));
    ui->lineEdit_PaSizeZ->setValidator(new QIntValidator(0,105,this));
//    ui->lineEdit_SFieldText->setValidator(new QIntValidator(0,20,this));

    model1 = new CustomSqlTableModel(this);
    model1->setTable("PatientInfo");
    model1->select();
    //model->setEditStrategy(QSqlTableModel::OnFieldChange);//对所有模型的改变都会立即应用到数据库
    model = new QSqlTableModel(this);
    model->setTable("DrInfo");
    model->select();
    ui->tableView->setModel(model1);

    m_QDataType=TypePatient;

    SetPatientCtlEditable(true);
    SetDoctorCtlEditable(true);
    SetTableViewHeader(1);
    m_PaDataRow.bClear=true;

    ui->pushButton_PaTreatment->setStyleSheet(NOT_WORK);
    ui->lineEdit_PaAddress->setHidden(1);
    ui->label_24->setHidden(1);
    ui->lineEdit_SFieldText->setMaxLength(12);

    ui->groupBox_3->setVisible(false);

    QFileSystemWatcher *watcher = new QFileSystemWatcher(this);
    watcher->addPath("D:/HIFU-V1.0/backups/database.db");
    QObject::connect(watcher, &QFileSystemWatcher::fileChanged, this, &UserManager::UpdateModel);
}

UserManager::~UserManager()
{
    delete model;
    delete model1;
    delete ui;
}

void UserManager::on_commandLinkButton_Patient_clicked()
{
    QSqlDatabase m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName("D:/HIFU-V1.0/backups/database.db");
    m_db.setUserName("root");
    m_db.setPassword("123456");
    m_db.open();
    ui->commandLinkButton_Patient->setChecked(true);
    ui->commandLinkButton_Doctor->setChecked(false);

    if (!ui->lineEdit_SFieldText->text().isEmpty())
    {
        ui->lineEdit_SFieldText->setText("");
    }
    model1 = new CustomSqlTableModel(this);
    model1->setTable("PatientInfo");
    //设置ID倒序显示
    model1->setSort(model1->fieldIndex("id"), Qt::DescendingOrder);
    model1->select();
    while(model1->canFetchMore())
    {
        model1->fetchMore();
    }
    ui->tableView->setModel(model1);

    ui->tabWidget->setCurrentIndex(0);
    m_QDataType=TypePatient;
    m_PaDataRow.clear();
    SetPatientCtlData(m_PaDataRow);
    SetTableViewHeader(1);
    ui->lineEdit_SID->setText("");
    ui->LineEdit_Doctor->setText("");
    ui->lineEdit_DrGender->setText("");
    ui->LineEdit_Birth->setText("");
    ui->lineEdit_PaGender->setText("");
    m_db.close();
 }

void UserManager::on_commandLinkButton_Doctor_clicked()
{
    QSqlDatabase m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName("D:/HIFU-V1.0/backups/database.db");
    m_db.setUserName("root");
    m_db.setPassword("123456");
    m_db.open();
    ui->commandLinkButton_Patient->setChecked(false);
    ui->commandLinkButton_Doctor->setChecked(true);

    if (!ui->lineEdit_SFieldText->text().isEmpty())
    {
        ui->lineEdit_SFieldText->setText("");
    }
    model = new QSqlTableModel(this);
    model->setTable("DrInfo");
    model->select();
    ui->tableView->setModel(model);
    SetTableViewHeader(2);
    ui->tabWidget->setCurrentIndex(1);
    m_QDataType=TypeDoctor;
    m_DrDataRow.clear();
    SetDoctorCtlData(m_DrDataRow);
    m_db.close();
}

//设置数据视图表头
void UserManager::SetTableViewHeader(int dataType)
{
    switch (dataType) {
    case 1:
//        qDebug()<<"病人数据表头";
        model1->setHeaderData(0,Qt::Horizontal,QObject::tr("ID"));
        model1->setHeaderData(1,Qt::Horizontal,QObject::tr("PID"));
        model1->setHeaderData(2,Qt::Horizontal,QObject::tr("Case ID"));
        model1->setHeaderData(3,Qt::Horizontal,QObject::tr("Name"));
        model1->setHeaderData(4,Qt::Horizontal,QObject::tr("Sex"));
        model1->setHeaderData(5,Qt::Horizontal,QObject::tr("Date of Birth"));
        model1->setHeaderData(6,Qt::Horizontal,QObject::tr("Age"));
        model1->setHeaderData(7,Qt::Horizontal,QObject::tr("Phone"));
        model1->setHeaderData(8,Qt::Horizontal,QObject::tr("Treatment Depth"));
        model1->setHeaderData(9,Qt::Horizontal,QObject::tr("Fibroid Size (X)"));
        model1->setHeaderData(10,Qt::Horizontal,QObject::tr("Fibroid Size (Y)"));
        model1->setHeaderData(11,Qt::Horizontal,QObject::tr("Fibroid Size (Z)"));
        model1->setHeaderData(12,Qt::Horizontal,QObject::tr("Address"));
        model1->setHeaderData(13,Qt::Horizontal,QObject::tr("Attending Physician"));
        this->ui->tableView->setColumnHidden(0,true);
        this->ui->tableView->setColumnHidden(1,true);
        this->ui->tableView->setColumnHidden(6,true);
        this->ui->tableView->setColumnHidden(12,true);
        this->ui->tableView->resizeColumnsToContents();
        this->ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        break;
    case 2:
//        qDebug()<<"Physicians数据表头";
        model->setHeaderData(0,Qt::Horizontal,QObject::tr("ID"));
        model->setHeaderData(1,Qt::Horizontal,QObject::tr("First Name"));
        model->setHeaderData(2,Qt::Horizontal,QObject::tr("Last Name"));
        model->setHeaderData(3,Qt::Horizontal,QObject::tr("Full Name"));
        model->setHeaderData(4,Qt::Horizontal,QObject::tr("Sex"));
        model->setHeaderData(5,Qt::Horizontal,QObject::tr("Age"));
        model->setHeaderData(6,Qt::Horizontal,QObject::tr("Position"));
        model->setHeaderData(7,Qt::Horizontal,QObject::tr("Administrator"));
        model->setHeaderData(8,Qt::Horizontal,QObject::tr("Phone"));
        model->setHeaderData(9,Qt::Horizontal,QObject::tr("Email"));
        this->ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        break;
    default:
        break;
    }
}

void UserManager::on_radioButton_SID_clicked()
{
    sSearchField="showid";
}

void UserManager::on_radioButton_SFirstName_clicked()
{
    if(m_QDataType==TypePatient)
    {
        sSearchField="name";
    }
    else
    {
        sSearchField="fullName";
    }
}

void UserManager::on_radioButton_SAge_clicked()
{
    sSearchField="age";
}

void UserManager::on_radioButton_SMobile_clicked()
{
    sSearchField="mobile";
}

//Query
void UserManager::on_toolButton_Search_clicked()
{
    QSqlDatabase m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName("D:/HIFU-V1.0/backups/database.db");
    m_db.setUserName("root");
    m_db.setPassword("123456");
    m_db.open();
    qDebug()<<"字符串长度为"<<ui->lineEdit_SFieldText->text().size();
    if(ui->lineEdit_SFieldText->text()=="")
    {
        QMessageBox::information(NULL,"Notice","Enter a search value.",QMessageBox::Ok);
        return;
    }

//    if ((ui->lineEdit_SFieldText->text().size()<6||ui->lineEdit_SFieldText->text().size()>12)&&(sSearchField=="name"||sSearchField=="fullName"))
//    {
//        QMessageBox::warning(0,"Notice","请Input6-12个字符","Confirm",0);
//        return;
//    }
    if(ui->radioButton_SFirstName->isChecked())
    {
        if(m_QDataType==TypePatient)
        {
            sSearchField="name";
        }
        else
        {
            sSearchField="fullName";
        }
    }
    model->setFilter(sSearchField+" = '"+ui->lineEdit_SFieldText->text()+"'");
    model1->setFilter(sSearchField+" = '"+ui->lineEdit_SFieldText->text()+"'");
    qDebug()<<model->filter();
    m_db.close();
}
//Clear Search
void UserManager::on_toolButton_UndoSearch_clicked()
{
    if(m_QDataType==TypePatient)
    {
        on_commandLinkButton_Patient_clicked();
    }
    else if(m_QDataType=TypeDoctor)
    {
        on_commandLinkButton_Doctor_clicked();
    }
}

//显示账户管理
void UserManager::ShowUserManager(bool isPatient)
{
    if(isPatient){
        ui->commandLinkButton_Patient->setEnabled(true);
        ui->commandLinkButton_Doctor->setEnabled(true);
        on_commandLinkButton_Patient_clicked();
    }
    else{
        on_commandLinkButton_Patient_clicked();
        ui->commandLinkButton_Patient->setEnabled(true);
        ui->commandLinkButton_Doctor->setEnabled(true);
//        ui->commandLinkButton_Patient->setEnabled(false);
    }
    this->show();
}

//TableView点击事件
void UserManager::on_tableView_clicked(const QModelIndex &index)
{
    if(m_QDataType==TypePatient)
    {
        m_PaDataRow.SetData(index.sibling(index.row(),0).data().toInt(),
                            index.sibling(index.row(),1).data().toString(),
                            index.sibling(index.row(),2).data().toString(),
                            index.sibling(index.row(),3).data().toString(),
                            index.sibling(index.row(),4).data().toString(),
                            index.sibling(index.row(),5).data().toString(),
                            index.sibling(index.row(),6).data().toInt(),
                            index.sibling(index.row(),7).data().toString(),
                            index.sibling(index.row(),8).data().toInt(),
                            index.sibling(index.row(),9).data().toInt(),
                            index.sibling(index.row(),10).data().toInt(),
                            index.sibling(index.row(),11).data().toInt(),
                            index.sibling(index.row(),12).data().toString(),
                            index.sibling(index.row(),13).data().toString()
                            );

        SetPatientCtlData(m_PaDataRow);
    }
    else if(m_QDataType==TypeDoctor)
    {
        m_DrDataRow.SetData(index.sibling(index.row(),0).data().toInt(),
                            index.sibling(index.row(),1).data().toString(),
                            index.sibling(index.row(),2).data().toString(),
                            index.sibling(index.row(),3).data().toString(),
                            index.sibling(index.row(),4).data().toString(),
                            index.sibling(index.row(),5).data().toInt(),
                            index.sibling(index.row(),6).data().toString(),
                            index.sibling(index.row(),7).data().toBool(),
                            index.sibling(index.row(),8).data().toString(),
                            index.sibling(index.row(),9).data().toString());
        SetDoctorCtlData(m_DrDataRow);
    }

    SetPatientCtlEditable(true);
    SetDoctorCtlEditable(true);
}

//Add用户
void UserManager::on_commandLinkButton_add_clicked()
{
    AddUser addForm;
    addForm.SetDefaultDataMode(m_QDataType);
    connect(&addForm,SIGNAL(setdata(int,QString,QString,int,QString,int,int,int,int,QString)),
            this,SLOT(Newtreatment(int,QString,QString,int,QString,int,int,int,int,QString)));
    if(addForm.exec()==Accepted)
    {
        if(m_QDataType==TypePatient)
        {
            on_commandLinkButton_Patient_clicked();
        }
        else if(m_QDataType=TypeDoctor)
        {
            on_commandLinkButton_Doctor_clicked();
        }
    }
}

void UserManager::SetPatientCtlData(PatientDataRow data)
{
    ui->lineEdit_PaAge->setText(data.age_str);
    ui->lineEdit_PaName->setText(data.name);
    ui->lineEdit_SID->setText(data.sid);
    ui->LineEdit_PID->setText(data.pid);
    ui->LineEdit_Doctor->setText(data.doctor);
    ui->LineEdit_Birth->setText(data.birth);
    ui->lineEdit_PaGender->setText(data.gender);
    ui->lineEdit_PaMobile->setText(data.mobile);
    ui->lineEdit_PaSizeX->setText(data.size_x_str);
    ui->lineEdit_PaSizeY->setText(data.size_y_str);
    ui->lineEdit_PaSizeZ->setText(data.size_z_str);
    ui->lineEdit_PaSizeSring->setText(data.size_str);
    ui->lineEdit_PaDepth->setText(data.depth_str);
    ui->lineEdit_PaAddress->setText(data.address);
}

void UserManager::SetDoctorCtlData(DoctorDataRow data)
{
    ui->lineEdit_DrAge->setText(data.age_str);
    ui->lineEdit_DrEmail->setText(data.email);
    ui->lineEdit_DrFirstName->setText(data.firstName);
    ui->lineEdit_DrFullName->setText(data.fullName);
    ui->lineEdit_DrGender->setText(data.gender);
    ui->lineEdit_DrID->setText(data.id_str);
    if(data.isClear())
    {
        ui->lineEdit_DrIsAdmin->setText("");
    }
    else
    {
        ui->lineEdit_DrIsAdmin->setText(QString::number(data.isAdmin));
    }
    ui->lineEdit_DrLastName->setText(data.lastName);
    ui->lineEdit_DrMobile->setText(data.mobile);
    ui->lineEdit_DrPost->setText(data.post);
}

//设置PatientsDetails控件可Edit性
void UserManager::SetPatientCtlEditable(bool editable)
{
    ui->lineEdit_PaAge->setReadOnly(editable);
    ui->lineEdit_PaName->setReadOnly(editable);
    ui->lineEdit_SID->setReadOnly(editable);
    ui->LineEdit_Birth->setReadOnly(editable);
//    ui->lineEdit_PaGender->setReadOnly(editable);
    ui->lineEdit_PaMobile->setReadOnly(editable);
    ui->lineEdit_PaSizeX->setReadOnly(editable);
    ui->lineEdit_PaSizeY->setReadOnly(editable);
    ui->lineEdit_PaSizeZ->setReadOnly(editable);
    ui->lineEdit_PaSizeSring->setReadOnly(editable);
    ui->lineEdit_PaDepth->setReadOnly(editable);
    ui->lineEdit_PaAddress->setReadOnly(editable);
    ui->LineEdit_Doctor->setReadOnly(editable);
    if(editable)
    {
        ui->lineEdit_SID->setStyleSheet("");
        ui->lineEdit_PaAge->setStyleSheet("");
        ui->lineEdit_PaName ->setStyleSheet("");
//        ui->lineEdit_PaGender->setStyleSheet("");
        ui->lineEdit_PaMobile->setStyleSheet("");
        ui->lineEdit_PaSizeX->setStyleSheet("");
        ui->lineEdit_PaSizeY->setStyleSheet("");
        ui->lineEdit_PaSizeZ->setStyleSheet("");
        ui->lineEdit_PaSizeSring->setStyleSheet("");
        ui->lineEdit_PaDepth->setStyleSheet("");
        ui->lineEdit_PaAddress->setStyleSheet("");
        ui->LineEdit_Doctor->setStyleSheet("");
        ui->LineEdit_Birth->setStyleSheet("");
        ui->pushButton_PaCommit->setEnabled(false);
    }
    else
    {
        ui->lineEdit_PaAge->setStyleSheet("background-color: rgb(248, 248, 248);color: rgb(0, 0, 0);");
        ui->lineEdit_PaName->setStyleSheet("background-color: rgb(248, 248, 248);color: rgb(0, 0, 0);");
//        ui->lineEdit_PaGender->setStyleSheet("background-color: rgb(248, 248, 248);color: rgb(0, 0, 0);");
        ui->lineEdit_PaMobile->setStyleSheet("background-color: rgb(248, 248, 248);color: rgb(0, 0, 0);");
        ui->lineEdit_PaSizeX->setStyleSheet("background-color: rgb(248, 248, 248);color: rgb(0, 0, 0);");
        ui->lineEdit_PaSizeY->setStyleSheet("background-color: rgb(248, 248, 248);color: rgb(0, 0, 0);");
        ui->lineEdit_PaSizeZ->setStyleSheet("background-color: rgb(248, 248, 248);color: rgb(0, 0, 0);");
        ui->lineEdit_PaSizeSring->setStyleSheet("background-color: rgb(248, 248, 248);color: rgb(0, 0, 0);");
        ui->lineEdit_PaDepth->setStyleSheet("background-color: rgb(248, 248, 248);color: rgb(0, 0, 0);");
        ui->lineEdit_PaAddress->setStyleSheet("background-color: rgb(248, 248, 248);color: rgb(0, 0, 0);");
        ui->lineEdit_SID->setStyleSheet("background-color: rgb(248, 248, 248);color: rgb(0, 0, 0);");
        ui->LineEdit_Doctor->setStyleSheet("background-color: rgb(248, 248, 248);color: rgb(0, 0, 0);");
        ui->LineEdit_Birth->setStyleSheet("background-color: rgb(248, 248, 248);color: rgb(0, 0, 0);");
        ui->pushButton_PaCommit->setEnabled(true);
    }

}

//设置医师详细控件可Edit性
void UserManager::SetDoctorCtlEditable(bool editable)
{
    ui->lineEdit_DrAge->setReadOnly(editable);
    ui->lineEdit_DrEmail->setReadOnly(editable);
    ui->lineEdit_DrFirstName->setReadOnly(editable);
    ui->lineEdit_DrFullName->setReadOnly(editable);
    ui->lineEdit_DrGender->setReadOnly(editable);
    //ui->lineEdit_DrID->setReadOnly(editable);
    ui->lineEdit_DrIsAdmin->setReadOnly(editable);
    ui->lineEdit_DrLastName->setReadOnly(editable);
    ui->lineEdit_DrMobile->setReadOnly(editable);
    ui->lineEdit_DrPost->setReadOnly(editable);

    if(editable)
    {
        ui->lineEdit_DrAge->setStyleSheet("");
        ui->lineEdit_DrEmail ->setStyleSheet("");
        ui->lineEdit_DrFirstName->setStyleSheet("");
        ui->lineEdit_DrFullName->setStyleSheet("");
        ui->lineEdit_DrGender->setStyleSheet("");
        ui->lineEdit_DrIsAdmin->setStyleSheet("");
        ui->lineEdit_DrLastName->setStyleSheet("");
        ui->lineEdit_DrMobile->setStyleSheet("");
        ui->lineEdit_DrPost->setStyleSheet("");
        ui->pushButton_DrCommit->setEnabled(false);
    }
    else
    {
        ui->lineEdit_DrAge->setStyleSheet("background-color: rgb(248, 248, 248);color: rgb(0, 0, 0);");
        ui->lineEdit_DrEmail ->setStyleSheet("background-color: rgb(248, 248, 248);color: rgb(0, 0, 0);");
        ui->lineEdit_DrFirstName->setStyleSheet("background-color: rgb(248, 248, 248);color: rgb(0, 0, 0);");
        ui->lineEdit_DrFullName->setStyleSheet("background-color: rgb(248, 248, 248);color: rgb(0, 0, 0);");
        ui->lineEdit_DrGender->setStyleSheet("background-color: rgb(248, 248, 248);color: rgb(0, 0, 0);");
        ui->lineEdit_DrIsAdmin->setStyleSheet("background-color: rgb(248, 248, 248);color: rgb(0, 0, 0);");
        ui->lineEdit_DrLastName->setStyleSheet("background-color: rgb(248, 248, 248);color: rgb(0, 0, 0);");
        ui->lineEdit_DrMobile->setStyleSheet("background-color: rgb(248, 248, 248);color: rgb(0, 0, 0);");
        ui->lineEdit_DrPost->setStyleSheet("background-color: rgb(248, 248, 248);color: rgb(0, 0, 0);");
        ui->pushButton_DrCommit->setEnabled(true);
    }
}

//PatientsEdit
void UserManager::on_pushButton_PaEdit_clicked()
{
    if(!m_PaDataRow.isClear())
    {
        SetPatientCtlEditable(false);
    }
}
//PhysiciansEdit
void UserManager::on_pushButton_DrEdit_clicked()
{
    if(!m_DrDataRow.isClear())
    {
        SetDoctorCtlEditable(false);
    }
}
//PhysiciansSubmit
void UserManager::on_pushButton_DrCommit_clicked()
{
     if(ui->lineEdit_DrFullName->text().isEmpty()||ui->lineEdit_DrLastName->text().isEmpty()||ui->lineEdit_DrFirstName->text().isEmpty())
    {
        QMessageBox::critical(0, QObject::tr("Notice"),"Name is required.");
        return;
    }
     QSqlDatabase m_db = QSqlDatabase::addDatabase("QSQLITE");
     m_db.setDatabaseName("D:/HIFU-V1.0/backups/database.db");
     m_db.setUserName("root");
     m_db.setPassword("123456");
     m_db.open();
    QString strSQl=QString("UPDATE DrInfo SET firstName = '%1',lastName='%2',fullName='%3',gender='%4',age='%5',post='%6',isAdmin='%7',mobile='%8',email='%9' WHERE ID = %10")
            .arg(ui->lineEdit_DrFirstName->text())
            .arg(ui->lineEdit_DrLastName->text())
            .arg(ui->lineEdit_DrFullName->text())
            .arg(ui->lineEdit_DrGender->text())
            .arg(ui->lineEdit_DrAge->text())
            .arg(ui->lineEdit_DrPost->text())
            .arg(ui->lineEdit_DrIsAdmin->text())
            .arg(ui->lineEdit_DrMobile->text())
            .arg(ui->lineEdit_DrEmail->text())
            .arg(m_DrDataRow.id_str);
    qDebug()<<strSQl;
    QSqlQuery updateQuery(strSQl);
    if(updateQuery.exec())
    {
        QMessageBox::information(NULL,"Notice","Updated successfully.",QMessageBox::Ok);
    }
    else
    {
        QMessageBox::critical(0, QObject::tr("Notice"),
                                          "Update failed.\nError:"+updateQuery.lastError().text());
        SetDoctorCtlData(m_DrDataRow);
    }
    SetDoctorCtlEditable(true);
    model->select();    
    m_db.close();
    on_commandLinkButton_Doctor_clicked();
}

//PatientsSubmit
void UserManager::on_pushButton_PaCommit_clicked()
{
    QSqlDatabase m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName("D:/HIFU-V1.0/backups/database.db");
    m_db.setUserName("root");
    m_db.setPassword("123456");
    m_db.open();
    QDate date1 = QDate::fromString(ui->LineEdit_Birth->text(),"yyyy-M-d");
    QDate date2 = QDate::fromString(ui->LineEdit_Birth->text(),"yyyy/M/d");
    QDate date3 = QDate::fromString(ui->LineEdit_Birth->text(),"yyyy-MM-dd");
    if(date1.isValid())
    {
        ui->LineEdit_Birth->setText(date1.toString("yyyy-MM-dd"));
    }
    else if(date2.isValid())
    {
        ui->LineEdit_Birth->setText(date2.toString("yyyy-MM-dd"));
    }
    else if(date3.isValid())
    {
        ui->LineEdit_Birth->setText(date3.toString("yyyy-MM-dd"));
    }
    else if(ui->LineEdit_Birth->text()=="")
    {

    }
    else
    {
        QMessageBox::critical(0, QObject::tr("Notice"),"Date of birth format is invalid.");
        m_db.close();
        return;
    }
    PID = ui->LineEdit_PID->text();
    if(ui->lineEdit_PaName->text().isEmpty()||ui->lineEdit_PaSizeX->text().isEmpty()
            ||ui->lineEdit_PaSizeY->text().isEmpty()||ui->lineEdit_PaSizeZ->text().isEmpty())
    {
        QMessageBox::critical(0, QObject::tr("Notice"),"Name and fibroid size are required.");
        m_db.close();
        return;
    }
    if(ui->lineEdit_PaName->text().contains(" ")||ui->lineEdit_PaSizeX->text().contains(" ")
            ||ui->lineEdit_PaSizeY->text().contains(" ")||ui->lineEdit_PaSizeZ->text().contains(" "))
    {
        QMessageBox::critical(0, QObject::tr("Notice"),"Name and fibroid size cannot contain spaces.");
        m_db.close();
        return;
    }


    QString strSQl=QString("UPDATE PatientInfo SET pid='%1' ,showid='%2' ,name = '%3', birth='%4', age='%5',mobile='%6',size_x='%7',size_y='%8',size_z='%9',depth='%10',address='%11',doctor='%12' WHERE ID = %13")
            .arg(ui->LineEdit_PID->text())
            .arg(ui->lineEdit_SID->text())
            .arg(ui->lineEdit_PaName->text())
            .arg(ui->LineEdit_Birth->text())
            .arg(ui->lineEdit_PaAge->text())
            .arg(ui->lineEdit_PaMobile->text())
            .arg(ui->lineEdit_PaSizeX->text())
            .arg(ui->lineEdit_PaSizeY->text())
            .arg(ui->lineEdit_PaSizeZ->text())
            .arg(ui->lineEdit_PaDepth->text())
            .arg(ui->lineEdit_PaAddress->text())
            .arg(ui->LineEdit_Doctor->text())
            .arg(m_PaDataRow.id_str);
    qDebug()<<strSQl;
    QSqlQuery updateQuery(strSQl);
    if(updateQuery.exec())
    {
        QMessageBox::information(NULL,"Notice","Updated successfully.",QMessageBox::Ok);
        updateInfo(PID+","+ui->lineEdit_SID->text()+","+ui->lineEdit_PaName->text()+","+"Female"+","+
                   ui->LineEdit_Birth->text()+","+ui->lineEdit_PaAge->text()+","+
                   ui->lineEdit_PaMobile->text()+","+ui->lineEdit_PaDepth->text()
                   +","+ui->lineEdit_PaSizeX->text()+","+ui->lineEdit_PaSizeY->text()+","+ui->lineEdit_PaSizeZ->text()+","+
                   ui->lineEdit_PaAddress->text()+","+ui->LineEdit_Doctor->text());
        SetPatientCtlEditable(true);
        if(m_QDataType==TypePatient)
        {
            on_commandLinkButton_Patient_clicked();
        }
        m_PaDataRow.clear();
    }
    else
    {
        QMessageBox::critical(0, QObject::tr("Notice"),
                                          "Update failed.\nError:" + m_PaDataRow.id_str +"\n"+updateQuery.lastError().text());
        SetPatientCtlData(m_PaDataRow);
    }
    m_db.close();
}
//Delete用户
void UserManager::on_commandLinkButton_delete_clicked()
{
    QSqlDatabase m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName("D:/HIFU-V1.0/backups/database.db");
    m_db.setUserName("root");
    m_db.setPassword("123456");
    m_db.open();
    if(m_QDataType==TypePatient)
    {//DeletePatients
        if(m_PaDataRow.isClear())
        {
            QMessageBox::information(NULL,"Notice","Select an item to delete.",QMessageBox::Ok);
            m_db.close();
            return;
        }
        QMessageBox msgBox("Delete Information",
                           "Delete this patient record?\nPatient ID:"+m_PaDataRow.id_str+"\nPatient Name:"+m_PaDataRow.name,
                           QMessageBox::Critical,
                           QMessageBox::Yes | QMessageBox::Default,
                           QMessageBox::No | QMessageBox::Escape,
                           QMessageBox::NoButton);
        msgBox.setWindowFlags(Qt::WindowTitleHint|Qt::CustomizeWindowHint);
//        msgBox.setGeometry(QRect(1400,400,msgBox.width(),msgBox.height()));
        msgBox.setButtonText(QMessageBox::Yes, "OK");
        msgBox.setButtonText(QMessageBox::No, "Cancel");

        int ret = msgBox.exec();
        if (ret == QMessageBox::No)
        {
            m_db.close();
            return;
        }
//        int ensure=QMessageBox::warning(0, QObject::tr("Warning"),
//                                         "Delete this patient record?\nPatient ID:"+m_PaDataRow.id_str+"\nPatient Name:"+m_PaDataRow.name,
//                                         QMessageBox::Ok,QMessageBox::Cancel);
//        if(ensure==QMessageBox::Cancel)
//        {
//            return;
//        }
        QString strSQl=QString("DELETE FROM PatientInfo WHERE ID = %1")
                .arg(m_PaDataRow.id_str);
        qDebug()<<strSQl;
        QSqlQuery updateQuery(strSQl);
        if(updateQuery.exec())
        {
            QMessageBox::information(NULL,"Notice","Deleted successfully.",QMessageBox::Ok);
            model1->setTable("PatientInfo");
            model1->select();
            ui->lineEdit_SID->setText("");
            ui->lineEdit_PaAge->setText("");
            ui->lineEdit_PaName ->setText("");
            ui->lineEdit_PaGender->setText("");
            ui->lineEdit_PaMobile->setText("");
            ui->lineEdit_PaSizeX->setText("");
            ui->lineEdit_PaSizeY->setText("");
            ui->lineEdit_PaSizeZ->setText("");
            ui->lineEdit_PaSizeSring->setText("");
            ui->lineEdit_PaDepth->setText("");
            ui->lineEdit_PaAddress->setText("");
            ui->LineEdit_Doctor->setText("");
            ui->LineEdit_Birth->setText("");
            m_PaDataRow.clear();
            if(m_QDataType==TypePatient)
            {
                on_commandLinkButton_Patient_clicked();
            }
            SetPatientCtlEditable(true);
        }
        else
        {
            QMessageBox::critical(0, QObject::tr("Notice"),
                                              "Delete failed.\nError:"+updateQuery.lastError().text());
        }
    }
    else if(m_QDataType==TypeDoctor)
    {//DeletePhysicians
        if(m_DrDataRow.isClear())
        {
            QMessageBox::information(NULL,"Notice","Select an item to delete.",QMessageBox::Ok);
            m_db.close();
            return;
        }
        QMessageBox msgBox("Delete Information",
                           "Delete this physician record, including login information?\nPhysician ID:"+m_DrDataRow.id_str+"\nPhysician Name:"+m_DrDataRow.fullName,
                           QMessageBox::Critical,
                           QMessageBox::Yes | QMessageBox::Default,
                           QMessageBox::No | QMessageBox::Escape,
                           QMessageBox::NoButton);
        msgBox.setWindowFlags(Qt::WindowTitleHint|Qt::CustomizeWindowHint);
//        msgBox.setGeometry(QRect(1400,400,msgBox.width(),msgBox.height()));
        msgBox.setButtonText(QMessageBox::Yes, "OK");
        msgBox.setButtonText(QMessageBox::No, "Cancel");

        int ret = msgBox.exec();
        if (ret == QMessageBox::No)
        {
            m_db.close();
            return;
        }
//        int ensure=QMessageBox::warning(0, QObject::tr("Warning"),
//                                         "Delete this physician record, including login information?\nPhysician ID:"+m_DrDataRow.id_str+"\nPhysician Name:"+m_DrDataRow.fullName,
//                                         QMessageBox::Ok,QMessageBox::Cancel);
//        if(ensure==QMessageBox::Cancel)
//        {
//            return;
//        }
        QString strSQl=QString("DELETE FROM DrInfo WHERE ID = %1")
                .arg(m_DrDataRow.id_str);
        qDebug()<<strSQl;
        QSqlQuery updateQuery(strSQl);
        if(updateQuery.exec())
        {
            QString strSQl2=QString("DELETE FROM LoginInfo WHERE DrID = %1")
                    .arg(m_DrDataRow.id_str);
            if(updateQuery.exec(strSQl2))
            {
                QMessageBox::information(NULL,"Notice","Deleted successfully.",QMessageBox::Ok);
            }
            else
            {
                QMessageBox::information(NULL,"Notice",
                                         "Physician profile was deleted, but login information could not be deleted.\nError:"+updateQuery.lastError().text()
                                         ,QMessageBox::Ok);
            }
            model->setTable("DrInfo");
            model->select();
            m_db.close();
            on_commandLinkButton_Doctor_clicked();
        }
        else
        {
            QMessageBox::critical(0, QObject::tr("Notice"),
                                              "Delete failed.\nError:"+updateQuery.lastError().text());
            m_db.close();
        }
    }
}

void UserManager::on_commandLinkButton_quit_clicked()
{
    this->close();
    qApp->quit();
}

void UserManager::on_pushButton_DrLogInfoEdit_clicked()
{
    if(m_DrDataRow.isClear())
    {
        return;
    }
    LoginInfoEdit lie(0,m_DrDataRow.id);
    lie.exec();
}
//Start treatment
void UserManager::on_pushButton_PaTreatment_clicked()
{
    if(m_PaDataRow.isClear())
    {
        QMessageBox::critical(0, QObject::tr("Notice"),"No patient selected.");
        return;
    }

    QMessageBox msgBox("Start treatment",
                       "Start treatment for the selected patient?",
                       QMessageBox::Question,
                       QMessageBox::Yes | QMessageBox::Default,
                       QMessageBox::No | QMessageBox::Escape,
                       QMessageBox::NoButton);
    msgBox.setGeometry(QRect(1400,400,msgBox.width(),msgBox.height()));
    QFont font;
    font.setPixelSize(16);
    font.setFamily("Microsoft YaHei");
    msgBox.setFont(font);
    msgBox.setButtonText(QMessageBox::Yes, "OK");
    msgBox.setButtonText(QMessageBox::No, "Cancel");

    int ret = msgBox.exec();
    if (ret == QMessageBox::No)
    {
        return;
    }

    bool bCP=PatientManager::getInstance()->createPatientDir(QString("%1_%2").arg(m_PaDataRow.id_str).arg(m_PaDataRow.name));
    qDebug()<<"UserManager "<<"Patients目录创建:"<<bCP;

    emit SendPatientInfo(m_PaDataRow.pid,m_PaDataRow.name,m_PaDataRow.age_str,m_PaDataRow.mobile,m_PaDataRow.size_x_str,m_PaDataRow.size_y_str,m_PaDataRow.size_z_str,m_iPaOrientation);
    emit ShowMainWindow();
//    emit startdeepmeasur()
    DelayClose();
    //this->hide();
}

void UserManager::UpdateModel()
{
    QSqlDatabase m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName("D:/HIFU-V1.0/backups/database.db");
    m_db.setUserName("root");
    m_db.setPassword("123456");
    m_db.open();

    qDebug()<<"UpdateModel!!!!!!!";

    if(m_QDataType==TypePatient)
    {
        model1 = new CustomSqlTableModel(this);
        model1->setTable("PatientInfo");
        //设置ID倒序显示
        model1->setSort(model1->fieldIndex("id"), Qt::DescendingOrder);
        model1->select();
        while(model1->canFetchMore())
        {
            model1->fetchMore();
        }
        SetTableViewHeader(1);
        ui->tableView->setModel(model1);
        m_db.close();
    }
    else if(m_QDataType=TypeDoctor)
    {
        model = new QSqlTableModel(this);
        model->setTable("DrInfo");
        model->select();
        SetTableViewHeader(2);
        ui->tableView->setModel(model);
        m_db.close();
    }
}

void UserManager::DelayClose()
{
    delayTimer.setSingleShot(true);
    delayTimer.setInterval(500);
    delayTimer.start();
    connect(&delayTimer,SIGNAL(timeout()),this,SLOT(hide()));
}

bool UserManager::eventFilter(QObject *obj, QEvent *event)
{
    if (qobject_cast<QLabel*>(obj)== ui->label_Body_Orientation_Left) {
        if (event->type() == QEvent::MouseButtonRelease) {
            qDebug("Patients头部朝向左侧");
            ui->label_Body_Orientation_Left->setStyleSheet("background-color: rgb(0, 255, 255);image: url(:/umimg/UMImage/patient_oritention_left.png);");
            ui->label_Body_Orientation_Right->setStyleSheet("background-color: rgb(200, 200, 200);image: url(:/umimg/UMImage/patient_oritention_right.png);");
            m_iPaOrientation=0;
            if(!ui->pushButton_PaTreatment->isEnabled())
            {
                ui->pushButton_PaTreatment->setEnabled(true);
                ui->pushButton_PaTreatment->setStyleSheet(SELECTED);
            }
        }
        return true;
    }
    else if(qobject_cast<QLabel*>(obj) == ui->label_Body_Orientation_Right)
    {
        if (event->type() == QEvent::MouseButtonRelease) {
            qDebug("Patients头部朝向右侧");
            ui->label_Body_Orientation_Right->setStyleSheet("background-color: rgb(0, 255, 255);image: url(:/umimg/UMImage/patient_oritention_right.png);");
            ui->label_Body_Orientation_Left->setStyleSheet("background-color: rgb(200, 200, 200);image: url(:/umimg/UMImage/patient_oritention_left.png);");
            m_iPaOrientation=1;
            if(!ui->pushButton_PaTreatment->isEnabled())
            {
                ui->pushButton_PaTreatment->setEnabled(true);
                ui->pushButton_PaTreatment->setStyleSheet(SELECTED);
            }
        }
        return true;
    }
    else {
        return false;
        //return UserManager::eventFilter(obj, event);
    }
}
//Patients朝向Default朝左
void UserManager::showEvent(QShowEvent *event)
{
    ui->pushButton_PaTreatment->setEnabled(true);
    ui->pushButton_PaTreatment->setStyleSheet(SELECTED);
    ui->label_Body_Orientation_Left->setStyleSheet("background-color: rgb(0, 255, 255);image: url(:/umimg/UMImage/patient_oritention_left.png);");
    ui->label_Body_Orientation_Right->setStyleSheet("background-color: rgb(200, 200, 200);image: url(:/umimg/UMImage/patient_oritention_right.png);");
}

void UserManager::Newtreatment(int ID,QString PID,QString SID,QString Name,QString Gender,QString Birth,
                               int Age,QString Mobile,int Depth,int x,int y,int z,QString Address,QString Doctor)
{
    QModelIndex index;
    m_PaDataRow.SetData(ID,PID,SID,Name,Gender,Birth,Age,Mobile,Depth,x,y,z,Address,Doctor);
}

void UserManager::keyPressEvent(QKeyEvent *event)
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

void UserManager::updateInfoFile(QString name, QByteArray &log)
{
    QFile file(name);
    if (file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        QTextStream stream(&file);
        stream.setCodec("UTF-8");
        stream << log << endl;
        file.close();
    }
}

void UserManager::updateInfo(const QString &msg)
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
