#include "handleitemappeararea.h"
#include "diagramitem.h"

#include "handlemanager.h"
#include "sizegrip.h"
#include "sizehandleitem.h"
#include "arrowmanager.h"
#include "arrowhandleitem.h"

#include "constants.h"

#include <QPainter>
#include <QDebug>

HandleItemAppearArea::HandleItemAppearArea(QGraphicsItem *handleItem, HandleManager *handleManager)
    : QGraphicsItem(handleManager->diagramItem())
{
    sizeGripItem_ = handleManager;
    handleItem_   = handleItem;
    handleItem_->setParentItem(this);

    if (auto sizeHandle = qgraphicsitem_cast<SizeHandleItem*>(handleItem))
        sizeHandle->setSizeGripItem(qobject_cast<SizeGrip*>(handleManager));
    else if (auto arrowHandle = qgraphicsitem_cast<ArrowHandleItem*>(handleItem))
        arrowHandle->setArrowManager(qobject_cast<ArrowManager*>(handleManager));



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

QGraphicsItem *HandleItemAppearArea::handleItem() const
{
    return handleItem_;
}
