#include "polygonresizer.h"

#include "diagramitem.h"

void PolygonResizer::operator ()(QGraphicsItem *item, QRectF &rect)
{
    DiagramItem* diagramItem = qgraphicsitem_cast<DiagramItem*>(item);
    QRectF itemRect = diagramItem->polygonBoundingRect();

    qreal sx = rect.width()  / itemRect.width();
    qreal sy = rect.height() / itemRect.height();

    QPolygonF oldPolygon = diagramItem->polygon();
    QPolygonF newPolygon = oldPolygon * QTransform::fromScale(sx, sy);

    QRectF polygonRect = newPolygon.boundingRect();

    qreal dx = rect.x() - polygonRect.x();
    qreal dy = rect.y() - polygonRect.y();

    diagramItem->moveBy(dx, dy);
    diagramItem->setPolygon(newPolygon);
    rect = polygonRect;
}
