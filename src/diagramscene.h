#ifndef DIAGRAMSCENE_H
#define DIAGRAMSCENE_H

#include <QGraphicsScene>
#include "scenebuffer.h"

class DiagramItem;
class GraphicsItemGroup;
class ArrowHandleItem;
class ArrowItem;
class SceneBoundary;

class DiagramScene : public QGraphicsScene
{
    Q_OBJECT

public:
    DiagramScene(QObject* parent = nullptr);

    DiagramItem* createDiagramItem(int diagramType);
    DiagramItem* createDiagramItem(const ItemProperties& itemProperties);

    void addDiagramItem(DiagramItem* diagramItem);
    void removeDiagramItem(DiagramItem* diagramItem);

    QImage toImage();

    void clearAllSelection();

    QList<DiagramItem *> getDiagramItems(Qt::SortOrder order) const;
    const QList<DiagramItem *> &diagramItems() const;

    QRectF boundary() const;

signals:
    void diagramItemAddedOrRemoved();

public slots:
    void addPositionLines();
    void deleteAllPositionLines();
    void selectAllItems();
    void destroyGraphicsItemGroup();
    void selectAndMakeGroup(const QRectF& rect);
    void deleteSelectedItems();
    void deleteItems(const QList<QGraphicsItem*>& items);
    void copySelectedItems();
    void copyItems(const QList<QGraphicsItem*>& items);
    void pasteItemsToMousePos();
    void pasteItems(const QPointF& posToPaste);
    void clearScene();
    void editSceneBoundary();

private slots:
    void onHandleClicked(ArrowHandleItem* handle,
                         DiagramItem* item);

    void updateMaxGripAreaOfDiagramItems();

protected:
    void drawBackground(QPainter *painter, const QRectF &rect) override;

private:
    void addPositionLine(const QLineF& line);
    void createGraphicsItemGroup(const QList<DiagramItem*>& diagramItems);
    void setItemPosWithoutAddingPositionLines(DiagramItem* item, const QPointF& pos);

    QPointF getMousePosMappedToScene() const;

private:
    bool addPositionLines_ = true;

    QList<DiagramItem*> diagramItems_;
    GraphicsItemGroup* group_ = nullptr;

    SceneBuffer buffer_;
    SceneBoundary* sceneBoundary_ = nullptr;

    ArrowItem* arrow_ = nullptr;
};

#endif // DIAGRAMSCENE_H
