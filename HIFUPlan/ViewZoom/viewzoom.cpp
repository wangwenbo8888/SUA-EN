#include <QMouseEvent>
#include <QApplication>
#include <QScrollBar>
#include <qmath.h>

#include "viewzoom.h"


ViewZoom::ViewZoom(QGraphicsView *view)
    : QObject(view), m_view(view)
{
    m_view->viewport()->installEventFilter(this);
    m_view->setMouseTracking(true);
    m_modifiers = Qt::ControlModifier;
    m_zoomFactorBase = 1.0015;
}

void ViewZoom::zoom(qreal factor) {
    m_view->scale(factor, factor);
    m_view->centerOn(m_targetScenePos);
    QPointF delta_viewport_pos = m_targetViewportPos -
            QPointF(m_view->viewport()->width() / 2,
                    m_view->viewport()->height() / 2);
    QPointF viewport_center = m_view->mapFromScene(m_targetScenePos) - delta_viewport_pos;
    m_view->centerOn(m_view->mapToScene(viewport_center.toPoint()));
    emit zoomed();
}

void ViewZoom::setModifiers(Qt::KeyboardModifiers modifiers) {
    m_modifiers = modifiers;
}

void ViewZoom::setZoomFactorBase(qreal base) {
    m_zoomFactorBase = base;
}

bool ViewZoom::eventFilter(QObject *object, QEvent *event)
{
    if (event->type() == QEvent::MouseMove)
    {
        QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
        QPointF delta = m_targetViewportPos - mouseEvent->pos();
        if (qAbs(delta.x()) > 5 || qAbs(delta.y()) > 5)
        {
            m_targetViewportPos = mouseEvent->pos();
            m_targetScenePos = m_view->mapToScene(mouseEvent->pos());
        }
    } else if (event->type() == QEvent::Wheel)
    {
        QWheelEvent* wheelEvent = static_cast<QWheelEvent*>(event);
        if (wheelEvent->orientation() == Qt::Vertical)
        {

        }
    }
    return true;
}
