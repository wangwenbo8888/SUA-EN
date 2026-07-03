#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QVBoxLayout>

#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    m_labelAngle = new QLabel("angle:",this);
    m_labelFAxis = new QLabel("front:",this);
    m_labelLAxis = new QLabel("left:",this);
    m_labelZAxis = new QLabel("up:",this);

    m_editAngle = new QLineEdit(this);
    m_editFAxis = new QLineEdit(this);
    m_editLAxis = new QLineEdit(this);
    m_editZAxis = new QLineEdit(this);

    QHBoxLayout* layoutInput = new QHBoxLayout;
    layoutInput->addWidget(m_labelAngle);
    layoutInput->addWidget(m_editAngle);
    layoutInput->addWidget(m_labelFAxis);
    layoutInput->addWidget(m_editFAxis);
    layoutInput->addWidget(m_labelLAxis);
    layoutInput->addWidget(m_editLAxis);
    layoutInput->addWidget(m_labelZAxis);
    layoutInput->addWidget(m_editZAxis);

    m_btnAngle = new QPushButton("angle",this);
    m_btnFAxis = new QPushButton("front-back",this);
    m_btnLAxis = new QPushButton("left-right",this);
    m_btnZAxis = new QPushButton("up-down",this);

    QHBoxLayout* layoutBtn = new QHBoxLayout;
    layoutBtn->addWidget(m_btnAngle);
    layoutBtn->addWidget(m_btnFAxis);
    layoutBtn->addWidget(m_btnLAxis);
    layoutBtn->addWidget(m_btnZAxis);

    m_status = new QLabel(this);

    QVBoxLayout* layout = new QVBoxLayout;
    layout->addLayout(layoutInput);
    layout->addLayout(layoutBtn);
    layout->addWidget(m_status);

    QWidget* widget = new QWidget;
    widget->setLayout(layout);

    setCentralWidget(widget);

    m_location = new Location(this);

    connect(m_btnAngle,SIGNAL(clicked()),this,SLOT(onBtnAngleClicked()));
    connect(m_btnFAxis,SIGNAL(clicked()),this,SLOT(onBtnFAxisClicked()));
    connect(m_btnLAxis,SIGNAL(clicked()),this,SLOT(onBtnLAxisClicked()));
    connect(m_btnZAxis,SIGNAL(clicked()),this,SLOT(onBtnZAxisClicked()));
    connect(m_location,SIGNAL(posDat(AID,APOS)),this,SLOT(onStatusUpdated(AID,APOS)));
    connect(m_location,SIGNAL(error(ERRORTYPE)),this,SLOT(onErrorUpdated(ERRORTYPE)));

    if (m_location->initialize()) {
        m_location->getAng();
    }
}

MainWindow::~MainWindow()
{
    delete m_location;
}

void MainWindow::onBtnAngleClicked()
{
    APOS pos = m_editAngle->text().toFloat();
    m_location->setAng(pos);
    m_status->setText("Starting angle ...");
}

void MainWindow::onBtnFAxisClicked()
{
    APOS pos = m_editFAxis->text().toFloat();
    m_location->setPos(AXIS_F,pos);
    m_status->setText("Starting front axis ...");
}

void MainWindow::onBtnLAxisClicked()
{
    APOS pos = m_editLAxis->text().toFloat();
    m_location->setPos(AXIS_L,pos);
    m_status->setText("Starting left axis ...");
}

void MainWindow::onBtnZAxisClicked()
{
    APOS pos = m_editZAxis->text().toFloat();
    m_location->setPos(AXIS_Z,pos);
    m_status->setText("Starting up axis ...");
}

void MainWindow::onStatusUpdated(AID id, APOS pos)
{
    QString str = QString("%1 axis: %2").arg(id).arg(pos);
    m_status->setText(str);
}

void MainWindow::onErrorUpdated(ERRORTYPE errorCode)
{
    QString str = QString("Error code: %1").arg(errorCode);
    m_status->setText(str);
}
