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
    enum DefaultSize {
        Width = 645,
        Height = 975
    };

    enum {
        GridSize = 20
    };

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
    void drawLevelLineWithItemOnSameAxis(const QPointF& pos);
    void deleteAllLevelLines();
    void selectAllItems();
    void clearScene();
    void destroyGraphicsItemGroup();
    void makeGroupOfSelectedItems();
    void deleteSelectedItems();
    void copySelectedItems();
    void pasteCopiedItems();

protected:
    void drawBackground(QPainter *painter, const QRectF &rect) override;

private:
    void deleteAllLines();
    void drawLevelLine(const QLineF& line);
    QPoint getItemCenter(const DiagramItem* item);
    void createGraphicsItemGroup(QList<DiagramItem*>& diagramItems);

    QPointF getMousePosMappedToScene() const;
    QPointF getPosThatItemCenterAtMousePos(const QPointF& mousePosition,
                                           const QGraphicsItem* item) const;

private:
    GraphicsItemGroup* group_ = nullptr;
    SceneBuffer buffer_;
};

#endif // DIAGRAMSCENE_H
