#include "realtimescene.h"
#include "drawscene.h"
#include <QSettings>
#include "math.h"

Q_LOGGING_CATEGORY(REALTIMESCENE,"REALTIMESCENE")

RealtimeScene::RealtimeScene(QObject *parent)
: QGraphicsScene(parent)
, m_scal(1.0)
, m_bIsInitContrast(true)
{
    m_pixmap = NULL;
    m_beam = NULL;
    m_focus = NULL;
    m_range = NULL;

    m_bAdjust = false;

    pAdjust = new AdjustWindow;
    m_pAdjContrast = new AdjContrast;

    m_frameAcquired = false;
    m_seriesAcquired = false;
    m_frameIndex = 1;

    m_beamPen.setColor("#63B8FF");
    m_beamPen.setWidthF(2);

    m_focusPen.setColor("#63B8FF");
    m_focusPen.setWidthF(5);

    m_rangePen.setColor("#63B8FF");
    m_rangePen.setWidthF(2);

    QSettings *settings = new QSettings(SETTINGS_PATH, QSettings::IniFormat);

    float delta = settings->value("DrawScene/Focus_Delta").toFloat();
    qCDebug(REALTIMESCENE()) << REALTIMESCENE().categoryName()
                         << "focus delta is "<<delta
                         << TRACE_CMH();

    m_dFocusDeep = delta*RESOLUTION+140 * RESOLUTION;

    delete settings;
}

RealtimeScene::~RealtimeScene()
{
    if (pAdjust!=NULL)
    {
        delete pAdjust;
        pAdjust = NULL;
    }

    if (m_pAdjContrast!=NULL)
    {
        delete m_pAdjContrast;
        m_pAdjContrast = NULL;
    }
}

float RealtimeScene::angle() const {
    return m_frameIndex <= ANGLE_COUNT? ANGLE_STEP * (m_frameIndex-(ANGLE_COUNT+1)/2) : 360;
}

void RealtimeScene::setScal(qreal scal)
{
    if (m_pixmap!=NULL)
    {
        m_pixmap->setScale(scal);
    }

    if (m_beam!=NULL)
    {
        m_beam->setScale(scal);
    }

    if (m_focus!=NULL)
    {
        m_focus->setScale(scal);
    }   // 自然焦点

    if (m_range!=NULL)
    {
        m_range->setScale(scal);
    }
}

void RealtimeScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->buttons()==Qt::MiddleButton)
    {
        // 滚轮按下
        m_pDelta = event->scenePos();
        m_bAdjust = true;
    }
    else
    {
        return;
    }
}

void RealtimeScene::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button()==Qt::LeftButton)
    {
        if (pAdjust!=NULL)
        {
            pAdjust->resumeWindow();
        }
    }
}

void RealtimeScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (m_bAdjust)
    {
        // 滚轮放开
        m_pDelta = event->scenePos()-m_pDelta;
        m_bAdjust = false;

        int deltaWW = m_pDelta.x()*20/576;
        int deltaWC = m_pDelta.y()*20/576;
        if (pAdjust!=NULL)
        {
            pAdjust->resetWindow(deltaWW,deltaWC);
        }

    }
    else
    {
        return;
    }
}

void RealtimeScene::zoomOut()
{
    ++m_scal;
    if (m_scal>4.0)
    {
        m_scal = 4.0;
    }

    QPoint center;
    if (m_scal>1.1)
    {
        center = QPoint(X_OFFSET*m_scal, m_dFocusDeep*m_scal);
    }
    else
    {
        center = QPoint(this->width()*0.5*m_scal, this->height()*0.5*m_scal);
    }

    setSceneRect(center.x()-this->width()*0.5,center.y()-this->height()*0.5,this->width(),this->height());

    setScal(m_scal);
}

void RealtimeScene::setBeamVisible(const bool status)
{
    if (m_beam==NULL)
    {
        drawBeam();
    }
    m_beam->setVisible(status);
}

void RealtimeScene::showFocus()  // 显示自然焦点
{
    if (m_focus==NULL)
    {
        drawFocus();
    }
    m_focus->setVisible(true);
}

void RealtimeScene::showRange()  // 显示辐照区域
{
    if (m_range==NULL)
    {
        drawRange();
    }
    m_range->setVisible(true);
}

void RealtimeScene::hideRange()  // 隐藏辐照区域
{
    if (m_range==NULL)
    {
        return;
    }
    m_range->setVisible(false);
}

void RealtimeScene::hideFocus()  // 隐藏自然焦点
{
    if (m_focus==NULL)
    {
        return;
    }
    m_focus->setVisible(false);
}

void RealtimeScene::drawRange()
{
    QPointF leftUp(X_OFFSET-6.0*RESOLUTION,m_dFocusDeep-15.0*RESOLUTION);
    QPointF leftDown(X_OFFSET-6.0*RESOLUTION,m_dFocusDeep+15.0*RESOLUTION);
    QPointF rightUp(X_OFFSET+6.0*RESOLUTION,m_dFocusDeep-15.0*RESOLUTION);
    QPointF rightDown(X_OFFSET+6.0*RESOLUTION,m_dFocusDeep+15.0*RESOLUTION);

    QPointF leftSecond(X_OFFSET-6.0*RESOLUTION,m_dFocusDeep-5.0*RESOLUTION);
    QPointF leftThird(X_OFFSET-6.0*RESOLUTION,m_dFocusDeep+5.0*RESOLUTION);
    QPointF rightSecond(X_OFFSET+6.0*RESOLUTION,m_dFocusDeep-5.0*RESOLUTION);
    QPointF rightThird(X_OFFSET+6.0*RESOLUTION,m_dFocusDeep+5.0*RESOLUTION);

    QPainterPath path;
    path.moveTo(leftUp);
    path.lineTo(rightUp);
    path.lineTo(rightDown);
    path.lineTo(leftDown);
    path.lineTo(leftUp);

    path.moveTo(leftSecond);
    path.lineTo(rightSecond);

    path.moveTo(leftThird);
    path.lineTo(rightThird);

    path.setFillRule(Qt::WindingFill);
    m_range = addPath(path, m_rangePen);
    if (m_pixmap!=NULL)
    {
        qDebug()<<"back ground not null !";
        m_range->setScale(m_scal);
    }
    m_range->setOpacity(0.2);
}

void RealtimeScene::drawFocus()
{
    QPointF rowPoint(X_OFFSET-2.5*RESOLUTION,m_dFocusDeep);
    QPointF columnPoint(X_OFFSET,m_dFocusDeep-2.5*RESOLUTION);
    QPainterPath path;
    path.moveTo(rowPoint);
    path.lineTo(X_OFFSET+2.5*RESOLUTION,m_dFocusDeep);

    path.moveTo(columnPoint);
    path.lineTo(X_OFFSET,m_dFocusDeep+2.5*RESOLUTION);

    path.setFillRule(Qt::WindingFill);
    m_focus = addPath(path, m_focusPen);
    if (m_pixmap!=NULL)
    {
        qDebug()<<"back ground not null !";
        m_focus->setScale(m_scal);
    }
    m_focus->setOpacity(0.2);
}

void RealtimeScene::drawBeam()
{
    qreal roc = 4, aperture = 4;
    QPointF spot(X_OFFSET, m_dFocusDeep);
    //qreal theta = asin(aperture/2/roc);
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
    if (m_pixmap!=NULL)
    {
        qDebug()<<"back ground not null !";
        m_beam->setScale(m_scal);
    }
    //m_beam->setBrush(QBrush("#63B8FF"));
    m_beam->setOpacity(0.2);

    // qDebug()<<"draw beam success !";
}

void RealtimeScene::zoomIn()
{
    --m_scal;
    if (m_scal<1.0)
    {
        m_scal = 1.0;
    }

    QPoint center;
    if (m_scal>1.1)
    {
        center = QPoint(X_OFFSET*m_scal, m_dFocusDeep*m_scal);
    }
    else
    {
        center = QPoint(this->width()*0.5*m_scal, this->height()*0.5*m_scal);
    }

    setSceneRect(center.x()-this->width()*0.5,center.y()-this->height()*0.5,this->width(),this->height());
    setScal(m_scal);
}

void RealtimeScene::resumeImage()
{
    m_scal = 1.0;
    setSceneRect(0,0,this->width(),this->height());
    setScal(m_scal);
}

float RealtimeScene::angle(int frameIndex) const {
    return frameIndex <= ANGLE_COUNT? ANGLE_STEP * (frameIndex-(ANGLE_COUNT+1)/2) : 360;
}

QString RealtimeScene::frameFileName(int frameIndex) const {
    return QString("Target_Image_Sequence_%1_degree.jpg").arg(angle(frameIndex));
}

QPixmap RealtimeScene::getPixmap()
{
    if (m_pixmap!=NULL)
    {
        return m_pixmap->pixmap();
    }
    else
    {
        return QPixmap();
    }
}

void RealtimeScene::adjContrast(QImage& image)
{
    if (m_pAdjContrast==NULL)
    {
        return;
    }

    QImage temp = image;
    switch(image.format())
    {
    case QImage::Format_RGB32:
         int srcWidth, srcHeight;
         srcWidth = temp.width();
         srcHeight = temp.height();

         uchar* rTable = m_pAdjContrast->getTableR();
         uchar* gTable = m_pAdjContrast->getTableG();
         uchar* bTable = m_pAdjContrast->getTableB();

         // 调整亮度
         for(int row = 0; row < srcHeight; row++)
         {
             uchar* ptr = temp.bits()+4*srcWidth*row;
             uchar* qtr = image.bits()+4*srcWidth*row;
             for(int col = 0; col < srcWidth; col++)
             {

                //为目标输出图片赋值,r,b颜色值对换
                qtr[2] = bTable[ptr[0]];
                qtr[1] = gTable[ptr[1]];
                qtr[0] = rTable[ptr[2]];

                ptr += 4;
                qtr += 4;
             }
        }

        break;
    }
}

void RealtimeScene::setContrast(float contrast)
{
    if (m_pAdjContrast!=NULL)
    {
        m_pAdjContrast->setContrast(contrast);
        m_bIsInitContrast = true;
    }
}

void RealtimeScene::adjustWindow(QImage& image)
{
    int height = image.height();
    int width = image.width();
    switch(image.format())
    {
    case QImage::Format_Indexed8:
        unsigned char* pTable = pAdjust->getTable();
        uchar* value = image.bits();
        for (int i = 0; i < height;++i)
        {
            for (int j = 0; j < width;++j)
            {
                value[i*width+j] = pTable[value[i*width+j]];
            }
        }
        break;
    }
}

void RealtimeScene::refresh(QImage image){
    // Display the image in the scene
    QImage temp = image;
    // 调节窗宽，窗位
    // adjustWindow(temp);
    if (m_bIsInitContrast)
    {
        if (m_pAdjContrast!=NULL)
        {
            m_pAdjContrast->setTable(temp);
            m_bIsInitContrast = false;
        }
    }

    adjContrast(temp);

    if (m_pixmap == NULL) {
        m_pixmap = addPixmap(QPixmap::fromImage(temp));
        m_pixmap->setTransformationMode(Qt::SmoothTransformation);
        }
    else m_pixmap->setPixmap(QPixmap::fromImage(temp));

    // 发布实时图像刷新消息，同步直方图
    emit synImage();

    if (seriesAcquired()) {
        setFrameFileName(QString("Target_Image_Sequence_%1_degree.jpg").arg(angle()));
        emit synPlanImage(image);
        if (m_frameIndex == ANGLE_COUNT)
            m_frameIndex = 1;
        else m_frameIndex++;
        setSeriesAcquired(false);
        setFrameAcquired(true);
    }

    if (frameAcquired()) {
        image.save(foldpath()+"/"+frameFileName(), 0, 180);
        setFrameAcquired(false);
        if (m_frameIndex == 1) emit seriesAcquireCompleted();
    }
}

