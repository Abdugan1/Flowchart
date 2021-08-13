#include "handleitem.h"
#include "sizegripitem.h"
#include "diagramitem.h"
#include "diagramscene.h"
#include "handleitemappeararea.h"

#include "constants.h"
#include "internal.h"

#include <QGraphicsSceneHoverEvent>
#include <QGuiApplication>
#include <QCursor>
#include <QPainter>

using PositionFlags = HandleItem::PositionFlags;

HandleItem::HandleItem(PositionFlags positionFlags, QGraphicsItem *parent)
    : QGraphicsRectItem(-Constants::HandleItem::OverralWidth  / 2,
                        -Constants::HandleItem::OverralHeight / 2,
                         Constants::HandleItem::OverralWidth,
                         Constants::HandleItem::OverralHeight,
                         parent)
    , visibleRect_(-Constants::HandleItem::VisibleWidth  / 2,
                   -Constants::HandleItem::VisibleHeight / 2,
                    Constants::HandleItem::VisibleWidth,
                    Constants::HandleItem::VisibleHeight)
    , positionFlags_(positionFlags)
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

    // Workaround
    QPointF fromParentToScene = mapToScene(mapFromItem(QGraphicsItem::parentItem(), pos));
    pos =  mapToItem(QGraphicsItem::parentItem()->parentItem(),
                     mapFromScene(scene->preventOutsideMove(fromParentToScene, fromParentToScene)));

    if (mapToScene(pos) != mapToScene(QGraphicsItem::pos()))
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

void HandleItem::setSizeGripItem(SizeGripItem *newSizeGripItem)
{
    sizeGripItem_ = newSizeGripItem;
}
