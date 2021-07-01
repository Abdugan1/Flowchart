#include "graphicsitemgroup.h"

#include <QPainter>

GraphicsItemGroup::GraphicsItemGroup(QGraphicsItem *parent)
    : QGraphicsItemGroup(parent)
{
}

QRectF GraphicsItemGroup::boundingRect() const
{
    QRectF rect = QGraphicsItemGroup::boundingRect();
    rect.setX(rect.x() - 15);
    rect.setY(rect.y() - 15);
    rect.setWidth(rect.width()   + 15);
    rect.setHeight(rect.height() + 15);
    return rect;
}

void GraphicsItemGroup::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)

    painter->setPen(QPen(Qt::darkCyan));
    painter->drawRect(boundingRect());
}

QVariant GraphicsItemGroup::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if (change == ItemSelectedHasChanged) {
        bool selected = value.toBool();
        if (!selected)
            emit lostSelection();
    }
    return QGraphicsItemGroup::itemChange(change, value);
}
