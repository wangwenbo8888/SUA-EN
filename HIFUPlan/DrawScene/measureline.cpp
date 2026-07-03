#include "measureline.h"
#include <QDebug>
#include <QSettings>
#include <QGraphicsScene>
#include <QPainterPath>
#include <QGraphicsPathItem>
#include "qgraphicsitem.h"
#include "qgraphicsscene.h"

//Q_LOGGING_CATEGORY(MEASURELINE,"MEASURELINE")

MeasureLine::MeasureLine()
{
    m_line = new QGraphicsLineItem;
    m_text = new QGraphicsTextItem;
    m_startCross = new QGraphicsPathItem;
    m_endCross = new QGraphicsPathItem;
    m_startpoint.setX(-1.0);
    m_startpoint.setY(-1.0);
    m_endpoint.setX(-1.0);
    m_endpoint.setY(-1.0);
}

MeasureLine::~MeasureLine()
{
    if(m_line!=NULL)
    {
        delete m_line;
        m_line = NULL;
    }
    if(m_text!=NULL)
    {
        delete m_text;
        m_text = NULL;
    }
    if(m_startCross!=NULL)
    {
        delete m_startCross;
        m_startCross = NULL;
    }
    if(m_endCross!=NULL)
    {
       delete m_endCross;
       m_endCross = NULL;
    }
}


 void MeasureLine::setLine(QGraphicsLineItem* lineitem)
 {
     m_line = lineitem;
 }
 void MeasureLine::setText(QGraphicsTextItem* textitem)
 {      
     m_text = textitem;
 }

 void MeasureLine::setStartCross(QGraphicsPathItem* startcross)
 {
    m_startCross = startcross;
 }

 void MeasureLine::setEndCross(QGraphicsPathItem* endcross)
 {
    m_endCross = endcross;
 }

 void MeasureLine::setStartPoint(qreal x, qreal y)
 {
      m_startpoint.setX(x);
      m_startpoint.setY(y);
 }

 void MeasureLine::setEndPoint(qreal x, qreal y)
 {
     m_endpoint.setX(x);
     m_endpoint.setY(y);
 }

 void MeasureLine::setMeasureScal(qreal s)
 {
    m_line->setScale(s);
    m_text->setScale(s);
    m_startCross->setScale(s);
    m_endCross->setScale(s);

 }

 QGraphicsLineItem* MeasureLine::getLine()
 {
     return m_line;
 }

 QGraphicsTextItem* MeasureLine::getText()
 {
     return m_text;
 }

 QGraphicsPathItem* MeasureLine::getStartCross()
 {
     return m_startCross;
 }

 QGraphicsPathItem* MeasureLine::getEndCross()
 {
     return m_endCross;
 }

 QPointF MeasureLine::getStartPoint()
 {
    return m_startpoint;
 }

 QPointF MeasureLine::getEndPoint()
 {
     return m_endpoint;
 }

 QPainterPath MeasureLine::setStartCrossPath(int delta)
 {
     QPainterPath path;
     QPointF up(m_startpoint.x(),m_startpoint.y()-delta);
     QPointF down(m_startpoint.x(),m_startpoint.y()+delta);
     QPointF left(m_startpoint.x()-delta,m_startpoint.y());
     QPointF right(m_startpoint.x()+delta,m_startpoint.y());

     path.moveTo(up);
     path.lineTo(down);

     path.moveTo(left);
     path.lineTo(right);

     return path;
 }

 QPainterPath MeasureLine::setEndCrossPath(int delta)
 {
     QPainterPath path;
     QPointF up(m_endpoint.x(),m_endpoint.y()-delta);
     QPointF down(m_endpoint.x(),m_endpoint.y()+delta);
     QPointF left(m_endpoint.x()-delta,m_endpoint.y());
     QPointF right(m_endpoint.x()+delta,m_endpoint.y());

     path.moveTo(up);
     path.lineTo(down);

     path.moveTo(left);
     path.lineTo(right);

     return path;
 }
