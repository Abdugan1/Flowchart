#ifndef DIAGRAMSCENE_H
#define DIAGRAMSCENE_H

#include <QGraphicsScene>

class DiagramItem;

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
    QPointF preventOutsideMove(QPointF pos, QGraphicsItem* item);

public slots:
    void onItemPositionChanged(const QPointF& pos);
    void onItemReleased();
    void selectAllItems();

protected:
    void drawBackground(QPainter *painter, const QRectF &rect) override;

private:
    QList<DiagramItem*> getAllDiagramItems();
    void deleteAllLines();
    void drawLevelLine(const QLineF& line);
    QPoint getItemCenter(const DiagramItem* item);
};

#endif // DIAGRAMSCENE_H
