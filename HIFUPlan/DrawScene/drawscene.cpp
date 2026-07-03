#include <math.h>
#include <QSettings>
#include <QDebug>
#include "drawscene.h"
#include "measureline.h"
#include "qmath.h"

#include "ellipsefit.h"
#include "spline.h"
#include "vect2f.h"

#include "ellipsefit.h"

 double RESOLUTION = 2.07692;                           // unit: pixel/mm
 double ZERO_DEVIATION = 2.2*RESOLUTION;
// 换算成140mm应该是多少个像素
 double FOCAL_PLANE_140MM = 140 * RESOLUTION+ZERO_DEVIATION;   // unit: pixel/mm

//#endif

 double X_OFFSET = 274;                                    // unit: pixel
 double Y_OFFSET = 294;                                    // unit: pixel



 double SPOT_WIDTH_DENSITY_HIGH  = 1.0*RESOLUTION;         // 辐照点高密度，辐照点直径1.0mm
 double SPOT_WIDTH_DENSITY_MIDDLE = 1.4*RESOLUTION;        // 辐照点高密度，辐照点直径1.4mm
 double SPOT_WIDTH_DENSITY_LOW  = 1.8*RESOLUTION;          // 辐照点高密度，辐照点直径1.8mm

 double SPOT_WIDTH  = 1.8 * RESOLUTION;                    // unit: pixel
 double SPOT_LENGTH = 10 * RESOLUTION;                     // unit: pixel

 double CENTER_LENGTH = 20*RESOLUTION;
 double BEAM_LENGTH = 10*RESOLUTION;

 double SPOT_END_LENGTH = 5*RESOLUTION;
 double BEAM_END_LENGTH = 40*RESOLUTION;

 double CELL4MM_LATERAL_DISTANCE = 2 * RESOLUTION;         // unit: pixel
 double CELL8MM_LATERAL_DISTANCE = 4 * RESOLUTION;         // unit: pixel
 double CELL12MM_LATERAL_DISTANCE = 6 * RESOLUTION;        // unit: pixel

 double SPOT_DISTANCE_HIGH = 1.2*RESOLUTION;
 double SPOT_DISTANCE_MIDDLE = 1.6*RESOLUTION;
 double SPOT_DISTANCE_LOW = 2.0*RESOLUTION;


Q_LOGGING_CATEGORY(DRAWSCENE,"DRAWSCENE")

DrawScene::DrawScene(float angle, SCENE_OPER oper, QObject *parent)
: QGraphicsScene(parent),
  m_operator(oper),
  m_angle(angle)
{
    m_drawable = false;
    m_targetDrawable = false;
	m_measureDrawable = false;
    m_bEleRec = false;
    m_isCtrlDown = true;
    _bChangeable = false;
    _bMeasureable = false;
    _bPressed = false;
    _bExist = false;
    _bManualDrag = false;
    m_targetContour=NULL;
    m_autoDraw = NULL;
    m_sysEdges = NULL;
    m_tempContour=NULL;
    m_displayContour = NULL;
    m_transducerContour=NULL;
    m_measureContour = NULL;
    m_outline = NULL;
    m_beam=NULL;
    m_marker=NULL;
    m_path = NULL;
    m_selectIndex = -1;
    m_delete = -1;
    m_change_path = NULL;
    m_newMeasureContour = NULL;
    m_measure = NULL;
    m_measure_changed = NULL;

    m_eDensity = DENSITY_UNKNOWN;
    m_dSpotWide = m_dSpotWide;
    m_dSpotDistance = SPOT_DISTANCE_LOW;
    // 以前删除的靶区区域，作为后来勾画的参考
    m_bakContour = NULL;
    m_backgroundPixmap=NULL;

    m_targetPen.setColor(Qt::yellow);
    m_targetPen.setWidth(2);
    m_targetPen.setStyle(Qt::SolidLine);

    m_autoPen.setColor(AUTO_COLOR);
    m_autoPen.setWidth(2);
    m_autoPen.setStyle(Qt::SolidLine);

    m_spotPen.setColor(SPOT_COLOR); // 辐照单元设置成褐色
    m_spotPen.setWidthF(1.5);

    m_edgePen.setColor(EDGE_COLOR);
    m_edgePen.setWidthF(1.5);

    m_sysPen.setColor(Qt::yellow);
    m_sysPen.setWidth(2);
    m_sysPen.setStyle(Qt::SolidLine);

    m_selected.setStyle(Qt::SolidLine);
    m_selected.setColor(Qt::red);
    m_selected.setWidthF(1.5);

    m_measurePen.setStyle(Qt::SolidLine);
    m_measurePen.setColor(Qt::green);
    m_measurePen.setWidthF(0);

    m_changedPen.setStyle(Qt::DashDotLine);
    m_changedPen.setColor(Qt::yellow);
    m_changedPen.setWidthF(1.5);

    m_extremPen.setStyle(Qt::SolidLine);
    m_extremPen.setColor(Qt::red);
    m_extremPen.setWidthF(0);

    m_selectOutLinePen.setStyle(Qt::CustomDashLine);
    m_selectOutLinePen.setColor(Qt::red);
    m_selectOutLinePen.setWidth(5);

    m_transparent.setWidthF(0.01);
    m_transparent.setColor(Qt::transparent);

    m_bakPen.setStyle(Qt::SolidLine);
    m_bakPen.setColor(Qt::blue);
    m_bakPen.setWidthF(2);

    m_transducerPen.setColor(Qt::darkGray);
    m_transducerPen.setWidth(2);
    m_beamPen.setColor("#63B8FF");
    m_beamPen.setWidthF(2);

    m_outlinePen.setColor(Qt::yellow);
    m_outlinePen.setWidthF(5);

    QSettings *settings = new QSettings(SETTINGS_PATH, QSettings::IniFormat);
    m_bEleRec = (bool)settings->value("Shape/rectange").toString().toUInt();
    RESOLUTION = settings->value("UltrasoundImage/RESOLUTION").toString().toDouble();
    X_OFFSET = settings->value("UltrasoundImage/X_OFFSET").toString().toDouble();
    Y_OFFSET = settings->value("UltrasoundImage/Y_OFFSET").toString().toDouble();
    float delta = settings->value("DrawScene/Focus_Delta").toFloat();
    qCDebug(DRAWSCENE()) << DRAWSCENE().categoryName()
                         << "focus delta is "<<delta
                         << TRACE_CMH();

    m_dFocusDeep = delta*RESOLUTION+140 * RESOLUTION;


    _sceneType = SCENE_TARGET;
    delete settings;
}

DrawScene::DrawScene(DrawScene*& pObj, const QPixmap& pix)
{
    m_tempContour = NULL;
    m_displayContour = NULL;
    m_outline = NULL;
    m_transducerContour = NULL;
    m_beam = NULL;
    m_marker = NULL;
    m_measureContour = NULL;
    m_bakContour = NULL;
    m_path = NULL;
    m_change_path = NULL;
    m_selectIndex = -1;
    m_delete = -1;
    m_newMeasureContour = NULL;
    m_measure = NULL;
    m_measure_changed = NULL;

    m_angle = pObj->m_angle;
    m_drawable = pObj->m_drawable;
    m_targetDrawable = pObj->m_targetDrawable;
    m_measureDrawable = pObj->m_measureDrawable;
    _bChangeable = pObj->_bChangeable;
    _bMeasureable = pObj->_bMeasureable;
    _bPressed = pObj->_bPressed;
    _bExist = pObj->_bExist;
    _bManualDrag = pObj->_bManualDrag;
    m_targetPen = pObj->m_targetPen;
    m_spotPen = pObj->m_spotPen;
    m_edgePen = pObj->m_edgePen;
    m_sysPen = pObj->m_sysPen;
    m_autoPen = pObj->m_autoPen;
    m_outlinePen = pObj->m_outlinePen;
    m_beamPen = pObj->m_beamPen;
    m_selected = pObj->m_selected;
    m_transducerPen=pObj->m_transducerPen;
    m_transparent = pObj->m_transparent;
    m_dFocusDeep = pObj->m_dFocusDeep;

    m_eDensity = pObj->m_eDensity;
    m_dSpotWide = pObj->m_dSpotWide;
    m_dSpotDistance = pObj->m_dSpotDistance;

    m_points = pObj->m_points;


    m_backgroundPixmap = addPixmap(pix);

    if (pObj->m_targetContour!=NULL)
    {
        m_targetContour = addPath(pObj->m_targetContour->path(),m_targetPen);
    }
    else
    {
        m_targetContour = NULL;
    }

    if (pObj->m_autoDraw!=NULL)
    {
        m_autoDraw = addPath(pObj->m_autoDraw->path(),m_autoPen);
    }
    else
    {
        m_autoDraw = NULL;
    }

    if (pObj->m_sysEdges!=NULL)
    {
        m_sysEdges = addPath(pObj->m_sysEdges->path(),m_sysPen);
    }
    else
    {
        m_sysEdges = NULL;
    }

    _sceneType = SCENE_SEQUENCE;
    m_bEleRec = pObj->m_bEleRec;
    if (!pObj->m_spots.empty())
    {
        target_spot spot;
        m_spots.clear();
        for (size_t i = 0;i<pObj->m_spots.size();++i)
        {
//            if (!pObj->m_spots[i].spot->isVisible())
//            {
//                continue;
//            }

            if (m_bEleRec)
            {
                spot.spot = addRect(pObj->m_spots[i].spot->boundingRect(),m_spotPen);
            }
            else
            {
                spot.spot = addEllipse(pObj->m_spots[i].spot->boundingRect(),m_spotPen);
            }
            // item->setScale(m_backgroundPixmap->scale());
            spot.idBorn = pObj->m_spots[i].idBorn;
            spot.pos = pObj->m_spots[i].pos;
            spot.state = pObj->m_spots[i].state;
            m_spots<< spot;
        }
    }
}

DrawScene::~DrawScene(){

    if (m_targetContour != NULL)
    {
        delete m_targetContour;
        removeItem(m_targetContour);
        m_targetContour = NULL;
    }

    if (m_tempContour!=NULL)
    {
        delete m_tempContour;
        removeItem(m_tempContour);
        m_tempContour = NULL;
    }

    if(m_displayContour!=NULL)
    {
        delete m_displayContour;
        removeItem(m_displayContour);
        m_displayContour = NULL;
    }

    if (m_transducerContour!=NULL)
    {
        delete m_transducerContour;
        removeItem(m_transducerContour);
        m_transducerContour = NULL;
    }

    if (m_outline!=NULL)
    {
//        delete m_outline;
        removeItem(m_outline);
//        m_outline = NULL;
    }

    if (m_beam!=NULL)
    {
        removeItem(m_beam);
    }

    if (m_marker!=NULL)
    {
        removeItem(m_marker);
    }

    if (m_backgroundPixmap!=NULL)
    {
        delete m_backgroundPixmap;
        removeItem(m_backgroundPixmap);
        m_backgroundPixmap = NULL;
    }

    if (m_bakContour!=NULL)
    {
        delete m_bakContour;
        removeItem(m_bakContour);
        m_bakContour = NULL;
    }

    if (m_measureContour!=NULL)
    {
        delete m_measureContour;
        removeItem(m_measureContour);
        m_measureContour = NULL;
    }

    if(m_path!=NULL)
    {
        delete m_path;
        m_path = NULL;
    }

    if (m_sysEdges!=NULL)
    {
        removeItem(m_sysEdges);
        m_sysEdges = NULL;
    }

    if(m_change_path!=NULL)
    {
        delete m_change_path;
        m_change_path = NULL;
    }

    if (m_newMeasureContour!=NULL)
    {
        delete m_newMeasureContour;
        removeItem(m_newMeasureContour);
        m_newMeasureContour = NULL;
    }
    if(m_measure!=NULL)
    {
        removeMeasureLine(m_measure);
        delete m_measure;
        m_measure = NULL;
    }

    if(m_measure_changed!=NULL)
    {
        removeMeasureLine(m_measure_changed);
        delete m_measure_changed;
        m_measure_changed = NULL;
    }

    m_spots.clear();
    m_circles.clear();
    m_measureSeries.clear();
    dist_and_index.clear();
}

void DrawScene::setDensity(DENSITY_TYPE type)
{
     m_eDensity = type;
     switch(m_eDensity)
     {
     case DENSITY_HIGH:
         m_dSpotWide = SPOT_WIDTH_DENSITY_HIGH;
         m_dSpotDistance = SPOT_DISTANCE_HIGH;
         break;

     case DENSITY_MIDDLE:
         m_dSpotWide = SPOT_WIDTH_DENSITY_MIDDLE;
         m_dSpotDistance = SPOT_DISTANCE_MIDDLE;
         break;

     case DENSITY_LOW:
         m_dSpotWide = SPOT_WIDTH_DENSITY_LOW;
         m_dSpotDistance = SPOT_DISTANCE_LOW;
         break;

     default:
         m_dSpotWide = SPOT_WIDTH_DENSITY_LOW;
         m_dSpotDistance = SPOT_DISTANCE_LOW;
         break;
     }
}

void DrawScene::setDrawEdges(QPainterPath path)
{
    qDebug()<<"Enter draw scene set draw edges ";
    if (m_sysEdges!=NULL)
    {
        removeItem(m_sysEdges);
        m_sysEdges=NULL;
    }

    m_sysEdges = addPath(path,m_sysPen);
    m_sysEdges->setVisible(true);
    m_sysEdges->setScale(m_backgroundPixmap->scale());
}

void DrawScene::setBackgroundPixmap(QPixmap pixmap) {
    if (m_backgroundPixmap!=NULL)
    {
        delete m_backgroundPixmap;
        removeItem(m_backgroundPixmap);
    }

    m_backgroundPixmap = addPixmap(pixmap);
}

void DrawScene::setSceneType(SCENE_TYPE type)
{
    _sceneType = type;
}

void DrawScene::setOperator(SCENE_OPER oper)
{
    m_operator = oper;
}

void DrawScene::sysDrawEdges()
{
    emit getDrawEdges();
}

QPainterPath DrawScene::targetPath()
{
    return m_targetContour->path();
}

void DrawScene::setTargetPath(QPainterPath path)
{
    m_autoDraw = addPath(path,m_autoPen);
    m_autoDraw->setScale(m_backgroundPixmap->scale());
}

void DrawScene::clearTargetPath()
{
    if (m_targetContour!=NULL)
    {
        removeItem(m_targetContour);

        m_targetContour = NULL;
    }

    if (m_autoDraw!=NULL)
    {
         removeItem(m_autoDraw);
         m_autoDraw = NULL;
    }
}

float DrawScene::getAngle()
{
    return m_angle;
}

SCENE_OPER DrawScene::getOperator()
{
    return m_operator;
}

QPixmap DrawScene::getPixmap()
{
    if (m_backgroundPixmap!=NULL)
    {
        return m_backgroundPixmap->pixmap();
    }
    else
    {
        return QPixmap();
    }
}
void DrawScene::keyPressEvent(QKeyEvent *event)
{
    if (event->key()==Qt::Key_Delete)
    {
        if (m_selectIndex!=-1)
        {
            removeMeasureLine(m_measureSeries[m_selectIndex]);
            m_measureSeries[m_selectIndex] = NULL;
            m_measureSeries.removeAt(m_selectIndex);
            m_selectIndex = -1;
            _bExist = false;            
        }       
        QGraphicsScene::keyPressEvent(event);
        return;
    }

}


void DrawScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    qreal scal;
    scal = m_backgroundPixmap->scale();
    if(drawable())
    {

        if(event->buttons()==Qt::LeftButton)
        {
            // 记录鼠标左键按下的位置
            m_leftDownPos = event->scenePos()/scal;

            _bPressed = true;
            if (m_backgroundPixmap==NULL)
            {
                return;
            }

            if(m_measureDrawable)
            {
                _bPressed = true;
                for(int i =0;i<m_measureSeries.size();i++)
                {
                    QRectF rect_start,rect_end;
                    rect_start.setRect(m_measureSeries[i]->getStartPoint().x()-10,m_measureSeries[i]->getStartPoint().y()-10,20,20);
                    rect_end.setRect(m_measureSeries[i]->getEndPoint().x()-10,m_measureSeries[i]->getEndPoint().y()-10,20,20);
                    if(rect_start.contains(event->scenePos()/scal))
                    {
                        _bChangeable = true;
                        m_delete = i;
                        if(m_newMeasureContour==NULL)
                        {
                            m_newMeasureContour = new QGraphicsPathItem;
                        }
                        else
                        {
                            removeItem(m_newMeasureContour);
                            delete m_newMeasureContour;
                            m_newMeasureContour = new QGraphicsPathItem;
                        }

                        if (m_change_path==NULL)
                        {
                            m_change_path = new QPainterPath(m_measureSeries[m_delete]->getEndPoint()*scal);
                        }
                        else
                        {
                            delete m_change_path;
                            m_change_path = new QPainterPath(m_measureSeries[m_delete]->getEndPoint()*scal);
                        }
                        m_change_path->lineTo(m_measureSeries[m_delete]->getStartPoint()*scal);
                        removeItem(m_newMeasureContour);
                        m_newMeasureContour = addPath(*m_change_path,m_changedPen);
                        removeMeasureLine(m_measureSeries[m_delete]);
                        m_measureSeries.removeAt(m_delete);
                        QGraphicsScene::event(event);
                        return;

                    }
                    if(rect_end.contains(event->scenePos()/scal))
                    {
                        _bChangeable = true;
                        m_delete = i;

                        if(m_newMeasureContour==NULL)
                        {
                            m_newMeasureContour = new QGraphicsPathItem;
                        }
                        else
                        {
                            delete m_newMeasureContour;
                            removeItem(m_newMeasureContour);
                            m_newMeasureContour = new QGraphicsPathItem;
                        }

                        if (m_change_path==NULL)
                        {
                            m_change_path = new QPainterPath(m_measureSeries[i]->getStartPoint()*scal);
                        }
                        else
                        {
                            delete m_change_path;
                            m_change_path = new QPainterPath(m_measureSeries[i]->getStartPoint()*scal);
                        }

                        m_change_path->lineTo(m_measureSeries[m_delete]->getEndPoint()*scal);
                        removeItem(m_newMeasureContour);
                        m_newMeasureContour = addPath(*m_change_path,m_changedPen);
                        removeMeasureLine(m_measureSeries[m_delete]);
                        m_measureSeries.removeAt(m_delete);

                        QGraphicsScene::mousePressEvent(event);
                        return;
                    }

                }

                if(_bPressed)
                {
                    startPos = event->scenePos();
                    _bMeasureable = false;
                    QGraphicsScene::mousePressEvent(event);
                    return;
                }

            }

            if(targetDrawable())
            {
                if (m_targetContour!=NULL)
                {
                    delete m_targetContour;
                    removeItem(m_targetContour);
                    m_targetContour = NULL;
                }
                    QPainterPath path(event->scenePos()/scal);
                    m_tempContour = new QGraphicsPathItem;
                    m_displayContour = new QGraphicsPathItem;
                    m_tempContour->setPath(path);
                    m_displayContour->setPath(QPainterPath(event->scenePos()/scal));
                    QGraphicsScene::mousePressEvent(event);
                    return;

            }
        }
    }


    if(event->buttons()==Qt::LeftButton)
    {
        if(m_operator==DRAG&&m_outline!=NULL)
        {
            if(m_outline->contains(event->scenePos()/scal))
            {
                m_outline->setPen(m_selectOutLinePen);
                _bManualDrag = true;
                m_outlinePos = event->scenePos()/scal;

            }
        }
    }
}

void DrawScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (_sceneType==SCENE_LAYER)
    {
        // 层图像不处理
        QGraphicsScene::mouseMoveEvent(event);
        return;
    }
    qreal scal;
    scal = m_backgroundPixmap->scale();
    if(drawable())
    {
        if(event->buttons()==Qt::LeftButton)
        {
            if(m_measureDrawable)
            {
                if(_bChangeable)
                {
                    QPainterPath::Element start = m_change_path->elementAt(0);

                    if (m_change_path!=NULL)
                    {
                        delete m_change_path;
                        m_change_path = new QPainterPath(QPointF(start.x,start.y));
                    }
                    removeItem(m_newMeasureContour);
                    m_change_path->lineTo(event->scenePos());
                    m_newMeasureContour = addPath(*m_change_path,m_changedPen);
                    QGraphicsScene::mouseMoveEvent(event);
                    return;
                }

                if(_bPressed&&!_bExist)
                {
                    _bMeasureable = true;
                    if(m_measureContour==NULL)
                    {
                        m_measureContour = new QGraphicsPathItem;
                    }
                    else
                    {
                        delete m_measureContour;
                        removeItem(m_measureContour);
                        m_measureContour = new QGraphicsPathItem;
                    }

                    if (m_path==NULL)
                    {
                        m_path = new QPainterPath(startPos);
                    }
                    else
                    {
                        delete m_path;
                        m_path = new QPainterPath(startPos);

                    }

                    removeItem(m_measureContour);
                    m_path->lineTo(event->scenePos());
                    m_measureContour = addPath(*m_path,m_measurePen);
                    QGraphicsScene::mouseMoveEvent(event);

                    return;
                }
                QGraphicsScene::mouseMoveEvent(event);
                return;
            }


            if(targetDrawable())
            {
                if (m_displayContour==NULL
                        || !m_displayContour->path().elementCount())
                {
                    qCDebug(DRAWSCENE()) << DRAWSCENE().categoryName()
                                         << "display contour NULL "
                                         << TRACE_CMH();
                    QGraphicsScene::mouseMoveEvent(event);
                    return;
                }

                if(m_tempContour==NULL
                        || !m_tempContour->path().elementCount())
                {
                    qCDebug(DRAWSCENE()) << DRAWSCENE().categoryName()
                                         << "temp contour NULL "
                                         << TRACE_CMH();
                    QGraphicsScene::mouseMoveEvent(event);
                    return;
                }

                if (m_tempContour->path().elementCount() > 1)
                {
                    removeItem(m_tempContour);
                }

                if (m_displayContour->path().elementCount() > 1)
                {
                    removeItem(m_displayContour);
                }

                QPainterPath path(m_displayContour->path());
                path.lineTo(event->scenePos()/scal);

                QPainterPath hidePath(m_tempContour->path());
                hidePath.lineTo(event->scenePos()/scal);

                delete m_tempContour;

                if(m_operator!=OPER_MEASURE)
                {
                    QPen pen;
                    pen.setWidthF(0.01);
                    pen.setColor(Qt::transparent);

                    m_displayContour = addPath(path,m_targetPen);
                    m_tempContour = addPath(hidePath,pen);
                    m_displayContour->setScale(scal);
                    m_tempContour->setScale(scal);
                    QGraphicsScene::mouseMoveEvent(event);
                    return;

                }


                QGraphicsScene::mouseMoveEvent(event);
                return;
            }

         }

    }

    if(event->buttons()==Qt::MidButton)
    {
        if(m_operator == DRAG&&m_outline!=NULL)
        {

            m_outlinePos = event->scenePos()/scal;
            removeItem(m_outline);
            m_outline = addPath(moveOutline(m_outlinePos),m_outlinePen);
            m_outline->setOpacity(0.2);
            m_outline->setVisible(true);
            m_outline->setScale(scal);
        }

        QGraphicsScene::mouseMoveEvent(event);
        return;

    }
    else if (event->button()==Qt::LeftButton)
    {
        if(m_operator == DRAG&&m_outline!=NULL)
        {
            QPointF vec = event->scenePos()/scal-m_leftDownPos;

            removeItem(m_outline);
            m_outline = addPath(deformOutline(vec),m_outlinePen);
            m_outline->setOpacity(0.2);
            m_outline->setVisible(true);
            m_outline->setScale(scal);
        }

        QGraphicsScene::mouseMoveEvent(event);
        return;

    }

}

void DrawScene::removeDeletedSpots()
{
    QQueue<target_spot> temp= m_spots;
    m_spots.clear();
    for (size_t i = 0; i<temp.size(); ++i)
    {
        if (temp[i].state==STATE_SELECTED)
        {
            m_spots<<temp[i];
        }
        else
        {
            removeItem(temp[i].spot);
        }
    }
}

void DrawScene::hideSpot()
{
    for (size_t i = 0; i<m_spots.size(); ++i)
    {
        if (m_spots[i].state==STATE_DELETED)
        {
            m_spots[i].spot->setVisible(false);
        }
    }
}

void DrawScene::zoomOut()    // 图像放大
{
    qreal scal;
    if (m_backgroundPixmap!=NULL)
    {
        scal = m_backgroundPixmap->scale();
    }
    else
    {
        scal = 1.0;
    }

    ++scal;
    if (scal>4.0)
    {
        scal = 4.0;
    }

    // QPoint center(X_OFFSET*scal, m_dFocusDeep*scal + 14*RESOLUTION*scal);
    QPoint center;
    if ((_sceneType==SCENE_SEQUENCE || _sceneType == SCENE_TARGET)&& scal>1.1)
    {
        center = QPoint(X_OFFSET*scal, m_dFocusDeep*scal);
    }
    else
    {
        center = QPoint(this->width()*0.5*scal, this->height()*0.5*scal);
    }

    setSceneRect(center.x()-this->width()*0.5,center.y()-this->height()*0.5,this->width(),this->height());
    setScal(scal);

    // 同步放大图像参数
    viewer2D_info info;
    info.scal = scal;
    info.rect = sceneRect();
    emit changeView2D(info);
}

void DrawScene::zoomIn()     // 图像缩小
{
    qreal scal;
    if (m_backgroundPixmap!=NULL)
    {
        scal = m_backgroundPixmap->scale();
    }
    else
    {
        scal = 1.0;
    }

    --scal;
    if (scal<1.0)
    {
        scal = 1.0;
    }

    // QPoint center(X_OFFSET*scal, m_dFocusDeep*scal + 14*RESOLUTION*scal);
    QPoint center;
    if ((_sceneType==SCENE_SEQUENCE || _sceneType == SCENE_TARGET)&& scal>1.1)
    {
        center = QPoint(X_OFFSET*scal, m_dFocusDeep*scal);
    }
    else
    {
        center = QPoint(this->width()*0.5*scal, this->height()*0.5*scal);
    }

    setSceneRect(center.x()-this->width()*0.5,center.y()-this->height()*0.5,this->width(),this->height());
    setScal(scal);

    // 同步放大图像参数
    viewer2D_info info;
    info.scal = scal;
    info.rect = sceneRect();
    emit changeView2D(info);
}

// 反选
void DrawScene::selectInvert()
{
    QPen rawPen;
    QColor color = SPOT_COLOR;
    rawPen.setWidthF(1.5);
    rawPen.setColor(color);

    QPen redPen;
    QColor redcolor = Qt::red;
    redPen.setWidthF(1.5);
    redPen.setColor(redcolor);
    for (size_t i = 0; i<m_spots.size(); ++i)
    {
        if (m_spots[i].spot->pen().color()==QColor(Qt::red))
        {
            m_spots[i].spot->setPen(rawPen);
            m_spots[i].state = STATE_SELECTED;
            emit spotSelected(m_spots[i].idBorn,color);
        }
        else
        {
            m_spots[i].spot->setPen(redPen);
            m_spots[i].state = STATE_DELETED;
            emit spotSelected(m_spots[i].idBorn,redcolor);
        }
    }
}

void DrawScene::smartSelect()     // 智能选择辐照单元
{
    if (m_outline==NULL)
    {
        return;
    }

    QPen pen;
    QColor color = Qt::red;
    pen.setWidthF(1.5);
    pen.setColor(color);
    for (size_t i = 0;i<m_spots.size();++i)
    {
        if (!m_outline->contains(m_spots[i].pos))
        {
            m_spots[i].spot->setPen(pen);
            // 剔除在外的点
            emit spotSelected(m_spots[i].idBorn,color);
        }
    }
}

SPOT_LOCATION DrawScene::getLocation(const target_spot& spot)
{
    if (m_autoDraw!=NULL)
    {
        if (m_autoDraw->contains(spot.pos))
        {
            return LOCATION_INNER;
        }
        else if (spot.spot->collidesWithItem(m_autoDraw))
        {
            return LOCATION_EDGE;
        }
        else
        {
            return LOCATION_OUTER;
        }
    }
    else
    {
        // 没有勾画的边界，视为辐照点在边界内
        return LOCATION_INNER;
    }
}

void DrawScene::resumeDelected()  // 恢复被删除的辐照单元
{
    QPen pen;
    QColor color = SPOT_COLOR;
    pen.setWidthF(1.5);
    pen.setColor(color);
    for (size_t i = 0; i<m_spots.size(); ++i)
    {
        if (m_spots[i].state=STATE_DELETED)
        {
            m_spots[i].state = STATE_SELECTED;
            m_spots[i].spot->setPen(pen);
            m_spots[i].spot->setVisible(true);
            emit spotSelected(m_spots[i].idBorn,color);
        }
    }
}

void DrawScene::showSpot()    // 显示删除的辐照点
{
    for (size_t i = 0; i<m_spots.size(); ++i)
    {
        if (m_spots[i].state==STATE_DELETED)
        {
            m_spots[i].spot->setVisible(true);
        }
    }
}

// 隐藏所有辐照单元
void DrawScene::hideElement()
{
    for (size_t i = 0; i<m_spots.size(); ++i)
    {
        m_spots[i].spot->setVisible(false);
    }
}

// 显示所有辐照单元
void DrawScene::showElement()
{
    for (size_t i = 0; i<m_spots.size(); ++i)
    {
        m_spots[i].spot->setVisible(true);
    }
}

bool DrawScene::selectedExectue(QString id,QColor color)
{
    if (m_spots.empty())
    {
        qCDebug(DRAWSCENE()) << DRAWSCENE().categoryName()
                             << "spots empty !"
                             << TRACE_CMH();
        return false;
    }
    else
    {
        for (int i = 0; i < m_spots.size(); ++i)
        {
            if (m_spots[i].idBorn==id)
            {
                QPen pen;
                pen.setWidthF(1.5);
                pen.setColor(color);
                m_spots[i].spot->setPen(pen);
                m_spots[i].spot->setVisible(true);

                if (color==SPOT_COLOR)
                {
                    m_spots[i].state = STATE_SELECTED;
                }
                else if (color==Qt::red)
                {
                    m_spots[i].state = STATE_DELETED;
                }

                return true;
            }
        }
    }

    return false;
}

void DrawScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    qreal scal;
    scal = m_backgroundPixmap->scale();

    if(drawable()&&event->button()==Qt::LeftButton)
    {

       if(m_measureDrawable)
            {
                qCDebug(DRAWSCENE())<<DRAWSCENE().categoryName()
                                    <<"m_measureDrawable is"<<m_measureDrawable
                                    <<TRACE_CMH();
                    if (_sceneType==SCENE_LAYER)
                    {
                        // 层图像不处理
                        QGraphicsScene::mouseReleaseEvent(event);
                        return;
                    }


                if(_bChangeable)
                {

                    QPainterPath::Element start = m_change_path->elementAt(0);
                    if (m_change_path!=NULL)
                    {
                        delete m_change_path;
                        m_change_path = new QPainterPath(QPointF(start.x,start.y));
                    }

                    else
                    {
                        m_change_path = new QPainterPath(QPointF(start.x,start.y));
                    }

                    m_change_path->lineTo(event->scenePos());
                    if (m_change_path->length()<5)
                    {
                        return;
                    }
                    int len = m_change_path->length();
                    float fLen = len/RESOLUTION;
                    fLen = fLen/scal;
                    QString length(QString::number(fLen,'f',1));
                    length.append(" mm");
                    removeItem(m_newMeasureContour);
                    m_newMeasureContour = addPath(*m_change_path,m_measurePen);
                    m_newMeasureContour->setScale(m_backgroundPixmap->scale());
                    if (m_measure_changed==NULL)
                    {
                        m_measure_changed = new MeasureLine;
                    }
                    else
                    {
                        delete m_measure_changed;
                        m_measure_changed =NULL;
                        m_measure_changed = new MeasureLine;
                    }
                    m_measure_changed->setStartPoint(start.x/scal,start.y/scal);
                    m_measure_changed->setEndPoint(event->scenePos().x()/scal,event->scenePos().y()/scal);
                    m_measure_changed->setLine(addLine(start.x/scal,start.y/scal,event->scenePos().x()/scal,
                                               event->scenePos().y()/scal,m_measurePen));
                    m_measure_changed->setEndCross(addPath(m_measure_changed->setEndCrossPath(5)));
                    m_measure_changed->setStartCross(addPath(m_measure_changed->setStartCrossPath(5)));
                    m_measure_changed->setText(addText(length, this->font()));

                    m_measure_changed->getLine()->setVisible(true);
                    m_measure_changed->getLine()->show();
                    m_measure_changed->getLine()->setPen(m_measurePen);

                    m_measure_changed->getText()->setPos(m_measure_changed->getEndPoint()*scal);
                    m_measure_changed->getText()->setDefaultTextColor(Qt::green);
                    m_measure_changed->getText()->show();

                    m_measure_changed->getStartCross()->setVisible(true);
                    m_measure_changed->getStartCross()->show();
                    m_measure_changed->getStartCross()->setPen(m_extremPen);

                    m_measure_changed->getEndCross()->setVisible(true);
                    m_measure_changed->getEndCross()->show();
                    m_measure_changed->getEndCross()->setPen(m_extremPen);

                    m_measure_changed->setMeasureScal(scal);
                    m_measureSeries<<m_measure_changed;

                    _bChangeable = false;
                    removeItem(m_newMeasureContour);
                    _bChangeable = false;
                    _bPressed = false;

                    m_measure_changed = NULL;
                    QGraphicsScene::mouseReleaseEvent(event);
                    return;
                }
                if (_bMeasureable)
                {
                    if(m_measureContour==NULL)
                    {
                        m_measureContour = new QGraphicsPathItem;
                    }
                    else
                    {
                        delete m_measureContour;
                        m_measureContour = NULL;
                        m_measureContour = new QGraphicsPathItem;
                    }
                    if (m_path==NULL)
                    {
                        m_path = new QPainterPath(startPos);
                    }
                    else
                    {
                        delete m_path;
                        m_path = new QPainterPath(startPos);
                    }

                    m_path->lineTo(event->scenePos());
                    removeItem(m_measureContour);
                    if (m_path->length()<5)
                    {
                        return;
                    }
                    int len = m_path->length();
                    float fLen = len/RESOLUTION;
                    fLen = fLen/scal;
                    QString length(QString::number(fLen,'f',1));
                    length.append(" mm");
                    m_measureContour = addPath(*m_path,m_measurePen);
                    m_measureContour->setScale(m_backgroundPixmap->scale());
                    if (m_measure==NULL)
                    {
                        m_measure = new MeasureLine;
                    }
                    else
                    {
                        delete m_measure;
                        m_measure =NULL;
                        m_measure = new MeasureLine;
                    }

                        m_measure->setStartPoint(startPos.x()/scal,startPos.y()/scal);
                        m_measure->setEndPoint(event->scenePos().x()/scal,event->scenePos().y()/scal);
                        m_measure->setLine(addLine(startPos.x()/scal,startPos.y()/scal,event->scenePos().x()/scal,
                                                   event->scenePos().y()/scal,m_measurePen));
                        m_measure->setStartCross(addPath(m_measure->setStartCrossPath(5)));
                        m_measure->setEndCross(addPath(m_measure->setEndCrossPath(5)));
                        m_measure->setText(addText(length, this->font()));

                        m_measure->getLine()->setVisible(true);
                        m_measure->getLine()->setPen(m_measurePen);
                        m_measure->getLine()->show();

                        m_measure->getStartCross()->setVisible(true);
                        m_measure->getStartCross()->setPen(m_extremPen);
                        m_measure->getStartCross()->show();

                        m_measure->getEndCross()->setVisible(true);
                        m_measure->getEndCross()->setPen(m_extremPen);
                        m_measure->getEndCross()->show();

                        m_measure->getText()->setPos(m_measure->getEndPoint()*scal);
                        m_measure->getText()->show();
                        m_measure->getText()->setDefaultTextColor(Qt::green);

                        m_measure->setMeasureScal(scal);
                        m_measureSeries<<m_measure;

                        _bPressed = false;
                        _bMeasureable = false;

                        m_measure = NULL;
                        removeItem(m_measureContour);

                        QGraphicsScene::mouseReleaseEvent(event);
                        return;


                }
                if (_bPressed)
                {
                    if(!dist_and_index.isEmpty())
                    {
                        dist_and_index.clear();
                    }

                    for(int i =0;i<m_measureSeries.size();++i)
                    {
                        double dis;
                        dis = DistancePtToSegt(event->scenePos()/scal,m_measureSeries[i]);
                        dist_and_index.insert(dis,i);
                    }

                    if(dist_and_index.isEmpty())
                    {

                         QGraphicsScene::mouseReleaseEvent(event);
                         return;
                    }
                    else
                    {
                         if(!_bExist)  //选择一条线段
                         {
                             double min;
                             int frequence =0;//min出现次数
                             QHash<double, int>::iterator iter;
                             min=dist_and_index.begin().key();
                             for( iter=dist_and_index.begin(); iter!=dist_and_index.end(); ++iter)
                             {
                                 if(min>=iter.key())
                                 {
                                     min = iter.key();
                                 }
                             }

                             for( iter=dist_and_index.begin(); iter!=dist_and_index.end(); ++iter)
                             {
                                 if(iter.key()==min)
                                 {
                                     ++frequence;
                                 }
                             }
                             if(frequence==1)//只有一个最小值
                             {
                                 m_selectIndex = dist_and_index[min];
                             }
                             else   //鼠标离多条直线的距离相等，返回。
                             {
//                                 qDebug()<<"鼠标离多条直线的距离相等，返回。";
                                 dist_and_index.clear();
                                 QGraphicsScene::mouseReleaseEvent(event);
                                 return;
                             }
                             //选择的线段变红
                             m_measureSeries[m_selectIndex]->getLine()->setVisible(true);
                             m_measureSeries[m_selectIndex]->getLine()->setPen(m_selected);
                             m_measureSeries[m_selectIndex]->getStartCross()->setPen(m_selected);
                             m_measureSeries[m_selectIndex]->getEndCross()->setPen(m_selected);
                             m_measureSeries[m_selectIndex]->getText()->setDefaultTextColor(Qt::red);

                             dist_and_index.clear();
                             _bPressed = false;
                             _bExist = true;    //已经存在一条被选择的线段
                             QGraphicsScene::mouseReleaseEvent(event);
                             return;
                         }
                         if(_bExist)      //已经存在一条被选择的线段
                         {
                                 m_measureSeries[m_selectIndex]->getLine()->setVisible(true);
                                 m_measureSeries[m_selectIndex]->getLine()->setPen(m_measurePen);
                                 m_measureSeries[m_selectIndex]->getStartCross()->setPen(m_extremPen);
                                 m_measureSeries[m_selectIndex]->getEndCross()->setPen(m_extremPen);
                                 m_measureSeries[m_selectIndex]->getText()->setDefaultTextColor(Qt::green);
                                 m_selectIndex = -1;
                                 _bPressed = false;
                                 _bExist = false;
                                 QGraphicsScene::mouseReleaseEvent(event);
                                 return;
                          }

                     }
                }
            }

       if(targetDrawable())
            {

                if(m_displayContour==NULL)
                {
                    qCDebug(DRAWSCENE()) << DRAWSCENE().categoryName()
                                         << "display contour NULL "
                                         << TRACE_CMH();
                    QGraphicsScene::mouseReleaseEvent(event);
                    return;
                }

                if(m_tempContour==NULL)
                {
                    qCDebug(DRAWSCENE()) << DRAWSCENE().categoryName()
                                         << "temp contour NULL "
                                         << TRACE_CMH();
                    QGraphicsScene::mouseReleaseEvent(event);
                    return;
                }

                // 移动距离太短返回
                if (m_tempContour->path().elementCount()<3)
                {
                    QGraphicsScene::mouseReleaseEvent(event);
                    return;
                }

                if(m_operator!=OPER_MEASURE)
                {
                    QPainterPath path;
                    // 画靶标
                    path = m_tempContour->path();
                    path.connectPath(path);
                    m_tempContour = addPath(path,m_targetPen);
                    m_targetContour = addPath(m_tempContour->path(),m_targetPen);
                    if (m_targetContour!=NULL)
                    {
                        m_targetContour->setScale(scal);
                    }

                    if (m_displayContour!=NULL)
                    {
                        m_displayContour->setScale(scal);
                    }

                    if (m_displayContour!=NULL)
                    {
                        // delete m_displayContour;
                        removeItem(m_displayContour);
                        m_displayContour = NULL;
                    }

                    if (m_tempContour!=NULL)
                    {
                        // delete m_tempContour;
                        removeItem(m_tempContour);
                        m_tempContour = NULL;
                    }

                    qCDebug(DRAWSCENE())<<"target contour path "<<m_targetContour->path()
                                       << TRACE_CMH();

                }
            }

    }

    if (drawable()&&event->button()==Qt::LeftButton)
    {
                //removeItem(m_tempContour);
                if (m_operator!=OPER_MEASURE && !m_spots.empty())
                {
                    QAbstractGraphicsShapeItem* item;
                    for (size_t i = 0;i<m_spots.size();++i)
                    {
                        item = m_spots[i].spot;
                        if (item->contains(QPointF(event->scenePos().rx()/scal,event->scenePos().ry()/scal)))
                        {
                            QPen pen;
                            QColor color;
                            SPOT_STATE state;
                            // 设置辐照单元颜色变化
                            if (QColor(Qt::red)==item->pen().color())
                            {
                                color = SPOT_COLOR;
                                state = STATE_SELECTED;
                            }
                            else
                            {
                                color = Qt::red;
                                state = STATE_DELETED;
                            }

                            pen.setWidthF(1.5);
                            pen.setColor(color);
                            item->setPen(pen);
                            m_spots[i].state = state;
                            emit spotSelected(m_spots[i].idBorn,color);
                            QGraphicsScene::mouseReleaseEvent(event);
                            return;
                        }
                    }
                }

     }

    if(event->button()==Qt::MidButton)
    {

        if(m_operator==DRAG&&m_outline!=NULL)
        {
            m_outlinePos = event->scenePos()/scal;
            removeItem(m_outline);
            m_outline = addPath(moveOutline(m_outlinePos),m_outlinePen);
            m_outline->setOpacity(0.2);
            m_outline->setVisible(true);
            m_outline->setScale(scal);
            _bManualDrag = false;
        }
    }
    else if (event->button()==Qt::LeftButton)
    {
        if(m_operator==DRAG&&m_outline!=NULL)
        {
            QPointF vec = event->scenePos()/scal - m_leftDownPos;

            removeItem(m_outline);
            m_outline = addPath(deformOutline(vec),m_outlinePen);
            m_outline->setOpacity(0.2);
            m_outline->setVisible(true);
            m_outline->setScale(scal);
            _bManualDrag = false;
        }
    }

    qCDebug(DRAWSCENE())<<DRAWSCENE().categoryName()
                       <<"Exit mouse release !"
                      << TRACE_CMH();

    QGraphicsScene::mouseReleaseEvent(event);
    return;

}


void DrawScene::setViewer2D( viewer2D_info info)
{
    setScal(info.scal);
    setSceneRect(info.rect);
}

void DrawScene::setScal(qreal scal)
{
    if (m_backgroundPixmap!=NULL)
    {
        if (scal!=m_backgroundPixmap->scale())
        {
            m_backgroundPixmap->setScale(scal);
            qDebug()<<"set scal is "<< scal;
        }
    }
    else
    {
        qCDebug(DRAWSCENE())<<"set scal m_backgroundPixmap null !"
                            << TRACE_CMH();
    }

    if (m_targetContour!=NULL)
    {
        if (scal!=m_targetContour->scale())
        {
            m_targetContour->setScale(scal);
        }
    }
    else
    {
        qCDebug(DRAWSCENE())<<"set scal target contour null !"
                            << TRACE_CMH();
    }

    if (m_autoDraw!=NULL)
    {
        if (scal!=m_autoDraw->scale())
        {
            m_autoDraw->setScale(scal);
        }
    }
    else
    {
        qCDebug(DRAWSCENE())<<"set scal auto draw null !"
                            << TRACE_CMH();
    }

    if (m_tempContour!=NULL)
    {
        if (m_tempContour->scale()!=scal)
        {
            m_tempContour->setScale(scal);
        }
    }
    else
    {
        qCDebug(DRAWSCENE())<<"set scal temp contour null !"
                            << TRACE_CMH();
    }

    if (m_transducerContour!=NULL)
    {
        if (scal!=m_transducerContour->scale())
        {
            m_transducerContour->setScale(scal);
        }
    }
    else
    {
        qCDebug(DRAWSCENE())<<"set scal transducer contour null !"
                            << TRACE_CMH();
    }

    if (m_beam!=NULL)
    {
        if (scal!=m_beam->scale())
        {
            m_beam->setScale(scal);
        }
    }
    else
    {
        qCDebug(DRAWSCENE())<<"set scal beam null !"
                            << TRACE_CMH();
    }

    if (m_marker!=NULL)
    {
        if (scal!=m_marker->scale())
        {
            m_marker->setScale(scal);
        }
    }
    else
    {
        qCDebug(DRAWSCENE())<<"set scal marker null !"
                            << TRACE_CMH();
    }

    if (m_bakContour!=NULL)
    {
        if (scal!=m_bakContour->scale())
        {
            m_bakContour->setScale(scal);
        }
    }
    else
    {
        qCDebug(DRAWSCENE())<<"set scal m_bakContour null !"
                            << TRACE_CMH();
    }

    if (m_displayContour!=NULL)
    {
        if (m_displayContour->scale()!=scal)
        {
            m_displayContour->setScale(scal);
        }
    }
    else
    {
        qCDebug(DRAWSCENE())<<"set scal m_displayContour null !"
                            << TRACE_CMH();
    }

    if (m_sysEdges!=NULL)
    {
        if (m_sysEdges->scale()!=scal)
        {
            m_sysEdges->setScale(scal);
        }
    }
    else
    {
        qCDebug(DRAWSCENE())<<"set scal m_sysEdges null !"
                            << TRACE_CMH();
    }

    if (m_outline!=NULL)
    {
        if (m_outline->scale()!=scal)
        {
            m_outline->setScale(scal);
        }
    }
    else
    {
        qCDebug(DRAWSCENE())<<"set scal m_outline null !"
                            << TRACE_CMH();
    }

    if (m_measureContour!=NULL)
    {
       if (m_measureContour->scale()!=scal)
       {
           m_measureContour->setScale(scal);
       }
    }
    else
    {
        qCDebug(DRAWSCENE())<<"set scal m_measureContour null !"
                            << TRACE_CMH();
    }

    if (m_newMeasureContour!=NULL)
    {
       if (m_newMeasureContour->scale()!=scal)
       {
           m_newMeasureContour->setScale(scal);
       }
    }
    else
    {
        qCDebug(DRAWSCENE())<<"set scal m_newMeasureContour null !"
                            << TRACE_CMH();
    }

    QQueue<target_spot>::iterator iter = m_spots.begin();
    for (iter = m_spots.begin(); iter != m_spots.end();++iter)
    {
        iter->spot->setScale(scal);
    }

    if (!m_measureSeries.empty())
    {
        for (int i = 0; i < m_measureSeries.size();++i)
        {
            m_measureSeries[i]->getLine()->setScale(scal);
            m_measureSeries[i]->getStartCross()->setScale(scal);
            m_measureSeries[i]->getEndCross()->setScale(scal);
            m_measureSeries[i]->getText()->setScale(scal);
            m_measureSeries[i]->getText()->setPos(m_measureSeries[i]->getEndPoint()*scal);
        }
    }

}

bool DrawScene::equalViewer2D(viewer2D_info info)
{
    if (m_backgroundPixmap!=NULL)
    {
        QRectF rect = sceneRect();
        if (m_backgroundPixmap->scale()!=info.scal
            || rect.left() != info.rect.left()
            || rect.top() != info.rect.top()
            || rect.width() != info.rect.width()
            || rect.height() != info.rect.height())
        {
            return false;
        }
        else
        {
            return true;
        }
    }
    else
    {
        return false;
    }
}

void DrawScene::wheelEvent(QGraphicsSceneWheelEvent *event)
{
    // ctrl+滚轮放大
    if (!(event->modifiers()&Qt::ControlModifier))
    {
        return;
    }

    int factor = event->delta();

    qreal scal;
    if (m_backgroundPixmap!=NULL)
    {
        scal = m_backgroundPixmap->scale();
    }
    else
    {
        scal = 1.0;
    }

    if (factor>0.0)
    {
        scal*=1.2;
    }
    else
    {
        scal*=0.8;
    }

    if (scal>4.0)
    {
        scal = 4.0;
    }
    else if (scal<1.0)
    {
        scal = 1.0;
    }

    // QPoint center(X_OFFSET*scal, m_dFocusDeep*scal + 14*RESOLUTION*scal);
    QPoint center;
    if ((_sceneType==SCENE_SEQUENCE || _sceneType == SCENE_TARGET)&& scal>1.1)
    {
        center = QPoint(X_OFFSET*scal, m_dFocusDeep*scal);
    }
    else
    {
        center = QPoint(this->width()*0.5*scal, this->height()*0.5*scal);
    }

    setSceneRect(center.x()-this->width()*0.5,center.y()-this->height()*0.5,this->width(),this->height());
    setScal(scal);

    // 同步放大图像参数
    viewer2D_info info;
    info.scal = scal;
    info.rect = sceneRect();
    emit changeView2D(info);

    QGraphicsScene::wheelEvent(event);
}

void DrawScene::setTransducerVisible(const bool status)
{
    if (m_transducerContour==NULL)
    {
        drawTransducer(QPointF(X_OFFSET, m_dFocusDeep), 140, 180);
    }
    m_transducerContour->setVisible(status);
}

void DrawScene::setContoursVisiable(const bool status)
{
    if (m_outline!=NULL)
    {
        m_outline->setVisible(status);
    }
}

void DrawScene::setBeamVisible(const bool status)
{
    if (m_beam==NULL)
    {
        drawBeam();
    }
    m_beam->setVisible(status);
}

void DrawScene::removeTarget() {
    if (m_targetContour != NULL)
        removeItem(m_targetContour);
#ifdef DEBUG
    qDebug() << "m_targetContour = " << m_targetContour;
    QList<QGraphicsItem*> items = this->items();
    qDebug() << "The count of items:" << items.size();
#endif
}

void DrawScene::showDraw()    // 显示勾画轮廓及自动合成轮廓
{
  if (m_targetContour!=NULL)
  {
      setTargetVisible(true);
  }

  if (m_autoDraw!=NULL)
  {
      m_autoDraw->setVisible(true);
  }
}

void DrawScene::hideDraw()    // 隐藏勾画轮廓及自动合成轮廓
{
    if (m_targetContour!=NULL)
    {
        setTargetVisible(false);
    }

    if (m_autoDraw!=NULL)
    {
        m_autoDraw->setVisible(false);
    }
}

void DrawScene::resetTarget()
{
    if (m_bakContour==NULL)
    {
        m_bakContour = addPath(m_targetContour->path(),m_bakPen);
        m_bakContour->setScale(m_backgroundPixmap->scale());
        delete m_targetContour;
        removeItem(m_targetContour);
        m_targetContour = NULL;
    }
    else
    {
        delete m_bakContour;
        removeItem(m_bakContour);
        m_bakContour = NULL;

        m_bakContour = addPath(m_targetContour->path(),m_bakPen);
        delete m_targetContour;
        removeItem(m_targetContour);
        m_targetContour = NULL;
    }
}

void DrawScene::removeSpots() {
    if (m_spots.size())
        for (int i=0; i<m_spots.size(); i++)
            removeItem(m_spots[i].spot);

    m_spots.clear();
    m_x.clear();
    m_y.clear();
    m_z.clear();
    m_circles.clear();
}

void DrawScene::drawTransducer(QPointF centroid, qreal roc, qreal aperture)
{
    qreal theta = M_PI/2 - asin(aperture/2/roc);
    QPointF startPoint(centroid.rx()+ roc*cos(theta)*RESOLUTION, centroid.ry()-roc*sin(theta)*RESOLUTION);
    QPointF endPoint(centroid.rx()-roc*cos(theta)*RESOLUTION, centroid.ry()-roc*sin(theta)*RESOLUTION);
    QPainterPath path;
    path.moveTo(startPoint);
    theta *= 180/M_PI;
    path.arcTo(centroid.x()-roc*RESOLUTION, centroid.y()-roc*RESOLUTION,
               2*roc*RESOLUTION,2*roc*RESOLUTION, theta, 180-2*theta);

    theta = M_PI/2 - asin(aperture/2/roc);
    qreal rocNew = roc-10;
    QPointF startNew(centroid.rx()+rocNew*cos(theta)*RESOLUTION, centroid.ry()-rocNew*sin(theta)*RESOLUTION);
    path.moveTo(startNew);
    QPolygonF front;
    front << startPoint << startNew;
    path.addPolygon(front);

    theta *= 180/M_PI;
    path.arcTo(centroid.x()-rocNew*RESOLUTION, centroid.y()-rocNew*RESOLUTION,
               2*rocNew*RESOLUTION,2*rocNew*RESOLUTION, theta, 180-2*theta);

     path.lineTo(endPoint);

    m_transducerContour = addPath(path, m_transducerPen);
}

bool DrawScene::addOutline()
{
//    Spline algo(1.0);
//    algo.simplyOutline(outline);
//    QPainterPath path = algo.getPath();

    QPainterPath path;
//    for (int j = 0; j < outline.size();++j)
//    {
//        path.moveTo(outline[j][0]);
//        for (int i = 1; i < outline[j].size(); ++i)
//        {
//            path.lineTo(outline[j][i]);
//        }
//    }
    path.addEllipse(m_center,m_vEllipseParas[2],m_vEllipseParas[3]);

    path.setFillRule(Qt::WindingFill);
    if (m_outline!=NULL)
    {
        delete m_outline;
        removeItem(m_outline);
    }

    m_outline = addPath(path, m_outlinePen);
    if (m_backgroundPixmap!=NULL)
    {
        m_outline->setScale(m_backgroundPixmap->scale());
    }

    m_outline->setOpacity(0.2);
    m_outline->setVisible(true);

    return true;
}

void DrawScene::drawBeam()
{
    qreal roc = 4, aperture = 4;
    QPointF spot(X_OFFSET, m_dFocusDeep);
    qreal theta = atan(aperture/2/roc);
    QPointF cp3(spot.x() - BEAM_LENGTH*tan(theta)-4*RESOLUTION,
                spot.y() - BEAM_LENGTH);
    QPointF cp4(spot.x() + BEAM_LENGTH*tan(theta)+4*RESOLUTION,
                spot.y() - BEAM_LENGTH);
    QPointF cp7(spot.x() - BEAM_LENGTH*tan(theta)-4*RESOLUTION,
                spot.y() + BEAM_LENGTH);
    QPointF cp8(spot.x() + BEAM_LENGTH*tan(theta)+4*RESOLUTION,
                spot.y() + BEAM_LENGTH);

    qreal rocNew = 140;
    qreal apertureNew = 180;
    theta = M_PI/2 - asin(apertureNew*0.5/rocNew);
    QPointF cp9(spot.x() - BEAM_END_LENGTH*tan(theta),
                spot.y() + BEAM_END_LENGTH);
    QPointF cp10(spot.x() + BEAM_END_LENGTH*tan(theta),
                spot.y() + BEAM_END_LENGTH);

    QPolygonF frontBeam;
    QPointF centroid = QPointF(X_OFFSET, m_dFocusDeep);

    QPointF startPoint(centroid.rx()+ rocNew*cos(theta)*RESOLUTION, centroid.ry()-rocNew*sin(theta)*RESOLUTION);
    QPointF endPoint(centroid.rx()- rocNew*cos(theta)*RESOLUTION, centroid.ry()-rocNew*sin(theta)*RESOLUTION);
    QPainterPath path;
    path.moveTo(startPoint);
    frontBeam.clear();
    frontBeam << startPoint << cp4;
    path.addPolygon(frontBeam);
    path.cubicTo(cp4.rx(),cp4.ry(),spot.rx(),spot.ry(),cp8.rx(),cp8.ry());
    frontBeam.clear();
    frontBeam << cp8 << cp10;
    path.addPolygon(frontBeam);
    frontBeam.clear();
    frontBeam << cp10 << cp9;
    path.addPolygon(frontBeam);
    frontBeam.clear();
    frontBeam << cp9 << cp7;
    path.addPolygon(frontBeam);
    path.cubicTo(cp7.rx(),cp7.ry(),spot.rx(),spot.ry(),cp3.rx(),cp3.ry());

    frontBeam.clear();
    frontBeam << cp3 << endPoint;
    path.addPolygon(frontBeam);
    frontBeam.clear();
    frontBeam << endPoint << startPoint;
    path.addPolygon(frontBeam);

    path.setFillRule(Qt::WindingFill);
    m_beam = addPath(path, m_beamPen);
    if (m_backgroundPixmap!=NULL)
    {
        qDebug()<<"back ground not null !";
        m_beam->setScale(m_backgroundPixmap->scale());
    }
    //m_beam->setBrush(QBrush("#63B8FF"));
    m_beam->setOpacity(0.6);

    // qDebug()<<"draw beam success !";
}

qreal DrawScene::rotationAxis() {
    // QRectF rect = sceneRect();
    // return rect.topLeft().rx() + rect.width()/2;
    // 图像大小为576*576，海扶焦点为274*310，上定点位置为273*40 wangwenbo 20160901
    // return rect.topLeft().rx() + rect.width()*274/576;
    return X_OFFSET;
}

// 计算将外轮廓改成六边形时投影到x方向偏移量
double DrawScene::getPosX(int rotate,Circle circle,double delta)
{
    int tempAngle = (int)m_angle+90+rotate;
    if (tempAngle%60==0)
    {
        return delta;
    }

    switch(circle)
    {
        case Circle_one:
        return delta;
        break;

        case Circle_two:
        return 1.732*0.5*delta;
        break;

        case Circle_three:
        return 0.3333*sqrtf(7.0)*delta;
        break;

        case Circle_four:
        if (tempAngle%30==0)
        {
            return 1.732*0.5*delta;
        }
        else
        {
            return 0.25*sqrtf(13.0)*delta;
        }
        break;

        case Circle_five:
        if (tempAngle%60==12||tempAngle%60==48)
        {
            return sqrtf(21.0)*0.2*delta;
        }
        else
        {
            return 0.2*sqrtf(19.0)*delta;
        }

        break;
    }

}

void DrawScene::drawSpotsNew(int rotate,SPOT_DISTRIBUTE distribute)
{
    qreal rAxis = rotationAxis();
    // 找到中间三行，从下向上添加
    if (distribute.isLayerDown) // 添加下层点
    {
        QPointF focus;
        focus.setY(SPOT_LENGTH + m_dFocusDeep);
        if (distribute.isCenter)
        {
            // 添加中间点
            focus.setX(rAxis);
            smartAddSpot(focus,Circle_center);
        }

        if (distribute.isCircleOne)
        {
            // 添加内圈点
            // Left of the rotation axis
            focus.setX(rAxis-m_dSpotDistance);
            smartAddSpot(focus,Circle_one);
            // Right of the rotation axis
            focus.setX(rAxis+m_dSpotDistance);
            smartAddSpot(focus,Circle_one);

        }

        if (distribute.isCircleTwo)
        {
            // 添加中圈点
            // Left of the rotation axis
            // focus.setX(rAxis-2*m_dSpotDistance);
            focus.setX(rAxis - getPosX(rotate,Circle_two,2*m_dSpotDistance));
            smartAddSpot(focus,Circle_two);

            // Right of the rotation axis
            // focus.setX(rAxis+2*m_dSpotDistance);
            focus.setX(rAxis + getPosX(rotate,Circle_two,2*m_dSpotDistance));
            smartAddSpot(focus,Circle_two);
        }

        if (distribute.isCircleThree)
        {
            // 添加外圈点
            // Left of the rotation axis
            // focus.setX(rAxis-3*m_dSpotDistance);
            focus.setX(rAxis - getPosX(rotate,Circle_three,3*m_dSpotDistance));
            smartAddSpot(focus,Circle_three);
            // Right of the rotation axis
            // focus.setX(rAxis+3*m_dSpotDistance);
            focus.setX(rAxis + getPosX(rotate,Circle_three,3*m_dSpotDistance));
            smartAddSpot(focus,Circle_three);
        }

        if (distribute.isCircleFour)
        {
            // 添加外圈点
            // Left of the rotation axis
            // focus.setX(rAxis-4*m_dSpotDistance);
            focus.setX(rAxis - getPosX(rotate,Circle_four,4*m_dSpotDistance));
            smartAddSpot(focus,Circle_four);
            // Right of the rotation axis
            // focus.setX(rAxis+4*m_dSpotDistance);
            focus.setX(rAxis + getPosX(rotate,Circle_four,4*m_dSpotDistance));
            smartAddSpot(focus,Circle_four);
        }

        if (distribute.isCircleFive)
        {
            // 添加外圈点
            // Left of the rotation axis
            // focus.setX(rAxis-5*m_dSpotDistance);
            focus.setX(rAxis - getPosX(rotate,Circle_five,5*m_dSpotDistance));
            smartAddSpot(focus,Circle_five);
            // Right of the rotation axis
            // focus.setX(rAxis+5*m_dSpotDistance);
            focus.setX(rAxis + getPosX(rotate,Circle_five,5*m_dSpotDistance));
            smartAddSpot(focus,Circle_five);
        }
    }

    if (distribute.isLayerMiddle) // 添加中层点
    {
        // qDebug()<<"layer middle select !";
        QPointF focus;
        focus.setY(m_dFocusDeep);
        if (distribute.isCenter)
        {
            // 添加中间点
            focus.setX(rAxis);
            smartAddSpot(focus,Circle_center);
        }

        if (distribute.isCircleOne)
        {
            // 添加内圈点
            // Left of the rotation axis
            focus.setX(rAxis-m_dSpotDistance);
            smartAddSpot(focus,Circle_one);
            // Right of the rotation axis
            focus.setX(rAxis+m_dSpotDistance);
            smartAddSpot(focus,Circle_one);
        }

        if (distribute.isCircleTwo)
        {
            // 添加中圈点
            // Left of the rotation axis
            // focus.setX(rAxis-2*m_dSpotDistance);
            focus.setX(rAxis - getPosX(rotate,Circle_two,2*m_dSpotDistance));
            smartAddSpot(focus,Circle_two);
            // Right of the rotation axis
            // focus.setX(rAxis+2*m_dSpotDistance);
            focus.setX(rAxis + getPosX(rotate,Circle_two,2*m_dSpotDistance));
            smartAddSpot(focus,Circle_two);
        }

        if (distribute.isCircleThree)
        {
            // 添加外圈点
            // Left of the rotation axis
            // focus.setX(rAxis-3*m_dSpotDistance);
            focus.setX(rAxis - getPosX(rotate,Circle_three,3*m_dSpotDistance));
            smartAddSpot(focus,Circle_three);
            // Right of the rotation axis
            // focus.setX(rAxis+3*m_dSpotDistance);
            focus.setX(rAxis + getPosX(rotate,Circle_three,3*m_dSpotDistance));
            smartAddSpot(focus,Circle_three);
        }

        if (distribute.isCircleFour)
        {
            // 添加外圈点
            // Left of the rotation axis
            // focus.setX(rAxis-4*m_dSpotDistance);
            focus.setX(rAxis - getPosX(rotate,Circle_four,4*m_dSpotDistance));
            smartAddSpot(focus,Circle_four);
            // Right of the rotation axis
            // focus.setX(rAxis+4*m_dSpotDistance);
            focus.setX(rAxis + getPosX(rotate,Circle_four,4*m_dSpotDistance));
            smartAddSpot(focus,Circle_four);
        }

        if (distribute.isCircleFive)
        {
            // 添加外圈点
            // Left of the rotation axis
            // focus.setX(rAxis-5*m_dSpotDistance);
            focus.setX(rAxis - getPosX(rotate,Circle_five,5*m_dSpotDistance));
            smartAddSpot(focus,Circle_five);
            // Right of the rotation axis
            // focus.setX(rAxis+5*m_dSpotDistance);
            focus.setX(rAxis + getPosX(rotate,Circle_five,5*m_dSpotDistance));
            smartAddSpot(focus,Circle_five);
        }

    }

    if (distribute.isLayerUp) // 添加中层点
    {
        // qDebug()<<"layer up selected !";
        QPointF focus;
        focus.setY(m_dFocusDeep-SPOT_LENGTH);
        if (distribute.isCenter)
        {
            // 添加中间点
            focus.setX(rAxis);
            smartAddSpot(focus,Circle_center);
        }
        if (distribute.isCircleOne)
        {
            // 添加内圈点
            // Left of the rotation axis
            focus.setX(rAxis-m_dSpotDistance);
            smartAddSpot(focus,Circle_one);
            // Right of the rotation axis
            focus.setX(rAxis+m_dSpotDistance);
            smartAddSpot(focus,Circle_one);
        }
        if (distribute.isCircleTwo)
        {
            // 添加中圈点
            // Left of the rotation axis
            // focus.setX(rAxis-2*m_dSpotDistance);
            focus.setX(rAxis - getPosX(rotate,Circle_two,2*m_dSpotDistance));
            smartAddSpot(focus,Circle_two);
            // Right of the rotation axis
            // focus.setX(rAxis+2*m_dSpotDistance);
            focus.setX(rAxis + getPosX(rotate,Circle_two,2*m_dSpotDistance));
            smartAddSpot(focus,Circle_two);
        }

        if (distribute.isCircleThree)
        {
            // 添加外圈点
            // Left of the rotation axis
            // focus.setX(rAxis-3*m_dSpotDistance);
            focus.setX(rAxis - getPosX(rotate,Circle_three,3*m_dSpotDistance));
            smartAddSpot(focus,Circle_three);
            // Right of the rotation axis
            // focus.setX(rAxis+3*m_dSpotDistance);
            focus.setX(rAxis + getPosX(rotate,Circle_three,3*m_dSpotDistance));
            smartAddSpot(focus,Circle_three);
        }

        if (distribute.isCircleFour)
        {
            // 添加外圈点
            // Left of the rotation axis
            // focus.setX(rAxis-4*m_dSpotDistance);
            focus.setX(rAxis - getPosX(rotate,Circle_four,4*m_dSpotDistance));
            smartAddSpot(focus,Circle_four);
            // Right of the rotation axis
            // focus.setX(rAxis+4*m_dSpotDistance);
            focus.setX(rAxis + getPosX(rotate,Circle_four,4*m_dSpotDistance));
            smartAddSpot(focus,Circle_four);
        }

        if (distribute.isCircleFive)
        {
            // 添加外圈点
            // Left of the rotation axis
            // focus.setX(rAxis-5*m_dSpotDistance);
            focus.setX(rAxis - getPosX(rotate,Circle_five,5*m_dSpotDistance));
            smartAddSpot(focus,Circle_five);
            // Right of the rotation axis
            // focus.setX(rAxis+5*m_dSpotDistance);
            focus.setX(rAxis + getPosX(rotate,Circle_five,5*m_dSpotDistance));
            smartAddSpot(focus,Circle_five);
        }
    }
}

void DrawScene::drawSpots(SPOT_DISTRIBUTE distribute)
{
    // qDebug()<<"Enter draw spots !";
    qreal rAxis = rotationAxis();
    // 找到中间三行，从下向上添加
    if (distribute.isLayerDown) // 添加下层点
    {
        QPointF focus;
        focus.setY(SPOT_LENGTH + m_dFocusDeep);
        if (distribute.isCenter)
        {
            // 添加中间点
            focus.setX(rAxis);
            smartAddSpot(focus,Circle_center);
        }
        if (distribute.isCircleOne)
        {
            // 添加内圈点
            // Left of the rotation axis
            focus.setX(rAxis-CELL4MM_LATERAL_DISTANCE);
            smartAddSpot(focus,Circle_one);
            // Right of the rotation axis
            focus.setX(rAxis+CELL4MM_LATERAL_DISTANCE);
            smartAddSpot(focus,Circle_one);
        }
        if (distribute.isCircleTwo)
        {
            // 添加中圈点
            // Left of the rotation axis
            focus.setX(rAxis-2*CELL4MM_LATERAL_DISTANCE);
            smartAddSpot(focus,Circle_two);
            // Right of the rotation axis
            focus.setX(rAxis+2*CELL4MM_LATERAL_DISTANCE);
            smartAddSpot(focus,Circle_two);
        }
        if (distribute.isCircleThree)
        {
            // 添加外圈点
            // Left of the rotation axis
            focus.setX(rAxis-3*CELL4MM_LATERAL_DISTANCE);
            smartAddSpot(focus,Circle_three);
            // Right of the rotation axis
            focus.setX(rAxis+3*CELL4MM_LATERAL_DISTANCE);
            smartAddSpot(focus,Circle_three);
        }
    }

    if (distribute.isLayerMiddle) // 添加中层点
    {
        // qDebug()<<"layer middle select !";
        QPointF focus;
        focus.setY(m_dFocusDeep);
        if (distribute.isCenter)
        {
            // 添加中间点
            focus.setX(rAxis);
            smartAddSpot(focus,Circle_center);
        }
        if (distribute.isCircleOne)
        {
            // 添加内圈点
            // Left of the rotation axis
            focus.setX(rAxis-CELL4MM_LATERAL_DISTANCE);
            smartAddSpot(focus,Circle_one);
            // Right of the rotation axis
            focus.setX(rAxis+CELL4MM_LATERAL_DISTANCE);
            smartAddSpot(focus,Circle_one);
        }
        if (distribute.isCircleTwo)
        {
            // 添加中圈点
            // Left of the rotation axis
            focus.setX(rAxis-2*CELL4MM_LATERAL_DISTANCE);
            smartAddSpot(focus,Circle_two);
            // Right of the rotation axis
            focus.setX(rAxis+2*CELL4MM_LATERAL_DISTANCE);
            smartAddSpot(focus,Circle_two);
        }
        if (distribute.isCircleThree)
        {
            // 添加外圈点
            // Left of the rotation axis
            focus.setX(rAxis-3*CELL4MM_LATERAL_DISTANCE);
            smartAddSpot(focus,Circle_three);
            // Right of the rotation axis
            focus.setX(rAxis+3*CELL4MM_LATERAL_DISTANCE);
            smartAddSpot(focus,Circle_three);
        }
    }

    if (distribute.isLayerUp) // 添加中层点
    {
        // qDebug()<<"layer up selected !";
        QPointF focus;
        focus.setY(m_dFocusDeep-SPOT_LENGTH);
        if (distribute.isCenter)
        {
            // 添加中间点
            focus.setX(rAxis);
            smartAddSpot(focus,Circle_center);
        }
        if (distribute.isCircleOne)
        {
            // 添加内圈点
            // Left of the rotation axis
            focus.setX(rAxis-CELL4MM_LATERAL_DISTANCE);
            smartAddSpot(focus,Circle_one);
            // Right of the rotation axis
            focus.setX(rAxis+CELL4MM_LATERAL_DISTANCE);
            smartAddSpot(focus,Circle_one);
        }
        if (distribute.isCircleTwo)
        {
            // 添加中圈点
            // Left of the rotation axis
            focus.setX(rAxis-2*CELL4MM_LATERAL_DISTANCE);
            smartAddSpot(focus,Circle_two);
            // Right of the rotation axis
            focus.setX(rAxis+2*CELL4MM_LATERAL_DISTANCE);
            smartAddSpot(focus,Circle_two);
        }

        if (distribute.isCircleThree)
        {
            // 添加外圈点
            // Left of the rotation axis
            focus.setX(rAxis-3*CELL4MM_LATERAL_DISTANCE);
            smartAddSpot(focus,Circle_three);
            // Right of the rotation axis
            focus.setX(rAxis+3*CELL4MM_LATERAL_DISTANCE);
            smartAddSpot(focus,Circle_three);
        }
    }
    // qDebug()<<"Draw spot success !";
}

// TODO: update the algorithm to generate the spots
void DrawScene::drawSpots()
{
    if (m_targetContour==NULL)
    {
        qCDebug(DRAWSCENE())<<DRAWSCENE().categoryName()
                            <<"target contour null"
                            <<TRACE_CMH();
        return;
    }

    if (m_targetContour->isVisible())
    {
//        qreal scal = m_backgroundPixmap->scale();
        qreal rAxis = rotationAxis();
        QRectF contourRect = m_targetContour->boundingRect();

        qreal top = contourRect.top();
        qreal bottom = contourRect.bottom();

        if (m_dFocusDeep <= top || m_dFocusDeep >= bottom) {
            qDebug() << "140 mm is not in the middle of the target";
            return;
        }


        qint8 up = (top - m_dFocusDeep) / SPOT_LENGTH;
        qint8 down = (bottom - m_dFocusDeep) / SPOT_LENGTH;

        // 找到中间三行，从下向上添加
        if (down>1)
        {
            down = 1;
        }

        if (up<-1)
        {
            up = -1;
        }

        for(qint8 j=down; j>=up; --j) {
            QPointF focus;
            focus.setY(j*SPOT_LENGTH + m_dFocusDeep);
            for(int i=1; i<4; i++) {
                // Left of the rotation axis
                focus.setX(rAxis-i*CELL4MM_LATERAL_DISTANCE);
                addSpot(focus);
                // Right of the rotation axis
                focus.setX(rAxis+i*CELL4MM_LATERAL_DISTANCE);
                addSpot(focus);
            }
        }
    }else qDebug() << "No target contour.";
}

void DrawScene::addExamplePoint(QString idBorn,QPointF p,int offset)
{
    QRectF rect;
    rect.setWidth(m_dSpotWide);
    rect.setHeight(m_dSpotWide);
    // 将单位为米的笛卡尔坐标系转换成图像坐标系
    QPointF imagePoint(offset-p.y()*RESOLUTION*1000,p.x()*RESOLUTION*1000+offset);
    rect.moveCenter(imagePoint);
    target_spot spot;
    spot.idBorn = idBorn;
    spot.pos = imagePoint;
    spot.state = STATE_SELECTED;
    spot.spot = addRect(rect, m_spotPen);
    if (m_backgroundPixmap!=NULL)
    {
        spot.spot->setScale(m_backgroundPixmap->scale());
    }
    m_spots << spot;
}

void DrawScene::clearExamplePoint()
{
    QAbstractGraphicsShapeItem* item;
    for (size_t i = 0;i<m_spots.size();++i)
    {
        item = m_spots[i].spot;
        delete item;
        removeItem(item);
        item = NULL;
    }
    m_spots.clear();
}

QGraphicsPathItem* DrawScene::targetContour()
{
    return m_targetContour;
}

void DrawScene::smartAddSpot(QPointF& focus,const Circle& circle)
{
    // QAbstractGraphicsShapeItem* item;
    target_spot spot;
    if (m_bEleRec)
    {
        QRectF rect;
        rect.setWidth(m_dSpotWide);
        rect.setHeight(SPOT_LENGTH);
        rect.moveCenter(focus);
        spot.spot = addRect(rect);
    }
    else
    {
        QRectF elli;
        elli.setWidth(m_dSpotWide);
        elli.setHeight(SPOT_LENGTH);
        elli.moveCenter(focus);
        spot.spot = addEllipse(elli);
    }

    // m_spots为做过放大处理的椭圆或矩形
    // m_points未做过缩放处理的原始位置，也是辐照对应的位置
    spot.spot->setScale(m_backgroundPixmap->scale());
    spot.pos = focus;
    spot.state = STATE_SELECTED;
    spot.spot->setPen(QPen(Qt::transparent));
    spot.idBorn = QString::number(getAngle());
    spot.idBorn.append("_");
    spot.idBorn.append(QString::number(m_spots.size()));
    spot.circle = circle;

    SPOT_LOCATION location = getLocation(spot);
    switch(location)
    {
        case LOCATION_INNER:
        spot.spot->setPen(m_spotPen);
        m_spots << spot;
        break;

        case LOCATION_EDGE:
        spot.spot->setPen(m_edgePen);
        m_spots << spot;
        break;

        case LOCATION_OUTER:
        break;

        default:
        break;
    }

}

void DrawScene::addSpot(QPointF& focus)
{
    if (m_targetContour==NULL)
    {
        return;
    }

    if (m_targetContour->contains(focus))
    {

    target_spot spot;
    spot.state = STATE_SELECTED;
    if (m_bEleRec)
    {
        QRectF rect;
        rect.setWidth(m_dSpotWide);
        rect.setHeight(SPOT_LENGTH);
        rect.moveCenter(focus);
        spot.spot = addRect(rect, m_spotPen);
    }
    else
    {
        QRectF elli;
        elli.setWidth(m_dSpotWide);
        elli.setHeight(SPOT_LENGTH);
        elli.moveCenter(focus);
        spot.spot = addEllipse(elli, m_spotPen);
    }
        // m_spots为做过放大处理的椭圆或矩形
        // m_points未做过缩放处理的原始位置，也是辐照对应的位置
        spot.spot->setScale(m_backgroundPixmap->scale());
        spot.pos = focus;
        // 创建id born
        spot.idBorn = QString::number(getAngle());
        spot.idBorn.append("_");
        spot.idBorn.append(QString::number(m_spots.size()));
        m_spots << spot;
        qDebug()<<"add point "<<focus.rx()<<focus.ry();
    }else qDebug() << "This spot is outside the contour";
}

bool DrawScene::isUsefulScene()
{
    return !m_spots.empty();
}

QQueue<QString>& DrawScene::getIdBorn()
{
    m_idBorn.clear();
    for (size_t i = 0;i<m_spots.size();++i)
    {
        QGraphicsItem* item = m_spots[i].spot;
        if (item->isVisible()&&m_spots[i].spot->pen().color()!=Qt::red)
        {
            m_idBorn << m_spots[i].idBorn;
        }
    }

    return m_idBorn;
}

QQueue<double>& DrawScene::getFocusX()
{
    m_x.clear();
    for (size_t i = 0;i<m_spots.size();++i)
    {
        if (m_spots[i].spot->isVisible()&&m_spots[i].spot->pen().color()!=QColor(Qt::red))
        {
            m_x << spotX(m_spots[i].pos);
        }
    }
    return m_x;
}

QQueue<double>& DrawScene::getFocusY()
{
    m_y.clear();
    for (size_t i = 0;i<m_spots.size();++i)
    {
        QAbstractGraphicsShapeItem* item = m_spots[i].spot;
        if (item->isVisible()&&item->pen().color()!=QColor(Qt::red))
        {
            m_y << spotY(m_spots[i].pos);
        }
    }
    return m_y;
}

QQueue<Circle>& DrawScene::getCircles()
{
    m_circles.clear();
    for (size_t i = 0;i<m_spots.size();++i)
    {
        QAbstractGraphicsShapeItem* item = m_spots[i].spot;
        if (item->isVisible()&&item->pen().color()!=Qt::red)
        {
            m_circles << m_spots[i].circle;
        }
    }

    return m_circles;
}

QQueue<double>& DrawScene::getFocusZ()
{
    m_z.clear();
    for (size_t i = 0;i<m_spots.size();++i)
    {
        QAbstractGraphicsShapeItem* item = m_spots[i].spot;
        if (item->isVisible()&&item->pen().color()!=Qt::red)
        {
            m_z << spotZ(m_spots[i].pos);
        }
    }

    return m_z;
}

void DrawScene::setSpotsVisible(const bool status) {
    drawSpots();
    if (m_spots.size())
        for (int i=0; i<m_spots.size(); i++)
            m_spots[i].spot->setVisible(status);
}

// 获取辐照点空间位置
double DrawScene::spotX(QPointF &focus)
{
    double temp = ((focus.rx() - rotationAxis()) / RESOLUTION * cos(m_angle / 180 * M_PI)) / 1000;
    return -1.0*temp;
}

double DrawScene::spotY(QPointF &focus)
{
    // 超声图像角度为负的时候，看到的点位置和正角度相反
    double temp = ((focus.rx() - rotationAxis()) / RESOLUTION * sin(m_angle / 180 * M_PI)) / 1000;
    return -1.0*temp;
}

double DrawScene::spotZ(QPointF &focus)
{
    return ((focus.ry() - m_dFocusDeep) / RESOLUTION + 140) / 1000;
}

float DrawScene::angle(QPointF center, QPointF point)
{
    float angle_temp;
    float xx, yy;

    xx = point.x() - center.x();
    yy = point.y() - center.y();

    if (xx == 0.0)
        angle_temp = PI / 2.0;
   else
        angle_temp = atan(fabs(yy / xx));

    if ((xx < 0.0) && (yy >= 0.0))
        angle_temp = PI - angle_temp;
    else if ((xx < 0.0) && (yy < 0.0))
        angle_temp = PI + angle_temp;
   else if ((xx >= 0.0) && (yy < 0.0))
        angle_temp = PI * 2.0 - angle_temp;

   return (angle_temp);
}

bool DrawScene::getIntersectPath(DrawScene* scene,QPainterPath& path)
{
    if (m_targetContour==NULL)
    {
        qCDebug(DRAWSCENE()) << DRAWSCENE().categoryName()
                             << "target contour null !"
                             << TRACE_CMH();
        return false;
    }

    if (scene==NULL)
    {
        qCDebug(DRAWSCENE()) << DRAWSCENE().categoryName()
                             << "scene null !"
                             << TRACE_CMH();
        return false;
    }

    float x = 0.0;
    float y = 0.0;
    QQueue<QPainterPath::Element> points;
    QGraphicsPathItem* target = scene->targetContour();
    qreal scal = m_backgroundPixmap->scale();

    if (target==NULL)
    {
        qCDebug(DRAWSCENE()) << DRAWSCENE().categoryName()
                             << "scene target null !"
                             << TRACE_CMH();
        return false;
    }

    int count = m_targetContour->path().elementCount();
    for (int i = 0 ; i < count; ++i)
    {
        QPainterPath::Element ele = m_targetContour->path().elementAt(i);
        if (target->contains(ele))
        {
            x += ele.x;
            y += ele.y;
            points.push_back(ele);
        }
    }

    count = target->path().elementCount();
    for (int i = 0 ; i < count; ++i)
    {
        QPainterPath::Element ele = target->path().elementAt(i);
        if (m_targetContour->contains(ele))
        {
            x += ele.x;
            y += ele.y;
            points.push_back(ele);
        }
    }

    if (points.size() > 0)
    {
        x /= points.size();
        y /= points.size();
    }
    else
    {
        qCDebug(DRAWSCENE()) << DRAWSCENE().categoryName()
                             << "edge pointer size zero !"
                             << TRACE_CMH();
        return false;
    }

    QPointF center(x,y);
    qDebug()<<"center coordinate "<<center;
    QMap<float,QPainterPath::Element> temp;
    for (int i = 0; i < points.size(); ++i)
    {
        temp.insert(angle(center,QPointF(points[i].x,points[i].y)),points[i]);
    }
    QMap<float,QPainterPath::Element>::iterator iter = temp.begin();
    path.moveTo(iter.value());
    qDebug()<<"first point"<<iter.value();
    ++iter;
    for (;iter!=temp.end();++iter)
    {
        path.lineTo(iter.value());
    }
    path.lineTo(temp.begin().value());

    return true;
}

void DrawScene::removeMeasureLine(MeasureLine *measureline)
{
    if(measureline == NULL)
    {
        return;
    }
    else
    {
        removeItem(measureline->getLine());
        removeItem(measureline->getText());
        removeItem(measureline->getStartCross());
        removeItem(measureline->getEndCross());
    }
}

double DrawScene::DistancePtToSegt(QPointF ptP,MeasureLine* measureLine)
{
   QPointF ptA,ptB;
   double distAP,distAB;
   double r;
   double angle;
   double dist;

   ptA = measureLine->getStartPoint();
   ptB = measureLine->getEndPoint();
   distAP = QLineF(ptA,ptP).length();
   distAB = QLineF(ptA,ptB).length();
   angle = QLineF(ptA,ptP).angleTo(QLineF(ptA,ptB));
   if(angle >180)
   {
        angle = 360 - angle;
   }

   r = distAP*sin(angle/180*M_PI)/distAB;

   if(r>0&&r<1)
   {
       if(ptA.x()==ptB.x())
       {
           dist = fabs(ptP.x()-ptA.x());
       }
       else if(ptA.y()==ptB.y())
       {
           dist = fabs(ptP.y()-ptA.y());
       }
       else
       {
           dist = sin(angle / 180 * M_PI)* distAP;
       }

   }
   else if(r<=0)
   {
       dist = distAP;
   }
   else if(r>=1)
   {
       dist =  QLineF(ptP,ptB).length();
   }
    return dist;
}

void DrawScene::setCenterAndOutline(std::vector<std::vector<QPointF> > points,QPointF center)
{
    m_points.clear();
    for (int j = 0; j < points.size();++j)
    {
        for (int i = 0; i < points[j].size(); ++i)
        {
            m_points.push_back(points[j][i]-center);
        }
    }

    LSEllipse algo;
    m_vEllipseParas = algo.getEllipseparGauss(m_points);
    m_center = center;
}
//std::vector<std::vector<QPointF> > DrawScene::getPoints()
//{
//    return m_points;
//}

void DrawScene::setPoints(std::vector<std::vector<QPointF> > points)
{
    // m_points = points;
    if(points.empty())
    {
        return;
    }
    QMap<qreal,int> m_xData;
    QMap<qreal,int> m_yData;
    if(!m_xData.isEmpty())
    {
        m_xData.clear();
    }
    if(!m_yData.isEmpty())
    {
        m_yData.clear();
    }
    int index=0;
    for (int j = 0; j < points.size();++j)
    {
        m_xData.insert(points[j][0].x(),index);
        m_yData.insert(points[j][0].y(),index);
        index++;
        for (int i = 1; i < points[j].size(); ++i)
        {
            m_xData.insert(points[j][i].x(),index);
            m_yData.insert(points[j][i].y(),index);
            index++;
        }
    }
        qreal x_Center;
        qreal y_Center;
        QPointF m_center_outline;
        QMap<qreal,int>::iterator iter;
        iter=m_xData.begin();
        x_Center=iter.key();
        iter = m_xData.end();
        iter--;
        x_Center+=iter.key();
        x_Center*=0.5;

        iter=m_yData.begin();
        y_Center=iter.key();
        iter = m_yData.end();
        iter--;
        y_Center+=iter.key();
        y_Center*=0.5;

        m_center_outline.setX(x_Center);
        m_center_outline.setY(y_Center);

        m_points.clear();
        for (int j = 0; j < points.size();++j)
        {
            for (int i = 0; i < points[j].size(); ++i)
            {
                m_points.push_back(points[j][i]-m_center_outline);
            }
        }

        LSEllipse algo;
        m_vEllipseParas = algo.getEllipseparGauss(m_points);
        m_center = m_center_outline;
}

QPainterPath DrawScene::deformOutline(QPointF vec)
{
    m_outlinePos;
    QPainterPath path;

    if (fabs(vec.x())>fabs(vec.y()))
    {
        m_vEllipseParas[2] *= 1.05;
        m_vEllipseParas[3] *= 0.952381;
    }
    else
    {
        m_vEllipseParas[2] *= 0.952381;
        m_vEllipseParas[3] *= 1.05;
    }

    path.addEllipse(m_center,m_vEllipseParas[2],m_vEllipseParas[3]);

    path.setFillRule(Qt::WindingFill);
    return path;
}

QPainterPath DrawScene::moveOutline(QPointF center)
{
    m_center = center;
    QPainterPath path;
    path.addEllipse(m_center,m_vEllipseParas[2],m_vEllipseParas[3]);

    path.setFillRule(Qt::WindingFill);
    return path;
}
