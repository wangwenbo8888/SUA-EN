
#include "histogram.h"
#include "qpainter.h"
#include "qpen.h"

#include "commondefine.h"

#include <vector>

using namespace std;

PaintedWidget::PaintedWidget(RealtimeScene* real,QWidget *parent)
: QWidget(parent)
, m_real(real)
{
    m_target = NULL;
    resize(parent->width(), parent->height());
    realTime = m_real->getPixmap();
}

void PaintedWidget::setDrawScene(DrawScene* target)
{
    m_target = target;
    targetPix = m_target->getPixmap();
}

vector<uchar> Histogram(QImage* img)
{
    uchar* graydata = img->bits();
    vector<uchar> hist(256,0);
    for (int i=0;i!=img->width();i++)
    {
       for (int j=0;j!=img->height();j++)
       {
           int index = int(*graydata);
           if (index>=0 && index<=255)
           {
              ++hist[index];
           }
           ++graydata;
        }
    }

    graydata = NULL;
    return hist;
}

bool PaintedWidget::isIrradiating()
{
    if (m_real==NULL || m_target==NULL)
    {
        return true;
    }

    QImage realImage = realTime.copy(340,510,5,5).toImage();
    QImage gray = toGray(realImage);
    uchar* graydata = gray.bits();
    int grayTotal = 0;
    for (int i=0;i!=gray.width();i++)
    {
       for (int j=0;j!=gray.height();j++)
       {
           int value = int(*graydata);
           if (value>=0 && value<=255)
           {
              grayTotal+= value;
           }
           ++graydata;
        }
    }


    QImage targetImage = targetPix.copy(340,510,5,5).toImage();
    QImage targetGray = toGray(targetImage);
    graydata = targetGray.bits();
    int targetTotal = 0;
    for (int i=0;i!=targetGray.width();i++)
    {
       for (int j=0;j!=targetGray.height();j++)
       {
           int value = int(*graydata);
           if (value>=0 && value<=255)
           {
              targetTotal+= value;
           }
           ++graydata;
        }
    }

    if (grayTotal-targetTotal>250)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void PaintedWidget::paintEvent(QPaintEvent *)
{
    if (m_target==NULL)
    {
        return;
    }

    // 判断是否在辐照
    if (isIrradiating())
    {
        return;
    }

    QPixmap realTime = m_real->getPixmap();
    QImage realImage = realTime.copy(242,252,64,84).toImage();
    QImage gray = toGray(realImage);
    vector<uchar> count = Histogram(&gray);

    QPixmap targetPix = m_target->getPixmap();
    QImage targetImage = targetPix.copy(242,252,64,84).toImage();
    QImage targetGray = toGray(targetImage);
    vector<uchar> targetCount = Histogram(&targetGray);

    QPainter p(this);
    p.setBrush(QBrush(QColor(121,121,121)));
    p.drawRect(0,0,this->width(),this->height());

    p.setBrush(QBrush(QColor(55,55,55)));
    p.drawRect(0,0,this->width(),this->height());
    std::vector<uchar> sortcount = count;
    std::sort(sortcount.begin(),sortcount.end());
    int maxcount = sortcount[sortcount.size()-1];

    sortcount = targetCount;
    std::sort(sortcount.begin(),sortcount.end());
    maxcount = maxcount>sortcount[sortcount.size()-1]?maxcount:sortcount[sortcount.size()-1];

    QImage hist = QImage(this->width(),this->height(),QImage::Format_RGB888);
    hist.fill(qRgb(255,255,255));
    p.translate(0,hist.height());
    //p.drawLine(0,0,100,100);

    int wid=this->width();
    int hei=this->height();

    int delta = 22;
    p.drawLine(10,-delta,wid-10,-delta);// 横轴
    p.drawLine(10,-delta,10,-hei+10);//纵轴

    float xstep = float(wid-delta) / 256;
    float ystep = float(hei-delta) / maxcount;

    //  字体放大
    QFont font("微软雅黑",20,QFont::Bold,false);
    p.setFont(font);

    // 淡蓝
    QColor blue(174,238,150);
    p.setBrush(blue);
    p.setPen(blue);
    for (int i=1;i<256;i++)
    {
        p.drawLine(10+(i-1)*xstep,-delta-ystep*targetCount[i-1],
                   10+i*xstep,-delta-ystep*targetCount[i]);
    }
    p.drawText(QPointF(this->width()*0.333,-1*this->height()+40),QString("Plan"));

    // 淡黄
    QColor yellow(139,90,43);
    p.setBrush(yellow);
    p.setPen(yellow);
    for (int i=1;i<256;i++)
    {
        p.drawLine(10+(i-1)*xstep,-delta-ystep*count[i-1],
                   10+i*xstep,-delta-ystep*count[i]);
    }
    p.drawText(QPointF(this->width()*0.667,-1*this->height()+40),QString("Live"));

    for (int i=0;i<256;i++)
    {

        if(i%32 == 0)
        {
            p.drawText(QPointF(10+(i-0.5)*xstep,0),QString::number(i));
        }
    }
}

