#include "filesavedialog.h"
#include "ui_filesavedialog.h"
#include <QFileDialog>
#include <QMessageBox>

#include <qtextcodec.h>

#include "qdebug.h"

#include "iostream"
#if _MSC_VER >1600
#pragma execution_character_set("utf-8")
#endif

FileSaveDialog::FileSaveDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FileSaveDialog)
{
    ui->setupUi(this);
//    _patientmanager =PatientManager::getInstance();
    this->setGeometry(150,380,this->width(),this->height());
    //connect(this,SIGNAL(fullpath1(QString)),_patientmanager,SLOT(getfullpath(QString)));
//    connect(this,SIGNAL(fullpath(QString)),_patientmanager,SLOT(getfullpath(QString)));
}

FileSaveDialog::FileSaveDialog(PAFILETYPE filetype, QWidget *parent):
    QDialog(parent),
    ui(new Ui::FileSaveDialog),
    m_FileType(filetype)
{
    ui->setupUi(this);
    this->setGeometry(150,380,this->width(),this->height());
    if(m_FileType==PA_IMAGE){
        this->setWindowTitle(QString("Save Image"));
        ui->label_fileNameSuffix->setText(".bmp");
        sPathDefault=PatientManager::paImageDir_default;
        pFilePath=&PatientManager::paImageDir;
        pFileName=&PatientManager::paImageName;
    }
    else if(m_FileType==PA_VIDEO){
        this->setWindowTitle(QString("Save Video"));
        ui->label_fileNameSuffix->setText(".avi");
        sPathDefault=PatientManager::paVideoDir_default;
        pFilePath=&PatientManager::paVideoDir;
        pFileName=&PatientManager::paVideoName;
    }
    else if(m_FileType==PA_REPORT){
        this->setWindowTitle(QString("Save Report"));
        ui->label_fileNameSuffix->setText(".pdf");
        sPathDefault=PatientManager::paReportDir_default;
        pFilePath=&PatientManager::paReportDir;
        pFileName=&PatientManager::paReportName;
    }
    ui->checkBox_defaultFilePath->setChecked(true);
    ui->checkBox_defaultFileName->setChecked(true);
    on_checkBox_defaultFilePath_clicked(true);
    on_checkBox_defaultFileName_clicked(true);
    updateFullPath();
}

FileSaveDialog::~FileSaveDialog()
{
    delete ui;
}

void FileSaveDialog::on_checkBox_defaultFilePath_clicked(bool checked)
{
    if(checked){
        ui->lineEdit_filePath->setText(sPathDefault);
        (*pFilePath)=sPathDefault;
    }
    updateFullPath();
}

void FileSaveDialog::on_checkBox_defaultFileName_clicked(bool checked)
{
    if(checked){
        ui->lineEdit_fileName->setText("");
        ui->lineEdit_fileName->setPlaceholderText(PatientManager::getInstance()->getPatientId()
                                                  +"-"+PatientManager::getInstance()->getTimeName());
        (*pFileName)=QString();
    }
    updateFullPath();
}

void FileSaveDialog::on_lineEdit_fileName_textChanged(const QString &arg1)
{
    if(!arg1.isEmpty()){
        (*pFileName)=arg1;
        ui->checkBox_defaultFileName->setChecked(false);
    }
    else{
        ui->checkBox_defaultFileName->setChecked(true);
    }
    updateFullPath();
}

void FileSaveDialog::on_pushButton_selectFilePath_clicked()
{
    QFileDialog pathDlg;
    pathDlg.setViewMode(QFileDialog::Detail);
    QDir dirtemp;
    pathDlg.setDirectory(dirtemp.rootPath());
    QString path=pathDlg.getExistingDirectory(this);
    path.append("/");
    QDir dir(path);
    if(path.isEmpty()||!dir.exists()){
        ui->checkBox_defaultFilePath->setChecked(true);
    }
    else{
        ui->checkBox_defaultFilePath->setChecked(false);
        ui->lineEdit_filePath->setText(path);
        (*pFilePath)=path;
    }
    updateFullPath();
}

void FileSaveDialog::updateFullPath()
{
    sFPath.clear();
    if(ui->lineEdit_filePath->text().isEmpty()||ui->lineEdit_filePath->text()=="/"){
        sFPath.append(sPathDefault);
        ui->lineEdit_filePath->setText(sPathDefault);
    }else{
        sFPath.append(ui->lineEdit_filePath->text());
    }

    if(ui->lineEdit_fileName->text().isEmpty()){
        sFPath.append(ui->lineEdit_fileName->placeholderText());
    }
    else{
        sFPath.append(ui->lineEdit_fileName->text());
    }
    sFPath.append(ui->label_fileNameSuffix->text());
    ui->textBrowser_fullPath->clear();
    ui->textBrowser_fullPath->setText(sFPath);
}

void FileSaveDialog::on_pushButton_Accepted_clicked()
{
//    QTextCodec *code = QTextCodec::codecForName("GB2312");//解决中文路径问题
//    std::string str = code->fromUnicode(*pFileName).data();

//    for(int i = 0 ; i < str.size() ; i++)
//    {
//        char cha = str.at(i);
//        if(
//                ((cha>='0')&&(cha<='9'))
//                ||((cha>='A')&&(cha<='Z'))
//                ||((cha>='a')&&(cha<='z'))
//                ||(cha=='-')
//                ||(cha=='_')
//                ||(cha=='(')
//                ||(cha==')')
//                ||(cha=='+')
//                ||(cha=='{')
//                ||(cha=='}')
//                ||(cha=='[')
//                ||(cha==']'))
//        {
//        }
//        else if (i<str.size()-1)
//        {
//            char next = str.at(i+1);
//            if ((cha&0x80)&&(next&0x80))
//            {
//                qDebug()<<"get chinese character !";
//                ++i;
//            }
//            else
//            {
//                QMessageBox::warning(0,"文件名错误","文件名不能含有特殊字符。\n"
//                                               "只能包含英文和字符 - _ ( ) + { } [ ]");
//                return;
//            }


//        }
//        else{
//            QMessageBox::warning(0,"文件名错误","文件名不能含有特殊字符。\n"
//                                           "只能包含英文和字符 - _ ( ) + { } [ ]");
//            return;
//        }

//    }

//    // QTextCodec *code = QTextCodec::codecForName("GB2312");//解决中文路径问题
//    std::string name2 = code->fromUnicode(*pFilePath).data();

//    QString str2 = QString(name2.c_str());
//    int str2Count = str2.count();
//    for(int i = 0 ; i < str2Count ; i++)
//    {
//        QChar cha = str2.at(i);
//        ushort uni = cha.unicode();
//        if(uni >= 0x4E00 && uni <= 0x9FA5)
//        {
//            QMessageBox::warning(0,"File Path错误",QString("Select的路径中不能包含中文 %1").arg(cha));
//            return;
//        }
//    }
//    qDebug()<<"before!"<<sFPath;


    PatientManager::getInstance()->SetFullPath(sFPath);

    this->accept();
}

void FileSaveDialog::on_pushButton_Cancel_clicked()
{
//    on_checkBox_defaultFileName_clicked(true);
    this->reject();
}

void FileSaveDialog::on_lineEdit_filePath_textChanged(const QString &arg1)
{
    if(!arg1.isEmpty()){
        (*pFilePath)=arg1;
        ui->checkBox_defaultFilePath->setChecked(false);
    }
    else{
        ui->checkBox_defaultFilePath->setChecked(true);
    }
    updateFullPath();
}
