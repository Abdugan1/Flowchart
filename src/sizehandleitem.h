#ifndef SIZEHANDLEITEM_H
#define SIZEHANDLEITEM_H

#include <QGraphicsRectItem>

class SizeGrip;
class DiagramItem;

class SizeHandleItem : public QGraphicsRectItem
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
    SizeHandleItem(PositionFlags positionFlags, QGraphicsItem* parent = nullptr);
    PositionFlags positionFlags() const;
    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option,
               QWidget *widget) override;

    void setSizeGripItem(SizeGrip *newSizeGripItem);

protected:
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event)    override;

private:
    QPointF restrictPosition(const QPointF& newPos);
    void changeParentBoundingRect(const QPointF& pos);
    void setCursorByFlag(PositionFlags positionFlags);

private:
    QRectF visibleRect_;

    PositionFlags positionFlags_;
    SizeGrip* sizeGripItem_;
};

#endif // SIZEHANDLEITEM_H
