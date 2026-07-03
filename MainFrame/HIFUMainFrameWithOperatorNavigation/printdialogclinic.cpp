#include "printdialogclinic.h"
#include "ui_printdialogclinic.h"
#include <QPixmap>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QMessageBox>
#include "filesavedialog.h"

#include "qprinterinfo.h"
#if _MSC_VER >1600
#pragma execution_character_set("utf-8")
#endif

PrintDialogClinic::PrintDialogClinic(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PrintDialogClinic)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);

    printer=new QPrinter();
    preview = new QPrintPreviewWidget (printer,this);

    this->ui->verticalLayout_printview->addWidget(preview);
    connect(preview, SIGNAL(paintRequested(QPrinter*)),this,SLOT(printPreview(QPrinter *)));//关联Print预览的内容
    preview->show();
    //preview->setZoomMode(QPrintPreviewWidget::FitToWidth);

    QList<QPrinterInfo> list = QPrinterInfo::availablePrinters();
    qDebug()<<"print info "<< list.size();

    QString info;
    foreach(QPrinterInfo printerInfo, list)
    {
        info.append(printerInfo.printerName().toStdString().c_str());
        info.append(" ");
    }

    qDebug()<<"print info "<<info;
}

PrintDialogClinic::~PrintDialogClinic()
{
    delete preview;
    delete printer;
    delete ui;
}
//Print
void PrintDialogClinic::on_pushButton_print_clicked()
{
//    QTextDocument *ted = new QTextDocument;
//    ted->setHtml(getHtmlStr(check1,check2));
//    ted->print(printer);
    qDebug()<<"before print !";
    preview->print();
    qDebug()<<"after print !";
    SaveReportInfoToDB();
}


//预览
void PrintDialogClinic::printPreview(QPrinter *p)
{
    QTextDocument *ted = new QTextDocument;
    ted->setHtml(getHtmlStr(check1,check2));
    ted->print(p);
    delete ted;
}
//设置Patient information槽
void PrintDialogClinic::setPatientInfo(QVariant id, QVariant fname, QVariant age,
                                 QVariant mobile, QVariant x, QVariant y, QVariant z)
{

//    ui->lineEdit_group_number->setText(id.toString());
    ui->lineEdit_name->setText(fname.toString());

    preview->updatePreview();

}

//设置Treatment信息槽
void PrintDialogClinic::setTerapyInfo(QVariant rate, QVariant startTime, QVariant endTime,
                                QVariant number, QVariant totalTime, QVariant deep,
                                QVariant doctor, QVariant pulse,QVariant volt)
{
    // ui->lineEdit_melt->setText(rate.toString());
    ui->lineEdit_btime->setText(startTime.toString());
    ui->lineEdit_etime->setText(endTime.toString());
    ui->lineEdit_irradiateSpotNumber->setText(number.toString());

    int total = totalTime.toInt();
    int hour = total/3600;
    int min = (total - hour*3600)/60;
    int second = total - 3600*hour-60*min;

    QString timeLabel;
    if (hour<10)
    {
        timeLabel.append('0');
    }
    timeLabel.append(QString::number(hour));
    timeLabel.append(":");

    if (min<10)
    {
        timeLabel.append('0');
    }
    timeLabel.append(QString::number(min));
    timeLabel.append(":");

    if (second<10)
    {
        timeLabel.append('0');
    }
    timeLabel.append(QString::number(second));

    ui->lineEdit_total_irradiate_time->setText(timeLabel);

//    ui->lineEdit_deep->setText(deep.toString());
    ui->lineEdit_doctor->setText(doctor.toString());
    ui->lineEdit_pulseNumber->setText(pulse.toString());
    ui->lineEdit_date->setText(QDateTime::currentDateTime().toString("yyyy-MM-dd"));
    ui->lineEdit_irradiateTotalPower->setText(volt.toString());
    preview->updatePreview();
}

void PrintDialogClinic::SaveReportInfoToDB()
{
    QSqlDatabase m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName("D:/HIFU-V1.0/backups/database.db");
    m_db.setUserName("root");
    m_db.setPassword("123456");
    m_db.open();
    qDebug()<<"SaveReportInfoToDB";
    QString strSQl=QString("INSERT INTO Report (pid,CreateTime,melt,stime,etime,conclusion,iSpotNumber,iVoltage,deep,pulseNumber,doctor) VALUES ('%1','%2','%3','%4','%5','%6','%7','%8','%9','%10','%11')")
            .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss"))
            .arg(ui->lineEdit_btime->text())
            .arg(ui->lineEdit_etime->text())
            .arg(ui->lineEdit_irradiateSpotNumber->text())
            .arg(ui->lineEdit_deep->text())
            .arg(ui->lineEdit_pulseNumber->text())
            .arg(ui->lineEdit_doctor->text());
    qDebug()<<strSQl;
    QSqlQuery updateQuery;
    if(updateQuery.exec(strSQl))
    {
        QMessageBox::information(NULL,"Notice","Treatment report imported into the database.",QMessageBox::Ok);
    }
    else
    {
        QMessageBox::critical(0, QObject::tr("Notice"),
                                          "Treatment report database import failed. Error:"+updateQuery.lastError().text());
    }
    m_db.close();
}

//Generate文档样式html
QString PrintDialogClinic::getHtmlStr(QString check1,QString check2)
{
    m_strPeintHtml=QString("<p align=\"center\">\
            <span style=\"font-size: 24px;\"><strong><span style=\"font-family: 微软雅黑,Microsoft YaHei;\">Zhonghui Medical HIFU Treatment Report</span></strong></span><br/><br/><br/><br/><br/>\
        </p>\
        <p>\
            <span style=\"font-size: 20px;\"><strong><span style=\"font-family: 微软雅黑,Microsoft YaHei;\">Patient information</span></strong></span><br/>\
        </p>\
        <hr/>\
        <table width=\"595\">\
            <tbody>\
                <tr class=\"firstRow\">\
                    <td style=\"border-width: 1px; border-style: solid;\" width=\"261\" valign=\"top\">\
                        <span style=\"font-family: 微软雅黑,Microsoft YaHei; font-size: 18px;\">Enrollment ID:%1<br/></span>\
                    </td>\
                    <td style=\"border-width: 1px; border-style: solid;\" width=\"261\" valign=\"top\">\
                        <span style=\"font-family: 微软雅黑,Microsoft YaHei; font-size: 18px;\">Name:%2<br/></span>\
                    </td>\
                </tr>\
                <tr>\
                    <td width=\"402\" valign=\"top\">\
                        <span style=\"font-family: 微软雅黑,Microsoft YaHei; font-size: 18px;\">Start time:%3<br/></span>\
                    </td>\
                    <td width=\"402\" valign=\"top\">\
                        <span style=\"font-family: 微软雅黑,Microsoft YaHei; font-size: 18px;\">End time:%4</span><br/>\
                    </td>\
                </tr>\
                <tr>\
                    <td width=\"402\" valign=\"top\">\
                        <span style=\"font-family: 微软雅黑,Microsoft YaHei; font-size: 18px;\">Total Sonication Time:%5<br/></span>\
                    </td>\
                    <td width=\"402\" valign=\"top\">\
                        <span style=\"font-family: 微软雅黑,Microsoft YaHei; font-size: 18px;\">Effective Treatment Time:%16<br/></span>\
                    </td>\
                </tr>\
            </tbody>\
        </table>\
        <hr/><br/><br/><br/>\
        <p>\
            <strong><span style=\"font-family: 微软雅黑,Microsoft YaHei; font-size: 20px;\">Treatment Parameters</span></strong><br/>\
        </p>\
        <hr/>\
        <table width=\"595\">\
            <tbody>\
                <tr class=\"firstRow\">\
                    <td width=\"250\" valign=\"top\">\
                        <span style=\"font-family: 微软雅黑,Microsoft YaHei; font-size: 18px;\">sonication point个数(pcs)：%6 <br/></span>\
                    </td>\
                    <td width=\"402\" valign=\"top\">\
                        <span style=\"font-family: 微软雅黑,Microsoft YaHei; font-size: 18px;\">Treatment depth(cm)：%7 <br/></span>\
                    </td>\
                </tr>\
                <tr>\
                    <td width=\"250\" valign=\"top\">\
                        <span style=\"font-family: 微软雅黑,Microsoft YaHei; font-size: 18px;\">Pulse Count:%8 </span><br/>\
                    </td>\
                    <td width=\"250\" valign=\"top\">\
                        <span style=\"font-family: 微软雅黑,Microsoft YaHei; font-size: 18px;\">Total Treatment Points:%9 </span><br/>\
                    </td>\
                </tr>\
                <tr>\
                    <td width=\"250\" valign=\"top\">\
                        <span style=\"font-family: 微软雅黑,Microsoft YaHei; font-size: 18px;\">Sonication Voltage(V)：%10<br/></span>\
                    </td>\
                    <td width=\"402\" valign=\"top\">\
                        <span style=\"font-family: 微软雅黑,Microsoft YaHei; font-size: 18px;\">Total Sonication Power (W):%11</span><br/>\
                    </td>\
                </tr>\
                <tr>\
                    <td width=\"250\" valign=\"top\">\
                        <span style=\"font-family: 微软雅黑,Microsoft YaHei; font-size: 18px;\">Thermal Injury:%12<br/></span>\
                    </td>\
                    <td width=\"402\" valign=\"top\">\
                        <span style=\"font-family: 微软雅黑,Microsoft YaHei; font-size: 18px;\">Mechanical Injury:%13</span><br/>\
                    </td>\
                </tr>\
            </tbody>\
        </table>\
        <hr/><br/><br/><br/>\
        <p>\
            <strong><span style=\"font-size: 20px; font-family: 微软雅黑,Microsoft YaHei;\">Attending Physician:</span></strong><span style=\"font-size: 20px; font-family: 微软雅黑,Microsoft YaHei;\">%14</span><br/><br/><br/>\
        </p>\
        <p>\
            <strong><span style=\"font-size: 20px; font-family: 微软雅黑,Microsoft YaHei;\">Treatment Date:</span></strong><span style=\"font-size: 20px; font-family: 微软雅黑,Microsoft YaHei;\">%15</span><br/>\
        </p>")
            .arg(ui->lineEdit_group_number->text())
            .arg(ui->lineEdit_name->text())
            .arg(ui->lineEdit_btime->text())
            .arg(ui->lineEdit_etime->text())
            .arg(ui->lineEdit_total_irradiate_time->text())
            .arg(ui->lineEdit_irradiateSpotNumber->text())
            .arg(ui->lineEdit_deep->text())
            .arg(ui->lineEdit_pulseNumber->text())
            .arg(ui->lineEdit_allirradiateSpotNumber->text())
            .arg(ui->lineEdit_irradiate_voltage->text())
            .arg(ui->lineEdit_irradiateTotalPower->text())
            .arg(check1)
            .arg(check2)
            .arg("")
            .arg("")
            .arg(ui->lineEdit_effective_time->text());
//            qDebug()<<check1<<"2"<<check2;
            return m_strPeintHtml;
//    m_strPeintHtml=QString("<p align=\"center\">\
//            <span style=\"font-size: 24px;\"><strong><span style=\"font-family: 微软雅黑,Microsoft YaHei;\">Zhonghui Medical HIFU Treatment Report</span></strong></span><br/><br/><br/><br/><br/>\
//        </p>\
//        <p>\
//            <span style=\"font-size: 20px;\"><strong><span style=\"font-family: 微软雅黑,Microsoft YaHei;\">Patient information</span></strong></span><br/>\
//        </p>\
//        <hr/>\
//        <table width=\"595\">\
//            <tbody>\
//                <tr class=\"firstRow\">\
//                    <td style=\"border-width: 1px; border-style: solid;\" width=\"261\" valign=\"top\">\
//                        <span style=\"font-family: 微软雅黑,Microsoft YaHei; font-size: 18px;\">Enrollment ID:%1<br/></span>\
//                    </td>\
//                    <td style=\"border-width: 1px; border-style: solid;\" width=\"261\" valign=\"top\">\
//                        <span style=\"font-family: 微软雅黑,Microsoft YaHei; font-size: 18px;\">Name:%2<br/></span>\
//                    </td>\
//                </tr>\
//                <tr>\
//                    <td width=\"402\" valign=\"top\">\
//                        <span style=\"font-family: 微软雅黑,Microsoft YaHei; font-size: 18px;\">Start time:%3<br/></span>\
//                    </td>\
//                    <td width=\"402\" valign=\"top\">\
//                        <span style=\"font-family: 微软雅黑,Microsoft YaHei; font-size: 18px;\">End time:%4</span><br/>\
//                    </td>\
//                </tr>\
//                <tr>\
//                    <td width=\"402\" valign=\"top\">\
//                        <span style=\"font-family: 微软雅黑,Microsoft YaHei; font-size: 18px;\">Total Sonication Time:%5<br/></span>\
//                    </td>\
//                </tr>\
//            </tbody>\
//        </table>\
//        <hr/><br/><br/><br/>\
//        <p>\
//            <strong><span style=\"font-family: 微软雅黑,Microsoft YaHei; font-size: 20px;\">Treatment Parameters</span></strong><br/>\
//        </p>\
//        <hr/>\
//        <table width=\"595\">\
//            <tbody>\
//                <tr class=\"firstRow\">\
//                    <td width=\"250\" valign=\"top\">\
//                        <span style=\"font-family: 微软雅黑,Microsoft YaHei; font-size: 18px;\">sonication point个数(pcs)：%6 <br/></span>\
//                    </td>\
//                    <td width=\"402\" valign=\"top\">\
//                        <span style=\"font-family: 微软雅黑,Microsoft YaHei; font-size: 18px;\">Treatment depth(cm)：%7 <br/></span>\
//                    </td>\
//                </tr>\
//                <tr>\
//                    <td width=\"250\" valign=\"top\">\
//                        <span style=\"font-family: 微软雅黑,Microsoft YaHei; font-size: 18px;\">Pulse Count:%8 </span><br/>\
//                    </td>\
//                </tr>\
//                <tr>\
//                    <td width=\"250\" valign=\"top\">\
//                        <span style=\"font-family: 微软雅黑,Microsoft YaHei; font-size: 18px;\">Sonication Voltage(V)：%9<br/></span>\
//                    </td>\
//                    <td width=\"402\" valign=\"top\">\
//                        <span style=\"font-family: 微软雅黑,Microsoft YaHei; font-size: 18px;\">Total Sonication Power (W):%10</span><br/>\
//                    </td>\
//                </tr>\
//            </tbody>\
//        </table>\
//        <hr/><br/><br/><br/>\
//        <p>\
//            <strong><span style=\"font-size: 20px; font-family: 微软雅黑,Microsoft YaHei;\">Attending Physician:</span></strong><span style=\"font-size: 20px; font-family: 微软雅黑,Microsoft YaHei;\">%11</span><br/><br/><br/>\
//        </p>\
//        <p>\
//            <strong><span style=\"font-size: 20px; font-family: 微软雅黑,Microsoft YaHei;\">Treatment Date:</span></strong><span style=\"font-size: 20px; font-family: 微软雅黑,Microsoft YaHei;\">%12</span><br/>\
//        </p>")
//            .arg(ui->lineEdit_group_number->text())
//            .arg(ui->lineEdit_name->text())
//            .arg(ui->lineEdit_btime->text())
//            .arg(ui->lineEdit_etime->text())
//            .arg(ui->lineEdit_total_irradiate_time->text())
//            .arg(ui->lineEdit_irradiateSpotNumber->text())
//            .arg(ui->lineEdit_deep->text())
//            .arg(ui->lineEdit_pulseNumber->text())
//            .arg(ui->lineEdit_irradiate_voltage->text())
//            .arg(ui->lineEdit_irradiateTotalPower->text())
//            .arg(ui->lineEdit_doctor->text())
//            .arg(ui->lineEdit_date->text());
//            return m_strPeintHtml;
}


void PrintDialogClinic::on_textEdit_conclusion_textChanged()
{
    preview->updatePreview();
}

void PrintDialogClinic::on_pushButton_back_clicked()
{
    emit showRegistWindow();
    this->close();
}

void PrintDialogClinic::saveReport()
{
    QString fFullpath=PatientManager::getInstance()->autoGetReportFullPath();
    QPrinter printer;
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setPageSize(QPrinter::A4);
    printer.setOutputFileName(fFullpath);

    QTextDocument *ted = new QTextDocument;
    ted->setHtml(getHtmlStr(check1,check2));
    ted->print(&printer);

    delete ted;
}

void PrintDialogClinic::on_pushButton_save_clicked()
{
    FileSaveDialog *fdig=new FileSaveDialog(PA_REPORT);
    if(fdig->exec()!=QDialog::Accepted)
    {
        delete fdig;
        return;
    }
    QString fFullpath=PatientManager::getInstance()->getReportFullPath();
    QPrinter printer;
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setPageSize(QPrinter::A4);
    printer.setOutputFileName(fFullpath);

    QTextDocument *ted = new QTextDocument;
    ted->setHtml(getHtmlStr(check1,check2));
    ted->print(&printer);
    QMessageBox::information(0,"Notice","Report saved.","Confirm",0);
    delete ted;
    delete fdig;
}

void PrintDialogClinic::on_lineEdit_irradiateSpotNumber_textChanged(const QString &arg)
{
    preview->updatePreview();
}

void PrintDialogClinic::on_lineEdit_irradiateAverageEnergy_textChanged(const QString &arg)
{
    preview->updatePreview();
}

void PrintDialogClinic::on_lineEdit_irradiateTotalEnergy_textChanged(const QString &arg)
{
    preview->updatePreview();
}

void PrintDialogClinic::on_lineEdit_deep_textChanged(const QString &arg)
{
    preview->updatePreview();
}

void PrintDialogClinic::on_lineEdit_pulseNumber_textChanged(const QString &arg)
{
    preview->updatePreview();
}

void PrintDialogClinic::on_lineEdit_doctor_textChanged(const QString &arg)
{
    preview->updatePreview();
}

void PrintDialogClinic::on_lineEdit_group_number_textChanged(const QString &arg)
{
    preview->updatePreview();
}

void PrintDialogClinic::on_lineEdit_name_textChanged(const QString &arg)
{
    preview->updatePreview();
}

void PrintDialogClinic::on_lineEdit_btime_textChanged(const QString &arg1)
{
    preview->updatePreview();
}

void PrintDialogClinic::on_lineEdit_etime_textChanged(const QString &arg)
{
    preview->updatePreview();
}

void PrintDialogClinic::on_lineEdit_total_irradiate_time_textChanged(const QString &arg)
{
    preview->updatePreview();
}

void PrintDialogClinic::on_lineEdit_irradiate_voltage_textChanged(const QString &arg)
{
    preview->updatePreview();
}

void PrintDialogClinic::on_lineEdit_irradiateTotalPower_textChanged(const QString &arg)
{
    preview->updatePreview();
}

void PrintDialogClinic::on_lineEdit_date_textChanged(const QString &arg)
{
    preview->updatePreview();
}

void PrintDialogClinic::on_checkBox_2_clicked()
{
    ui->checkBox->setChecked(false);
    check1 = "None";
    preview->updatePreview();
    ui->checkBox_2->setChecked(true);

}

void PrintDialogClinic::on_checkBox_clicked()
{
    ui->checkBox_2->setChecked(false);
    check1 = "Yes";
    preview->updatePreview();
    ui->checkBox->setChecked(true);

}

void PrintDialogClinic::on_checkBox_3_clicked()
{
    ui->checkBox_4->setChecked(false);
    check2 = "Yes";
    preview->updatePreview();
    ui->checkBox_3->setChecked(true);
}

void PrintDialogClinic::on_checkBox_4_clicked()
{
    ui->checkBox_3->setChecked(false);
    check2 = "None";
    preview->updatePreview();
    ui->checkBox_4->setChecked(true);
}

void PrintDialogClinic::on_lineEdit_allirradiateSpotNumber_textChanged(const QString &arg)
{
    preview->updatePreview();
}

void PrintDialogClinic::on_lineEdit_effective_time_textChanged(const QString &arg1)
{
    preview->updatePreview();
}
