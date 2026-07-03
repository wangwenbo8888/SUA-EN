#ifndef SCENE_H
#define SCENE_H

#include <QGraphicsScene>
#include <QGraphicsPathItem>
#include <QPainterPath>
#include <QGraphicsSceneMouseEvent>
#include <QPointF>
#include <QLineF>
#include <QList>

#include "scene_global.h"

class SCENESHARED_EXPORT Scene : public QGraphicsScene
{
    Q_OBJECT
public:
    Scene(QObject* parent = 0);

    bool enableDraw() const;
    void setEnableDraw(bool enableDraw);

    void deleteTarget();
    void generateSpots();
    void deleteSpots();

    int getSpotWidth() const;
    void setSpotWidth(int value);

    int getSpotHeight() const;
    void setSpotHeight(int value);

    void addCentralLine();

//  get the 2D coordinates of spots in the B-mode ultrasound image
    QList<QPointF> spot2DCoordinates() const;

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent* event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent* event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event);

signals:
    void spotsReady();

private:
    QPointF m_targetStartPoint;
    QGraphicsPathItem* m_currTarget;
    QList<QGraphicsPathItem*> m_target;
    QList<QGraphicsRectItem*> m_spots;
    QList<QPointF> m_spot2DCoordinates;
    QLineF* m_centralLine;
    bool m_enableDraw;

    int m_spotWidth, m_spotHeight;

    void startDrawTarget(QGraphicsSceneMouseEvent* event);
    void drawingTarget(QGraphicsSceneMouseEvent* event);
    void finishDrawTarget();
};

#endif // SCENE_H
