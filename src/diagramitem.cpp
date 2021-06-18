#include "diagramitem.h"
#include "diagramscene.h"

#include "internal.h"

#include <QGraphicsScene>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
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
        DiagramScene* scene = static_cast<DiagramScene*>(this->scene());
        newPos = internal::getPointByStep(newPos, DiagramScene::GridSize / 2);
        newPos = scene->preventOutsideMove(newPos, this);

        if (newPos != pos())
            emit itemPositionChanging(pos(), newPos);

        return newPos;

    } else if (change == ItemSelectedChange
               && textItem_->textInteractionFlags() != Qt::NoTextInteraction
               && !value.toBool()) {

        textItem_->setTextInteraction(false);
    }
    return QGraphicsItem::itemChange(change, value);
}

void DiagramItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    emit itemReleased();
    QGraphicsPolygonItem::mouseReleaseEvent(event);
}

void DiagramItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    if (textItem_->textInteractionFlags() == Qt::TextEditorInteraction) {
        QGraphicsPolygonItem::mouseDoubleClickEvent(event);
        return;
    }
    textItem_->setTextInteraction(true);
    QGraphicsPolygonItem::mouseDoubleClickEvent(event);
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

void DiagramItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget)

    bool selected = (option->state & QStyle::State_Selected);

    QColor polygonColor = (selected ? QColor(Qt::green) : QColor(Qt::black));
    int width = (selected ? SelectedPenWidth : DefaultPenWidth);

    QPen pen(polygonColor);
    pen.setWidth(width);
    painter->setPen(pen);
    painter->setBrush(QBrush(Qt::white));
    painter->drawPolygon(polygon());
}

QRectF DiagramItem::boundingRect() const
{
    QRectF rect = polygon().boundingRect();
    rect.setX(rect.x() - SelectedPenWidth / 2);
    rect.setY(rect.y() - SelectedPenWidth / 2);
    rect.setWidth(rect.width() + SelectedPenWidth / 2);
    rect.setHeight(rect.height() + SelectedPenWidth / 2);
    return rect;
}

QRectF DiagramItem::polygonBoundingRect() const
{
    return polygon().boundingRect();
}

void DiagramItem::updateTextItemPosition()
{
    textItem_->updatePosition();
}
