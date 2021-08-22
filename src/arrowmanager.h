#ifndef ARROWMANAGER_H
#define ARROWMANAGER_H

#include "handlemanager.h"

class ArrowHandleItem;
class DiagramItem;

class ArrowManager : public HandleManager
{
    Q_OBJECT

public:
    explicit ArrowManager(DiagramItem* diagramItem, QObject *parent = nullptr);

    void emitHandleClicked(const QPointF& mappedToScenePos);

signals:
    void handleClicked(const QPointF& mappedToScenePos);

public slots:
    void updateHandleItemsPositions();
};

#endif // ARROWMANAGER_H
