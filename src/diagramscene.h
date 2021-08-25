#ifndef DIAGRAMSCENE_H
#define DIAGRAMSCENE_H

#include <QGraphicsScene>
#include "scenebuffer.h"
#include "arrowconnector.h"

class DiagramItem;
class GraphicsItemGroup;
class Node;
class ArrowHandleItem;
class ArrowItem;

class DiagramScene : public QGraphicsScene
{
    Q_OBJECT

public:
    enum Mode {
        Normal,
        Line,
    };

public:
    DiagramScene(QObject* parent = nullptr);

    DiagramItem* createDiagramItem(int diagramType);
    DiagramItem* createDiagramItem(const ItemProperties& itemProperties);

    QList<DiagramItem*> getDiagramItems(Qt::SortOrder order = Qt::DescendingOrder) const;

    QImage toImage();

    void clearAllSelection();

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

private slots:
    void onHandleClicked(ArrowHandleItem* handle,
                         DiagramItem* item);

protected:
    void drawBackground(QPainter *painter, const QRectF &rect) override;

private:
    void deleteAllLines();
    void drawLevelLine(const QLineF& line);
    QPoint getItemCenter(const DiagramItem* item) const;
    void createGraphicsItemGroup(QList<DiagramItem*>& diagramItems);
    void setItemPosWithoutDrawingPositionLines(DiagramItem* item, const QPointF& pos);

    QPointF getMousePosMappedToScene() const;
    QPointF getPosThatItemCenterAtMousePos(const QPointF& mousePosition,
                                           const QGraphicsItem* item) const;

private:
    bool drawPositionLines_ = true;

    GraphicsItemGroup* group_ = nullptr;
    SceneBuffer buffer_;
    Mode mode_;
    QGraphicsLineItem* line_ = nullptr;

    ArrowItem* arrow_ = nullptr;
    ArrowConnector arrowConnector_;
};

#endif // DIAGRAMSCENE_H
