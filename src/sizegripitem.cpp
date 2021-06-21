#include "sizegripitem.h"
#include "internal.h"
#include "diagramscene.h"
#include "diagramitem.h"

#include <QBrush>
#include <QCursor>
#include <QGuiApplication>
#include <QGraphicsSceneMouseEvent>

#include <QDebug>

SizeGripItem::HandleItem::HandleItem(int positionFlags, SizeGripItem* parent)
    : QGraphicsRectItem(-4, -4, 8, 8, parent),
      positionFlags_(positionFlags),
      parent_(parent)
{
    setBrush(QBrush(Qt::lightGray));
    setFlag(ItemIsMovable);
    setAcceptHoverEvents(true);
}

int SizeGripItem::HandleItem::positionFlags() const
{
    return positionFlags_;
}

void SizeGripItem::HandleItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    if (event->modifiers() != Qt::CTRL)
        changeCursor();
    QGraphicsRectItem::hoverEnterEvent(event);
}

void SizeGripItem::HandleItem::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    if (!QGuiApplication::overrideCursor() && event->modifiers() != Qt::CTRL)
        changeCursor();
    QGraphicsRectItem::hoverMoveEvent(event);
}

void SizeGripItem::HandleItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    QGuiApplication::restoreOverrideCursor();
    QGraphicsRectItem::hoverLeaveEvent(event);
}

void SizeGripItem::HandleItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QPointF pos = mapToParent(event->pos());
    pos = internal::getPointByStep(pos, DiagramScene::GridSize);
    pos = restrictPosition(pos);

    if (pos != this->pos())
        changeParentBoundingRect(pos);
}

QPointF SizeGripItem::HandleItem::restrictPosition(const QPointF& newPos)
{
    QPointF retVal = pos();

    if (positionFlags_ & Top || positionFlags_ & Bottom)
        retVal.setY(newPos.y());

    if (positionFlags_ & Left || positionFlags_ & Right)
        retVal.setX(newPos.x());

    return retVal;
}

void SizeGripItem::HandleItem::changeParentBoundingRect(const QPointF &pos)
{
    switch (positionFlags_)
    {
    case TopLeft:
        parent_->setTopLeft(pos);
        break;
    case Top:
        parent_->setTop(pos.y());
        break;
    case TopRight:
        parent_->setTopRight(pos);
        break;
    case Right:
        parent_->setRight(pos.x());
        break;
    case BottomRight:
        parent_->setBottomRight(pos);
        break;
    case Bottom:
        parent_->setBottom(pos.y());
        break;
    case BottomLeft:
        parent_->setBottomLeft(pos);
        break;
    case Left:
        parent_->setLeft(pos.x());
        break;
    }
}

void SizeGripItem::HandleItem::changeCursor()
{
    QCursor newCursor;
    switch (positionFlags_)
    {
    case TopLeft:
        newCursor.setShape(Qt::SizeFDiagCursor);
        break;
    case Top:
        newCursor.setShape(Qt::SizeVerCursor);
        break;
    case TopRight:
        newCursor.setShape(Qt::SizeBDiagCursor);
        break;
    case Right:
        newCursor.setShape(Qt::SizeHorCursor);
        break;
    case BottomLeft:
        newCursor.setShape(Qt::SizeBDiagCursor);
        break;
    case Bottom:
        newCursor.setShape(Qt::SizeVerCursor);
        break;
    case BottomRight:
        newCursor.setShape(Qt::SizeFDiagCursor);
        break;
    case Left:
        newCursor.setShape(Qt::SizeHorCursor);
        break;
    }
    QGuiApplication::setOverrideCursor(newCursor);
}

SizeGripItem::SizeGripItem(Resizer* resizer, QGraphicsItem* parent)
    : QGraphicsItem(parent),
      resizer_(resizer)
{
    DiagramItem* diagramItem = qgraphicsitem_cast<DiagramItem*>(parent);
    if (diagramItem)
        rect_ = diagramItem->polygonBoundingRect();

    handleItems_.append(new HandleItem(TopLeft, this));
    handleItems_.append(new HandleItem(Top, this));
    handleItems_.append(new HandleItem(TopRight, this));
    handleItems_.append(new HandleItem(Right, this));
    handleItems_.append(new HandleItem(BottomRight, this));
    handleItems_.append(new HandleItem(Bottom, this));
    handleItems_.append(new HandleItem(BottomLeft, this));
    handleItems_.append(new HandleItem(Left, this));
    updateHandleItemPositions();
}

SizeGripItem::~SizeGripItem()
{
    if (resizer_)
        delete resizer_;
}

QRectF SizeGripItem::boundingRect() const
{
    return rect_;
}

void SizeGripItem::paint(QPainter* painter,
                         const QStyleOptionGraphicsItem* option,
                         QWidget* widget)
{
    Q_UNUSED(painter)
    Q_UNUSED(option)
    Q_UNUSED(widget)
}

#define IMPL_SET_FN(TYPE, POS)                  \
    void SizeGripItem::set ## POS (TYPE v)      \
{                                           \
    rect_.set ## POS (v);                   \
    doResize();                             \
}

IMPL_SET_FN(qreal, Top)
IMPL_SET_FN(qreal, Right)
IMPL_SET_FN(qreal, Bottom)
IMPL_SET_FN(qreal, Left)
IMPL_SET_FN(const QPointF&, TopLeft)
IMPL_SET_FN(const QPointF&, TopRight)
IMPL_SET_FN(const QPointF&, BottomRight)
IMPL_SET_FN(const QPointF&, BottomLeft)

void SizeGripItem::doResize()
{
    if (resizer_)
    {
        (*resizer_)(parentItem(), rect_);
        emit itemResized();
        updateHandleItemPositions();
    }
}

void SizeGripItem::updateHandleItemPositions()
{
    foreach (HandleItem* item, handleItems_)
    {
        item->setFlag(ItemSendsGeometryChanges, false);

        switch (item->positionFlags())
        {
        case TopLeft:
            item->setPos(rect_.topLeft());
            break;
        case Top:
            item->setPos(rect_.left() + rect_.width() / 2,
                         rect_.top());
            break;
        case TopRight:
            item->setPos(rect_.topRight());
            break;
        case Right:
            item->setPos(rect_.right(),
                         rect_.top() + rect_.height() / 2);
            break;
        case BottomRight:
            item->setPos(rect_.bottomRight());
            break;
        case Bottom:
            item->setPos(rect_.left() + rect_.width() / 2,
                         rect_.bottom());
            break;
        case BottomLeft:
            item->setPos(rect_.bottomLeft());
            break;
        case Left:
            item->setPos(rect_.left(),
                         rect_.top() + rect_.height() / 2);
            break;
        }

        item->setFlag(ItemSendsGeometryChanges, true);
    }
}
