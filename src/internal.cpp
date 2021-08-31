#include "internal.h"
#include "diagramitem.h"
#include "itemproperties.h"

#include <QDebug>
#include <QPointF>

QPointF internal::snapToGrid(const QPointF& point, int gridSize)
{
    return QPointF(qRound(point.x() / gridSize) * gridSize,
                   qRound(point.y() / gridSize) * gridSize);
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
