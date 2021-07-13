#include "handleitem.h"
#include "sizegripitem.h"
#include "diagramscene.h"

#include "internal.h"

#include <QBrush>
#include <QGraphicsSceneHoverEvent>
#include <QGuiApplication>
#include <QCursor>
#include <QDebug>
#include <QPainter>

using PositionFlags = HandleItem::PositionFlags;

HandleItem::HandleItem(PositionFlags positionFlags, SizeGripItem* parent)
    : QGraphicsRectItem(-4, -4, 8, 8, parent),
      positionFlags_(positionFlags),
      parent_(parent)
{
    setBrush(QBrush(Qt::lightGray));
    setFlag(ItemIsMovable);
    setCursorByFlag(positionFlags);
}

PositionFlags HandleItem::positionFlags() const
{
    return positionFlags_;
}

void HandleItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QPointF pos = mapToParent(event->pos());
    pos = internal::getPointByStep(pos, DiagramScene::GridSize);
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
