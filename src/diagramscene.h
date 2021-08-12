#ifndef DIAGRAMSCENE_H
#define DIAGRAMSCENE_H

#include <QGraphicsScene>
#include "scenebuffer.h"

class DiagramItem;
class GraphicsItemGroup;

class DiagramScene : public QGraphicsScene
{
    Q_OBJECT

public:
    DiagramScene(QObject* parent = nullptr);

    QPointF preventOutsideMove(QPointF newPosTopLeft, QGraphicsItem* item);
    QPointF preventOutsideMove(QPointF newPosTopLeft, QPointF newPosBottomRight);

    DiagramItem* createDiagramItem(int diagramType);
    DiagramItem* createDiagramItem(const ItemProperties& itemProperties);

    QList<DiagramItem*> getDiagramItems() const;
    QList<ItemProperties> getDiagramItemsProperties(
            const QList<DiagramItem*>& diagramItems) const;

public slots:
    void drawPositionLines();
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

protected:
    void drawBackground(QPainter *painter, const QRectF &rect) override;

private:
    void deleteAllLines();
    void drawLevelLine(const QLineF& line);
    QPoint getItemCenter(const DiagramItem* item);
    void createGraphicsItemGroup(QList<DiagramItem*>& diagramItems);
    void setItemPosWithoutDrawingPositionLines(DiagramItem* item, const QPointF& pos);

    QPointF getMousePosMappedToScene() const;
    QPointF getPosThatItemCenterAtMousePos(const QPointF& mousePosition,
                                           const QGraphicsItem* item) const;

private:
    bool drawPositionLines_ = true;

    GraphicsItemGroup* group_ = nullptr;
    SceneBuffer buffer_;
};

#endif // DIAGRAMSCENE_H
