#ifndef DIAGRAMITEM_H
#define DIAGRAMITEM_H

#include "diagramtextitem.h"

#include <QGraphicsPolygonItem>

class DiagramItem : public QGraphicsPolygonItem
{
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
protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;

private:
    QPointF preventOutsideMove(QPointF pos);

private:
    DiagramType diagramType_;
    QPolygonF   polygon_;

    DiagramTextItem* textItem_;
};

#endif // DIAGRAMITEM_H
