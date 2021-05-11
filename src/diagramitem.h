#ifndef DIAGRAMITEM_H
#define DIAGRAMITEM_H

#include <QAbstractGraphicsShapeItem>

class DiagramItem : public QAbstractGraphicsShapeItem
{
public:
    enum DiagramType {
        Terminal,
        Process,
        Desicion,
        InOut
    };
public:
    DiagramItem(DiagramType diagramType, QGraphicsItem* parent = nullptr);
    DiagramType diagramType() const;
protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;
private:
    DiagramType diagramType_;
};

#endif // DIAGRAMITEM_H
