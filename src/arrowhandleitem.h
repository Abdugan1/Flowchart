#ifndef ARROWHANDLEITEM_H
#define ARROWHANDLEITEM_H

#include <QGraphicsEllipseItem>
#include <QtCore/qglobal.h>

class ArrowManager;

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

    void setArrowManager(ArrowManager *newArrowManager);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

private:
    PositionFlags positionFlag_;
    ArrowManager* arrowManager_;
};

#endif // ARROWHANDLEITEM_H
