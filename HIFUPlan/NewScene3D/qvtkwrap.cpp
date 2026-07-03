#include "qvtkwrap.h"
#include <QDebug>
#include <QDir>
#include <vtkTexture.h>
#include <qtextcodec.h>

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
     _ShowUsImgPlane=NULL;
     PatientHeadDirection=0;
     isInitCam=false;
     bInitIrraPoint=false;
     m_bShowUsImg=false;
     m_bAddUsActor=false;
     m_dUSImgAngle=-100;
     //m_curUsImg=QImage();

     m_pSampleUsImg=new QImage();
     if(!m_pSampleUsImg->load(QString::fromUtf8(":/smapleusimg.png"),"PNG"))
     {
         qDebug()<<"NewScene3D 加载示例超声图像失败";
     }
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

    _Scene3d_TumorStlReader=vtkSmartPointer<vtkSTLReader>::New();
    _Scene3d_TumorMapper=vtkSmartPointer<vtkPolyDataMapper>::New();
    _Scene3d_TumorActor=vtkSmartPointer<vtkActor>::New();
    _Scene3d_HumanStlReader=vtkSmartPointer<vtkSTLReader>::New();
    _Scene3d_HumanMapper=vtkSmartPointer<vtkPolyDataMapper>::New();
    _Scene3d_HumanActor=vtkSmartPointer<vtkActor>::New();
    _Scene3d_OrientationWidget=vtkSmartPointer<vtkOrientationMarkerWidget>::New();
    _Scene3d_CubeAxes=vtkSmartPointer<vtkAnnotatedCubeActor>::New();
    _Scene3d_SpotStlReader=vtkSmartPointer<vtkSTLReader>::New();
    _Scene3d_SpotTransformFilter=vtkSmartPointer<vtkTransformPolyDataFilter>::New();

    //超声平面初始化
    UsImgQImageToImageSource=vtkSmartPointer<vtkQImageToImageSource>::New();
    UsImgTexture=vtkSmartPointer<vtkTexture>::New();
    UsImgPlaneSource=vtkSmartPointer<vtkPlaneSource>::New();
    UsImgPlaneMapper=vtkSmartPointer<vtkPolyDataMapper>::New();
    UsImgPlaneActor=vtkSmartPointer<vtkActor>::New();
    UsImgPlaneActor->GetProperty()->SetOpacity(0.5);    
    UsImgQImageToImageSource->SetQImage(m_pSampleUsImg);
    UsImgQImageToImageSource->Update();
    UsImgTexture->SetInputConnection(0,UsImgQImageToImageSource->GetOutputPort());
    UsImgTexture->Update();
    UsImgPlaneSource->SetCenter(0,0,0);
    UsImgPlaneMapper->SetInputConnection(0,UsImgPlaneSource->GetOutputPort());
    UsImgPlaneMapper->Update();
    UsImgPlaneActor->SetMapper(UsImgPlaneMapper);
    UsImgPlaneActor->SetTexture(UsImgTexture);

    _Scene3d_SpotStlReader->SetFileName(SPOT_FPATH.toStdString().c_str());
    _Scene3d_SpotStlReader->Update();

    _Scene3d_HumanStlReader->SetFileName(HUMANMODEL_FPATH.toStdString().c_str());
    _Scene3d_HumanStlReader->Update();
    _Scene3d_HumanMapper->SetInputConnection(_Scene3d_HumanStlReader->GetOutputPort());
    _Scene3d_HumanActor->SetMapper(_Scene3d_HumanMapper);
    _Scene3d_HumanActor->GetProperty()->SetColor(1,0.49,0.25);

//    _Scene3d_Renderer->SetBackground(0.0, 0.2265625, 0.421875); // Background color
    _Scene3d_Renderer->SetBackground(0.0, 0.0, 0.0); // Background color
    _Scene3d_OrientationWidget->SetOutlineColor( 0.9300, 0.5700, 0.1300 );
    _Scene3d_OrientationWidget->SetOrientationMarker( _Scene3d_HumanActor );
    _Scene3d_OrientationWidget->SetInteractor(_qvtkWidget_Scene3d->GetRenderWindow()->GetInteractor());
    _Scene3d_OrientationWidget->SetViewport( 0.0, 0.0, 0.35, 0.35 );
    _Scene3d_OrientationWidget->SetEnabled( 1 );
    _Scene3d_OrientationWidget->InteractiveOn();

    _Scene3d_Renderer->ResetCamera();
    _qvtkWidget_Scene3d->GetRenderWindow()->Render();

    _vtkQtSlotConnect->Connect(_qvtkWidget_Scene3d->GetRenderWindow()->GetInteractor(),vtkCommand::RightButtonPressEvent,
                               this,SLOT(Scene3d_qvtkWidget_ContextMenu(vtkObject*,ulong,void*,void*,vtkCommand *)));
    InitqvtkWIdgetContextMenu();
    NowIrraPointKey="";
}

void QVTKWrap::Wrap_LoadTumorStl(const char* path,double *centerOffset)
{
    _Scene3d_TumorStlReader->SetFileName(path);
    _Scene3d_TumorStlReader->Update();
    double *TumorCenter=_Scene3d_TumorStlReader->GetOutput()->GetCenter();

    vtkSmartPointer<vtkTransform> translation =vtkSmartPointer<vtkTransform>::New();
    translation ->Translate(-TumorCenter[0],-TumorCenter[1],-TumorCenter[2]);
    translation ->Translate(centerOffset[0],centerOffset[1],centerOffset[2]);
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
    bool bShowUs=m_bShowUsImg;
    if(bShowUs){
        m_bShowUsImg=false;
        _Scene3d_Renderer->RemoveActor(UsImgPlaneActor);
        m_bAddUsActor=false;
    }
    QMap<QString,vtkSmartPointer<vtkActor> >::const_iterator i = _Scene3d_IrradiationPointActorMap.constBegin();
    while (i != _Scene3d_IrradiationPointActorMap.constEnd()) {
        _Scene3d_Renderer->RemoveActor(i.value());
        ++i;
    }
    double center[3]={0,0,0};
    center[0]=center[0]+centerOffset[0];
    center[1]=center[1]+centerOffset[1];
    center[2]=center[2]+centerOffset[2];
    _Scene3d_SpotPloyData.clear();
    for(int c=-1;c<2;c++)
    {
        double nCenter1=center[1]+c*10;

        vtkSmartPointer<vtkTransform> translation=vtkSmartPointer<vtkTransform>::New();
        translation->Translate(center[0],nCenter1,center[2]);
        _Scene3d_SpotTransformFilter->SetInputConnection(_Scene3d_SpotStlReader->GetOutputPort());
        _Scene3d_SpotTransformFilter->SetTransform(translation);
        _Scene3d_SpotTransformFilter->Update();
        vtkSmartPointer<vtkPolyData> spotData=vtkSmartPointer<vtkPolyData>::New();
        spotData->DeepCopy(_Scene3d_SpotTransformFilter->GetOutput());
        _Scene3d_SpotPloyData.append(spotData);

        for(int q=1;q<4;q++)
        {
            for(int p=0;p<(6*q);p++)
            {
                double angle=2.0*3.1415926*double((double)p/(6*(double)q));
                double ncx=center[0]+1.8*(double)q*cos(angle);
                double ncz=center[2]+1.8*(double)q*sin(angle);

                vtkSmartPointer<vtkTransform> translation =vtkSmartPointer<vtkTransform>::New();
                translation ->Translate(ncx,nCenter1,ncz);
                _Scene3d_SpotTransformFilter->SetInputConnection(_Scene3d_SpotStlReader->GetOutputPort());
                _Scene3d_SpotTransformFilter->SetTransform(translation);
                _Scene3d_SpotTransformFilter->Update();
                vtkSmartPointer<vtkPolyData> spotData=vtkSmartPointer<vtkPolyData>::New();
                spotData->DeepCopy(_Scene3d_SpotTransformFilter->GetOutput());
                _Scene3d_SpotPloyData.append(spotData);
            }
        }
    }
    _Scene3d_IrradiationPointMapper.clear();
    _Scene3d_IrradiationPointActorMap.clear();

    for(int i=0;i<_Scene3d_SpotPloyData.count();i++)
    {
        vtkSmartPointer<vtkPolyDataMapper> mapper =
                vtkSmartPointer<vtkPolyDataMapper>::New();
        mapper->SetInputData(_Scene3d_SpotPloyData[i]);

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

//    _Scene3d_Renderer->GetActiveCamera()->GetPosition(CamInitPosition);
//    _Scene3d_Renderer->GetActiveCamera()->GetFocalPoint(CamInitFocalPoint);
//    _Scene3d_Renderer->GetActiveCamera()->GetViewUp(CamInitViewUp);
//    qDebug()<<"CamInitPosition"<<CamInitPosition[0]<<CamInitPosition[1]<<CamInitPosition[2];
//    qDebug()<<"CamInitViewUp"<<CamInitViewUp[0]<<CamInitViewUp[1]<<CamInitViewUp[2];
//    qDebug()<<"CamInitFocalPoint"<<CamInitFocalPoint[0]<<CamInitFocalPoint[1]<<CamInitFocalPoint[2];

     _Scene3d_Renderer->GetActiveCamera()->Roll(180);
     if(PatientHeadDirection)
        _Scene3d_Renderer->GetActiveCamera()->Azimuth(-90);
    else
        _Scene3d_Renderer->GetActiveCamera()->Azimuth(90);
    _Scene3d_Renderer->GetActiveCamera()->Zoom(0.5);
    _qvtkWidget_Scene3d->GetRenderWindow()->Render();

    if(bShowUs){
        m_bShowUsImg=true;
        _Scene3d_Renderer->AddActor(UsImgPlaneActor);
        m_bAddUsActor=true;
        if(m_dUSImgAngle!=-100)
            updateUsImg();
    }else{
        m_bShowUsImg=true;
    }
    bInitIrraPoint=true;
}

void QVTKWrap::Wrap_Init_IrradiationPoint(double *centerOffset,QMap<QString,QVector<double> > &SpotList,int HeadDirection)
{
    bool bShowUs=m_bShowUsImg;
    if(bShowUs){
        m_bShowUsImg=false;
        _Scene3d_Renderer->RemoveActor(UsImgPlaneActor);
        m_bAddUsActor=false;
    }
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
    _Scene3d_SpotPloyData.clear();
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
            tarx=iSpot.value()[0]*1000.0+ncenter[0];
            tary=-iSpot.value()[1]*1000.0+ncenter[2];
        }
        else
        {
            tarx=-iSpot.value()[0]*1000.0+ncenter[0];
            tary=iSpot.value()[1]*1000.0+ncenter[2];
        }

        if (oriZ>0.145&&oriZ<0.155)
        {
            tarz=ncenter[1]+10;
        }
        else if (oriZ<0.145&&oriZ>0.135)
        {
            tarz=ncenter[1];
        }
        else if(oriZ<0.135&&oriZ>0.125)
        {
            tarz=ncenter[1]-10;
        }

        vtkSmartPointer<vtkTransform> translation =vtkSmartPointer<vtkTransform>::New();
        translation ->Translate(tarx,tarz,tary);
        _Scene3d_SpotTransformFilter->SetInputConnection(_Scene3d_SpotStlReader->GetOutputPort());
        _Scene3d_SpotTransformFilter->SetTransform(translation);
        _Scene3d_SpotTransformFilter->Update();
        vtkSmartPointer<vtkPolyData> spotData=vtkSmartPointer<vtkPolyData>::New();
        spotData->DeepCopy(_Scene3d_SpotTransformFilter->GetOutput());
        _Scene3d_SpotPloyData.append(spotData);

        vtkSmartPointer<vtkPolyDataMapper> mapper =
                vtkSmartPointer<vtkPolyDataMapper>::New();
        mapper->SetInputData(spotData);

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

//    _Scene3d_Renderer->GetActiveCamera()->GetPosition(CamInitPosition);
//    _Scene3d_Renderer->GetActiveCamera()->GetFocalPoint(CamInitFocalPoint);
//    _Scene3d_Renderer->GetActiveCamera()->GetViewUp(CamInitViewUp);
//    qDebug()<<"CamInitPosition"<<CamInitPosition[0]<<CamInitPosition[1]<<CamInitPosition[2];
//    qDebug()<<"CamInitViewUp"<<CamInitViewUp[0]<<CamInitViewUp[1]<<CamInitViewUp[2];
//    qDebug()<<"CamInitFocalPoint"<<CamInitFocalPoint[0]<<CamInitFocalPoint[1]<<CamInitFocalPoint[2];

     _Scene3d_Renderer->GetActiveCamera()->Roll(180);
     if(PatientHeadDirection)
        _Scene3d_Renderer->GetActiveCamera()->Azimuth(-90);
    else
        _Scene3d_Renderer->GetActiveCamera()->Azimuth(90);
    _Scene3d_Renderer->GetActiveCamera()->Zoom(0.5);
    _qvtkWidget_Scene3d->GetRenderWindow()->Render();

    if(bShowUs){
        m_bShowUsImg=true;
        _Scene3d_Renderer->AddActor(UsImgPlaneActor);
        m_bAddUsActor=true;
        if(m_dUSImgAngle!=-100)
            updateUsImg();
    }
    else{
        m_bShowUsImg=true;
    }
    bInitIrraPoint=true;
}

void QVTKWrap::InitqvtkWIdgetContextMenu()
{
    QTextCodec *codec;
    codec = QTextCodec::codecForName("GBK");
    QString info1Str =  codec->toUnicode("隐藏辐照点");
    QString info2Str =  codec->toUnicode("辐照层选择");
    QString info3Str =  codec->toUnicode("显示所有层");
    QString info4Str =  codec->toUnicode("显示上层");
    QString info5Str =  codec->toUnicode("显示中层");
    QString info6Str =  codec->toUnicode("显示下层");
    QString info7Str =  codec->toUnicode("锁定人体模型");
    QString info8Str =  codec->toUnicode("隐藏超声平面");



    _qvtkWIdgetContextMenu=new QMenu();

    connect(_qvtkWIdgetContextMenu,SIGNAL(aboutToHide()),this,SLOT(Scene3d_ContextMenu_Hide()));

    if (_ShowIrraPointElement==NULL)
    {
        _ShowIrraPointElement = _qvtkWIdgetContextMenu->addAction(info1Str);
        connect(_ShowIrraPointElement,SIGNAL(triggered(bool)),this,SLOT(Scene3d_ContextAction_ShowIrraPoint()));
    }
    QMenu* levelMenu = _qvtkWIdgetContextMenu->addMenu(info2Str);

    if (_ShowIrraPointAllLevel==NULL)
    {
        _ShowIrraPointAllLevel = levelMenu->addAction(info3Str);
        connect(_ShowIrraPointAllLevel,SIGNAL(triggered(bool)),this,SLOT(Scene3d_ContextAction_ShowIrraPointAllLevel()));
    }

    if (_ShowIrraPointTopLevel==NULL)
    {
        _ShowIrraPointTopLevel = levelMenu->addAction(info4Str);
        connect(_ShowIrraPointTopLevel,SIGNAL(triggered(bool)),this,SLOT(Scene3d_ContextAction_ShowIrraPointTopLevel()));
    }
    if (_ShowIrraPointMidlleLevel==NULL)
    {
        _ShowIrraPointMidlleLevel = levelMenu->addAction(info5Str);
        connect(_ShowIrraPointMidlleLevel,SIGNAL(triggered(bool)),this,SLOT(Scene3d_ContextAction_ShowIrraPointMidlleLevel()));
    }

    if (_ShowIrraPointDownLevel==NULL)
    {
        _ShowIrraPointDownLevel = levelMenu->addAction(info6Str);
        connect(_ShowIrraPointDownLevel,SIGNAL(triggered(bool)),this,SLOT(Scene3d_ContextAction_ShowIrraPointDownLevel()));
    }
    _qvtkWIdgetContextMenu->addSeparator();
    if (_ShowTuborElement==NULL)
    {
//        _ShowTuborElement = _qvtkWIdgetContextMenu->addAction("隐藏肌瘤模型");
//        connect(_ShowTuborElement,SIGNAL(triggered(bool)),this,SLOT(Scene3d_ContextAction_ShowTumor()));
    }
//    QMenu* OpacityMenu = _qvtkWIdgetContextMenu->addMenu(tr("肌瘤透明度"));
//    if (_ShowTumorNoOpacity==NULL)
//    {
//        _ShowTumorNoOpacity = OpacityMenu->addAction("不透明");
//        connect(_ShowTumorNoOpacity,SIGNAL(triggered(bool)),this,SLOT(Scene3d_ContextAction_ShowTumorNoOpacity()));
//    }
//    if (_ShowTumorLowOpacity==NULL)
//    {
//        _ShowTumorLowOpacity = OpacityMenu->addAction("低透明度");
//        connect(_ShowTumorLowOpacity,SIGNAL(triggered(bool)),this,SLOT(Scene3d_ContextAction_ShowTumorLowOpacity()));
//    }

//    if (_ShowTumorMidlleOpacity==NULL)
//    {
//        _ShowTumorMidlleOpacity = OpacityMenu->addAction("中透明度");
//        connect(_ShowTumorMidlleOpacity,SIGNAL(triggered(bool)),this,SLOT(Scene3d_ContextAction_ShowTumorMidlleOpacity()));
//    }
//    if (_ShowTumorHighOpacity==NULL)
//    {
//        _ShowTumorHighOpacity = OpacityMenu->addAction("高透明度");
//        connect(_ShowTumorHighOpacity,SIGNAL(triggered(bool)),this,SLOT(Scene3d_ContextAction_ShowTumorHighOpacity()));
//    }
    _qvtkWIdgetContextMenu->addSeparator();
    if (_LockHumanModel==NULL)
    {
        _LockHumanModel = _qvtkWIdgetContextMenu->addAction(info7Str);
        connect(_LockHumanModel,SIGNAL(triggered(bool)),this,SLOT(Scene3d_ContextAction_LockHumanModel()));
    }
    if(_ShowUsImgPlane==NULL)
    {
        _ShowUsImgPlane = _qvtkWIdgetContextMenu->addAction(info8Str);
        connect(_ShowUsImgPlane,SIGNAL(triggered(bool)),this,SLOT(Scene3d_ContextAction_ShowUsImgPlane()));
    }
}
void QVTKWrap::Scene3d_ContextMenu_Hide()
{
    _qvtkWidget_Scene3d->GetRenderWindow()->GetInteractor()->RightButtonReleaseEvent();
}

void QVTKWrap::updateUsImg()
{
    //更改为固定图像不变
//    UsImgQImageToImageSource->SetQImage(&m_curUsImg);
//    UsImgQImageToImageSource->Update();

//    if(UsImgTexture->GetNumberOfInputConnections(0)<=0)
//    {
//        UsImgTexture->SetInputConnection(0,UsImgQImageToImageSource->GetOutputPort());
//        UsImgTexture->InterpolateOn();
//    }

    //计算两个点坐标（实际超声平面位置）
//    double isPos;
//    isPos=m_dUSImgAngle>=0?-1.0:1.0;
//    double pAngle=-isPos*m_dUSImgAngle;
//    double pRadian=pAngle/180.0*vtkMath::Pi();
//    double p1[3],p2[3];
//    p1[0]=cos(pRadian)*USImg_X_Lenth;
//    p1[1]=0;
//    p1[2]=isPos*sin(pRadian)*USImg_X_Lenth;
//    p2[0]=0;
//    p2[1]=-USImg_Y_Lenth;
//    p2[2]=0;
//    //计算焦点和中心点偏移
//    double CenterPoint[3],FocalPoint[3],PlaneCenter[3];
//    CenterPoint[0]=p1[0]/2.0;
//    CenterPoint[1]=-USImg_Y_Lenth/2.0;
//    CenterPoint[2]=p1[2]/2.0;
//    FocalPoint[0]=cos(pRadian)*USImg_FocalX_Lenth_VTK;
//    FocalPoint[1]=-USImg_FocalY_Lenth_VTK;
//    FocalPoint[2]=isPos*sin(pRadian)*USImg_FocalX_Lenth_VTK;
//    PlaneCenter[0]=CenterPoint[0]-FocalPoint[0];
//    PlaneCenter[1]=CenterPoint[1]-FocalPoint[1];
//    PlaneCenter[2]=CenterPoint[2]-FocalPoint[2];

    //计算两点坐标（示意缩略图）
    double isPos;
    isPos=m_dUSImgAngle>=0?-1.0:1.0;
    double pAngle=-isPos*m_dUSImgAngle;
    double pRadian=pAngle/180.0*vtkMath::Pi();
    double p1[3],p2[3];
    p1[0]=cos(pRadian)*40;
    p1[1]=0;
    p1[2]=isPos*sin(pRadian)*40;
    p2[0]=0;
    p2[1]=-40;
    p2[2]=0;

    UsImgPlaneSource->SetOrigin(0,0,0);
    UsImgPlaneSource->SetPoint1(p1);
    UsImgPlaneSource->SetPoint2(p2);
    //UsImgPlaneSource->SetCenter(PlaneCenter);
    UsImgPlaneSource->SetCenter(0,0,0);
    UsImgPlaneSource->Update();

    if(!m_bAddUsActor){
        _Scene3d_Renderer->AddActor(UsImgPlaneActor);
        m_bAddUsActor=true;
    }

    _qvtkWidget_Scene3d->GetRenderWindow()->Render();
}

void QVTKWrap::Scene3d_ContextAction_ShowTumor()
{
    int iVisible=_Scene3d_TumorActor->GetVisibility();
    if(iVisible)
    {
        QTextCodec *codec;
        codec = QTextCodec::codecForName("GBK");
        QString info1Str =  codec->toUnicode("显示肌瘤模型");
        _Scene3d_TumorActor->SetVisibility(0);
        _ShowTuborElement->setText(info1Str);
    }
    else
    {
        QTextCodec *codec;
        codec = QTextCodec::codecForName("GBK");
        QString info1Str =  codec->toUnicode("隐藏肌瘤模型");
        _Scene3d_TumorActor->SetVisibility(1);
        _ShowTuborElement->setText(info1Str);
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
        QTextCodec *codec;
        codec = QTextCodec::codecForName("GBK");
        QString info1Str =  codec->toUnicode("显示辐照点");
        _ShowIrraPointElement->setText(info1Str);
    }
    else
    {
        while (i != _Scene3d_IrradiationPointActorMap.constEnd()) {
            i.value()->SetVisibility(1);
            ++i;
        }
        QTextCodec *codec;
        codec = QTextCodec::codecForName("GBK");
        QString info1Str =  codec->toUnicode("隐藏辐照点");
        _ShowIrraPointElement->setText(info1Str);
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
        QTextCodec *codec;
        codec = QTextCodec::codecForName("GBK");
        QString info1Str =  codec->toUnicode("解锁人体模型");
        _Scene3d_OrientationWidget->InteractiveOff();
        _LockHumanModel->setText(info1Str);
    }
    else
    {
        QTextCodec *codec;
        codec = QTextCodec::codecForName("GBK");
        QString info1Str =  codec->toUnicode("锁定人体模型");
        _Scene3d_OrientationWidget->InteractiveOn();
        _LockHumanModel->setText(info1Str);
    }
    _qvtkWidget_Scene3d->GetRenderWindow()->Render();
}

void QVTKWrap::Scene3d_ContextAction_ShowUsImgPlane()
{
    if(UsImgPlaneActor->GetVisibility())
    {
        QTextCodec *codec;
        codec = QTextCodec::codecForName("GBK");
        QString info1Str =  codec->toUnicode("显示超声指示图");
        UsImgPlaneActor->SetVisibility(0);
        _ShowUsImgPlane->setText(info1Str);
    }
    else
    {
        QTextCodec *codec;
        codec = QTextCodec::codecForName("GBK");
        QString info1Str =  codec->toUnicode("隐藏超声指示图");
        UsImgPlaneActor->SetVisibility(1);
        _ShowUsImgPlane->setText(info1Str);
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

void QVTKWrap::Wrap_setSpotColor(QString id,QColor color)
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

void QVTKWrap::Wrap_ClearSpot()
{
    if(_Scene3d_IrradiationPointActorMap.count()==0)
        return;
    QMap<QString,vtkSmartPointer<vtkActor> >::const_iterator i = _Scene3d_IrradiationPointActorMap.constBegin();
    while (i != _Scene3d_IrradiationPointActorMap.constEnd()) {
        _Scene3d_Renderer->RemoveActor(i.value());
        ++i;
    }
    _Scene3d_SpotPloyData.clear();
    _Scene3d_IrradiationPointMapper.clear();
    _Scene3d_IrradiationPointActorMap.clear();
    _Scene3d_Renderer->RemoveActor(UsImgPlaneActor);
    m_bAddUsActor=false;
    _qvtkWidget_Scene3d->GetRenderWindow()->Render();
    bInitIrraPoint=false;
    m_bShowUsImg=false;
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

void QVTKWrap::Wrap_ChangeUsImg(double angle)
{
    if(!bInitIrraPoint)
        return;
    m_dUSImgAngle=angle;
    if(!m_bShowUsImg)
        return;
    updateUsImg();
}
