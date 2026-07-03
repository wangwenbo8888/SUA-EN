#ifndef DRAWSCENE_H
#define DRAWSCENE_H

#ifdef WHEEL
#include <QGraphicsSceneWheelEvent>
#endif

#include <QPainterPath>
#include <QGraphicsPathItem>
#include <QGraphicsPixmapItem>
#include <QPen>
#include <QPolygonF>
#include <QPointF>
#include <QQueue>
#include "qevent.h"
#include <QPainter>
#include <vector>
#include <QLoggingCategory>
#include <QHash>
#include "constant.h"
#include "drawscene_global.h"
#include "measureline.h"
#include "qgraphicsscene.h"

#include "spline.h"

// 超声图像不同分辨率定义的高度于分辨率
#define IMAGE_VERSION_1 1
#define IMAGE_VERSION_2 0
#define IMAGE_VERSION_3 0

// 图像 576*576
#if IMAGE_VERSION_1
//#define PI 3.1415926



#endif



// 内部点颜色
#define SPOT_COLOR QColor(65,47,31,200)
// 边界点颜色
#define EDGE_COLOR QColor(Qt::red)

#define AUTO_COLOR QColor(255,227,132,200)

Q_DECLARE_LOGGING_CATEGORY(DARWSCENE)

// 针对scene的操作
enum SCENE_OPER
{
   OPER_DRAW_TARGET = 0,
   OPER_MEASURE,
   OPER_DELETE,
   DRAG
};

enum SCENE_TYPE
{
    SCENE_TARGET = 0,
    SCENE_SEQUENCE,
    SCENE_LAYER
};

enum SPOT_LOCATION
{
    LOCATION_INNER = 0,
    LOCATION_EDGE,
    LOCATION_OUTER
};

enum DENSITY_TYPE
{
    DENSITY_HIGH = 0,
    DENSITY_MIDDLE,
    DENSITY_LOW,

    DENSITY_UNKNOWN,
};

enum SPOT_STATE
{
    STATE_SELECTED = 0,
    STATE_DELETED
};

// 辐照点分布情况
struct SPOT_DISTRIBUTE
{
   SPOT_DISTRIBUTE()
   {
       init();
   }

   void init()
   {
       isCenter = false;        // 是否需要中心点
       isLayerUp = false;       // 是否需要上层点
       isLayerMiddle = false;   // 是否需要中层点
       isLayerDown = false;     // 是否需要下层点
       isCircleOne = false;     // 是否需要第一圈点
       isCircleTwo = false;     // 是否需要第二圈点
       isCircleThree = false;   // 是否需要第三圈点
       isCircleFour = false;    // 是否需要第四圈点
       isCircleFive = false;    // 是否需要第五圈点
   }

   void setAllSpots(bool b)
   {
       isCenter       = b;   // 是否需要中心点
       isLayerUp      = b;   // 是否需要上层点
       isLayerMiddle  = b;   // 是否需要中层点
       isLayerDown    = b;   // 是否需要下层点
       isCircleOne  = b;     // 是否需要第一圈点
       isCircleTwo = b;      // 是否需要第二圈点
       isCircleThree  = b;   // 是否需要第三圈点
       isCircleFour = b;     // 是否需要第四圈点
       isCircleFive = b;     // 是否需要第五圈点
   }

   void setCenter(bool b)
   {
       isCenter = b;
   }

   void setLayerUp(bool b)
   {
       isLayerUp = b;
   }

   void setLayerMiddle(bool b)
   {
       isLayerMiddle = b;
   }

   void setLayerDown(bool b)
   {
       isLayerDown = b;
   }

   void setCircleOne(bool b)
   {
       isCircleOne = b;
   }

   void setCircleTwo(bool b)
   {
       isCircleTwo = b;
   }

   void setCircleThree(bool b)
   {
       isCircleThree = b;
   }

   void setCircleFour(bool b)
   {
       isCircleFour = b;
   }

   void setCircleFive(bool b)
   {
       isCircleFive = b;
   }

   void setDensity(DENSITY_TYPE density)
   {
       eDensity = density;
   }

   bool isCenter;        // 是否需要中心点
   bool isLayerUp;       // 是否需要上层点
   bool isLayerMiddle;   // 是否需要中层点
   bool isLayerDown;     // 是否需要下层点
   bool isCircleOne;     // 是否需要内圈点
   bool isCircleTwo;     // 是否需要中圈点
   bool isCircleThree;   // 是否需要外圈点
   bool isCircleFour;    // 是否需要第四圈点
   bool isCircleFive;    // 是否需要第五圈点
   DENSITY_TYPE eDensity;
};

struct viewer2D_info
{
    qreal scal;      // 图像放大倍数
    QRectF rect;    // 图像视窗
};

struct target_spot
{
    QString idBorn;                    // 辐照点诞生id
    Circle  circle;                    // 圈
    QAbstractGraphicsShapeItem* spot;  // 辐照点图标
    SPOT_STATE state;                  // 辐照点状态
    QPointF pos;                       // 辐照点二维位置
};


class DRAWSCENESHARED_EXPORT DrawScene : public QGraphicsScene
{
    Q_OBJECT

public:

    // resolution = 270/130 每mm时多少个像素
//    double RESOLUTION;                           // unit: pixel/mm
//    double ZERO_DEVIATION = 2.2*RESOLUTION;
//    // 换算成140mm应该是多少个像素
//    double FOCAL_PLANE_140MM = 140 * RESOLUTION+ZERO_DEVIATION;   // unit: pixel/mm
//    double X_OFFSET = 274;                                    // unit: pixel
//    double Y_OFFSET = 294;                                    // unit: pixel
//    // const double RESOLUTION = 1.3;                               // unit: pixel/mm

//    // const double FOCAL_PLANE_140MM = 130 * RESOLUTION;           // unit: pixel

//    double SPOT_WIDTH_DENSITY_HIGH  = 1.0*RESOLUTION;         // 辐照点高密度，辐照点直径1.0mm
//    double SPOT_WIDTH_DENSITY_MIDDLE = 1.4*RESOLUTION;        // 辐照点高密度，辐照点直径1.4mm
//    double SPOT_WIDTH_DENSITY_LOW  = 1.8*RESOLUTION;          // 辐照点高密度，辐照点直径1.8mm

//    double SPOT_WIDTH  = 1.8 * RESOLUTION;                    // unit: pixel
//    double SPOT_LENGTH = 10 * RESOLUTION;                     // unit: pixel
//    //const double BEAM_LENGTH = 40 * RESOLUTION;                   // unit: pixel

//    double CENTER_LENGTH = 20*RESOLUTION;
//    double BEAM_LENGTH = 10*RESOLUTION;

//    double SPOT_END_LENGTH = 5*RESOLUTION;
//    double BEAM_END_LENGTH = 40*RESOLUTION;

//    double CELL4MM_LATERAL_DISTANCE = 2 * RESOLUTION;         // unit: pixel
//    double CELL8MM_LATERAL_DISTANCE = 4 * RESOLUTION;         // unit: pixel
//    double CELL12MM_LATERAL_DISTANCE = 6 * RESOLUTION;        // unit: pixel

//    double SPOT_DISTANCE_HIGH = 1.2*RESOLUTION;
//    double SPOT_DISTANCE_MIDDLE = 1.6*RESOLUTION;
//    double SPOT_DISTANCE_LOW = 2.0*RESOLUTION;


    DrawScene(float angle, SCENE_OPER = OPER_DRAW_TARGET, QObject *parent=0);
    DrawScene(DrawScene*& pObj,const QPixmap& pix);
    ~DrawScene();
    void setSceneType(SCENE_TYPE type);
    void setBackgroundPixmap(QPixmap);
    void drawSpots(SPOT_DISTRIBUTE distribute);

    // rotate 整个block旋转角度
    void drawSpotsNew(int rotate,SPOT_DISTRIBUTE distribute);

    void setDensity(DENSITY_TYPE type);

    inline bool drawable() const;
    inline void setDrawable(const bool status);
    inline bool targetDrawable() const;
    inline void setTargetDrawable(const bool status);
    inline bool targetVisible() const;
    inline void setTargetVisible(const bool status);
    inline bool marginVisible() const;
    inline void setMarginVisible(const bool status);
    inline bool transducerVisible() const;
    inline bool measureable() const;
    inline void setMeasureDrawable(const bool status);
    void setTransducerVisible(const bool status);
    void setBeamVisible(const bool status);
    void setContoursVisiable(const bool status);
    void setSpotsVisible(const bool status);
    void setScal(qreal scal);
    void setViewer2D(viewer2D_info info);
    bool equalViewer2D(viewer2D_info info);
    void addExamplePoint(QString idBorn,QPointF p, int offset);    // 增加辐照层面中的示例点
    void clearExamplePoint();
    bool selectedExectue(QString id,QColor pen);
    bool addOutline();
    bool getIntersectPath(DrawScene* scene,QPainterPath& path);
    void sysDrawEdges();

    QPixmap getPixmap();

    bool isUsefulScene();    // scene中是否包含勾画点

    QPainterPath targetPath();

    void setTargetPath(QPainterPath path);
    void clearTargetPath();

    QGraphicsPathItem* targetContour();

    float getAngle();
    void setOperator(SCENE_OPER oper);

    // 删除被删除的辐照点
    void removeDeletedSpots();

    void hideElement();    // 隐藏所有辐照单元
    void showElement();    // 显示所有辐照单元

    void hideSpot();    // 隐藏计划辐照点
    void showSpot();    // 显示计划辐照点

    void showDraw();    // 显示勾画轮廓及自动合成轮廓
    void hideDraw();    // 隐藏勾画轮廓及自动合成轮廓

    void smartSelect();     // 智能选择辐照单元
    void resumeDelected();  // 恢复被删除的辐照单元

    void selectInvert();    // 反选

    void zoomOut();    // 图像放大
    void zoomIn();     // 图像缩小

    SCENE_OPER getOperator();

    void removeTarget();
    void removeSpots();
    void resetTarget();

    QQueue<QString>& getIdBorn();
    QQueue<double>& getFocusX();
    QQueue<double>& getFocusY();
    QQueue<double>& getFocusZ();
    QQueue<Circle>& getCircles();
    std::vector<std::vector<QPointF> > getPoints();//读取m_points
    void setPoints(std::vector<std::vector<QPointF> > points);//写入m_points

    void setCenterAndOutline(std::vector<std::vector<QPointF> > points,QPointF center);

signals :
    void changeView2D(viewer2D_info);
    bool spotSelected(QString id,QColor pen);

    void getDrawEdges();

public slots:
    void setDrawEdges(QPainterPath path);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void wheelEvent(QGraphicsSceneWheelEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void removeMeasureLine(MeasureLine *measureline);
private:




    float m_angle;
    bool m_drawable, m_targetDrawable,m_measureDrawable;
    QGraphicsPathItem* m_targetContour;    // targetContour保存的是相对
    QGraphicsPathItem* m_autoDraw;         // 根据手动勾画的边界生成的两个边界交集

    // 当前图像的位置
    QGraphicsPathItem *m_tempContour,
    *m_transducerContour, *m_beam, *m_measureContour, *m_marker;

    // 大肌瘤勾画时勾画的肌瘤边缘，只对自然焦点层有效
    QGraphicsPathItem* m_sysEdges;

    // 肌瘤轮廓
    QGraphicsPathItem* m_outline;
    QGraphicsPathItem *m_bakContour;
    QGraphicsPathItem* m_displayContour;    // 勾画时显示用
    QGraphicsPixmapItem *m_backgroundPixmap;
    std::vector<QPointF> m_points;                 //配准后轮廓的点
    std::vector<double> m_vEllipseParas;           // 肌瘤模拟椭圆参数
    QPointF m_center;

    QPen m_autoPen;
    QPen m_targetPen,m_measurePen;
    QPen m_spotPen, m_transducerPen, m_beamPen;
    QPen m_edgePen;    // 边界点颜色
    QPen m_sysPen;     // 同步的肌瘤边界颜色
    QPen m_outlinePen;
    QPen m_bakPen;
    QPen m_transparent;  // 有些路径隐藏

    QPen m_selected; //选择删除线段，线段颜色发生变化
    QPen m_changedPen;//修改线段时，线段颜色和格式发生变化
    QPen m_extremPen; //选段两端点的样式
    QPen m_selectOutLinePen;   //选择轮廓后，颜色发生变化
    QPainterPath *m_path;

    // QList<QAbstractGraphicsShapeItem*> m_spots;
    int m_selectIndex;
    int m_delete;

    void drawTransducer(QPointF centroid, qreal roc, qreal aperture);
    void drawBeam();
    void drawSpots();
    void addSpot(QPointF &);
    void smartAddSpot(QPointF& focus,const Circle& circle);

    float angle(QPointF center, QPointF point);

    // 肌瘤轮廓移动
    QPainterPath moveOutline(QPointF center);
    // 肌瘤轮廓变形
    QPainterPath deformOutline(QPointF vec);

    SPOT_LOCATION getLocation(const target_spot& spot);

    qreal rotationAxis();
    double spotX(QPointF&);
    double spotY(QPointF&);
    double spotZ(QPointF&);

    bool m_isCtrlDown;
    // 鼠标左键按下位置
    QPointF m_leftDownPos;

    double getPosX(int rotate,Circle circle,double delta);

    SCENE_OPER m_operator;      // 针对scene的操作
    // QQueue<QPointF> m_points;   // 辐照点坐标
    QQueue<target_spot> m_spots;
    QQueue<double> m_x, m_y, m_z ;
    QQueue<QString> m_idBorn;
    QQueue<Circle> m_circles;
    bool m_bEleRec;    // 根据配置文件判断辐照单元显示矩形还是椭圆
    SCENE_TYPE _sceneType;


    DENSITY_TYPE m_eDensity;
    double m_dSpotWide;
    double m_dSpotDistance;
    double m_dFocusDeep;

    QList<MeasureLine*> m_measureSeries;//保存measureline
    MeasureLine* m_measure;
    MeasureLine* m_measure_changed;
    bool _bChangeable;//已有线段发生改变的标识
    bool _bMeasureable;//画线的标识
    bool _bPressed;//按键是否被按下
    bool _bExist; //线段是否处于选择状态,即线段颜色是否是红色
    bool _bManualDrag;//配准的轮廓是否处于被选择状态，即被拖动状态
    QPointF startPos;//保存鼠标第一次按下的键所处的位置
    QPointF m_outlinePos;
    double DistancePtToSegt(QPointF point,MeasureLine* measureLine);//计算点到measureline的距离
    QHash<double,int> dist_and_index ;//保存鼠标位置到画面上所有线段的距离
    QPainterPath *m_change_path;
    QGraphicsPathItem *m_newMeasureContour;
};

inline bool DrawScene::drawable() const { return m_drawable; }
inline void DrawScene::setDrawable(const bool status){m_drawable = status;}
inline bool DrawScene::targetDrawable() const { return m_targetDrawable; }
inline void DrawScene::setTargetDrawable(const bool status) { m_targetDrawable = status; }
inline bool DrawScene::targetVisible() const { return m_targetContour->isVisible(); }
inline void DrawScene::setTargetVisible(const bool status){m_targetContour->setVisible(status);}
inline bool DrawScene::transducerVisible() const { return m_transducerContour->isVisible(); }
inline bool DrawScene::measureable() const { return m_measureDrawable;}
inline void DrawScene::setMeasureDrawable(const bool status){m_measureDrawable = status;}

//inline void DrawScene::setTransducerVisible(const bool status) { m_transducerContour->setVisible(status); }

#endif // DRAWSCENE_H
