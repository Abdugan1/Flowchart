#ifndef HANDLEITEMAPPEARAREA_H
#define HANDLEITEMAPPEARAREA_H

#include <QGraphicsItem>
#include <QtCore/qglobal.h>

class HandleItem;
class SizeGripItem;

class HandleItemAppearArea : public QGraphicsItem
{
public:
    enum { DefaultDistance = 3 };

public:
    HandleItemAppearArea(HandleItem* handleItem, SizeGripItem* parent);

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    HandleItem *handleItem() const;

protected:
    void hoverMoveEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;

private:
    SizeGripItem* sizeGripItem_;
    HandleItem* handleItem_;
    QRectF appearArea_;
};

#endif // HANDLEITEMAPPEARAREA_H
