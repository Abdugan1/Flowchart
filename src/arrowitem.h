#ifndef ARROWITEM_H
#define ARROWITEM_H

#include <QGraphicsPathItem>
#include <QtCore/qglobal.h>

#include "handleitem.h"
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

    void updateConnectionPath();

    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option,
               QWidget *widget) override;

    QPainterPath shape() const override;

private:
    void calculateShape();

    void updateArrowHead(const QPointF& endPoint, PositionFlags pf);

    static QList<QLineF> getMainConnectionLines(const QPoint& startPoint, const QPoint& endPoint);

    static QList<QLineF> getDefaultConnection(const QPoint& startPos, const QPoint& endPos,
                                              const QPointF& startFinish, const QPointF& endFinish);

    static QList<QLineF> getReverseConnection(const QPoint& startPos, const QPoint& endPos,
                                              const QPointF& startFinish, const QPointF& endFinish);

    static QList<QLineF> getConnectionLines(const QPoint& startPos, const QPoint& endPos,
                                            const QPointF& startFinish, const QPointF& endFinish);

    static void setFinishConnections(QList<QLineF>& lines, const QPointF& startFinish, const QPointF& endFinish);

    static bool isCaseOfReverseConnection(PositionFlags startHandleFlag, PositionFlags endHandleFlag,
                                          const QPointF& startPos, const QPointF& endPos);

private:
    struct Info {
        DiagramItem* item = nullptr;
        ArrowHandleItem* handle = nullptr;
    };

    Info startItem_;
    Info endItem_;
    QList<QLineF> lines_;
    QPainterPath shape_;
    QPolygonF arrowHead_;
};

QPointF getFinishConnectPoint(DiagramItem* diagramItem, PositionFlags handlePosFlag);

#endif // ARROWITEM_H
