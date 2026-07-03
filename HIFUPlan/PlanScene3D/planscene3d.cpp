#include "planscene3d.h"
#include "qvtkwrap.h"

PlanScene3D::PlanScene3D(QObject *parent):QObject(parent)
{
}

PlanScene3D::PlanScene3D(QVBoxLayout* frame,QWidget *parent):
    QObject(parent),
    isInit(false),
    _qvtkWarp(NULL),
    wigetFrame(frame),
    wigetParent(parent)
{
}

void PlanScene3D::InitScene3D()
{
    if(isInit||_qvtkWarp!=NULL)
        return;

    _qvtkWarp=new QVTKWrap();
    _qvtkWarp->Wrap_Init_Scene3d(wigetFrame,wigetParent);
    isInit=true;
}

void PlanScene3D::LoadTumorStl(const char* path,double *centerOffset,float size)
{
    if(_qvtkWarp==NULL)
        return;
    _qvtkWarp->Wrap_LoadTumorStl(path,centerOffset,size);
}

void PlanScene3D::Init_IrradiationPoint(double *centerOffset)
{
    if(_qvtkWarp==NULL)
        return;
    _qvtkWarp->Wrap_Init_IrradiationPoint(centerOffset);
}

void PlanScene3D::Init_IrradiationPoint(double *centerOffset,QMap<QString,QVector<double> > &SpotList,int HeadDirection)
{
    if(_qvtkWarp==NULL)
        return;
    PatientHeadDirection=HeadDirection;
    _qvtkWarp->Wrap_Init_IrradiationPoint(centerOffset,SpotList,HeadDirection);
}

void PlanScene3D::setBlockColor(QString id,QColor color)
{
    if(_qvtkWarp==NULL)
        return;
    _qvtkWarp->Wrap_setBlockColor(id,color);
}

void PlanScene3D::SetHeadDirection(int direction)
{
    PatientHeadDirection=direction;
    if(_qvtkWarp==NULL)
        return;
    _qvtkWarp->PatientHeadDirection=direction;
}

void PlanScene3D::ClearTumor()
{
    if(_qvtkWarp==NULL)
        return;
    _qvtkWarp->Wrap_ClearTumor();
}

void PlanScene3D::ClearSpot()
{
    if(_qvtkWarp==NULL)
        return;
    _qvtkWarp->Wrap_ClearSpot();
}

