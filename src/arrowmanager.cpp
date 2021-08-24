#include "arrowmanager.h"
#include "arrowhandleitem.h"
#include "handleitemappeararea.h"
#include "diagramitem.h"

#include "constants.h"

ArrowManager::ArrowManager(DiagramItem *diagramItem, QObject *parent)
    : HandleManager(diagramItem, parent)
{
    using HIAA = HandleItemAppearArea;

    HandleManager::addHandleItemAppearArea(new HIAA(new ArrowHandleItem(ArrowHandleItem::Top,    diagramItem), this));
    HandleManager::addHandleItemAppearArea(new HIAA(new ArrowHandleItem(ArrowHandleItem::Left,   diagramItem), this));
    HandleManager::addHandleItemAppearArea(new HIAA(new ArrowHandleItem(ArrowHandleItem::Right,  diagramItem), this));
    HandleManager::addHandleItemAppearArea(new HIAA(new ArrowHandleItem(ArrowHandleItem::Bottom, diagramItem), this));

    updateHandleItemsPositions();
    hideHandleItems();
}

void ArrowManager::emitHandleClicked(ArrowHandleItem *handleItem)
{
    emit handleClicked(handleItem, diagramItem());
}

void ArrowManager::updateHandleItemsPositions()
{
    DiagramItem* diagramItem = HandleManager::diagramItem();

    QRectF rect = diagramItem->pathBoundingRect();

    for (auto appearArea : qAsConst(HandleManager::appearAreaItems())) {
        auto arrowHandle = qgraphicsitem_cast<ArrowHandleItem*>(appearArea->handleItem());

        switch (arrowHandle->positionFlag()) {
        case ArrowHandleItem::Top:
            appearArea->setPos(rect.left() + rect.width() / 2,
                         rect.top() - Constants::ArrowManagerItem::Margin);
            break;
        case ArrowHandleItem::Left:
            appearArea->setPos(rect.left() - Constants::ArrowManagerItem::Margin,
                         rect.top() + rect.height() / 2);
            break;
        case ArrowHandleItem::Right:
            appearArea->setPos(rect.right() + Constants::ArrowManagerItem::Margin,
                         rect.top() + rect.height() / 2);
            break;
        case ArrowHandleItem::Bottom:
            appearArea->setPos(rect.left() + rect.width() / 2,
                         rect.bottom() + Constants::ArrowManagerItem::Margin);
            break;
        }

    }
}
