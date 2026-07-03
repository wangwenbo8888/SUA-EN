#include "choseimage.h"
#include "ui_choseimage.h"
#include "newmainwindow.h"
#include "commondefine.h"
#include <QDialogButtonBox>
#include <QPushButton>

Choseimage::Choseimage(NewMainWindow* pMain,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::choseimage),
    _pMainWindow(pMain)
{
    ui->setupUi(this);

    ui->spinBox->setStyleSheet(SELECTED);
    ui->horizontalScrollBar->setStyleSheet(SELECTED);

    connect(this, SIGNAL(setPos(int)),_pMainWindow, SLOT(setPos(int)));

    ui->pushButton_cross->setStyleSheet(SELECTED);
    ui->pushButton_length_anti->setStyleSheet(SELECTED);
    ui->pushButton_length_normal->setStyleSheet(SELECTED);

    QFont font;
    font.setFamily("Microsoft YaHei");
    font.setPointSize(12);

    ui->buttonBox->setStandardButtons(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);

    ui->buttonBox->button(QDialogButtonBox::Ok)->setText("OK");
    ui->buttonBox->button(QDialogButtonBox::Ok)->setFont(font);

    ui->buttonBox->button(QDialogButtonBox::Cancel)->setText("Cancel");
    ui->buttonBox->button(QDialogButtonBox::Cancel)->setFont(font);
}

Choseimage::~Choseimage()
{
    delete ui;
}

void Choseimage::on_spinBox_editingFinished()
{
    ui->horizontalScrollBar->setValue(ui->spinBox->value());
}

void Choseimage::on_buttonBox_accepted()
{
    emit setPos(ui->spinBox->value());
    this->show();
}

void Choseimage::setValue(int value)
{
    ui->spinBox->setValue(value);
    ui->horizontalScrollBar->setValue(value);
}

void Choseimage::setEnabled()
{
    ui->spinBox->setEnabled(true);
    ui->buttonBox->setEnabled(true);
    ui->horizontalScrollBar->setEnabled(true);
    ui->pushButton_cross->setEnabled(true);
    ui->pushButton_length_anti->setEnabled(true);
    ui->pushButton_length_normal->setEnabled(true);

    ui->pushButton_cross->setStyleSheet(SELECTED);
    ui->pushButton_length_anti->setStyleSheet(SELECTED);
    ui->pushButton_length_normal->setStyleSheet(SELECTED);
}

void Choseimage::setDisabled()
{
    ui->spinBox->setEnabled(false);
    ui->buttonBox->setEnabled(false);
    ui->horizontalScrollBar->setEnabled(false);

    ui->pushButton_cross->setEnabled(false);
    ui->pushButton_length_anti->setEnabled(false);
    ui->pushButton_length_normal->setEnabled(false);
    ui->pushButton_cross->setStyleSheet(NOT_WORK);
    ui->pushButton_length_anti->setStyleSheet(NOT_WORK);
    ui->pushButton_length_normal->setStyleSheet(NOT_WORK);
}

void Choseimage::on_horizontalScrollBar_valueChanged(int value)
{
    ui->spinBox->setValue(ui->horizontalScrollBar->value());
}

void Choseimage::on_pushButton_length_anti_clicked()
{
    emit setPos(-90);
    this->show();
}

void Choseimage::on_pushButton_cross_clicked()
{
    emit setPos(0);
    this->show();
}

void Choseimage::on_pushButton_length_normal_clicked()
{
    emit setPos(90);
    this->show();
}
