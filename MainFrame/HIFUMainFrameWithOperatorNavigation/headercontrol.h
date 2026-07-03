#ifndef HEADERCONTROL_H_
#define HEADERCONTROL_H_

#include <QDialog>
#include "constant.h"

class NewMainWindow;

namespace Ui {
class headercontrol;
}

class HeaderControl : public QDialog
{
    Q_OBJECT

public:
    HeaderControl(NewMainWindow* pMain,QWidget *parent=0);
    ~HeaderControl();

    void Bed_Stop();

private slots:

    void on_pushButton_zero_clicked();

    void keyPressEvent(QKeyEvent *event);

    void on_spinBox_bed_left_right_editingFinished();

    void on_spinBox_bed_front_back_editingFinished();

    void on_spinBox_bed_up_down_editingFinished();

    void on_doubleSpinBox_left_right_editingFinished();

    void on_doubleSpinBox_front_back_editingFinished();

    void on_doubleSpinBox_up_down_editingFinished();

    void on_pushButton_bed_zero_clicked();
    void on_pushButton_left_clicked();

    void on_pushButton_right_clicked();

    void on_pushButton_forward_clicked();

    void on_pushButton_backward_clicked();

    void on_pushButton_up_clicked();

    void on_pushButton_down_clicked();

    void on_pushButton_stop_clicked();

signals:

    void setPos(AID axis,float angle);
    void setRodPos(int ,int ,int);//
    void getRodPos();
    void setBedZero();

private:
    Ui::headercontrol *ui;
    int Rec_PosX,Rec_PosY,Rec_PosZ;
    double Rec_H_PosX,Rec_H_PosY,Rec_H_PosZ;
    bool getRod_Moving;
    void Delay_MSec(unsigned int msec);

public slots:
    void getHeadPos(float posL,float posF,float posZ);
    void getBedPos(int posL, int posF, int posZ);
};

#endif // HEADERCONTROL_H_
