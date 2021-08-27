#ifndef ARROWMANAGER_H
#define ARROWMANAGER_H

#include "handlemanager.h"

class ArrowHandleItem;
class DiagramItem;
class ArrowItem;

class ArrowManager : public HandleManager
{
    Q_OBJECT

public:
    explicit ArrowManager(DiagramItem* diagramItem, QObject *parent = nullptr);

    void emitHandleClicked(ArrowHandleItem* handleItem);

    void addArrow(ArrowItem* arrow);
    void removeArrow(ArrowItem* arrow);
    void removeArrows();
    const QList<ArrowItem *> &arrows() const;
    void updateArrows();

signals:
    void handleClicked(ArrowHandleItem* handleItem,
                       DiagramItem* item);

public slots:
    void updateHandleItemsPositions();

private:
    QList<ArrowItem*> arrows_;

};

#endif // ARROWMANAGER_H
