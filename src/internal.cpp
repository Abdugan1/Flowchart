#include "internal.h"
#include "diagramitem.h"
#include "itemproperties.h"

#include <QDebug>
#include <QPointF>
#include <QGraphicsScene>

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

QPointF internal::preventOutsideMove(QPointF newPosTopLeft, QPointF newPosBottomRight, const QRectF &rect)
{
    if (!rect.contains(newPosTopLeft)) {
        newPosTopLeft.setX(qMax(newPosTopLeft.x(), rect.left()));
        newPosTopLeft.setY(qMax(newPosTopLeft.y(), rect.top()));
    }
    if (!rect.contains(newPosBottomRight)) {
        newPosTopLeft.setX(qMin(newPosBottomRight.x(), rect.right())  -
                           (newPosBottomRight.x() - newPosTopLeft.x()));

        newPosTopLeft.setY(qMin(newPosBottomRight.y(), rect.bottom()) -
                           (newPosBottomRight.y() - newPosTopLeft.y()));
    }
    return newPosTopLeft;
}

QList<ItemProperties> internal::getDiagramItemsProperties(const QList<DiagramItem *> &diagramItems)
{
    QList<ItemProperties> itemsProperties;
    itemsProperties.reserve(diagramItems.count());
    for (auto item : qAsConst(diagramItems)) {
        ItemProperties properties;
        obtainItemProperties(item, &properties);

        itemsProperties.append(properties);
    }
    return itemsProperties;
}
