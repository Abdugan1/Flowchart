#include "sizegrip.h"
#include "sizehandleitem.h"
#include "diagramitem.h"
#include "handleitemappeararea.h"

#include "constants.h"
#include "internal.h"

using PositionFlags = SizeHandleItem::PositionFlags;

SizeGrip::SizeGrip(DiagramItem *diagramItem, QObject *parent)
    : HandleManager(diagramItem, parent)
{
    rect_ = diagramItem->pathBoundingRect();

    using HIAA = HandleItemAppearArea;

    HandleManager::addHandleItemAppearArea(new HIAA(new SizeHandleItem(PositionFlags::TopLeft    ), this));
    HandleManager::addHandleItemAppearArea(new HIAA(new SizeHandleItem(PositionFlags::Top        ), this));
    HandleManager::addHandleItemAppearArea(new HIAA(new SizeHandleItem(PositionFlags::TopRight   ), this));
    HandleManager::addHandleItemAppearArea(new HIAA(new SizeHandleItem(PositionFlags::Left       ), this));
    HandleManager::addHandleItemAppearArea(new HIAA(new SizeHandleItem(PositionFlags::Right      ), this));
    HandleManager::addHandleItemAppearArea(new HIAA(new SizeHandleItem(PositionFlags::BottomLeft ), this));
    HandleManager::addHandleItemAppearArea(new HIAA(new SizeHandleItem(PositionFlags::Bottom     ), this));
    HandleManager::addHandleItemAppearArea(new HIAA(new SizeHandleItem(PositionFlags::BottomRight), this));

    updateHandleItemsPositions();
    hideHandleItems();
}

void SizeGrip::setTop(qreal y)
{
    QRectF oldRect = rect_;
    rect_.setTop(y);
    if (rect_.height() < Constants::DiagramItem::MinHeight) {
        rect_ = oldRect;
        return;
    }
    doResize();
}

void SizeGrip::setRight(qreal x)
{
    QRectF oldRect = rect_;
    rect_.setRight(x);
    if (rect_.width() < Constants::DiagramItem::MinWidth) {
        rect_ = oldRect;
        return;
    }
    doResize();
}

void SizeGrip::setBottom(qreal y)
{
    QRectF oldRect = rect_;
    rect_.setBottom(y);
    if (rect_.height() < Constants::DiagramItem::MinHeight) {
        rect_ = oldRect;
        return;
    }
    doResize();
}

void SizeGrip::setLeft(qreal v)
{
    QRectF oldRect = rect_;
    rect_.setLeft(v);
    if (rect_.width() < Constants::DiagramItem::MinWidth) {
        rect_ = oldRect;
        return;
    }
    doResize();
}

void SizeGrip::setTopLeft(const QPointF &pos)
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

void SizeGrip::setTopRight(const QPointF &pos)
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

void SizeGrip::setBottomRight(const QPointF &pos)
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

void SizeGrip::setBottomLeft(const QPointF &pos)
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

void SizeGrip::setRect(const QRectF &rect)
{
    rect_ = rect;
    doResize();
}

void SizeGrip::doResize()
{
    resizeDiagramItem();
    emit resizeBeenMade();
    updateHandleItemsPositions();
}

void SizeGrip::resizeDiagramItem()
{
    DiagramItem* diagramItem = HandleManager::diagramItem();

    diagramItem->setFlag(QGraphicsItem::ItemSendsGeometryChanges, false);
    QRectF itemRect = diagramItem->pathBoundingRect();

    qreal sx = rect_.width()  / itemRect.width();
    qreal sy = rect_.height() / itemRect.height();

    QPainterPath oldPath = diagramItem->path();
    QPainterPath newPath = oldPath * QTransform::fromScale(sx, sy);

    QRectF pathRect = newPath.boundingRect();

    qreal dx = rect_.x() - pathRect.x();
    qreal dy = rect_.y() - pathRect.y();

    diagramItem->prepareGeomChange();
    diagramItem->moveBy(dx, dy);
    diagramItem->setPath(newPath);
    diagramItem->setSize(QSizeF(rect_.width(), rect_.height()));

    rect_ = pathRect;
    diagramItem->setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
}

void SizeGrip::updateHandleItemsPositions()
{
    for (auto appearArea : qAsConst(HandleManager::appearAreaItems())) {

        auto sizeHandle = qgraphicsitem_cast<SizeHandleItem*>(appearArea->handleItem());

        switch (sizeHandle->positionFlags())
        {
        case PositionFlags::TopLeft:
            appearArea->setPos(rect_.topLeft());
            break;
        case PositionFlags::Top:
            appearArea->setPos(rect_.left() + rect_.width() / 2,
                         rect_.top());
            break;
        case PositionFlags::TopRight:
            appearArea->setPos(rect_.topRight());
            break;
        case PositionFlags::Right:
            appearArea->setPos(rect_.right(),
                         rect_.top() + rect_.height() / 2);
            break;
        case PositionFlags::BottomRight:
            appearArea->setPos(rect_.bottomRight());
            break;
        case PositionFlags::Bottom:
            appearArea->setPos(rect_.left() + rect_.width() / 2,
                         rect_.bottom());
            break;
        case PositionFlags::BottomLeft:
            appearArea->setPos(rect_.bottomLeft());
            break;
        case PositionFlags::Left:
            appearArea->setPos(rect_.left(),
                         rect_.top() + rect_.height() / 2);
            break;
        }

    }
}
