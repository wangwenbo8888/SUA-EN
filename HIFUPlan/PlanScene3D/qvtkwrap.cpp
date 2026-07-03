#include "qvtkwrap.h"
#include <QDebug>
#include <QDir>

QVTKWrap::QVTKWrap(QObject *parent) :
    QObject(parent),
    _qvtkWIdgetContextMenu(NULL)
{
     _vtkQtSlotConnect=vtkSmartPointer<vtkEventQtSlotConnect>::New();
     _ShowTuborElement=NULL;
     _ShowIrraPointElement=NULL;
     _ShowIrraPointAllLevel=NULL;
     _ShowIrraPointTopLevel=NULL;
     _ShowIrraPointMidlleLevel=NULL;
     _ShowIrraPointDownLevel=NULL;
     _ShowTumorNoOpacity=NULL;
     _ShowTumorLowOpacity=NULL;
     _ShowTumorMidlleOpacity=NULL;
     _ShowTumorHighOpacity=NULL;
     _LockHumanModel=NULL;
     PatientHeadDirection=0;
     isInitCam=false;
     bInitIrraPoint=false;
}

QVTKWrap::~QVTKWrap()
{

}

void QVTKWrap::Wrap_Init_Scene3d(QVBoxLayout* frame,QWidget *parent)
{
    _qvtkWidget_Scene3d=new QVTKWidget(parent);
    frame->addWidget(_qvtkWidget_Scene3d);
    //_qvtkWidget_Scene3d->setAutoBufferSwap(false);
    //_qvtkWidget_Scene3d->SetUseTDx(true);
    _Scene3d_Renderer=vtkSmartPointer<vtkRenderer>::New();
    _qvtkWidget_Scene3d->GetRenderWindow()->AddRenderer(_Scene3d_Renderer);

    _Scene3d_TransducerStlReader=vtkSmartPointer<vtkSTLReader>::New();
    _Scene3d_TransducerMapper=vtkSmartPointer<vtkPolyDataMapper>::New();
    _Scene3d_TransducerActor=vtkSmartPointer<vtkActor>::New();
    _Scene3d_TransducerStlReader->SetFileName(TRANSDUCER_PATH.toStdString().c_str());
    _Scene3d_TransducerStlReader->Update();
    vtkSmartPointer<vtkTransform> translation =vtkSmartPointer<vtkTransform>::New();
    double matrix[16] = {1.0,0.0,0.0,0.0,
                         0.0,-1.0,0.0,-140.0,
                         0.0,0.0,1.0,0.0,
                         0.0,0.0,0.0,0.0};
    translation->SetMatrix(matrix);
    vtkSmartPointer<vtkTransformPolyDataFilter> transformFilter = vtkSmartPointer<vtkTransformPolyDataFilter>::New();
    transformFilter->SetInputConnection(_Scene3d_TransducerStlReader->GetOutputPort());
    transformFilter->SetTransform(translation);
    transformFilter->Update();
    _Scene3d_TransducerMapper->SetInputConnection(transformFilter->GetOutputPort());
    _Scene3d_TransducerActor->SetMapper(_Scene3d_TransducerMapper);
    _Scene3d_TransducerActor->GetProperty()->SetColor(1.0,1.0,1.0);

    _Scene3d_TumorStlReader=vtkSmartPointer<vtkSTLReader>::New();
    _Scene3d_TumorMapper=vtkSmartPointer<vtkPolyDataMapper>::New();
    _Scene3d_TumorActor=vtkSmartPointer<vtkActor>::New();

    _Scene3d_HumanStlReader=vtkSmartPointer<vtkSTLReader>::New();
    _Scene3d_HumanMapper=vtkSmartPointer<vtkPolyDataMapper>::New();
    _Scene3d_HumanActor=vtkSmartPointer<vtkActor>::New();

    _Scene3d_OrientationWidget=vtkSmartPointer<vtkOrientationMarkerWidget>::New();
    _Scene3d_CubeAxes=vtkSmartPointer<vtkAnnotatedCubeActor>::New();

    _Scene3d_BlockStlReader=vtkSmartPointer<vtkSTLReader>::New();
    _Scene3d_BlockTransformFilter=vtkSmartPointer<vtkTransformPolyDataFilter>::New();

    _Scene3d_BlockStlReader->SetFileName(BLOCK_FPATH.toStdString().c_str());
    _Scene3d_BlockStlReader->Update();

    _Scene3d_HumanStlReader->SetFileName(HUMANMODEL_FPATH.toStdString().c_str());
    _Scene3d_HumanStlReader->Update();
    _Scene3d_HumanMapper->SetInputConnection(_Scene3d_HumanStlReader->GetOutputPort());
    _Scene3d_HumanActor->SetMapper(_Scene3d_HumanMapper);
    _Scene3d_HumanActor->GetProperty()->SetColor(1,0.49,0.25);

    _Scene3d_Renderer->SetBackground(0.0, 0.2265625, 0.421875); // Background color
    _Scene3d_OrientationWidget->SetOutlineColor( 0.9300, 0.5700, 0.1300 );
    _Scene3d_OrientationWidget->SetOrientationMarker( _Scene3d_HumanActor );
    _Scene3d_OrientationWidget->SetInteractor(_qvtkWidget_Scene3d->GetRenderWindow()->GetInteractor());
    _Scene3d_OrientationWidget->SetViewport( 0.0, 0.0, 0.35, 0.35 );
    _Scene3d_OrientationWidget->SetEnabled( 1 );
    _Scene3d_OrientationWidget->InteractiveOn();

    _Scene3d_Renderer->ResetCamera();
    _Scene3d_Renderer->AddActor(_Scene3d_TransducerActor);
    _qvtkWidget_Scene3d->GetRenderWindow()->Render();

    _vtkQtSlotConnect->Connect(_qvtkWidget_Scene3d->GetRenderWindow()->GetInteractor(),vtkCommand::RightButtonPressEvent,
                               this,SLOT(Scene3d_qvtkWidget_ContextMenu(vtkObject*,ulong,void*,void*,vtkCommand *)));
    InitqvtkWIdgetContextMenu();
    NowIrraPointKey="";
}

void QVTKWrap::Wrap_LoadTumorStl(const char* path,double *centerOffset,float size)
{
    _Scene3d_TumorStlReader->SetFileName(path);
    _Scene3d_TumorStlReader->Update();
    double *TumorCenter=_Scene3d_TumorStlReader->GetOutput()->GetCenter();

    vtkSmartPointer<vtkTransform> translation =vtkSmartPointer<vtkTransform>::New();
    translation ->Translate(-TumorCenter[0],-TumorCenter[1],-TumorCenter[2]);
    translation ->Translate(centerOffset[0],centerOffset[1],centerOffset[2]);
    translation->Scale(size,size,size);
    vtkSmartPointer<vtkTransformPolyDataFilter> transformFilter = vtkSmartPointer<vtkTransformPolyDataFilter>::New();
    transformFilter->SetInputConnection(_Scene3d_TumorStlReader->GetOutputPort());
    transformFilter->SetTransform(translation);
    transformFilter->Update();

    _Scene3d_TumorMapper->SetInputConnection(transformFilter->GetOutputPort());
    _Scene3d_TumorActor->SetMapper(_Scene3d_TumorMapper);
    _Scene3d_TumorActor->GetProperty()->SetOpacity(0.5);
    _Scene3d_TumorActor->GetProperty()->BackfaceCullingOn();
    _Scene3d_Renderer->RemoveActor(_Scene3d_TumorActor);
    _Scene3d_Renderer->AddActor(_Scene3d_TumorActor);

    _qvtkWidget_Scene3d->GetRenderWindow()->Render();
}

void QVTKWrap::Wrap_Init_IrradiationPoint(double *centerOffset)
{
    QMap<QString,vtkSmartPointer<vtkActor> >::const_iterator i = _Scene3d_IrradiationPointActorMap.constBegin();
    while (i != _Scene3d_IrradiationPointActorMap.constEnd()) {
        _Scene3d_Renderer->RemoveActor(i.value());
        ++i;
    }
    double center[3]={0,0,0};
    center[0]=center[0]+centerOffset[0];
    center[1]=center[1]+centerOffset[1];
    center[2]=center[2]+centerOffset[2];
    _Scene3d_BlockPloyData.clear();
    for(int c=-1;c<2;c++)
    {
        double nCenter1=center[1]+c*10;

        vtkSmartPointer<vtkTransform> translation=vtkSmartPointer<vtkTransform>::New();
        translation->Translate(center[0],nCenter1,center[2]);
        _Scene3d_BlockTransformFilter->SetInputConnection(_Scene3d_BlockStlReader->GetOutputPort());
        _Scene3d_BlockTransformFilter->SetTransform(translation);
        _Scene3d_BlockTransformFilter->Update();
        vtkSmartPointer<vtkPolyData> blockData=vtkSmartPointer<vtkPolyData>::New();
        blockData->DeepCopy(_Scene3d_BlockTransformFilter->GetOutput());
        _Scene3d_BlockPloyData.append(blockData);

        for(int q=1;q<4;q++)
        {
            for(int p=0;p<(6*q);p++)
            {
                double angle=2.0*3.1415926*double((double)p/(6*(double)q));
                double ncx=center[0]+1.8*(double)q*cos(angle);
                double ncz=center[2]+1.8*(double)q*sin(angle);

                vtkSmartPointer<vtkTransform> translation =vtkSmartPointer<vtkTransform>::New();
                translation ->Translate(ncx,nCenter1,ncz);
                _Scene3d_BlockTransformFilter->SetInputConnection(_Scene3d_BlockStlReader->GetOutputPort());
                _Scene3d_BlockTransformFilter->SetTransform(translation);
                _Scene3d_BlockTransformFilter->Update();
                vtkSmartPointer<vtkPolyData> blockData=vtkSmartPointer<vtkPolyData>::New();
                blockData->DeepCopy(_Scene3d_BlockTransformFilter->GetOutput());
                _Scene3d_BlockPloyData.append(blockData);
            }
        }
    }
    _Scene3d_IrradiationPointMapper.clear();
    _Scene3d_IrradiationPointActorMap.clear();

    for(int i=0;i<_Scene3d_BlockPloyData.count();i++)
    {
        vtkSmartPointer<vtkPolyDataMapper> mapper =
                vtkSmartPointer<vtkPolyDataMapper>::New();
        mapper->SetInputData(_Scene3d_BlockPloyData[i]);

        vtkSmartPointer<vtkActor> actor =
                vtkSmartPointer<vtkActor>::New();
        actor->SetMapper(mapper);
        _Scene3d_IrradiationPointMapper.append(mapper);
        _Scene3d_IrradiationPointActorMap.insert(QString("%1").arg(i),actor);
        _Scene3d_Renderer->AddActor(actor);
    }

    _Scene3d_Renderer->GetActiveCamera()->SetPosition(0,0,1) ;
    _Scene3d_Renderer->GetActiveCamera()->SetViewUp(0,1,0) ;
    _Scene3d_Renderer->GetActiveCamera()->SetFocalPoint(0,0,0);
    _Scene3d_Renderer->ResetCamera() ;

     _Scene3d_Renderer->GetActiveCamera()->Roll(180);
     if(PatientHeadDirection)
        _Scene3d_Renderer->GetActiveCamera()->Azimuth(-90);
    else
        _Scene3d_Renderer->GetActiveCamera()->Azimuth(90);
    _Scene3d_Renderer->GetActiveCamera()->Zoom(0.5);
    _qvtkWidget_Scene3d->GetRenderWindow()->Render();

    bInitIrraPoint=true;
}

void QVTKWrap::Wrap_Init_IrradiationPoint(double *centerOffset,QMap<QString,QVector<double> > &SpotList,int HeadDirection)
{
    PatientHeadDirection=HeadDirection;
#ifdef QT_DEBUG
    qDebug()<<SpotList.count();
#endif
    //double *center=_Scene3d_TumorMapper->GetCenter();
    double center[3]={0,0,0};
    double ncenter[3];
    ncenter[0]=center[0]+centerOffset[0];
    ncenter[1]=center[1]+centerOffset[1];
    ncenter[2]=center[2]+centerOffset[2];
    QMap<QString,vtkSmartPointer<vtkActor> >::const_iterator i = _Scene3d_IrradiationPointActorMap.constBegin();
    while (i != _Scene3d_IrradiationPointActorMap.constEnd()) {
        _Scene3d_Renderer->RemoveActor(i.value());
        ++i;
    }
    _Scene3d_BlockPloyData.clear();
    _Scene3d_IrradiationPointMapper.clear();
    _Scene3d_IrradiationPointActorMap.clear();
    QMap<QString,QVector<double> >::const_iterator iSpot = SpotList.constBegin();
    while (iSpot != SpotList.constEnd()) {
#ifdef QT_DEBUG
        qDebug()<<"SpotList"<<iSpot.key()<<iSpot.value()[0]<<iSpot.value()[1]<<iSpot.value()[2];
#endif
        double oriZ=iSpot.value()[2];
        double tarx=0,tary=0,tarz=0;
        if(HeadDirection)
        {
            tarx=iSpot.value()[0]+ncenter[0];
            tary=-iSpot.value()[1]+ncenter[2];
        }
        else
        {
            tarx=-iSpot.value()[0]+ncenter[0];
            tary=iSpot.value()[1]+ncenter[2];
        }

        vtkSmartPointer<vtkTransform> translation =vtkSmartPointer<vtkTransform>::New();
        translation ->Translate(tarx,tarz,tary);
        _Scene3d_BlockTransformFilter->SetInputConnection(_Scene3d_BlockStlReader->GetOutputPort());
        _Scene3d_BlockTransformFilter->SetTransform(translation);
        _Scene3d_BlockTransformFilter->Update();
        vtkSmartPointer<vtkPolyData> blockData=vtkSmartPointer<vtkPolyData>::New();
        blockData->DeepCopy(_Scene3d_BlockTransformFilter->GetOutput());
        _Scene3d_BlockPloyData.append(blockData);

        vtkSmartPointer<vtkPolyDataMapper> mapper =
                vtkSmartPointer<vtkPolyDataMapper>::New();
        mapper->SetInputData(blockData);

        vtkSmartPointer<vtkActor> actor =
                vtkSmartPointer<vtkActor>::New();
        actor->SetMapper(mapper);
        _Scene3d_IrradiationPointMapper.append(mapper);
        _Scene3d_IrradiationPointActorMap.insert(iSpot.key(),actor);
        _Scene3d_Renderer->AddActor(actor);
        ++iSpot;
    }

    _Scene3d_Renderer->GetActiveCamera()->SetPosition(0,0,1) ;
    _Scene3d_Renderer->GetActiveCamera()->SetViewUp(0,1,0) ;
    _Scene3d_Renderer->GetActiveCamera()->SetFocalPoint(0,0,0);
    _Scene3d_Renderer->ResetCamera() ;

     _Scene3d_Renderer->GetActiveCamera()->Roll(180);
     if(PatientHeadDirection)
        _Scene3d_Renderer->GetActiveCamera()->Azimuth(-90);
    else
        _Scene3d_Renderer->GetActiveCamera()->Azimuth(90);
    _Scene3d_Renderer->GetActiveCamera()->Zoom(0.5);
    _qvtkWidget_Scene3d->GetRenderWindow()->Render();

    bInitIrraPoint=true;
}

void QVTKWrap::InitqvtkWIdgetContextMenu()
{
    _qvtkWIdgetContextMenu=new QMenu();

    connect(_qvtkWIdgetContextMenu,SIGNAL(aboutToHide()),this,SLOT(Scene3d_ContextMenu_Hide()));

    if (_ShowIrraPointElement==NULL)
    {
        _ShowIrraPointElement = _qvtkWIdgetContextMenu->addAction("Hide Sonication Points");
        connect(_ShowIrraPointElement,SIGNAL(triggered(bool)),this,SLOT(Scene3d_ContextAction_ShowIrraPoint()));
    }
    QMenu* levelMenu = _qvtkWIdgetContextMenu->addMenu(tr("Sonication Plane Selection"));

    if (_ShowIrraPointAllLevel==NULL)
    {
        _ShowIrraPointAllLevel = levelMenu->addAction("Show All Planes");
        connect(_ShowIrraPointAllLevel,SIGNAL(triggered(bool)),this,SLOT(Scene3d_ContextAction_ShowIrraPointAllLevel()));
    }

    if (_ShowIrraPointTopLevel==NULL)
    {
        _ShowIrraPointTopLevel = levelMenu->addAction("Show Upper Plane");
        connect(_ShowIrraPointTopLevel,SIGNAL(triggered(bool)),this,SLOT(Scene3d_ContextAction_ShowIrraPointTopLevel()));
    }
    if (_ShowIrraPointMidlleLevel==NULL)
    {
        _ShowIrraPointMidlleLevel = levelMenu->addAction("Show Middle Plane");
        connect(_ShowIrraPointMidlleLevel,SIGNAL(triggered(bool)),this,SLOT(Scene3d_ContextAction_ShowIrraPointMidlleLevel()));
    }

    if (_ShowIrraPointDownLevel==NULL)
    {
        _ShowIrraPointDownLevel = levelMenu->addAction("Show Lower Plane");
        connect(_ShowIrraPointDownLevel,SIGNAL(triggered(bool)),this,SLOT(Scene3d_ContextAction_ShowIrraPointDownLevel()));
    }
    _qvtkWIdgetContextMenu->addSeparator();
    if (_ShowTuborElement==NULL)
    {
        _ShowTuborElement = _qvtkWIdgetContextMenu->addAction("Hide Fibroid Model");
        connect(_ShowTuborElement,SIGNAL(triggered(bool)),this,SLOT(Scene3d_ContextAction_ShowTumor()));
    }
    QMenu* OpacityMenu = _qvtkWIdgetContextMenu->addMenu(tr("Fibroid Opacity "));
    if (_ShowTumorNoOpacity==NULL)
    {
        _ShowTumorNoOpacity = OpacityMenu->addAction("Opaque");
        connect(_ShowTumorNoOpacity,SIGNAL(triggered(bool)),this,SLOT(Scene3d_ContextAction_ShowTumorNoOpacity()));
    }
    if (_ShowTumorLowOpacity==NULL)
    {
        _ShowTumorLowOpacity = OpacityMenu->addAction("Low Opacity");
        connect(_ShowTumorLowOpacity,SIGNAL(triggered(bool)),this,SLOT(Scene3d_ContextAction_ShowTumorLowOpacity()));
    }

    if (_ShowTumorMidlleOpacity==NULL)
    {
        _ShowTumorMidlleOpacity = OpacityMenu->addAction("Medium Opacity ");
        connect(_ShowTumorMidlleOpacity,SIGNAL(triggered(bool)),this,SLOT(Scene3d_ContextAction_ShowTumorMidlleOpacity()));
    }
    if (_ShowTumorHighOpacity==NULL)
    {
        _ShowTumorHighOpacity = OpacityMenu->addAction("High Opacity");
        connect(_ShowTumorHighOpacity,SIGNAL(triggered(bool)),this,SLOT(Scene3d_ContextAction_ShowTumorHighOpacity()));
    }
    _qvtkWIdgetContextMenu->addSeparator();
    if (_LockHumanModel==NULL)
    {
        _LockHumanModel = _qvtkWIdgetContextMenu->addAction("Lock Body Model");
        connect(_LockHumanModel,SIGNAL(triggered(bool)),this,SLOT(Scene3d_ContextAction_LockHumanModel()));
    }
}
void QVTKWrap::Scene3d_ContextMenu_Hide()
{
    _qvtkWidget_Scene3d->GetRenderWindow()->GetInteractor()->RightButtonReleaseEvent();
}

void QVTKWrap::Scene3d_ContextAction_ShowTumor()
{
    int iVisible=_Scene3d_TumorActor->GetVisibility();
    if(iVisible)
    {
        _Scene3d_TumorActor->SetVisibility(0);
        _ShowTuborElement->setText("Show Fibroid Model");
    }
    else
    {
        _Scene3d_TumorActor->SetVisibility(1);
        _ShowTuborElement->setText("Hide Fibroid Model");
    }
    _qvtkWidget_Scene3d->GetRenderWindow()->Render();
}
void QVTKWrap::Scene3d_ContextAction_ShowIrraPoint()
{
    if(_Scene3d_IrradiationPointActorMap.count()<=0)
        return;
    QMap<QString,vtkSmartPointer<vtkActor> >::const_iterator i = _Scene3d_IrradiationPointActorMap.constBegin();
    int iVisible=i.value()->GetVisibility();
    if(iVisible)
    {
        while (i != _Scene3d_IrradiationPointActorMap.constEnd()) {
            i.value()->SetVisibility(0);
            ++i;
        }
        _ShowIrraPointElement->setText("Show Sonication Area");
    }
    else
    {
        while (i != _Scene3d_IrradiationPointActorMap.constEnd()) {
            i.value()->SetVisibility(1);
            ++i;
        }
        _ShowIrraPointElement->setText("Hide Sonication Area");
    }
    _qvtkWidget_Scene3d->GetRenderWindow()->Render();
}
void QVTKWrap::Scene3d_ContextAction_ShowIrraPointAllLevel()
{
    QMap<QString,vtkSmartPointer<vtkActor> >::const_iterator i = _Scene3d_IrradiationPointActorMap.constBegin();
    while (i != _Scene3d_IrradiationPointActorMap.constEnd()) {
        i.value()->GetProperty()->SetOpacity(1);
        ++i;
    }
    _qvtkWidget_Scene3d->GetRenderWindow()->Render();
}
void QVTKWrap::Scene3d_ContextAction_ShowIrraPointTopLevel()
{
    QMap<QString,vtkSmartPointer<vtkActor> >::const_iterator i = _Scene3d_IrradiationPointActorMap.constBegin();
    while (i != _Scene3d_IrradiationPointActorMap.constEnd()) {
        if(i.value()->GetCenter()[1]<0)
        {
            i.value()->GetProperty()->SetOpacity(1);
        }
        else
        {
            i.value()->GetProperty()->SetOpacity(0.2);
            i.value()->GetProperty()->BackfaceCullingOn();
        }
        ++i;
    }
    _qvtkWidget_Scene3d->GetRenderWindow()->Render();
}
void QVTKWrap::Scene3d_ContextAction_ShowIrraPointMidlleLevel()
{
    QMap<QString,vtkSmartPointer<vtkActor> >::const_iterator i = _Scene3d_IrradiationPointActorMap.constBegin();
    while (i != _Scene3d_IrradiationPointActorMap.constEnd()) {
        if(i.value()->GetCenter()[1]==0)
        {
            i.value()->GetProperty()->SetOpacity(1);
        }
        else
        {
            i.value()->GetProperty()->SetOpacity(0.2);
            i.value()->GetProperty()->BackfaceCullingOn();
        }
        ++i;
    }
    _qvtkWidget_Scene3d->GetRenderWindow()->Render();
}
void QVTKWrap::Scene3d_ContextAction_ShowIrraPointDownLevel()
{
    QMap<QString,vtkSmartPointer<vtkActor> >::const_iterator i = _Scene3d_IrradiationPointActorMap.constBegin();
    while (i != _Scene3d_IrradiationPointActorMap.constEnd()) {
        if(i.value()->GetCenter()[1]>0)
        {
            i.value()->GetProperty()->SetOpacity(1);
        }
        else
        {
            i.value()->GetProperty()->SetOpacity(0.2);
            i.value()->GetProperty()->BackfaceCullingOn();
        }
        ++i;
    }
    _qvtkWidget_Scene3d->GetRenderWindow()->Render();
}
void QVTKWrap::Scene3d_ContextAction_ShowTumorNoOpacity()
{
    _Scene3d_TumorActor->GetProperty()->SetOpacity(1);
    _qvtkWidget_Scene3d->GetRenderWindow()->Render();
}
void QVTKWrap::Scene3d_ContextAction_ShowTumorLowOpacity()
{
    _Scene3d_TumorActor->GetProperty()->SetOpacity(0.8);
    _qvtkWidget_Scene3d->GetRenderWindow()->Render();
}
void QVTKWrap::Scene3d_ContextAction_ShowTumorMidlleOpacity()
{
    _Scene3d_TumorActor->GetProperty()->SetOpacity(0.5);
    _qvtkWidget_Scene3d->GetRenderWindow()->Render();
}
void QVTKWrap::Scene3d_ContextAction_ShowTumorHighOpacity()
{
    _Scene3d_TumorActor->GetProperty()->SetOpacity(0.3);
    _qvtkWidget_Scene3d->GetRenderWindow()->Render();
}
void QVTKWrap::Scene3d_ContextAction_LockHumanModel()
{
    if(_Scene3d_OrientationWidget->GetInteractive())
    {
        _Scene3d_OrientationWidget->InteractiveOff();
        _LockHumanModel->setText("Unlock Body Model");
    }
    else
    {
        _Scene3d_OrientationWidget->InteractiveOn();
        _LockHumanModel->setText("Lock Body Model");
    }
    _qvtkWidget_Scene3d->GetRenderWindow()->Render();
}

void QVTKWrap::Scene3d_qvtkWidget_ContextMenu(vtkObject * obj, unsigned long, void * client_data, void *, vtkCommand * command)
{
    vtkRenderWindowInteractor* iren = vtkRenderWindowInteractor::SafeDownCast(obj);
    command->AbortFlagOn();
    int* sz = iren->GetSize();
    int* position = iren->GetEventPosition();
    QPoint pt = QPoint(position[0], sz[1]-position[1]);
    QPoint global_pt = _qvtkWidget_Scene3d->parentWidget()->mapToGlobal(pt);
    _qvtkWIdgetContextMenu->exec(global_pt);
}

void QVTKWrap::SetNowIrraPoint(QString idBound)
{
    if(!NowIrraPointKey.isEmpty())
    {
        _Scene3d_IrradiationPointActorMap[NowIrraPointKey]->GetProperty()->SetColor(1,1,1);
    }
    _Scene3d_IrradiationPointActorMap[idBound]->GetProperty()->SetColor(0,1,0);
    NowIrraPointKey=idBound;
}

void QVTKWrap::Wrap_setBlockColor(QString id,QColor color)
{
    if(_Scene3d_IrradiationPointActorMap.count()<=0)
        return;
    if(!_Scene3d_IrradiationPointActorMap.contains(id))
    {
        qDebug()<<"no key in ActorMap"<<id<<_Scene3d_IrradiationPointActorMap.count();
        return;
    }
    double scolor[3];
    scolor[0]=((double)color.red())/255.0;
    scolor[1]=((double)color.green())/255.0;
    scolor[2]=((double)color.blue())/255.0;
    _Scene3d_IrradiationPointActorMap[id]->GetProperty()->SetColor(scolor);
    _qvtkWidget_Scene3d->GetRenderWindow()->Render();
}

void QVTKWrap::Wrap_ClearTumor()
{
    _Scene3d_TumorMapper->RemoveAllInputs();
    _Scene3d_Renderer->RemoveActor(_Scene3d_TumorActor);
    _qvtkWidget_Scene3d->GetRenderWindow()->Render();
}

void QVTKWrap::Wrap_ClearSpot()
{
    if(_Scene3d_IrradiationPointActorMap.count()==0)
        return;
    QMap<QString,vtkSmartPointer<vtkActor> >::const_iterator i = _Scene3d_IrradiationPointActorMap.constBegin();
    while (i != _Scene3d_IrradiationPointActorMap.constEnd()) {
        _Scene3d_Renderer->RemoveActor(i.value());
        ++i;
    }
    _Scene3d_BlockPloyData.clear();
    _Scene3d_IrradiationPointMapper.clear();
    _Scene3d_IrradiationPointActorMap.clear();
    _qvtkWidget_Scene3d->GetRenderWindow()->Render();
    bInitIrraPoint=false;
}

void QVTKWrap::Wrap_ChangeUsImg(double angle,QImage &img)
{
//    m_curUsImg=img;
//    m_dUSImgAngle=angle;
//    if(!m_bShowUsImg)
//        return;
//    updateUsImg();
}

void QVTKWrap::Wrap_ChangeUsImg(double angle,QPixmap &pix)
{
//    QImage img=pix.toImage();
//    Wrap_ChangeUsImg(angle,img);
}

//void QVTKWrap::Wrap_ChangeUsImg(double angle)
//{
//    if(!bInitIrraPoint)
//        return;
//    m_dUSImgAngle=angle;
//    if(!m_bShowUsImg)
//        return;
//    updateUsImg();
//}
