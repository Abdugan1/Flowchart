#include "handleitemappeararea.h"
#include "sizegripitem.h"
#include "handleitem.h"

#include <QPainter>

HandleItemAppearArea::HandleItemAppearArea(HandleItem *handleItem, SizeGripItem *parent)
    : QGraphicsItem(parent)
{
    sizeGripItem_ = parent;
    handleItem_   = handleItem;

    setAcceptHoverEvents(true);

    appearArea_ = handleItem_->boundingRect();
    appearArea_.setX(appearArea_.x() - DefaultDistance);
    appearArea_.setY(appearArea_.y() - DefaultDistance);
    appearArea_.setWidth(appearArea_.width()   + DefaultDistance);
    appearArea_.setHeight(appearArea_.height() + DefaultDistance);
}

QRectF HandleItemAppearArea::boundingRect() const
{
    return appearArea_;
}

void HandleItemAppearArea::paint(QPainter *painter,
                                 const QStyleOptionGraphicsItem *option,
                                 QWidget *widget)
{
    Q_UNUSED(painter)
    Q_UNUSED(option)
    Q_UNUSED(widget)
}

void HandleItemAppearArea::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    if (!handleItem_->isVisible()) {
        sizeGripItem_->showHandleItems();
    }
    QGraphicsItem::hoverMoveEvent(event);
}

void HandleItemAppearArea::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    QGraphicsItem::hoverLeaveEvent(event);
}

HandleItem *HandleItemAppearArea::handleItem() const
{
    return handleItem_;
}
