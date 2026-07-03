#ifndef NEWSCENE3D_H
#define NEWSCENE3D_H

#include "newscene3d_global.h"
#include <QObject>
#include "QVBoxLayout"
#include <QColor>



class QVTKWrap;

class NEWSCENE3DSHARED_EXPORT NewScene3D:public QObject
{
    Q_OBJECT
public:
    NewScene3D(QObject *parent = 0);
    NewScene3D(QVBoxLayout* frame,QWidget *parent = 0);

    bool isInit;
    void InitScene3D();
    void LoadTumorStl(const char* path,double *centerOffset);
    void Init_IrradiationPoint(double *centerOffset);
    void Init_IrradiationPoint(double *centerOffset,QMap<QString,QVector<double> > &SpotList,int HeadDirection=0);
    void ClearSpot();

    void SetHeadDirection(int direction);
    inline int GetHeadDirection(){return PatientHeadDirection;}

    void ChangeUsImg(double angle);

private slots :
    void setSpotColor(QString id,QColor color);

private:
    QVTKWrap *_qvtkWarp;
    QVBoxLayout* wigetFrame;
    QWidget* wigetParent;
    int PatientHeadDirection;  //头的朝向 0：正对床看朝左侧  1：正对床看朝右侧
};

#endif // NEWSCENE3D_H
