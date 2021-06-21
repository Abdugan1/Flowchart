#include "pathresizer.h"

#include "diagramitem.h"

void PathResizer::operator ()(QGraphicsItem *item, QRectF &rect)
{
    DiagramItem* diagramItem = qgraphicsitem_cast<DiagramItem*>(item);
    diagramItem->setFlag(QGraphicsItem::ItemSendsGeometryChanges, false);
    QRectF itemRect = diagramItem->pathBoundingRect();

    qreal sx = rect.width()  / itemRect.width();
    qreal sy = rect.height() / itemRect.height();

    QPainterPath oldPath = diagramItem->path();
    QPainterPath newPath = oldPath * QTransform::fromScale(sx, sy);

    QRectF pathRect = newPath.boundingRect();

    qreal dx = rect.x() - pathRect.x();
    qreal dy = rect.y() - pathRect.y();

    diagramItem->moveBy(dx, dy);
    diagramItem->setPath(newPath);
    rect = pathRect;
    diagramItem->setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
}
