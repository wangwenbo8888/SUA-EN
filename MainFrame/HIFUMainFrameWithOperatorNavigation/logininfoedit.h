#ifndef LOGININFOEDIT_H
#define LOGININFOEDIT_H

#include <QDialog>
#include "usermanager_global.h"
#if _MSC_VER >1600
#pragma execution_character_set("utf-8")
#endif

namespace Ui {
class LoginInfoEdit;
}

class LoginInfoEdit : public QDialog
{
    Q_OBJECT

public:
    explicit LoginInfoEdit(QWidget *parent = 0,int id=-1);
    ~LoginInfoEdit();

private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    Ui::LoginInfoEdit *ui;

    void GetLoginInfo(int id);
    DrLoginDataRow m_DrLoginData;
};

#endif // LOGININFOEDIT_H
