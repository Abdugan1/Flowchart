#include "diagramitem.h"
#include "diagramscene.h"
#include "pathresizer.h"

#include "internal.h"

#include <QGraphicsScene>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QGraphicsSceneMouseEvent>
#include <QKeyEvent>
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

    sizeGripItem_ = new SizeGripItem(new PathResizer, this);
    connect(sizeGripItem_, &SizeGripItem::resizeBeenMade,
            this,          &DiagramItem::updateTextItemPosition);

    setFlag(QGraphicsItem::ItemIsMovable,            true);
    setFlag(QGraphicsItem::ItemIsSelectable,         true);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
    setFlag(QGraphicsItem::ItemIsFocusable,          true);

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

        state_ = State::Other;
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

void DiagramItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (state_ == State::TextEditing)
        return;

    if (!QGuiApplication::overrideCursor())
        QGuiApplication::setOverrideCursor(QCursor(Qt::SizeAllCursor));

    QGraphicsPathItem::mouseMoveEvent(event);
}

void DiagramItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (state_ == State::TextEditing)
        return;

    emit itemReleased();
    QGuiApplication::restoreOverrideCursor();
    QGraphicsPathItem::mouseReleaseEvent(event);
}

void DiagramItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    if (textItem_->textInteractionFlags() == Qt::TextEditorInteraction) {
        QGraphicsPathItem::mouseDoubleClickEvent(event);
        return;
    }
    enableTextEditing();
    QGraphicsPathItem::mouseDoubleClickEvent(event);
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

void DiagramItem::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_F2) {
        enableTextEditing();
    }

    QGraphicsPathItem::keyPressEvent(event);
}

void DiagramItem::enableTextEditing()
{
    state_ = State::TextEditing;
    textItem_->setTextInteraction(true);
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
    return QRectF(rect.x()      - SelectedPenWidth / 2,
                  rect.y()      - SelectedPenWidth / 2,
                  rect.width()  + SelectedPenWidth,
                  rect.height() + SelectedPenWidth);
}

QRectF DiagramItem::pathBoundingRect() const
{
    return path().boundingRect();
}

void DiagramItem::resize(const QSizeF &size)
{
    sizeGripItem_->setRect(QRectF(0, 0, size.width(), size.height()));
}

void DiagramItem::resize(qreal width, qreal height)
{
    resize(QSizeF(width, height));
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

QPixmap DiagramItem::image(DiagramType diagramType)
{
    QPixmap pixmap(DefaultSize::Width, DefaultSize::Height);
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setPen(QPen(Qt::black, 6));
    painter.setBrush(QBrush(Qt::white));
    painter.drawPath(getDefaultShape(diagramType));
    return pixmap;
}

QString DiagramItem::getToolTip(int diagramType)
{
    QString toolTip;
    switch (diagramType) {
    case Terminal:
    {
        toolTip = tr("<p><b>Terminal</b></p>"
                    "Indicates the beginning and ending of a program or sub-process");
        break;
    }
    case Process:
    {
        toolTip = tr("<p><b>Process</b></p>"
                    "Represents a set of operations that changes value, form, or location of data");
        break;
    }
    case Desicion:
    {
        toolTip = tr("<p><b>Desicion</b></p>"
                    "Shows a conditional operation that determines which one of the two paths the program will take");
        break;
    }
    case InOut:
    {
        toolTip = tr("<p><b>Input/Output</b></p>"
                    "Indicates the process of inputting and outputting data");
        break;
    }
    case ForLoop:
    {
        toolTip = tr("<p><b>For loop</b></p>"
                    "Used for steps like setting a switch or initializing a routine");
        break;
    }
    }
    return toolTip;
}

QString DiagramItem::text() const
{
    return textItem_->document()->toPlainText();
}

void DiagramItem::setText(const QString &text)
{
    textItem_->document()->setPlainText(text);
}

void DiagramItem::updateTextItemPosition()
{
    textItem_->updatePosition();
}
