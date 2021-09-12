#include "handleitem.h"

HandleItem::HandleItem(PositionFlags positionFlags, QGraphicsItem *parent)
    : QGraphicsItem(parent)
    , positionFlags_(positionFlags)
{
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
