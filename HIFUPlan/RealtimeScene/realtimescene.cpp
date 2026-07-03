#include "realtimescene.h"
#include "drawscene.h"
#include <QSettings>
#include "math.h"

// double RESOLUTION = 2.07692;                           // unit: pixel/mm
double RESOLUTION = 1.95276;
double ZERO_DEVIATION = 2.2*RESOLUTION;
// 换算成140mm应该是多少个像素
double FOCAL_PLANE_140MM = 140 * RESOLUTION+ZERO_DEVIATION;   // unit: pixel/mm

//#endif

// double X_OFFSET = 274;                                    // unit: pixel
// double Y_OFFSET = 294;                                    // unit: pixel

// 对应开立超声S_VIDEO数据线
double X_OFFSET = 308;                                    // unit: pixel
double Y_OFFSET = 342;                                    // unit: pixel

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
//    QSettings *settings = new QSettings(SETTINGS_PATH, QSettings::IniFormat);
//    m_bEleRec = (bool)settings->value("Shape/rectange").toString().toUInt();
    RESOLUTION = settings->value("UltrasoundImage/RESOLUTION").toString().toDouble();
    X_OFFSET = settings->value("UltrasoundImage/X_OFFSET").toString().toDouble();
    Y_OFFSET = settings->value("UltrasoundImage/Y_OFFSET").toString().toDouble();

    qCDebug(REALTIMESCENE()) << REALTIMESCENE().categoryName()
                         << " Xoffset is "<<X_OFFSET
                         << " Yoffset is "<<Y_OFFSET
                         << TRACE_CMH();

    float delta = settings->value("DrawScene/Focus_Delta").toFloat();
    qCDebug(REALTIMESCENE()) << REALTIMESCENE().categoryName()
                         << "focus delta is "<<delta
                         << TRACE_CMH();

    m_dFocusDeep = delta*RESOLUTION+140 * RESOLUTION;

    delete settings;
}

RealtimeScene::~RealtimeScene()
{
//    qDebug()<<"rts销毁";
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
    //qreal rocNew = 150;
    qreal apertureNew = 180;
    theta = 3.1415926/2 - asin(apertureNew*0.5/rocNew);
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
    m_beam->setOpacity(0.6);

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

void RealtimeScene::refresh_report(QImage image)
{
    qDebug()<<"进入refresh";
}

void RealtimeScene::refresh(QImage image){
    // Display the image in the scene
//    qDebug()<<"进入realtimescene";
//    image.save("D:\\dicom\\image2222.jpg");
    //QImage image1("D:\\image332.jpg");
    //QImage temp = image;
    // 调节窗宽，窗位
    // adjustWindow(temp);
//    if (m_bIsInitContrast)
//    {
//        if (m_pAdjContrast!=NULL)
//        {
//            m_pAdjContrast->setTable(temp);
//            m_bIsInitContrast = false;
//        }
//    }

    //adjContrast(temp);

    if (m_pixmap == NULL) {
        m_pixmap = addPixmap(QPixmap::fromImage(image));
        m_pixmap->setTransformationMode(Qt::SmoothTransformation);
        }
    else m_pixmap->setPixmap(QPixmap::fromImage(image));

//    image.save("D:\\dicom\\image332.jpg");

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
        QString filename = foldpath()+"/"+frameFileName();
        image.save(filename, 0, 180);
        emit sendimage(image,angle());
//        image.save(filename);
//        bool ok = image.save(filename);
//        qDebug()<<"chaosheng图像保存路径："<<ok+" baocunshifouchgenggong"<<filename;
        setFrameAcquired(false);
        if (m_frameIndex == 1) emit seriesAcquireCompleted();
    }
}
