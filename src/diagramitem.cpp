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
        QRectF sceneRect = scene()->sceneRect();
        QPointF bottomRight;
        bottomRight.setX(boundingRect().width()  + newPos.x());
        bottomRight.setY(boundingRect().height() + newPos.y());
        if (!sceneRect.contains(newPos) || !sceneRect.contains(bottomRight)) {
            // Keep item inside scene rect
            qDebug() << "Vnimanie! Govorit Moskva!";
            newPos.setX(qMin(sceneRect.right(),  qMax(newPos.x(), sceneRect.left())));
            newPos.setY(qMin(sceneRect.bottom(), qMax(newPos.y(), sceneRect.top())));
            return newPos;
        }
    }
    return QGraphicsItem::itemChange(change, value);
}

DiagramItem::DiagramType DiagramItem::diagramType() const
{
    return diagramType_;
}
