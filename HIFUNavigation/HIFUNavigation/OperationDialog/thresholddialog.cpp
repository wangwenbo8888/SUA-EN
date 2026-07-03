#include "thresholddialog.h"
#include "ui_thresholddialog.h"

ThresholdDialog::ThresholdDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ThresholdDialog)
{
    ui->setupUi(this);
    //setWindowFlags(windowFlags() | Qt::FramelessWindowHint);
}

ThresholdDialog::~ThresholdDialog()
{
    delete ui;
}

void ThresholdDialog::on_spinBox_Threshold_Min_valueChanged(int arg1)
{
    ui->horizontalSlider_Threshold_Min->setValue(arg1);
}

void ThresholdDialog::on_spinBox_Threshold_Max_valueChanged(int arg1)
{
    ui->horizontalSlider_Threshold_Max->setValue(arg1);
}

void ThresholdDialog::on_horizontalSlider_Threshold_Min_valueChanged(int value)
{
    if(ui->horizontalSlider_Threshold_Min->value()>=ui->horizontalSlider_Threshold_Max->value())
    {
        ui->horizontalSlider_Threshold_Min->setValue(ui->horizontalSlider_Threshold_Max->value());
    }
    ui->spinBox_Threshold_Min->setValue(ui->horizontalSlider_Threshold_Min->value());

    if(!this->isHidden())
    {
        emit on_ThresholdValueChanged(ui->horizontalSlider_Threshold_Min->value(),
                                      ui->horizontalSlider_Threshold_Max->value());
    }
}

void ThresholdDialog::on_horizontalSlider_Threshold_Max_valueChanged(int value)
{
    if(ui->horizontalSlider_Threshold_Max->value()<=ui->horizontalSlider_Threshold_Min->value())
    {
        ui->horizontalSlider_Threshold_Max->setValue(ui->horizontalSlider_Threshold_Min->value());
    }
    ui->spinBox_Threshold_Max->setValue(ui->horizontalSlider_Threshold_Max->value());

    if(!this->isHidden())
    {
        emit on_ThresholdValueChanged(ui->horizontalSlider_Threshold_Min->value(),
                                      ui->horizontalSlider_Threshold_Max->value());
    }
}

void ThresholdDialog::UpdateCtlValueRange(int min,int max)
{
    ui->horizontalSlider_Threshold_Max->setMinimum(min);
    ui->horizontalSlider_Threshold_Max->setMaximum(max);
    ui->spinBox_Threshold_Max->setMaximum(max);
    ui->spinBox_Threshold_Max->setMinimum(min);
    ui->horizontalSlider_Threshold_Min->setMinimum(min);
    ui->horizontalSlider_Threshold_Min->setMaximum(max);
    ui->spinBox_Threshold_Min->setMaximum(max);
    ui->spinBox_Threshold_Min->setMinimum(min);
    ui->spinBox_Threshold_Max->setValue(max/3*2);
    ui->spinBox_Threshold_Min->setValue(max/3);
}

void ThresholdDialog::showEvent(QShowEvent *event)
{
    emit on_ThresholdValueChanged(ui->horizontalSlider_Threshold_Min->value(),
                                  ui->horizontalSlider_Threshold_Max->value());
}

