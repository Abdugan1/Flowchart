#ifndef ARROWHANDLEITEM_H
#define ARROWHANDLEITEM_H

#include <QGraphicsEllipseItem>
#include <QtCore/qglobal.h>

class ArrowHandleItem : public QGraphicsEllipseItem
{
public:
    enum PositionFlags {
        Top,
        Left,
        Right,
        Bottom
    };

public:
    ArrowHandleItem(PositionFlags positionFlag, QGraphicsItem* parent = nullptr);

    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option,
               QWidget *widget) override;

    PositionFlags positionFlag() const;

private:
    PositionFlags positionFlag_;
};

#endif // ARROWHANDLEITEM_H
