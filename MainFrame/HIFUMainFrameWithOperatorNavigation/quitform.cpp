#include "quitform.h"
#include "ui_quitform.h"
#include "constant.h"
#include "windows.h"

quitform::quitform(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::quitform)
{
    ui->setupUi(this);

    this->setAutoFillBackground(true);
    QPixmap pixmap(QString(ICON_PIC_PATH).append("quitform.png"));
    QPalette palette;
    palette.setBrush(this->backgroundRole(),QBrush(pixmap));
    this->setPalette(palette);
}

quitform::~quitform()
{
    delete ui;
}
