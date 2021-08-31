#ifndef INTERNAL_H
#define INTERNAL_H

#include <QtGlobal>

class QPointF;
class QRectF;
class DiagramItem;
class QGraphicsItem;
class QGraphicsScene;
class ItemProperties;

namespace internal {

QPointF snapToGrid(const QPointF& point, int gridSize);

int map(int value, int fromLow, int fromHigh, int toLow, int toHigh);

QList<DiagramItem*> getDiagramItemsFromQGraphics(const QList<QGraphicsItem*> items);

QPointF preventOutsideMove(QPointF newPosTopLeft, QPointF newPosBottomRight, const QRectF& rect);

QList<ItemProperties> getDiagramItemsProperties(const QList<DiagramItem*>& diagramItems);

}

#endif // INTERNAL_H
