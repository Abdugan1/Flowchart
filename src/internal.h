#ifndef INTERNAL_H
#define INTERNAL_H

#include <QtCore>

class DiagramItem;
class QGraphicsItem;
class ItemProperties;

namespace internal {

QPointF snapToGrid(const QPointF& point, int gridSize);

int map(int value, int fromLow, int fromHigh, int toLow, int toHigh);

QList<DiagramItem*> getDiagramItemsFromQGraphics(const QList<QGraphicsItem*> items);

QPointF preventOutsideMove(QPointF newPosTopLeft, QPointF newPosBottomRight, const QRectF& rect);

QList<ItemProperties> getDiagramItemsProperties(const QList<DiagramItem*>& diagramItems);

QPointF getItemCenterPosInScene(const DiagramItem* item);

QPointF getPosThatItemCenterAtMousePos(const QPointF& mousePosition,
                                       const QGraphicsItem* item);

}

#endif // INTERNAL_H
