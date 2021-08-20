#include "handleitemappeararea.h"
#include "sizegripitem.h"
#include "diagramitem.h"
#include "sizehandleitem.h"

#include "constants.h"

#include <QPainter>
#include <QDebug>

HandleItemAppearArea::HandleItemAppearArea(SizeHandleItem *handleItem, SizeGripItem *sizeGripItem)
    : QGraphicsItem(sizeGripItem->diagramItem())
{
    sizeGripItem_ = sizeGripItem;
    handleItem_   = handleItem;
    handleItem_->setParentItem(this);
    handleItem_->setSizeGripItem(sizeGripItem);

    setAcceptHoverEvents(true);

    appearArea_ = handleItem_->boundingRect();

    appearArea_.setX(appearArea_.x() - Constants::HandleItem::Margin);
    appearArea_.setY(appearArea_.y() - Constants::HandleItem::Margin);
    appearArea_.setWidth(appearArea_.width()   + Constants::HandleItem::Margin);
    appearArea_.setHeight(appearArea_.height() + Constants::HandleItem::Margin);
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
    sizeGripItem_->hideHandleItems();
    QGraphicsItem::hoverLeaveEvent(event);
}

SizeHandleItem *HandleItemAppearArea::handleItem() const
{
    return handleItem_;
}
