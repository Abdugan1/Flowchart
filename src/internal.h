#ifndef INTERNAL_H
#define INTERNAL_H

#include <QtGlobal>

class QPointF;
class DiagramItem;
class QGraphicsItem;

namespace internal {

QPointF getPointByStep(QPointF point, int step);
int map(int value, int fromLow, int fromHigh, int toLow, int toHigh);
QList<DiagramItem*> getDiagramItemsFromQGraphics(const QList<QGraphicsItem*> items);

}

#endif // INTERNAL_H
