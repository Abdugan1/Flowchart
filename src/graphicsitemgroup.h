#ifndef GRAPHICSITEMGROUP_H
#define GRAPHICSITEMGROUP_H

#include <QGraphicsItemGroup>

#include "itemtypes.h"

class DiagramItem;

class GraphicsItemGroup : public QObject, public QGraphicsItemGroup
{
    Q_OBJECT

public:
    enum {Type = ItemTypes::GraphicsItemGroupType};

public:
    GraphicsItemGroup(const QPointF& pos, QGraphicsItem *parent = nullptr);
    QRectF boundingRect() const override;

    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option,
               QWidget *widget) override;

    int type() const override;

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
