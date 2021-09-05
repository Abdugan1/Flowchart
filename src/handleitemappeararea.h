#ifndef HANDLEITEMAPPEARAREA_H
#define HANDLEITEMAPPEARAREA_H

#include <QGraphicsItem>
#include <QtCore/qglobal.h>

#include "itemtypes.h"

class HandleManager;

class HandleItemAppearArea : public QGraphicsItem
{
public:
    enum {Type = ItemTypes::HandleItemAppearAreaType};

public:
    HandleItemAppearArea(QGraphicsItem* handleItem, HandleManager* handleManager);

    QRectF boundingRect() const override;
    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option,
               QWidget *widget) override;
    int type() const override;

    QGraphicsItem *handleItem() const;

protected:
    void hoverMoveEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;

private:
    HandleManager* sizeGripItem_ = nullptr;
    QGraphicsItem* handleItem_ = nullptr;
    QRectF appearArea_;
};

#endif // HANDLEITEMAPPEARAREA_H
