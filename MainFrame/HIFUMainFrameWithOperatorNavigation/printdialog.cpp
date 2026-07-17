#include "printdialog.h"
#include "ui_printdialog.h"
#include <QPixmap>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QMessageBox>
#include "filesavedialog.h"

PrintDialog::PrintDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PrintDialog)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);

    ted = new QTextDocument;
    ted->setHtml(getHtmlStr());

    printer=new QPrinter();
    preview = new QPrintPreviewWidget (printer,this);

    this->ui->verticalLayout_printview->addWidget(preview);
    connect(preview, SIGNAL(paintRequested(QPrinter*)),this,SLOT(printPreview(QPrinter *)));//关联Print预览的内容
    preview->show();
    //preview->setZoomMode(QPrintPreviewWidget::FitToWidth);

}

PrintDialog::~PrintDialog()
{
    delete preview;
    delete ted;
    delete printer;
    delete ui;
}
//Print
void PrintDialog::on_pushButton_print_clicked()
{
//    QTextDocument *ted = new QTextDocument;
//    ted->setHtml(getHtmlStr());
//    ted->print(printer);
    preview->print();
    SaveReportInfoToDB();
}


//预览
void PrintDialog::printPreview(QPrinter *p)
{
    QTextDocument *ted = new QTextDocument;
    ted->setHtml(getHtmlStr());
    ted->print(p);
    delete ted;
}
//设置Patient information槽
void PrintDialog::setPatientInfo(QVariant id, QVariant fname, QVariant age,
                                 QVariant mobile, QVariant x, QVariant y, QVariant z)
{
    ui->lineEdit_id->setText(id.toString());
    ui->lineEdit_name->setText(fname.toString());
    ui->lineEdit_age->setText(age.toString());
    ui->lineEdit_gender->setText("female ");
    ui->lineEdit_mobile->setText(mobile.toString());
    ui->lineEdit_sizex->setText(x.toString());
    ui->lineEdit_sizey->setText(y.toString());
    ui->lineEdit_sizez->setText(z.toString());

    preview->updatePreview();

}

//设置Treatment信息槽
void PrintDialog::setTerapyInfo(QVariant rate, QVariant startTime, QVariant endTime,
                                QVariant number, QVariant energy, QVariant deep, QVariant doctor, QVariant pulse)
{
    // 更新Treatment效果图
    QPixmap pixUp("E:/upimage.bmp");
    QPixmap pixMiddle("E:/middleimage.bmp");
    QPixmap pixDown("E:/downimage.bmp");

    QPixmap upClip;
    if (pixUp.width()==328)
    {
       upClip = pixUp.copy(64,64,200,200);
    }
    else
    {
        upClip = pixUp;
    }

    QPixmap middleClip;
    if (pixMiddle.width()==328)
    {
        middleClip = pixMiddle.copy(64,64,200,200);
    }
    else
    {
        middleClip = pixMiddle;
    }
    QPixmap downClip;
    if (pixDown.width()==328)
    {
        downClip= pixDown.copy(64,64,200,200);
    }
    else
    {
        downClip = pixDown;
    }

    ui->label_imgUp->setPixmap(upClip);
    ui->label_imgMiddle->setPixmap(middleClip);
    ui->label_imgDown->setPixmap(downClip);

    // ui->lineEdit_melt->setText(rate.toString());
    ui->lineEdit_btime->setText(startTime.toString());
    ui->lineEdit_etime->setText(endTime.toString());
    ui->lineEdit_irradiateSpotNumber->setText(number.toString());
    ui->lineEdit_irradiateAverageEnergy->setText(energy.toString());

    int totalEnergy = energy.toInt()*number.toInt();
    qDebug()<<"energy is "<< totalEnergy;

    ui->lineEdit_irradiateTotalEnergy->setText(QString::number(totalEnergy));
    ui->lineEdit_deep->setText(deep.toString());
    ui->lineEdit_doctor->setText(doctor.toString());
    ui->lineEdit_pulseNumber->setText(pulse.toString());
    preview->updatePreview();
}

void PrintDialog::SaveReportInfoToDB()
{
    qDebug()<<"SaveReportInfoToDB";
    QString strSQl=QString("INSERT INTO Report (pid,CreateTime,melt,stime,etime,conclusion,iSpotNumber,iVoltage,deep,pulseNumber,doctor) VALUES ('%1','%2','%3','%4','%5','%6','%7','%8','%9','%10','%11')")
            .arg(ui->lineEdit_id->text())
            .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss"))
            //.arg(ui->lineEdit_melt->text())
            .arg(ui->lineEdit_btime->text())
            .arg(ui->lineEdit_etime->text())
            .arg(ui->textEdit_conclusion->toPlainText())
            .arg(ui->lineEdit_irradiateSpotNumber->text())
            .arg(ui->lineEdit_irradiateAverageEnergy->text())
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
        QMessageBox::critical(0, QObject::tr("Notice"),"Treatment report database import failed. Error:"+updateQuery.lastError().text());
    }
}

//Generate文档样式html
QString PrintDialog::getHtmlStr()
{
    m_strPeintHtml=QString("<p align=\"center\">\
            <span style=\"font-size: 24px;\"><strong><span style=\"font-family: 微软雅黑,Microsoft YaHei;\">Zhonghui Medical HIFU Treatment Report</span></strong></span>\
        </p>\
        <p align=\"right\">\
            PrintTime：%1\
        </p>\
        <p>\
            <span style=\"font-size: 20px;\"><strong><span style=\"font-family: 微软雅黑,Microsoft YaHei;\">Patient information</span></strong></span>\
        </p>\
        <hr/>\
        <table width=\"595\">\
            <tbody>\
                <tr class=\"firstRow\">\
                    <td style=\"border-width: 1px; border-style: solid;\" width=\"261\" valign=\"top\">\
                        <span style=\"font-family: 微软雅黑,Microsoft YaHei; font-size: 18px;\">Patient ID：%2<br/></span>\
                    </td>\
                    <td style=\"border-width: 1px; border-style: solid;\" width=\"261\" valign=\"top\">\
                        <span style=\"font-family: 微软雅黑,Microsoft YaHei; font-size: 18px;\">Name:%3<br/></span>\
                    </td>\
                    <td style=\"border-width: 1px; border-style: solid;\" width=\"261\" valign=\"top\">\
                        <span style=\"font-family: 微软雅黑,Microsoft YaHei; font-size: 18px;\">Sex：%4<br/></span>\
                    </td>\
                </tr>\
                <tr>\
                    <td style=\"border-width: 1px; border-style: solid;\" width=\"261\" valign=\"top\">\
                        <span style=\"font-family: 微软雅黑,Microsoft YaHei; font-size: 18px;\">Age：%5<br/></span>\
                    </td>\
                    <td rowspan=\"1\" colspan=\"2\" style=\"border-width: 1px; border-style: solid;\" valign=\"top\">\
                        <span style=\"font-family: 微软雅黑,Microsoft YaHei; font-size: 18px;\">Phone：%6<br/></span>\
                    </td>\
                </tr>\
            </tbody>\
        </table>\
        <hr/>\
        <table width=\"595\">\
            <tbody>\
                <tr class=\"firstRow\">\
                    <td rowspan=\"1\" colspan=\"2\" valign=\"top\">\
                        <span style=\"font-family: 微软雅黑,Microsoft YaHei; font-size: 18px;\">Fibroid Size：%7 * %8 * %9 mm3<br/></span>\
                    </td>\
                </tr>\
                <tr>\
                    <td width=\"402\" valign=\"top\">\
                        <span style=\"font-family: 微软雅黑,Microsoft YaHei; font-size: 16px;\">Start time:%11<br/></span>\
                    </td>\
                    <td width=\"402\" valign=\"top\">\
                        <span style=\"font-family: 微软雅黑,Microsoft YaHei; font-size: 16px;\">End time:%12</span><br/>\
                    </td>\
                </tr>\
            </tbody>\
        </table>\
        <hr/>\
        <p>\
            <span style=\"font-family: 微软雅黑,Microsoft YaHei;\"><strong><span style=\"font-size: 20px;\">Treatment结论</span></strong></span>\
        </p>\
        <table width=\"620\">\
            <tbody>\
                <tr class=\"firstRow\">\
                    <td style=\"word-break: break-all;\" width=\"620\" valign=\"top\">\
                        <span style=\"font-size: 18px; font-family: 微软雅黑,Microsoft YaHei;\">%13</span><br/>\
                    </td>\
                </tr>\
            </tbody>\
        </table>\
        <hr/>\
        <p>\
            <strong><span style=\"font-family: 微软雅黑,Microsoft YaHei; font-size: 20px;\">Treatment Parameters</span></strong>\
        </p>\
        <table width=\"600\">\
            <tbody>\
                <tr class=\"firstRow\">\
                    <td width=\"250\" valign=\"top\">\
                        <span style=\"font-family: 微软雅黑,Microsoft YaHei; font-size: 18px;\">Sonication Points：%14 pcs<br/></span>\
                    </td>\
                    <td width=\"250\" valign=\"top\">\
                        <span style=\"font-family: 微软雅黑,Microsoft YaHei; font-size: 18px;\">Energy per Point：%15 Focus<br/></span>\
                    </td>\
                    <td width=\"250\" valign=\"top\">\
                        <span style=\"font-family: 微软雅黑,Microsoft YaHei; font-size: 18px;\">Total Energy：%16 Focus<br/></span>\
                    </td>\
                </tr>\
                <tr>\
                    <td width=\"402\" valign=\"top\">\
                        <span style=\"font-family: 微软雅黑,Microsoft YaHei; font-size: 18px;\">Treatment depth：%17 cm<br/></span>\
                    </td>\
                    <td width=\"250\" valign=\"top\">\
                        <span style=\"font-family: 微软雅黑,Microsoft YaHei; font-size: 18px;\">Pulse Count：%18 pcs</span><br/>\
                    </td>\
                </tr>\
            </tbody>\
        </table>\
        <hr/>\
        <p>\
            <strong><span style=\"font-size: 20px; font-family: 微软雅黑,Microsoft YaHei;\">Treatment效果</span></strong>\
        </p>\
        <table>\
            <tbody>\
                <tr class=\"firstRow\">\
                    <td width=\"200\" valign=\"middle\" align=\"center\">\
                        <img src=\"E:/downimage.bmp\" title=\"\" alt=\"demo.jpg\" width=\"150\" height=\"150\"/>\
                    </td>\
                    <td width=\"200\" valign=\"middle\" align=\"center\">\
                        <img src=\"E:/middleimage.bmp\" title=\"\" alt=\"demo.jpg\" width=\"150\" height=\"150\"/>\
                    </td>\
                    <td width=\"200\" valign=\"middle\" align=\"center\">\
                        <img src=\"E:/upimage.bmp\" title=\"\" alt=\"demo.jpg\" width=\"150\" height=\"150\"/>\
                    </td>\
                </tr>\
                <tr>\
                    <td style=\"word-break: break-all;\" width=\"261\" valign=\"middle\" align=\"center\">\
                        <span style=\"font-size: 18px; font-family: 微软雅黑,Microsoft YaHei;\">Lower<br/></span>\
                    </td>\
                    <td style=\"word-break: break-all;\" width=\"261\" valign=\"middle\" align=\"center\">\
                        <span style=\"font-size: 18px; font-family: 微软雅黑,Microsoft YaHei;\">Middle<br/></span>\
                    </td>\
                    <td style=\"word-break: break-all;\" width=\"261\" valign=\"middle\" align=\"center\">\
                        <span style=\"font-size: 18px; font-family: 微软雅黑,Microsoft YaHei;\">Upper</span><br/>\
                    </td>\
                </tr>\
            </tbody>\
        </table>\
        <hr/>\
        <p>\
            <strong><span style=\"font-size: 20px; font-family: 微软雅黑,Microsoft YaHei;\">Attending Physician:</span></strong><span style=\"font-size: 18px; font-family: 微软雅黑,Microsoft YaHei;\">%19</span><br/>\
        </p>")
            .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"))
            .arg(ui->lineEdit_id->text())
            .arg(ui->lineEdit_name->text())
            .arg(ui->lineEdit_gender->text())
            .arg(ui->lineEdit_age->text())
            .arg(ui->lineEdit_mobile->text())
            .arg(ui->lineEdit_sizex->text())
            .arg(ui->lineEdit_sizey->text())
            .arg(ui->lineEdit_sizez->text())
            //.arg(ui->lineEdit_melt->text())
            .arg(ui->lineEdit_btime->text())
            .arg(ui->lineEdit_etime->text())
            .arg(ui->textEdit_conclusion->toPlainText())
            .arg(ui->lineEdit_irradiateSpotNumber->text())
            .arg(ui->lineEdit_irradiateAverageEnergy->text())
            .arg(ui->lineEdit_irradiateTotalEnergy->text())
            .arg(ui->lineEdit_deep->text())
            .arg(ui->lineEdit_pulseNumber->text())
            .arg(ui->lineEdit_doctor->text());
            return m_strPeintHtml;
}


void PrintDialog::on_textEdit_conclusion_textChanged()
{
    preview->updatePreview();
}

void PrintDialog::on_pushButton_back_clicked()
{
    ui->textEdit_conclusion->setText("");
    emit showRegistWindow();
    this->close();
}

void PrintDialog::saveReport()
{
    QString fFullpath=PatientManager::getInstance()->getReportFullPath();
    QPrinter printer;
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setPageSize(QPrinter::A4);
    printer.setOutputFileName(fFullpath);

    QTextDocument *ted = new QTextDocument;
    ted->setHtml(getHtmlStr());
    ted->print(&printer);

    delete ted;
}

void PrintDialog::on_pushButton_save_clicked()
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
    ted->setHtml(getHtmlStr());
    ted->print(&printer);

    delete ted;
    delete fdig;
}

void PrintDialog::on_lineEdit_irradiateSpotNumber_textChanged(const QString &arg)
{
    preview->updatePreview();
}

void PrintDialog::on_lineEdit_irradiateAverageEnergy_textChanged(const QString &arg)
{
    preview->updatePreview();
}

void PrintDialog::on_lineEdit_irradiateTotalEnergy_textChanged(const QString &arg)
{
    preview->updatePreview();
}

void PrintDialog::on_lineEdit_deep_textChanged(const QString &arg)
{
    preview->updatePreview();
}

void PrintDialog::on_lineEdit_pulseNumber_textChanged(const QString &arg)
{
    preview->updatePreview();
}

void PrintDialog::on_lineEdit_doctor_textChanged(const QString &arg)
{
    preview->updatePreview();
}
