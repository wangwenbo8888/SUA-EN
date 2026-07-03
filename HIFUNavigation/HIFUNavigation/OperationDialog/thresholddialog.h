#ifndef THRESHOLDDIALOG_H
#define THRESHOLDDIALOG_H

#include <QDialog>

namespace Ui {
class ThresholdDialog;
}

class ThresholdDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ThresholdDialog(QWidget *parent = 0);
    ~ThresholdDialog();

    void UpdateCtlValueRange(int min,int max);
    //void SetThreSholdRange(int tmin,int tmax);

protected:
    void showEvent(QShowEvent *event);

signals:
    void on_ThresholdValueChanged(int min,int max);

private slots:
    void on_spinBox_Threshold_Min_valueChanged(int arg1);

    void on_horizontalSlider_Threshold_Min_valueChanged(int value);

    void on_horizontalSlider_Threshold_Max_valueChanged(int value);

    void on_spinBox_Threshold_Max_valueChanged(int arg1);

private:
    Ui::ThresholdDialog *ui;
};

#endif // THRESHOLDDIALOG_H
