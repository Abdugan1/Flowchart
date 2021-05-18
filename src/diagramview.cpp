#include "diagramview.h"

#include <QtEvents>
#include <QtDebug>


DiagramView::DiagramView(QWidget *parent)
    : QGraphicsView(parent)
{
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

DiagramView::DiagramView(QGraphicsScene *scene, QWidget *parent)
    : QGraphicsView(scene, parent)
{
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

void DiagramView::wheelEvent(QWheelEvent *event)
{
    if (event->modifiers() == Qt::CTRL) {
        setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
        QPoint angle = event->angleDelta(); // Wheel
        QPoint pixel = event->pixelDelta(); // Touchpad
        bool zoomIn = true;
        if (!angle.isNull() && angle.y() < 0) {
            zoomIn = false;
        } else if (!pixel.isNull() && pixel.y() < 0) {
            zoomIn = false;
        }
        zoomIn ? scale(1.1, 1.1) : scale(1 / 1.1, 1 / 1.1);
    } else {
        QGraphicsView::wheelEvent(event);
    }
}

void DiagramView::mousePressEvent(QMouseEvent *event)
{
    if (event->modifiers() == Qt::CTRL) {
        originX_ = event->x();
        originY_ = event->y();
    }
    QGraphicsView::mousePressEvent(event);
}

void DiagramView::mouseMoveEvent(QMouseEvent *event)
{
    setTransformationAnchor(QGraphicsView::NoAnchor);
    // Moving around by Ctrl and mouse move.
    if (event->modifiers() == Qt::CTRL && event->buttons() & Qt::LeftButton) {
        QPointF oldPos = mapToScene(originX_, originY_);
        QPointF newPos = mapToScene(event->pos());
        QPointF translation = newPos - oldPos;

        translate(translation.x(), translation.y());

        originX_ = event->x();
        originX_ = event->y();

    } else {
        QGraphicsView::mouseMoveEvent(event);
    }
}
