#ifndef HANDLEITEMAPPEARAREA_H
#define HANDLEITEMAPPEARAREA_H

#include <QGraphicsItem>
#include <QtCore/qglobal.h>

class SizeHandleItem;
class SizeGripItem;

class HandleItemAppearArea : public QGraphicsItem
{
public:
    HandleItemAppearArea(SizeHandleItem* handleItem, SizeGripItem* sizeGripItem);

    QRectF boundingRect() const override;
    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option,
               QWidget *widget) override;

    SizeHandleItem *handleItem() const;

protected:
    void hoverMoveEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;

private:
    SizeGripItem* sizeGripItem_;
    SizeHandleItem* handleItem_;
    QRectF appearArea_;
};

#endif // HANDLEITEMAPPEARAREA_H
