#ifndef ARROWITEM_H
#define ARROWITEM_H

#include <QGraphicsPathItem>
#include <QtCore/qglobal.h>
#include <QObject>

class DiagramItem;
class ArrowHandleItem;

class ArrowItem : public QObject, public QGraphicsPathItem
{
    Q_OBJECT

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

    void updatePath();

signals:
    void updateMyPath(ArrowItem* arrow, const QPoint& startPoint, const QPoint& endPoint);

private:
    struct Info {
        DiagramItem* item;
        ArrowHandleItem* handle;
    };

    Info startItem_;
    Info endItem_;
};

#endif // ARROWITEM_H
