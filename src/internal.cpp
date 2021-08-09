#include "internal.h"
#include "diagramitem.h"
#include <QDebug>

#include <QPointF>

QPointF internal::getPointByStep(QPointF point, int step)
{
    qreal xV = qRound(point.x() / step) * step;
    qreal yV = qRound(point.y() / step) * step;

    point.setX(xV);
    point.setY(yV);

    return point;
}

int internal::map(int value, int frowLow, int fromHigh, int toLow, int toHigh)
{
    return (value - frowLow) * (toHigh - toLow) / (fromHigh - frowLow) + toLow;
}

QList<DiagramItem *> internal::getDiagramItemsFromQGraphics(const QList<QGraphicsItem *> items)
{
    QList<DiagramItem*> diagramItems;

    for (auto i : qAsConst(items)) {
        if (DiagramItem* item = qgraphicsitem_cast<DiagramItem*>(i))
            diagramItems.append(item);
    }

    return diagramItems;
}
