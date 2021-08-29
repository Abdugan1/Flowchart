#include "diagramitem.h"
#include "diagramtextitem.h"
#include "sizegrip.h"
#include "arrowmanager.h"

#include "constants.h"
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
#include <QElapsedTimer>

DiagramItem::DiagramItem(DiagramItem::DiagramType diagramType, QGraphicsItem *parent)
    : QGraphicsItem(parent)
    , diagramType_(diagramType)
    , textItem_(new DiagramTextItem(this))
{
    path_ = getDefaultShape(diagramType);
    size_ = QSizeF(Constants::DiagramItem::DefaultWidth,
                   Constants::DiagramItem::DefaultHeight);

    textItem_->setZValue(1000.0);
    textItem_->setAlignment(Qt::AlignCenter);
    textItem_->updatePosition();

    sizeGrip_ = new SizeGrip(this);
    connect(sizeGrip_, &SizeGrip::resizeBeenMade,
            this,      &DiagramItem::updateTextItemPosition);

    arrowManager_ = new ArrowManager(this);
    connect(sizeGrip_,      &SizeGrip::resizeBeenMade,
            arrowManager_,  &ArrowManager::updateHandleItemsPositions);

    connect(sizeGrip_,      &SizeGrip::resizeBeenMade,
            arrowManager_,  &ArrowManager::updateArrows);

    connect(this,           &DiagramItem::itemPositionChanged,
            arrowManager_,  &ArrowManager::updateArrows);

    setFlag(QGraphicsItem::ItemIsMovable,            true);
    setFlag(QGraphicsItem::ItemIsSelectable,         true);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
    setFlag(QGraphicsItem::ItemIsFocusable,          true);

    setAcceptHoverEvents(true);
}

DiagramItem::~DiagramItem()
{
    delete sizeGrip_;
    delete arrowManager_;
}

QVariant DiagramItem::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if (change == ItemPositionChange && scene()) {
        QPointF newPos = value.toPointF();

        newPos = internal::getPointByStep(newPos, Constants::DiagramScene::GridSize);

        newPos = internal::preventOutsideMove(newPos, newPos + QPointF(size_.width(), size_.height()),
                                              QRectF(0, 0, Constants::DiagramScene::A4Width,
                                                     Constants::DiagramScene::A4Height));

        return newPos;

    } else if (change == ItemPositionHasChanged) {
        emit itemPositionChanged();

    } else if (change == ItemSelectedChange
               && textItem_->textInteractionFlags() != Qt::NoTextInteraction
               && !value.toBool()) {

        textEditing_ = false;
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

    QGraphicsItem::mousePressEvent(event);
}

void DiagramItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (textEditing_)
        return;

    sizeGrip_->hideHandleItems();
    arrowManager_->hideHandleItems();

    if (!QGuiApplication::overrideCursor())
        QGuiApplication::setOverrideCursor(QCursor(Qt::SizeAllCursor));

    QGraphicsItem::mouseMoveEvent(event);
}

void DiagramItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (textEditing_)
        return;

    emit itemReleased();
    QGuiApplication::restoreOverrideCursor();
    QGraphicsItem::mouseReleaseEvent(event);
}

void DiagramItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    if (textItem_->textInteractionFlags() == Qt::TextEditorInteraction) {
        QGraphicsItem::mouseDoubleClickEvent(event);
        return;
    }
    enableTextEditing();
    QGraphicsItem::mouseDoubleClickEvent(event);
}

void DiagramItem::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    sizeGrip_->showHandleItems();
    arrowManager_->showHandleItems();

    if (textItem_->textInteractionFlags() == Qt::TextEditorInteraction
            && !QGuiApplication::overrideCursor())
        QGuiApplication::setOverrideCursor(QCursor(Qt::IBeamCursor));

    QGraphicsItem::hoverMoveEvent(event);
}

void DiagramItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    sizeGrip_->hideHandleItems();
    arrowManager_->hideHandleItems();

    if (QGuiApplication::overrideCursor())
        QGuiApplication::restoreOverrideCursor();

    QGraphicsItem::hoverLeaveEvent(event);
}

void DiagramItem::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_F2) {
        enableTextEditing();
    }

    QGraphicsItem::keyPressEvent(event);
}

void DiagramItem::enableTextEditing()
{
    textEditing_ = true;
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

ArrowManager *DiagramItem::arrowManager() const
{
    return arrowManager_;
}

void DiagramItem::addArrow(ArrowItem *arrow)
{
    arrowManager_->addArrow(arrow);
}

void DiagramItem::removeArrow(ArrowItem *arrow)
{
    arrowManager_->removeArrow(arrow);
}

void DiagramItem::removeArrows()
{
    arrowManager_->removeArrows();
}

void DiagramItem::updateArrows()
{
    arrowManager_->updateArrows();
}

const QSizeF &DiagramItem::size() const
{
    return size_;
}

void DiagramItem::setSize(const QSizeF &newSize)
{
    size_ = newSize;
}

QPainterPath DiagramItem::path() const
{
    return path_;
}

void DiagramItem::setPath(QPainterPath newPath)
{
    path_ = newPath;
}

DiagramItem::DiagramType DiagramItem::diagramType() const
{
    return diagramType_;
}

void DiagramItem::paint(QPainter *painter,
                        const QStyleOptionGraphicsItem *option,
                        QWidget *widget)
{
    Q_UNUSED(widget)

    bool selected = (option->state & QStyle::State_Selected);

    QColor polygonColor = (selected ? QColor(Qt::green) : QColor(Qt::black));
    int width = (selected ? Constants::DiagramItem::SelectedPenWidth :
                            Constants::DiagramItem::DefaultPenWidth);

    QPen pen(polygonColor);
    pen.setWidth(width);
    painter->setPen(pen);
    painter->setBrush(QBrush(Qt::white));
    painter->drawPath(path_);
}

QRectF DiagramItem::boundingRect() const
{
    return QRectF(-Constants::DiagramItem::SelectedPenWidth / 2,
                  -Constants::DiagramItem::SelectedPenWidth / 2,
                   size_.width()  + Constants::DiagramItem::SelectedPenWidth,
                   size_.height() + Constants::DiagramItem::SelectedPenWidth);
}

QRectF DiagramItem::pathBoundingRect() const
{
    return path_.boundingRect();
}

QPainterPath DiagramItem::shape() const
{
    return path_;
}

void DiagramItem::resize(const QSizeF &size)
{
    sizeGrip_->setRect(QRectF(0, 0, size.width(), size.height()));
}

void DiagramItem::resize(qreal width, qreal height)
{
    resize(QSizeF(width, height));
}

QPainterPath DiagramItem::getDefaultShape(DiagramType diagramType)
{
    int w = Constants::DiagramItem::DefaultWidth;
    int h = Constants::DiagramItem::DefaultHeight;
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
    QPixmap pixmap(Constants::DiagramItem::DefaultWidth,
                   Constants::DiagramItem::DefaultHeight);
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
