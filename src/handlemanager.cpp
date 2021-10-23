#include "handlemanager.h"
#include "diagramitem.h"

#include <QGraphicsItem>
#include <QDebug>

HandleManager::HandleManager(QGraphicsItem* handlingItem, QObject *parent)
    : QObject(parent)
    , handlingItem_(handlingItem)
{
}

void HandleManager::setShouldDrawForHandleItems(bool shouldDraw)
{
    for (auto handle : qAsConst(handleItems_))
        handle->setShouldDraw(shouldDraw);
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
