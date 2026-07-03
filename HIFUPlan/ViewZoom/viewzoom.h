#ifndef VIEWZOOM_H
#define VIEWZOOM_H

#include <QObject>
#include <QGraphicsView>

#include "viewzoom_global.h"

class VIEWZOOMSHARED_EXPORT ViewZoom : public QObject
{
    Q_OBJECT

public:
    ViewZoom(QGraphicsView* view);
    void zoom(qreal factor);
    void setModifiers(Qt::KeyboardModifiers modifiers);
    void setZoomFactorBase(qreal base);

signals:
    void zoomed();

private:
    QGraphicsView* m_view;
    Qt::KeyboardModifiers m_modifiers;
    qreal m_zoomFactorBase;
    QPointF m_targetScenePos, m_targetViewportPos;
    bool eventFilter(QObject *, QEvent *);
};

#endif // VIEWZOOM_H
