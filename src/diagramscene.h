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

public slots:
    void onItemPositionChanging(const QPointF& oldPos, const QPointF& newPos);
    void onItemReleased();

protected:
    void drawBackground(QPainter *painter, const QRectF &rect) override;

private:
    void deleteAllLines(const QPoint& point);
    void drawGreenDashLine(QLineF line);
    QPoint getItemCenter(const DiagramItem* item);
};

#endif // DIAGRAMSCENE_H
