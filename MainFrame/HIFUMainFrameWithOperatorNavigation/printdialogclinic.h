#ifndef PRINTDIALOGCLINIC_H
#define PRINTDIALOGCLINIC_H

#include <QDialog>
#include <QDate>
#include <QTime>
#include <QPrintDialog>
#include <QPrintPreviewDialog>
#include <QTextEdit>
#include <QTextDocument>
#include <QPrintPreviewWidget>
#include <QLayout>
#if _MSC_VER >1600
#pragma execution_character_set("utf-8")
#endif

namespace Ui {
class PrintDialogClinic;
}
//热损伤和机械损伤信息
//struct Checks{
//    QString check1;
//    QString check2;

//    Checks()
//    {
//        check1 = "None";
//        check2 = "None";

//    }

//};

class PrintDialogClinic : public QDialog
{
    Q_OBJECT

public:
    explicit PrintDialogClinic(QWidget *parent = 0);
    ~PrintDialogClinic();

private slots:
    void printPreview(QPrinter *p);

    void on_pushButton_print_clicked();

    void on_textEdit_conclusion_textChanged();

    void on_pushButton_back_clicked();

    void on_pushButton_save_clicked();

    void on_lineEdit_irradiateSpotNumber_textChanged(const QString &arg1);

    void on_lineEdit_irradiateAverageEnergy_textChanged(const QString &arg1);

    void on_lineEdit_irradiateTotalEnergy_textChanged(const QString &arg1);

    void on_lineEdit_deep_textChanged(const QString &arg1);

    void on_lineEdit_pulseNumber_textChanged(const QString &arg1);

    void on_lineEdit_doctor_textChanged(const QString &arg1);

    void on_lineEdit_group_number_textChanged(const QString &arg1);

    void on_lineEdit_name_textChanged(const QString &arg1);

    void on_lineEdit_btime_textChanged(const QString &arg1);

    void on_lineEdit_etime_textChanged(const QString &arg1);

    void on_lineEdit_total_irradiate_time_textChanged(const QString &arg1);

    void on_lineEdit_irradiate_voltage_textChanged(const QString &arg1);

    void on_lineEdit_irradiateTotalPower_textChanged(const QString &arg1);

    void on_lineEdit_date_textChanged(const QString &arg1);

    void on_checkBox_2_clicked();

    void on_checkBox_clicked();

    void on_checkBox_3_clicked();

    void on_checkBox_4_clicked();

    void on_lineEdit_allirradiateSpotNumber_textChanged(const QString &arg1);

    void on_lineEdit_effective_time_textChanged(const QString &arg1);

public slots:
    void setPatientInfo(QVariant id,QVariant fname,QVariant age,QVariant mobile,QVariant x,QVariant y,QVariant z);
    void setTerapyInfo(QVariant rate,QVariant startTime,QVariant endTime,QVariant number,QVariant totalTime,QVariant deep,QVariant doctor,QVariant pulse,QVariant volt);

    void saveReport();

private:
    Ui::PrintDialogClinic *ui;
    QString m_strPeintHtml;
//    QString id;
//    QString name;
//    QString stime;
//    QString etime;
//    QString totaltime;
//    QString spotnum;
//    QString deep;
//    QString palsenum;
//    QString totalspot;
    QString check1 = "None";
    QString check2 = "None";
    QString getHtmlStr(QString check1,QString check2);
    QPrinter *printer;
    QPrintPreviewWidget *preview;
//    Checks _checks;
//    QVector<QString> checks;
    void SaveReportInfoToDB();


signals:
    void showRegistWindow();

};

#endif // PRINTDIALOGCLINIC_H
