#include "diagramitem.h"

#include <QGraphicsScene>
#include <QtDebug>

DiagramItem::DiagramItem(DiagramItem::DiagramType diagramType, QGraphicsItem *parent)
    : QAbstractGraphicsShapeItem(parent)
    , diagramType_(diagramType)
{
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
}

QVariant DiagramItem::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if (change == ItemPositionChange && scene()) {
        QPointF newPos = value.toPointF();
        newPos = preventOutsideMove(newPos);
        return newPos;
    }
    return QGraphicsItem::itemChange(change, value);
}

DiagramItem::DiagramType DiagramItem::diagramType() const
{
    return diagramType_;
}

QPointF DiagramItem::preventOutsideMove(QPointF topLeft)
{
    QRectF sceneRect = scene()->sceneRect();

    QPointF bottomRight;
    bottomRight.setX(boundingRect().width()  + topLeft.x());
    bottomRight.setY(boundingRect().height() + topLeft.y());

    if (!sceneRect.contains(topLeft)) {
        topLeft.setX(qMax(topLeft.x(), sceneRect.left()));
        topLeft.setY(qMax(topLeft.y(), sceneRect.top()));
    }
    if (!sceneRect.contains(bottomRight)) {
        topLeft.setX(qMin(bottomRight.x(), sceneRect.right())  - (bottomRight.x() - topLeft.x()));
        topLeft.setY(qMin(bottomRight.y(), sceneRect.bottom()) - (bottomRight.y() - topLeft.y()));
    }
    return topLeft;
}
