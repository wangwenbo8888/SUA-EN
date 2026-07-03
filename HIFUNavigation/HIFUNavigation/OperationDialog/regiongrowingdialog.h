#ifndef REGIONGROWINGDIALOG_H
#define REGIONGROWINGDIALOG_H

#include <QDialog>

namespace Ui {
class RegionGrowingDialog;
}

class RegionGrowingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RegionGrowingDialog(QWidget *parent = 0);
    ~RegionGrowingDialog();
    void SetSeedPos(int x,int y,int z);
    void UpdateGrowRegionPosInfo(short value,int posx,int posy,int posz);

    bool isSettingLimitedRegion1;
    bool isSettingLimitedRegion2;
    bool isSetingSeedPoint;
    bool isErasing;

    int EraseSize;

    int LimitRegion_z1;
    int LimitRegion_z2;

    void SetLimitPoint1(int p1x,int p1y);
    void SetLimitPoint2(int p2x,int p2y,int w,int h);

signals:
    void GrowProcess();
    void ReGrowProcess();

private slots:
    void on_pushButton_Grow_clicked();

    void on_toolButton_SetRegionXY_clicked();

    void on_pushButton_PickSeedPoint_clicked();

    void on_spinBox_Limit_z1_valueChanged(int arg1);

    void on_spinBox_Limit_z2_valueChanged(int arg1);

    void on_pushButton_ReGrow_clicked();

    void on_toolButton_Erase_clicked();

    void on_horizontalSlider_EraseSize_valueChanged(int value);

private:
    Ui::RegionGrowingDialog *ui;
    QDialog * _mainWindow;
};

#endif // REGIONGROWINGDIALOG_H
