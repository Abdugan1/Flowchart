#include "sizegripitem.h"
#include "handleitem.h"
#include "diagramscene.h"
#include "diagramitem.h"

#include "internal.h"

using PositionFlags = HandleItem::PositionFlags;

SizeGripItem::SizeGripItem(Resizer* resizer, QGraphicsItem* parent)
    : QGraphicsItem(parent),
      resizer_(resizer)
{
    DiagramItem* diagramItem = qgraphicsitem_cast<DiagramItem*>(parent);
    if (diagramItem)
        rect_ = diagramItem->polygonBoundingRect();

    handleItems_.append(new HandleItem(PositionFlags::TopLeft,      this));
    handleItems_.append(new HandleItem(PositionFlags::Top,          this));
    handleItems_.append(new HandleItem(PositionFlags::TopRight,     this));
    handleItems_.append(new HandleItem(PositionFlags::Right,        this));
    handleItems_.append(new HandleItem(PositionFlags::BottomRight,  this));
    handleItems_.append(new HandleItem(PositionFlags::Bottom,       this));
    handleItems_.append(new HandleItem(PositionFlags::BottomLeft,   this));
    handleItems_.append(new HandleItem(PositionFlags::Left,         this));
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
        case PositionFlags::TopLeft:
            item->setPos(rect_.topLeft());
            break;
        case PositionFlags::Top:
            item->setPos(rect_.left() + rect_.width() / 2,
                         rect_.top());
            break;
        case PositionFlags::TopRight:
            item->setPos(rect_.topRight());
            break;
        case PositionFlags::Right:
            item->setPos(rect_.right(),
                         rect_.top() + rect_.height() / 2);
            break;
        case PositionFlags::BottomRight:
            item->setPos(rect_.bottomRight());
            break;
        case PositionFlags::Bottom:
            item->setPos(rect_.left() + rect_.width() / 2,
                         rect_.bottom());
            break;
        case PositionFlags::BottomLeft:
            item->setPos(rect_.bottomLeft());
            break;
        case PositionFlags::Left:
            item->setPos(rect_.left(),
                         rect_.top() + rect_.height() / 2);
            break;
        }

        item->setFlag(ItemSendsGeometryChanges, true);
    }
}
