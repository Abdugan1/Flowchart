#include "handlemanager.h"
#include "diagramitem.h"

#include <QGraphicsItem>

HandleManager::HandleManager(QGraphicsItem* handlingItem, QObject *parent)
    : QObject(parent)
    , handlingItem_(handlingItem)
{
}

void HandleManager::hideHandleItems()
{
    for (auto appearArea : qAsConst(appearAreaItems_)) {
        auto handle = appearArea->handleItem();
        handle->hide();
    }
}

void HandleManager::showHandleItems()
{
    for (auto appearArea : qAsConst(appearAreaItems_)) {
        auto handle = appearArea->handleItem();
        handle->show();
    }
}

void HandleManager::addHandleItemAppearArea(HandleItemAppearArea *handle)
{
    appearAreaItems_.append(handle);
}

QGraphicsItem *HandleManager::handlingItem() const
{
    return handlingItem_;
}

const QList<HandleItemAppearArea *> &HandleManager::appearAreaItems() const
{
    return appearAreaItems_;
}
