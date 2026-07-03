#include "regiongrowingdialog.h"
#include "ui_regiongrowingdialog.h"

RegionGrowingDialog::RegionGrowingDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RegionGrowingDialog),
    isSettingLimitedRegion1(false),
    isSettingLimitedRegion2(false),
    isSetingSeedPoint(false),
    isErasing(false),
    _mainWindow((QDialog*)(parent)),
    EraseSize(0)
{
    ui->setupUi(this);
}

RegionGrowingDialog::~RegionGrowingDialog()
{
    delete ui;
}

void RegionGrowingDialog::SetSeedPos(int x,int y,int z)
{
    ui->label_SeedPos->setText(QString("(%1,%2,%3)").arg(x).arg(y).arg(z));
}

void RegionGrowingDialog::on_pushButton_Grow_clicked()
{
    emit GrowProcess();
}

void RegionGrowingDialog::UpdateGrowRegionPosInfo(short value,int posx,int posy,int posz)
{
    ui->label_MouseValue->setText(QString("%1").arg(value));
    ui->label_MouseInfo->setText(QString("X:%1  Y:%2  Z:%3").arg(posx).arg(posy).arg(posz));
}

void RegionGrowingDialog::on_toolButton_SetRegionXY_clicked()
{
    isSettingLimitedRegion1=true;
    isSettingLimitedRegion2=false;
    setCursor(Qt::CrossCursor);
    _mainWindow->setCursor(Qt::CrossCursor);
}

void RegionGrowingDialog::on_pushButton_PickSeedPoint_clicked()
{
    isSetingSeedPoint=true;
    setCursor(Qt::CrossCursor);
    _mainWindow->setCursor(Qt::CrossCursor);
}

void RegionGrowingDialog::on_spinBox_Limit_z1_valueChanged(int arg1)
{
    LimitRegion_z1=arg1;
    ui->label__LimitZ->setText(QString("%1-%2").arg(LimitRegion_z1).arg(LimitRegion_z2));
}

void RegionGrowingDialog::on_spinBox_Limit_z2_valueChanged(int arg1)
{
    LimitRegion_z2=arg1;
    ui->label__LimitZ->setText(QString("%1-%2").arg(LimitRegion_z1).arg(LimitRegion_z2));
}

void RegionGrowingDialog::on_pushButton_ReGrow_clicked()
{
    emit ReGrowProcess();
}

void RegionGrowingDialog::SetLimitPoint1(int p1x,int p1y)
{
    ui->label_LimitPoint1->setText(QString("(%1,%2)").arg(p1x).arg(p1y));
}

void RegionGrowingDialog::SetLimitPoint2(int p2x,int p2y,int w,int h)
{
    ui->label_LimitPoint2->setText(QString("(%1,%2)").arg(p2x).arg(p2y));
    ui->label__LimitWH->setText(QString("%1*%2").arg(w).arg(h));
}

void RegionGrowingDialog::on_toolButton_Erase_clicked()
{
    isErasing=!isErasing;
    if(isErasing)
    {
        setCursor(Qt::ForbiddenCursor);
        _mainWindow->setCursor(Qt::ForbiddenCursor);
    }
    else
    {
        setCursor(Qt::ArrowCursor);
        _mainWindow->setCursor(Qt::ArrowCursor);
    }
}

void RegionGrowingDialog::on_horizontalSlider_EraseSize_valueChanged(int value)
{
    EraseSize=value;
    ui->label_EraseSize->setText(QString("%1").arg(EraseSize));
}
