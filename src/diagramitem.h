#ifndef DIAGRAMITEM_H
#define DIAGRAMITEM_H

#include "diagramtextitem.h"

#include <QGraphicsPathItem>

class DiagramItem : public QObject, public QGraphicsPathItem
{
    Q_OBJECT
public:
    enum DiagramType {
        Terminal,
        Process,
        Desicion,
        InOut,
        ForLoop,
    };

private:
    enum DefaultSize {
        Width  = 200,
        Height = 80
    };
    enum {
        DefaultPenWidth  = 1,
        SelectedPenWidth = 2
    };

public:
    DiagramItem(DiagramType diagramType, QGraphicsItem* parent = nullptr);
    DiagramType diagramType() const;
    QPixmap image() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    QRectF boundingRect() const override;
    QRectF pathBoundingRect() const;

signals:
    void itemPositionChanged(const QPointF& pos);
    void itemReleased();

public slots:
    void updateTextItemPosition();

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;

private:
    DiagramType  diagramType_;
    QPainterPath painterPath_;

    DiagramTextItem* textItem_;
};

#endif // DIAGRAMITEM_H
