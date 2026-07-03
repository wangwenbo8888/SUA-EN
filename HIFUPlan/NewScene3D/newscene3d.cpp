#include "newscene3d.h"
#include "qvtkwrap.h"

NewScene3D::NewScene3D(QObject *parent):QObject(parent)
{
}

NewScene3D::NewScene3D(QVBoxLayout* frame,QWidget *parent):
    QObject(parent),
    isInit(false),
    _qvtkWarp(NULL),
    wigetFrame(frame),
    wigetParent(parent)
{

}

void NewScene3D::InitScene3D()
{
    if(isInit||_qvtkWarp!=NULL)
        return;

    _qvtkWarp=new QVTKWrap();
    _qvtkWarp->Wrap_Init_Scene3d(wigetFrame,wigetParent);
    isInit=true;
}

void NewScene3D::LoadTumorStl(const char* path,double *centerOffset)
{
    if(_qvtkWarp==NULL)
        return;
    _qvtkWarp->Wrap_LoadTumorStl(path,centerOffset);
}

void NewScene3D::Init_IrradiationPoint(double *centerOffset)
{
    if(_qvtkWarp==NULL)
        return;
    _qvtkWarp->Wrap_Init_IrradiationPoint(centerOffset);
}

void NewScene3D::Init_IrradiationPoint(double *centerOffset,QMap<QString,QVector<double> > &SpotList,int HeadDirection)
{
    if(_qvtkWarp==NULL)
        return;
    PatientHeadDirection=HeadDirection;
    _qvtkWarp->Wrap_Init_IrradiationPoint(centerOffset,SpotList,HeadDirection);
}

void NewScene3D::setSpotColor(QString id,QColor color)
{
    if(_qvtkWarp==NULL)
        return;
    _qvtkWarp->Wrap_setSpotColor(id,color);
}

void NewScene3D::SetHeadDirection(int direction)
{
    PatientHeadDirection=direction;
    if(_qvtkWarp==NULL)
        return;
    _qvtkWarp->PatientHeadDirection=direction;
}

void NewScene3D::ClearSpot()
{
    if(_qvtkWarp==NULL)
        return;
    _qvtkWarp->Wrap_ClearSpot();
}

void NewScene3D::ChangeUsImg(double angle)
{
    if(_qvtkWarp==NULL)
        return;
    _qvtkWarp->Wrap_ChangeUsImg(angle);
}
