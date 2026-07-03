#include "draintankform.h"
#include "ui_draintankform.h"

DrainTankForm::DrainTankForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DrainTankForm)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);
}

DrainTankForm::~DrainTankForm()
{
    delete ui;
}
