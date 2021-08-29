#ifndef ARROWITEM_H
#define ARROWITEM_H

#include <QGraphicsPathItem>
#include <QtCore/qglobal.h>

#include "itemtypes.h"

class DiagramItem;
class ArrowHandleItem;

class ArrowItem : public QGraphicsPathItem
{
public:
    enum {Type = ItemTypes::ArrowItemType};

public:
    ArrowItem(QGraphicsItem *parent = nullptr);

    int type() const override;

    void setPathShape(const QList<QLineF>& lines);

    void setStartItem(ArrowHandleItem* handle ,DiagramItem* item);
    DiagramItem* startItem() const;

    void setEndItem(ArrowHandleItem* handle, DiagramItem* item);
    DiagramItem* endItem() const;

    QPoint startPoint() const;
    QPoint endPoint() const;

    void updatePathShape();

    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option,
               QWidget *widget) override;

    QPainterPath shape() const override;

private:
    QList<QLineF> getConnectionPath(const QPoint& startPoint, const QPoint& endPoint);
    void calculateShape();

private:
    struct Info {
        DiagramItem* item;
        ArrowHandleItem* handle;
    };

    Info startItem_;
    Info endItem_;
    QList<QLineF> lines_;
    QPainterPath shape_;
};

#endif // ARROWITEM_H
