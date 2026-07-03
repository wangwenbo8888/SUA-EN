#ifndef CHOSEIMAGE_H
#define CHOSEIMAGE_H

#include <QDialog>

class NewMainWindow;

namespace Ui {
class choseimage;
}

class Choseimage : public QDialog
{
    Q_OBJECT

public:
    Choseimage(NewMainWindow* pMain,QWidget *parent = 0);
    ~Choseimage();

    void setValue(int value);

    void setEnabled();

    void setDisabled();

private slots:
    void on_spinBox_editingFinished();

    void on_buttonBox_accepted();

    void on_horizontalScrollBar_valueChanged(int value);

    void on_pushButton_length_anti_clicked();

    void on_pushButton_cross_clicked();

    void on_pushButton_length_normal_clicked();

signals:

    void setPos(int);

private:
    Ui::choseimage *ui;
    NewMainWindow* _pMainWindow;
};

#endif // CHOSEIMAGE_H
