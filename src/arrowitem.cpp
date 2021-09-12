#include "arrowitem.h"
#include "arrowhandleitem.h"

#include <QDebug>
#include <QPainter>
#include <QStyleOptionGraphicsItem>

ArrowItem::ArrowItem(QGraphicsItem *parent)
    : QGraphicsPathItem(parent)
{
    setZValue(-1);
    setAcceptHoverEvents(true);
    setFlag(ItemIsSelectable);
}

int ArrowItem::type() const
{
    return Type;
}

void ArrowItem::setPathShape(const QList<QLineF> &lines)
{
    lines_ = lines;
    calculateShape();
    QPainterPath pathShape;
    pathShape.moveTo(lines.first().p1());
    for (auto line : lines) {
        pathShape.lineTo(line.p2());
    }

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

void ArrowItem::updatePathShape()
{
    QList<QLineF> lines;

    PositionFlags sf = startItem_.handle->positionFlags();
    PositionFlags ef = endItem_.handle->positionFlags();

    QPoint sp = startPoint();
    QPoint ep = endPoint();

    if (sf == Top && ef == Top && sp.y() < ep.y()) {
        /// Top
        lines = getConnectionPath(ep, sp);
    } else if (sf == Left && sp.x() > ep.x()) {
        /// Left
        lines = getConnectionPath(ep, sp);
    } else if (ef  == Right && ep.x() > sp.x()) {
        /// Right -> not implemented!
        lines = getConnectionPath(ep, sp);
    } else if (sf == Bottom
               && (ef == Top || ef == Bottom || ef == Left)
               && sp.y() > ep.y()) {
        /// Bottom
        lines = getConnectionPath(ep, sp);
    } else {
        lines = getConnectionPath(sp, ep);
    }
    setPathShape(lines);
}

void ArrowItem::paint(QPainter *painter,
                      const QStyleOptionGraphicsItem *option,
                      QWidget *widget)
{
    Q_UNUSED(widget)

    QColor color;
    if (option->state & QStyle::State_Selected) {
        color = Qt::green;
    } else if (option->state & QStyle::State_MouseOver) {
        color = QColor(52, 201, 36);
    }

    painter->setPen(QPen(color));
    painter->drawPath(path());
}

QPainterPath ArrowItem::shape() const
{
    return shape_;
}

QList<QLineF> ArrowItem::getConnectionPath(const QPoint &startPoint, const QPoint &endPoint)
{
    QPoint p(startPoint.x(), endPoint.y());
    QLineF l1(startPoint, p);
    QLineF l2(p, endPoint);
    return {l1, l2};
}

void ArrowItem::calculateShape()
{
    shape_.clear();
    shape_.setFillRule(Qt::WindingFill);
    for (auto line : qAsConst(lines_)) {
        QPointF topLeft(qMin(line.p1().x(), line.p2().x()) - 5,
                        qMin(line.p1().y(), line.p2().y()) - 5);
        QPointF bottomRight(qMax(line.p1().x(), line.p2().x()) + 5,
                            qMax(line.p1().y(), line.p2().y()) + 5);
        QRectF rect(topLeft, bottomRight);
        shape_.addRect(rect);
    }
}
