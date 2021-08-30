#ifndef GRAPHICSITEMGROUP_H
#define GRAPHICSITEMGROUP_H

#include <QGraphicsItemGroup>

class DiagramItem;

class GraphicsItemGroup : public QObject, public QGraphicsItemGroup
{
    Q_OBJECT

public:
    GraphicsItemGroup(const QPointF& pos, QGraphicsItem *parent = nullptr);
    QRectF boundingRect() const override;

    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option,
               QWidget *widget) override;

    void addDiagramItem(DiagramItem* item);
    const QList<DiagramItem *> &diagramItems() const;

signals:
    void lostSelection();

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;

    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

private:
    QPointF calculateBottomRight(QPointF topLeft) const;

private:
    QList<DiagramItem*> diagramItems_;
};

#endif // GRAPHICSITEMGROUP_H
