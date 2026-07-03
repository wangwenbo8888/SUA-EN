#include "tramentmentparatables.h"
#include "ui_tramentmentparatables.h"

tramentmentparatables::tramentmentparatables(QWidget *parent) :
    QStackedWidget(parent),
    ui(new Ui::tramentmentparatables)
{
    ui->setupUi(this);
}

tramentmentparatables::~tramentmentparatables()
{
    delete ui;
}
