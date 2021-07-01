#include "diagramview.h"
#include "diagramitem.h"

#include <QtEvents>
#include <QDebug>
#include <QGraphicsSceneHoverEvent>


DiagramView::DiagramView(QWidget *parent)
    : QGraphicsView(parent)
{
    init();
}

DiagramView::DiagramView(QGraphicsScene *scene, QWidget *parent)
    : QGraphicsView(scene, parent)
{
    init();
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
        setInteractive(false);
        setDragMode(QGraphicsView::ScrollHandDrag);
    }
    QGraphicsView::mousePressEvent(event);
}

void DiagramView::mouseMoveEvent(QMouseEvent *event)
{
    QList<QGraphicsItem*> itemsUnderMouse = items(event->pos());

    for (auto* item : itemsUnderMouse) {
        QGraphicsSceneHoverEvent hoverMoveEvent(QEvent::GraphicsSceneHoverMove);
        scene()->sendEvent(item, &hoverMoveEvent);
    }

    if (event->buttons() & Qt::LeftButton && event->modifiers() != Qt::CTRL
            && !isInteractive()) {
        setInteractive(true);
        setDragMode(NoDrag);
        setDragMode(RubberBandDrag);
    }
    QGraphicsView::mouseMoveEvent(event);
}

void DiagramView::mouseReleaseEvent(QMouseEvent *event)
{
    setInteractive(true);
    setDragMode(NoDrag);
    setDragMode(RubberBandDrag);
    QGraphicsView::mouseReleaseEvent(event);
}

void DiagramView::init()
{
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setDragMode(QGraphicsView::RubberBandDrag);
    setMouseTracking(true);

    connect(this, &DiagramView::rubberBandChanged, this,
            [this](QRect rubberBandRect, QPointF fromScenePoint, QPointF toScenePoint)
    {
        if (isRubberBandFinishedSelecting(rubberBandRect, fromScenePoint, toScenePoint))
            emit rubberBandSelectingFinished();
    });
}

bool DiagramView::isRubberBandFinishedSelecting(const QRect &rubberBandRect,
                                                const QPointF &fromScenePoint,
                                                const QPointF &toScenePoint)
{
    return (rubberBandRect.isNull() && fromScenePoint.isNull() && toScenePoint.isNull());
}
