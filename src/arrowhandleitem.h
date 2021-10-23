#ifndef ARROWHANDLEITEM_H
#define ARROWHANDLEITEM_H

#include <QtCore/qglobal.h>

#include "handleitem.h"
#include "itemtypes.h"

class ArrowManager;

class ArrowHandleItem : public HandleItem
{
public:
    enum {Type = ItemTypes::ArrowHandleItemType};

public:
    explicit ArrowHandleItem(PositionFlags positionFlag, ArrowManager* arrowManager);

    void setHandleManager(HandleManager *newHandleManager) override;

    QRectF boundingRect() const override;

    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option,
               QWidget *widget) override;

    int type() const override;

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

private:
    void drawMainCircle(QPainter* painter);
    void drawOuterCircle(QPainter* painter);

private:
    QRectF boundingRect_;
    ArrowManager* arrowManager_ = nullptr;
};

#endif // ARROWHANDLEITEM_H
