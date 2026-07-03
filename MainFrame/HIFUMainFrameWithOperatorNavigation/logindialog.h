#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H




#include <QDialog>
#include <QtSql/QSqlDatabase>
#include <QTimer>
#include "qevent.h"
#include <QCryptographicHash>
#include <QMessageBox>


namespace Ui {
class LoginDialog;
}

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDialog(QWidget *parent = 0);
    ~LoginDialog();

    QSqlDatabase m_db;

protected:
    void showEvent(QShowEvent *event);
    void keyPressEvent(QKeyEvent *event );

signals:
    void ShowUserManager(bool isPatient);

private slots:
    void on_pushButtonLogin_clicked();

    void on_commandLinkButton_clicked();

//    void on_pushButtonLogin_usermanager_clicked();

private:
    Ui::LoginDialog *ui;
    void DelayClose();
//    QCryptographicHash hash(QCryptographicHash::Sha256);
    QTimer delayTimer;
};

#endif // LOGINDIALOG_H
