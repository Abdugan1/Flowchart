#ifndef RESIZEHANDLE_H
#define RESIZEHANDLE_H

#include "handleitem.h"
#include "itemtypes.h"

class SizeGrip;
class DiagramItem;

class ResizeHandle : public HandleItem
{
public:
    enum {Type = ItemTypes::SizeHandleItemType};

public:
    explicit ResizeHandle(PositionFlags positionFlags, SizeGrip* sizeGrip);

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

#endif // RESIZEHANDLE_H
