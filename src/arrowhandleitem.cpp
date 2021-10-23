#include "arrowhandleitem.h"
#include "arrowmanager.h"

#include "constants.h"

#include <QDebug>
#include <QPainter>
#include <QCursor>
#include <QStyleOptionGraphicsItem>
#include <QGraphicsSceneMouseEvent>

ArrowHandleItem::ArrowHandleItem(PositionFlags positionFlag, ArrowManager *arrowManager)
    : HandleItem(positionFlag, arrowManager)
    , boundingRect_(-Constants::ArrowHandleItem::OverralWidth  / 2,
                    -Constants::ArrowHandleItem::OverralHeight / 2,
                     Constants::ArrowHandleItem::OverralWidth,
                     Constants::ArrowHandleItem::OverralHeight)
    , arrowManager_(arrowManager)
{
    Q_ASSERT(arrowManager_);
    setCursor(QCursor(Qt::PointingHandCursor));
    setAcceptHoverEvents(true);
}

void ArrowHandleItem::setHandleManager(HandleManager *newHandleManager)
{
    arrowManager_ = qobject_cast<ArrowManager*>(newHandleManager);
    HandleItem::setHandleManager(newHandleManager);
}

QRectF ArrowHandleItem::boundingRect() const
{
    return boundingRect_;
}

void ArrowHandleItem::paint(QPainter *painter,
                            const QStyleOptionGraphicsItem *option,
                            QWidget *widget)
{
    Q_UNUSED(widget)

    if (shouldDraw()) {

        drawMainCircle(painter);

        if (option->state & QStyle::State_MouseOver)
            drawOuterCircle(painter);
    }
}

int ArrowHandleItem::type() const
{
    return Type;
}

void ArrowHandleItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    Q_ASSERT(arrowManager_);
    arrowManager_->emitHandleClicked(this);
    HandleItem::mousePressEvent(event);
}

void ArrowHandleItem::drawMainCircle(QPainter *painter)
{
    painter->save();
    painter->setPen(Qt::NoPen);
    painter->setBrush(QBrush(Qt::darkGray));
    painter->drawEllipse(-Constants::ArrowHandleItem::InnerCircleDiameter / 2,
                         -Constants::ArrowHandleItem::InnerCircleDiameter / 2,
                          Constants::ArrowHandleItem::InnerCircleDiameter,
                          Constants::ArrowHandleItem::InnerCircleDiameter);
    painter->restore();
}

void ArrowHandleItem::drawOuterCircle(QPainter *painter)
{
    painter->save();
    painter->setPen(QPen(Qt::green, Constants::ArrowHandleItem::OuterStrokePenWidth));
    painter->setBrush(Qt::NoBrush);
    painter->drawEllipse(-Constants::ArrowHandleItem::OuterStrokeDiameter / 2,
                         -Constants::ArrowHandleItem::OuterStrokeDiameter / 2,
                          Constants::ArrowHandleItem::OuterStrokeDiameter,
                          Constants::ArrowHandleItem::OuterStrokeDiameter);
    painter->restore();
}
