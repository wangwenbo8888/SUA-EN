#include "myvtkcontourwidget.h"
#include <QDebug>

MyVtkContourWidget* widget=NULL;
MyVtkContourWidget::MyVtkContourWidget()
{
    curSlice=-1;
    widget=this;
    rep=vtkSmartPointer<vtkOrientedGlyphContourRepresentation>::New();
    this->SetRepresentation(rep);
    imageActorPointPlacer = vtkSmartPointer<vtkImageActorPointPlacer>::New();
    rep->SetPointPlacer(imageActorPointPlacer);
    rep->GetProperty()->SetColor(0,1,1);
    rep->SetLineColor(1,0,0);

    this->CallbackMapper->SetCallbackMethod(vtkCommand::LeftButtonReleaseEvent,
                                            vtkWidgetEvent::EndSelect,
                                            this,
                                            MyVtkContourWidget::myEndSelectAction);
    //qDebug()<<"Number:"<<rep->GetNumberOfConsumers()<<rep->GetNumberOfIntermediatePoints(0)<<rep->GetNumberOfNodes()<<rep->GetNumberOfPaths();

}

MyVtkContourWidget::~MyVtkContourWidget()
{
}

void MyVtkContourWidget::SetCurSlice(int var)
{
    curSlice=var;
    if(contourlist.contains(var))
    {
        this->Initialize(contourlist[var]);
    }
    else
        this->Initialize();

    this->Render();
}

void MyVtkContourWidget::myEndSelectAction(vtkAbstractWidget* w)
{
    vtkContourWidget::EndSelectAction(w);
    vtkSmartPointer<vtkOrientedGlyphContourRepresentation> rep1=widget->GetMyRepresentation();
    if(widget->GetMyRepresentation()->GetClosedLoop())
    {
        //qDebug()<<"Number:"<<rep1->GetNumberOfConsumers()<<rep1->GetNumberOfIntermediatePoints(0)<<rep1->GetNumberOfNodes()<<rep1->GetNumberOfPaths();
        int slice=widget->GetImageActorPointPlacer()->GetImageActor()->GetSliceNumber();
        vtkSmartPointer<vtkPolyData> contourdata=vtkSmartPointer<vtkPolyData>::New();
        rep1->GetNodePolyData(contourdata);
        (*widget->GetConturList())[slice]=contourdata;

        vtkSmartPointer<vtkPolyData> contourdata_rep=vtkSmartPointer<vtkPolyData>::New();
        // contourdata_rep->DeepCopy(rep1->GetContourRepresentationAsPolyData());
        rep1->GetNodePolyData(contourdata_rep);

        (*widget->GetConturList_Rep())[slice]=contourdata_rep;
    }
}

void MyVtkContourWidget::ClearContour(int slice)
{
    if(contourlist.empty())
        return;

    if(!this->GetEnabled())
        return;

    if(!contourlist.contains(slice))
        return;

    contourlist.remove(slice);
    contourlist_rep.remove(slice);

    if(slice==curSlice)
    {
        this->Initialize();
        this->Render();
    }
}

void MyVtkContourWidget::ClearContour()
{
    if(contourlist.empty())
        return;

    if(!this->GetEnabled())
        return;

    contourlist.clear();
    contourlist_rep.clear();

    this->Initialize();
    this->Render();
}

