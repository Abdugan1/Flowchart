#ifndef ARROWMANAGERITEM_H
#define ARROWMANAGERITEM_H

#include <QtCore/QObject>
#include <QtCore/qglobal.h>

class ArrowHandleItem;
class DiagramItem;

class ArrowManagerItem : public QObject
{
    Q_OBJECT

public:
    explicit ArrowManagerItem(DiagramItem* diagramItem, QObject *parent = nullptr);

    DiagramItem *diagramItem() const;

public slots:
    void updateHandleItemsPositions();
    void hideHandleItems();
    void showHandleItems();

private:
    QList<ArrowHandleItem*> handleItems_;
    DiagramItem* diagramItem_;
};

#endif // ARROWMANAGERITEM_H
