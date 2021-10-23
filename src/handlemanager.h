#ifndef HANDLEMANAGER_H
#define HANDLEMANAGER_H

#include <QtCore/QObject>
#include <QtCore/qglobal.h>

#include "handleitem.h"

class QGraphicsItem;

class HandleManager : public QObject
{
    Q_OBJECT

public:
    explicit HandleManager(QGraphicsItem* handlingItem, QObject *parent = nullptr);

    void setShouldDrawForHandleItems(bool shouldDraw);

    QGraphicsItem *handlingItem() const;

    const QList<HandleItem *> &handleItems() const;

protected:
    void addHandleItem(HandleItem* handle);

private:
    QList<HandleItem*> handleItems_;
    QGraphicsItem* handlingItem_;
};

#endif // HANDLEMANAGER_H
