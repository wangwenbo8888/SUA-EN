#ifndef FILESAVEDIALOG_H
#define FILESAVEDIALOG_H

#include <QDialog>
#include "patientmanager.h"
#if _MSC_VER >1600
#pragma execution_character_set("utf-8")
#endif

namespace Ui {
class FileSaveDialog;
}

class FileSaveDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FileSaveDialog(QWidget *parent = 0);
    explicit FileSaveDialog(PAFILETYPE filetype,QWidget *parent = 0);
    ~FileSaveDialog();

private slots:
    void on_checkBox_defaultFilePath_clicked(bool checked);

    void on_checkBox_defaultFileName_clicked(bool checked);

    void on_lineEdit_fileName_textChanged(const QString &arg1);

    void on_pushButton_selectFilePath_clicked();

    void on_pushButton_Accepted_clicked();

    void on_pushButton_Cancel_clicked();

    void on_lineEdit_filePath_textChanged(const QString &arg1);

signals:
    void fullpath1(QString path);

private:
    Ui::FileSaveDialog *ui;
    PAFILETYPE m_FileType;
    PatientManager *_patientmanager;
    QString sPathDefault;
    QString sNmaeDefault;
    QString *pFilePath;
    QString *pFileName;
    QString sFPath;//Save路径

    QString temp_path;//缓存

    void updateFullPath();
};

#endif // FILESAVEDIALOG_H
