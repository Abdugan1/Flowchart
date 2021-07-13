#include "graphicsitemgroup.h"
#include "diagramscene.h"
#include "diagramitem.h"
#include "internal.h"

#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include <QGuiApplication>
#include <QCursor>
#include <QDebug>

GraphicsItemGroup::GraphicsItemGroup(const QPointF &pos, QGraphicsItem *parent)
    : QGraphicsItemGroup(parent)
{
    setPos(pos);
    setCursor(QCursor(Qt::OpenHandCursor));
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
    if (change == ItemPositionChange) {
        QPointF newPos = value.toPointF();

        newPos = internal::getPointByStep(newPos, DiagramScene::GridSize / 2);

        QPointF bottomRight = calculateBottomRight(newPos);
        newPos = static_cast<DiagramScene*>(scene())->
                preventOutsideMove(newPos, bottomRight);

        return newPos;

    } else if (change == ItemSelectedHasChanged) {
        bool selected = value.toBool();
        if (!selected)
            emit lostSelection();
    }
    return QGraphicsItemGroup::itemChange(change, value);
}

void GraphicsItemGroup::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGuiApplication::setOverrideCursor(QCursor(Qt::ClosedHandCursor));
    QGraphicsItemGroup::mousePressEvent(event);
}

void GraphicsItemGroup::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QGuiApplication::restoreOverrideCursor();
    QGraphicsItemGroup::mouseReleaseEvent(event);
}

QPointF GraphicsItemGroup::calculateBottomRight(QPointF topLeft) const
{
    QList<DiagramItem*> diagramItems =
            internal::getDiagramItemsFromQGraphics(childItems());

    QPointF bottomRight = diagramItems.at(0)->pos() +
            diagramItems.at(0)->boundingRect().bottomRight();

    for (auto item : qAsConst(diagramItems)) {
        QPointF pos = item->pos() + item->boundingRect().bottomRight();
        bottomRight.setX(qMax(pos.x(), bottomRight.x()));
        bottomRight.setY(qMax(pos.y(), bottomRight.y()));
    }

    return topLeft + bottomRight;
}
