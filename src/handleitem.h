#ifndef HANDLEITEM_H
#define HANDLEITEM_H

class SizeGripItem;

#include <QGraphicsRectItem>

class HandleItem : public QGraphicsRectItem
{
public:
    enum PositionFlags
    {
        Top         = 0x1,
        Bottom      = 0x2,
        Left        = 0x4,
        TopLeft     = Top | Left,
        BottomLeft  = Bottom | Left,
        Right       = 0x8,
        TopRight    = Top | Right,
        BottomRight = Bottom | Right
    };

public:
    HandleItem(PositionFlags positionFlags, SizeGripItem* parent);
    PositionFlags positionFlags() const;
    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option,
               QWidget *widget) override;

protected:
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event)    override;

private:
    QPointF restrictPosition(const QPointF& newPos);
    void changeParentBoundingRect(const QPointF& pos);
    void setCursorByFlag(PositionFlags positionFlags);

private:
    QRectF visibleRect_;

    PositionFlags positionFlags_;
    SizeGripItem* parent_;
};

#endif // HANDLEITEM_H
