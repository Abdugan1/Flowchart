#include "sizegrip.h"
#include "sizehandleitem.h"
#include "diagramitem.h"
#include "handleitemappeararea.h"

#include "constants.h"
#include "internal.h"

#include <QDebug>

using PositionFlags = SizeHandleItem::PositionFlags;

SizeGrip::SizeGrip(QGraphicsItem *item, const QRectF &resizeRect, const QSizeF &minGripSize, QObject *parent)
    : HandleManager(item, parent)
    , resizeRect_(resizeRect)
    , minGripSize_(minGripSize)
{
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
    QRectF oldRect = resizeRect_;
    resizeRect_.setTop(y);
    if (resizeRect_.height() < minGripSize_.height()) {
        resizeRect_ = oldRect;
        return;
    }
    doResize();
}

void SizeGrip::setRight(qreal x)
{
    QRectF oldRect = resizeRect_;
    resizeRect_.setRight(x);
    if (resizeRect_.width() < minGripSize_.width()) {
        resizeRect_ = oldRect;
        return;
    }
    doResize();
}

void SizeGrip::setBottom(qreal y)
{
    QRectF oldRect = resizeRect_;
    resizeRect_.setBottom(y);
    if (resizeRect_.height() < minGripSize_.height()) {
        resizeRect_ = oldRect;
        return;
    }
    doResize();
}

void SizeGrip::setLeft(qreal v)
{
    QRectF oldRect = resizeRect_;
    resizeRect_.setLeft(v);
    if (resizeRect_.width() < minGripSize_.width()) {
        resizeRect_ = oldRect;
        return;
    }
    doResize();
}

void SizeGrip::setTopLeft(const QPointF &pos)
{
    QRectF oldRect = resizeRect_;
    resizeRect_.setTopLeft(pos);

    if (resizeRect_.width() < minGripSize_.width())
        resizeRect_.setLeft(oldRect.left());

    if (resizeRect_.height() < minGripSize_.height())
        resizeRect_.setTop(oldRect.top());

    if (resizeRect_ != oldRect)
        doResize();
}

void SizeGrip::setTopRight(const QPointF &pos)
{
    QRectF oldRect = resizeRect_;
    resizeRect_.setTopRight(pos);

    if (resizeRect_.width() < minGripSize_.width())
        resizeRect_.setRight(oldRect.right());

    if (resizeRect_.height() < minGripSize_.height())
        resizeRect_.setTop(oldRect.top());

    if (resizeRect_ != oldRect)
        doResize();
}

void SizeGrip::setBottomRight(const QPointF &pos)
{
    QRectF oldRect = resizeRect_;
    resizeRect_.setBottomRight(pos);

    if (resizeRect_.width() < minGripSize_.width())
        resizeRect_.setRight(oldRect.right());

    if (resizeRect_.height() < minGripSize_.height())
        resizeRect_.setBottom(oldRect.bottom());

    if (resizeRect_ != oldRect)
        doResize();
}

void SizeGrip::setBottomLeft(const QPointF &pos)
{
    QRectF oldRect = resizeRect_;
    resizeRect_.setBottomLeft(pos);

    if (resizeRect_.width() < minGripSize_.width())
        resizeRect_.setLeft(oldRect.left());

    if (resizeRect_.height() < minGripSize_.height())
        resizeRect_.setBottom(oldRect.bottom());

    if (resizeRect_ != oldRect)
        doResize();
}

void SizeGrip::resize(const QRectF &rect)
{
    resizeRect_ = rect;
    doResize();
}

void SizeGrip::doResize()
{
    resizeLogic();
    emit resizeBeenMade();
    updateHandleItemsPositions();
}

void SizeGrip::updateHandleItemsPositions()
{
    for (auto appearArea : qAsConst(HandleManager::appearAreaItems())) {

        auto sizeHandle = qgraphicsitem_cast<SizeHandleItem*>(appearArea->handleItem());

        switch (sizeHandle->positionFlags())
        {
        case PositionFlags::TopLeft:
            appearArea->setPos(resizeRect_.topLeft());
            break;
        case PositionFlags::Top:
            appearArea->setPos(resizeRect_.left() + resizeRect_.width() / 2,
                         resizeRect_.top());
            break;
        case PositionFlags::TopRight:
            appearArea->setPos(resizeRect_.topRight());
            break;
        case PositionFlags::Right:
            appearArea->setPos(resizeRect_.right(),
                         resizeRect_.top() + resizeRect_.height() / 2);
            break;
        case PositionFlags::BottomRight:
            appearArea->setPos(resizeRect_.bottomRight());
            break;
        case PositionFlags::Bottom:
            appearArea->setPos(resizeRect_.left() + resizeRect_.width() / 2,
                         resizeRect_.bottom());
            break;
        case PositionFlags::BottomLeft:
            appearArea->setPos(resizeRect_.bottomLeft());
            break;
        case PositionFlags::Left:
            appearArea->setPos(resizeRect_.left(),
                         resizeRect_.top() + resizeRect_.height() / 2);
            break;
        }

    }
}

const QSizeF &SizeGrip::minGripSize() const
{
    return minGripSize_;
}

void SizeGrip::setMinGripSize(const QSizeF &newMinGripSize)
{
    minGripSize_ = newMinGripSize;
}

const QRectF &SizeGrip::maxGripArea() const
{
    return maxGripArea_;
}

void SizeGrip::setMaxGripArea(const QRectF &newMaxGripArea)
{
    maxGripArea_ = newMaxGripArea;
}

const QRectF &SizeGrip::resizeRect() const
{
    return resizeRect_;
}

void SizeGrip::setResizeRect(const QRectF &newRect)
{
    resizeRect_ = newRect;
}
