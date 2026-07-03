#ifndef HIFUCONNECTION_H
#define HIFUCONNECTION_H

#include <qobject.h>
#include "HIFUConnection_global.h"

#include <QImage>
#include <QMap>

class MainDialog;

class HIFUCONNECTIONSHARED_EXPORT HIFUConnection : public QObject
{
    Q_OBJECT

public:
    HIFUConnection();
    ~HIFUConnection();

    void showWindow();
    void ClearData();

    QWidget* GetWindowWidget();

signals:
//    void sendQImage(QImage,int);

//    // void sendContour(QImage,QImage,int);
//    void sendContour(QImage,int);

//    // 配准介质，肌膜轮廓
//    void sendOuter(QImage,int);

//    void sendSearchRadius(int);

//    void sendOrientation(int);

//    void sendGetContour();

public slots:

//    void receiveGetContour();

//    void sendImageData(QImage image,int angle);

//    void receiveSearchRadius(int radius);

//    void receivePatientOrientation(int ori);

//    void receiveContour(QImage image,int angle);

//    void receiveContourNew(QImage contour,int angle);

    void on_Maindialog_Close();

private:

    // 保存接收到的肌膜轮廓
    // QMap<int,QImage> m_mapContours;

    MainDialog* _pMdlg;

};

#endif // HIFUNAVIGATION_H
