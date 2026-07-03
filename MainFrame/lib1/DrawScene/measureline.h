#ifndef MEASURELINE
#define MEASURELINE

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPointF>
#include <QGraphicsPathItem>
#include <QPainterPath>

class MeasureLine
{
   public:

        MeasureLine();
        ~MeasureLine();

        void setLine(QGraphicsLineItem* lineitem);
        void setText(QGraphicsTextItem* textitem);
        void setStartCross(QGraphicsPathItem* startcross);
        void setEndCross(QGraphicsPathItem* endcross);
        void setStartPoint(qreal x, qreal y);
        void setEndPoint(qreal x, qreal y);

        void setMeasureScal(qreal scal);

        QGraphicsLineItem* getLine();
        QGraphicsTextItem* getText();
        QGraphicsPathItem* getStartCross();
        QGraphicsPathItem* getEndCross();
        QPointF getStartPoint();
        QPointF getEndPoint();

        QPainterPath setStartCrossPath(int delta);
        QPainterPath setEndCrossPath(int delta);

    private:
        QGraphicsLineItem* m_line;  //lineItem的信息
        QGraphicsTextItem* m_text;  //线段末尾的长度标签
        QGraphicsPathItem* m_startCross;
        QGraphicsPathItem* m_endCross;
        QPointF m_startpoint;      // 线段起点坐标
        QPointF m_endpoint;        //线段终点坐标

};



#endif // MEASURELINE

