#ifndef DIAGRAMITEM_H
#define DIAGRAMITEM_H

#include "diagramtextitem.h"

#include <QGraphicsPolygonItem>

class DiagramItem : public QObject, public QGraphicsPolygonItem
{
    Q_OBJECT
public:
    enum DiagramType {
        Terminal,
        Process,
        Desicion,
        InOut
    };

private:
    enum DefaultSize {
        Width  = 200,
        Height = 80
    };

public:
    DiagramItem(DiagramType diagramType, QGraphicsItem* parent = nullptr);
    DiagramType diagramType() const;
    QPixmap image() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

signals:
    void itemPositionChanging(const QPointF& oldPos, const QPointF& newPos);
    void itemReleased();

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

private:
    QPointF preventOutsideMove(QPointF pos);
    QPointF moveByGrid(QPointF pos);

private:
    DiagramType diagramType_;
    QPolygonF   polygon_;

    DiagramTextItem* textItem_;
};

#endif // DIAGRAMITEM_H
