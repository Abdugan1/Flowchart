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

protected:
    void hoverMoveEvent(QGraphicsSceneHoverEvent *event)    override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event)   override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event)    override;

private:
    QPointF restrictPosition(const QPointF& newPos);
    void changeParentBoundingRect(const QPointF& pos);
    void changeCursor();

    PositionFlags positionFlags_;
    SizeGripItem* parent_;
    QPointF anchor_;
};

#endif // HANDLEITEM_H
