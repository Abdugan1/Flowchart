#include "handlemanager.h"
#include "diagramitem.h"

#include <QGraphicsItem>

HandleManager::HandleManager(DiagramItem* diagramItem, QObject *parent)
    : QObject(parent)
    , diagramItem_(diagramItem)
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

const QList<HandleItemAppearArea *> &HandleManager::appearAreaItems() const
{
    return appearAreaItems_;
}

DiagramItem *HandleManager::diagramItem() const
{
    return diagramItem_;
}
