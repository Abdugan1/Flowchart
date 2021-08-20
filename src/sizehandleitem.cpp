#include "sizehandleitem.h"
#include "sizegripitem.h"
#include "diagramitem.h"
#include "handleitemappeararea.h"

#include "constants.h"
#include "internal.h"

#include <QGraphicsSceneHoverEvent>
#include <QGuiApplication>
#include <QCursor>
#include <QPainter>

using PositionFlags = SizeHandleItem::PositionFlags;

SizeHandleItem::SizeHandleItem(PositionFlags positionFlags, QGraphicsItem *parent)
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

PositionFlags SizeHandleItem::positionFlags() const
{
    return positionFlags_;
}

void SizeHandleItem::paint(QPainter *painter,
                       const QStyleOptionGraphicsItem *option,
                       QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)

    painter->setPen(QPen(Qt::darkGray));
    painter->setBrush(QBrush(Qt::white));
    painter->drawRect(visibleRect_);
}

void SizeHandleItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QPointF pos = mapToParent(event->pos());
    pos = internal::getPointByStep(pos, Constants::DiagramScene::GridSize);
    pos = restrictPosition(pos);

    // Workaround
    QPointF fromParentToScene = mapToScene(mapFromItem(QGraphicsItem::parentItem(), pos));
    pos =  mapToItem(QGraphicsItem::parentItem()->parentItem(),
                     mapFromScene(internal::preventOutsideMove(fromParentToScene, fromParentToScene,
                                                               QRectF(0, 0, Constants::DiagramScene::A4Width,
                                                                      Constants::DiagramScene::A4Height))));

    if (mapToScene(pos) != mapToScene(QGraphicsItem::pos()))
        changeParentBoundingRect(pos);
}

QPointF SizeHandleItem::restrictPosition(const QPointF& newPos)
{
    QPointF retVal = pos();

    if (positionFlags_ & Top || positionFlags_ & Bottom)
        retVal.setY(newPos.y());

    if (positionFlags_ & Left || positionFlags_ & Right)
        retVal.setX(newPos.x());

    return retVal;
}

void SizeHandleItem::changeParentBoundingRect(const QPointF &pos)
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

void SizeHandleItem::setCursorByFlag(PositionFlags positionFlags)
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

void SizeHandleItem::setSizeGripItem(SizeGripItem *newSizeGripItem)
{
    sizeGripItem_ = newSizeGripItem;
}
