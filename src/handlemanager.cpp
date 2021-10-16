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
    for (auto handle : qAsConst(handleItems_))
        handle->hide();
}

void HandleManager::showHandleItems()
{
    for (auto handle : qAsConst(handleItems_))
        handle->show();
}

void HandleManager::addHandleItem(HandleItem *handle)
{
    handleItems_.append(handle);
}

const QList<HandleItem *> &HandleManager::handleItems() const
{
    return handleItems_;
}

QGraphicsItem *HandleManager::handlingItem() const
{
    return handlingItem_;
}
