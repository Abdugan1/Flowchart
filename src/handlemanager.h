#ifndef HANDLEMANAGER_H
#define HANDLEMANAGER_H

#include <QtCore/QObject>
#include <QtCore/qglobal.h>

#include "handleitemappeararea.h"

class QGraphicsItem;
class DiagramItem;

class HandleManager : public QObject
{
    Q_OBJECT

public:
    explicit HandleManager(DiagramItem* diagramItem, QObject *parent = nullptr);

    void hideHandleItems();
    void showHandleItems();

    DiagramItem *diagramItem() const;

    const QList<HandleItemAppearArea *> &appearAreaItems() const;

protected:
    void addHandleItemAppearArea(HandleItemAppearArea* handle);

private:
    QList<HandleItemAppearArea*> appearAreaItems_;
    DiagramItem* diagramItem_;
};

#endif // HANDLEMANAGER_H
