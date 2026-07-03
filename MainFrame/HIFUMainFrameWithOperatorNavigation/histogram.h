#ifndef HISTOGRAM_H_
#define HISTOGRAM_H_

#include "qwidget.h"
#include "realtimescene.h"
#include "drawscene.h"

class PaintedWidget : public QWidget
{
    Q_OBJECT
public:
    PaintedWidget(RealtimeScene* real,QWidget *parent = 0);
    void setDrawScene(DrawScene* target);

protected:
    void paintEvent(QPaintEvent *);

    // 判断是否在辐照
    bool isIrradiating();

    RealtimeScene* m_real;
    DrawScene* m_target;
    QPixmap realTime;
    QPixmap targetPix;

};


#endif // HISTOGRAM

