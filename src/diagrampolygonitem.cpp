#include "diagrampolygonitem.h"

#include <QPainter>

DiagramPolygonItem::DiagramPolygonItem(DiagramPolygonType diagramType, QGraphicsItem *parent)
    : DiagramItem(DiagramType(diagramType), parent)
{
    int w = DefaultSize::Width;
    int h = DefaultSize::Height;
    switch (diagramType) {
    case Process:
        polygonF_ << QPointF(0, 0)  << QPointF(w, 0)
                  << QPointF(w, h) << QPointF(0, h);
        break;
    case Desicion:
        polygonF_ << QPointF(0, h / 2) << QPointF(w / 2, 0)
                  << QPointF(w, h / 2) << QPointF(w / 2, h);
        break;
    case InOut:
        polygonF_ << QPointF(w * 0.25, 0) << QPointF(w, 0)
                  << QPointF(w * 0.75, h) << QPointF(0, h);
        break;
    }
}

QRectF DiagramPolygonItem::boundingRect() const
{
    qreal penWidth = 1;
    QRectF r = polygonF_.boundingRect();
    r.setX(r.x() - penWidth / 2);
    r.setY(r.y() - penWidth / 2);
    r.setWidth(r.width() + penWidth);
    r.setHeight(r.height() + penWidth);
    return r;
}

void DiagramPolygonItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)

    painter->save();
    painter->setBrush(QBrush(Qt::white));
    painter->drawPolygon(polygonF_);
    painter->restore();
}
