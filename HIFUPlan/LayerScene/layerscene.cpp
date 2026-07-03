#include <math.h>
#include <QSettings>

#include "layerscene.h"
#include <QDebug>

#include "drawscene.h"
#include "qmath.h"
double RESOLUTION = 2.07692;

Q_LOGGING_CATEGORY(LAYERSCENE,"LAYERSCENE")

LayerScene::LayerScene(QObject *parent)
: QGraphicsScene(parent)
{
    m_drawable = false;
    m_targetDrawable = false;
    m_targetContour=NULL;
    m_marginContour=NULL;
    m_currentBlockId = " ";

    m_range = NULL;
    m_border = NULL;

    m_eOper = OPER_ERROR;
    m_rangePos = QPoint(0,0);
    m_iOrder = 1;

    // 以前删除的靶区区域，作为后来勾画的参考
    m_backgroundPixmap=NULL;

    m_targetPen.setColor(Qt::yellow);
    m_targetPen.setWidth(2);
    m_targetPen.setStyle(Qt::SolidLine);

    m_marginPen.setColor(Qt::yellow);
    m_marginPen.setWidth(2);
    m_marginPen.setStyle(Qt::SolidLine);

    m_rangePen.setColor(Qt::red);
    m_rangePen.setWidth(2);
    m_rangePen.setStyle(Qt::SolidLine);

    m_selectedPen.setColor(Qt::blue);
    m_selectedPen.setWidth(2);
    m_selectedPen.setStyle(Qt::SolidLine);

    m_measurePen.setStyle(Qt::SolidLine);
    m_measurePen.setColor(Qt::darkGreen);
    m_measurePen.setWidthF(1.5);

    m_blockPen.setColor(SPOT_COLOR);
    m_blockPen.setWidthF(1.5);

    m_bSelectedRange = false;

    initFullRangeBlocks();
}

LayerScene::~LayerScene(){

    if (m_targetContour != NULL)
    {
        // delete m_targetContour;
        removeItem(m_targetContour);
        m_targetContour = NULL;
    }

    if (m_marginContour!=NULL)
    {
        // delete m_marginContour;
        removeItem(m_marginContour);
        m_marginContour = NULL;
    }

    if (m_backgroundPixmap!=NULL)
    {
        // delete m_backgroundPixmap;
        removeItem(m_backgroundPixmap);
        m_backgroundPixmap = NULL;
    }

    if (m_range!=NULL)
    {
        removeItem(m_range);
        m_range = NULL;
    }

    if (!m_qBlocks.empty())
    {
        QQueue<target_block>::iterator iter = m_qBlocks.begin();
        for (iter = m_qBlocks.begin(); iter != m_qBlocks.end(); ++iter)
        {
            removeItem(iter->block);
            iter->block = NULL;

            if (iter->text!=NULL)
            {
                removeItem(iter->text);
                iter->text = NULL;
            }

        }

        m_qBlocks.clear();
    }
}

void LayerScene::clearAll()
{
    if (m_backgroundPixmap!=NULL)
    {
        removeItem(m_backgroundPixmap);
        m_backgroundPixmap = NULL;
    }

    removeTarget();
    // m_qCenters.clear();
    if (!m_qBlocks.empty())
    {
        QQueue<target_block>::iterator iter = m_qBlocks.begin();
        for (iter = m_qBlocks.begin(); iter != m_qBlocks.end(); ++iter)
        {
            removeItem(iter->block);
            iter->block = NULL;

            if (iter->text!=NULL)
            {
                removeItem(iter->text);
                iter->text = NULL;
            }
        }

        m_qBlocks.clear();
    }

    if (m_marginContour!=NULL)
    {
        removeItem(m_marginContour);
        m_marginContour = NULL;
    }

    if (m_border!=NULL)
    {
        removeItem(m_border);
        m_border = NULL;
    }

    if (m_range!=NULL)
    {
        removeItem(m_range);
        m_range = NULL;
    }

    m_blocks.clear();
    m_currentBlockId = " ";

    m_eOper = OPER_ERROR;
}

void LayerScene::setBackgroundPixmap(QPixmap pixmap)
{
    m_backgroundPixmap = addPixmap(pixmap);
}

QPainterPath LayerScene::targetPath()
{
    return m_targetContour->path();
}

void LayerScene::getDrawEdges()
{
    if (m_marginContour!=NULL)
    {
        QPainterPath path = m_marginContour->path();

        QPointF pos;
        bool isGetedBlock = false;
        QQueue<target_block>::iterator iter = m_qBlocks.begin();
        for (iter = m_qBlocks.begin(); iter != m_qBlocks.end(); ++iter)
        {
            if (iter->idBorn == m_currentBlockId)
            {
                pos = iter->pos;
                isGetedBlock = true;
                break;
            }
        }

        if (isGetedBlock)
        {
            path.translate(HALF_CLIP_WIDE - pos.x(), HALF_CLIP_HIGH - pos.y());
        }
        else
        {
            // 未找到对应block，取拖动block中心
            pos = m_rangePos;
            path.translate(HALF_CLIP_WIDE - pos.x(), HALF_CLIP_HIGH - pos.y());
        }

        emit setDrawEdges(path);
    }
}

void LayerScene::clearTargetPath()
{
    if (m_targetContour!=NULL)
    {
        removeItem(m_targetContour);
        m_targetContour = NULL;
    }
}

QPixmap LayerScene::getPixmap()
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

void LayerScene::clearMarginAndBlocks()
{
    if (m_marginContour!=NULL)
    {
        removeItem(m_marginContour);
        m_marginContour = NULL;

        clearBlocks();
    }
}

void LayerScene::clearOrder(const QMap<QString,bool>& completed)
{
    for (size_t i = 0;i<m_qBlocks.size();++i)
    {
        if (completed.find(m_qBlocks[i].idBorn)==completed.end())
        {
            removeItem(m_qBlocks[i].text);
            m_qBlocks[i].text = NULL;
        }

    }
}

void LayerScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (drawable() && event->button()==Qt::LeftButton)
    {
        if (m_eOper==OPER_DRAW)
        {
            if (m_backgroundPixmap==NULL)
            {
                return;
            }

            if (m_marginContour!=NULL)
            {
                qDebug()<<"remove margin contour !";
                // delete m_marginContour;
                removeItem(m_marginContour);
                m_marginContour = NULL;

                clearBlocks();
            }

//            qreal scal = m_backgroundPixmap->scale();
//            QPainterPath path(event->scenePos()/scal);

            QPainterPath path(event->scenePos());

            m_marginContour = new QGraphicsPathItem;
            m_marginContour->setPath(path);
            m_marginContour->setVisible(true);
            setTargetDrawable(true);
        }

    }

    if (!drawable() && event->button()==Qt::LeftButton)
    {
        if (m_eOper==OPER_DRAG&&m_range!=NULL)
        {
            if (m_range->contains(event->scenePos()))
            {
                m_range->setPen(m_selectedPen);
                m_bSelectedRange = true;
            }
        }
    }

    QGraphicsScene::mousePressEvent(event);
    return;
}

void LayerScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (targetDrawable()/*&& event->button()==Qt::LeftButton*/)
    {
        if (m_eOper==OPER_DRAW)
        {
            qDebug()<<"Enter move event ! "<<event->scenePos().x();

            QPainterPath path(m_marginContour->path());
            path.lineTo(event->scenePos());
            if (m_marginContour->path().elementCount()>1)
            {
                removeItem(m_marginContour);
            }

            delete m_marginContour;

            m_marginContour = addPath(path, m_targetPen);
            m_marginContour->setVisible(true);
        }
    }

    if (!drawable())
    {
        if (m_eOper==OPER_DRAG&&m_bSelectedRange)
        {
            removeItem(m_range);
            QPointF pos = event->scenePos();
            drawRange(pos,m_selectedPen);
            m_rangePos = QPoint(pos.x(),pos.y());
        }
    }

    QGraphicsScene::mouseMoveEvent(event);
    return;
}

void LayerScene::setOper(Layer_Oper oper,size_t size)
{
    m_eOper = oper;
    if (m_eOper==OPER_SORT)
    {
        m_iOrder = size;
    }
}

void LayerScene::setBlockColor(QString id,QColor color)
{
    QQueue<target_block>::iterator iter = m_qBlocks.begin();
    for (iter = m_qBlocks.begin(); iter != m_qBlocks.end();++iter)
    {
        if (iter->idBorn==id)
        {
            QPen pen;
            pen.setWidth(1.5);
            iter->block->setPen(color);

            break;
        }
    }
}

void LayerScene::setCurrentBlockId(QString id)
{
    m_currentBlockId = id;
}

void LayerScene::clearBlocks()
{
    QAbstractGraphicsShapeItem* item;
    for (size_t i = 0;i<m_qBlocks.size();++i)
    {
        item = m_qBlocks[i].block;
        delete item;
        removeItem(item);
        item = NULL;

        removeItem(m_qBlocks[i].text);
        m_qBlocks[i].text = NULL;
    }
    m_qBlocks.clear();
}

QQueue<Irradiate_Block>& LayerScene::getBlocks()
{
    return m_blocks;
}

void LayerScene::createDrawBlocks()
{
    m_blocks.clear();
    clearBlocks();

    double radius = 277.0;
    int index = 1;
    for (QQueue<QPointF>::iterator iter = m_qCenters.begin();
         iter != m_qCenters.end();++iter)
    {
        if (m_marginContour!=NULL
            && m_marginContour->contains(*iter))
        {
            QPoint pos = QPoint(iter->x(),iter->y());

            // 点按逆时针排列
            QPointF p0(pos.x()+6.0*RESOLUTION,pos.y());
            QPointF p1(pos.x()+3.0*RESOLUTION,pos.y()-3.0*sqrtf(3.0)*RESOLUTION);
            QPointF p2(pos.x()-3.0*RESOLUTION,pos.y()-3.0*sqrtf(3.0)*RESOLUTION);
            QPointF p3(pos.x()-6.0*RESOLUTION,pos.y());
            QPointF p4(pos.x()-3.0*RESOLUTION,pos.y()+3.0*sqrtf(3.0)*RESOLUTION);
            QPointF p5(pos.x()+3.0*RESOLUTION,pos.y()+3.0*sqrtf(3.0)*RESOLUTION);

            QPainterPath path;
            path.moveTo(p0);
            path.lineTo(p1);
            path.lineTo(p2);
            path.lineTo(p3);
            path.lineTo(p4);
            path.lineTo(p5);
            path.lineTo(p0);

            path.setFillRule(Qt::WindingFill);

            target_block block;
            block.idBorn = QString::number(index++);
            block.block = addPath(path, m_blockPen);

            block.text = addText(block.idBorn,this->font());
            block.text->setDefaultTextColor(Qt::red);

            block.pos = QPointF(iter->x(),iter->y());

            // 离中心有一定偏移
            block.text->setPos(pos.x()-3.0*RESOLUTION,pos.y()-6.0*RESOLUTION);

            m_qBlocks<<block;

            Irradiate_Block irr;
            irr.id = block.idBorn;
            irr.leftRight = qAtan((iter->x()-HALF_CLIP_WIDE)/(radius*RESOLUTION))*180.0/PI;
            irr.frontBack = qAtan((iter->y()-HALF_CLIP_HIGH)/(radius*RESOLUTION))*180.0/PI;

            m_blocks<<irr;
        }
    }
}

void LayerScene::refreshBlocks()
{
    QMap<int,target_block> temp;
    QQueue<target_block>::iterator iter = m_qBlocks.begin();
    for (iter = m_qBlocks.begin();iter != m_qBlocks.end();++iter)
    {
        temp.insert((*iter).iOrder,*iter);
    }

    double radius = 277.0;
    m_blocks.clear();
    m_qBlocks.clear();
    QMap<int,target_block>::iterator it = temp.begin();
    for (it = temp.begin();it != temp.end();++it)
    {
        m_qBlocks.push_back(*it);

        Irradiate_Block irr;
        irr.id = it.value().idBorn;
        QPointF pos = it.value().pos;
        irr.leftRight = qAtan((pos.x()-HALF_CLIP_WIDE)/(radius*RESOLUTION))*180.0/PI;
        irr.frontBack = qAtan((pos.y()-HALF_CLIP_HIGH)/(radius*RESOLUTION))*180.0/PI;

        m_blocks<<irr;
    }
}

void LayerScene::drawRange(QPointF pos,QPen pen)
{
    if (m_range!=NULL)
    {
        removeItem(m_range);
        m_range = NULL;
    }

    m_rangePos = QPoint(pos.x(),pos.y());

    // 点按逆时针排列
    QPointF p0(pos.x()+6.0*RESOLUTION,pos.y());
    QPointF p1(pos.x()+3.0*RESOLUTION,pos.y()-3.0*sqrtf(3.0)*RESOLUTION);
    QPointF p2(pos.x()-3.0*RESOLUTION,pos.y()-3.0*sqrtf(3.0)*RESOLUTION);
    QPointF p3(pos.x()-6.0*RESOLUTION,pos.y());
    QPointF p4(pos.x()-3.0*RESOLUTION,pos.y()+3.0*sqrtf(3.0)*RESOLUTION);
    QPointF p5(pos.x()+3.0*RESOLUTION,pos.y()+3.0*sqrtf(3.0)*RESOLUTION);

    QPainterPath path;
    path.moveTo(p0);
    path.lineTo(p1);
    path.lineTo(p2);
    path.lineTo(p3);
    path.lineTo(p4);
    path.lineTo(p5);
    path.lineTo(p0);

    path.setFillRule(Qt::WindingFill);
    m_range = addPath(path, pen);

    m_range->setOpacity(0.2);
}

void LayerScene::drawBorder(QPointF pos)
{
    if (m_border!=NULL)
    {
        removeItem(m_border);
        m_border = NULL;
    }

    // 边界点X正向相对于中心偏移
    int deltaX0 = (qTan(15.0*PI/180.0)-qTan(pos.x()*PI/180.0))*277.0*RESOLUTION;
    // 边界点Y正向相对于中心偏移
    int deltaY0 = (qTan(15.0*PI/180.0)-qTan(pos.y()*PI/180.0))*277.0*RESOLUTION;
    // 边界点负正向相对于中心偏移
    int deltaX1 = (qTan(-15.0*PI/180.0)+qTan(pos.x()*PI/180.0))*277.0*RESOLUTION;
    // 边界点Y负向相对于中心偏移
    int deltaY1 = (qTan(-15.0*PI/180.0)+qTan(pos.y()*PI/180.0))*277.0*RESOLUTION;;

    qDebug()<<"delta is "<<deltaX0 << deltaX1 << deltaY0 << deltaY1;

    // 点按逆时针排列
    QPointF p0(HALF_CLIP_WIDE+deltaX0,HALF_CLIP_HIGH+deltaY0);
    QPointF p1(HALF_CLIP_WIDE+deltaX0,HALF_CLIP_HIGH+deltaY1);
    QPointF p2(HALF_CLIP_WIDE+deltaX1,HALF_CLIP_HIGH+deltaY1);
    QPointF p3(HALF_CLIP_WIDE+deltaX1,HALF_CLIP_HIGH+deltaY0);

    QPainterPath path;
    path.moveTo(p0);
    path.lineTo(p1);
    path.lineTo(p2);
    path.lineTo(p3);
    path.lineTo(p0);

    path.setFillRule(Qt::WindingFill);
    m_border = addPath(path, m_marginPen);

    m_border->setOpacity(0.2);
}

void LayerScene::initFullRangeBlocks()
{
    // 添加中心
    m_qCenters.push_back(QPointF(HALF_CLIP_WIDE,HALF_CLIP_HIGH));
    // 添加中间的一行
    for (int i = 1; i < 4; ++i)
    {
        m_qCenters.push_back(QPointF(HALF_CLIP_WIDE+i*18.0*RESOLUTION,HALF_CLIP_HIGH));
        m_qCenters.push_back(QPointF(HALF_CLIP_WIDE+i*(-18.0)*RESOLUTION,HALF_CLIP_HIGH));
    }
    // 添加中间一列
    for (int i = 1; i < 6; ++i)
    {
        m_qCenters.push_back(QPointF(HALF_CLIP_WIDE,HALF_CLIP_HIGH+6.0*sqrtf(3.0)*i*RESOLUTION));
        m_qCenters.push_back(QPointF(HALF_CLIP_WIDE,HALF_CLIP_HIGH-6.0*sqrtf(3.0)*i*RESOLUTION));
    }

    for (int j = 1; j < 6;++j)
    {
        for (int i = 1; i <4; ++i)
        {
            m_qCenters.push_back(QPointF(HALF_CLIP_WIDE+18.0*i*RESOLUTION,
                                         HALF_CLIP_HIGH+6.0*sqrtf(3.0)*j*RESOLUTION));
            m_qCenters.push_back(QPointF(HALF_CLIP_WIDE-18.0*i*RESOLUTION,
                                         HALF_CLIP_HIGH+6.0*sqrtf(3.0)*j*RESOLUTION));
            m_qCenters.push_back(QPointF(HALF_CLIP_WIDE+18.0*i*RESOLUTION,
                                         HALF_CLIP_HIGH-6.0*sqrtf(3.0)*j*RESOLUTION));
            m_qCenters.push_back(QPointF(HALF_CLIP_WIDE-18.0*i*RESOLUTION,
                                         HALF_CLIP_HIGH-6.0*sqrtf(3.0)*j*RESOLUTION));
        }

        for (int i = 1; i <4; ++i)
        {
            m_qCenters.push_back(QPointF(HALF_CLIP_WIDE+9.0f*RESOLUTION+18.0*(i-1)*RESOLUTION,
                                         HALF_CLIP_HIGH+(3.0+6.0*(j-1))*sqrtf(3.0)*RESOLUTION));
            m_qCenters.push_back(QPointF(HALF_CLIP_WIDE-9.0f*RESOLUTION-18.0*(i-1)*RESOLUTION,
                                         HALF_CLIP_HIGH+(3.0+6.0*(j-1))*sqrtf(3.0)*RESOLUTION));
            m_qCenters.push_back(QPointF(HALF_CLIP_WIDE+9.0f*RESOLUTION+18.0*(i-1)*RESOLUTION,
                                         HALF_CLIP_HIGH-(3.0+6.0*(j-1))*sqrtf(3.0)*RESOLUTION));
            m_qCenters.push_back(QPointF(HALF_CLIP_WIDE-9.0f*RESOLUTION-18.0*(i-1)*RESOLUTION,
                                         HALF_CLIP_HIGH-(3.0+6.0*(j-1))*sqrtf(3.0)*RESOLUTION));
        }

    }
}

void LayerScene::drawRange(QPointF pos)
{
    if (m_range!=NULL)
    {
        removeItem(m_range);
        m_range = NULL;
    }

    clearBlocks();

    m_rangePos = QPoint(pos.x(),pos.y());

    // 点按逆时针排列
    QPointF p0(pos.x()+6.0*RESOLUTION,pos.y());
    QPointF p1(pos.x()+3.0*RESOLUTION,pos.y()-3.0*sqrtf(3.0)*RESOLUTION);
    QPointF p2(pos.x()-3.0*RESOLUTION,pos.y()-3.0*sqrtf(3.0)*RESOLUTION);
    QPointF p3(pos.x()-6.0*RESOLUTION,pos.y());
    QPointF p4(pos.x()-3.0*RESOLUTION,pos.y()+3.0*sqrtf(3.0)*RESOLUTION);
    QPointF p5(pos.x()+3.0*RESOLUTION,pos.y()+3.0*sqrtf(3.0)*RESOLUTION);

    QPainterPath path;
    path.moveTo(p0);
    path.lineTo(p1);
    path.lineTo(p2);
    path.lineTo(p3);
    path.lineTo(p4);
    path.lineTo(p5);
    path.lineTo(p0);

    path.setFillRule(Qt::WindingFill);
    m_range = addPath(path, m_rangePen);

    m_range->setOpacity(0.2);
}

//void LayerScene::zoomOut()    // 图像放大
//{
//    qreal scal;
//    if (m_backgroundPixmap!=NULL)
//    {
//        scal = m_backgroundPixmap->scale();
//    }
//    else
//    {
//        scal = 1.0;
//    }

//    ++scal;
//    if (scal>4.0)
//    {
//        scal = 4.0;
//    }
//}

//void LayerScene::zoomIn()     // 图像缩小
//{
//    qreal scal;
//    if (m_backgroundPixmap!=NULL)
//    {
//        scal = m_backgroundPixmap->scale();
//    }
//    else
//    {
//        scal = 1.0;
//    }

//    --scal;
//    if (scal<1.0)
//    {
//        scal = 1.0;
//    }

//    QPoint center;
//    center = QPoint(this->width()*0.5*scal, this->height()*0.5*scal);
//    setSceneRect(center.x()-this->width()*0.5,center.y()-this->height()*0.5,this->width(),this->height());
//    setScal(scal);
//}

void LayerScene::setMarginVisible(bool flag)
{
    if (m_marginContour!=NULL)
    {
        m_marginContour->setVisible(flag);
    }
}

void LayerScene::setRangeVisible(bool flag)
{
    if (m_range!=NULL)
    {
        m_range->setVisible(flag);
    }
}

void LayerScene::setBorderVisible(bool flag)
{
    if (m_border!=NULL)
    {
        m_border->setVisible(flag);
    }
}

void LayerScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (drawable() && event->button()==Qt::LeftButton)
    {
        if (m_eOper == OPER_DRAW)
        {
            QPainterPath path(m_marginContour->path());
            path.connectPath(path);

            if (m_marginContour->path().elementCount()>1)
            {
                removeItem(m_marginContour);
            }

            // delete m_marginContour;
            m_marginContour = addPath(path,m_targetPen);
            m_marginContour->setVisible(true);
            setTargetDrawable(false);
        }
    }

    if (!drawable() && event->button()==Qt::LeftButton)
    {
        if (m_eOper==OPER_DRAG && m_bSelectedRange)
        {
            removeItem(m_range);
            QPointF pos = event->scenePos();
            drawRange(pos,m_rangePen);
            m_rangePos = QPoint(pos.x(),pos.y());
            m_bSelectedRange = false;
        }
    }

    // 排序
    if (m_eOper==OPER_SORT && event->button()==Qt::LeftButton)
    {
        for (size_t i = 0;i<m_qBlocks.size();++i)
        {
             if (m_qBlocks[i].block->contains(event->scenePos())
                     && m_qBlocks[i].text==NULL)
             {
                 m_qBlocks[i].iOrder = m_iOrder;
                 m_qBlocks[i].text = addText(QString::number(m_iOrder++),this->font());
                 QPointF pos = m_qBlocks[i].pos;
                 m_qBlocks[i].text->setPos(pos.x()-3.0*RESOLUTION,pos.y()-6.0*RESOLUTION);
                 m_qBlocks[i].text->setDefaultTextColor(Qt::red);

                 if (isAllBlocksSeted())
                 {
                     emit emitSetConfirmSetOrderEnable();
                 }

                 break;
             }
        }
    }

    QGraphicsScene::mouseReleaseEvent(event);
    return;
}

bool LayerScene::isAllBlocksSeted()
{
    for (size_t i = 0;i<m_qBlocks.size();++i)
    {
         if (m_qBlocks[i].text==NULL)
         {
             return false;
         }
    }

    return true;
}

QPoint LayerScene::getRangePos()
{
    return m_rangePos;
}

//void LayerScene::setScal(qreal scal)
//{
//    if (m_backgroundPixmap!=NULL)
//    {
//        if (scal!=m_backgroundPixmap->scale())
//        {
//            m_backgroundPixmap->setScale(scal);
//            qDebug()<<"set scal is "<< scal;
//        }
//    }
//    else
//    {
//        qCDebug(LAYERSCENE())<<"set scal m_backgroundPixmap null !"
//                            << TRACE_CMH();
//    }

//    if (m_targetContour!=NULL)
//    {
//        if (scal!=m_targetContour->scale())
//        {
//            m_targetContour->setScale(scal);
//        }
//    }
//    else
//    {
//        qCDebug(LAYERSCENE())<<"set scal target contour null !"
//                            << TRACE_CMH();
//    }



//    if (m_marginContour!=NULL)
//    {
//        if (scal!=m_marginContour->scale())
//        {
//            m_marginContour->setScale(scal);
//        }
//    }
//    else
//    {
//        qCDebug(LAYERSCENE())<<"set scal margin contour null !"
//                            << TRACE_CMH();
//    }
//}

//void LayerScene::wheelEvent(QGraphicsSceneWheelEvent *event)
//{
//    // ctrl+滚轮放大
//    if (!(event->modifiers()&Qt::ControlModifier))
//    {
//        return;
//    }

//    int factor = event->delta();

//    qreal scal;
//    if (m_backgroundPixmap!=NULL)
//    {
//        scal = m_backgroundPixmap->scale();
//    }
//    else
//    {
//        scal = 1.0;
//    }

//    if (factor>0.0)
//    {
//        scal*=1.2;
//    }
//    else
//    {
//        scal*=0.8;
//    }

//    if (scal>4.0)
//    {
//        scal = 4.0;
//    }
//    else if (scal<1.0)
//    {
//        scal = 1.0;
//    }

//    QPoint center;
//    center = QPoint(this->width()*0.5*scal, this->height()*0.5*scal);
//    setSceneRect(center.x()-this->width()*0.5,center.y()-this->height()*0.5,this->width(),this->height());
//    setScal(scal);

//    QGraphicsScene::wheelEvent(event);
//}


void LayerScene::removeTarget()
{
    if (m_targetContour != NULL)
        removeItem(m_targetContour);
#ifdef DEBUG
    qDebug() << "m_targetContour = " << m_targetContour;
    QList<QGraphicsItem*> items = this->items();
    qDebug() << "The count of items:" << items.size();
#endif
}

QGraphicsPathItem* LayerScene::targetContour()
{
    return m_targetContour;
}
