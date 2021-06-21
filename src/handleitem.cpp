#include "handleitem.h"
#include "sizegripitem.h"
#include "diagramscene.h"

#include "internal.h"

#include <QBrush>
#include <QGraphicsSceneHoverEvent>
#include <QGuiApplication>
#include <QCursor>

using PositionFlags = HandleItem::PositionFlags;

HandleItem::HandleItem(PositionFlags positionFlags, SizeGripItem* parent)
    : QGraphicsRectItem(-4, -4, 8, 8, parent),
      positionFlags_(positionFlags),
      parent_(parent)
{
    setBrush(QBrush(Qt::lightGray));
    setFlag(ItemIsMovable);
    setAcceptHoverEvents(true);
}

PositionFlags HandleItem::positionFlags() const
{
    return positionFlags_;
}

void HandleItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    if (event->modifiers() != Qt::CTRL)
        changeCursor();
    QGraphicsRectItem::hoverEnterEvent(event);
}

void HandleItem::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    if (!QGuiApplication::overrideCursor() && event->modifiers() != Qt::CTRL)
        changeCursor();
    QGraphicsRectItem::hoverMoveEvent(event);
}

void HandleItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    QGuiApplication::restoreOverrideCursor();
    QGraphicsRectItem::hoverLeaveEvent(event);
}

void HandleItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QPointF pos = mapToParent(event->pos());
    pos = internal::getPointByStep(pos, DiagramScene::GridSize);
    pos = restrictPosition(pos);

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
        parent_->setTopLeft(pos);
        break;
    case Top:
        parent_->setTop(pos.y());
        break;
    case TopRight:
        parent_->setTopRight(pos);
        break;
    case Right:
        parent_->setRight(pos.x());
        break;
    case BottomRight:
        parent_->setBottomRight(pos);
        break;
    case Bottom:
        parent_->setBottom(pos.y());
        break;
    case BottomLeft:
        parent_->setBottomLeft(pos);
        break;
    case Left:
        parent_->setLeft(pos.x());
        break;
    }
}

void HandleItem::changeCursor()
{
    QCursor newCursor;
    switch (positionFlags_)
    {
    case TopLeft:
        newCursor.setShape(Qt::SizeFDiagCursor);
        break;
    case Top:
        newCursor.setShape(Qt::SizeVerCursor);
        break;
    case TopRight:
        newCursor.setShape(Qt::SizeBDiagCursor);
        break;
    case Right:
        newCursor.setShape(Qt::SizeHorCursor);
        break;
    case BottomLeft:
        newCursor.setShape(Qt::SizeBDiagCursor);
        break;
    case Bottom:
        newCursor.setShape(Qt::SizeVerCursor);
        break;
    case BottomRight:
        newCursor.setShape(Qt::SizeFDiagCursor);
        break;
    case Left:
        newCursor.setShape(Qt::SizeHorCursor);
        break;
    }
    QGuiApplication::setOverrideCursor(newCursor);
}
