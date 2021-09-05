#ifndef ARROWHANDLEITEM_H
#define ARROWHANDLEITEM_H

#include <QGraphicsEllipseItem>
#include <QtCore/qglobal.h>

#include "itemtypes.h"

class ArrowManager;

class ArrowHandleItem : public QGraphicsEllipseItem
{
public:
    enum {Type = ItemTypes::ArrowHandleItemType};

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

    int type() const override;

    PositionFlags positionFlag() const;

    void setArrowManager(ArrowManager *newArrowManager);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

private:
    PositionFlags positionFlag_;
    ArrowManager* arrowManager_ = nullptr;
};

#endif // ARROWHANDLEITEM_H
