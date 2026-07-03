#include "hifunavigation.h"
#include <qdebug.h>

#include "maindialog.h"

HIFUNavigation::HIFUNavigation():_pMdlg(NULL)
{
    MyVtkContourWidget* myContourWidget=new MyVtkContourWidget();
    _pMdlg = new MainDialog;
    _pMdlg->SetVtkContourWidget(myContourWidget);

    connect(_pMdlg,SIGNAL(sendImageData(QImage,int)),
            this,SLOT(sendImageData(QImage,int)));

    connect(_pMdlg,SIGNAL(sendContour(QImage,int)),
            this,SLOT(receiveContour(QImage,int)));

    connect(_pMdlg,SIGNAL(sendContourNew(QImage,int)),
            this,SLOT(receiveContourNew(QImage,int)));

    connect(this,SIGNAL(sendOrientation(int)),
            _pMdlg,SLOT(setOrientation(int)));

    connect(_pMdlg,SIGNAL(MainDialogClose()),
            this,SLOT(on_Maindialog_Close()));

    connect(_pMdlg,SIGNAL(sendSearchRadius(int)),
            this,SLOT(receiveSearchRadius(int)));

    connect(this,SIGNAL(sendGetContour()),
            _pMdlg,SLOT(sendGetCoutour()));

}

HIFUNavigation::~HIFUNavigation()
{
    if (_pMdlg!=NULL)
    {
        delete _pMdlg;
        _pMdlg = NULL;
    }
}

void HIFUNavigation::showWindow()
{
    if(_pMdlg->isHidden())
        _pMdlg->show();
    else
        _pMdlg->activateWindow();

}

void HIFUNavigation::receiveSearchRadius(int radius)
{
    emit sendSearchRadius(radius);
}

void HIFUNavigation::receivePatientOrientation(int ori)
{
    emit sendOrientation(ori);
}

// mainframe 发送过来的获取肌瘤轮廓消息
void HIFUNavigation::receiveGetContour()
{
    emit sendGetContour();
}
// 接收保存肌膜轮廓
void HIFUNavigation::receiveContour(QImage image,int angle)
{
    // m_mapContours[angle] = image;
    emit sendOuter(image,angle);
}

void HIFUNavigation::receiveContourNew(QImage contour,int angle)
{
    emit sendContour(contour,angle);
}

void HIFUNavigation::sendImageData(QImage image,int angle)
{
    emit sendQImage(image,angle);
}

void HIFUNavigation::ClearData()
{
    _pMdlg->ClearAllData();
}

void HIFUNavigation::on_Maindialog_Close()
{

}

QWidget* HIFUNavigation::GetWindowWidget()
{
    return _pMdlg;
}
