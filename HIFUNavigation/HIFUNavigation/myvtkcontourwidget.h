#ifndef MYVTKCONTOURWIDGET_H
#define MYVTKCONTOURWIDGET_H
//VTK
#include <vtkContourWidget.h>
#include <vtkWidgetCallbackMapper.h>
#include <vtkCommand.h>
#include <vtkWidgetEvent.h>
#include <vtkOrientedGlyphContourRepresentation.h>
#include <vtkSmartPointer.h>
#include <vtkImageActorPointPlacer.h>
#include <vtkProperty.h>
#include <vtkImageActor.h>
#include <vtkPolyData.h>
#include <vtkSphereSource.h>
#include <vtkCellArray.h>

//Qt
#include <QPair>
#include <QMap>

class MyVtkContourWidget : public vtkContourWidget
{
public:
    MyVtkContourWidget();
    ~MyVtkContourWidget();

    vtkSmartPointer<vtkImageActorPointPlacer> GetImageActorPointPlacer(){return imageActorPointPlacer;}
    vtkSmartPointer<vtkOrientedGlyphContourRepresentation> GetMyRepresentation(){return rep;}

    QMap<int,vtkSmartPointer<vtkPolyData> >* GetConturList(){return &contourlist;}
    QMap<int,vtkSmartPointer<vtkPolyData> >* GetConturList_Rep(){return &contourlist_rep;}

    void SetCurSlice(int var);

    void ClearContour(int slice);
    void ClearContour();

protected:
    static void myEndSelectAction(vtkAbstractWidget* w);

private:
    vtkSmartPointer<vtkOrientedGlyphContourRepresentation> rep;
    vtkSmartPointer<vtkImageActorPointPlacer> imageActorPointPlacer;

    QMap<int,vtkSmartPointer<vtkPolyData> > contourlist;
    QMap<int,vtkSmartPointer<vtkPolyData> > contourlist_rep;

    // 真正的肌瘤边界
    QMap<int,vtkSmartPointer<vtkPolyData> > contourMap;

    int curSlice;
};

#endif // MYVTKCONTOURWIDGET_H
