#include "arrowmanageritem.h"
#include "arrowhandleitem.h"
#include "diagramitem.h"

#include "constants.h"

ArrowManagerItem::ArrowManagerItem(DiagramItem *diagramItem, QObject *parent)
    : QObject(parent)
    , diagramItem_(diagramItem)
{
    handleItems_.append(new ArrowHandleItem(ArrowHandleItem::Top,    diagramItem_));
    handleItems_.append(new ArrowHandleItem(ArrowHandleItem::Left,   diagramItem_));
    handleItems_.append(new ArrowHandleItem(ArrowHandleItem::Right,  diagramItem_));
    handleItems_.append(new ArrowHandleItem(ArrowHandleItem::Bottom, diagramItem_));

    updateHandleItemsPositions();
    hideHandleItems();
}


DiagramItem *ArrowManagerItem::diagramItem() const
{
    return diagramItem_;
}

void ArrowManagerItem::updateHandleItemsPositions()
{
    QRectF rect = diagramItem_->pathBoundingRect();

    for (auto item : qAsConst(handleItems_)) {

        switch (item->positionFlag()) {
        case ArrowHandleItem::Top:
            item->setPos(rect.left() + rect.width() / 2,
                         rect.top() - Constants::ArrowManagerItem::Margin);
            break;
        case ArrowHandleItem::Left:
            item->setPos(rect.left() - Constants::ArrowManagerItem::Margin,
                         rect.top() + rect.height() / 2);
            break;
        case ArrowHandleItem::Right:
            item->setPos(rect.right() + Constants::ArrowManagerItem::Margin,
                         rect.top() + rect.height() / 2);
            break;
        case ArrowHandleItem::Bottom:
            item->setPos(rect.left() + rect.width() / 2,
                         rect.bottom() + Constants::ArrowManagerItem::Margin);
            break;
        }

    }
}

void ArrowManagerItem::hideHandleItems()
{
    for (auto item : qAsConst(handleItems_))
        item->hide();
}

void ArrowManagerItem::showHandleItems()
{
    for (auto item : qAsConst(handleItems_))
        item->show();
}
