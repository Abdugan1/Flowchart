#include "graphicsitemgroup.h"
#include "diagramitem.h"

#include "constants.h"
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
    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemIsSelectable);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges);
}

QRectF GraphicsItemGroup::boundingRect() const
{
    QRectF rect = QGraphicsItemGroup::boundingRect();
    return QRectF(rect.x()      - Constants::GraphicsItemGroup::Margin - Constants::GraphicsItemGroup::PenWidth / 2,
                  rect.y()      - Constants::GraphicsItemGroup::Margin - Constants::GraphicsItemGroup::PenWidth / 2,
                  rect.width()  + Constants::GraphicsItemGroup::Margin * 2 + Constants::GraphicsItemGroup::PenWidth,
                  rect.height() + Constants::GraphicsItemGroup::Margin * 2 + Constants::GraphicsItemGroup::PenWidth);
}

void GraphicsItemGroup::paint(QPainter *painter,
                              const QStyleOptionGraphicsItem *option,
                              QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)

    painter->setPen(QPen(Qt::darkCyan));
    QRectF rect = boundingRect();
    painter->drawRect(QRectF(rect.x()      + Constants::GraphicsItemGroup::PenWidth / 2,
                             rect.y()      + Constants::GraphicsItemGroup::PenWidth / 2,
                             rect.width()  - Constants::GraphicsItemGroup::PenWidth,
                             rect.height() - Constants::GraphicsItemGroup::PenWidth));
}

int GraphicsItemGroup::type() const
{
    return Type;
}

void GraphicsItemGroup::addDiagramItem(DiagramItem *item)
{
    diagramItems_.append(item);
    addToGroup(item);
}

QVariant GraphicsItemGroup::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if (change == ItemPositionChange) {
        QPointF newPos = value.toPointF();

        newPos = internal::getPointByStep(newPos, Constants::DiagramScene::GridSize);

        QPointF bottomRight = calculateBottomRight(newPos);
        newPos = internal::preventOutsideMove(newPos, bottomRight,
                                              QRectF(0, 0,Constants::DiagramScene::A4Width,
                                                     Constants::DiagramScene::A4Height));

        return newPos;

    } else if (change == ItemPositionHasChanged) {
        for (auto item : qAsConst(diagramItems_))
            item->updateArrows();

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
    QPointF bottomRight = diagramItems_.at(0)->pos() +
            diagramItems_.at(0)->pathBoundingRect().bottomRight();

    for (auto item : qAsConst(diagramItems_)) {
        QPointF pos = item->pos() + item->pathBoundingRect().bottomRight();
        bottomRight.setX(qMax(pos.x(), bottomRight.x()));
        bottomRight.setY(qMax(pos.y(), bottomRight.y()));
    }

    return topLeft + bottomRight;
}

const QList<DiagramItem *> &GraphicsItemGroup::diagramItems() const
{
    return diagramItems_;
}
