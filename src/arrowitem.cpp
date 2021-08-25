#include "arrowitem.h"
#include "arrowhandleitem.h"

#include <QDebug>

ArrowItem::ArrowItem(QGraphicsItem *parent)
    : QGraphicsPathItem(parent)
{
    setZValue(-1);
}

int ArrowItem::type() const
{
    return Type;
}

void ArrowItem::setPathShape(const QList<QLineF> &lines)
{
    QPolygonF polygon;
    for (auto line : lines) {
        QPointF p1 = line.p1();
        QPointF p2 = line.p2();
        polygon << p1 << p2;
    }
    QPainterPath pathShape;
    pathShape.addPolygon(polygon);
    QGraphicsPathItem::setPath(pathShape);
}

void ArrowItem::setStartItem(ArrowHandleItem *handle, DiagramItem *item)
{
    startItem_.item   = item;
    startItem_.handle = handle;
}

DiagramItem *ArrowItem::startItem() const
{
    return startItem_.item;
}

void ArrowItem::setEndItem(ArrowHandleItem *handle, DiagramItem *item)
{
    endItem_.item   = item;
    endItem_.handle = handle;
}

DiagramItem *ArrowItem::endItem() const
{
    return endItem_.item;
}

QPoint ArrowItem::startPoint() const
{
    return startItem_.handle->scenePos().toPoint();
}

QPoint ArrowItem::endPoint() const
{
    return endItem_.handle->scenePos().toPoint();
}

void ArrowItem::updatePath()
{
    emit updateMyPath(this, startItem_.handle->scenePos().toPoint(), endItem_.handle->scenePos().toPoint());
}
