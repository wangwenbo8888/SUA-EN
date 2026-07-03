#include "headercontrol.h"
#include "ui_headercontrol.h"
#include "newmainwindow.h"
#include "commondefine.h"

HeaderControl::HeaderControl(NewMainWindow* pMain,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::headercontrol)
{
    connect(this,SIGNAL(setPos(AID,float)),
            pMain,SLOT(setPos(AID,float)));
    ui->setupUi(this);

    ui->doubleSpinBox_left_right->setToolTip("Negative left, positive right");
    ui->doubleSpinBox_front_back->setToolTip("Negative forward, positive back");
    ui->doubleSpinBox_up_down->setToolTip("Negative down, positive up");
    ui->pushButton_zero->setStyleSheet(SELECTED);
    ui->pushButton_bed_zero->setStyleSheet(SELECTED);
    ui->pushButton_forward->setStyleSheet(SELECTED);
    ui->pushButton_backward->setStyleSheet(SELECTED);
    ui->pushButton_left->setStyleSheet(SELECTED);
    ui->pushButton_right->setStyleSheet(SELECTED);
    ui->pushButton_up->setStyleSheet(SELECTED);
    ui->pushButton_down->setStyleSheet(SELECTED);
    ui->pushButton_stop->setStyleSheet(SELECTED);
    ui->tabWidget->setCurrentIndex(0);
    Rec_PosX=0,Rec_PosY=0,Rec_PosZ=0;
    Rec_H_PosX=0,Rec_H_PosY=0,Rec_H_PosZ=0;
    ui->tabWidget->removeTab(1);
//    QDoubleValidator* aDoubleValidoator = new QDoubleValidator;
//    aDoubleValidoator->setRange(-50.00,50.00,4);
//    ui->lineEdit->setValidator(aDoubleValidoator);
}

HeaderControl::~HeaderControl()
{
    delete ui;
}

void HeaderControl::Bed_Stop()
{
    on_pushButton_stop_clicked();
}

void HeaderControl::on_pushButton_zero_clicked()
{
    emit setPos(AXIS_L,0.0);
    emit setPos(AXIS_F,0.0);
    ui->doubleSpinBox_front_back->setValue(0.0);
    ui->doubleSpinBox_left_right->setValue(0.0);
}

void HeaderControl::on_doubleSpinBox_left_right_editingFinished()
{
    if(ui->doubleSpinBox_left_right->hasFocus())
    {
        double angle = ui->doubleSpinBox_left_right->value();
        emit setPos(AXIS_L,angle);
    }
    else
    {
        ui->doubleSpinBox_left_right->setValue(Rec_H_PosX);
    }
}


void HeaderControl::on_doubleSpinBox_front_back_editingFinished()
{
    if(ui->doubleSpinBox_front_back->hasFocus())
    {
        double angle = ui->doubleSpinBox_front_back->value();
        emit setPos(AXIS_F,angle);
    }
    else
    {
        ui->doubleSpinBox_front_back->setValue(Rec_H_PosY);
    }
}


void HeaderControl::on_doubleSpinBox_up_down_editingFinished()
{
    if(ui->doubleSpinBox_up_down->hasFocus())
    {
        double angle = ui->doubleSpinBox_up_down->value();
        emit setPos(AXIS_Z,angle);
    }
    else
    {
        ui->doubleSpinBox_up_down->setValue(Rec_H_PosZ);
    }
}

void HeaderControl::keyPressEvent(QKeyEvent *event )

{
    if( event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return )  //判断是否是回车键按下
    {
        return;
    }
}

void HeaderControl::on_spinBox_bed_left_right_editingFinished()
{
    qDebug()<<"Rec_PosX1====="<<Rec_PosX;
    if(ui->spinBox_bed_left_right->hasFocus())
    {
        qDebug()<<"Rec_PosX2====="<<Rec_PosX;
        int posx=ui->spinBox_bed_left_right->value();
        getRod_Moving=true;
//        emit getRodPos();
        Delay_MSec(200);
        if(posx==Rec_PosX)
            return;
        else if(posx>Rec_PosX)
        {
            emit setRodPos(1,1,posx-Rec_PosX);//向左移动posx-Rec_PosXDistance
            Rec_PosX=posx;
            qDebug()<<"Rec_PosX3====="<<Rec_PosX;
        }
        else if(posx<Rec_PosX)
        {
            emit setRodPos(1,2,Rec_PosX-posx);//向右移动Rec_PosX-posxDistance
            Rec_PosX=posx;
            qDebug()<<"Rec_PosX4====="<<Rec_PosX;
        }
    }
    else
    {
        qDebug()<<"Rec_PosX5====="<<Rec_PosX;
        ui->spinBox_bed_left_right->setValue(Rec_PosX);
    }
}


void HeaderControl::on_spinBox_bed_front_back_editingFinished()
{
    if(ui->spinBox_bed_front_back->hasFocus())
    {
        int posy=ui->spinBox_bed_front_back->value();
        getRod_Moving=true;
//        emit getRodPos();
        Delay_MSec(200);
        if(posy==Rec_PosY)
            return;
        else if(posy>Rec_PosY)
        {
            emit setRodPos(2,1,posy-Rec_PosY);
            Rec_PosY=posy;
        }
        else if(posy<Rec_PosY)
        {
            emit setRodPos(2,2,Rec_PosY-posy);
            Rec_PosY=posy;
        }
    }
    else
    {
        ui->spinBox_bed_front_back->setValue(Rec_PosY);
    }
}


void HeaderControl::on_spinBox_bed_up_down_editingFinished()
{
    if(ui->spinBox_bed_up_down->hasFocus())
        {
        int posz=ui->spinBox_bed_up_down->value();
        getRod_Moving=true;
//        emit getRodPos();
        Delay_MSec(200);
        if(posz==Rec_PosZ)
            return;
        else if(posz>Rec_PosZ)
        {
            emit setRodPos(3,1,posz-Rec_PosZ);
            Rec_PosZ=posz;
        }
        else if(posz<Rec_PosZ)
        {
            emit setRodPos(3,2,Rec_PosZ-posz);
            Rec_PosZ=posz;
        }
    }
    else
    {
        ui->spinBox_bed_up_down->setValue(Rec_PosZ);
    }
}

void HeaderControl::getHeadPos(float posL, float posF, float posZ)
{
    ui->doubleSpinBox_left_right->setValue(posL);
    ui->doubleSpinBox_front_back->setValue(posF);
    ui->doubleSpinBox_up_down->setValue(posZ);
    Rec_H_PosX=posL,Rec_H_PosY=posF,Rec_H_PosZ=posZ;
}

void HeaderControl::getBedPos(int posL, int posF, int posZ)
{
    qDebug()<<"getBedPos111111111111111111111";
    qDebug()<<"Rec_PosX6====="<<Rec_PosX;
    qDebug()<<"posL====="<<posL;
    if(!getRod_Moving)
    {
        ui->spinBox_bed_left_right->setValue(posL-130);
        ui->spinBox_bed_front_back->setValue(posF-70);
        ui->spinBox_bed_up_down->setValue(posZ);
    }
    Rec_PosX=posL-130;
    Rec_PosY=posF-70;
    Rec_PosZ=posZ;
    getRod_Moving=false;
    qDebug()<<"Rec_PosX7====="<<Rec_PosX;
}

void HeaderControl::on_pushButton_bed_zero_clicked()
{
    emit setBedZero();
    ui->spinBox_bed_front_back->setValue(0);
    ui->spinBox_bed_left_right->setValue(0);
    ui->spinBox_bed_up_down->setValue(0);
}

void HeaderControl::Delay_MSec(unsigned int msec)
{
    QTime _Timer = QTime::currentTime().addMSecs(msec);
    while(QTime::currentTime() < _Timer)
        QCoreApplication::processEvents(QEventLoop::AllEvents,100);
}

void HeaderControl::on_pushButton_left_clicked()
{
    int pos =ui->spinBox_bedmove->value()*(100/15);
    emit setRodPos(1,2,pos);
}


void HeaderControl::on_pushButton_right_clicked()
{
    int pos =ui->spinBox_bedmove->value()*(100/15);
    emit setRodPos(1,1,pos);
}


void HeaderControl::on_pushButton_forward_clicked()
{
    int pos =ui->spinBox_bedmove->value()*(100/7);
    emit setRodPos(2,2,pos);
}


void HeaderControl::on_pushButton_backward_clicked()
{
    int pos =ui->spinBox_bedmove->value()*(100/7);
    emit setRodPos(2,1,pos);
}


void HeaderControl::on_pushButton_up_clicked()
{
    int pos =ui->spinBox_bedmove->value()*(100/7);
    emit setRodPos(3,1,pos);
}


void HeaderControl::on_pushButton_down_clicked()
{
    int pos =ui->spinBox_bedmove->value()*(100/7);
    emit setRodPos(3,2,pos);
}


void HeaderControl::on_pushButton_stop_clicked()
{
    emit setRodPos(1,3,0);
    emit setRodPos(2,3,0);
    emit setRodPos(3,3,0);
}

