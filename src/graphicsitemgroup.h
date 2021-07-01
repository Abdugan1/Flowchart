#ifndef GRAPHICSITEMGROUP_H
#define GRAPHICSITEMGROUP_H

#include <QGraphicsItemGroup>

class GraphicsItemGroup : public QObject, public QGraphicsItemGroup
{
    Q_OBJECT

public:
    GraphicsItemGroup(QGraphicsItem *parent = nullptr);
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

signals:
    void lostSelection();

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;
};

#endif // GRAPHICSITEMGROUP_H
