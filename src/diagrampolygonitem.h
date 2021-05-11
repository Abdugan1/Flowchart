#ifndef DIAGRAMPOLYGONITEM_H
#define DIAGRAMPOLYGONITEM_H

#include "diagramitem.h"

#include <QGraphicsPolygonItem>

class DiagramPolygonItem : public DiagramItem
{
public:
    enum DiagramPolygonType {
        Process  = DiagramItem::Process,
        Desicion = DiagramItem::Desicion,
        InOut    = DiagramItem::InOut
    };
private:
    enum DefaultSize {
        Width  = 200,
        Height = 80
    };
public:
    DiagramPolygonItem(DiagramPolygonType diagramType, QGraphicsItem* parent = nullptr);
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
private:
    QPolygonF polygonF_;
};

#endif // DIAGRAMPOLYGONITEM_H
