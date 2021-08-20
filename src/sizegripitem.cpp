#include "sizegripitem.h"
#include "sizehandleitem.h"
#include "diagramitem.h"
#include "handleitemappeararea.h"

#include "constants.h"
#include "internal.h"

using PositionFlags = SizeHandleItem::PositionFlags;

SizeGripItem::SizeGripItem(DiagramItem *diagramItem, QObject *parent)
    : QObject(parent)
    , diagramItem_(diagramItem)
{
    rect_ = diagramItem->pathBoundingRect();

    handleItems_.append(new HandleItemAppearArea(new SizeHandleItem(PositionFlags::TopLeft    ), this));
    handleItems_.append(new HandleItemAppearArea(new SizeHandleItem(PositionFlags::Top        ), this));
    handleItems_.append(new HandleItemAppearArea(new SizeHandleItem(PositionFlags::TopRight   ), this));
    handleItems_.append(new HandleItemAppearArea(new SizeHandleItem(PositionFlags::Right      ), this));
    handleItems_.append(new HandleItemAppearArea(new SizeHandleItem(PositionFlags::BottomRight), this));
    handleItems_.append(new HandleItemAppearArea(new SizeHandleItem(PositionFlags::Bottom     ), this));
    handleItems_.append(new HandleItemAppearArea(new SizeHandleItem(PositionFlags::BottomLeft ), this));
    handleItems_.append(new HandleItemAppearArea(new SizeHandleItem(PositionFlags::Left       ), this));

    updateHandleItemsPositions();
    hideHandleItems();
}

void SizeGripItem::setTop(qreal y)
{
    QRectF oldRect = rect_;
    rect_.setTop(y);
    if (rect_.height() < Constants::DiagramItem::MinHeight) {
        rect_ = oldRect;
        return;
    }
    doResize();
}

void SizeGripItem::setRight(qreal x)
{
    QRectF oldRect = rect_;
    rect_.setRight(x);
    if (rect_.width() < Constants::DiagramItem::MinWidth) {
        rect_ = oldRect;
        return;
    }
    doResize();
}

void SizeGripItem::setBottom(qreal y)
{
    QRectF oldRect = rect_;
    rect_.setBottom(y);
    if (rect_.height() < Constants::DiagramItem::MinHeight) {
        rect_ = oldRect;
        return;
    }
    doResize();
}

void SizeGripItem::setLeft(qreal v)
{
    QRectF oldRect = rect_;
    rect_.setLeft(v);
    if (rect_.width() < Constants::DiagramItem::MinWidth) {
        rect_ = oldRect;
        return;
    }
    doResize();
}

void SizeGripItem::setTopLeft(const QPointF &pos)
{
    QRectF oldRect = rect_;
    rect_.setTopLeft(pos);

    if (rect_.width() < Constants::DiagramItem::MinWidth)
        rect_.setLeft(oldRect.left());

    if (rect_.height() < Constants::DiagramItem::MinHeight)
        rect_.setTop(oldRect.top());

    if (rect_ != oldRect)
        doResize();
}

void SizeGripItem::setTopRight(const QPointF &pos)
{
    QRectF oldRect = rect_;
    rect_.setTopRight(pos);

    if (rect_.width() < Constants::DiagramItem::MinWidth)
        rect_.setRight(oldRect.right());

    if (rect_.height() < Constants::DiagramItem::MinHeight)
        rect_.setTop(oldRect.top());

    if (rect_ != oldRect)
        doResize();
}

void SizeGripItem::setBottomRight(const QPointF &pos)
{
    QRectF oldRect = rect_;
    rect_.setBottomRight(pos);

    if (rect_.width() < Constants::DiagramItem::MinWidth)
        rect_.setRight(oldRect.right());

    if (rect_.height() < Constants::DiagramItem::MinHeight)
        rect_.setBottom(oldRect.bottom());

    if (rect_ != oldRect)
        doResize();
}

void SizeGripItem::setBottomLeft(const QPointF &pos)
{
    QRectF oldRect = rect_;
    rect_.setBottomLeft(pos);

    if (rect_.width() < Constants::DiagramItem::MinWidth)
        rect_.setLeft(oldRect.left());

    if (rect_.height() < Constants::DiagramItem::MinHeight)
        rect_.setBottom(oldRect.bottom());

    if (rect_ != oldRect)
        doResize();
}

void SizeGripItem::setRect(const QRectF &rect)
{
    rect_ = rect;
    doResize();
}

void SizeGripItem::hideHandleItems()
{
    for (auto item : qAsConst(handleItems_))
        item->handleItem()->hide();
}

void SizeGripItem::showHandleItems()
{
    for (auto item : qAsConst(handleItems_))
        item->handleItem()->show();
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

        switch (item->handleItem()->positionFlags())
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
