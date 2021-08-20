#include "arrowhandleitem.h"

#include "constants.h"

#include <QPainter>
#include <QCursor>
#include <QStyleOptionGraphicsItem>

using PositionFlags = ArrowHandleItem::PositionFlags;

ArrowHandleItem::ArrowHandleItem(PositionFlags positionFlag, QGraphicsItem *parent)
    : QGraphicsEllipseItem(-Constants::ArrowHandleItem::OverralWidth  / 2,
                           -Constants::ArrowHandleItem::OverralHeight / 2,
                            Constants::ArrowHandleItem::OverralWidth,
                            Constants::ArrowHandleItem::OverralHeight,
                            parent)
    , positionFlag_(positionFlag)
{
    setCursor(QCursor(Qt::PointingHandCursor));
    setAcceptHoverEvents(true);
}

void ArrowHandleItem::paint(QPainter *painter,
                            const QStyleOptionGraphicsItem *option,
                            QWidget *widget)
{
    Q_UNUSED(widget)

    painter->setPen(Qt::NoPen);
    painter->setBrush(QBrush(Qt::darkGray));
    painter->drawEllipse(-Constants::ArrowHandleItem::InnerCircleDiameter / 2,
                         -Constants::ArrowHandleItem::InnerCircleDiameter / 2,
                          Constants::ArrowHandleItem::InnerCircleDiameter,
                          Constants::ArrowHandleItem::InnerCircleDiameter);

    if (option->state & QStyle::State_MouseOver) {
        painter->setPen(QPen(Qt::green, Constants::ArrowHandleItem::OuterStrokePenWidth));
        painter->setBrush(Qt::NoBrush);
        painter->drawEllipse(-Constants::ArrowHandleItem::OuterStrokeDiameter / 2,
                             -Constants::ArrowHandleItem::OuterStrokeDiameter / 2,
                              Constants::ArrowHandleItem::OuterStrokeDiameter,
                              Constants::ArrowHandleItem::OuterStrokeDiameter);
    }
}

PositionFlags ArrowHandleItem::positionFlag() const
{
    return positionFlag_;
}
