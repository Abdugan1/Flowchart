#ifndef SIZEHANDLEITEM_H
#define SIZEHANDLEITEM_H

#include "handleitem.h"
#include "itemtypes.h"

class SizeGrip;
class DiagramItem;

class SizeHandleItem : public HandleItem
{
public:
    enum {Type = ItemTypes::SizeHandleItemType};

public:
    explicit SizeHandleItem(PositionFlags positionFlags, SizeGrip* sizeGrip);

    void setHandleManager(HandleManager *newHandleManager) override;

    QRectF boundingRect() const override;

    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option,
               QWidget *widget) override;
    int type() const override;

protected:
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event)    override;

private:
    QPointF restrictPosition(const QPointF& newPos);
    void changeParentBoundingRect(const QPointF& pos);
    void setCursorByFlag(PositionFlags positionFlags);

private:
    QRectF boundingRect_;
    QRectF visibleRect_;

    SizeGrip* sizeGrip_ = nullptr;
};

#endif // SIZEHANDLEITEM_H
