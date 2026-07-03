#ifndef PRINTDIALOG_H
#define PRINTDIALOG_H

#include <QDialog>
#include <QDate>
#include <QTime>
#include <QPrintDialog>
#include <QPrintPreviewDialog>
#include <QTextEdit>
#include <QTextDocument>
#include <QPrintPreviewWidget>
#include <QLayout>

namespace Ui {
class PrintDialog;
}

class PrintDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PrintDialog(QWidget *parent = 0);
    ~PrintDialog();

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

public slots:
    void setPatientInfo(QVariant id,QVariant fname,QVariant age,QVariant mobile,QVariant x,QVariant y,QVariant z);
    void setTerapyInfo(QVariant rate,QVariant startTime,QVariant endTime,QVariant number,QVariant voltage,QVariant deep,QVariant doctor,QVariant pulse);

    void saveReport();

private:
    Ui::PrintDialog *ui;
    QString m_strPeintHtml;
    QString getHtmlStr();
    QPrinter *printer;
    QTextDocument *ted;
    QPrintPreviewWidget *preview;

    void SaveReportInfoToDB();


signals:
    void showRegistWindow();

};

#endif // PRINTDIALOG_H
