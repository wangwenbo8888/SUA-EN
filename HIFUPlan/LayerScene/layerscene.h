#ifndef LAYERSCENE_H_
#define LAYERSCENE_H_

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>

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

#include "constant.h"
#include "layerscene_global.h"

#include <queue>
#include "irradiateblock.h"

enum Layer_Oper
{
  OPER_DRAW = 0,
  OPER_DRAG,
  OPER_SORT,

  OPER_ERROR

};

struct target_block
{
    target_block()
    {
        idBorn = "";                                   // 辐照块诞生id
        iOrder = -1;                                   // 模块执行顺序
        block=NULL;                                    // 辐照块图标
        text=NULL;                                     // 辐照次序
        pos=QPointF(-1.0,-1.0);                        // 辐照块图像坐标系下二维位置
    }

    QString idBorn;                     // 辐照块诞生id
    int iOrder;                         // 执行顺序
    QAbstractGraphicsShapeItem* block;  // 辐照块图标
    QGraphicsTextItem* text;            // 辐照次序
    QPointF pos;                        // 辐照块图像坐标系下二维位置

};

Q_DECLARE_LOGGING_CATEGORY(LAYERSCENE)

class LAYERSCENESHARED_EXPORT LayerScene : public QGraphicsScene
{
    Q_OBJECT

public:
    LayerScene(QObject *parent=0);
    ~LayerScene();

    void clearAll();

    void setBackgroundPixmap(QPixmap);

    inline bool drawable() const;
    inline void setDrawable(const bool status);
    inline bool targetDrawable() const;
    inline void setTargetDrawable(const bool status);
    inline bool targetVisible() const;
    inline void setTargetVisible(const bool status);
    inline bool marginVisible() const;

//    void setScal(qreal scal);

    QPixmap getPixmap();

    QPainterPath targetPath();

    void setTargetPath(QPainterPath path);
    void clearTargetPath();

    // 清除勾画的边界和排列的模块
    void clearMarginAndBlocks();

    // 清除图标中的执行顺序数字
    void clearOrder(const QMap<QString,bool>& completed);

    size_t getBlocksSize()
    {
        return m_blocks.size();
    }

    QGraphicsPathItem* targetContour();

//    void zoomOut();    // 图像放大
//    void zoomIn();     // 图像缩小

    void setOper(Layer_Oper oper,size_t size = 1);

    QPoint getRangePos();

    void setMarginVisible(bool flag);
    void setRangeVisible(bool flag);
    void setBorderVisible(bool flag);

    void refreshBlocks();

    void removeTarget();

    void drawRange(QPointF pos);

    // pos 当前治疗盘前后左右轴所在角度
    void drawBorder(QPointF pos);

    void createDrawBlocks();

    QQueue<Irradiate_Block>& getBlocks();

signals :
    void setDrawEdges(QPainterPath path);

    void emitSetConfirmSetOrderEnable();

public slots:

    void getDrawEdges();

    void setBlockColor(QString id,QColor color);

    void setCurrentBlockId(QString id);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
//    void wheelEvent(QGraphicsSceneWheelEvent *event);

private:

    void drawRange(QPointF pos,QPen pen);

    // 初始化大约10cm*10cm的辐照范围
    void initFullRangeBlocks();

    void clearBlocks();

    bool isAllBlocksSeted();

    bool m_drawable, m_targetDrawable;
    QGraphicsPathItem* m_targetContour;    // targetContour保存的是相对

    // 当前图像的位置
    QGraphicsPathItem *m_marginContour;

    // 六边形辐照范围
    QGraphicsPathItem* m_range;
    bool m_bSelectedRange;

    // 可拖动的边界范围
    QGraphicsPathItem* m_border;

    QGraphicsPixmapItem *m_backgroundPixmap;

    QPen m_targetPen, m_marginPen, m_measurePen;
    QPen m_rangePen;
    QPen m_selectedPen;
    QPen m_blockPen;

    QPoint m_rangePos;  // 辐照块中心位置
    Layer_Oper m_eOper;


    QQueue<QPointF> m_qCenters;
    QQueue<target_block> m_qBlocks;
    // 三维空间blocks;
    QQueue<Irradiate_Block> m_blocks;

    QString m_currentBlockId;

    int m_iOrder;

};

inline bool LayerScene::drawable() const { return m_drawable; }
inline void LayerScene::setDrawable(const bool status) { m_drawable = status; }
inline bool LayerScene::targetDrawable() const { return m_targetDrawable; }
inline void LayerScene::setTargetDrawable(const bool status) { m_targetDrawable = status; }
inline bool LayerScene::targetVisible() const { return m_targetContour->isVisible(); }
inline void LayerScene::setTargetVisible(const bool status)
{
    if (m_targetContour==NULL)
    {
        return;
    }
    else
    {
        m_targetContour->setVisible(status);
    }
}

#endif // LAYERSCENE_H
