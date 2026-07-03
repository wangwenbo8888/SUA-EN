#include "surfacerenderingdialog.h"
#include "ui_surfacerenderingdialog.h"

SurfaceRenderingDialog::SurfaceRenderingDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SurfaceRenderingDialog)
{
    ui->setupUi(this);
}

SurfaceRenderingDialog::~SurfaceRenderingDialog()
{
    delete ui;
}

void SurfaceRenderingDialog::on_pushButton_SurfaceRendering_clicked()
{
    emit SurfaceRenderingProcess();
}
