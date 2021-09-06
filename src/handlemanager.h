#ifndef HANDLEMANAGER_H
#define HANDLEMANAGER_H

#include <QtCore/QObject>
#include <QtCore/qglobal.h>

#include "handleitemappeararea.h"

class QGraphicsItem;

class HandleManager : public QObject
{
    Q_OBJECT

public:
    explicit HandleManager(QGraphicsItem* handlingItem, QObject *parent = nullptr);

    void hideHandleItems();
    void showHandleItems();

    const QList<HandleItemAppearArea *> &appearAreaItems() const;

    QGraphicsItem *handlingItem() const;

protected:
    void addHandleItemAppearArea(HandleItemAppearArea* handle);

private:
    QList<HandleItemAppearArea*> appearAreaItems_;
    QGraphicsItem* handlingItem_;
};

#endif // HANDLEMANAGER_H
