#include "handleitem.h"
#include "handleitemappeararea.h"

#include <QDebug>

HandleItem::HandleItem(PositionFlags positionFlags, HandleManager *handleManager)
    : QGraphicsItem(new HandleItemAppearArea(this, handleManager))
    , positionFlags_(positionFlags)
    , handleManager_(handleManager)
{
    appearArea_ = qgraphicsitem_cast<HandleItemAppearArea*>(parentItem());
    Q_ASSERT(appearArea_);
    setFlag(ItemSendsGeometryChanges);
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

const QRectF &HandleItem::appearArea() const
{
    return appearArea_->appearArea();
}

void HandleItem::setAppearArea(const QRectF &newAppearArea)
{
    appearArea_->setAppearArea(newAppearArea);
}

int HandleItem::type() const
{
    return Type;
}

QVariant HandleItem::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if (change == ItemPositionChange) {
        // Do not move self. Move parent, i. e. appearArea_
        appearArea_->setPos(value.toPointF());
        return QPointF();
    }

    return QGraphicsItem::itemChange(change, value);
}
