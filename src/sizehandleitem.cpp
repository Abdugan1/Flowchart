#include "sizehandleitem.h"
#include "sizegripdiagramitem.h"
#include "diagramitem.h"
#include "handleitemappeararea.h"

#include "constants.h"
#include "internal.h"

#include <QGraphicsSceneHoverEvent>
#include <QGuiApplication>
#include <QCursor>
#include <QPainter>

SizeHandleItem::SizeHandleItem(PositionFlags positionFlags, QGraphicsItem *parent)
    : HandleItem(positionFlags, parent)
    , boundingRect_(-Constants::SizeHandleItem::OverralWidth  / 2,
                    -Constants::SizeHandleItem::OverralHeight / 2,
                     Constants::SizeHandleItem::OverralWidth,
                     Constants::SizeHandleItem::OverralHeight)

    , visibleRect_(-Constants::SizeHandleItem::VisibleWidth  / 2,
                   -Constants::SizeHandleItem::VisibleHeight / 2,
                    Constants::SizeHandleItem::VisibleWidth,
                    Constants::SizeHandleItem::VisibleHeight)
{
    setFlag(ItemIsMovable);
    setCursorByFlag(positionFlags);
}

void SizeHandleItem::setHandleManager(HandleManager *newHandleManager)
{
    sizeGripItem_ = qobject_cast<SizeGrip*>(newHandleManager);
    Q_ASSERT(sizeGripItem_);
    HandleItem::setHandleManager(sizeGripItem_);
}

QRectF SizeHandleItem::boundingRect() const
{
    return boundingRect_;
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

int SizeHandleItem::type() const
{
    return Type;
}

void SizeHandleItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    Q_ASSERT(sizeGripItem_);

    QPointF pos = mapToParent(event->pos());
    pos = internal::snapToGrid(pos, Constants::DiagramScene::GridSize);
    pos = restrictPosition(pos);

    // Workaround
    QPointF fromParentToScene = mapToScene(mapFromItem(QGraphicsItem::parentItem(), pos));
    pos =  mapToItem(QGraphicsItem::parentItem()->parentItem(),
                     mapFromScene(internal::preventOutsideMove(fromParentToScene, fromParentToScene,
                                                               sizeGripItem_->maxGripArea())));

    if (mapToScene(pos) != mapToScene(QGraphicsItem::pos()))
        changeParentBoundingRect(pos);
}

QPointF SizeHandleItem::restrictPosition(const QPointF& newPos)
{
    QPointF retVal = pos();

    auto positionFlags = HandleItem::positionFlags();
    if (positionFlags & Top || positionFlags & Bottom)
        retVal.setY(newPos.y());

    if (positionFlags & Left || positionFlags & Right)
        retVal.setX(newPos.x());

    return retVal;
}

void SizeHandleItem::changeParentBoundingRect(const QPointF &pos)
{
    Q_ASSERT(sizeGripItem_);
    switch (HandleItem::positionFlags())
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
