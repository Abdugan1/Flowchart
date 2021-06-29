#include "diagramitem.h"
#include "diagramscene.h"

#include "internal.h"

#include <QGraphicsScene>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QGraphicsSceneMouseEvent>
#include <QDebug>
#include <QTextCursor>
#include <QTextDocument>
#include <QGuiApplication>

DiagramItem::DiagramItem(DiagramItem::DiagramType diagramType, QGraphicsItem *parent)
    : QGraphicsPathItem(parent)
    , diagramType_(diagramType)
    , textItem_(new DiagramTextItem(this))
{
    setPath(getDefaultShape(diagramType));
    setBrush(Qt::white);

    textItem_->setZValue(1000.0);
    textItem_->setAlignment(Qt::AlignCenter);
    textItem_->updatePosition();

    setFlag(QGraphicsItem::ItemIsMovable,            true);
    setFlag(QGraphicsItem::ItemIsSelectable,         true);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);

    setAcceptHoverEvents(true);
}

QVariant DiagramItem::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if (change == ItemPositionChange && scene()) {
        QPointF newPos = value.toPointF();
        DiagramScene* scene = static_cast<DiagramScene*>(this->scene());
        newPos = internal::getPointByStep(newPos, DiagramScene::GridSize / 2);
        newPos = scene->preventOutsideMove(newPos, this);

        return newPos;

    } else if (change == ItemPositionHasChanged) {
        emit itemPositionChanged(value.toPointF());

    } else if (change == ItemSelectedChange
               && textItem_->textInteractionFlags() != Qt::NoTextInteraction
               && !value.toBool()) {

        textItem_->setTextInteraction(false);
    }
    return QGraphicsItem::itemChange(change, value);
}

void DiagramItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (textItem_->textInteractionFlags() == Qt::TextEditorInteraction) {
        QPointF clickPos = mapToItem(textItem_, event->pos());
        setTextCursorMappedToTextItem(clickPos);
    }

    QGraphicsPathItem::mousePressEvent(event);
}

void DiagramItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    emit itemReleased();
    QGraphicsPathItem::mouseReleaseEvent(event);
}

void DiagramItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    if (textItem_->textInteractionFlags() == Qt::TextEditorInteraction) {
        QGraphicsPathItem::mouseDoubleClickEvent(event);
        return;
    }
    textItem_->setTextInteraction(true);
    QGraphicsPathItem::mouseDoubleClickEvent(event);
}

void DiagramItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    if (textItem_->textInteractionFlags() == Qt::TextEditorInteraction)
        QGuiApplication::setOverrideCursor(QCursor(Qt::IBeamCursor));

    QGraphicsPathItem::hoverEnterEvent(event);
}

void DiagramItem::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    if (textItem_->textInteractionFlags() == Qt::TextEditorInteraction
            && !QGuiApplication::overrideCursor())
        QGuiApplication::setOverrideCursor(QCursor(Qt::IBeamCursor));

    QGraphicsPathItem::hoverMoveEvent(event);
}

void DiagramItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    if (QGuiApplication::overrideCursor())
        QGuiApplication::restoreOverrideCursor();

    QGraphicsPathItem::hoverLeaveEvent(event);
}

void DiagramItem::setTextCursorMappedToTextItem(const QPointF &clickPos)
{
    QSizeF textItemSize = textItem_->boundingRect().size();
    QFontMetrics fontMetrics(textItem_->font());
    QString text = textItem_->document()->toPlainText();

    int position = 0;

    int y = qMax(0.0, qMin(textItemSize.height(), clickPos.y()));
    int div = y / fontMetrics.height();
    int h = div < text.count('\n') ? div : text.count('\n');
    QStringList strings = text.split('\n');
    auto itStr = strings.begin();
    for (int i = 0; i < h; ++i) {
        static const int delimLength = 1;
        position += (itStr++)->length() + delimLength;
    }

    QString str = *itStr;
    qreal strWidth = fontMetrics.horizontalAdvance(str);
    qreal strBeginPos = textItemSize.width() / 2 - strWidth / 2;

    int x = qMax(strBeginPos, qMin(double(strWidth) + strBeginPos, clickPos.x()));
    int l = str.length();
    if (l != 0)
        position += internal::map(x, strBeginPos, strWidth + strBeginPos, 0, l);

    QTextCursor cursor = textItem_->textCursor();
    cursor.setPosition(position);
    textItem_->setFocus(Qt::MouseFocusReason);
    textItem_->setSelected(true);
    textItem_->setTextCursor(cursor);
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
    painter.drawPath(getDefaultShape(diagramType()));
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
    painter->drawPath(path());
}

QRectF DiagramItem::boundingRect() const
{
    QRectF rect = path().boundingRect();
    rect.setX(rect.x() - SelectedPenWidth / 2);
    rect.setY(rect.y() - SelectedPenWidth / 2);
    rect.setWidth(rect.width() + SelectedPenWidth / 2);
    rect.setHeight(rect.height() + SelectedPenWidth / 2);
    return rect;
}

QRectF DiagramItem::pathBoundingRect() const
{
    return path().boundingRect();
}

QPainterPath DiagramItem::getDefaultShape(DiagramType diagramType)
{
    int w = DefaultSize::Width;
    int h = DefaultSize::Height;
    QPainterPath painterPath;
    switch (diagramType) {
    case Terminal:
    {
        QRectF rect(0, 0, w, h);
        painterPath.addRoundedRect(rect, h / 2, h / 2);
        break;
    }
    case Process:
    {
        QRectF rect(0, 0, w, h);
        painterPath.addRect(rect);
        break;
    }
    case Desicion:
    {
        QPolygonF polygon;
        polygon << QPointF(0, h / 2) << QPointF(w / 2, 0)
                << QPointF(w, h / 2) << QPointF(w / 2, h)
                << QPointF(0, h / 2);
        painterPath.addPolygon(polygon);
        break;
    }
    case InOut:
    {
        QPolygonF polygon;
        polygon << QPointF(w * 0.25, 0) << QPointF(w, 0)
                << QPointF(w * 0.75, h) << QPointF(0, h)
                << QPointF(w * 0.25, 0);
        painterPath.addPolygon(polygon);
        break;
    }
    case ForLoop:
    {
        QPolygonF polygon;
        polygon << QPointF(0, h / 2)     << QPointF(w * 0.125, 0)
                << QPointF(w * 0.875, 0) << QPointF(w, h / 2)
                << QPointF(w * 0.875, h) << QPointF(w * 0.125, h)
                << QPointF(0, h / 2);
        painterPath.addPolygon(polygon);
        break;
    }
    }
    return painterPath;
}

void DiagramItem::updateTextItemPosition()
{
    textItem_->updatePosition();
}
