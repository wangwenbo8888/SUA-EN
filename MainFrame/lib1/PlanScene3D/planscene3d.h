#ifndef PLANSCENE3D_H_
#define PLANSCENE3D_H_

#include "planscene3d_global.h"
#include <QObject>
#include "QVBoxLayout"
#include <QColor>

class QVTKWrap;

class PLANSCENE3DSHARED_EXPORT PlanScene3D:public QObject
{
    Q_OBJECT
public:
    PlanScene3D(QObject *parent = 0);
    PlanScene3D(QVBoxLayout* frame,QWidget *parent = 0);

    bool isInit;
    void InitScene3D();
    void LoadTumorStl(const char* path,double *centerOffset,float size);
    void Init_IrradiationPoint(double *centerOffset);
    void Init_IrradiationPoint(double *centerOffset,QMap<QString,QVector<double> > &SpotList,int HeadDirection=0);
    void ClearSpot();
    void ClearTumor();

    void SetHeadDirection(int direction);
    inline int GetHeadDirection(){return PatientHeadDirection;}

private slots :
    void setBlockColor(QString id,QColor color);

private:
    QVTKWrap *_qvtkWarp;
    QVBoxLayout* wigetFrame;
    QWidget* wigetParent;
    int PatientHeadDirection;  //头的朝向 0：正对床看朝左侧  1：正对床看朝右侧
};

#endif // PLANSCENE3D_H
