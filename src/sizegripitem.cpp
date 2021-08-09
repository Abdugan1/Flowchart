#include "sizegripitem.h"
#include "handleitem.h"
#include "diagramscene.h"
#include "diagramitem.h"
#include "handleitemappeararea.h"

#include "constants.h"
#include "internal.h"

#include <QDebug>

using PositionFlags = HandleItem::PositionFlags;

SizeGripItem::SizeGripItem(DiagramItem *diagramItem, QObject *parent)
    : QObject(parent)
    , diagramItem_(diagramItem)
{
    rect_ = diagramItem->pathBoundingRect();

    handleItems_.append(new HandleItem(PositionFlags::TopLeft,      this));
    handleItems_.append(new HandleItem(PositionFlags::Top,          this));
    handleItems_.append(new HandleItem(PositionFlags::TopRight,     this));
    handleItems_.append(new HandleItem(PositionFlags::Right,        this));
    handleItems_.append(new HandleItem(PositionFlags::BottomRight,  this));
    handleItems_.append(new HandleItem(PositionFlags::Bottom,       this));
    handleItems_.append(new HandleItem(PositionFlags::BottomLeft,   this));
    handleItems_.append(new HandleItem(PositionFlags::Left,         this));

    updateHandleItemsPositions();
    hideHandleItems();
}

#define IMPL_SET_FN(TYPE, POS)                                      \
    void SizeGripItem::set ## POS (TYPE v)                          \
{                                                                   \
    QRectF oldRect = rect_;                                         \
    rect_.set ## POS (v);                                           \
    if (rect_.width() < Constants::DiagramItem::MinWidth            \
        || rect_.height() < Constants::DiagramItem::MinHeight) {    \
    rect_ = oldRect;                                                \
    return;                                                         \
    }                                                               \
    doResize();                                                     \
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
    resizeDiagramItem();
    emit resizeBeenMade();
    updateHandleItemsPositions();
}

void SizeGripItem::resizeDiagramItem()
{
    diagramItem_->setFlag(QGraphicsItem::ItemSendsGeometryChanges, false);
    QRectF itemRect = diagramItem_->pathBoundingRect();

    qreal sx = rect_.width()  / itemRect.width();
    qreal sy = rect_.height() / itemRect.height();

    QPainterPath oldPath = diagramItem_->path();
    QPainterPath newPath = oldPath * QTransform::fromScale(sx, sy);

    QRectF pathRect = newPath.boundingRect();

    qreal dx = rect_.x() - pathRect.x();
    qreal dy = rect_.y() - pathRect.y();

    diagramItem_->moveBy(dx, dy);
    diagramItem_->setPath(newPath);
    diagramItem_->setSize(QSizeF(rect_.width(), rect_.height()));

    rect_ = pathRect;
    diagramItem_->setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
}

void SizeGripItem::updateHandleItemsPositions()
{
    for (auto item : qAsConst(handleItems_)) {
        item->setFlag(QGraphicsItem::ItemSendsGeometryChanges, false);

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

        item->setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
    }
}

DiagramItem *SizeGripItem::diagramItem() const
{
    return diagramItem_;
}
