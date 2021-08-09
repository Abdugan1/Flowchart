#include "handleitem.h"
#include "sizegripitem.h"
#include "diagramitem.h"
#include "diagramscene.h"

#include "constants.h"
#include "internal.h"

#include <QGraphicsSceneHoverEvent>
#include <QGuiApplication>
#include <QCursor>
#include <QDebug>
#include <QPainter>

using PositionFlags = HandleItem::PositionFlags;

HandleItem::HandleItem(PositionFlags positionFlags, SizeGripItem *sizeGripItem)
    : QGraphicsRectItem(-10, -10, 20, 20, sizeGripItem->diagramItem())
    , visibleRect_(-4, -4, 8, 8)
    , positionFlags_(positionFlags)
    , sizeGripItem_(sizeGripItem)
{
    setFlag(ItemIsMovable);
    setCursorByFlag(positionFlags);
}

PositionFlags HandleItem::positionFlags() const
{
    return positionFlags_;
}

void HandleItem::paint(QPainter *painter,
                       const QStyleOptionGraphicsItem *option,
                       QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)

    painter->setBrush(QBrush(Qt::lightGray));
    painter->drawRect(visibleRect_);
}

void HandleItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QPointF pos = mapToParent(event->pos());
    pos = internal::getPointByStep(pos, Constants::DiagramScene::GridSize);
    pos = restrictPosition(pos);

    DiagramScene* scene = static_cast<DiagramScene*>(this->scene());
    pos = mapToParent(mapFromScene(scene->preventOutsideMove(
                                       mapToScene(mapFromParent(pos)),
                                       this)));

    if (pos != this->pos())
        changeParentBoundingRect(pos);
}

QPointF HandleItem::restrictPosition(const QPointF& newPos)
{
    QPointF retVal = pos();

    if (positionFlags_ & Top || positionFlags_ & Bottom)
        retVal.setY(newPos.y());

    if (positionFlags_ & Left || positionFlags_ & Right)
        retVal.setX(newPos.x());

    return retVal;
}

void HandleItem::changeParentBoundingRect(const QPointF &pos)
{
    switch (positionFlags_)
    {
    case TopLeft:
        sizeGripItem_->setTopLeft(pos);
        break;
    case Top:
        sizeGripItem_->setTop(pos.y());
        break;
    case TopRight:
        sizeGripItem_->setTopRight(pos);
        break;
    case Right:
        sizeGripItem_->setRight(pos.x());
        break;
    case BottomRight:
        sizeGripItem_->setBottomRight(pos);
        break;
    case Bottom:
        sizeGripItem_->setBottom(pos.y());
        break;
    case BottomLeft:
        sizeGripItem_->setBottomLeft(pos);
        break;
    case Left:
        sizeGripItem_->setLeft(pos.x());
        break;
    }
}

void HandleItem::setCursorByFlag(PositionFlags positionFlags)
{
    QCursor cursor;
    switch (positionFlags)
    {
    case TopLeft:
        cursor.setShape(Qt::SizeFDiagCursor);
        break;
    case Top:
        cursor.setShape(Qt::SizeVerCursor);
        break;
    case TopRight:
        cursor.setShape(Qt::SizeBDiagCursor);
        break;
    case Right:
        cursor.setShape(Qt::SizeHorCursor);
        break;
    case BottomLeft:
        cursor.setShape(Qt::SizeBDiagCursor);
        break;
    case Bottom:
        cursor.setShape(Qt::SizeVerCursor);
        break;
    case BottomRight:
        cursor.setShape(Qt::SizeFDiagCursor);
        break;
    case Left:
        cursor.setShape(Qt::SizeHorCursor);
        break;
    }
    setCursor(cursor);
}
