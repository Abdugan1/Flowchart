#include "diagramitem.h"
#include "diagramscene.h"

#include <QGraphicsScene>
#include <QPainter>
#include <QtDebug>

DiagramItem::DiagramItem(DiagramItem::DiagramType diagramType, QGraphicsItem *parent)
    : QGraphicsPolygonItem(parent)
    , diagramType_(diagramType)
    , textItem_(new DiagramTextItem(this))
{

    int w = DefaultSize::Width;
    int h = DefaultSize::Height;
    switch (diagramType) {
    case Terminal:
        break;
    case Process:
        polygon_ << QPointF(0, 0)  << QPointF(w, 0)
                  << QPointF(w, h) << QPointF(0, h);
        textItem_->setPlainText("Process");
        break;
    case Desicion:
        polygon_ << QPointF(0, h / 2) << QPointF(w / 2, 0)
                  << QPointF(w, h / 2) << QPointF(w / 2, h);
        textItem_->setPlainText("Desicion\nABDU");
        break;
    case InOut:
        polygon_ << QPointF(w * 0.25, 0) << QPointF(w, 0)
                  << QPointF(w * 0.75, h) << QPointF(0, h);
        textItem_->setPlainText("InOut");
        break;
    }

    setPolygon(polygon_);
    setBrush(Qt::white);

    textItem_->setZValue(1000.0);
    textItem_->setAlignment(Qt::AlignCenter);
    textItem_->setX(boundingRect().center().x() - textItem_->boundingRect().width()  / 2);
    textItem_->setY(boundingRect().center().y() - textItem_->boundingRect().height() / 2);

    setFlag(QGraphicsItem::ItemIsMovable,            true);
    setFlag(QGraphicsItem::ItemIsSelectable,         true);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
}

QVariant DiagramItem::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if (change == ItemPositionChange && scene()) {
        QPointF newPos = value.toPointF();
        newPos = moveByGrid(newPos);
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

QPointF DiagramItem::moveByGrid(QPointF pos)
{
    int gridSize = 20;
    qreal xV = round(pos.x() / gridSize) * gridSize;
    qreal yV = round(pos.y() / gridSize) * gridSize;

    pos.setX(xV);
    pos.setY(yV);

    return pos;
}
