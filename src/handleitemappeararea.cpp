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
    , handleManager_(handleManager)
    , handleItem_(handleItem)
{
    setAcceptHoverEvents(true);
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
        handleManager_->showHandleItems();
    }
    QGraphicsItem::hoverMoveEvent(event);
}

void HandleItemAppearArea::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    handleManager_->hideHandleItems();
    QGraphicsItem::hoverLeaveEvent(event);
}

const QRectF &HandleItemAppearArea::appearArea() const
{
    return appearArea_;
}

void HandleItemAppearArea::setAppearArea(const QRectF &newAppearArea)
{
    prepareGeometryChange();
    appearArea_ = newAppearArea;
}

HandleItem *HandleItemAppearArea::handleItem() const
{
    return handleItem_;
}
