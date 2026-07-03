#include "maindialog.h"
#include "ui_maindialog.h"
#include <QDebug>
#include <QDesktopWidget>
#include <QMessageBox>
#include <QMouseEvent>
#include <qprocess.h>
#include <qtextcodec.h>
#include <QFile>
#include <QTextStream>
#include <QDir>
#include <QTimer>
#include <QDateTime>
#include <QTableView>
#include <QStandardItemModel>
#include <QFile>
#include <QTextStream>
#include <QToolTip>
#include <QVector>

#define PI 3.1415916
//using namespace cv;

Q_LOGGING_CATEGORY(HIFUCONNECTION,"HIFUCONNECTION")

MainDialog::MainDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MainDialog)
//    _curdata(NULL),
//    _inImageData(NULL),
//    _inImageOutputPort(NULL),
//    _preProImageData(NULL),
//    _pPreProData(NULL),
//    _STLPolyData(NULL),
//    m_pPreProMask(NULL),
//    m_pPreProContour(NULL),
//    ContoursMaskArray(NULL),
//    RegionGrowImageDataArray(NULL)
    //RegionGrowImageArray_NewOri(NULL)
{
    ui->setupUi(this);

    self_echoscu();
    QTimer *time = new QTimer(this);
    connect(time,SIGNAL(timeout()),this,SLOT(self_echoscu()));
    time->start(30000);



    //设置表格格式
    ui->tableView->verticalHeader()->setDefaultAlignment(Qt::AlignCenter);
    ui->tableView->horizontalHeader()
                ->setStyleSheet("QHeaderView::section {color: black;font: 75 12pt \"微软雅黑\";border: 1px solid #6c6c6c;}");
    ui->tableView->verticalHeader()
                ->setStyleSheet("QHeaderView::section {color: black;font: 75 10pt \"微软雅黑\";border: 1px solid #6c6c6c;}");
    this->ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);  //单击Select一行
    this->ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection); //设置只能Select一行，不能多行选中
    this->ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);   //设置每行内容不可更改
    ui->tableView->setModel(model);

//    ui->tabWidget->setTabEnabled(0,false);

    //setWindowFlags(windowFlags() | Qt::Widget);
//    ui->pushButton_findseries->setEnabled(false);
//    ui->pushButton_findseries->setStyleSheet(NOT_WORK);

    ui->pushButton_nameconfirm->setEnabled(false);
    ui->pushButton_nameconfirm->setStyleSheet(NOT_WORK);

    ui->pushButton_movestudy->setEnabled(false);
    ui->pushButton_movestudy->setStyleSheet(NOT_WORK);

    ui->pushButton_input->setEnabled(false);
    ui->pushButton_input->setStyleSheet(NOT_WORK);

    ui->comboBox_date->setEnabled(false);
    ui->comboBox_series->setEnabled(false);

//    ui->pushButton_echoscu->setStyleSheet(SELECTED);

    ui->pushButton_findname->setStyleSheet(SELECTED);



}

MainDialog::~MainDialog()
{


    delete ui;
}

void MainDialog::setToolTip()
{
    //ui->label->setStyleSheet();
//    QPalette palette=QToolTip::palette();
//    palette.setColor(QPalette::Inactive,QPalette::ToolTipText,QColor(255,255,255,255));
//    QToolTip::setPalette(palette);
    //设置边框, 边框色, 背景色, 字体色, 字号
//    ui->spinBox_long_axis->setStyleSheet("QToolTip{border:1px solid black; background: white; color:black; font-size:12px;}");
//    ui->spinBox_short_axis->setStyleSheet("QToolTip{border:1px solid black; background: white; color:black; font-size:12px;}");
//    ui->spinBox_thick_axis->setStyleSheet("QToolTip{border:1px solid black; background: white; color:black; font-size:12px;}");
//    ui->spinBox_long_axis->setToolTip("Ellipse major-axis radius, max 105 mm");
//    ui->spinBox_short_axis->setToolTip("Ellipse minor-axis radius, max 105 mm");
//    ui->spinBox_thick_axis->setToolTip("Ellipse thickness radius, max 105 mm");

//    ui->lineEdit_inputname->setStyleSheet("QToolTip{border:1px solid black; background: white; color:black; font-size:12px;}");
//    ui->lineEdit_inputname->setToolTip("InputName为拼音时需要大写，且Name之间使用空格隔开！");

}

void MainDialog::SetTableViewHeader()
{
    model->setHorizontalHeaderItem(0,new QStandardItem(QObject::tr("Name")));
//    model->setHorizontalHeaderItem(1,new QStandardItem(QObject::tr("ID")));
//    model->setHeaderData(1,Qt::Horizontal,QObject::tr("ID"));
//    QStringList labels = QObject::trUtf8("Name,ID").simplified().split(",");
//    model->setHorizontalHeaderLabels(labels);

    this->ui->tableView->setColumnWidth(0,150);
//    this->ui->tableView->setColumnWidth(1,60);


}

void MainDialog::self_echoscu()
{
    QProcess echoscu(this);
    //路径位置（路径中需要使用双斜线！！！）
    QDir dirtemp;
    QString rootpath=dirtemp.currentPath();
    rootpath=rootpath.left(rootpath.indexOf(":")+1);//存储在当前项目的盘
    rootpath.append("/DuShuai/branch20210315/dependency/dcmtk/bin/");//Default的dicom图像存储地址
    QString path=rootpath;
    QDir dir(path);
//    if(path.isEmpty()||!dir.exists())
//    {
//        QMessageBox::warning(NULL,"Notice","Default image address not found.");
//        return;
//    }

//    qDebug()<<"Default地址："<<path;

    QString process = path+"echoscu.exe";
    QStringList parameter;
//    QString PC_name = QHostInfo::localHostName();
    QString AEtitle = "localhost";
    QString SCPport = "11110";
    QString ACME_STORE = "ACME_STORE";
    QString ACME = "ACME1";
    //Input参数
//    parameter << "--debug localhost 11110 -aec ACME_STORE -aet ACME1";
    //Input的参数如果含有空格则需要对单个字符串进行分离
    parameter << "-d" <<AEtitle <<SCPport <<"-aec"<<ACME_STORE<<"-aet"<< ACME;
    qDebug() << parameter;
    echoscu.start(process,parameter);
//    qDebug() << "echoscu";
//    if(echoscu.waitForFinished(100) == false)
//    {
    echoscu.waitForStarted();
    echoscu.waitForFinished();
//        qDebug()<<"1";
        QByteArray echooutput = echoscu.readAllStandardOutput();
//        qDebug() << QString::fromLocal8Bit(echooutput);
        if (!echooutput.isEmpty())
        {
            if (echooutput.contains("Received Echo Response (Success)"))
            {
                qDebug()<<"Connecting...";
                isechoscufailed = 1;
                ui->label_echoscustate->setText("Connecting...");
                return;
            }
            else {
                if(isechoscufailed)
                {
//                    QMessageBox::warning(NULL,"Notice","Connect to Server失败！");
                    isechoscufailed=0;
                    ui->label_echoscustate->setText("Connection lost.");
                    return;
                }
                else return;

            }

        }
}



//void MainDialog::on_pushButton_echoscu_clicked()
//{
//    QProcess echoscu(this);
//    //路径位置（路径中需要使用双斜线！！！）
//    QDir dirtemp;
//    QString rootpath=dirtemp.currentPath();
//    rootpath=rootpath.left(rootpath.indexOf(":")+1);//存储在当前项目的盘
//    rootpath.append("/SVN/dependency/dcmtk/bin/");//Default的dicom图像存储地址
//    QString path=rootpath;
//    QDir dir(path);
//    if(path.isEmpty()||!dir.exists())
//    {
//        QMessageBox::warning(NULL,"Notice","Default address not found.");
//        return;
//    }

//    qDebug()<<"Default地址："<<path;

//    QString process = path+"echoscu.exe";
//    QStringList parameter;
//    //QString PC_name = QHostInfo::localHostName();
//    QString AEtitle = "localhost";
//    QString SCPport = "11110";
//    QString ACME_STORE = "ACME_STORE";
//    QString ACME = "ACME1";
//    //Input参数
//    //parameter << "--debug localhost 11110 -aec ACME_STORE -aet ACME1";
//    //Input的参数如果含有空格则需要对单个字符串进行分离
//    parameter << "-d" <<AEtitle <<SCPport <<"-aec"<<ACME_STORE<<"-aet"<< ACME;
//    qDebug() << parameter;
//    echoscu.start(process,parameter);
////    qDebug() << "echoscu";
////    if(echoscu.waitForFinished(100) == false)
////    {
//    echoscu.waitForStarted();
//    echoscu.waitForFinished();
////        qDebug()<<"1";
//        QByteArray echooutput = echoscu.readAllStandardOutput();
//        qDebug() << QString::fromLocal8Bit(echooutput);
//        if (!echooutput.isEmpty())
//        {
//            if (echooutput.contains("Received Echo Response (Success)"))
//            {
//                QMessageBox::warning(NULL,"Notice","Connect to Server成功！");
//                return;
//            }
//            else {
//                QMessageBox::warning(NULL,"Notice","Connect to Server失败！");
//                return;
//            }

//        }
////    }
////        qDebug()<<"Complete";
//}

void MainDialog::on_pushButton_findname_clicked()
{
    SetTableViewHeader();
    //路径位置（路径中需要使用双斜线！！！）
    QDir dirtemp;
    QString rootpath=dirtemp.currentPath();
    rootpath=rootpath.left(rootpath.indexOf(":")+1);//存储在当前项目的盘
    rootpath.append("/DuShuai/branch20210315/dependency/dcmtk/bin/");//Default的dicom图像存储地址
    QString path=rootpath;
    QDir dir(path);
    if(path.isEmpty()||!dir.exists())
    {
        QMessageBox::warning(NULL,"Notice","Default address not found.");
        return;
    }

    qDebug()<<"Default地址："<<path;

    QString find_process = path+"findscu.exe";
    QStringList find_parameter;
    QString patientname = "PatientName="+ui->lineEdit_inputname->text();
    QString patientid = "PatientID";
    if(ui->lineEdit_inputname->text().indexOf(" ",0)==0)
    {
        QMessageBox::warning(NULL,"Notice","Name cannot start with a space.");
        return;
    }
    if(ui->lineEdit_inputname->text().isEmpty())
    {
        QMessageBox::warning(NULL,"Notice","Name is required.");
        return;
    }
    find_parameter << "-v"<<"-P"<<"localhost" <<"11110" <<"-aec"<<"ACME_STORE"<<"-aet"<< "ACME1"
                   <<"-k"<<"QueryRetrieveLevel=PATIENT"<<"-k"<<patientname<<"-k"<<patientid;
                  /*<<"-k"<<SeriesInstanceUID*/
    qDebug() << find_parameter;
    findscu.start(find_process,find_parameter);
    findscu.waitForStarted();
    findscu.waitForFinished();
    QByteArray find_output = findscu.readAllStandardOutput();
    qDebug() << QString::fromLocal8Bit(find_output);
    //清除内存，锁定按钮和下拉菜单
    model->clear();
    ui->comboBox_date->setCurrentIndex(-1);
    ui->comboBox_date->setEnabled(false);
    ui->comboBox_series->setCurrentIndex(-1);
    ui->comboBox_series->setEnabled(false);
    ui->pushButton_nameconfirm->setEnabled(false);
    ui->pushButton_nameconfirm->setStyleSheet(NOT_WORK);
    ui->pushButton_movestudy->setEnabled(false);
    ui->pushButton_movestudy->setStyleSheet(NOT_WORK);
    ui->pushButton_input->setEnabled(false);
    ui->pushButton_input->setStyleSheet(NOT_WORK);
    if(!PatientName.isEmpty() && !PatientID.isEmpty())
    {
        PatientName.clear();
        PatientID.clear();
    }
    if (!find_output.isEmpty())
    {
        if (find_output.contains("Find Response: 1 (Pending)"))
        {
            int Index_patientnamepath = find_output.indexOf("(0010,0010)");
            int Index_patientidpath = find_output.indexOf("(0010,0020)");
            int patientnum = 0;
            QVector<int> Index_patientname;
            QVector<int> Index_patientid;
            qDebug()<<"NAMEPATH"<<Index_patientnamepath<<"IDPATH"<<Index_patientidpath;

            while(Index_patientnamepath!=-1)//计算有多少个series
            {
                Index_patientnamepath = find_output.indexOf("(0010,0010)",Index_patientnamepath+1);
                Index_patientidpath = find_output.indexOf("(0010,0020)",Index_patientidpath+1);
                if(Index_patientnamepath!=-1 || Index_patientidpath!=-1)
                {
                    Index_patientname.push_back(Index_patientnamepath);
                    Index_patientid.push_back(Index_patientidpath);
                    patientnum++;
                }
            }
            for(int i=0;i<Index_patientname.size();i++)
            {
                int name_start = find_output.indexOf("[",Index_patientname[i]);
                int name_end = find_output.indexOf("]",Index_patientname[i]);
                int patientid_start = find_output.indexOf("[",Index_patientid[i]);
                int patientid_end = find_output.indexOf("]",Index_patientid[i]);
                PatientName.push_back(find_output.mid(name_start+1,name_end-name_start-1));
                PatientID.push_back(find_output.mid(patientid_start+1,patientid_end-patientid_start-1));
            }

            qDebug()<<"PatientName:"<<PatientName<<"ID"<<PatientID;
            for(int i=0;i<patientnum;i++)
            {
                qDebug()<<"NAME选项为："<<PatientName[i];
                qDebug()<<"ID选项为："<<PatientID[i];
                QStandardItem *itemname = new QStandardItem(PatientName[i]);
                model->setItem(i,0,itemname);
            }
            QMessageBox::warning(NULL,"Notice","Found.");
            return;
        }
        else
        {
            QMessageBox::warning(NULL,"Notice","Search failed.");
            return;
        }
    }
}




void MainDialog::on_pushButton_nameconfirm_clicked()
{
    QDir dirtemp;
    QString rootpath=dirtemp.currentPath();
    rootpath=rootpath.left(rootpath.indexOf(":")+1);//存储在当前项目的盘
    rootpath.append("/DuShuai/branch20210315/dependency/dcmtk/bin/");//Default的dicom图像存储地址
    QString path=rootpath;
    QDir dir(path);
    if(path.isEmpty()||!dir.exists())
    {
        QMessageBox::warning(NULL,"Notice","Default image address not found.");
        return;
    }

//    qDebug()<<"Default地址："<<path;

    QString find_process = path+"findscu.exe";
    QStringList find_parameter;
    QString patientid = "PatientID="+PatientID[patientnameid];
    patientidnum = patientid;
    find_parameter << "-v"<<"-S" <<"localhost" <<"11110" <<"-aec"<<"ACME_STORE"<<"-aet"<< "ACME1"
                   <<"-k"<<"QueryRetrieveLevel=STUDY"<<"-k"<<patientid<<"-k"<<"StudyDate"
                   <<"-k"<<"StudyInstanceUID";
    qDebug() << find_parameter;
    findscu.start(find_process,find_parameter);
    findscu.waitForStarted();
    findscu.waitForFinished();
    QByteArray find_output = findscu.readAllStandardOutput();
    //QByteArray findalloutput = findscu.readAllStandardError();
    qDebug() << QString::fromLocal8Bit(find_output);
    //qDebug() << QString::fromLocal8Bit(findalloutput);
    if(!StudyDate.isEmpty() && !StudyUID.isEmpty())
    {
        StudyDate.clear();
        StudyUID.clear();
    }


    if (!find_output.isEmpty())
    {
        if (find_output.contains("Find Response: 1 (Pending)"))
        {
            int Index_datepath = find_output.indexOf("(0008,0020)");//FindTime
            int Index_uidpath = find_output.indexOf("(0020,000d)");
            int Studynum = 0;
            QVector<int> Index_studydate;
            QVector<int> Index_studyuid;
            qDebug()<<"INDEX_DATEPATH"<<Index_datepath<<"UIDPATH"<<Index_uidpath;
            while(Index_datepath!=-1)//计算有多少个结果
            {
                 Index_datepath = find_output.indexOf("(0008,0020)",Index_datepath+1);
                 Index_uidpath = find_output.indexOf("(0020,000d)",Index_uidpath+1);
                 if(Index_datepath!=-1 && Index_uidpath!=-1)
                 {
                    Index_studydate.push_back(Index_datepath);
                    Index_studyuid.push_back(Index_uidpath);
                    Studynum++;
                 }
            }
//            qDebug()<<"数量："<<Series_num<<" 地址："<<Index_series;
            for(int i=0;i<Index_studydate.size();i++)
            {
                 int date_start = find_output.indexOf("[",Index_studydate[i]);
                 int date_end = find_output.indexOf("]",Index_studydate[i]);
                 int studyuid_start = find_output.indexOf("[",Index_studyuid[i]);
                 int studyuid_end = find_output.indexOf("]",Index_studyuid[i]);
                 StudyDate.push_back(find_output.mid(date_start+1,date_end-date_start-1));
                 StudyUID.push_back(find_output.mid(studyuid_start+1,studyuid_end-studyuid_start-1));

             }

             qDebug()<<"Studydate:"<<StudyDate<<"studyuid"<<StudyUID;
             ui->comboBox_date->clear();
             //ui->label_PatientName->setText(ui->lineEdit_inputname->text());
             for(int i=0;i<Studynum;i++)
             {

                 qDebug()<<"选项为："<<StudyDate[i];
                 ui->comboBox_date->addItem(StudyDate[i]);
             }
             //ui->movescu->setEnabled(true);
             ui->comboBox_date->setEnabled(true);
             ui->comboBox_date->setCurrentIndex(-1);

//             ui->comboBox_series->setCurrentIndex(-1);
//             ui->movescu->setEnabled(true);
//             ui->comboBox_series->setEnabled(true);
             if(find_output.contains("Received Final Find Response (Success)"))
             {
                 QMessageBox::warning(NULL,"Notice","Found.");
                 ui->pushButton_movestudy->setEnabled(true);
                 ui->pushButton_movestudy->setStyleSheet(SELECTED);
                 return;
             }

       }
       else {
            QMessageBox::warning(NULL,"Notice","Search failed.");
            return;
            }

    }
}

void MainDialog::on_tableView_clicked(const QModelIndex &index)
{
    int row = index.row();
    patientnameid = row;
    qDebug()<<"行数："<<patientnameid;
    ui->pushButton_nameconfirm->setEnabled(true);
    ui->pushButton_nameconfirm->setStyleSheet(SELECTED);

}

void MainDialog::on_pushButton_movestudy_clicked()
{
    if(ui->comboBox_date->currentText().isEmpty())
    {
        QMessageBox::warning(NULL,"Notice","Select a time.");
        return;
    }
    QProcess movescu(this);
    QString patient_id = patientidnum;
    //路径位置（路径中需要使用双斜线！！！）
    QDir dirtemp;
    QString root_path=dirtemp.currentPath();
    QString rootpath=root_path.left(root_path.indexOf(":")+1);//存储在当前项目的盘
    rootpath.append("/DuShuai/branch20210315/dependency/dcmtk/bin/");//Default的dicom图像存储地址
    QString path=rootpath;
    QDir dir(path);
    if(path.isEmpty()||!dir.exists())
    {
        QMessageBox::warning(NULL,"Notice","Default image address not found.");
        return;
    }

//    qDebug()<<"Default地址："<<path;

    QString move_process = path+"movescu.exe";
    QStringList move_parameter;
    QString AEtitle = "localhost";
    QString SCPport = "11110";
    QString ACME_STORE = "ACME_STORE";
    QString ACME = "ACME1";
    QString port ="1234";

    //新建文件夹存储下载的MRI images
    QDateTime time;
//    QDir dirtemp;
    QString str_time = time.currentDateTime().toString("yyMMdd_hhmmss");
    QString save_path = rootpath=root_path.left(root_path.indexOf(":")+1);//存储在当前项目的盘"D:/dicom/move/";//存储地址
    save_path.append("/dicom/"+PatientName[patientnameid]+"temp"+"/");
    save_path.append(str_time);
    QDir movestudy_dir;
    QString movestudy_path = save_path;
    movestudy_dir.mkpath(movestudy_path);
    QString SavePath = movestudy_path;
    QString Studydate = "StudyDate="+ui->comboBox_date->currentText();
    qDebug()<<"UID is："<<ui->comboBox_date->currentText();
    //Input的参数如果含有空格则需要对单个字符串进行分离
    move_parameter << "-v" <<"-S"<<AEtitle <<SCPport <<"-aec"<<ACME_STORE<<"-aet"<<
                      ACME<<"-aem"<<ACME<<"--port"<<port<<"-od"<<SavePath<<"-k"<<
                      "QueryRetrieveLevel=STUDY"<<"-k"<<patient_id<<"-k"<<
                      Studydate;
    qDebug() << move_parameter;
    movescu.start(move_process,move_parameter);
    qDebug() << "movescu";
    movescu.waitForStarted();
    movescu.waitForFinished();
    qDebug()<<"1";
    QByteArray moveoutput = movescu.readAllStandardOutput();
    qDebug() << QString::fromLocal8Bit(moveoutput);
    if (!moveoutput.isEmpty())
    {
        if (moveoutput.contains("Received Final Move Response (Success)"))
        {
            QMessageBox::warning(NULL,"Notice","Download complete. Saved to"+SavePath);
        }
        else {
            QMessageBox::warning(NULL,"Notice","Download failed.");
            return;
        }
    }
    //把同一study图像下载到指定路径
    QString move_path = save_path;
    QDir move_dir(move_path);
    if (move_path.isEmpty() || !move_dir.exists())
        return;
    move_dir.setFilter(QDir::Files | QDir::NoSymLinks);
    QFileInfoList dicomlist = move_dir.entryInfoList();
    QStringList string_list;
    string_list.clear();
    QTextCodec *code = QTextCodec::codecForName("GB2312");//解决中文路径问题
    for (int i = 0; i < dicomlist.count(); i++)
    {
        QFileInfo file_info = dicomlist.at(i);
        QString absolute_file_path = file_info.absoluteFilePath();
        string_list.append(absolute_file_path);
    }
    qDebug()<<"MRI images数量："<<dicomlist.count();
    if (string_list.isEmpty())
    {
        QMessageBox::warning(NULL, QStringLiteral("Notice"), QStringLiteral("No readable MRI images found."));
        return;
    }
    dcm_list = string_list;
    if(!SeriesUID.isEmpty() && !Description.isEmpty())
    {
        SeriesUID.clear();
        Description.clear();
    }
    QString fileName;
    int series_num=0;
    int size = string_list.count();
    for (int i = 0; i < size; ++i)
    {
        fileName = string_list[i];
        qDebug() << "file name is " << string_list[i];
        gdcm::Reader reader;
        qDebug()<<"gdcm:";
        QString name = code->fromUnicode(fileName).data();
        reader.SetFileName(name.toStdString().c_str());
        if (!reader.Read())
        {
            qDebug() << "Could not read: " << fileName;
            continue;
        }
        gdcm::File &file = reader.GetFile();
        gdcm::DataSet &ds = file.GetDataSet();
        if (ds.FindDataElement(gdcm::Tag(0x0020, 0x000E)))
        {
            gdcm::Attribute<0x0020, 0x000E> atSeriesUID;
            atSeriesUID.Set(ds);
            gdcm::LOComp gdcmSeriesUID = atSeriesUID.GetValue();
            QString series_uid = QString(gdcmSeriesUID.c_str());
            if(SeriesUID.isEmpty())
            {
                SeriesUID.push_back(series_uid);
                isadd_seriesuid = 1;
            }
            else
            {
                qDebug()<<"uid_size"<<SeriesUID.size();
                if(SeriesUID.contains(series_uid))
                {
                    continue;
                }
                else
                {
                    SeriesUID.push_back(series_uid);
                    qDebug()<<"SeriesUID[series_num]"<<SeriesUID[series_num];
                    isadd_seriesuid = 1;
                }
            }
        }
        else
        {
            continue;
        }
        if (ds.FindDataElement(gdcm::Tag(0x0008, 0x103E)))
        {
            gdcm::Attribute<0x0008, 0x103E> atDescription;
            atDescription.Set(ds);
            QString description = (QString)atDescription.GetValue();
            qDebug()<<"DESCRIPTION"<<description;
            if(Description.isEmpty())
            {
                Description.push_back(description);
                isadd_description = 1;
            }
            else
            {
                if(Description.contains(description))
                {
                    continue;
                }
                else
                {
                    Description.push_back(description);
                    qDebug()<<"Description[description]"<<Description[series_num];
                    isadd_description = 1;
                }
            }
        }
        else
        {
            continue;
        }
        if(isadd_description && isadd_seriesuid)
        {
            series_num++;
            qDebug()<<"series_num:"<<series_num;
        }
    }

        //Adddescription
        ui->comboBox_series->clear();
        for(int i=0;i<Description.count();i++)
        {

            qDebug()<<"选项为："<<Description[i];
            ui->comboBox_series->addItem(Description[i]);
            ui->comboBox_series->setEnabled(true);
        }
        ui->pushButton_input->setEnabled(true);
        ui->pushButton_input->setStyleSheet(SELECTED);
}

void MainDialog::on_pushButton_input_clicked()
{
    if(ui->comboBox_series->currentText().isEmpty())
    {
        QMessageBox::warning(NULL,"Notice","Select a series.");
        return;
    }
    QString str_name = ui->comboBox_series->currentText();
    QDir dirtemp;
    QString root_path=dirtemp.currentPath();
    QString save_path = root_path.left(root_path.indexOf(":")+1);//存储在当前项目的盘;
    save_path.append("/dicom/"+PatientName[patientnameid]+"/");
    save_path.append(str_name);
    QDir dir;
    QString path = save_path;
    qDebug()<<"存储路径："<<path;
    QTextCodec *code = QTextCodec::codecForName("GB2312");//解决中文路径问题

    if(!dir.exists(path))
    {
        dir.mkpath(path);
//        QMessageBox::warning(NULL,"Notice","路径为空，已新建文件夹！");

    }
    QString fileName;
    int size = dcm_list.count();
    qDebug()<<"size"<<size;
    int num = 0;
    for (int i = 0; i < size; ++i)
    {
        // GDCM获取tag
        fileName = dcm_list[i];
        QFile dcmfile(fileName);

        qDebug() << "file name is " << dcm_list[i];
        gdcm::Reader reader;
        qDebug()<<"gdcm:";
        QString name = code->fromUnicode(fileName).data();
        reader.SetFileName(name.toStdString().c_str());
        if (!reader.Read())
        {
            qDebug() << "Could not read: " << fileName;
            continue;
        }
        gdcm::File &file = reader.GetFile();
        gdcm::DataSet &ds = file.GetDataSet();

        if (ds.FindDataElement(gdcm::Tag(0x0008, 0x103E)))
        {
            gdcm::Attribute<0x0008, 0x103E> atDescription;
            atDescription.Set(ds);
            QString description = (QString)atDescription.GetValue();
            qDebug()<<"DESCRIPTION"<<description;

            if(ui->comboBox_series->currentText()==description)
            {
                QString dcmfilename = path+"/MR";
                QString str_num = QString::number(num,10);
                dcmfilename.append(str_num);

                qDebug()<<"File Path和文件名："<<dcmfilename;
                dcmfile.copy(dcmfilename);
                num++;

                qDebug()<<"复制成功："<<dcmfilename;
            }
        }
    }
    QMessageBox::warning(NULL,"Notice","Imported successfully.");
}
