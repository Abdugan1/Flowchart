#ifndef ARROWITEM_H
#define ARROWITEM_H

#include <QGraphicsPathItem>
#include <QtCore/qglobal.h>

class DiagramItem;
class ArrowHandleItem;

class ArrowItem : public QGraphicsPathItem
{
public:
    ArrowItem(QGraphicsItem *parent = nullptr);

    enum {Type = UserType + 2};

public:
    int type() const override;

    void setPathShape(const QList<QLineF>& lines);

    void setStartItem(ArrowHandleItem* handle ,DiagramItem* item);
    DiagramItem* startItem() const;

    void setEndItem(ArrowHandleItem* handle, DiagramItem* item);
    DiagramItem* endItem() const;

    QPoint startPoint() const;
    QPoint endPoint() const;

    void updatePathShape();

private:
    QList<QLineF> getConnectionPath(const QPoint& startPoint, const QPoint& endPoint);

private:
    struct Info {
        DiagramItem* item;
        ArrowHandleItem* handle;
    };

    Info startItem_;
    Info endItem_;
};

#endif // ARROWITEM_H
