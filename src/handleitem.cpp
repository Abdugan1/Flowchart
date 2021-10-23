#include "handleitem.h"
#include "handlemanager.h"

#include <QDebug>

HandleItem::HandleItem(PositionFlags positionFlags, HandleManager *handleManager)
    : QGraphicsItem(handleManager->handlingItem())
    , positionFlags_(positionFlags)
    , handleManager_(handleManager)
{
    setAcceptHoverEvents(true);
}

PositionFlags HandleItem::positionFlags() const
{
    return positionFlags_;
}

void HandleItem::setPositionFlags(PositionFlags newPositionFlags)
{
    positionFlags_ = newPositionFlags;
}

HandleManager *HandleItem::handleManager() const
{
    return handleManager_;
}

void HandleItem::setHandleManager(HandleManager *newHandleManager)
{
    handleManager_ = newHandleManager;
}

int HandleItem::type() const
{
    return Type;
}

bool HandleItem::shouldDraw() const
{
    return shouldDraw_;
}

void HandleItem::setShouldDraw(bool newShouldDraw)
{
    shouldDraw_ = newShouldDraw;
    update();
}

void HandleItem::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    handleManager_->setShouldDrawForHandleItems(true);
    QGraphicsItem::hoverMoveEvent(event);
}

void HandleItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    handleManager_->setShouldDrawForHandleItems(false);
    QGraphicsItem::hoverLeaveEvent(event);
}
