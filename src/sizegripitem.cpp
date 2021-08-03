#include "sizegripitem.h"
#include "handleitem.h"
#include "diagramscene.h"
#include "diagramitem.h"
#include "handleitemappeararea.h"

#include "internal.h"

#include <QDebug>

using PositionFlags = HandleItem::PositionFlags;

SizeGripItem::SizeGripItem(Resizer* resizer, QGraphicsItem* parent)
    : QGraphicsObject(parent),
      resizer_(resizer)
{
    DiagramItem* diagramItem = qgraphicsitem_cast<DiagramItem*>(parent);
    if (diagramItem)
        rect_ = diagramItem->pathBoundingRect();

    handleItems_.append(new HandleItem(PositionFlags::TopLeft,      this));
    handleItems_.append(new HandleItem(PositionFlags::Top,          this));
    handleItems_.append(new HandleItem(PositionFlags::TopRight,     this));
    handleItems_.append(new HandleItem(PositionFlags::Right,        this));
    handleItems_.append(new HandleItem(PositionFlags::BottomRight,  this));
    handleItems_.append(new HandleItem(PositionFlags::Bottom,       this));
    handleItems_.append(new HandleItem(PositionFlags::BottomLeft,   this));
    handleItems_.append(new HandleItem(PositionFlags::Left,         this));

    for (auto item : qAsConst(handleItems_)) {
        appearAreas_.append(new HandleItemAppearArea(item, this));
    }

    updateHandleItemsPositions();
    hideHandleItems();

    setAcceptHoverEvents(true);
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
{                                               \
    QRectF oldRect = rect_;                     \
    rect_.set ## POS (v);                       \
    if (rect_.width() < MinWidth                \
        || rect_.height() < MinHeight) {        \
    rect_ = oldRect;                            \
    return;                                     \
    }                                           \
    doResize();                                 \
}

IMPL_SET_FN(qreal, Top)
IMPL_SET_FN(qreal, Right)
IMPL_SET_FN(qreal, Bottom)
IMPL_SET_FN(qreal, Left)
IMPL_SET_FN(const QPointF&, TopLeft)
IMPL_SET_FN(const QPointF&, TopRight)
IMPL_SET_FN(const QPointF&, BottomRight)
IMPL_SET_FN(const QPointF&, BottomLeft)


void SizeGripItem::setRect(const QRectF &rect)
{
    rect_ = rect;
    doResize();
}

void SizeGripItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    showHandleItems();
    QGraphicsItem::hoverEnterEvent(event);
}

void SizeGripItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    hideHandleItems();
    QGraphicsItem::hoverLeaveEvent(event);
}

void SizeGripItem::hideHandleItems()
{
    for (auto item : qAsConst(handleItems_))
        item->hide();
}

void SizeGripItem::showHandleItems()
{
    for (auto item : qAsConst(handleItems_))
        item->show();
}

void SizeGripItem::doResize()
{
    if (resizer_)
    {
        (*resizer_)(parentItem(), rect_);
        emit resizeBeenMade();
        updateHandleItemsPositions();
    }
}

void SizeGripItem::updateHandleItemsPositions()
{
    for (auto appearArea : qAsConst(appearAreas_)) {
        HandleItem* item = appearArea->handleItem();

        item->setFlag(ItemSendsGeometryChanges, false);

        switch (item->positionFlags())
        {
        case PositionFlags::TopLeft:
            item->setPos(rect_.topLeft());
            appearArea->setPos(rect_.topLeft());
            break;
        case PositionFlags::Top:
            item->setPos(rect_.left() + rect_.width() / 2,
                         rect_.top());
            appearArea->setPos(rect_.left() + rect_.width() / 2,
                         rect_.top());
            break;
        case PositionFlags::TopRight:
            item->setPos(rect_.topRight());
            appearArea->setPos(rect_.topRight());
            break;
        case PositionFlags::Right:
            item->setPos(rect_.right(),
                         rect_.top() + rect_.height() / 2);
            appearArea->setPos(rect_.right(),
                         rect_.top() + rect_.height() / 2);
            break;
        case PositionFlags::BottomRight:
            item->setPos(rect_.bottomRight());
            appearArea->setPos(rect_.bottomRight());
            break;
        case PositionFlags::Bottom:
            item->setPos(rect_.left() + rect_.width() / 2,
                         rect_.bottom());
            appearArea->setPos(rect_.left() + rect_.width() / 2,
                         rect_.bottom());
            break;
        case PositionFlags::BottomLeft:
            item->setPos(rect_.bottomLeft());
            appearArea->setPos(rect_.bottomLeft());
            break;
        case PositionFlags::Left:
            item->setPos(rect_.left(),
                         rect_.top() + rect_.height() / 2);
            appearArea->setPos(rect_.left(),
                         rect_.top() + rect_.height() / 2);
            break;
        }

        item->setFlag(ItemSendsGeometryChanges, true);
    }
}
