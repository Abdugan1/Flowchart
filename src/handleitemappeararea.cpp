#include "handleitemappeararea.h"
#include "diagramitem.h"

#include "handlemanager.h"
#include "sizegripdiagramitem.h"
#include "sizehandleitem.h"
#include "arrowmanager.h"
#include "arrowhandleitem.h"

#include "constants.h"

#include <QPainter>
#include <QDebug>

HandleItemAppearArea::HandleItemAppearArea(HandleItem *handleItem, HandleManager *handleManager)
    : QGraphicsItem(handleManager->handlingItem())
{
    sizeGripItem_ = handleManager;

    handleItem->setParentItem(this);
    handleItem->setHandleManager(handleManager);

#ifndef QT_NO_QDEBUG
    if (qgraphicsitem_cast<SizeHandleItem*>(handleItem)) {
        Q_ASSERT_X(qobject_cast<SizeGrip*>(handleItem->handleManager()),
                   "setHandleManager", "SizeGrip was not installed");
    } else {
        Q_ASSERT_X(qobject_cast<ArrowManager*>(handleItem->handleManager()),
                   "setHandleManager", "ArrowManager was not installed");
    }
#endif

    handleItem_ = handleItem;

    setAcceptHoverEvents(true);

    appearArea_ = handleItem_->boundingRect();

    appearArea_.setX(appearArea_.x() - Constants::HandleItemAppearArea::Margin);
    appearArea_.setY(appearArea_.y() - Constants::HandleItemAppearArea::Margin);
    appearArea_.setWidth(appearArea_.width()   + Constants::HandleItemAppearArea::Margin);
    appearArea_.setHeight(appearArea_.height() + Constants::HandleItemAppearArea::Margin);
}

QRectF HandleItemAppearArea::boundingRect() const
{
    return appearArea_;
}

void HandleItemAppearArea::paint(QPainter *painter,
                                 const QStyleOptionGraphicsItem *option,
                                 QWidget *widget)
{
    Q_UNUSED(painter)
    Q_UNUSED(option)
    Q_UNUSED(widget)
}

int HandleItemAppearArea::type() const
{
    return Type;
}

void HandleItemAppearArea::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    if (!handleItem_->isVisible()) {
        sizeGripItem_->showHandleItems();
    }
    QGraphicsItem::hoverMoveEvent(event);
}

void HandleItemAppearArea::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    sizeGripItem_->hideHandleItems();
    QGraphicsItem::hoverLeaveEvent(event);
}

HandleItem *HandleItemAppearArea::handleItem() const
{
    return handleItem_;
}
