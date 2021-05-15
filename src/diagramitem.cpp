#include "diagramitem.h"

#include <QGraphicsScene>
#include <QPainter>
#include <QtDebug>

DiagramItem::DiagramItem(DiagramItem::DiagramType diagramType, QGraphicsItem *parent)
    : QGraphicsPolygonItem(parent)
    , diagramType_(diagramType)
{

    int w = DefaultSize::Width;
    int h = DefaultSize::Height;
    switch (diagramType) {
    case Terminal:
        break;
    case Process:
        polygon_ << QPointF(0, 0)  << QPointF(w, 0)
                  << QPointF(w, h) << QPointF(0, h);
        break;
    case Desicion:
        polygon_ << QPointF(0, h / 2) << QPointF(w / 2, 0)
                  << QPointF(w, h / 2) << QPointF(w / 2, h);
        break;
    case InOut:
        polygon_ << QPointF(w * 0.25, 0) << QPointF(w, 0)
                  << QPointF(w * 0.75, h) << QPointF(0, h);
        break;
    }

    setPolygon(polygon_);
    setBrush(Qt::white);

    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
    setFlag(QGraphicsItem::ItemIsSelectable,         true);
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

QPixmap DiagramItem::image() const
{
    QPixmap pixmap(DefaultSize::Width, DefaultSize::Height);
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setPen(QPen(Qt::black, 6));
    painter.drawPolygon(polygon_);
    return pixmap;
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
